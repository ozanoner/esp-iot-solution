idf_component_register(SRCS "ina219.c"
                        INCLUDE_DIRS "include"
                        PRIV_INCLUDE_DIRS "priv_include"
                        REQUIRES i2c_bus)
