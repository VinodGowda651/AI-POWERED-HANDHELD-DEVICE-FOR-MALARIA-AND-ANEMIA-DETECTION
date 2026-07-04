/* Edge Impulse + ESP32-CAM + OLED Display (STABLE INTEGRATED VERSION) */

/* Includes ---------------------------------------------------------------- */
#include <Malaria_Detection__inferencing.h>
#include "edge-impulse-sdk/dsp/image/image.hpp"
#include "esp_camera.h"

// OLED Library
#include <Wire.h>
#include <U8g2lib.h>

/* OLED Configuration (Updated to Software I2C) --------------------------- */
#define SDA_PIN 14
#define SCL_PIN 15
U8G2_SH1106_128X64_NONAME_F_SW_I2C display(U8G2_R0, SCL_PIN, SDA_PIN);

/* Camera Model Selection -------------------------------------------------- */
#define CAMERA_MODEL_AI_THINKER

#if defined(CAMERA_MODEL_AI_THINKER)
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22
#endif

/* Constants --------------------------------------------------------------- */
#define EI_CAMERA_RAW_FRAME_BUFFER_COLS 320
#define EI_CAMERA_RAW_FRAME_BUFFER_ROWS 240
#define EI_CAMERA_FRAME_BYTE_SIZE 3

/* Variables --------------------------------------------------------------- */
static bool debug_nn = false;
static bool is_initialised = false;
uint8_t *snapshot_buf;

String last_label = "";
float last_conf = 0;

const float CONF_THRESHOLD = 0.50;
const float CHANGE_THRESHOLD = 0.10;

/* Camera Config ----------------------------------------------------------- */
static camera_config_t camera_config = {
    .pin_pwdn = PWDN_GPIO_NUM,
    .pin_reset = RESET_GPIO_NUM,
    .pin_xclk = XCLK_GPIO_NUM,
    .pin_sscb_sda = SIOD_GPIO_NUM,
    .pin_sscb_scl = SIOC_GPIO_NUM,
    .pin_d7 = Y9_GPIO_NUM,
    .pin_d6 = Y8_GPIO_NUM,
    .pin_d5 = Y7_GPIO_NUM,
    .pin_d4 = Y6_GPIO_NUM,
    .pin_d3 = Y5_GPIO_NUM,
    .pin_d2 = Y4_GPIO_NUM,
    .pin_d1 = Y3_GPIO_NUM,
    .pin_d0 = Y2_GPIO_NUM,
    .pin_vsync = VSYNC_GPIO_NUM,
    .pin_href = HREF_GPIO_NUM,
    .pin_pclk = PCLK_GPIO_NUM,
    .xclk_freq_hz = 20000000,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,
    .pixel_format = PIXFORMAT_JPEG,
    .frame_size = FRAMESIZE_QVGA,
    .jpeg_quality = 12,
    .fb_count = 1,
    .fb_location = CAMERA_FB_IN_PSRAM,
    .grab_mode = CAMERA_GRAB_WHEN_EMPTY,
};

/* Function Declarations --------------------------------------------------- */
bool ei_camera_init(void);
bool ei_camera_capture(uint32_t img_width, uint32_t img_height, uint8_t *out_buf);
static int ei_camera_get_data(size_t offset, size_t length, float *out_ptr);

/* Setup ------------------------------------------------------------------- */
void setup()
{
    Serial.begin(115200);
    while (!Serial);

    Serial.println("Malaria Detection System");

    display.begin();
    display.clearBuffer();
    display.setFont(u8g2_font_ncenB08_tr);
    display.drawStr(0, 12, "Malaria Detector");
    display.drawStr(0, 30, "Init Camera...");
    display.sendBuffer();

    if (!ei_camera_init()) {
        Serial.println("Camera init failed!");
        display.clearBuffer();
        display.drawStr(0, 20, "Camera Error!");
        display.sendBuffer();
        while (1);
    }

    display.clearBuffer();
    display.drawStr(0, 20, "System Ready...");
    display.sendBuffer();
    delay(1000);
}

/* Loop -------------------------------------------------------------------- */
void loop()
{
    if (ei_sleep(5) != EI_IMPULSE_OK) return;

    snapshot_buf = (uint8_t*)malloc(EI_CAMERA_RAW_FRAME_BUFFER_COLS *
                                   EI_CAMERA_RAW_FRAME_BUFFER_ROWS *
                                   EI_CAMERA_FRAME_BYTE_SIZE);

    if (!snapshot_buf) {
        Serial.println("Memory alloc failed");
        return;
    }

    ei::signal_t signal;
    signal.total_length = EI_CLASSIFIER_INPUT_WIDTH * EI_CLASSIFIER_INPUT_HEIGHT;
    signal.get_data = &ei_camera_get_data;

    if (!ei_camera_capture(EI_CLASSIFIER_INPUT_WIDTH,
                           EI_CLASSIFIER_INPUT_HEIGHT,
                           snapshot_buf)) {
        Serial.println("Capture failed");
        free(snapshot_buf);
        return;
    }

    ei_impulse_result_t result = {0};
    if (run_classifier(&signal, &result, debug_nn) != EI_IMPULSE_OK) {
        Serial.println("Classifier failed");
        free(snapshot_buf);
        return;
    }

    float max_val = 0;
    String raw_label = "";

    for (uint16_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
        float val = result.classification[i].value;
        if (val > max_val) {
            max_val = val;
            raw_label = String(ei_classifier_inferencing_categories[i]);
        }
    }

    if (max_val < CONF_THRESHOLD) {
        free(snapshot_buf);
        delay(200);
        return;
    }

    /* ✅ MODIFIED LABEL MAPPING ONLY */
   String display_label = "";

// Convert to lowercase for safe comparison
raw_label.toLowerCase();

if (raw_label.indexOf("infect") >= 0 || raw_label.indexOf("parasite") >= 0) {
    display_label = "Malaria Positive";
} 
else if (raw_label.indexOf("uninfect") >= 0 || raw_label.indexOf("healthy") >= 0) {
    display_label = "Malaria Negative";
} s
else {
    display_label = raw_label;  // fallback
}


    if (display_label != last_label || abs(max_val - last_conf) > CHANGE_THRESHOLD) {

        last_label = display_label;
        last_conf = max_val;

        Serial.printf("\nResult: %s (%.2f%%)\n", display_label.c_str(), max_val * 100);

        display.clearBuffer();
        display.setFont(u8g2_font_ncenB14_tr);
        display.drawStr(0, 25, display_label.c_str());

        char conf_str[20];
        sprintf(conf_str, "Conf: %.1f%%", max_val * 100);
        display.setFont(u8g2_font_6x12_tr);
        display.drawStr(0, 55, conf_str);
        display.sendBuffer();
    }

    free(snapshot_buf);
    delay(500);
}

/* Camera Functions -------------------------------------------------------- */
bool ei_camera_init(void) {
    if (is_initialised) return true;
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK) {
        Serial.printf("Camera error: 0x%x\n", err);
        return false;
    }
    is_initialised = true;
    return true;
}

bool ei_camera_capture(uint32_t img_width, uint32_t img_height, uint8_t *out_buf) {
    if (!is_initialised) return false;
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) return false;

    bool converted = fmt2rgb888(fb->buf, fb->len, PIXFORMAT_JPEG, out_buf);
    esp_camera_fb_return(fb);
    if (!converted) return false;

    if (img_width != EI_CAMERA_RAW_FRAME_BUFFER_COLS ||
        img_height != EI_CAMERA_RAW_FRAME_BUFFER_ROWS) {
        ei::image::processing::crop_and_interpolate_rgb888(
            out_buf, EI_CAMERA_RAW_FRAME_BUFFER_COLS, EI_CAMERA_RAW_FRAME_BUFFER_ROWS,
            out_buf, img_width, img_height);
    }
    return true;
}

static int ei_camera_get_data(size_t offset, size_t length, float *out_ptr) {
    size_t pixel_ix = offset * 3;
    for (size_t i = 0; i < length; i++) {
        out_ptr[i] = (snapshot_buf[pixel_ix + 2] << 16) |
                     (snapshot_buf[pixel_ix + 1] << 8) |
                     snapshot_buf[pixel_ix];
        pixel_ix += 3;
    }
    return 0;
}

#if !defined(EI_CLASSIFIER_SENSOR) || EI_CLASSIFIER_SENSOR != EI_CLASSIFIER_SENSOR_CAMERA
#error "Invalid model for current sensor"
#endif