onbreak {quit -force}
onerror {quit -force}

asim -t 1ps +access +r +m+seq_detector -L xil_defaultlib -L secureip -O5 xil_defaultlib.seq_detector

do {wave.do}

view wave
view structure

do {seq_detector.udo}

run -all

endsim

quit -force
