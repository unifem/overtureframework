      subroutine duStepWaveGen2d6rc( 
     *   nd1a,nd1b,nd2a,nd2b,
     *   n1a,n1b,n2a,n2b,
     *   u,ut,unew,utnew,
     *   dx,dy,dt,cc,
     *   i,j,n )

      implicit none
c
c.. declarations of incoming variables      
      integer nd1a,nd1b,nd2a,nd2b
      integer n1a,n1b,n2a,n2b
      integer i,j,n

      real u    (nd1a:nd1b,nd2a:nd2b,*)
      real ut   (nd1a:nd1b,nd2a:nd2b,*)
      real unew (nd1a:nd1b,nd2a:nd2b)
      real utnew(nd1a:nd1b,nd2a:nd2b)
      real dx,dy,dt,cc
c
c.. generated code to follow
c
        real t1
        integer t10
        real t100
        real t1000
        real t1002
        real t1004
        real t1005
        real t1006
        real t1007
        real t1008
        real t1012
        real t1014
        real t1015
        real t1016
        real t1017
        real t1019
        real t102
        real t1020
        real t1022
        real t1023
        real t1025
        real t1026
        real t1028
        real t1029
        real t1030
        real t1032
        real t1033
        real t1035
        real t104
        real t1040
        real t1041
        real t1042
        real t1044
        real t1046
        real t1047
        real t1048
        real t1050
        real t1054
        real t1056
        real t1057
        real t106
        real t1062
        real t1063
        real t1064
        real t1066
        real t1067
        real t1068
        real t1069
        real t107
        real t1070
        real t1071
        real t1072
        real t1079
        real t108
        real t1080
        real t1082
        real t1085
        real t1086
        real t1087
        real t1088
        real t1089
        real t1091
        real t1092
        real t1094
        real t1095
        real t1097
        real t1098
        real t1099
        real t11
        real t110
        real t1100
        real t1102
        real t1103
        real t1105
        real t1106
        real t1107
        real t1109
        real t1111
        real t1113
        real t1115
        real t1117
        real t1119
        real t112
        real t1120
        real t1121
        real t1124
        real t1126
        real t1127
        real t1128
        real t1129
        real t1131
        real t1132
        real t1133
        real t1135
        real t1137
        real t1138
        real t1139
        real t114
        real t1141
        real t1142
        real t1143
        real t1148
        real t1150
        real t1155
        real t1157
        real t116
        real t1161
        real t1163
        real t1165
        real t1167
        real t1170
        real t1171
        real t1172
        real t1174
        real t1177
        real t1179
        real t118
        real t1183
        real t1184
        real t1196
        real t12
        real t120
        real t1202
        real t1209
        real t121
        real t1211
        real t1212
        real t1213
        real t1214
        real t1216
        real t1217
        real t1218
        real t122
        real t1220
        real t1222
        real t1223
        real t1224
        real t1225
        real t1226
        real t1227
        real t1228
        real t1233
        real t1235
        real t1238
        real t1241
        real t1243
        real t1245
        real t1247
        real t1249
        real t125
        real t1250
        real t1253
        real t1256
        real t1257
        real t1258
        real t1259
        integer t126
        real t1260
        real t1261
        real t1263
        real t1265
        real t1266
        real t1268
        real t127
        real t1270
        real t1271
        real t1273
        real t1274
        real t1275
        real t1278
        real t1279
        real t128
        real t1280
        real t1283
        real t1286
        real t1287
        real t1289
        real t1292
        real t1293
        real t1297
        real t1299
        real t13
        real t130
        real t1302
        real t1303
        real t1306
        real t131
        real t1312
        real t1315
        real t1318
        integer t132
        real t1321
        real t1324
        real t1327
        real t1328
        real t133
        real t1330
        real t1331
        real t1333
        real t1334
        real t1336
        real t1337
        real t1339
        real t134
        real t1340
        real t1342
        real t1343
        real t1345
        real t1346
        real t1351
        real t1353
        real t1356
        real t1358
        real t136
        real t1360
        real t1361
        real t1364
        real t1365
        real t1370
        real t1371
        real t1374
        real t1378
        real t138
        real t1381
        real t139
        real t1399
        integer t140
        real t1400
        real t1402
        real t1404
        real t1406
        real t1408
        real t141
        real t1410
        real t1412
        real t1413
        real t1418
        real t142
        real t1420
        real t1423
        real t1425
        real t1429
        real t143
        real t1435
        real t144
        real t1441
        real t1447
        real t1453
        real t1458
        real t146
        real t147
        real t1474
        real t1479
        real t1484
        real t149
        real t1493
        real t1496
        real t15
        real t150
        real t1501
        real t1504
        real t1510
        real t1511
        real t1513
        real t1515
        real t1519
        real t152
        real t1521
        real t1523
        real t1529
        integer t153
        real t1533
        real t1537
        real t154
        real t1541
        real t1542
        real t1546
        real t155
        real t1550
        real t1554
        real t156
        real t1560
        real t1564
        real t1569
        integer t1570
        real t1572
        real t1576
        real t158
        real t1589
        real t159
        real t1593
        integer t16
        real t1603
        real t1604
        real t1608
        real t161
        real t1615
        real t1616
        real t162
        real t1620
        real t163
        real t1631
        real t1634
        real t1636
        real t165
        real t167
        real t1679
        real t1685
        real t169
        real t1694
        real t1698
        real t17
        real t171
        real t1710
        real t1711
        real t1717
        real t1718
        real t1720
        real t1723
        real t1725
        real t1729
        real t173
        real t1730
        real t175
        real t1757
        real t1759
        real t176
        real t1763
        real t177
        real t1774
        real t1779
        real t1786
        real t1794
        real t18
        real t180
        real t1802
        real t1803
        real t1805
        real t1808
        real t1810
        real t1814
        real t1815
        real t182
        real t183
        real t184
        real t186
        real t1864
        real t1869
        real t1872
        real t1878
        real t188
        real t1881
        real t1886
        real t1889
        real t1891
        real t1894
        real t1897
        real t19
        real t190
        real t1901
        real t1904
        real t1908
        real t191
        real t1911
        real t1914
        real t1917
        real t192
        real t1921
        real t1924
        real t1927
        real t1930
        real t1933
        real t1936
        real t1941
        real t195
        real t196
        real t1961
        real t197
        real t1975
        real t1980
        real t1983
        real t1987
        real t199
        real t1993
        real t1999
        real t2
        real t200
        real t2005
        real t201
        real t2028
        real t203
        real t2031
        real t2032
        real t2033
        real t2035
        real t2036
        real t2037
        real t2038
        real t2039
        real t2040
        real t2041
        real t2042
        real t2043
        real t2044
        real t2047
        real t205
        real t2050
        real t2051
        real t2053
        real t2054
        real t2056
        real t2057
        real t2059
        real t206
        real t2060
        real t2062
        real t2063
        real t2065
        real t2066
        real t2067
        real t2069
        real t207
        real t2071
        real t2072
        real t2073
        real t2076
        real t2079
        real t208
        real t2081
        real t2082
        real t2084
        real t2085
        real t2087
        real t2089
        real t209
        real t2090
        real t2091
        real t2092
        real t2094
        real t2095
        real t2097
        real t2098
        real t21
        real t2101
        real t211
        real t2110
        real t2112
        real t2115
        real t2117
        real t212
        real t2137
        real t2139
        real t214
        real t2143
        real t2145
        real t2147
        real t2149
        real t215
        real t2152
        real t2154
        real t2155
        real t2157
        real t2158
        real t2160
        real t2162
        real t2163
        real t2164
        real t2165
        real t2166
        real t2167
        real t2168
        real t2169
        real t217
        real t2170
        real t2171
        real t2174
        real t2177
        real t218
        real t2180
        real t2183
        real t219
        real t2197
        real t2198
        real t22
        real t220
        real t2202
        real t2203
        real t2207
        real t2219
        real t222
        real t2220
        real t2224
        real t2225
        real t2228
        real t223
        integer t2231
        real t2233
        real t2237
        real t2241
        real t225
        real t2254
        real t2258
        real t226
        real t227
        real t2277
        real t2281
        real t2288
        real t229
        real t2292
        real t2303
        real t2306
        real t2308
        real t231
        real t233
        real t2344
        real t2348
        real t235
        real t2353
        real t2355
        real t2359
        real t2365
        real t237
        real t2372
        real t2373
        real t2376
        real t2377
        real t2379
        real t2382
        real t2384
        real t2388
        real t239
        real t240
        real t241
        real t2418
        real t2420
        real t2434
        real t2435
        real t244
        real t2440
        real t2447
        real t2448
        real t2450
        real t2453
        real t2455
        real t2459
        real t246
        real t247
        real t248
        real t2492
        real t25
        real t250
        real t2503
        real t251
        real t252
        real t2520
        real t2525
        real t2530
        real t2533
        real t2539
        real t254
        real t2542
        real t2543
        real t2548
        real t2549
        real t2550
        real t2552
        real t2553
        real t2554
        real t2555
        real t2556
        real t2557
        real t2558
        real t256
        real t2565
        real t2566
        real t2567
        real t2569
        real t257
        real t2570
        real t2572
        real t2573
        real t2575
        real t2576
        real t2578
        real t2579
        real t258
        real t2581
        real t2582
        real t2583
        real t2585
        real t2587
        real t2588
        real t2589
        real t259
        real t2592
        real t2595
        real t2597
        real t2598
        real t2599
        real t26
        real t260
        real t2601
        real t2602
        real t2604
        real t2606
        real t2607
        real t2608
        real t2610
        real t2611
        real t2612
        real t2617
        real t2619
        real t262
        real t2624
        real t2626
        real t2629
        real t263
        real t2631
        real t264
        real t2651
        real t2653
        real t2657
        real t2659
        real t266
        real t2661
        real t2663
        real t2666
        real t2668
        real t2669
        real t267
        real t2670
        real t2672
        real t2673
        real t2675
        real t2677
        real t2678
        real t2679
        real t268
        real t2680
        real t2681
        real t2682
        real t2683
        real t2688
        real t2690
        real t2693
        real t2696
        real t2698
        integer t27
        real t270
        real t2700
        real t2702
        real t2704
        real t2705
        real t2710
        real t2713
        real t2715
        real t2718
        real t272
        real t2721
        real t2725
        real t2728
        real t273
        real t2732
        real t2735
        real t2738
        real t274
        real t2744
        real t2747
        real t2750
        real t2753
        real t2756
        real t2759
        real t276
        real t2760
        real t2762
        real t2763
        real t2765
        real t2766
        real t2768
        real t2769
        real t277
        real t2771
        real t2772
        real t2774
        real t2775
        real t2780
        real t28
        real t280
        real t2813
        real t2815
        real t2817
        real t2819
        real t2821
        real t2823
        real t2824
        real t2829
        real t2832
        real t2836
        real t2842
        real t2848
        real t285
        real t2854
        real t2875
        real t2880
        real t2885
        real t2894
        real t2897
        real t29
        real t290
        real t2902
        real t2905
        real t291
        real t2919
        real t292
        real t2920
        real t2924
        real t2928
        real t294
        real t2940
        real t2941
        real t2945
        real t2950
        integer t2955
        real t2957
        real t296
        real t2961
        real t2974
        real t298
        real t2983
        real t2987
        real t2995
        real t2999
        real t30
        real t300
        real t3006
        real t3010
        real t302
        real t3023
        real t3026
        real t3028
        real t306
        real t3064
        real t3073
        real t3077
        real t308
        real t3089
        real t3090
        real t3093
        real t3095
        real t3098
        real t310
        real t3100
        real t3104
        real t312
        real t3134
        real t314
        real t3149
        real t3154
        real t316
        real t3161
        real t3162
        real t3164
        real t3167
        real t3169
        real t3173
        real t318
        real t32
        real t3206
        real t321
        real t322
        real t323
        real t3237
        real t3242
        real t3245
        real t325
        real t3251
        real t3254
        real t3259
        real t326
        real t3262
        real t3264
        real t3267
        real t327
        real t3270
        real t3274
        real t3277
        real t3281
        real t3284
        real t3287
        real t329
        real t3290
        real t3294
        real t3297
        real t33
        real t3300
        real t3303
        real t3306
        real t3309
        real t331
        real t3314
        real t332
        real t3320
        real t333
        real t3334
        real t3348
        real t335
        real t3353
        real t3356
        real t3360
        real t3366
        real t3372
        real t3378
        real t338
        real t34
        real t340
        real t344
        real t345
        real t35
        real t357
        real t363
        real t37
        real t370
        real t372
        real t376
        real t378
        integer t38
        real t380
        real t382
        real t385
        real t386
        real t387
        real t389
        real t39
        real t390
        real t391
        real t393
        real t395
        real t396
        real t397
        real t398
        real t399
        real t4
        real t40
        real t401
        real t402
        real t404
        real t405
        real t407
        real t408
        real t409
        real t41
        real t410
        real t412
        real t413
        real t415
        real t419
        real t421
        real t423
        real t425
        real t427
        real t429
        real t43
        real t431
        real t434
        real t436
        real t437
        real t438
        real t44
        real t440
        real t441
        real t442
        real t444
        real t446
        real t447
        real t448
        real t449
        real t45
        real t450
        real t451
        real t452
        real t453
        real t454
        real t456
        real t457
        real t458
        real t460
        real t462
        real t463
        real t464
        real t465
        real t466
        real t467
        real t47
        real t470
        real t473
        real t476
        real t479
        real t48
        real t482
        real t483
        real t487
        real t489
        real t490
        real t492
        real t493
        real t495
        real t497
        integer t5
        real t501
        real t503
        real t504
        real t506
        real t508
        real t515
        real t517
        real t518
        real t52
        real t520
        real t522
        real t523
        real t525
        real t526
        real t528
        real t53
        real t530
        real t531
        real t533
        real t535
        real t536
        real t54
        real t540
        real t541
        real t545
        real t546
        real t547
        real t55
        real t551
        real t553
        real t554
        real t557
        real t559
        real t56
        real t561
        real t565
        real t568
        real t57
        real t570
        real t572
        real t579
        real t58
        real t581
        real t582
        real t584
        real t586
        real t587
        real t589
        real t59
        real t590
        real t592
        real t594
        real t595
        real t597
        real t599
        real t6
        real t600
        real t604
        real t605
        real t608
        real t61
        real t611
        integer t612
        real t613
        real t614
        real t618
        real t62
        integer t625
        real t626
        real t627
        real t63
        real t631
        real t64
        integer t645
        real t647
        real t651
        real t659
        real t66
        real t666
        real t67
        real t670
        real t677
        real t681
        real t684
        real t693
        real t696
        real t698
        real t7
        real t70
        real t71
        real t72
        real t734
        real t738
        real t74
        real t743
        real t745
        real t749
        real t75
        real t755
        real t76
        real t762
        real t763
        real t766
        real t77
        real t770
        real t771
        real t773
        real t776
        real t778
        real t78
        real t782
        real t783
        real t79
        real t8
        real t80
        real t81
        real t810
        real t812
        real t813
        real t817
        real t82
        real t828
        real t829
        real t834
        real t84
        real t841
        real t849
        real t85
        real t857
        real t858
        real t86
        real t860
        real t863
        real t865
        real t869
        real t87
        real t870
        real t88
        real t89
        real t899
        real t9
        real t916
        real t92
        real t921
        real t926
        real t929
        real t935
        real t938
        integer t941
        real t942
        real t943
        real t944
        real t946
        real t947
        real t948
        real t95
        real t950
        real t951
        real t953
        real t958
        real t959
        real t96
        real t960
        real t962
        real t969
        real t971
        real t973
        real t974
        real t975
        real t976
        real t977
        real t98
        real t985
        real t986
        real t988
        real t990
        real t992
        real t993
        real t994
        real t995
        real t996
        real t997
        real t998
        t1 = u(i,j,n)
        t2 = ut(i,j,n)
        t4 = cc ** 2
        t5 = i + 1
        t6 = u(t5,j,n)
        t7 = t6 - t1
        t8 = 0.1E1 / dx
        t9 = t7 * t8
        t10 = i + 2
        t11 = u(t10,j,n)
        t12 = t11 - t6
        t13 = t12 * t8
        t15 = (t13 - t9) * t8
        t16 = i - 1
        t17 = u(t16,j,n)
        t18 = t1 - t17
        t19 = t18 * t8
        t21 = (t9 - t19) * t8
        t22 = t15 - t21
        t25 = dx ** 2
        t26 = t25 * dx
        t27 = i + 3
        t28 = u(t27,j,n)
        t29 = t28 - t11
        t30 = t29 * t8
        t32 = (t30 - t13) * t8
        t33 = t32 - t15
        t34 = t33 * t8
        t35 = t22 * t8
        t37 = (t34 - t35) * t8
        t38 = i - 2
        t39 = u(t38,j,n)
        t40 = t17 - t39
        t41 = t40 * t8
        t43 = (t19 - t41) * t8
        t44 = t21 - t43
        t45 = t44 * t8
        t47 = (t35 - t45) * t8
        t48 = t37 - t47
        t52 = t4 * (t9 - dx * t22 / 0.24E2 + 0.3E1 / 0.640E3 * t26 * t48
     #)
        t53 = t4 * dt
        t54 = ut(t5,j,n)
        t55 = t54 - t2
        t56 = t55 * t8
        t57 = ut(t10,j,n)
        t58 = t57 - t54
        t59 = t58 * t8
        t61 = (t59 - t56) * t8
        t62 = ut(t16,j,n)
        t63 = t2 - t62
        t64 = t63 * t8
        t66 = (t56 - t64) * t8
        t67 = t61 - t66
        t70 = ut(t27,j,n)
        t71 = t70 - t57
        t72 = t71 * t8
        t74 = (t72 - t59) * t8
        t75 = t74 - t61
        t76 = t75 * t8
        t77 = t67 * t8
        t78 = t76 - t77
        t79 = t78 * t8
        t80 = ut(t38,j,n)
        t81 = t62 - t80
        t82 = t81 * t8
        t84 = (t64 - t82) * t8
        t85 = t66 - t84
        t86 = t85 * t8
        t87 = t77 - t86
        t88 = t87 * t8
        t89 = t79 - t88
        t92 = t56 - dx * t67 / 0.24E2 + 0.3E1 / 0.640E3 * t26 * t89
        t95 = dt ** 2
        t96 = t4 * t95
        t98 = t4 * t12 * t8
        t100 = t4 * t7 * t8
        t102 = (t98 - t100) * t8
        t104 = t4 * t33 * t8
        t106 = t4 * t22 * t8
        t107 = t104 - t106
        t108 = t107 * t8
        t110 = t4 * t29 * t8
        t112 = (t110 - t98) * t8
        t114 = (t112 - t102) * t8
        t116 = t4 * t18 * t8
        t118 = (t100 - t116) * t8
        t120 = (t102 - t118) * t8
        t121 = t114 - t120
        t122 = t121 * t8
        t125 = t25 * (t108 + t122) / 0.24E2
        t126 = j + 1
        t127 = u(t5,t126,n)
        t128 = t127 - t6
        t130 = 0.1E1 / dy
        t131 = t4 * t128 * t130
        t132 = j - 1
        t133 = u(t5,t132,n)
        t134 = t6 - t133
        t136 = t4 * t134 * t130
        t138 = (t131 - t136) * t130
        t139 = dy ** 2
        t140 = j + 2
        t141 = u(t5,t140,n)
        t142 = t141 - t127
        t143 = t142 * t130
        t144 = t128 * t130
        t146 = (t143 - t144) * t130
        t147 = t134 * t130
        t149 = (t144 - t147) * t130
        t150 = t146 - t149
        t152 = t4 * t150 * t130
        t153 = j - 2
        t154 = u(t5,t153,n)
        t155 = t133 - t154
        t156 = t155 * t130
        t158 = (t147 - t156) * t130
        t159 = t149 - t158
        t161 = t4 * t159 * t130
        t162 = t152 - t161
        t163 = t162 * t130
        t165 = t4 * t142 * t130
        t167 = (t165 - t131) * t130
        t169 = (t167 - t138) * t130
        t171 = t4 * t155 * t130
        t173 = (t136 - t171) * t130
        t175 = (t138 - t173) * t130
        t176 = t169 - t175
        t177 = t176 * t130
        t180 = t139 * (t163 + t177) / 0.24E2
        t182 = t4 * t44 * t8
        t183 = t106 - t182
        t184 = t183 * t8
        t186 = t4 * t40 * t8
        t188 = (t116 - t186) * t8
        t190 = (t118 - t188) * t8
        t191 = t120 - t190
        t192 = t191 * t8
        t195 = t25 * (t184 + t192) / 0.24E2
        t196 = u(i,t126,n)
        t197 = t196 - t1
        t199 = t4 * t197 * t130
        t200 = u(i,t132,n)
        t201 = t1 - t200
        t203 = t4 * t201 * t130
        t205 = (t199 - t203) * t130
        t206 = u(i,t140,n)
        t207 = t206 - t196
        t208 = t207 * t130
        t209 = t197 * t130
        t211 = (t208 - t209) * t130
        t212 = t201 * t130
        t214 = (t209 - t212) * t130
        t215 = t211 - t214
        t217 = t4 * t215 * t130
        t218 = u(i,t153,n)
        t219 = t200 - t218
        t220 = t219 * t130
        t222 = (t212 - t220) * t130
        t223 = t214 - t222
        t225 = t4 * t223 * t130
        t226 = t217 - t225
        t227 = t226 * t130
        t229 = t4 * t207 * t130
        t231 = (t229 - t199) * t130
        t233 = (t231 - t205) * t130
        t235 = t4 * t219 * t130
        t237 = (t203 - t235) * t130
        t239 = (t205 - t237) * t130
        t240 = t233 - t239
        t241 = t240 * t130
        t244 = t139 * (t227 + t241) / 0.24E2
        t246 = (t102 - t125 + t138 - t180 - t118 + t195 - t205 + t244) *
     # t8
        t247 = u(t10,t126,n)
        t248 = t247 - t11
        t250 = t4 * t248 * t130
        t251 = u(t10,t132,n)
        t252 = t11 - t251
        t254 = t4 * t252 * t130
        t256 = (t250 - t254) * t130
        t257 = t112 + t256 - t102 - t138
        t258 = t257 * t8
        t259 = t102 + t138 - t118 - t205
        t260 = t259 * t8
        t262 = (t258 - t260) * t8
        t263 = u(t16,t126,n)
        t264 = t263 - t17
        t266 = t4 * t264 * t130
        t267 = u(t16,t132,n)
        t268 = t17 - t267
        t270 = t4 * t268 * t130
        t272 = (t266 - t270) * t130
        t273 = t118 + t205 - t188 - t272
        t274 = t273 * t8
        t276 = (t260 - t274) * t8
        t277 = t262 - t276
        t280 = t246 - dx * t277 / 0.24E2
        t285 = t122 - t192
        t290 = t25 * ((t102 - t125 - t118 + t195) * t8 - dx * t285 / 0.2
     #4E2) / 0.24E2
        t291 = t95 * dt
        t292 = t4 * t291
        t294 = t4 * t58 * t8
        t296 = t4 * t55 * t8
        t298 = (t294 - t296) * t8
        t300 = t4 * t75 * t8
        t302 = t4 * t67 * t8
        t306 = t4 * t71 * t8
        t308 = (t306 - t294) * t8
        t310 = (t308 - t298) * t8
        t312 = t4 * t63 * t8
        t314 = (t296 - t312) * t8
        t316 = (t298 - t314) * t8
        t318 = (t310 - t316) * t8
        t321 = t25 * ((t300 - t302) * t8 + t318) / 0.24E2
        t322 = ut(t5,t126,n)
        t323 = t322 - t54
        t325 = t4 * t323 * t130
        t326 = ut(t5,t132,n)
        t327 = t54 - t326
        t329 = t4 * t327 * t130
        t331 = (t325 - t329) * t130
        t332 = ut(t5,t140,n)
        t333 = t332 - t322
        t335 = t323 * t130
        t338 = t327 * t130
        t340 = (t335 - t338) * t130
        t344 = ut(t5,t153,n)
        t345 = t326 - t344
        t357 = (t4 * t333 * t130 - t325) * t130
        t363 = (t329 - t4 * t345 * t130) * t130
        t370 = t139 * ((t4 * ((t333 * t130 - t335) * t130 - t340) * t130
     # - t4 * (t340 - (t338 - t345 * t130) * t130) * t130) * t130 + ((t3
     #57 - t331) * t130 - (t331 - t363) * t130) * t130) / 0.24E2
        t372 = t4 * t85 * t8
        t376 = t4 * t81 * t8
        t378 = (t312 - t376) * t8
        t380 = (t314 - t378) * t8
        t382 = (t316 - t380) * t8
        t385 = t25 * ((t302 - t372) * t8 + t382) / 0.24E2
        t386 = ut(i,t126,n)
        t387 = t386 - t2
        t389 = t4 * t387 * t130
        t390 = ut(i,t132,n)
        t391 = t2 - t390
        t393 = t4 * t391 * t130
        t395 = (t389 - t393) * t130
        t396 = ut(i,t140,n)
        t397 = t396 - t386
        t398 = t397 * t130
        t399 = t387 * t130
        t401 = (t398 - t399) * t130
        t402 = t391 * t130
        t404 = (t399 - t402) * t130
        t405 = t401 - t404
        t407 = t4 * t405 * t130
        t408 = ut(i,t153,n)
        t409 = t390 - t408
        t410 = t409 * t130
        t412 = (t402 - t410) * t130
        t413 = t404 - t412
        t415 = t4 * t413 * t130
        t419 = t4 * t397 * t130
        t421 = (t419 - t389) * t130
        t423 = (t421 - t395) * t130
        t425 = t4 * t409 * t130
        t427 = (t393 - t425) * t130
        t429 = (t395 - t427) * t130
        t431 = (t423 - t429) * t130
        t434 = t139 * ((t407 - t415) * t130 + t431) / 0.24E2
        t436 = (t298 - t321 + t331 - t370 - t314 + t385 - t395 + t434) *
     # t8
        t437 = ut(t10,t126,n)
        t438 = t437 - t57
        t440 = t4 * t438 * t130
        t441 = ut(t10,t132,n)
        t442 = t57 - t441
        t444 = t4 * t442 * t130
        t446 = (t440 - t444) * t130
        t447 = t308 + t446 - t298 - t331
        t448 = t447 * t8
        t449 = t298 + t331 - t314 - t395
        t450 = t449 * t8
        t451 = t448 - t450
        t452 = t451 * t8
        t453 = ut(t16,t126,n)
        t454 = t453 - t62
        t456 = t4 * t454 * t130
        t457 = ut(t16,t132,n)
        t458 = t62 - t457
        t460 = t4 * t458 * t130
        t462 = (t456 - t460) * t130
        t463 = t314 + t395 - t378 - t462
        t464 = t463 * t8
        t465 = t450 - t464
        t466 = t465 * t8
        t467 = t452 - t466
        t470 = t436 - dx * t467 / 0.24E2
        t473 = dt * t25
        t476 = t318 - t382
        t479 = (t298 - t321 - t314 + t385) * t8 - dx * t476 / 0.24E2
        t482 = t95 ** 2
        t483 = t4 * t482
        t487 = t4 * t259 * t8
        t489 = (t4 * t257 * t8 - t487) * t8
        t490 = t247 - t127
        t492 = t4 * t490 * t8
        t493 = t127 - t196
        t495 = t4 * t493 * t8
        t497 = (t492 - t495) * t8
        t501 = t251 - t133
        t503 = t4 * t501 * t8
        t504 = t133 - t200
        t506 = t4 * t504 * t8
        t508 = (t503 - t506) * t8
        t515 = t4 * t273 * t8
        t517 = (t487 - t515) * t8
        t518 = t196 - t263
        t520 = t4 * t518 * t8
        t522 = (t495 - t520) * t8
        t523 = t522 + t231 - t118 - t205
        t525 = t4 * t523 * t130
        t526 = t200 - t267
        t528 = t4 * t526 * t8
        t530 = (t506 - t528) * t8
        t531 = t118 + t205 - t530 - t237
        t533 = t4 * t531 * t130
        t535 = (t525 - t533) * t130
        t536 = t489 + (t4 * (t497 + t167 - t102 - t138) * t130 - t4 * (t
     #102 + t138 - t508 - t173) * t130) * t130 - t517 - t535
        t540 = t95 * dx
        t541 = t489 - t517
        t545 = 0.7E1 / 0.5760E4 * t26 * t285
        t546 = t482 * dt
        t547 = t4 * t546
        t551 = t4 * t449 * t8
        t553 = (t4 * t447 * t8 - t551) * t8
        t554 = t437 - t322
        t557 = t322 - t386
        t559 = t4 * t557 * t8
        t561 = (t4 * t554 * t8 - t559) * t8
        t565 = t441 - t326
        t568 = t326 - t390
        t570 = t4 * t568 * t8
        t572 = (t4 * t565 * t8 - t570) * t8
        t579 = t4 * t463 * t8
        t581 = (t551 - t579) * t8
        t582 = t386 - t453
        t584 = t4 * t582 * t8
        t586 = (t559 - t584) * t8
        t587 = t586 + t421 - t314 - t395
        t589 = t4 * t587 * t130
        t590 = t390 - t457
        t592 = t4 * t590 * t8
        t594 = (t570 - t592) * t8
        t595 = t314 + t395 - t594 - t427
        t597 = t4 * t595 * t130
        t599 = (t589 - t597) * t130
        t600 = t553 + (t4 * (t561 + t357 - t298 - t331) * t130 - t4 * (t
     #298 + t331 - t572 - t363) * t130) * t130 - t581 - t599
        t604 = t291 * dx
        t605 = t553 - t581
        t608 = dt * t26
        t611 = t139 * dy
        t612 = j + 3
        t613 = u(t5,t612,n)
        t614 = t613 - t141
        t618 = (t614 * t130 - t143) * t130 - t146
        t625 = j - 3
        t626 = u(t5,t625,n)
        t627 = t154 - t626
        t631 = t158 - (t156 - t627 * t130) * t130
        t645 = i + 4
        t647 = u(t645,j,n) - t28
        t651 = (t647 * t8 - t30) * t8 - t32
        t659 = t4 * t48 * t8
        t666 = (t4 * t651 * t8 - t104) * t8
        t670 = (t108 - t184) * t8
        t677 = (t4 * t647 * t8 - t110) * t8
        t681 = ((t677 - t112) * t8 - t114) * t8
        t684 = t285 * t8
        t693 = t150 * t130
        t696 = t159 * t130
        t698 = (t693 - t696) * t130
        t734 = t102 + t138 + t611 * (((t4 * t618 * t130 - t152) * t130 -
     # t163) * t130 - (t163 - (t161 - t4 * t631 * t130) * t130) * t130) 
     #/ 0.576E3 - dx * t107 / 0.24E2 - dx * t121 / 0.24E2 + 0.3E1 / 0.64
     #0E3 * t26 * (t4 * ((t651 * t8 - t34) * t8 - t37) * t8 - t659) + t2
     #6 * ((t666 - t108) * t8 - t670) / 0.576E3 + 0.3E1 / 0.640E3 * t26 
     #* ((t681 - t122) * t8 - t684) - dy * t162 / 0.24E2 - dy * t176 / 0
     #.24E2 + 0.3E1 / 0.640E3 * t611 * (t4 * ((t618 * t130 - t693) * t13
     #0 - t698) * t130 - t4 * (t698 - (t696 - t631 * t130) * t130) * t13
     #0) + 0.3E1 / 0.640E3 * t611 * (((((t4 * t614 * t130 - t165) * t130
     # - t167) * t130 - t169) * t130 - t177) * t130 - (t177 - (t175 - (t
     #173 - (t171 - t4 * t627 * t130) * t130) * t130) * t130) * t130)
        t738 = t56 / 0.2E1
        t743 = t25 ** 2
        t745 = ut(t645,j,n) - t70
        t749 = (t745 * t8 - t72) * t8 - t74
        t755 = t89 * t8
        t762 = dx * (t59 / 0.2E1 + t738 - t25 * (t76 / 0.2E1 + t77 / 0.2
     #E1) / 0.6E1 + t743 * (((t749 * t8 - t76) * t8 - t79) * t8 / 0.2E1 
     #+ t755 / 0.2E1) / 0.30E2) / 0.2E1
        t763 = t298 - t321 + t331 - t370
        t766 = dt * dx
        t770 = u(t10,t140,n)
        t771 = t770 - t247
        t773 = t248 * t130
        t776 = t252 * t130
        t778 = (t773 - t776) * t130
        t782 = u(t10,t153,n)
        t783 = t251 - t782
        t810 = (t112 - t25 * (t666 + t681) / 0.24E2 + t256 - t139 * ((t4
     # * ((t771 * t130 - t773) * t130 - t778) * t130 - t4 * (t778 - (t77
     #6 - t783 * t130) * t130) * t130) * t130 + (((t4 * t771 * t130 - t2
     #50) * t130 - t256) * t130 - (t256 - (t254 - t4 * t783 * t130) * t1
     #30) * t130) * t130) / 0.24E2 - t102 + t125 - t138 + t180) * t8
        t812 = t246 / 0.2E1
        t813 = u(t27,t126,n)
        t817 = u(t27,t132,n)
        t828 = (((t677 + (t4 * (t813 - t28) * t130 - t4 * (t28 - t817) *
     # t130) * t130 - t112 - t256) * t8 - t258) * t8 - t262) * t8
        t829 = t277 * t8
        t834 = t810 / 0.2E1 + t812 - t25 * (t828 / 0.2E1 + t829 / 0.2E1)
     # / 0.6E1
        t841 = t25 * (t61 - dx * t78 / 0.12E2) / 0.12E2
        t849 = (t4 * t745 * t8 - t306) * t8
        t857 = ut(t10,t140,n)
        t858 = t857 - t437
        t860 = t438 * t130
        t863 = t442 * t130
        t865 = (t860 - t863) * t130
        t869 = ut(t10,t153,n)
        t870 = t441 - t869
        t899 = t436 / 0.2E1
        t916 = t467 * t8
        t921 = (t308 - t25 * ((t4 * t749 * t8 - t300) * t8 + ((t849 - t3
     #08) * t8 - t310) * t8) / 0.24E2 + t446 - t139 * ((t4 * ((t858 * t1
     #30 - t860) * t130 - t865) * t130 - t4 * (t865 - (t863 - t870 * t13
     #0) * t130) * t130) * t130 + (((t4 * t858 * t130 - t440) * t130 - t
     #446) * t130 - (t446 - (t444 - t4 * t870 * t130) * t130) * t130) * 
     #t130) / 0.24E2 - t298 + t321 - t331 + t370) * t8 / 0.2E1 + t899 - 
     #t25 * ((((t849 + (t4 * (ut(t27,t126,n) - t70) * t130 - t4 * (t70 -
     # ut(t27,t132,n)) * t130) * t130 - t308 - t446) * t8 - t448) * t8 -
     # t452) * t8 / 0.2E1 + t916 / 0.2E1) / 0.6E1
        t926 = t828 - t829
        t929 = (t810 - t246) * t8 - dx * t926 / 0.12E2
        t935 = t26 * t78 / 0.720E3
        t938 = t54 + dt * t734 / 0.2E1 - t762 + t95 * t763 / 0.8E1 - t76
     #6 * t834 / 0.4E1 + t841 - t540 * t921 / 0.16E2 + t473 * t929 / 0.2
     #4E2 + t540 * t451 / 0.96E2 - t935 - t608 * t926 / 0.1440E4
        t941 = i - 3
        t942 = u(t941,j,n)
        t943 = t39 - t942
        t944 = t943 * t8
        t946 = (t41 - t944) * t8
        t947 = t43 - t946
        t948 = t947 * t8
        t950 = (t45 - t948) * t8
        t951 = t47 - t950
        t953 = t4 * t951 * t8
        t958 = t4 * t947 * t8
        t959 = t182 - t958
        t960 = t959 * t8
        t962 = (t184 - t960) * t8
        t969 = t4 * t943 * t8
        t971 = (t186 - t969) * t8
        t973 = (t188 - t971) * t8
        t974 = t190 - t973
        t975 = t974 * t8
        t976 = t192 - t975
        t977 = t976 * t8
        t985 = u(i,t612,n)
        t986 = t985 - t206
        t988 = t4 * t986 * t130
        t990 = (t988 - t229) * t130
        t992 = (t990 - t231) * t130
        t993 = t992 - t233
        t994 = t993 * t130
        t995 = t994 - t241
        t996 = t995 * t130
        t997 = u(i,t625,n)
        t998 = t218 - t997
        t1000 = t4 * t998 * t130
        t1002 = (t235 - t1000) * t130
        t1004 = (t237 - t1002) * t130
        t1005 = t239 - t1004
        t1006 = t1005 * t130
        t1007 = t241 - t1006
        t1008 = t1007 * t130
        t1012 = t986 * t130
        t1014 = (t1012 - t208) * t130
        t1015 = t1014 - t211
        t1016 = t1015 * t130
        t1017 = t215 * t130
        t1019 = (t1016 - t1017) * t130
        t1020 = t223 * t130
        t1022 = (t1017 - t1020) * t130
        t1023 = t1019 - t1022
        t1025 = t4 * t1023 * t130
        t1026 = t998 * t130
        t1028 = (t220 - t1026) * t130
        t1029 = t222 - t1028
        t1030 = t1029 * t130
        t1032 = (t1020 - t1030) * t130
        t1033 = t1022 - t1032
        t1035 = t4 * t1033 * t130
        t1040 = t4 * t1015 * t130
        t1041 = t1040 - t217
        t1042 = t1041 * t130
        t1044 = (t1042 - t227) * t130
        t1046 = t4 * t1029 * t130
        t1047 = t225 - t1046
        t1048 = t1047 * t130
        t1050 = (t227 - t1048) * t130
        t1054 = t118 + t205 - dx * t183 / 0.24E2 + 0.3E1 / 0.640E3 * t26
     # * (t659 - t953) + t26 * (t670 - t962) / 0.576E3 - dx * t191 / 0.2
     #4E2 + 0.3E1 / 0.640E3 * t26 * (t684 - t977) - dy * t226 / 0.24E2 -
     # dy * t240 / 0.24E2 + 0.3E1 / 0.640E3 * t611 * (t996 - t1008) + 0.
     #3E1 / 0.640E3 * t611 * (t1025 - t1035) + t611 * (t1044 - t1050) / 
     #0.576E3
        t1056 = dt * t1054 / 0.2E1
        t1057 = t64 / 0.2E1
        t1062 = ut(t941,j,n)
        t1063 = t80 - t1062
        t1064 = t1063 * t8
        t1066 = (t82 - t1064) * t8
        t1067 = t84 - t1066
        t1068 = t1067 * t8
        t1069 = t86 - t1068
        t1070 = t1069 * t8
        t1071 = t88 - t1070
        t1072 = t1071 * t8
        t1079 = dx * (t738 + t1057 - t25 * (t77 / 0.2E1 + t86 / 0.2E1) /
     # 0.6E1 + t743 * (t755 / 0.2E1 + t1072 / 0.2E1) / 0.30E2) / 0.2E1
        t1080 = t314 - t385 + t395 - t434
        t1082 = t95 * t1080 / 0.8E1
        t1085 = t25 * (t960 + t975) / 0.24E2
        t1086 = u(t16,t140,n)
        t1087 = t1086 - t263
        t1088 = t1087 * t130
        t1089 = t264 * t130
        t1091 = (t1088 - t1089) * t130
        t1092 = t268 * t130
        t1094 = (t1089 - t1092) * t130
        t1095 = t1091 - t1094
        t1097 = t4 * t1095 * t130
        t1098 = u(t16,t153,n)
        t1099 = t267 - t1098
        t1100 = t1099 * t130
        t1102 = (t1092 - t1100) * t130
        t1103 = t1094 - t1102
        t1105 = t4 * t1103 * t130
        t1106 = t1097 - t1105
        t1107 = t1106 * t130
        t1109 = t4 * t1087 * t130
        t1111 = (t1109 - t266) * t130
        t1113 = (t1111 - t272) * t130
        t1115 = t4 * t1099 * t130
        t1117 = (t270 - t1115) * t130
        t1119 = (t272 - t1117) * t130
        t1120 = t1113 - t1119
        t1121 = t1120 * t130
        t1124 = t139 * (t1107 + t1121) / 0.24E2
        t1126 = (t118 - t195 + t205 - t244 - t188 + t1085 - t272 + t1124
     #) * t8
        t1127 = t1126 / 0.2E1
        t1128 = u(t38,t126,n)
        t1129 = t1128 - t39
        t1131 = t4 * t1129 * t130
        t1132 = u(t38,t132,n)
        t1133 = t39 - t1132
        t1135 = t4 * t1133 * t130
        t1137 = (t1131 - t1135) * t130
        t1138 = t188 + t272 - t971 - t1137
        t1139 = t1138 * t8
        t1141 = (t274 - t1139) * t8
        t1142 = t276 - t1141
        t1143 = t1142 * t8
        t1148 = t812 + t1127 - t25 * (t829 / 0.2E1 + t1143 / 0.2E1) / 0.
     #6E1
        t1150 = t766 * t1148 / 0.4E1
        t1155 = t25 * (t66 - dx * t87 / 0.12E2) / 0.12E2
        t1157 = t4 * t1067 * t8
        t1161 = t4 * t1063 * t8
        t1163 = (t376 - t1161) * t8
        t1165 = (t378 - t1163) * t8
        t1167 = (t380 - t1165) * t8
        t1170 = t25 * ((t372 - t1157) * t8 + t1167) / 0.24E2
        t1171 = ut(t16,t140,n)
        t1172 = t1171 - t453
        t1174 = t454 * t130
        t1177 = t458 * t130
        t1179 = (t1174 - t1177) * t130
        t1183 = ut(t16,t153,n)
        t1184 = t457 - t1183
        t1196 = (t4 * t1172 * t130 - t456) * t130
        t1202 = (t460 - t4 * t1184 * t130) * t130
        t1209 = t139 * ((t4 * ((t1172 * t130 - t1174) * t130 - t1179) * 
     #t130 - t4 * (t1179 - (t1177 - t1184 * t130) * t130) * t130) * t130
     # + ((t1196 - t462) * t130 - (t462 - t1202) * t130) * t130) / 0.24E
     #2
        t1211 = (t314 - t385 + t395 - t434 - t378 + t1170 - t462 + t1209
     #) * t8
        t1212 = t1211 / 0.2E1
        t1213 = ut(t38,t126,n)
        t1214 = t1213 - t80
        t1216 = t4 * t1214 * t130
        t1217 = ut(t38,t132,n)
        t1218 = t80 - t1217
        t1220 = t4 * t1218 * t130
        t1222 = (t1216 - t1220) * t130
        t1223 = t378 + t462 - t1163 - t1222
        t1224 = t1223 * t8
        t1225 = t464 - t1224
        t1226 = t1225 * t8
        t1227 = t466 - t1226
        t1228 = t1227 * t8
        t1233 = t899 + t1212 - t25 * (t916 / 0.2E1 + t1228 / 0.2E1) / 0.
     #6E1
        t1235 = t540 * t1233 / 0.16E2
        t1238 = t829 - t1143
        t1241 = (t246 - t1126) * t8 - dx * t1238 / 0.12E2
        t1243 = t473 * t1241 / 0.24E2
        t1245 = t540 * t465 / 0.96E2
        t1247 = t26 * t87 / 0.720E3
        t1249 = t608 * t1238 / 0.1440E4
        t1250 = -t2 - t1056 - t1079 - t1082 - t1150 - t1155 - t1235 - t1
     #243 - t1245 + t1247 + t1249
        t1253 = sqrt(0.256E3)
        t1256 = t52 + t53 * t92 / 0.2E1 + t96 * t280 / 0.8E1 - t290 + t2
     #92 * t470 / 0.48E2 - t473 * t479 / 0.48E2 + t483 * t536 * t8 / 0.3
     #84E3 - t540 * t541 / 0.192E3 + t545 + t547 * t600 * t8 / 0.3840E4 
     #- t604 * t605 / 0.2304E4 + 0.7E1 / 0.11520E5 * t608 * t476 + cc * 
     #(t938 + t1250) * t1253 / 0.32E2
        t1257 = dt / 0.2E1
        t1258 = sqrt(0.15E2)
        t1259 = t1258 / 0.10E2
        t1260 = 0.1E1 / 0.2E1 - t1259
        t1261 = dt * t1260
        t1263 = 0.1E1 / (t1257 - t1261)
        t1265 = 0.1E1 / 0.2E1 + t1259
        t1266 = dt * t1265
        t1268 = 0.1E1 / (t1257 - t1266)
        t1270 = t4 * t1260
        t1271 = dt * t92
        t1273 = t1260 ** 2
        t1274 = t4 * t1273
        t1275 = t95 * t280
        t1278 = t1273 * t1260
        t1279 = t4 * t1278
        t1280 = t291 * t470
        t1283 = t25 * t479
        t1286 = t1273 ** 2
        t1287 = t4 * t1286
        t1289 = t482 * t536 * t8
        t1292 = t1273 * t95
        t1293 = dx * t541
        t1297 = t4 * t1286 * t1260
        t1299 = t546 * t600 * t8
        t1302 = t1278 * t291
        t1303 = dx * t605
        t1306 = t26 * t476
        t1312 = dx * t834
        t1315 = dx * t921
        t1318 = t25 * t929
        t1321 = dx * t451
        t1324 = t26 * t926
        t1327 = t54 + t1261 * t734 - t762 + t1292 * t763 / 0.2E1 - t1261
     # * t1312 / 0.2E1 + t841 - t1292 * t1315 / 0.4E1 + t1261 * t1318 / 
     #0.12E2 + t1292 * t1321 / 0.24E2 - t935 - t1261 * t1324 / 0.720E3
        t1328 = t1261 * t1054
        t1330 = t1292 * t1080 / 0.2E1
        t1331 = dx * t1148
        t1333 = t1261 * t1331 / 0.2E1
        t1334 = dx * t1233
        t1336 = t1292 * t1334 / 0.4E1
        t1337 = t25 * t1241
        t1339 = t1261 * t1337 / 0.12E2
        t1340 = dx * t465
        t1342 = t1292 * t1340 / 0.24E2
        t1343 = t26 * t1238
        t1345 = t1261 * t1343 / 0.720E3
        t1346 = -t2 - t1328 - t1079 - t1330 - t1333 - t1155 - t1336 - t1
     #339 - t1342 + t1247 + t1345
        t1351 = t52 + t1270 * t1271 + t1274 * t1275 / 0.2E1 - t290 + t12
     #79 * t1280 / 0.6E1 - t1261 * t1283 / 0.24E2 + t1287 * t1289 / 0.24
     #E2 - t1292 * t1293 / 0.48E2 + t545 + t1297 * t1299 / 0.120E3 - t13
     #02 * t1303 / 0.288E3 + 0.7E1 / 0.5760E4 * t1261 * t1306 + cc * (t1
     #327 + t1346) * t1253 / 0.32E2
        t1353 = -t1263
        t1356 = 0.1E1 / (t1261 - t1266)
        t1358 = t4 * t1265
        t1360 = t1265 ** 2
        t1361 = t4 * t1360
        t1364 = t1360 * t1265
        t1365 = t4 * t1364
        t1370 = t1360 ** 2
        t1371 = t4 * t1370
        t1374 = t1360 * t95
        t1378 = t4 * t1370 * t1265
        t1381 = t1364 * t291
        t1399 = t54 + t1266 * t734 - t762 + t1374 * t763 / 0.2E1 - t1266
     # * t1312 / 0.2E1 + t841 - t1374 * t1315 / 0.4E1 + t1266 * t1318 / 
     #0.12E2 + t1374 * t1321 / 0.24E2 - t935 - t1266 * t1324 / 0.720E3
        t1400 = t1266 * t1054
        t1402 = t1374 * t1080 / 0.2E1
        t1404 = t1266 * t1331 / 0.2E1
        t1406 = t1374 * t1334 / 0.4E1
        t1408 = t1266 * t1337 / 0.12E2
        t1410 = t1374 * t1340 / 0.24E2
        t1412 = t1266 * t1343 / 0.720E3
        t1413 = -t2 - t1400 - t1079 - t1402 - t1404 - t1155 - t1406 - t1
     #408 - t1410 + t1247 + t1412
        t1418 = t52 + t1358 * t1271 + t1361 * t1275 / 0.2E1 - t290 + t13
     #65 * t1280 / 0.6E1 - t1266 * t1283 / 0.24E2 + t1371 * t1289 / 0.24
     #E2 - t1374 * t1293 / 0.48E2 + t545 + t1378 * t1299 / 0.120E3 - t13
     #81 * t1303 / 0.288E3 + 0.7E1 / 0.5760E4 * t1266 * t1306 + cc * (t1
     #399 + t1413) * t1253 / 0.32E2
        t1420 = -t1356
        t1423 = -t1268
        t1425 = t1256 * t1263 * t1268 + t1351 * t1353 * t1356 + t1418 * 
     #t1420 * t1423
        t1429 = t1351 * dt
        t1435 = t1256 * dt
        t1441 = t1418 * dt
        t1447 = (-t1429 / 0.2E1 - t1429 * t1265) * t1353 * t1356 + (-t14
     #35 * t1260 - t1435 * t1265) * t1263 * t1268 + (-t1441 * t1260 - t1
     #441 / 0.2E1) * t1420 * t1423
        t1453 = t1265 * t1353 * t1356
        t1458 = t1260 * t1420 * t1423
        t1474 = t4 * (t19 - dx * t44 / 0.24E2 + 0.3E1 / 0.640E3 * t26 * 
     #t951)
        t1479 = t64 - dx * t85 / 0.24E2 + 0.3E1 / 0.640E3 * t26 * t1071
        t1484 = t1126 - dx * t1142 / 0.24E2
        t1493 = t25 * ((t118 - t195 - t188 + t1085) * t8 - dx * t976 / 0
     #.24E2) / 0.24E2
        t1496 = t1211 - dx * t1227 / 0.24E2
        t1501 = t382 - t1167
        t1504 = (t314 - t385 - t378 + t1170) * t8 - dx * t1501 / 0.24E2
        t1510 = (t515 - t4 * t1138 * t8) * t8
        t1511 = t263 - t1128
        t1513 = t4 * t1511 * t8
        t1515 = (t520 - t1513) * t8
        t1519 = t267 - t1132
        t1521 = t4 * t1519 * t8
        t1523 = (t528 - t1521) * t8
        t1529 = t517 + t535 - t1510 - (t4 * (t1515 + t1111 - t188 - t272
     #) * t130 - t4 * (t188 + t272 - t1523 - t1117) * t130) * t130
        t1533 = t517 - t1510
        t1537 = 0.7E1 / 0.5760E4 * t26 * t976
        t1541 = (t579 - t4 * t1223 * t8) * t8
        t1542 = t453 - t1213
        t1546 = (t584 - t4 * t1542 * t8) * t8
        t1550 = t457 - t1217
        t1554 = (t592 - t4 * t1550 * t8) * t8
        t1560 = t581 + t599 - t1541 - (t4 * (t1546 + t1196 - t378 - t462
     #) * t130 - t4 * (t378 + t462 - t1554 - t1202) * t130) * t130
        t1564 = t581 - t1541
        t1569 = t2 + t1056 - t1079 + t1082 - t1150 + t1155 - t1235 + t12
     #43 + t1245 - t1247 - t1249
        t1570 = i - 4
        t1572 = t942 - u(t1570,j,n)
        t1576 = t946 - (t944 - t1572 * t8) * t8
        t1589 = (t969 - t4 * t1572 * t8) * t8
        t1593 = (t973 - (t971 - t1589) * t8) * t8
        t1603 = u(t16,t612,n)
        t1604 = t1603 - t1086
        t1608 = (t1604 * t130 - t1088) * t130 - t1091
        t1615 = u(t16,t625,n)
        t1616 = t1098 - t1615
        t1620 = t1102 - (t1100 - t1616 * t130) * t130
        t1631 = t1095 * t130
        t1634 = t1103 * t130
        t1636 = (t1631 - t1634) * t130
        t1679 = (t958 - t4 * t1576 * t8) * t8
        t1685 = 0.3E1 / 0.640E3 * t26 * (t953 - t4 * (t950 - (t948 - t15
     #76 * t8) * t8) * t8) + 0.3E1 / 0.640E3 * t26 * (t977 - (t975 - t15
     #93) * t8) + t188 - dy * t1106 / 0.24E2 - dy * t1120 / 0.24E2 + t61
     #1 * (((t4 * t1608 * t130 - t1097) * t130 - t1107) * t130 - (t1107 
     #- (t1105 - t4 * t1620 * t130) * t130) * t130) / 0.576E3 + 0.3E1 / 
     #0.640E3 * t611 * (t4 * ((t1608 * t130 - t1631) * t130 - t1636) * t
     #130 - t4 * (t1636 - (t1634 - t1620 * t130) * t130) * t130) + 0.3E1
     # / 0.640E3 * t611 * (((((t4 * t1604 * t130 - t1109) * t130 - t1111
     #) * t130 - t1113) * t130 - t1121) * t130 - (t1121 - (t1119 - (t111
     #7 - (t1115 - t4 * t1616 * t130) * t130) * t130) * t130) * t130) - 
     #dx * t959 / 0.24E2 - dx * t974 / 0.24E2 + t26 * (t962 - (t960 - t1
     #679) * t8) / 0.576E3 + t272
        t1694 = t1062 - ut(t1570,j,n)
        t1698 = t1066 - (t1064 - t1694 * t8) * t8
        t1710 = dx * (t1057 + t82 / 0.2E1 - t25 * (t86 / 0.2E1 + t1068 /
     # 0.2E1) / 0.6E1 + t743 * (t1072 / 0.2E1 + (t1070 - (t1068 - t1698 
     #* t8) * t8) * t8 / 0.2E1) / 0.30E2) / 0.2E1
        t1711 = t378 - t1170 + t462 - t1209
        t1717 = u(t38,t140,n)
        t1718 = t1717 - t1128
        t1720 = t1129 * t130
        t1723 = t1133 * t130
        t1725 = (t1720 - t1723) * t130
        t1729 = u(t38,t153,n)
        t1730 = t1132 - t1729
        t1757 = (t188 - t1085 + t272 - t1124 - t971 + t25 * (t1679 + t15
     #93) / 0.24E2 - t1137 + t139 * ((t4 * ((t1718 * t130 - t1720) * t13
     #0 - t1725) * t130 - t4 * (t1725 - (t1723 - t1730 * t130) * t130) *
     # t130) * t130 + (((t4 * t1718 * t130 - t1131) * t130 - t1137) * t1
     #30 - (t1137 - (t1135 - t4 * t1730 * t130) * t130) * t130) * t130) 
     #/ 0.24E2) * t8
        t1759 = u(t941,t126,n)
        t1763 = u(t941,t132,n)
        t1774 = (t1141 - (t1139 - (t971 + t1137 - t1589 - (t4 * (t1759 -
     # t942) * t130 - t4 * (t942 - t1763) * t130) * t130) * t8) * t8) * 
     #t8
        t1779 = t1127 + t1757 / 0.2E1 - t25 * (t1143 / 0.2E1 + t1774 / 0
     #.2E1) / 0.6E1
        t1786 = t25 * (t84 - dx * t1069 / 0.12E2) / 0.12E2
        t1794 = (t1161 - t4 * t1694 * t8) * t8
        t1802 = ut(t38,t140,n)
        t1803 = t1802 - t1213
        t1805 = t1214 * t130
        t1808 = t1218 * t130
        t1810 = (t1805 - t1808) * t130
        t1814 = ut(t38,t153,n)
        t1815 = t1217 - t1814
        t1864 = t1212 + (t378 - t1170 + t462 - t1209 - t1163 + t25 * ((t
     #1157 - t4 * t1698 * t8) * t8 + (t1165 - (t1163 - t1794) * t8) * t8
     #) / 0.24E2 - t1222 + t139 * ((t4 * ((t1803 * t130 - t1805) * t130 
     #- t1810) * t130 - t4 * (t1810 - (t1808 - t1815 * t130) * t130) * t
     #130) * t130 + (((t4 * t1803 * t130 - t1216) * t130 - t1222) * t130
     # - (t1222 - (t1220 - t4 * t1815 * t130) * t130) * t130) * t130) / 
     #0.24E2) * t8 / 0.2E1 - t25 * (t1228 / 0.2E1 + (t1226 - (t1224 - (t
     #1163 + t1222 - t1794 - (t4 * (ut(t941,t126,n) - t1062) * t130 - t4
     # * (t1062 - ut(t941,t132,n)) * t130) * t130) * t8) * t8) * t8 / 0.
     #2E1) / 0.6E1
        t1869 = t1143 - t1774
        t1872 = (t1126 - t1757) * t8 - dx * t1869 / 0.12E2
        t1878 = t26 * t1069 / 0.720E3
        t1881 = -t62 - dt * t1685 / 0.2E1 - t1710 - t95 * t1711 / 0.8E1 
     #- t766 * t1779 / 0.4E1 - t1786 - t540 * t1864 / 0.16E2 - t473 * t1
     #872 / 0.24E2 - t540 * t1225 / 0.96E2 + t1878 + t608 * t1869 / 0.14
     #40E4
        t1886 = t1474 + t53 * t1479 / 0.2E1 + t96 * t1484 / 0.8E1 - t149
     #3 + t292 * t1496 / 0.48E2 - t473 * t1504 / 0.48E2 + t483 * t1529 *
     # t8 / 0.384E3 - t540 * t1533 / 0.192E3 + t1537 + t547 * t1560 * t8
     # / 0.3840E4 - t604 * t1564 / 0.2304E4 + 0.7E1 / 0.11520E5 * t608 *
     # t1501 + cc * (t1569 + t1881) * t1253 / 0.32E2
        t1889 = dt * t1479
        t1891 = t95 * t1484
        t1894 = t291 * t1496
        t1897 = t25 * t1504
        t1901 = t482 * t1529 * t8
        t1904 = dx * t1533
        t1908 = t546 * t1560 * t8
        t1911 = dx * t1564
        t1914 = t26 * t1501
        t1917 = t2 + t1328 - t1079 + t1330 - t1333 + t1155 - t1336 + t13
     #39 + t1342 - t1247 - t1345
        t1921 = dx * t1779
        t1924 = dx * t1864
        t1927 = t25 * t1872
        t1930 = dx * t1225
        t1933 = t26 * t1869
        t1936 = -t62 - t1261 * t1685 - t1710 - t1292 * t1711 / 0.2E1 - t
     #1261 * t1921 / 0.2E1 - t1786 - t1292 * t1924 / 0.4E1 - t1261 * t19
     #27 / 0.12E2 - t1292 * t1930 / 0.24E2 + t1878 + t1261 * t1933 / 0.7
     #20E3
        t1941 = t1474 + t1270 * t1889 + t1274 * t1891 / 0.2E1 - t1493 + 
     #t1279 * t1894 / 0.6E1 - t1261 * t1897 / 0.24E2 + t1287 * t1901 / 0
     #.24E2 - t1292 * t1904 / 0.48E2 + t1537 + t1297 * t1908 / 0.120E3 -
     # t1302 * t1911 / 0.288E3 + 0.7E1 / 0.5760E4 * t1261 * t1914 + cc *
     # (t1917 + t1936) * t1253 / 0.32E2
        t1961 = t2 + t1400 - t1079 + t1402 - t1404 + t1155 - t1406 + t14
     #08 + t1410 - t1247 - t1412
        t1975 = -t62 - t1266 * t1685 - t1710 - t1374 * t1711 / 0.2E1 - t
     #1266 * t1921 / 0.2E1 - t1786 - t1374 * t1924 / 0.4E1 - t1266 * t19
     #27 / 0.12E2 - t1374 * t1930 / 0.24E2 + t1878 + t1266 * t1933 / 0.7
     #20E3
        t1980 = t1474 + t1358 * t1889 + t1361 * t1891 / 0.2E1 - t1493 + 
     #t1365 * t1894 / 0.6E1 - t1266 * t1897 / 0.24E2 + t1371 * t1901 / 0
     #.24E2 - t1374 * t1904 / 0.48E2 + t1537 + t1378 * t1908 / 0.120E3 -
     # t1381 * t1911 / 0.288E3 + 0.7E1 / 0.5760E4 * t1266 * t1914 + cc *
     # (t1961 + t1975) * t1253 / 0.32E2
        t1983 = t1886 * t1263 * t1268 + t1941 * t1353 * t1356 + t1980 * 
     #t1420 * t1423
        t1987 = t1941 * dt
        t1993 = t1886 * dt
        t1999 = t1980 * dt
        t2005 = (-t1987 / 0.2E1 - t1987 * t1265) * t1353 * t1356 + (-t19
     #93 * t1260 - t1993 * t1265) * t1263 * t1268 + (-t1999 * t1260 - t1
     #999 / 0.2E1) * t1420 * t1423
        t2028 = t4 * (t209 - dy * t215 / 0.24E2 + 0.3E1 / 0.640E3 * t611
     # * t1023)
        t2031 = ut(i,t612,n)
        t2032 = t2031 - t396
        t2033 = t2032 * t130
        t2035 = (t2033 - t398) * t130
        t2036 = t2035 - t401
        t2037 = t2036 * t130
        t2038 = t405 * t130
        t2039 = t2037 - t2038
        t2040 = t2039 * t130
        t2041 = t413 * t130
        t2042 = t2038 - t2041
        t2043 = t2042 * t130
        t2044 = t2040 - t2043
        t2047 = t399 - dy * t405 / 0.24E2 + 0.3E1 / 0.640E3 * t611 * t20
     #44
        t2050 = t490 * t8
        t2051 = t493 * t8
        t2053 = (t2050 - t2051) * t8
        t2054 = t518 * t8
        t2056 = (t2051 - t2054) * t8
        t2057 = t2053 - t2056
        t2059 = t4 * t2057 * t8
        t2060 = t1511 * t8
        t2062 = (t2054 - t2060) * t8
        t2063 = t2056 - t2062
        t2065 = t4 * t2063 * t8
        t2066 = t2059 - t2065
        t2067 = t2066 * t8
        t2069 = (t497 - t522) * t8
        t2071 = (t522 - t1515) * t8
        t2072 = t2069 - t2071
        t2073 = t2072 * t8
        t2076 = t25 * (t2067 + t2073) / 0.24E2
        t2079 = t139 * (t1042 + t994) / 0.24E2
        t2081 = (t522 - t2076 + t231 - t2079 - t118 + t195 - t205 + t244
     #) * t130
        t2082 = t141 - t206
        t2084 = t4 * t2082 * t8
        t2085 = t206 - t1086
        t2087 = t4 * t2085 * t8
        t2089 = (t2084 - t2087) * t8
        t2090 = t2089 + t990 - t522 - t231
        t2091 = t2090 * t130
        t2092 = t523 * t130
        t2094 = (t2091 - t2092) * t130
        t2095 = t531 * t130
        t2097 = (t2092 - t2095) * t130
        t2098 = t2094 - t2097
        t2101 = t2081 - dy * t2098 / 0.24E2
        t2110 = t139 * ((t231 - t2079 - t205 + t244) * t130 - dy * t995 
     #/ 0.24E2) / 0.24E2
        t2112 = t557 * t8
        t2115 = t582 * t8
        t2117 = (t2112 - t2115) * t8
        t2137 = t25 * ((t4 * ((t554 * t8 - t2112) * t8 - t2117) * t8 - t
     #4 * (t2117 - (t2115 - t1542 * t8) * t8) * t8) * t8 + ((t561 - t586
     #) * t8 - (t586 - t1546) * t8) * t8) / 0.24E2
        t2139 = t4 * t2036 * t130
        t2143 = t4 * t2032 * t130
        t2145 = (t2143 - t419) * t130
        t2147 = (t2145 - t421) * t130
        t2149 = (t2147 - t423) * t130
        t2152 = t139 * ((t2139 - t407) * t130 + t2149) / 0.24E2
        t2154 = (t586 - t2137 + t421 - t2152 - t314 + t385 - t395 + t434
     #) * t130
        t2155 = t332 - t396
        t2157 = t4 * t2155 * t8
        t2158 = t396 - t1171
        t2160 = t4 * t2158 * t8
        t2162 = (t2157 - t2160) * t8
        t2163 = t2162 + t2145 - t586 - t421
        t2164 = t2163 * t130
        t2165 = t587 * t130
        t2166 = t2164 - t2165
        t2167 = t2166 * t130
        t2168 = t595 * t130
        t2169 = t2165 - t2168
        t2170 = t2169 * t130
        t2171 = t2167 - t2170
        t2174 = t2154 - dy * t2171 / 0.24E2
        t2177 = dt * t139
        t2180 = t2149 - t431
        t2183 = (t421 - t2152 - t395 + t434) * t130 - dy * t2180 / 0.24E
     #2
        t2197 = (t4 * t2090 * t130 - t525) * t130
        t2198 = (t4 * (t497 + t167 - t522 - t231) * t8 - t4 * (t522 + t2
     #31 - t1515 - t1111) * t8) * t8 + t2197 - t517 - t535
        t2202 = t95 * dy
        t2203 = t2197 - t535
        t2207 = 0.7E1 / 0.5760E4 * t611 * t995
        t2219 = (t4 * t2163 * t130 - t589) * t130
        t2220 = (t4 * (t561 + t357 - t586 - t421) * t8 - t4 * (t586 + t4
     #21 - t1546 - t1196) * t8) * t8 + t2219 - t581 - t599
        t2224 = t291 * dy
        t2225 = t2219 - t599
        t2228 = t611 * dt
        t2231 = j + 4
        t2233 = u(i,t2231,n) - t985
        t2237 = (t4 * t2233 * t130 - t988) * t130
        t2241 = ((t2237 - t990) * t130 - t992) * t130
        t2254 = (t2233 * t130 - t1012) * t130 - t1014
        t2258 = (t4 * t2254 * t130 - t1040) * t130
        t2277 = t813 - t247
        t2281 = (t2277 * t8 - t2050) * t8 - t2053
        t2288 = t1128 - t1759
        t2292 = t2062 - (t2060 - t2288 * t8) * t8
        t2303 = t2057 * t8
        t2306 = t2063 * t8
        t2308 = (t2303 - t2306) * t8
        t2344 = t231 + t522 + 0.3E1 / 0.640E3 * t611 * ((t2241 - t994) *
     # t130 - t996) - dy * t1041 / 0.24E2 - dy * t993 / 0.24E2 + t611 * 
     #((t2258 - t1042) * t130 - t1044) / 0.576E3 + 0.3E1 / 0.640E3 * t61
     #1 * (t4 * ((t2254 * t130 - t1016) * t130 - t1019) * t130 - t1025) 
     #- dx * t2066 / 0.24E2 - dx * t2072 / 0.24E2 + t26 * (((t4 * t2281 
     #* t8 - t2059) * t8 - t2067) * t8 - (t2067 - (t2065 - t4 * t2292 * 
     #t8) * t8) * t8) / 0.576E3 + 0.3E1 / 0.640E3 * t26 * (t4 * ((t2281 
     #* t8 - t2303) * t8 - t2308) * t8 - t4 * (t2308 - (t2306 - t2292 * 
     #t8) * t8) * t8) + 0.3E1 / 0.640E3 * t26 * (((((t4 * t2277 * t8 - t
     #492) * t8 - t497) * t8 - t2069) * t8 - t2073) * t8 - (t2073 - (t20
     #71 - (t1515 - (t1513 - t4 * t2288 * t8) * t8) * t8) * t8) * t8)
        t2348 = t399 / 0.2E1
        t2353 = t139 ** 2
        t2355 = ut(i,t2231,n) - t2031
        t2359 = (t2355 * t130 - t2033) * t130 - t2035
        t2365 = t2044 * t130
        t2372 = dy * (t398 / 0.2E1 + t2348 - t139 * (t2037 / 0.2E1 + t20
     #38 / 0.2E1) / 0.6E1 + t2353 * (((t2359 * t130 - t2037) * t130 - t2
     #040) * t130 / 0.2E1 + t2365 / 0.2E1) / 0.30E2) / 0.2E1
        t2373 = t586 - t2137 + t421 - t2152
        t2376 = dt * dy
        t2377 = t770 - t141
        t2379 = t2082 * t8
        t2382 = t2085 * t8
        t2384 = (t2379 - t2382) * t8
        t2388 = t1086 - t1717
        t2418 = (t2089 - t25 * ((t4 * ((t2377 * t8 - t2379) * t8 - t2384
     #) * t8 - t4 * (t2384 - (t2382 - t2388 * t8) * t8) * t8) * t8 + (((
     #t4 * t2377 * t8 - t2084) * t8 - t2089) * t8 - (t2089 - (t2087 - t4
     # * t2388 * t8) * t8) * t8) * t8) / 0.24E2 + t990 - t139 * (t2258 +
     # t2241) / 0.24E2 - t522 + t2076 - t231 + t2079) * t130
        t2420 = t2081 / 0.2E1
        t2434 = ((((t4 * (t613 - t985) * t8 - t4 * (t985 - t1603) * t8) 
     #* t8 + t2237 - t2089 - t990) * t130 - t2091) * t130 - t2094) * t13
     #0
        t2435 = t2098 * t130
        t2440 = t2418 / 0.2E1 + t2420 - t139 * (t2434 / 0.2E1 + t2435 / 
     #0.2E1) / 0.6E1
        t2447 = t139 * (t401 - dy * t2039 / 0.12E2) / 0.12E2
        t2448 = t857 - t332
        t2450 = t2155 * t8
        t2453 = t2158 * t8
        t2455 = (t2450 - t2453) * t8
        t2459 = t1171 - t1802
        t2492 = (t4 * t2355 * t130 - t2143) * t130
        t2503 = t2154 / 0.2E1
        t2520 = t2171 * t130
        t2525 = (t2162 - t25 * ((t4 * ((t2448 * t8 - t2450) * t8 - t2455
     #) * t8 - t4 * (t2455 - (t2453 - t2459 * t8) * t8) * t8) * t8 + (((
     #t4 * t2448 * t8 - t2157) * t8 - t2162) * t8 - (t2162 - (t2160 - t4
     # * t2459 * t8) * t8) * t8) * t8) / 0.24E2 + t2145 - t139 * ((t4 * 
     #t2359 * t130 - t2139) * t130 + ((t2492 - t2145) * t130 - t2147) * 
     #t130) / 0.24E2 - t586 + t2137 - t421 + t2152) * t130 / 0.2E1 + t25
     #03 - t139 * (((((t4 * (ut(t5,t612,n) - t2031) * t8 - t4 * (t2031 -
     # ut(t16,t612,n)) * t8) * t8 + t2492 - t2162 - t2145) * t130 - t216
     #4) * t130 - t2167) * t130 / 0.2E1 + t2520 / 0.2E1) / 0.6E1
        t2530 = t2434 - t2435
        t2533 = (t2418 - t2081) * t130 - dy * t2530 / 0.12E2
        t2539 = t611 * t2039 / 0.720E3
        t2542 = t386 + dt * t2344 / 0.2E1 - t2372 + t95 * t2373 / 0.8E1 
     #- t2376 * t2440 / 0.4E1 + t2447 - t2202 * t2525 / 0.16E2 + t2177 *
     # t2533 / 0.24E2 + t2202 * t2166 / 0.96E2 - t2539 - t2228 * t2530 /
     # 0.1440E4
        t2543 = t402 / 0.2E1
        t2548 = ut(i,t625,n)
        t2549 = t408 - t2548
        t2550 = t2549 * t130
        t2552 = (t410 - t2550) * t130
        t2553 = t412 - t2552
        t2554 = t2553 * t130
        t2555 = t2041 - t2554
        t2556 = t2555 * t130
        t2557 = t2043 - t2556
        t2558 = t2557 * t130
        t2565 = dy * (t2348 + t2543 - t139 * (t2038 / 0.2E1 + t2041 / 0.
     #2E1) / 0.6E1 + t2353 * (t2365 / 0.2E1 + t2558 / 0.2E1) / 0.30E2) /
     # 0.2E1
        t2566 = t501 * t8
        t2567 = t504 * t8
        t2569 = (t2566 - t2567) * t8
        t2570 = t526 * t8
        t2572 = (t2567 - t2570) * t8
        t2573 = t2569 - t2572
        t2575 = t4 * t2573 * t8
        t2576 = t1519 * t8
        t2578 = (t2570 - t2576) * t8
        t2579 = t2572 - t2578
        t2581 = t4 * t2579 * t8
        t2582 = t2575 - t2581
        t2583 = t2582 * t8
        t2585 = (t508 - t530) * t8
        t2587 = (t530 - t1523) * t8
        t2588 = t2585 - t2587
        t2589 = t2588 * t8
        t2592 = t25 * (t2583 + t2589) / 0.24E2
        t2595 = t139 * (t1048 + t1006) / 0.24E2
        t2597 = (t118 - t195 + t205 - t244 - t530 + t2592 - t237 + t2595
     #) * t130
        t2598 = t2597 / 0.2E1
        t2599 = t154 - t218
        t2601 = t4 * t2599 * t8
        t2602 = t218 - t1098
        t2604 = t4 * t2602 * t8
        t2606 = (t2601 - t2604) * t8
        t2607 = t530 + t237 - t2606 - t1002
        t2608 = t2607 * t130
        t2610 = (t2095 - t2608) * t130
        t2611 = t2097 - t2610
        t2612 = t2611 * t130
        t2617 = t2420 + t2598 - t139 * (t2435 / 0.2E1 + t2612 / 0.2E1) /
     # 0.6E1
        t2619 = t2376 * t2617 / 0.4E1
        t2624 = t139 * (t404 - dy * t2042 / 0.12E2) / 0.12E2
        t2626 = t568 * t8
        t2629 = t590 * t8
        t2631 = (t2626 - t2629) * t8
        t2651 = t25 * ((t4 * ((t565 * t8 - t2626) * t8 - t2631) * t8 - t
     #4 * (t2631 - (t2629 - t1550 * t8) * t8) * t8) * t8 + ((t572 - t594
     #) * t8 - (t594 - t1554) * t8) * t8) / 0.24E2
        t2653 = t4 * t2553 * t130
        t2657 = t4 * t2549 * t130
        t2659 = (t425 - t2657) * t130
        t2661 = (t427 - t2659) * t130
        t2663 = (t429 - t2661) * t130
        t2666 = t139 * ((t415 - t2653) * t130 + t2663) / 0.24E2
        t2668 = (t314 - t385 + t395 - t434 - t594 + t2651 - t427 + t2666
     #) * t130
        t2669 = t2668 / 0.2E1
        t2670 = t344 - t408
        t2672 = t4 * t2670 * t8
        t2673 = t408 - t1183
        t2675 = t4 * t2673 * t8
        t2677 = (t2672 - t2675) * t8
        t2678 = t594 + t427 - t2677 - t2659
        t2679 = t2678 * t130
        t2680 = t2168 - t2679
        t2681 = t2680 * t130
        t2682 = t2170 - t2681
        t2683 = t2682 * t130
        t2688 = t2503 + t2669 - t139 * (t2520 / 0.2E1 + t2683 / 0.2E1) /
     # 0.6E1
        t2690 = t2202 * t2688 / 0.16E2
        t2693 = t2435 - t2612
        t2696 = (t2081 - t2597) * t130 - dy * t2693 / 0.12E2
        t2698 = t2177 * t2696 / 0.24E2
        t2700 = t2202 * t2169 / 0.96E2
        t2702 = t611 * t2042 / 0.720E3
        t2704 = t2228 * t2693 / 0.1440E4
        t2705 = -t2 - t1056 - t2565 - t1082 - t2619 - t2624 - t2690 - t2
     #698 - t2700 + t2702 + t2704
        t2710 = t2028 + t53 * t2047 / 0.2E1 + t96 * t2101 / 0.8E1 - t211
     #0 + t292 * t2174 / 0.48E2 - t2177 * t2183 / 0.48E2 + t483 * t2198 
     #* t130 / 0.384E3 - t2202 * t2203 / 0.192E3 + t2207 + t547 * t2220 
     #* t130 / 0.3840E4 - t2224 * t2225 / 0.2304E4 + 0.7E1 / 0.11520E5 *
     # t2228 * t2180 + cc * (t2542 + t2705) * t1253 / 0.32E2
        t2713 = dt * t2047
        t2715 = t95 * t2101
        t2718 = t291 * t2174
        t2721 = t139 * t2183
        t2725 = t482 * t2198 * t130
        t2728 = dy * t2203
        t2732 = t546 * t2220 * t130
        t2735 = dy * t2225
        t2738 = t611 * t2180
        t2744 = dy * t2440
        t2747 = dy * t2525
        t2750 = t139 * t2533
        t2753 = dy * t2166
        t2756 = t611 * t2530
        t2759 = t386 + t1261 * t2344 - t2372 + t1292 * t2373 / 0.2E1 - t
     #1261 * t2744 / 0.2E1 + t2447 - t1292 * t2747 / 0.4E1 + t1261 * t27
     #50 / 0.12E2 + t1292 * t2753 / 0.24E2 - t2539 - t1261 * t2756 / 0.7
     #20E3
        t2760 = dy * t2617
        t2762 = t1261 * t2760 / 0.2E1
        t2763 = dy * t2688
        t2765 = t1292 * t2763 / 0.4E1
        t2766 = t139 * t2696
        t2768 = t1261 * t2766 / 0.12E2
        t2769 = dy * t2169
        t2771 = t1292 * t2769 / 0.24E2
        t2772 = t611 * t2693
        t2774 = t1261 * t2772 / 0.720E3
        t2775 = -t2 - t1328 - t2565 - t1330 - t2762 - t2624 - t2765 - t2
     #768 - t2771 + t2702 + t2774
        t2780 = t2028 + t1270 * t2713 + t1274 * t2715 / 0.2E1 - t2110 + 
     #t1279 * t2718 / 0.6E1 - t1261 * t2721 / 0.24E2 + t1287 * t2725 / 0
     #.24E2 - t1292 * t2728 / 0.48E2 + t2207 + t1297 * t2732 / 0.120E3 -
     # t1302 * t2735 / 0.288E3 + 0.7E1 / 0.5760E4 * t1261 * t2738 + cc *
     # (t2759 + t2775) * t1253 / 0.32E2
        t2813 = t386 + t1266 * t2344 - t2372 + t1374 * t2373 / 0.2E1 - t
     #1266 * t2744 / 0.2E1 + t2447 - t1374 * t2747 / 0.4E1 + t1266 * t27
     #50 / 0.12E2 + t1374 * t2753 / 0.24E2 - t2539 - t1266 * t2756 / 0.7
     #20E3
        t2815 = t1266 * t2760 / 0.2E1
        t2817 = t1374 * t2763 / 0.4E1
        t2819 = t1266 * t2766 / 0.12E2
        t2821 = t1374 * t2769 / 0.24E2
        t2823 = t1266 * t2772 / 0.720E3
        t2824 = -t2 - t1400 - t2565 - t1402 - t2815 - t2624 - t2817 - t2
     #819 - t2821 + t2702 + t2823
        t2829 = t2028 + t1358 * t2713 + t1361 * t2715 / 0.2E1 - t2110 + 
     #t1365 * t2718 / 0.6E1 - t1266 * t2721 / 0.24E2 + t1371 * t2725 / 0
     #.24E2 - t1374 * t2728 / 0.48E2 + t2207 + t1378 * t2732 / 0.120E3 -
     # t1381 * t2735 / 0.288E3 + 0.7E1 / 0.5760E4 * t1266 * t2738 + cc *
     # (t2813 + t2824) * t1253 / 0.32E2
        t2832 = t2710 * t1263 * t1268 + t2780 * t1353 * t1356 + t2829 * 
     #t1420 * t1423
        t2836 = t2780 * dt
        t2842 = t2710 * dt
        t2848 = t2829 * dt
        t2854 = (-t2836 / 0.2E1 - t2836 * t1265) * t1353 * t1356 + (-t28
     #42 * t1260 - t2842 * t1265) * t1263 * t1268 + (-t2848 * t1260 - t2
     #848 / 0.2E1) * t1420 * t1423
        t2875 = t4 * (t212 - dy * t223 / 0.24E2 + 0.3E1 / 0.640E3 * t611
     # * t1033)
        t2880 = t402 - dy * t413 / 0.24E2 + 0.3E1 / 0.640E3 * t611 * t25
     #57
        t2885 = t2597 - dy * t2611 / 0.24E2
        t2894 = t139 * ((t205 - t244 - t237 + t2595) * t130 - dy * t1007
     # / 0.24E2) / 0.24E2
        t2897 = t2668 - dy * t2682 / 0.24E2
        t2902 = t431 - t2663
        t2905 = (t395 - t434 - t427 + t2666) * t130 - dy * t2902 / 0.24E
     #2
        t2919 = (t533 - t4 * t2607 * t130) * t130
        t2920 = t517 + t535 - (t4 * (t508 + t173 - t530 - t237) * t8 - t
     #4 * (t530 + t237 - t1523 - t1117) * t8) * t8 - t2919
        t2924 = t535 - t2919
        t2928 = 0.7E1 / 0.5760E4 * t611 * t1007
        t2940 = (t597 - t4 * t2678 * t130) * t130
        t2941 = t581 + t599 - (t4 * (t572 + t363 - t594 - t427) * t8 - t
     #4 * (t594 + t427 - t1554 - t1202) * t8) * t8 - t2940
        t2945 = t599 - t2940
        t2950 = t2 + t1056 - t2565 + t1082 - t2619 + t2624 - t2690 + t26
     #98 + t2700 - t2702 - t2704
        t2955 = j - 4
        t2957 = t997 - u(i,t2955,n)
        t2961 = t1028 - (t1026 - t2957 * t130) * t130
        t2974 = (t1046 - t4 * t2961 * t130) * t130
        t2983 = (t1000 - t4 * t2957 * t130) * t130
        t2987 = (t1004 - (t1002 - t2983) * t130) * t130
        t2995 = t817 - t251
        t2999 = (t2995 * t8 - t2566) * t8 - t2569
        t3006 = t1132 - t1763
        t3010 = t2578 - (t2576 - t3006 * t8) * t8
        t3023 = t2573 * t8
        t3026 = t2579 * t8
        t3028 = (t3023 - t3026) * t8
        t3064 = t237 + t530 - dy * t1047 / 0.24E2 - dy * t1005 / 0.24E2 
     #+ 0.3E1 / 0.640E3 * t611 * (t1035 - t4 * (t1032 - (t1030 - t2961 *
     # t130) * t130) * t130) + t611 * (t1050 - (t1048 - t2974) * t130) /
     # 0.576E3 + 0.3E1 / 0.640E3 * t611 * (t1008 - (t1006 - t2987) * t13
     #0) - dx * t2582 / 0.24E2 + t26 * (((t4 * t2999 * t8 - t2575) * t8 
     #- t2583) * t8 - (t2583 - (t2581 - t4 * t3010 * t8) * t8) * t8) / 0
     #.576E3 - dx * t2588 / 0.24E2 + 0.3E1 / 0.640E3 * t26 * (t4 * ((t29
     #99 * t8 - t3023) * t8 - t3028) * t8 - t4 * (t3028 - (t3026 - t3010
     # * t8) * t8) * t8) + 0.3E1 / 0.640E3 * t26 * (((((t4 * t2995 * t8 
     #- t503) * t8 - t508) * t8 - t2585) * t8 - t2589) * t8 - (t2589 - (
     #t2587 - (t1523 - (t1521 - t4 * t3006 * t8) * t8) * t8) * t8) * t8)
        t3073 = t2548 - ut(i,t2955,n)
        t3077 = t2552 - (t2550 - t3073 * t130) * t130
        t3089 = dy * (t2543 + t410 / 0.2E1 - t139 * (t2041 / 0.2E1 + t25
     #54 / 0.2E1) / 0.6E1 + t2353 * (t2558 / 0.2E1 + (t2556 - (t2554 - t
     #3077 * t130) * t130) * t130 / 0.2E1) / 0.30E2) / 0.2E1
        t3090 = t594 - t2651 + t427 - t2666
        t3093 = t782 - t154
        t3095 = t2599 * t8
        t3098 = t2602 * t8
        t3100 = (t3095 - t3098) * t8
        t3104 = t1098 - t1729
        t3134 = (t530 - t2592 + t237 - t2595 - t2606 + t25 * ((t4 * ((t3
     #093 * t8 - t3095) * t8 - t3100) * t8 - t4 * (t3100 - (t3098 - t310
     #4 * t8) * t8) * t8) * t8 + (((t4 * t3093 * t8 - t2601) * t8 - t260
     #6) * t8 - (t2606 - (t2604 - t4 * t3104 * t8) * t8) * t8) * t8) / 0
     #.24E2 - t1002 + t139 * (t2974 + t2987) / 0.24E2) * t130
        t3149 = (t2610 - (t2608 - (t2606 + t1002 - (t4 * (t626 - t997) *
     # t8 - t4 * (t997 - t1615) * t8) * t8 - t2983) * t130) * t130) * t1
     #30
        t3154 = t2598 + t3134 / 0.2E1 - t139 * (t2612 / 0.2E1 + t3149 / 
     #0.2E1) / 0.6E1
        t3161 = t139 * (t412 - dy * t2555 / 0.12E2) / 0.12E2
        t3162 = t869 - t344
        t3164 = t2670 * t8
        t3167 = t2673 * t8
        t3169 = (t3164 - t3167) * t8
        t3173 = t1183 - t1814
        t3206 = (t2657 - t4 * t3073 * t130) * t130
        t3237 = t2669 + (t594 - t2651 + t427 - t2666 - t2677 + t25 * ((t
     #4 * ((t3162 * t8 - t3164) * t8 - t3169) * t8 - t4 * (t3169 - (t316
     #7 - t3173 * t8) * t8) * t8) * t8 + (((t4 * t3162 * t8 - t2672) * t
     #8 - t2677) * t8 - (t2677 - (t2675 - t4 * t3173 * t8) * t8) * t8) *
     # t8) / 0.24E2 - t2659 + t139 * ((t2653 - t4 * t3077 * t130) * t130
     # + (t2661 - (t2659 - t3206) * t130) * t130) / 0.24E2) * t130 / 0.2
     #E1 - t139 * (t2683 / 0.2E1 + (t2681 - (t2679 - (t2677 + t2659 - (t
     #4 * (ut(t5,t625,n) - t2548) * t8 - t4 * (t2548 - ut(t16,t625,n)) *
     # t8) * t8 - t3206) * t130) * t130) * t130 / 0.2E1) / 0.6E1
        t3242 = t2612 - t3149
        t3245 = (t2597 - t3134) * t130 - dy * t3242 / 0.12E2
        t3251 = t611 * t2555 / 0.720E3
        t3254 = -t390 - dt * t3064 / 0.2E1 - t3089 - t95 * t3090 / 0.8E1
     # - t2376 * t3154 / 0.4E1 - t3161 - t2202 * t3237 / 0.16E2 - t2177 
     #* t3245 / 0.24E2 - t2202 * t2680 / 0.96E2 + t3251 + t2228 * t3242 
     #/ 0.1440E4
        t3259 = t2875 + t53 * t2880 / 0.2E1 + t96 * t2885 / 0.8E1 - t289
     #4 + t292 * t2897 / 0.48E2 - t2177 * t2905 / 0.48E2 + t483 * t2920 
     #* t130 / 0.384E3 - t2202 * t2924 / 0.192E3 + t2928 + t547 * t2941 
     #* t130 / 0.3840E4 - t2224 * t2945 / 0.2304E4 + 0.7E1 / 0.11520E5 *
     # t2228 * t2902 + cc * (t2950 + t3254) * t1253 / 0.32E2
        t3262 = dt * t2880
        t3264 = t95 * t2885
        t3267 = t291 * t2897
        t3270 = t139 * t2905
        t3274 = t482 * t2920 * t130
        t3277 = t2924 * dy
        t3281 = t546 * t2941 * t130
        t3284 = dy * t2945
        t3287 = t611 * t2902
        t3290 = t2 + t1328 - t2565 + t1330 - t2762 + t2624 - t2765 + t27
     #68 + t2771 - t2702 - t2774
        t3294 = dy * t3154
        t3297 = dy * t3237
        t3300 = t139 * t3245
        t3303 = dy * t2680
        t3306 = t611 * t3242
        t3309 = -t390 - t1261 * t3064 - t3089 - t1292 * t3090 / 0.2E1 - 
     #t1261 * t3294 / 0.2E1 - t3161 - t1292 * t3297 / 0.4E1 - t1261 * t3
     #300 / 0.12E2 - t1292 * t3303 / 0.24E2 + t3251 + t1261 * t3306 / 0.
     #720E3
        t3314 = t2875 + t1270 * t3262 + t1274 * t3264 / 0.2E1 - t2894 + 
     #t1279 * t3267 / 0.6E1 - t1261 * t3270 / 0.24E2 + t1287 * t3274 / 0
     #.24E2 - t1292 * t3277 / 0.48E2 + t2928 + t1297 * t3281 / 0.120E3 -
     # t1302 * t3284 / 0.288E3 + 0.7E1 / 0.5760E4 * t1261 * t3287 + cc *
     # (t3290 + t3309) * t1253 / 0.32E2
        t3334 = t2 + t1400 - t2565 + t1402 - t2815 + t2624 - t2817 + t28
     #19 + t2821 - t2702 - t2823
        t3348 = -t390 - t1266 * t3064 - t3089 - t1374 * t3090 / 0.2E1 - 
     #t1266 * t3294 / 0.2E1 - t3161 - t1374 * t3297 / 0.4E1 - t1266 * t3
     #300 / 0.12E2 - t1374 * t3303 / 0.24E2 + t3251 + t1266 * t3306 / 0.
     #720E3
        t3353 = t2875 + t1358 * t3262 + t1361 * t3264 / 0.2E1 - t2894 + 
     #t1365 * t3267 / 0.6E1 - t1266 * t3270 / 0.24E2 + t1371 * t3274 / 0
     #.24E2 - t1374 * t3277 / 0.48E2 + t2928 + t1378 * t3281 / 0.120E3 -
     # t1381 * t3284 / 0.288E3 + 0.7E1 / 0.5760E4 * t1266 * t3287 + cc *
     # (t3334 + t3348) * t1253 / 0.32E2
        t3356 = t3259 * t1263 * t1268 + t3314 * t1353 * t1356 + t3353 * 
     #t1420 * t1423
        t3360 = t3314 * dt
        t3366 = t3259 * dt
        t3372 = t3353 * dt
        t3378 = (-t3360 / 0.2E1 - t3360 * t1265) * t1353 * t1356 + (-t33
     #66 * t1260 - t3366 * t1265) * t1263 * t1268 + (-t3372 * t1260 - t3
     #372 / 0.2E1) * t1420 * t1423
        t3320 = t1260 * t1265 * t1263 * t1268

        unew(i,j) = t1 + dt * t2 + (t1425 * t482 / 0.12E2 + t1447 *
     # t291 / 0.6E1 + (t1351 * t95 * t1453 / 0.2E1 + t1418 * t95 * t1458
     # / 0.2E1 + t1256 * t95 * t3320) * t95 / 0.2E1 - t1983 * t482 / 0.1
     #2E2 - t2005 * t291 / 0.6E1 - (t1941 * t95 * t1453 / 0.2E1 + t1980 
     #* t95 * t1458 / 0.2E1 + t1886 * t95 * t3320) * t95 / 0.2E1) * t8 +
     # (t2832 * t482 / 0.12E2 + t2854 * t291 / 0.6E1 + (t2780 * t95 * t1
     #453 / 0.2E1 + t2829 * t95 * t1458 / 0.2E1 + t2710 * t95 * t3320) *
     # t95 / 0.2E1 - t3356 * t482 / 0.12E2 - t3378 * t291 / 0.6E1 - (t33
     #14 * t95 * t1453 / 0.2E1 + t3353 * t95 * t1458 / 0.2E1 + t3259 * t
     #95 * t3320) * t95 / 0.2E1) * t130

        utnew(i,j) = t2 + (t1425 * t291 / 0.3E1 
     #+ t1447 * t95 / 0.2E1 + t1351 * t291 * t1453 / 0.2E1 + t1418 * t29
     #1 * t1458 / 0.2E1 + t1256 * t291 * t3320 - t1983 * t291 / 0.3E1 - 
     #t2005 * t95 / 0.2E1 - t1941 * t291 * t1453 / 0.2E1 - t1980 * t291 
     #* t1458 / 0.2E1 - t1886 * t291 * t3320) * t8 + (t2832 * t291 / 0.3
     #E1 + t2854 * t95 / 0.2E1 + t2780 * t291 * t1453 / 0.2E1 + t2829 * 
     #t291 * t1458 / 0.2E1 + t2710 * t291 * t3320 - t3356 * t291 / 0.3E1
     # - t3378 * t95 / 0.2E1 - t3314 * t291 * t1453 / 0.2E1 - t3353 * t2
     #91 * t1458 / 0.2E1 - t3259 * t291 * t3320) * t130

c        blah = array(int(t1 + dt * t2 + (t1425 * t482 / 0.12E2 + t1447 *
c     # t291 / 0.6E1 + (t1351 * t95 * t1453 / 0.2E1 + t1418 * t95 * t1458
c     # / 0.2E1 + t1256 * t95 * t3320) * t95 / 0.2E1 - t1983 * t482 / 0.1
c     #2E2 - t2005 * t291 / 0.6E1 - (t1941 * t95 * t1453 / 0.2E1 + t1980 
c     #* t95 * t1458 / 0.2E1 + t1886 * t95 * t3320) * t95 / 0.2E1) * t8 +
c     # (t2832 * t482 / 0.12E2 + t2854 * t291 / 0.6E1 + (t2780 * t95 * t1
c     #453 / 0.2E1 + t2829 * t95 * t1458 / 0.2E1 + t2710 * t95 * t3320) *
c     # t95 / 0.2E1 - t3356 * t482 / 0.12E2 - t3378 * t291 / 0.6E1 - (t33
c     #14 * t95 * t1453 / 0.2E1 + t3353 * t95 * t1458 / 0.2E1 + t3259 * t
c     #95 * t3320) * t95 / 0.2E1) * t130),int(t2 + (t1425 * t291 / 0.3E1 
c     #+ t1447 * t95 / 0.2E1 + t1351 * t291 * t1453 / 0.2E1 + t1418 * t29
c     #1 * t1458 / 0.2E1 + t1256 * t291 * t3320 - t1983 * t291 / 0.3E1 - 
c     #t2005 * t95 / 0.2E1 - t1941 * t291 * t1453 / 0.2E1 - t1980 * t291 
c     #* t1458 / 0.2E1 - t1886 * t291 * t3320) * t8 + (t2832 * t291 / 0.3
c     #E1 + t2854 * t95 / 0.2E1 + t2780 * t291 * t1453 / 0.2E1 + t2829 * 
c     #t291 * t1458 / 0.2E1 + t2710 * t291 * t3320 - t3356 * t291 / 0.3E1
c     # - t3378 * t95 / 0.2E1 - t3314 * t291 * t1453 / 0.2E1 - t3353 * t2
c     #91 * t1458 / 0.2E1 - t3259 * t291 * t3320) * t130))

        return
      end
