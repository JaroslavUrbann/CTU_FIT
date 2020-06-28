onbreak {quit -f}
onerror {quit -f}

vsim -voptargs="+acc" -t 1ps -L xil_defaultlib -L secureip -lib xil_defaultlib xil_defaultlib.seq_detector

do {wave.do}

view wave
view structure
view signals

do {seq_detector.udo}

run -all

quit -force
