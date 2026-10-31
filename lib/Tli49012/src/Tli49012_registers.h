#pragma once

// TLx49012 register address map.
// Source: document/infineon-tlx49012-usermanual-en.pdf,
//         Rev. 1.00, 2026-07-16:
//         8.2.1, pp. 49-50 (SPI); 8.3.1, pp. 118-119 (NVM).
// Checked against the individual register definitions in 8.2 and 8.3.
//
// These are logical register addresses, before SPI address/RW encoding.
// All constants are hexadecimal unsigned integers. Use a uint16_t (or wider)
// address when accessing NVM; uint8_t cannot hold addresses above 0x00FF.
// Only documented register addresses are listed; gaps remain reserved.
// Bit-field masks and reset values are not defined in this header.

// SPI: status and diagnostic registers
#define TLI49012_REG_STAT_DEVICE                    0x0000U // stat_device_reg, p. 51
#define TLI49012_REG_STAT_0                         0x0001U // stat_0_reg, p. 53
#define TLI49012_REG_STAT_EN_0                      0x0002U // stat_en_0_reg, p. 55
#define TLI49012_REG_STAT_1                         0x0003U // stat_1_reg, p. 57
#define TLI49012_REG_STAT_EN_1                      0x0004U // stat_en_1_reg, p. 60
#define TLI49012_REG_STAT_2                         0x0005U // stat_2_reg, p. 63
#define TLI49012_REG_STAT_EN_2                      0x0006U // stat_en_2_reg, p. 66

// SPI: angle, velocity, magnetic field and temperature
#define TLI49012_REG_ANGLE_SYNC                     0x000AU // angle_sync_reg, p. 70
#define TLI49012_REG_ANGLE_VEL_SYNC                 0x000BU // angle_vel_sync_reg, p. 71
#define TLI49012_REG_ANGLE_PRED                     0x000CU // angle_pred_reg, p. 72
#define TLI49012_REG_ANGLE_SET                      0x000DU // angle_set_reg, p. 73
#define TLI49012_REG_ANGLE_VEL                      0x000EU // angle_vel_reg, p. 74
#define TLI49012_REG_ANGLE                          0x000FU // angle_reg, p. 75
#define TLI49012_REG_ANGLE_RAW                      0x0010U // angle_raw_reg, p. 76
#define TLI49012_REG_VEC_LEN                        0x0011U // vec_len_reg, p. 77
#define TLI49012_REG_XN                             0x0012U // xn_reg, p. 78
#define TLI49012_REG_YN                             0x0013U // yn_reg, p. 79
#define TLI49012_REG_ANGLE3                         0x0014U // angle3_reg, p. 80
#define TLI49012_REG_VEC_LEN3                       0x0015U // vec_len3_reg, p. 81
#define TLI49012_REG_X3                             0x0016U // x3_reg, p. 82
#define TLI49012_REG_Y3                             0x0017U // y3_reg, p. 83
#define TLI49012_REG_T_LIN                          0x0025U // t_lin_reg, p. 84

// SPI: system self-calibration results
#define TLI49012_REG_SYSCAL_ANGLE_BASE              0x002BU // syscal_angle_base_reg, p. 85
#define TLI49012_REG_SYSCAL_OFFSET_X                0x002CU // syscal_offset_x_reg, p. 86
#define TLI49012_REG_SYSCAL_OFFSET_Y                0x002DU // syscal_offset_y_reg, p. 87
#define TLI49012_REG_SYSCAL_ELLIPTICITY             0x002EU // syscal_ellipticity_reg, p. 88
#define TLI49012_REG_SYSCAL_OOS_ROTATION            0x002FU // syscal_oos_rotation_reg, p. 89

// SPI: multi-point calibration LUT access
#define TLI49012_REG_MPC_LUT_IDX                    0x0038U // mpc_lut_idx_reg, p. 90
#define TLI49012_REG_MPC_LUT_DATA_0                 0x003AU // mpc_lut_data_0_reg, p. 91
#define TLI49012_REG_MPC_LUT_DATA_1                 0x003BU // mpc_lut_data_1_reg, p. 92
#define TLI49012_REG_MPC_LUT_DATA_2                 0x003CU // mpc_lut_data_2_reg, p. 93
#define TLI49012_REG_MPC_LUT_DATA_3                 0x003DU // mpc_lut_data_3_reg, p. 94

// SPI: user configuration registers
#define TLI49012_REG_USR_CONFIG_0                   0x003EU // usr_config_0_reg, p. 95
#define TLI49012_REG_USR_CONFIG_1                   0x003FU // usr_config_1_reg, p. 96
#define TLI49012_REG_USR_CONFIG_2                   0x0040U // usr_config_2_reg, p. 100
#define TLI49012_REG_USR_CONFIG_3                   0x0041U // usr_config_3_reg, p. 101
#define TLI49012_REG_USR_CONFIG_4                   0x0042U // usr_config_4_reg, p. 103
#define TLI49012_REG_USR_CONFIG_5                   0x0043U // usr_config_5_reg, p. 104
#define TLI49012_REG_USR_CONFIG_6                   0x0044U // usr_config_6_reg, p. 105
#define TLI49012_REG_USR_CONFIG_7                   0x0045U // usr_config_7_reg, p. 106
#define TLI49012_REG_USR_CONFIG_8                   0x0046U // usr_config_8_reg, p. 107
#define TLI49012_REG_USR_CONFIG_9                   0x0047U // usr_config_9_reg, p. 108
#define TLI49012_REG_USR_CONFIG_10                  0x0048U // usr_config_10_reg, p. 109
#define TLI49012_REG_USR_CONFIG_11                  0x0049U // usr_config_11_reg, p. 110
#define TLI49012_REG_USR_CONFIG_12                  0x004AU // usr_config_12_reg, p. 111
#define TLI49012_REG_USR_CONFIG_13                  0x004BU // usr_config_13_reg, p. 112
#define TLI49012_REG_USR_CONFIG_14                  0x004CU // usr_config_14_reg, p. 113
#define TLI49012_REG_USR_CONFIG_15                  0x004DU // usr_config_15_reg, p. 114

// SPI: clock monitoring, unlock and identification
#define TLI49012_REG_SM_CLK_CNT                     0x0068U // sm_clk_cnt_reg, p. 115
#define TLI49012_REG_REG_UNLOCK                     0x0078U // reg_unlock_reg, p. 116
#define TLI49012_REG_HW_FW_ID                       0x0079U // hw_fw_id_reg, p. 117
#define TLI49012_EXPECTED_HW_FW_ID                  0x0023U // expected value for hw_fw_id_reg, p. 117

// NVM: control and status (next-frame access)
#define TLI49012_REG_CMTP_CTRL                      0x00FDU // cmtp_ctrl_reg, p. 120
#define TLI49012_REG_CMTP_STAT                      0x00FEU // cmtp_stat_reg, p. 121
#define TLI49012_REG_CMTP_STAT_2                    0x00FFU // cmtp_stat_2_reg, p. 123

// NVM: LUT words 0-31 (next-frame access)
#define TLI49012_REG_CMTP_WORD_0                    0x0100U // cmtp_word_0_reg, p. 124
#define TLI49012_REG_CMTP_WORD_1                    0x0101U // cmtp_word_1_reg, p. 125
#define TLI49012_REG_CMTP_WORD_2                    0x0102U // cmtp_word_2_reg, p. 126
#define TLI49012_REG_CMTP_WORD_3                    0x0103U // cmtp_word_3_reg, p. 127
#define TLI49012_REG_CMTP_WORD_4                    0x0104U // cmtp_word_4_reg, p. 128
#define TLI49012_REG_CMTP_WORD_5                    0x0105U // cmtp_word_5_reg, p. 129
#define TLI49012_REG_CMTP_WORD_6                    0x0106U // cmtp_word_6_reg, p. 130
#define TLI49012_REG_CMTP_WORD_7                    0x0107U // cmtp_word_7_reg, p. 131
#define TLI49012_REG_CMTP_WORD_8                    0x0108U // cmtp_word_8_reg, p. 132
#define TLI49012_REG_CMTP_WORD_9                    0x0109U // cmtp_word_9_reg, p. 133
#define TLI49012_REG_CMTP_WORD_10                   0x010AU // cmtp_word_10_reg, p. 134
#define TLI49012_REG_CMTP_WORD_11                   0x010BU // cmtp_word_11_reg, p. 135
#define TLI49012_REG_CMTP_WORD_12                   0x010CU // cmtp_word_12_reg, p. 136
#define TLI49012_REG_CMTP_WORD_13                   0x010DU // cmtp_word_13_reg, p. 137
#define TLI49012_REG_CMTP_WORD_14                   0x010EU // cmtp_word_14_reg, p. 138
#define TLI49012_REG_CMTP_WORD_15                   0x010FU // cmtp_word_15_reg, p. 139
#define TLI49012_REG_CMTP_WORD_16                   0x0110U // cmtp_word_16_reg, p. 140
#define TLI49012_REG_CMTP_WORD_17                   0x0111U // cmtp_word_17_reg, p. 141
#define TLI49012_REG_CMTP_WORD_18                   0x0112U // cmtp_word_18_reg, p. 142
#define TLI49012_REG_CMTP_WORD_19                   0x0113U // cmtp_word_19_reg, p. 143
#define TLI49012_REG_CMTP_WORD_20                   0x0114U // cmtp_word_20_reg, p. 144
#define TLI49012_REG_CMTP_WORD_21                   0x0115U // cmtp_word_21_reg, p. 145
#define TLI49012_REG_CMTP_WORD_22                   0x0116U // cmtp_word_22_reg, p. 146
#define TLI49012_REG_CMTP_WORD_23                   0x0117U // cmtp_word_23_reg, p. 147
#define TLI49012_REG_CMTP_WORD_24                   0x0118U // cmtp_word_24_reg, p. 148
#define TLI49012_REG_CMTP_WORD_25                   0x0119U // cmtp_word_25_reg, p. 149
#define TLI49012_REG_CMTP_WORD_26                   0x011AU // cmtp_word_26_reg, p. 150
#define TLI49012_REG_CMTP_WORD_27                   0x011BU // cmtp_word_27_reg, p. 151
#define TLI49012_REG_CMTP_WORD_28                   0x011CU // cmtp_word_28_reg, p. 152
#define TLI49012_REG_CMTP_WORD_29                   0x011DU // cmtp_word_29_reg, p. 153
#define TLI49012_REG_CMTP_WORD_30                   0x011EU // cmtp_word_30_reg, p. 154
#define TLI49012_REG_CMTP_WORD_31                   0x011FU // cmtp_word_31_reg, p. 155

// NVM: LUT CRC word 32 (next-frame access)
#define TLI49012_REG_CMTP_WORD_32                   0x0120U // cmtp_word_32_reg, p. 156

// NVM: configuration words 33-47 (next-frame access)
#define TLI49012_REG_CMTP_WORD_33                   0x0121U // cmtp_word_33_reg, p. 157
#define TLI49012_REG_CMTP_WORD_34                   0x0122U // cmtp_word_34_reg, p. 161
#define TLI49012_REG_CMTP_WORD_35                   0x0123U // cmtp_word_35_reg, p. 162
#define TLI49012_REG_CMTP_WORD_36                   0x0124U // cmtp_word_36_reg, p. 164
#define TLI49012_REG_CMTP_WORD_37                   0x0125U // cmtp_word_37_reg, p. 165
#define TLI49012_REG_CMTP_WORD_38                   0x0126U // cmtp_word_38_reg, p. 166
#define TLI49012_REG_CMTP_WORD_39                   0x0127U // cmtp_word_39_reg, p. 167
#define TLI49012_REG_CMTP_WORD_40                   0x0128U // cmtp_word_40_reg, p. 168
#define TLI49012_REG_CMTP_WORD_41                   0x0129U // cmtp_word_41_reg, p. 169
#define TLI49012_REG_CMTP_WORD_42                   0x012AU // cmtp_word_42_reg, p. 170
#define TLI49012_REG_CMTP_WORD_43                   0x012BU // cmtp_word_43_reg, p. 171
#define TLI49012_REG_CMTP_WORD_44                   0x012CU // cmtp_word_44_reg, p. 172
#define TLI49012_REG_CMTP_WORD_45                   0x012DU // cmtp_word_45_reg, p. 173
#define TLI49012_REG_CMTP_WORD_46                   0x012EU // cmtp_word_46_reg, p. 175
#define TLI49012_REG_CMTP_WORD_47                   0x012FU // cmtp_word_47_reg, p. 177