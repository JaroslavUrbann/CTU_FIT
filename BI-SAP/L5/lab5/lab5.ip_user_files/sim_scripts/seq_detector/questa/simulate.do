onbreak {quit -f}
onerror {quit -f}

vsim -t 1ps -lib xil_defaultlib seq_detector_opt

do {wave.do}

view wave
view structure
view signals

do {seq_detector.udo}

run -all

quit -force
