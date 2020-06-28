vlib modelsim_lib/work
vlib modelsim_lib/msim

vlib modelsim_lib/msim/xil_defaultlib

vmap xil_defaultlib modelsim_lib/msim/xil_defaultlib

vcom -work xil_defaultlib -64 -93 \
"../../../bd/seq_detector/ip/seq_detector_next_state_logic_0_0/sim/seq_detector_next_state_logic_0_0.vhd" \
"../../../bd/seq_detector/ip/seq_detector_output_logic_0_0/sim/seq_detector_output_logic_0_0.vhd" \
"../../../bd/seq_detector/ipshared/9628/dff.vhd" \
"../../../bd/seq_detector/ip/seq_detector_dff_0_0/sim/seq_detector_dff_0_0.vhd" \
"../../../bd/seq_detector/ip/seq_detector_dff_0_1/sim/seq_detector_dff_0_1.vhd" \
"../../../bd/seq_detector/ip/seq_detector_dff_0_2/sim/seq_detector_dff_0_2.vhd" \
"../../../bd/seq_detector/ipshared/c2d1/debounce.vhd" \
"../../../bd/seq_detector/ip/seq_detector_debounce_0_0/sim/seq_detector_debounce_0_0.vhd" \
"../../../bd/seq_detector/sim/seq_detector.vhd" \


