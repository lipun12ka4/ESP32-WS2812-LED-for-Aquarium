set(requires "")
idf_build_get_property(target IDF_TARGET)

if(${target} STREQUAL "linux")
    list(APPEND requires esp_stubs esp-tls esp_http_server protocol_examples_common nvs_flash)
endif()

idf_component_register(SRCS "main.c"
							"UART0_Communication_Functions.c"
							"Parse_Commands.c"
							"LED_Strip_Functions.c"
							"HTTP_Server.c"
							"native_ota_example.c"
                    INCLUDE_DIRS "")
