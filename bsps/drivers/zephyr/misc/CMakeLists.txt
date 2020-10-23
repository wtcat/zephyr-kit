# Misc driver

zephyr_sources_ifdef(CONFIG_LCD_APOLLO3P rm6903_lcd.c)
zephyr_sources_ifdef(CONFIG_TP_APOLLO3P ite7259_tp.c)

