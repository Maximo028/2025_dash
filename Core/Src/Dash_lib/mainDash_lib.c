#include "Dash_lib/mainDash_lib.h"

#include <stdint.h>

volatile uint8_t dma_done = 0;
static uint8_t dma_buffer[ILI9488_WIDTH * PIXEL_SIZE * BLOCK_ROWS];

//DMA - Callback
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
    if (hspi == &hspi1) {
        dma_done = 1;
    }
}
// Private function to send command
static void ILI9488_SendCommand(uint8_t cmd) {
    HAL_GPIO_WritePin(ILI9488_DC_GPIO_Port, ILI9488_DC_Pin, GPIO_PIN_RESET); // DC = 0
    HAL_GPIO_WritePin(ILI9488_CS_GPIO_Port, ILI9488_CS_Pin, GPIO_PIN_RESET); // CS = 0
    HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(ILI9488_CS_GPIO_Port, ILI9488_CS_Pin, GPIO_PIN_SET);   // CS = 1
}

// Private function to send data
static void ILI9488_SendData(uint8_t data) {
    HAL_GPIO_WritePin(ILI9488_DC_GPIO_Port, ILI9488_DC_Pin, GPIO_PIN_SET);   // DC = 1
    HAL_GPIO_WritePin(ILI9488_CS_GPIO_Port, ILI9488_CS_Pin, GPIO_PIN_RESET); // CS = 0
    HAL_SPI_Transmit(&hspi1, &data, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(ILI9488_CS_GPIO_Port, ILI9488_CS_Pin, GPIO_PIN_SET);   // CS = 1
}

// Functin to execute SW reset (NSS - SW mode)
static void ILI9488_Reset(void) {
    HAL_GPIO_WritePin(ILI9488_RST_GPIO_Port, ILI9488_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(20);
    HAL_GPIO_WritePin(ILI9488_RST_GPIO_Port, ILI9488_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(150);
}

static void ILI9488_Color(uint16_t color){
	uint8_t r = ((color >> 11) & 0x1F) << 1;
	uint8_t g = ((color >> 5) & 0x3F);
	uint8_t b = ((color & 0x1F) << 1);

	r = r << 2;
	g = g << 2;
	b = b << 2;
	uint8_t data[3] = {b, g, r};
}

// Init for the dashboard
void ILI9488_Init(void) {
	 ILI9488_Reset();
	    HAL_Delay(10);

	    ILI9488_SendCommand(0x11); // Sleep out
	    HAL_Delay(120);

	    ILI9488_SendCommand(0x3A); // Pixel format
	    ILI9488_SendData(0x66);    // 18 bits per pixel (RGB666)

	    ILI9488_SendCommand(0x36); // MADCTL
	    ILI9488_SendData(0x28);    // Landscape + BGR

	    ILI9488_SendCommand(0x29); // Display on
	    HAL_Delay(20);
}

// Private function to set window
static void ILI9488_SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    ILI9488_SendCommand(ILI9488_CMD_COLUMN_ADDR);
    ILI9488_SendData(x0 >> 8);
    ILI9488_SendData(x0 & 0xFF);
    ILI9488_SendData(x1 >> 8);
    ILI9488_SendData(x1 & 0xFF);

    ILI9488_SendCommand(ILI9488_CMD_PAGE_ADDR);
    ILI9488_SendData(y0 >> 8);
    ILI9488_SendData(y0 & 0xFF);
    ILI9488_SendData(y1 >> 8);
    ILI9488_SendData(y1 & 0xFF);

    ILI9488_SendCommand(ILI9488_CMD_MEMORY_WRITE);
}

// Draw a single pixel (avoid the use, it is very very slow for big volum of points)
void ILI9488_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    // Convertir color de RGB565 → RGB666
    uint8_t r = ((color >> 11) & 0x1F) << 1; // 5 bits -> 6 bits
    uint8_t g = ((color >> 5) & 0x3F);       // 6 bits
    uint8_t b = ((color & 0x1F) << 1);        // 5 bits -> 6 bits

    // Expandimos 6 bits a 8 bits (rellenando a la izquierda)
    r = r << 2;
    g = g << 2;
    b = b << 2;

    // Seleccionar la ventana de 1x1 píxel
    ILI9488_SetWindow(x, y, x, y);

    // Activar modo de transmisión de datos
    HAL_GPIO_WritePin(ILI9488_DC_GPIO_Port, ILI9488_DC_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ILI9488_CS_GPIO_Port, ILI9488_CS_Pin, GPIO_PIN_RESET);

    // Mandar 3 bytes para 1 píxel (RGB666)
    uint8_t data[3] = {r, g, b};
    HAL_SPI_Transmit(&hspi1, data, 3, HAL_MAX_DELAY);

    HAL_GPIO_WritePin(ILI9488_CS_GPIO_Port, ILI9488_CS_Pin, GPIO_PIN_SET);
}

//Fill all the screen with a single color && DMA for a CPU better flow
void ILI9488_FillScreen_DMA(uint16_t color) {
    // Conversión correcta a RGB666
    uint8_t r = ((color >> 11) & 0x1F) << 1;
    uint8_t g = ((color >> 5) & 0x3F);
    uint8_t b = ((color & 0x1F) << 1);

    r = r << 2;
    g = g << 2;
    b = b << 2;

    // Rellenar buffer de bloque
    for (int i = 0; i < ILI9488_WIDTH * BLOCK_ROWS; i++) {
        dma_buffer[i * 3 + 0] = r;
        dma_buffer[i * 3 + 1] = g;
        dma_buffer[i * 3 + 2] = b;
    }

    // Enviar pantalla por bloques
    for (int y = 0; y < ILI9488_HEIGHT; y += BLOCK_ROWS) {
        ILI9488_SetWindow(0, y, ILI9488_WIDTH - 1, y + BLOCK_ROWS - 1);

        HAL_GPIO_WritePin(ILI9488_DC_GPIO_Port, ILI9488_DC_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(ILI9488_CS_GPIO_Port, ILI9488_CS_Pin, GPIO_PIN_RESET);

        dma_done = 0;
        HAL_SPI_Transmit_DMA(&hspi1, dma_buffer, sizeof(dma_buffer));
        while (!dma_done);

        HAL_GPIO_WritePin(ILI9488_CS_GPIO_Port, ILI9488_CS_Pin, GPIO_PIN_SET);
    }
}

static uint8_t rect_buffer[ILI9488_WIDTH * PIXEL_SIZE];
void ILI9488_Square(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color){
	    if (x1 < x0 || y1 < y0 || x1 >= ILI9488_WIDTH || y1 >= ILI9488_HEIGHT) return;

	    uint8_t r = ((color >> 11) & 0x1F) << 1; // 5 bits -> 6 bits
	    uint8_t g = ((color >> 5) & 0x3F);       // 6 bits
	    uint8_t b = ((color & 0x1F) << 1);       // 5 bits -> 6 bits

	    r <<= 2; g <<= 2; b <<= 2; // Expansión a 8 bits para ILI9488

	    uint16_t width = x1 - x0 + 1;
	    uint16_t height = y1 - y0 + 1;

	    // Preparar una línea del color deseado
	    for (int i = 0; i < width; i++) {
	        rect_buffer[i * 3 + 0] = r;
	        rect_buffer[i * 3 + 1] = g;
	        rect_buffer[i * 3 + 2] = b;
	    }

	    // Definir ventana del rectángulo
	    ILI9488_SetWindow(x0, y0, x1, y1);

	    // Empezar transmisión de líneas
	    HAL_GPIO_WritePin(ILI9488_DC_GPIO_Port, ILI9488_DC_Pin, GPIO_PIN_SET);
	    HAL_GPIO_WritePin(ILI9488_CS_GPIO_Port, ILI9488_CS_Pin, GPIO_PIN_RESET);

	    for (int y = 0; y < height; y++) {
	        HAL_SPI_Transmit(&hspi1, rect_buffer, width * 3, HAL_MAX_DELAY);
	    }

	    HAL_GPIO_WritePin(ILI9488_CS_GPIO_Port, ILI9488_CS_Pin, GPIO_PIN_SET);
}

void ILI9488_DrawBitmapRGB565(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, const uint16_t *bitmap) {
	 if (x0 + width > ILI9488_WIDTH || y0 + height > ILI9488_HEIGHT) return;

	    // Definir la ventana de dibujo
	    ILI9488_SetWindow(x0, y0, x0 + width - 1, y0 + height - 1);

	    // Preparar para mandar datos
	    HAL_GPIO_WritePin(ILI9488_DC_GPIO_Port, ILI9488_DC_Pin, GPIO_PIN_SET); // DC = 1 -> Datos
	    HAL_GPIO_WritePin(ILI9488_CS_GPIO_Port, ILI9488_CS_Pin, GPIO_PIN_RESET); // CS = 0 -> Activo

	    uint8_t pixel_buffer[3];

	    // Para cada píxel
	    for (uint32_t i = 0; i < width * height; i++) {
	        uint16_t color = bitmap[i];

	        // Extraer componentes
	        uint8_t r = ((color >> 11) & 0x1F) << 1; // R5 -> R6
	        uint8_t g = ((color >> 5) & 0x3F);        // G6
	        uint8_t b = ((color & 0x1F) <<1);        // B5 -> B6

	        // Expandir a 8 bits
	        r <<= 2;
	        g <<= 2;
	        b <<= 2;

	        // Guardar en orden BGR (por tu pantalla)
	        pixel_buffer[0] = b;
	        pixel_buffer[1] = g;
	        pixel_buffer[2] = r;

	        // Mandar un pixel
	        HAL_SPI_Transmit(&hspi1, pixel_buffer, 3, HAL_MAX_DELAY);
	    }

	    // Finalizar transmisión
	    HAL_GPIO_WritePin(ILI9488_CS_GPIO_Port, ILI9488_CS_Pin, GPIO_PIN_SET);
}

void ILI9488_DrawChar(uint16_t x, uint16_t y, char c, FontDef font, uint16_t color){





	uint32_t offset = (c - 32) * font.height * font.bytes;
	    const uint8_t *bitmap = &font.table[offset];

	    for (uint8_t row = 0; row < font.height; row++) {
	        uint64_t line = 0;

	        if (font.height == 8) {
	        	line = bitmap[row];
				line = line >> (8 - font.width);
	        } else if (font.height == 16) {
	        	line = (bitmap[row * font.bytes] << 8) | bitmap[row * font.bytes + 1];
				line = line >> (16 - font.width);
	        } else if (font.height == 24) {
	            line = (bitmap[row * font.bytes] << 16) | (bitmap[row * font.bytes + 1] << 8) | bitmap[row * font.bytes + 2];
			    line = line >> (24 - font.width);
	        } else if (font.height == 36) {
	        	line = (bitmap[row * font.bytes] << 24) | (bitmap[row * font.bytes + 1 ] << 16) | (bitmap[row * font.bytes + 2] << 8) | (bitmap[row * font.bytes + 3]);
	        	line = line >> (32 - font.width);
	        }
	        line &= (1 << font.width) - 1;

	        for (uint8_t col = 0; col < font.width; col++) {
	        	if (line & (1 << (font.width - 1 - col))) {
	                ILI9488_DrawPixel(x + col, y + row, color);
	            }
	        }
	    }

}

void ILI9488_DrawString(uint16_t x, uint16_t y, const char *str, FontDef font, uint16_t color){
	uint16_t x_start = x;
	while (*str) {
		if(*str == '\n'){
			y += font.height;
			x = x_start;
			str++;
			continue;
		}
		if((x + font.width) > ILI9488_WIDTH){
			y += font.height;
			x = x_start;
		}
		if((y + font.height) > ILI9488_HEIGHT){
			break;
		}
		ILI9488_DrawChar(x, y, *str, font, color);
		x += font.width;
		str++;
	    }
}
