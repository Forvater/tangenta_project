const short vdx[12] = {     0,  3792,  3795,     0, -3792, -3792, -2387,  1992,
                         4377,  2390, -1990, -4370};

const short vdy[12] = {  4377,  2182, -2197, -4382, -2197,  2187,  3675,  3887,
                          222, -3672, -3902,  -245};

const short ftx[216] = {  351,   480,   605,   730,   817,   905,  1016,  1127,
                         1277,  1427,  1544,  1662,  1718,  1775,  2046,  2317,
                         2422,  2527,  2550,  2572,  2665,  2757,  2931,  3105,
                         3193,  3282,  3267,  3252,  3328,  3405,  3575,  3745,
                         3820,  3895,  3897,  3900,  4066,  4232,  4278,  4325,
                         4248,  4172,  4203,  4235,  4386,  4537,  4563,  4590,
                         4470,  4350,  4493,  4637,  4640,  4642,  4498,  4355,
                         4346,  4337,  4441,  4545,  4516,  4487,  4328,  4170,
                         4136,  4102,  4171,  4240,  4191,  4142,  3911,  3680,
                         3717,  3755,  3680,  3605,  3430,  3255,  3176,  3097,
                         3101,  3105,  3018,  2932,  2756,  2580,  2447,  2315,
                         2208,  2102,  1941,  1780,  1672,  1565,  1493,  1422,
                         1310,  1197,  1050,   902,   796,   690,   583,   477,
                          361,   245,     3,  -237,  -367,  -497,  -615,  -732,
                         -825,  -917, -1030, -1142, -1290, -1437, -1557, -1677,
                        -1732, -1787, -2058, -2330, -2431, -2532, -2555, -2577,
                        -2661, -2745, -2926, -3107, -3193, -3280, -3271, -3262,
                        -3332, -3402, -3577, -3752, -3825, -3897, -3825, -3752,
                        -3997, -4242, -4288, -4335, -4251, -4167, -4200, -4232,
                        -4387, -4542, -4562, -4582, -4470, -4357, -4498, -4640,
                        -4640, -4640, -4496, -4352, -4341, -4330, -4436, -4542,
                        -4522, -4502, -4335, -4167, -4130, -4092, -4165, -4237,
                        -4182, -4127, -3906, -3685, -3718, -3752, -3681, -3610,
                        -3433, -3257, -3182, -3107, -3110, -3112, -3021, -2930,
                        -2753, -2577, -2451, -2325, -2217, -2110, -1946, -1782,
                        -1676, -1570, -1503, -1437, -1318, -1200, -1058,  -917,
                         -805,  -692,  -592,  -492,  -368,  -245,   -11,   222};

const short fty[216] = { 4490,  4612,  4596,  4580,  4430,  4280,  4248,  4217,
                         4315,  4412,  4363,  4315,  4160,  4005,  4010,  4015,
                         3940,  3865,  3703,  3542,  3465,  3387,  3418,  3450,
                         3357,  3265,  3096,  2927,  2837,  2747,  2736,  2725,
                         2623,  2522,  2248,  1975,  1927,  1880,  1766,  1652,
                         1501,  1350,  1238,  1127,  1045,   962,   837,   712,
                          580,   447,   220,    -7,  -128,  -250,  -357,  -465,
                         -578,  -692,  -831,  -970, -1087, -1205, -1280, -1355,
                        -1462, -1570, -1725, -1880, -1998, -2117, -2258, -2400,
                        -2566, -2732, -2827, -2922, -2931, -2940, -3023, -3107,
                        -3277, -3447, -3533, -3620, -3582, -3545, -3778, -4012,
                        -4085, -4157, -4082, -4007, -4053, -4100, -4257, -4415,
                        -4452, -4490, -4386, -4282, -4386, -4490, -4553, -4617,
                        -4632, -4647, -4515, -4382, -4500, -4617, -4597, -4577,
                        -4430, -4282, -4257, -4232, -4323, -4415, -4377, -4340,
                        -4170, -4000, -4008, -4017, -3955, -3892, -3722, -3552,
                        -3407, -3407, -3430, -3452, -3366, -3280, -3107, -2935,
                        -2848, -2762, -2742, -2722, -2629, -2537, -2630, -2722,
                        -2303, -1885, -1776, -1667, -1511, -1355, -1243, -1132,
                        -1048,  -965,  -852,  -740,  -601,  -462,  -235,    -7,
                          110,   227,   343,   460,   568,   677,   810,   942,
                         1066,  1190,  1262,  1335,  1450,  1565,  1726,  1887,
                         1991,  2095,  2242,  2390,  2551,  2712,  2813,  2915,
                         2917,  2920,  3003,  3087,  3257,  3427,  3513,  3600,
                         3568,  3537,  3768,  4000,  4058,  4117,  4048,  3980,
                         4028,  4077,  4236,  4395,  4432,  4470,  4372,  4275,
                         4300,  4325,  4463,  4602,  4617,  4632,  4500,  4367};
