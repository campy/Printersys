/**
 * Marlin 3D Printer Firmware 1.1.8
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * About Marlin
 *
 * This firmware is a mashup between Sprinter and grbl.
 *  - https://github.com/kliment/Sprinter
 *  - https://github.com/simen/grbl/tree
 */

 /**
 * Modificaciones PRINTERSYS
 *
 * Printersys ha realizado algunas modificaciones sobre la versión original.
 * 1. La electronica seleccionada en el "Configuration.h" es propia de Printersys
 * 2. Se ha añadido al directorio de Marlin el archivo "pins_printersys.h" donde se definen los pines de la electronica diseñada por Printersys. Cuando se quiera usar doble extrusor revisar como gestiona la electornica MKS Gen el paso por la RAMPS porque lo que añade no esta puesto.
 * 3. El control del ventilador del radiador del extrusor (FAN2) se define dentro de "Configuration_adv.h" en el apartado "Extruder cooling fans" donde se han realizado las siguientes pruebas:
 *  3.1. Ventilador a 255/255: Temperaturas de 29ºC y 40ºC en la parte baja del radiador
 *  3.2. Ventilador a 200/255: Temperaturas de 29ºC y 40ºC en la parte baja del radiador  --> Esta es la opción seleccionada porque también en el FAN2 es donde conectamos el ventilador de refigeración de la electrónica.
 *  3.3. Ventilador a 150/255: Temperaturas de 30ºC y 41ºC en la parte baja del radiador
 * 4. Control de la temperatura de los drivers de motor MKS TMC2100
 *  4.1. Sin ventilador (000%): Driver del Extrusor a 76ºC; Driver del Motor X a 72ºC
 *  4.2. Con ventilador (100%): Driver del Extrusor a 46ºC; Driver del Motor X a 54ºC
 *  4.3. Sin ventilador (078%): Driver del Extrusor a 50ºC; Driver del Motor X a 56ºC
 * 5. En "Configuration_adv.h" se ha reducido el "BED_CHECK_INTERVAL" a 3s
 * 6. Control de las aceleraciones (https://www.prusaprinters.org/calculator/)
 *  Partiendo de estos datos:
 *    Aceleración Máxima:           (1200,1200,75,1200) 
 *    Aceleración por defecto:      750
 *    Aceleración de la retracción: 750
 *    Aceleración de viaje:         750
 *    --Si el movimiento requiere de la actiación de los 2 motores del Core XY podemos llegar a velocidades de 160 mm/s
 *    --Si el movimiento requiere solo de 1 motor del sistema Core XY podemos llegar solo a velocidades de 110 mm/s (con garantia de que no perdemos precisión)
 *  Partiendo de estos datos:
 *    Aceleración Máxima:           (3000,3000,75,1200) 
 *    Aceleración por defecto:      1400
 *    Aceleración de la retracción: 750
 *    Aceleración de viaje:         1400
 *    --Esto es todavia peor! ya que obligamos al motor a seguir una curva de aceleración que requiere mucha más fuerza y esto genera una perdia de precisión
 *  Partiendo de estos datos:
 *    Aceleración Máxima:           (1200,1200,75,1200) 
 *    Aceleración por defecto:      750
 *    Aceleración de la retracción: 750
 *    Aceleración de viaje:         500
 *    --Podemos alcanzar sin problemas movimientos de 250mm/s pero para tramos cortos no llegamos nunca a la velocidad de consigna
  */

#include "MarlinConfig.h"

#if ENABLED(ULTRA_LCD)
  #if ENABLED(LCD_I2C_TYPE_PCF8575)
    #include <Wire.h>
    #include <LiquidCrystal_I2C.h>
  #elif ENABLED(LCD_I2C_TYPE_MCP23017) || ENABLED(LCD_I2C_TYPE_MCP23008)
    #include <Wire.h>
    #include <LiquidTWI2.h>
  #elif ENABLED(LCM1602)
    #include <Wire.h>
    #include <LCD.h>
    #include <LiquidCrystal_I2C.h>
  #elif ENABLED(DOGLCD)
    #include <U8glib.h> // library for graphics LCD by Oli Kraus (https://github.com/olikraus/U8glib_Arduino)
  #else
    #include <LiquidCrystal.h> // library for character LCD
  #endif
#endif

#if HAS_DIGIPOTSS
  #include <SPI.h>
#endif

#if ENABLED(DIGIPOT_I2C)
  #include <Wire.h>
#endif

#if ENABLED(HAVE_TMCDRIVER)
  #include <SPI.h>
  #include <TMC26XStepper.h>
#endif

#if ENABLED(HAVE_TMC2130)
  #include <SPI.h>
  #include <TMC2130Stepper.h>
#endif

#if ENABLED(HAVE_L6470DRIVER)
  #include <SPI.h>
  #include <L6470.h>
#endif
