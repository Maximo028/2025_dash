/*

void ILI9341_Init(void) {
    ILI9488_Reset(); // Puedes usar la misma función de reset

    HAL_Delay(10);
    ILI9488_SendCommand(0x11); // Sleep out
    HAL_Delay(120);

    ILI9488_SendCommand(0x3A); // Pixel format
    ILI9488_SendData(0x55);    // 16 bits/pixel (RGB565)

    ILI9488_SendCommand(0x29); // Display ON
    HAL_Delay(20);
}

void ILI9341_FillScreen(uint16_t color) {
    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;

    static uint8_t line_buffer[480 * 2]; // 2 bytes por píxel

    for (int i = 0; i < 480; i++) {
        line_buffer[i * 2]     = hi;
        line_buffer[i * 2 + 1] = lo;
    }

    ILI9488_SetWindow(0, 0, 319, 479); // ILI9341 suele ser 320x240

    HAL_GPIO_WritePin(ILI9488_DC_GPIO_Port, ILI9488_DC_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ILI9488_CS_GPIO_Port, ILI9488_CS_Pin, GPIO_PIN_RESET);

    for (int y = 0; y < 320; y++) {
        HAL_SPI_Transmit(&hspi1, line_buffer, sizeof(line_buffer), HAL_MAX_DELAY);
    }

    HAL_GPIO_WritePin(ILI9488_CS_GPIO_Port, ILI9488_CS_Pin, GPIO_PIN_SET);
}

static uint8_t dma_buffer2[ILI9341_WIDTH * PIXEL_SIZE * BLOCK_ROWS];

void ILI9341_FillScreen_DMA(uint16_t color) {
    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;

    // Llenar el buffer
    for (int i = 0; i < ILI9341_WIDTH * BLOCK_ROWS; i++) {
        dma_buffer2[i * 2 + 0] = hi;
        dma_buffer2[i * 2 + 1] = lo;
    }

    // Definir la ventana de la pantalla completa
    ILI9488_SetWindow(0, 0, 319, 479);

    for (int y = 0; y < ILI9341_HEIGHT; y += BLOCK_ROWS) {
        HAL_GPIO_WritePin(ILI9488_DC_GPIO_Port, ILI9488_DC_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(ILI9488_CS_GPIO_Port, ILI9488_CS_Pin, GPIO_PIN_RESET);

        dma_done = 0;
        HAL_SPI_Transmit_DMA(&hspi1, dma_buffer2, sizeof(dma_buffer));
        while (!dma_done);

        HAL_GPIO_WritePin(ILI9488_CS_GPIO_Port, ILI9488_CS_Pin, GPIO_PIN_SET);
    }
}

*/
