      subroutine duStepWaveGen3d4rc_tz( 
     *   nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *   n1a,n1b,n2a,n2b,n3a,n3b,
     *   ndf4a,ndf4b,nComp,
     *   u,ut,unew,utnew,
     *   src,
     *   dx,dy,dz,dt,cc,beta,
     *   i,j,k,n )

      implicit none
c
c.. declarations of incoming variables      
      integer nd1a,nd1b,nd2a,nd2b,nd3a,nd3b
      integer n1a,n1b,n2a,n2b,n3a,n3b
      integer ndf4a,ndf4b,nComp
      integer i,j,k,n

      real u    (nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,*)
      real ut   (nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,*)
      real unew (nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real utnew(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real src  (nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,ndf4a:ndf4b,0:*)
      real dx,dy,dz,dt,cc,beta
c
c.. generated code to follow
c
        real t1
        real t10
        real t1006
        real t1010
        real t1016
        real t1020
        real t1029
        real t1031
        real t1033
        real t1035
        real t1037
        real t1039
        real t1041
        real t1043
        real t1045
        real t1047
        real t1048
        real t105
        real t1051
        real t106
        real t1062
        real t1064
        real t1067
        real t1069
        real t1070
        real t1071
        real t1072
        real t1074
        real t1075
        real t1077
        real t1079
        real t108
        real t1080
        real t1082
        real t1084
        real t1086
        real t1087
        real t1089
        real t109
        real t1091
        real t1093
        real t1099
        integer t11
        real t110
        real t1100
        real t1101
        real t1108
        real t1115
        real t112
        real t1120
        real t1121
        real t1129
        real t1131
        real t1135
        real t1136
        real t114
        integer t1140
        real t1141
        real t1146
        real t1147
        real t115
        real t1151
        real t1157
        real t116
        real t1161
        real t1167
        real t1168
        real t118
        real t1182
        real t119
        real t1190
        real t1191
        real t1195
        real t12
        real t120
        real t1200
        real t1201
        real t1205
        real t1208
        real t1210
        real t1212
        real t1214
        real t1216
        real t122
        real t1221
        real t1223
        real t1226
        real t1228
        real t1229
        real t124
        real t1245
        real t1248
        real t1251
        real t1253
        real t1257
        real t126
        real t1270
        real t1276
        real t128
        real t1284
        real t1285
        real t1287
        real t129
        real t1290
        real t1292
        real t1296
        real t1297
        real t13
        real t1309
        real t1315
        real t132
        real t1323
        real t1324
        real t1326
        real t133
        real t1331
        real t1342
        real t1345
        real t1348
        real t135
        real t1350
        real t1354
        real t138
        real t1381
        real t1382
        real t1384
        real t1387
        real t1389
        real t139
        real t1393
        real t1394
        real t1404
        real t141
        real t1414
        real t142
        real t1421
        real t1423
        real t1426
        real t1427
        real t1428
        real t143
        real t1438
        real t1439
        real t1441
        real t1443
        real t1448
        real t145
        real t1456
        real t1457
        real t1461
        real t1466
        real t147
        real t1476
        real t148
        real t1484
        real t149
        real t1492
        real t15
        real t1500
        real t151
        real t1511
        real t1515
        real t152
        real t1521
        real t1525
        real t153
        real t1534
        real t1536
        real t1538
        real t1540
        real t155
        real t1556
        real t1558
        real t1561
        real t1563
        real t1564
        real t1566
        real t157
        real t1576
        real t1583
        real t1589
        real t159
        real t1591
        real t1595
        real t1597
        real t1598
        real t1599
        real t16
        real t1601
        real t1605
        real t1607
        real t1609
        real t161
        real t1611
        real t1613
        real t1615
        real t1617
        real t162
        real t1620
        real t1622
        real t1623
        real t1625
        real t1626
        real t1628
        real t1630
        real t1631
        real t1632
        real t1634
        real t1635
        real t1636
        real t1638
        real t1640
        real t1643
        real t1644
        real t1647
        real t1648
        real t165
        real t1651
        real t1653
        real t1655
        real t1658
        real t166
        real t1660
        real t168
        integer t1681
        real t1695
        real t17
        real t170
        real t1703
        real t1704
        real t1706
        real t1709
        real t171
        real t1710
        real t1711
        real t1715
        real t1716
        real t1717
        real t1723
        real t1728
        real t173
        real t1734
        real t1743
        real t1745
        real t1748
        real t1751
        real t1753
        real t176
        real t177
        real t1783
        real t179
        real t1796
        real t1804
        real t1805
        real t1807
        real t1810
        real t1812
        real t1816
        real t1817
        real t183
        real t1839
        real t184
        real t1844
        real t1846
        real t1851
        real t1854
        real t1855
        real t1859
        real t186
        real t1864
        real t1865
        real t1869
        real t1872
        real t1874
        real t188
        real t189
        real t1895
        real t19
        real t1900
        real t1911
        real t1916
        real t193
        real t1935
        real t1939
        real t1948
        real t1950
        real t1951
        real t1960
        real t1964
        real t198
        real t1981
        real t1985
        real t1988
        real t199
        real t1990
        real t1992
        real t1994
        real t1997
        real t1999
        real t2
        real t2000
        real t2002
        real t2003
        real t2005
        real t2007
        real t2008
        real t2009
        real t2011
        real t2012
        real t2013
        real t2015
        real t2017
        real t2020
        real t2021
        real t2024
        real t2025
        real t2027
        real t2029
        real t203
        real t2032
        real t2034
        real t2036
        real t2040
        real t2042
        real t2045
        real t2047
        real t2048
        real t2066
        real t2068
        real t2069
        real t2071
        real t2075
        real t2076
        real t2078
        real t208
        real t2080
        real t2082
        real t2089
        real t2090
        real t2092
        real t2094
        real t2096
        real t2098
        real t21
        real t2102
        real t2104
        real t211
        real t2111
        real t2113
        real t2117
        real t2118
        real t2120
        real t2122
        real t2124
        real t213
        real t2130
        real t2131
        real t2138
        real t2139
        real t2140
        real t2141
        real t2147
        real t215
        real t2152
        real t2156
        real t2159
        real t216
        real t2161
        real t217
        integer t2182
        real t219
        real t2191
        real t2195
        real t2196
        integer t22
        real t220
        real t2202
        real t2204
        real t2205
        real t2207
        real t221
        real t2210
        real t2212
        real t2216
        real t2217
        real t2229
        real t223
        real t2235
        real t2244
        real t2249
        real t225
        real t2252
        real t2254
        real t226
        real t227
        real t2284
        real t229
        real t2297
        real t23
        real t230
        real t2305
        real t2306
        real t2308
        real t231
        real t2311
        real t2313
        real t2317
        real t2318
        real t2322
        real t233
        real t2332
        real t2345
        real t235
        real t2355
        real t2356
        real t2360
        real t2365
        real t2366
        real t237
        real t2370
        real t2373
        real t239
        real t2396
        real t24
        real t240
        real t2401
        real t241
        real t2412
        real t2417
        real t243
        real t2436
        real t2440
        real t2449
        real t245
        real t246
        real t2460
        real t2464
        real t247
        real t2484
        real t2488
        real t249
        real t2492
        real t2494
        real t2496
        real t2498
        real t250
        real t2500
        real t2502
        real t2504
        real t2506
        real t2507
        real t251
        real t2511
        real t2512
        real t2513
        real t2515
        real t2519
        real t2521
        real t2524
        real t2525
        real t2527
        real t253
        real t2535
        real t2542
        real t2546
        real t255
        real t2552
        real t2554
        real t256
        real t2560
        real t2561
        real t2563
        real t2564
        real t2566
        real t2568
        real t2569
        real t257
        real t2571
        real t2572
        real t2574
        real t2576
        real t2579
        real t2580
        real t2583
        real t2584
        real t2586
        real t2588
        real t2589
        real t259
        real t2591
        real t2592
        real t2594
        real t2596
        real t2597
        real t2599
        real t26
        real t260
        real t2600
        real t2602
        real t2604
        real t2607
        real t2608
        real t261
        real t2611
        real t2612
        real t2614
        real t2616
        real t2618
        real t2619
        real t2622
        real t2624
        real t2626
        real t2628
        real t263
        real t2630
        real t2631
        real t2632
        real t2637
        real t2639
        real t2642
        real t2644
        real t2646
        real t2649
        real t265
        real t2651
        real t2665
        real t267
        real t2671
        real t2673
        real t2676
        real t2678
        real t269
        real t2697
        integer t2699
        real t2701
        real t2705
        real t271
        real t2713
        real t2722
        real t2724
        real t2727
        real t2730
        real t2732
        real t274
        real t275
        real t276
        real t2764
        real t2767
        real t2769
        real t277
        real t279
        real t2799
        real t28
        real t280
        real t2800
        real t2812
        real t282
        real t2821
        real t2823
        real t283
        real t2831
        real t2839
        real t2840
        real t2844
        real t2847
        real t2849
        real t285
        integer t286
        real t287
        real t288
        real t289
        real t2895
        real t2897
        real t29
        real t291
        real t292
        real t2928
        real t2930
        real t2932
        real t2933
        real t2935
        real t2937
        real t2939
        real t294
        real t2948
        real t2957
        real t2961
        real t2963
        real t2964
        real t2966
        real t2968
        real t297
        real t2971
        real t2972
        real t2973
        real t2974
        real t2977
        real t2979
        real t298
        real t2982
        real t2984
        real t2986
        real t2987
        real t2988
        real t2990
        real t2993
        real t2994
        real t30
        real t300
        real t3001
        real t3003
        real t3004
        real t3006
        real t3008
        real t3010
        real t3013
        real t3017
        real t3018
        real t3019
        real t302
        real t3021
        real t3023
        real t3024
        real t3025
        real t303
        real t3032
        real t3036
        real t3038
        real t304
        real t3044
        real t3048
        real t3049
        real t3055
        real t3058
        real t3060
        real t3092
        real t3095
        real t3097
        real t310
        integer t311
        real t312
        integer t3127
        real t3128
        real t3129
        real t313
        real t314
        real t3141
        real t315
        real t3150
        real t3152
        real t3160
        real t3168
        real t317
        real t3170
        real t3174
        real t3175
        real t3179
        real t318
        real t3182
        real t3184
        real t32
        real t320
        real t321
        real t323
        real t3230
        real t3232
        integer t324
        real t325
        real t326
        real t3263
        real t3265
        real t3267
        real t3269
        real t327
        real t3272
        real t3277
        real t3280
        real t3283
        real t3287
        real t329
        real t3290
        real t3292
        real t33
        real t330
        real t3314
        real t3317
        real t3319
        real t332
        real t3356
        real t336
        real t3370
        real t3371
        real t3373
        real t3375
        real t3377
        real t3379
        real t338
        real t3381
        real t3383
        real t3389
        real t339
        real t3391
        real t3394
        real t3396
        real t3397
        real t3399
        real t34
        real t340
        real t3409
        real t3416
        real t3417
        real t3418
        real t342
        real t3420
        real t3426
        real t3431
        real t3433
        real t3434
        real t344
        real t3443
        real t3445
        real t345
        real t3455
        real t3456
        real t346
        real t3462
        real t3463
        real t3471
        real t3472
        real t3478
        real t3479
        real t3487
        real t3488
        real t3494
        real t3495
        real t35
        real t352
        integer t353
        real t354
        real t355
        real t356
        real t357
        real t359
        real t36
        real t360
        real t362
        real t363
        real t365
        integer t366
        real t367
        real t368
        real t369
        real t371
        real t372
        real t374
        real t378
        real t38
        real t380
        real t381
        real t382
        real t384
        real t386
        real t387
        real t388
        real t39
        real t395
        real t397
        real t398
        real t4
        real t400
        real t401
        real t402
        real t403
        real t405
        real t406
        real t408
        real t41
        real t411
        real t413
        real t415
        real t416
        real t417
        real t42
        real t423
        real t424
        real t425
        real t426
        real t428
        real t429
        real t431
        real t432
        real t434
        real t435
        real t436
        real t437
        real t439
        real t440
        real t442
        real t446
        real t448
        real t449
        real t45
        real t450
        real t452
        real t454
        real t455
        real t456
        real t46
        real t462
        real t463
        real t464
        real t465
        real t467
        real t468
        real t470
        real t471
        real t473
        real t474
        real t475
        real t476
        real t478
        real t479
        real t48
        real t481
        real t485
        real t487
        real t488
        real t489
        real t491
        real t493
        real t494
        real t495
        real t5
        real t502
        real t504
        real t505
        real t506
        real t508
        real t509
        real t51
        real t510
        real t512
        real t514
        real t515
        real t516
        real t518
        real t519
        real t52
        real t520
        real t522
        real t524
        real t525
        real t527
        real t529
        real t53
        real t532
        real t534
        real t535
        real t536
        real t537
        real t538
        real t539
        real t54
        real t542
        real t545
        real t548
        real t55
        real t550
        real t551
        real t553
        real t555
        real t556
        real t557
        real t559
        real t56
        real t560
        real t561
        real t563
        real t565
        real t568
        real t569
        real t57
        real t571
        real t572
        real t574
        real t576
        real t577
        real t578
        integer t58
        real t580
        real t581
        real t582
        real t584
        real t586
        real t589
        real t59
        real t592
        real t594
        real t595
        real t597
        real t599
        real t6
        real t600
        real t602
        real t603
        real t605
        real t607
        real t610
        real t611
        real t613
        real t614
        real t616
        real t618
        real t619
        real t621
        real t622
        real t624
        real t626
        real t629
        real t634
        real t637
        real t64
        real t640
        real t643
        real t644
        real t647
        integer t65
        real t650
        real t653
        real t654
        real t657
        real t66
        real t663
        real t665
        real t666
        real t667
        real t669
        real t670
        real t671
        real t673
        real t675
        real t676
        real t677
        real t679
        real t680
        real t681
        real t683
        real t685
        real t688
        real t689
        real t69
        real t692
        real t693
        real t695
        real t697
        real t7
        real t700
        real t702
        real t704
        real t705
        real t706
        real t707
        real t708
        integer t71
        real t711
        real t712
        real t713
        real t714
        real t715
        real t718
        real t72
        real t720
        real t721
        real t737
        real t740
        real t743
        real t745
        real t749
        real t762
        real t768
        real t776
        real t777
        real t779
        integer t78
        real t782
        real t784
        real t788
        real t789
        real t79
        real t8
        real t801
        real t807
        real t816
        real t818
        real t819
        real t82
        real t835
        real t836
        real t838
        integer t84
        real t841
        real t843
        real t844
        real t847
        real t848
        real t85
        real t874
        real t875
        real t877
        real t879
        real t880
        real t882
        real t886
        real t887
        integer t9
        integer t91
        real t911
        real t914
        real t916
        real t917
        real t920
        real t922
        real t923
        real t929
        real t93
        real t933
        real t934
        real t938
        real t943
        real t947
        real t95
        real t951
        real t952
        real t956
        real t961
        real t967
        real t971
        real t979
        integer t98
        real t983
        real t987
        real t995
        t1 = u(i,j,k,n)
        t2 = ut(i,j,k,n)
        t4 = sqrt(0.3E1)
        t5 = t4 / 0.6E1
        t6 = 0.1E1 / 0.2E1 - t5
        t7 = t6 * dt
        t8 = cc ** 2
        t9 = i + 2
        t10 = ut(t9,j,k,n)
        t11 = i + 1
        t12 = ut(t11,j,k,n)
        t13 = t10 - t12
        t15 = 0.1E1 / dx
        t16 = t8 * t13 * t15
        t17 = t12 - t2
        t19 = t8 * t17 * t15
        t21 = (t16 - t19) * t15
        t22 = i - 1
        t23 = ut(t22,j,k,n)
        t24 = t2 - t23
        t26 = t8 * t24 * t15
        t28 = (t19 - t26) * t15
        t29 = t21 - t28
        t30 = dx * t29
        t32 = t7 * t30 / 0.24E2
        t33 = 0.1E1 / 0.2E1 + t5
        t34 = t8 * t33
        t35 = t17 * t15
        t36 = t13 * t15
        t38 = (t36 - t35) * t15
        t39 = t24 * t15
        t41 = (t35 - t39) * t15
        t42 = t38 - t41
        t46 = dt * (t35 - dx * t42 / 0.24E2)
        t48 = t33 * dt
        t51 = t8 * t6
        t52 = t51 * t46
        t53 = beta ** 2
        t54 = t33 ** 2
        t55 = t53 * t54
        t56 = dt ** 2
        t57 = t56 * dx
        t58 = i + 3
        t59 = ut(t58,j,k,n)
        t45 = t15 * (t59 - t10)
        t64 = (t45 * t8 - t16) * t15
        t65 = j + 1
        t66 = ut(t9,t65,k,n)
        t69 = 0.1E1 / dy
        t71 = j - 1
        t72 = ut(t9,t71,k,n)
        t78 = k + 1
        t79 = ut(t9,j,t78,n)
        t82 = 0.1E1 / dz
        t84 = k - 1
        t85 = ut(t9,j,t84,n)
        t91 = n + 1
        t93 = src(t9,j,k,nComp,n)
        t95 = 0.1E1 / dt
        t98 = n - 1
        t105 = ut(t11,t65,k,n)
        t106 = t105 - t12
        t108 = t8 * t106 * t69
        t109 = ut(t11,t71,k,n)
        t110 = t12 - t109
        t112 = t8 * t110 * t69
        t114 = (t108 - t112) * t69
        t115 = ut(t11,j,t78,n)
        t116 = t115 - t12
        t118 = t8 * t116 * t82
        t119 = ut(t11,j,t84,n)
        t120 = t12 - t119
        t122 = t8 * t120 * t82
        t124 = (t118 - t122) * t82
        t126 = src(t11,j,k,nComp,n)
        t128 = (src(t11,j,k,nComp,t91) - t126) * t95
        t129 = t128 / 0.2E1
        t132 = (t126 - src(t11,j,k,nComp,t98)) * t95
        t133 = t132 / 0.2E1
        t135 = cc * (t21 + t114 + t124 + t129 + t133)
        t138 = ut(i,t65,k,n)
        t139 = t138 - t2
        t141 = t8 * t139 * t69
        t142 = ut(i,t71,k,n)
        t143 = t2 - t142
        t145 = t8 * t143 * t69
        t147 = (t141 - t145) * t69
        t148 = ut(i,j,t78,n)
        t149 = t148 - t2
        t151 = t8 * t149 * t82
        t152 = ut(i,j,t84,n)
        t153 = t2 - t152
        t155 = t8 * t153 * t82
        t157 = (t151 - t155) * t82
        t159 = src(i,j,k,nComp,n)
        t161 = (src(i,j,k,nComp,t91) - t159) * t95
        t162 = t161 / 0.2E1
        t165 = (t159 - src(i,j,k,nComp,t98)) * t95
        t166 = t165 / 0.2E1
        t168 = cc * (t28 + t147 + t157 + t162 + t166)
        t170 = (t135 - t168) * t15
        t173 = t57 * ((cc * (t64 + (t8 * (t66 - t10) * t69 - t8 * (t10 -
     # t72) * t69) * t69 + (t8 * (t79 - t10) * t82 - t8 * (t10 - t85) * 
     #t82) * t82 + (src(t9,j,k,nComp,t91) - t93) * t95 / 0.2E1 + (t93 - 
     #src(t9,j,k,nComp,t98)) * t95 / 0.2E1) - t135) * t15 / 0.2E1 + t170
     # / 0.2E1)
        t176 = beta * t33
        t177 = dt * dx
        t179 = u(t9,j,k,n)
        t183 = u(t11,j,k,n)
        t184 = t179 - t183
        t186 = t8 * t184 * t15
        t171 = t15 * (u(t58,j,k,n) - t179)
        t188 = (t171 * t8 - t186) * t15
        t189 = u(t9,t65,k,n)
        t193 = u(t9,t71,k,n)
        t198 = (t8 * (t189 - t179) * t69 - t8 * (t179 - t193) * t69) * t
     #69
        t199 = u(t9,j,t78,n)
        t203 = u(t9,j,t84,n)
        t208 = (t8 * (t199 - t179) * t82 - t8 * (t179 - t203) * t82) * t
     #82
        t211 = t183 - t1
        t213 = t8 * t211 * t15
        t215 = (t186 - t213) * t15
        t216 = u(t11,t65,k,n)
        t217 = t216 - t183
        t219 = t8 * t217 * t69
        t220 = u(t11,t71,k,n)
        t221 = t183 - t220
        t223 = t8 * t221 * t69
        t225 = (t219 - t223) * t69
        t226 = u(t11,j,t78,n)
        t227 = t226 - t183
        t229 = t8 * t227 * t82
        t230 = u(t11,j,t84,n)
        t231 = t183 - t230
        t233 = t8 * t231 * t82
        t235 = (t229 - t233) * t82
        t237 = cc * (t215 + t225 + t235 + t126)
        t239 = (cc * (t188 + t198 + t208 + t93) - t237) * t15
        t240 = u(t22,j,k,n)
        t241 = t1 - t240
        t243 = t8 * t241 * t15
        t245 = (t213 - t243) * t15
        t246 = u(i,t65,k,n)
        t247 = t246 - t1
        t249 = t8 * t247 * t69
        t250 = u(i,t71,k,n)
        t251 = t1 - t250
        t253 = t8 * t251 * t69
        t255 = (t249 - t253) * t69
        t256 = u(i,j,t78,n)
        t257 = t256 - t1
        t259 = t8 * t257 * t82
        t260 = u(i,j,t84,n)
        t261 = t1 - t260
        t263 = t8 * t261 * t82
        t265 = (t259 - t263) * t82
        t267 = cc * (t245 + t255 + t265 + t159)
        t269 = (t237 - t267) * t15
        t271 = t177 * (t239 - t269)
        t274 = dt * cc
        t275 = dx ** 2
        t276 = t184 * t15
        t277 = t211 * t15
        t279 = (t276 - t277) * t15
        t280 = t241 * t15
        t282 = (t277 - t280) * t15
        t283 = t279 - t282
        t285 = t8 * t283 * t15
        t286 = i - 2
        t287 = u(t286,j,k,n)
        t288 = t240 - t287
        t289 = t288 * t15
        t291 = (t280 - t289) * t15
        t292 = t282 - t291
        t294 = t8 * t292 * t15
        t297 = t215 - t245
        t298 = t297 * t15
        t300 = t8 * t288 * t15
        t302 = (t243 - t300) * t15
        t303 = t245 - t302
        t304 = t303 * t15
        t310 = dy ** 2
        t311 = j + 2
        t312 = u(i,t311,k,n)
        t313 = t312 - t246
        t314 = t313 * t69
        t315 = t247 * t69
        t317 = (t314 - t315) * t69
        t318 = t251 * t69
        t320 = (t315 - t318) * t69
        t321 = t317 - t320
        t323 = t8 * t321 * t69
        t324 = j - 2
        t325 = u(i,t324,k,n)
        t326 = t250 - t325
        t327 = t326 * t69
        t329 = (t318 - t327) * t69
        t330 = t320 - t329
        t332 = t8 * t330 * t69
        t336 = t8 * t313 * t69
        t338 = (t336 - t249) * t69
        t339 = t338 - t255
        t340 = t339 * t69
        t342 = t8 * t326 * t69
        t344 = (t253 - t342) * t69
        t345 = t255 - t344
        t346 = t345 * t69
        t352 = dz ** 2
        t353 = k + 2
        t354 = u(i,j,t353,n)
        t355 = t354 - t256
        t356 = t355 * t82
        t357 = t257 * t82
        t359 = (t356 - t357) * t82
        t360 = t261 * t82
        t362 = (t357 - t360) * t82
        t363 = t359 - t362
        t365 = t8 * t363 * t82
        t366 = k - 2
        t367 = u(i,j,t366,n)
        t368 = t260 - t367
        t369 = t368 * t82
        t371 = (t360 - t369) * t82
        t372 = t362 - t371
        t374 = t8 * t372 * t82
        t378 = t8 * t355 * t82
        t380 = (t378 - t259) * t82
        t381 = t380 - t265
        t382 = t381 * t82
        t384 = t8 * t368 * t82
        t386 = (t263 - t384) * t82
        t387 = t265 - t386
        t388 = t387 * t82
        t395 = t274 * (-t275 * ((t285 - t294) * t15 + (t298 - t304) * t1
     #5) / 0.24E2 + t245 - t310 * ((t323 - t332) * t69 + (t340 - t346) *
     # t69) / 0.24E2 + t255 - t352 * ((t365 - t374) * t82 + (t382 - t388
     #) * t82) / 0.24E2 + t265 + t159)
        t397 = t176 * t395 / 0.2E1
        t398 = t56 * cc
        t400 = t8 * t42 * t15
        t401 = ut(t286,j,k,n)
        t402 = t23 - t401
        t403 = t402 * t15
        t405 = (t39 - t403) * t15
        t406 = t41 - t405
        t408 = t8 * t406 * t15
        t411 = t29 * t15
        t413 = t8 * t402 * t15
        t415 = (t26 - t413) * t15
        t416 = t28 - t415
        t417 = t416 * t15
        t423 = ut(i,t311,k,n)
        t424 = t423 - t138
        t425 = t424 * t69
        t426 = t139 * t69
        t428 = (t425 - t426) * t69
        t429 = t143 * t69
        t431 = (t426 - t429) * t69
        t432 = t428 - t431
        t434 = t8 * t432 * t69
        t435 = ut(i,t324,k,n)
        t436 = t142 - t435
        t437 = t436 * t69
        t439 = (t429 - t437) * t69
        t440 = t431 - t439
        t442 = t8 * t440 * t69
        t446 = t8 * t424 * t69
        t448 = (t446 - t141) * t69
        t449 = t448 - t147
        t450 = t449 * t69
        t452 = t8 * t436 * t69
        t454 = (t145 - t452) * t69
        t455 = t147 - t454
        t456 = t455 * t69
        t462 = ut(i,j,t353,n)
        t463 = t462 - t148
        t464 = t463 * t82
        t465 = t149 * t82
        t467 = (t464 - t465) * t82
        t468 = t153 * t82
        t470 = (t465 - t468) * t82
        t471 = t467 - t470
        t473 = t8 * t471 * t82
        t474 = ut(i,j,t366,n)
        t475 = t152 - t474
        t476 = t475 * t82
        t478 = (t468 - t476) * t82
        t479 = t470 - t478
        t481 = t8 * t479 * t82
        t485 = t8 * t463 * t82
        t487 = (t485 - t151) * t82
        t488 = t487 - t157
        t489 = t488 * t82
        t491 = t8 * t475 * t82
        t493 = (t155 - t491) * t82
        t494 = t157 - t493
        t495 = t494 * t82
        t502 = t398 * (-t275 * ((t400 - t408) * t15 + (t411 - t417) * t1
     #5) / 0.24E2 + t28 - t310 * ((t434 - t442) * t69 + (t450 - t456) * 
     #t69) / 0.24E2 + t147 - t352 * ((t473 - t481) * t82 + (t489 - t495)
     # * t82) / 0.24E2 + t157 + t162 + t166)
        t504 = t55 * t502 / 0.4E1
        t505 = u(t22,t65,k,n)
        t506 = t505 - t240
        t508 = t8 * t506 * t69
        t509 = u(t22,t71,k,n)
        t510 = t240 - t509
        t512 = t8 * t510 * t69
        t514 = (t508 - t512) * t69
        t515 = u(t22,j,t78,n)
        t516 = t515 - t240
        t518 = t8 * t516 * t82
        t519 = u(t22,j,t84,n)
        t520 = t240 - t519
        t522 = t8 * t520 * t82
        t524 = (t518 - t522) * t82
        t525 = src(t22,j,k,nComp,n)
        t527 = cc * (t302 + t514 + t524 + t525)
        t529 = (t267 - t527) * t15
        t532 = t177 * (t269 / 0.2E1 + t529 / 0.2E1)
        t534 = t176 * t532 / 0.4E1
        t535 = t53 * beta
        t536 = t54 * t33
        t537 = t535 * t536
        t538 = t56 * dt
        t539 = t538 * cc
        t542 = t8 * (t215 + t225 + t235 - t245 - t255 - t265) * t15
        t545 = t8 * (t245 + t255 + t265 - t302 - t514 - t524) * t15
        t548 = t216 - t246
        t550 = t8 * t548 * t15
        t551 = t246 - t505
        t553 = t8 * t551 * t15
        t555 = (t550 - t553) * t15
        t556 = u(i,t65,t78,n)
        t557 = t556 - t246
        t559 = t8 * t557 * t82
        t560 = u(i,t65,t84,n)
        t561 = t246 - t560
        t563 = t8 * t561 * t82
        t565 = (t559 - t563) * t82
        t568 = t8 * (t555 + t338 + t565 - t245 - t255 - t265) * t69
        t569 = t220 - t250
        t571 = t8 * t569 * t15
        t572 = t250 - t509
        t574 = t8 * t572 * t15
        t576 = (t571 - t574) * t15
        t577 = u(i,t71,t78,n)
        t578 = t577 - t250
        t580 = t8 * t578 * t82
        t581 = u(i,t71,t84,n)
        t582 = t250 - t581
        t584 = t8 * t582 * t82
        t586 = (t580 - t584) * t82
        t589 = t8 * (t245 + t255 + t265 - t576 - t344 - t586) * t69
        t592 = t226 - t256
        t594 = t8 * t592 * t15
        t595 = t256 - t515
        t597 = t8 * t595 * t15
        t599 = (t594 - t597) * t15
        t600 = t556 - t256
        t602 = t8 * t600 * t69
        t603 = t256 - t577
        t605 = t8 * t603 * t69
        t607 = (t602 - t605) * t69
        t610 = t8 * (t599 + t607 + t380 - t245 - t255 - t265) * t82
        t611 = t230 - t260
        t613 = t8 * t611 * t15
        t614 = t260 - t519
        t616 = t8 * t614 * t15
        t618 = (t613 - t616) * t15
        t619 = t560 - t260
        t621 = t8 * t619 * t69
        t622 = t260 - t581
        t624 = t8 * t622 * t69
        t626 = (t621 - t624) * t69
        t629 = t8 * (t245 + t255 + t265 - t618 - t626 - t386) * t82
        t634 = t8 * (t126 - t159) * t15
        t637 = t8 * (t159 - t525) * t15
        t640 = src(i,t65,k,nComp,n)
        t643 = t8 * (t640 - t159) * t69
        t644 = src(i,t71,k,nComp,n)
        t647 = t8 * (t159 - t644) * t69
        t650 = src(i,j,t78,nComp,n)
        t653 = t8 * (t650 - t159) * t82
        t654 = src(i,j,t84,nComp,n)
        t657 = t8 * (t159 - t654) * t82
        t663 = t539 * ((t542 - t545) * t15 + (t568 - t589) * t69 + (t610
     # - t629) * t82 + (t634 - t637) * t15 + (t643 - t647) * t69 + (t653
     # - t657) * t82 + (t161 - t165) * t95)
        t665 = t537 * t663 / 0.12E2
        t666 = ut(t22,t65,k,n)
        t667 = t666 - t23
        t669 = t8 * t667 * t69
        t670 = ut(t22,t71,k,n)
        t671 = t23 - t670
        t673 = t8 * t671 * t69
        t675 = (t669 - t673) * t69
        t676 = ut(t22,j,t78,n)
        t677 = t676 - t23
        t679 = t8 * t677 * t82
        t680 = ut(t22,j,t84,n)
        t681 = t23 - t680
        t683 = t8 * t681 * t82
        t685 = (t679 - t683) * t82
        t688 = (src(t22,j,k,nComp,t91) - t525) * t95
        t689 = t688 / 0.2E1
        t692 = (t525 - src(t22,j,k,nComp,t98)) * t95
        t693 = t692 / 0.2E1
        t695 = cc * (t415 + t675 + t685 + t689 + t693)
        t697 = (t168 - t695) * t15
        t700 = t57 * (t170 / 0.2E1 + t697 / 0.2E1)
        t702 = t55 * t700 / 0.8E1
        t704 = t177 * (t269 - t529)
        t706 = t176 * t704 / 0.24E2
        t707 = t6 ** 2
        t708 = t8 * t707
        t711 = t56 * (t215 + t225 + t235 + t126 - t245 - t255 - t265 - t
     #159) * t15
        t713 = t708 * t711 / 0.2E1
        t714 = t707 * t6
        t715 = t8 * t714
        t718 = t538 * (t21 + t114 + t124 + t129 + t133 - t28 - t147 - t1
     #57 - t162 - t166) * t15
        t720 = t715 * t718 / 0.6E1
        t721 = beta * t6
        t737 = u(t11,t311,k,n)
        t740 = t217 * t69
        t743 = t221 * t69
        t745 = (t740 - t743) * t69
        t749 = u(t11,t324,k,n)
        t705 = t69 * (t737 - t216)
        t762 = (t705 * t8 - t219) * t69
        t712 = t69 * (t220 - t749)
        t768 = (-t8 * t712 + t223) * t69
        t776 = u(t11,j,t353,n)
        t777 = t776 - t226
        t779 = t227 * t82
        t782 = t231 * t82
        t784 = (t779 - t782) * t82
        t788 = u(t11,j,t366,n)
        t789 = t230 - t788
        t801 = (t777 * t8 * t82 - t229) * t82
        t807 = (-t789 * t8 * t82 + t233) * t82
        t816 = t274 * (-t275 * ((t8 * ((t171 - t276) * t15 - t279) * t15
     # - t285) * t15 + ((t188 - t215) * t15 - t298) * t15) / 0.24E2 + t2
     #15 + t225 - t310 * ((t8 * ((t705 - t740) * t69 - t745) * t69 - t8 
     #* (t745 - (-t712 + t743) * t69) * t69) * t69 + ((t762 - t225) * t6
     #9 - (t225 - t768) * t69) * t69) / 0.24E2 + t235 - t352 * ((t8 * ((
     #t777 * t82 - t779) * t82 - t784) * t82 - t8 * (t784 - (-t789 * t82
     # + t782) * t82) * t82) * t82 + ((t801 - t235) * t82 - (t235 - t807
     #) * t82) * t82) / 0.24E2 + t126)
        t818 = t721 * t816 / 0.2E1
        t819 = t53 * t707
        t835 = ut(t11,t311,k,n)
        t836 = t835 - t105
        t838 = t106 * t69
        t841 = t110 * t69
        t843 = (t838 - t841) * t69
        t847 = ut(t11,t324,k,n)
        t848 = t109 - t847
        t874 = ut(t11,j,t353,n)
        t875 = t874 - t115
        t877 = t116 * t82
        t880 = t120 * t82
        t882 = (t877 - t880) * t82
        t886 = ut(t11,j,t366,n)
        t887 = t119 - t886
        t844 = t69 * t8
        t879 = t8 * t82
        t914 = t398 * (-t275 * ((t8 * ((t45 - t36) * t15 - t38) * t15 - 
     #t400) * t15 + ((t64 - t21) * t15 - t411) * t15) / 0.24E2 + t21 + t
     #114 - t310 * ((t8 * ((t69 * t836 - t838) * t69 - t843) * t69 - t8 
     #* (t843 - (-t69 * t848 + t841) * t69) * t69) * t69 + (((t836 * t84
     #4 - t108) * t69 - t114) * t69 - (t114 - (-t844 * t848 + t112) * t6
     #9) * t69) * t69) / 0.24E2 - t352 * ((t8 * ((t82 * t875 - t877) * t
     #82 - t882) * t82 - t8 * (t882 - (-t82 * t887 + t880) * t82) * t82)
     # * t82 + (((t875 * t879 - t118) * t82 - t124) * t82 - (t124 - (-t8
     #79 * t887 + t122) * t82) * t82) * t82) / 0.24E2 + t124 + t129 + t1
     #33)
        t916 = t819 * t914 / 0.4E1
        t917 = t32 + t34 * t46 - t48 * t30 / 0.24E2 - t52 - t55 * t173 /
     # 0.8E1 + t176 * t271 / 0.24E2 - t397 - t504 - t534 - t665 - t702 -
     # t706 - t713 - t720 - t818 - t916
        t920 = t177 * (t239 / 0.2E1 + t269 / 0.2E1)
        t922 = t721 * t920 / 0.4E1
        t923 = t535 * t714
        t929 = t189 - t216
        t911 = t15 * t8
        t933 = (t911 * t929 - t550) * t15
        t934 = u(t11,t65,t78,n)
        t938 = u(t11,t65,t84,n)
        t943 = (t8 * (t934 - t216) * t82 - t8 * (t216 - t938) * t82) * t
     #82
        t947 = t193 - t220
        t951 = (t911 * t947 - t571) * t15
        t952 = u(t11,t71,t78,n)
        t956 = u(t11,t71,t84,n)
        t961 = (t8 * (t952 - t220) * t82 - t8 * (t220 - t956) * t82) * t
     #82
        t967 = t199 - t226
        t971 = (t911 * t967 - t594) * t15
        t979 = (t8 * (t934 - t226) * t69 - t8 * (t226 - t952) * t69) * t
     #69
        t983 = t203 - t230
        t987 = (t911 * t983 - t613) * t15
        t995 = (t8 * (t938 - t230) * t69 - t8 * (t230 - t956) * t69) * t
     #69
        t1006 = src(t11,t65,k,nComp,n)
        t1010 = src(t11,t71,k,nComp,n)
        t1016 = src(t11,j,t78,nComp,n)
        t1020 = src(t11,j,t84,nComp,n)
        t1029 = t539 * ((t8 * (t188 + t198 + t208 - t215 - t225 - t235) 
     #* t15 - t542) * t15 + (t8 * (t933 + t762 + t943 - t215 - t225 - t2
     #35) * t69 - t8 * (t215 + t225 + t235 - t951 - t768 - t961) * t69) 
     #* t69 + (t8 * (t971 + t979 + t801 - t215 - t225 - t235) * t82 - t8
     # * (t215 + t225 + t235 - t987 - t995 - t807) * t82) * t82 + (t8 * 
     #(t93 - t126) * t15 - t634) * t15 + (t8 * (t1006 - t126) * t69 - t8
     # * (t126 - t1010) * t69) * t69 + (t8 * (t1016 - t126) * t82 - t8 *
     # (t126 - t1020) * t82) * t82 + (t128 - t132) * t95)
        t1031 = t923 * t1029 / 0.12E2
        t1033 = t819 * t173 / 0.8E1
        t1035 = t721 * t271 / 0.24E2
        t1037 = t721 * t395 / 0.2E1
        t1039 = t819 * t502 / 0.4E1
        t1041 = t721 * t532 / 0.4E1
        t1043 = t923 * t663 / 0.12E2
        t1045 = t819 * t700 / 0.8E1
        t1047 = t721 * t704 / 0.24E2
        t1048 = t8 * t54
        t1051 = t8 * t536
        t1062 = t922 - t1031 + t1033 - t1035 + t1037 + t1039 + t1041 + t
     #1043 + t1045 + t1047 + t1048 * t711 / 0.2E1 + t1051 * t718 / 0.6E1
     # + t176 * t816 / 0.2E1 + t55 * t914 / 0.4E1 - t176 * t920 / 0.4E1 
     #+ t537 * t1029 / 0.12E2
        t1064 = (t917 + t1062) * t4
        t1067 = -t32 + t52 + t713 + t720 + t818 + t916 - t922 + t1031 - 
     #t1033 + t1035 - t1037 - t1039
        t1069 = cc * t2
        t1070 = t1069 / 0.2E1
        t1071 = cc * t12
        t1072 = t1071 / 0.2E1
        t1074 = (-t1069 + t1071) * t15
        t1075 = cc * t23
        t1077 = (t1069 - t1075) * t15
        t1079 = (t1074 - t1077) * t15
        t1080 = cc * t10
        t1082 = (-t1071 + t1080) * t15
        t1084 = (t1082 - t1074) * t15
        t1086 = (t1084 - t1079) * t15
        t1087 = cc * t401
        t1089 = (-t1087 + t1075) * t15
        t1091 = (t1077 - t1089) * t15
        t1093 = (t1079 - t1091) * t15
        t1099 = t275 * (t1079 - dx * (t1086 - t1093) / 0.12E2) / 0.24E2
        t1100 = t1074 / 0.2E1
        t1101 = t1077 / 0.2E1
        t1108 = dx * (t1100 + t1101 - t275 * (t1086 / 0.2E1 + t1093 / 0.
     #2E1) / 0.6E1) / 0.4E1
        t1115 = (((cc * t59 - t1080) * t15 - t1082) * t15 - t1084) * t15
        t1121 = t275 * (t1084 - dx * (t1115 - t1086) / 0.12E2) / 0.24E2
        t1129 = dx * (t1082 / 0.2E1 + t1100 - t275 * (t1115 / 0.2E1 + t1
     #086 / 0.2E1) / 0.6E1) / 0.4E1
        t1131 = dx * t297 / 0.24E2
        t1135 = t8 * (t277 - dx * t283 / 0.24E2)
        t1136 = -t1064 * t6 - t1041 - t1043 - t1045 - t1047 - t1070 + t1
     #072 - t1099 - t1108 + t1121 - t1129 - t1131 + t1135
        t1140 = i - 3
        t1141 = ut(t1140,j,k,n)
        t1120 = (t401 - t1141) * t15
        t1146 = (-t1120 * t8 + t413) * t15
        t1147 = ut(t286,t65,k,n)
        t1151 = ut(t286,t71,k,n)
        t1157 = ut(t286,j,t78,n)
        t1161 = ut(t286,j,t84,n)
        t1168 = src(t286,j,k,nComp,n)
        t1182 = t57 * (t697 / 0.2E1 + (t695 - cc * (t1146 + (t8 * (t1147
     # - t401) * t69 - t8 * (t401 - t1151) * t69) * t69 + (t8 * (t1157 -
     # t401) * t82 - t8 * (t401 - t1161) * t82) * t82 + (src(t286,j,k,nC
     #omp,t91) - t1168) * t95 / 0.2E1 + (t1168 - src(t286,j,k,nComp,t98)
     #) * t95 / 0.2E1)) * t15 / 0.2E1)
        t1167 = (t287 - u(t1140,j,k,n)) * t15
        t1190 = (-t1167 * t8 + t300) * t15
        t1191 = u(t286,t65,k,n)
        t1195 = u(t286,t71,k,n)
        t1200 = (t8 * (t1191 - t287) * t69 - t8 * (t287 - t1195) * t69) 
     #* t69
        t1201 = u(t286,j,t78,n)
        t1205 = u(t286,j,t84,n)
        t1210 = (t8 * (t1201 - t287) * t82 - t8 * (t287 - t1205) * t82) 
     #* t82
        t1214 = (t527 - cc * (t1190 + t1200 + t1210 + t1168)) * t15
        t1216 = t177 * (t529 - t1214)
        t1221 = t56 * (t245 + t255 + t265 + t159 - t302 - t514 - t524 - 
     #t525) * t15
        t1223 = t708 * t1221 / 0.2E1
        t1226 = t538 * (t28 + t147 + t157 + t162 + t166 - t415 - t675 - 
     #t685 - t689 - t693) * t15
        t1228 = t715 * t1226 / 0.6E1
        t1229 = t397 + t504 - t534 + t665 - t702 + t706 - t1037 - t1039 
     #+ t1041 - t1043 + t1045 - t1047 - t55 * t1182 / 0.8E1 - t176 * t12
     #16 / 0.24E2 - t1223 - t1228
        t1245 = u(t22,t311,k,n)
        t1248 = t506 * t69
        t1251 = t510 * t69
        t1253 = (t1248 - t1251) * t69
        t1257 = u(t22,t324,k,n)
        t1208 = (t1245 - t505) * t69
        t1270 = (t1208 * t8 - t508) * t69
        t1212 = (t509 - t1257) * t69
        t1276 = (-t1212 * t8 + t512) * t69
        t1284 = u(t22,j,t353,n)
        t1285 = t1284 - t515
        t1287 = t516 * t82
        t1290 = t520 * t82
        t1292 = (t1287 - t1290) * t82
        t1296 = u(t22,j,t366,n)
        t1297 = t519 - t1296
        t1309 = (t1285 * t8 * t82 - t518) * t82
        t1315 = (-t1297 * t8 * t82 + t522) * t82
        t1324 = t274 * (-t275 * ((t294 - t8 * (t291 - (-t1167 + t289) * 
     #t15) * t15) * t15 + (t304 - (t302 - t1190) * t15) * t15) / 0.24E2 
     #+ t302 + t514 - t310 * ((t8 * ((t1208 - t1248) * t69 - t1253) * t6
     #9 - t8 * (t1253 - (-t1212 + t1251) * t69) * t69) * t69 + ((t1270 -
     # t514) * t69 - (t514 - t1276) * t69) * t69) / 0.24E2 + t524 - t352
     # * ((t8 * ((t1285 * t82 - t1287) * t82 - t1292) * t82 - t8 * (t129
     #2 - (-t1297 * t82 + t1290) * t82) * t82) * t82 + ((t1309 - t524) *
     # t82 - (t524 - t1315) * t82) * t82) / 0.24E2 + t525)
        t1326 = t721 * t1324 / 0.2E1
        t1342 = ut(t22,t311,k,n)
        t1345 = t667 * t69
        t1348 = t671 * t69
        t1350 = (t1345 - t1348) * t69
        t1354 = ut(t22,t324,k,n)
        t1381 = ut(t22,j,t353,n)
        t1382 = t1381 - t676
        t1384 = t677 * t82
        t1387 = t681 * t82
        t1389 = (t1384 - t1387) * t82
        t1393 = ut(t22,j,t366,n)
        t1394 = t680 - t1393
        t1323 = (t1342 - t666) * t69
        t1331 = (t670 - t1354) * t69
        t1421 = t398 * (-t275 * ((t408 - t8 * (t405 - (-t1120 + t403) * 
     #t15) * t15) * t15 + (t417 - (t415 - t1146) * t15) * t15) / 0.24E2 
     #+ t415 - t310 * ((t8 * ((t1323 - t1345) * t69 - t1350) * t69 - t8 
     #* (t1350 - (-t1331 + t1348) * t69) * t69) * t69 + (((t1323 * t8 - 
     #t669) * t69 - t675) * t69 - (t675 - (-t1331 * t8 + t673) * t69) * 
     #t69) * t69) / 0.24E2 + t675 - t352 * ((t8 * ((t1382 * t82 - t1384)
     # * t82 - t1389) * t82 - t8 * (t1389 - (-t1394 * t82 + t1387) * t82
     #) * t82) * t82 + (((t1382 * t8 * t82 - t679) * t82 - t685) * t82 -
     # (t685 - (-t1394 * t8 * t82 + t683) * t82) * t82) * t82) / 0.24E2 
     #+ t685 + t689 + t693)
        t1423 = t819 * t1421 / 0.4E1
        t1426 = t177 * (t529 / 0.2E1 + t1214 / 0.2E1)
        t1428 = t721 * t1426 / 0.4E1
        t1404 = (t505 - t1191) * t15
        t1438 = (-t1404 * t8 + t553) * t15
        t1439 = u(t22,t65,t78,n)
        t1443 = u(t22,t65,t84,n)
        t1448 = (t8 * (t1439 - t505) * t82 - t8 * (t505 - t1443) * t82) 
     #* t82
        t1414 = (t509 - t1195) * t15
        t1456 = (-t1414 * t8 + t574) * t15
        t1457 = u(t22,t71,t78,n)
        t1461 = u(t22,t71,t84,n)
        t1466 = (t8 * (t1457 - t509) * t82 - t8 * (t509 - t1461) * t82) 
     #* t82
        t1427 = (t515 - t1201) * t15
        t1476 = (-t1427 * t8 + t597) * t15
        t1484 = (t8 * (t1439 - t515) * t69 - t8 * (t515 - t1457) * t69) 
     #* t69
        t1441 = (t519 - t1205) * t15
        t1492 = (-t1441 * t8 + t616) * t15
        t1500 = (t8 * (t1443 - t519) * t69 - t8 * (t519 - t1461) * t69) 
     #* t69
        t1511 = src(t22,t65,k,nComp,n)
        t1515 = src(t22,t71,k,nComp,n)
        t1521 = src(t22,j,t78,nComp,n)
        t1525 = src(t22,j,t84,nComp,n)
        t1534 = t539 * ((t545 - t8 * (t302 + t514 + t524 - t1190 - t1200
     # - t1210) * t15) * t15 + (t8 * (t1438 + t1270 + t1448 - t302 - t51
     #4 - t524) * t69 - t8 * (t302 + t514 + t524 - t1456 - t1276 - t1466
     #) * t69) * t69 + (t8 * (t1476 + t1484 + t1309 - t302 - t514 - t524
     #) * t82 - t8 * (t302 + t514 + t524 - t1492 - t1500 - t1315) * t82)
     # * t82 + (t637 - t8 * (t525 - t1168) * t15) * t15 + (t8 * (t1511 -
     # t525) * t69 - t8 * (t525 - t1515) * t69) * t69 + (t8 * (t1521 - t
     #525) * t82 - t8 * (t525 - t1525) * t82) * t82 + (t688 - t692) * t9
     #5)
        t1536 = t923 * t1534 / 0.12E2
        t1538 = t819 * t1182 / 0.8E1
        t1540 = t721 * t1216 / 0.24E2
        t1556 = dt * (t39 - dx * t406 / 0.24E2)
        t1558 = dx * t416
        t1561 = t51 * t1556
        t1563 = t7 * t1558 / 0.24E2
        t1564 = t1326 + t1423 + t1428 + t1536 + t1538 + t1540 + t1048 * 
     #t1221 / 0.2E1 + t1051 * t1226 / 0.6E1 - t176 * t1324 / 0.2E1 - t55
     # * t1421 / 0.4E1 - t176 * t1426 / 0.4E1 - t537 * t1534 / 0.12E2 + 
     #t34 * t1556 - t48 * t1558 / 0.24E2 - t1561 + t1563
        t1566 = (t1229 + t1564) * t4
        t1576 = (t1091 - (t1089 - (-cc * t1141 + t1087) * t15) * t15) * 
     #t15
        t1583 = dx * (t1101 + t1089 / 0.2E1 - t275 * (t1093 / 0.2E1 + t1
     #576 / 0.2E1) / 0.6E1) / 0.4E1
        t1589 = t275 * (t1091 - dx * (t1093 - t1576) / 0.12E2) / 0.24E2
        t1591 = dx * t303 / 0.24E2
        t1595 = t8 * (t280 - dx * t292 / 0.24E2)
        t1597 = -t1566 * t6 + t1037 + t1039 - t1041 + t1043 - t1045 + t1
     #047 + t1070 - t1583 - t1589 - t1591 + t1595
        t1598 = t1075 / 0.2E1
        t1599 = -t1598 + t1099 - t1108 + t1223 + t1228 - t1326 - t1423 -
     # t1428 - t1536 - t1538 - t1540 + t1561 - t1563
        t1605 = dt * dy
        t1607 = cc * (t555 + t338 + t565 + t640)
        t1609 = (t1607 - t267) * t69
        t1611 = cc * (t576 + t344 + t586 + t644)
        t1613 = (t267 - t1611) * t69
        t1615 = t1605 * (t1609 - t1613)
        t1617 = t176 * t1615 / 0.24E2
        t1620 = t56 * (t555 + t338 + t565 + t640 - t245 - t255 - t265 - 
     #t159) * t69
        t1622 = t708 * t1620 / 0.2E1
        t1623 = t105 - t138
        t1625 = t8 * t1623 * t15
        t1626 = t138 - t666
        t1628 = t8 * t1626 * t15
        t1630 = (t1625 - t1628) * t15
        t1631 = ut(i,t65,t78,n)
        t1632 = t1631 - t138
        t1634 = t8 * t1632 * t82
        t1635 = ut(i,t65,t84,n)
        t1636 = t138 - t1635
        t1638 = t8 * t1636 * t82
        t1640 = (t1634 - t1638) * t82
        t1643 = (src(i,t65,k,nComp,t91) - t640) * t95
        t1644 = t1643 / 0.2E1
        t1647 = (t640 - src(i,t65,k,nComp,t98)) * t95
        t1648 = t1647 / 0.2E1
        t1651 = t538 * (t1630 + t448 + t1640 + t1644 + t1648 - t28 - t14
     #7 - t157 - t162 - t166) * t69
        t1653 = t715 * t1651 / 0.6E1
        t1655 = t548 * t15
        t1658 = t551 * t15
        t1660 = (t1655 - t1658) * t15
        t1681 = j + 3
        t1601 = (u(i,t1681,k,n) - t312) * t69
        t1695 = (t1601 * t8 - t336) * t69
        t1703 = u(i,t65,t353,n)
        t1704 = t1703 - t556
        t1706 = t557 * t82
        t1709 = t561 * t82
        t1711 = (t1706 - t1709) * t82
        t1715 = u(i,t65,t366,n)
        t1716 = t560 - t1715
        t1728 = (t1704 * t8 * t82 - t559) * t82
        t1734 = (-t1716 * t8 * t82 + t563) * t82
        t1743 = t274 * (-t275 * ((t8 * ((t15 * t929 - t1655) * t15 - t16
     #60) * t15 - t8 * (t1660 - (-t1404 + t1658) * t15) * t15) * t15 + (
     #(t933 - t555) * t15 - (t555 - t1438) * t15) * t15) / 0.24E2 + t555
     # - t310 * ((t8 * ((t1601 - t314) * t69 - t317) * t69 - t323) * t69
     # + ((t1695 - t338) * t69 - t340) * t69) / 0.24E2 + t338 - t352 * (
     #(t8 * ((t1704 * t82 - t1706) * t82 - t1711) * t82 - t8 * (t1711 - 
     #(-t1716 * t82 + t1709) * t82) * t82) * t82 + ((t1728 - t565) * t82
     # - (t565 - t1734) * t82) * t82) / 0.24E2 + t565 + t640)
        t1745 = t721 * t1743 / 0.2E1
        t1748 = t1623 * t15
        t1751 = t1626 * t15
        t1753 = (t1748 - t1751) * t15
        t1783 = ut(i,t1681,k,n)
        t1710 = (t1783 - t423) * t69
        t1796 = (t1710 * t8 - t446) * t69
        t1804 = ut(i,t65,t353,n)
        t1805 = t1804 - t1631
        t1807 = t1632 * t82
        t1810 = t1636 * t82
        t1812 = (t1807 - t1810) * t82
        t1816 = ut(i,t65,t366,n)
        t1817 = t1635 - t1816
        t1717 = t15 * (t66 - t105)
        t1723 = t15 * (t666 - t1147)
        t1844 = t398 * (-t275 * ((t8 * ((t1717 - t1748) * t15 - t1753) *
     # t15 - t8 * (t1753 - (-t1723 + t1751) * t15) * t15) * t15 + (((t17
     #17 * t8 - t1625) * t15 - t1630) * t15 - (t1630 - (-t1723 * t8 + t1
     #628) * t15) * t15) * t15) / 0.24E2 + t1630 + t448 - t310 * ((t8 * 
     #((t1710 - t425) * t69 - t428) * t69 - t434) * t69 + ((t1796 - t448
     #) * t69 - t450) * t69) / 0.24E2 - t352 * ((t8 * ((t1805 * t82 - t1
     #807) * t82 - t1812) * t82 - t8 * (t1812 - (-t1817 * t82 + t1810) *
     # t82) * t82) * t82 + (((t1805 * t8 * t82 - t1634) * t82 - t1640) *
     # t82 - (t1640 - (-t1817 * t8 * t82 + t1638) * t82) * t82) * t82) /
     # 0.24E2 + t1640 + t1644 + t1648)
        t1846 = t819 * t1844 / 0.4E1
        t1854 = (t8 * (t737 - t312) * t15 - t8 * (t312 - t1245) * t15) *
     # t15
        t1855 = u(i,t311,t78,n)
        t1859 = u(i,t311,t84,n)
        t1864 = (t8 * (t1855 - t312) * t82 - t8 * (t312 - t1859) * t82) 
     #* t82
        t1865 = src(i,t311,k,nComp,n)
        t1869 = (cc * (t1854 + t1695 + t1864 + t1865) - t1607) * t69
        t1872 = t1605 * (t1869 / 0.2E1 + t1609 / 0.2E1)
        t1874 = t721 * t1872 / 0.4E1
        t1895 = (t8 * (t934 - t556) * t15 - t8 * (t556 - t1439) * t15) *
     # t15
        t1839 = (t1855 - t556) * t69
        t1900 = (t1839 * t8 - t602) * t69
        t1911 = (t8 * (t938 - t560) * t15 - t8 * (t560 - t1443) * t15) *
     # t15
        t1851 = (t1859 - t560) * t69
        t1916 = (t1851 * t8 - t621) * t69
        t1935 = src(i,t65,t78,nComp,n)
        t1939 = src(i,t65,t84,nComp,n)
        t1948 = t539 * ((t8 * (t933 + t762 + t943 - t555 - t338 - t565) 
     #* t15 - t8 * (t555 + t338 + t565 - t1438 - t1270 - t1448) * t15) *
     # t15 + (t8 * (t1854 + t1695 + t1864 - t555 - t338 - t565) * t69 - 
     #t568) * t69 + (t8 * (t1895 + t1900 + t1728 - t555 - t338 - t565) *
     # t82 - t8 * (t555 + t338 + t565 - t1911 - t1916 - t1734) * t82) * 
     #t82 + (t8 * (t1006 - t640) * t15 - t8 * (t640 - t1511) * t15) * t1
     #5 + (t8 * (t1865 - t640) * t69 - t643) * t69 + (t8 * (t1935 - t640
     #) * t82 - t8 * (t640 - t1939) * t82) * t82 + (t1643 - t1647) * t95
     #)
        t1950 = t923 * t1948 / 0.12E2
        t1951 = t56 * dy
        t1960 = ut(i,t311,t78,n)
        t1964 = ut(i,t311,t84,n)
        t1981 = cc * (t1630 + t448 + t1640 + t1644 + t1648)
        t1985 = (t1981 - t168) * t69
        t1988 = t1951 * ((cc * ((t8 * (t835 - t423) * t15 - t8 * (t423 -
     # t1342) * t15) * t15 + t1796 + (t8 * (t1960 - t423) * t82 - t8 * (
     #t423 - t1964) * t82) * t82 + (src(i,t311,k,nComp,t91) - t1865) * t
     #95 / 0.2E1 + (t1865 - src(i,t311,k,nComp,t98)) * t95 / 0.2E1) - t1
     #981) * t69 / 0.2E1 + t1985 / 0.2E1)
        t1990 = t819 * t1988 / 0.8E1
        t1992 = t1605 * (t1869 - t1609)
        t1994 = t721 * t1992 / 0.24E2
        t1997 = t1605 * (t1609 / 0.2E1 + t1613 / 0.2E1)
        t1999 = t721 * t1997 / 0.4E1
        t2000 = t109 - t142
        t2002 = t8 * t2000 * t15
        t2003 = t142 - t670
        t2005 = t8 * t2003 * t15
        t2007 = (t2002 - t2005) * t15
        t2008 = ut(i,t71,t78,n)
        t2009 = t2008 - t142
        t2011 = t8 * t2009 * t82
        t2012 = ut(i,t71,t84,n)
        t2013 = t142 - t2012
        t2015 = t8 * t2013 * t82
        t2017 = (t2011 - t2015) * t82
        t2020 = (src(i,t71,k,nComp,t91) - t644) * t95
        t2021 = t2020 / 0.2E1
        t2024 = (t644 - src(i,t71,k,nComp,t98)) * t95
        t2025 = t2024 / 0.2E1
        t2027 = cc * (t2007 + t454 + t2017 + t2021 + t2025)
        t2029 = (t168 - t2027) * t69
        t2032 = t1951 * (t1985 / 0.2E1 + t2029 / 0.2E1)
        t2034 = t819 * t2032 / 0.8E1
        t2036 = t721 * t1615 / 0.24E2
        t2040 = dt * (t426 - dy * t432 / 0.24E2)
        t2042 = dy * t449
        t2045 = t51 * t2040
        t2047 = t7 * t2042 / 0.24E2
        t2048 = -t1617 - t1622 - t1653 - t1745 - t1846 + t1874 - t1950 +
     # t1990 - t1994 + t1999 + t2034 + t2036 + t34 * t2040 - t48 * t2042
     # / 0.24E2 - t2045 + t2047
        t2066 = t176 * t1997 / 0.4E1
        t2068 = t55 * t2032 / 0.8E1
        t2069 = -t397 - t504 - t665 + t1037 + t1039 + t1043 + t1048 * t1
     #620 / 0.2E1 + t1051 * t1651 / 0.6E1 + t176 * t1743 / 0.2E1 + t55 *
     # t1844 / 0.4E1 - t176 * t1872 / 0.4E1 + t537 * t1948 / 0.12E2 - t5
     #5 * t1988 / 0.8E1 + t176 * t1992 / 0.24E2 - t2066 - t2068
        t2071 = (t2048 + t2069) * t4
        t2075 = cc * t138
        t2076 = cc * t423
        t2078 = (-t2075 + t2076) * t69
        t2080 = (-t1069 + t2075) * t69
        t2082 = (t2078 - t2080) * t69
        t2089 = (((cc * t1783 - t2076) * t69 - t2078) * t69 - t2082) * t
     #69
        t2090 = cc * t142
        t2092 = (t1069 - t2090) * t69
        t2094 = (t2080 - t2092) * t69
        t2096 = (t2082 - t2094) * t69
        t2102 = t310 * (t2082 - dy * (t2089 - t2096) / 0.12E2) / 0.24E2
        t2104 = t2080 / 0.2E1
        t2111 = dy * (t2078 / 0.2E1 + t2104 - t310 * (t2089 / 0.2E1 + t2
     #096 / 0.2E1) / 0.6E1) / 0.4E1
        t2113 = dy * t339 / 0.24E2
        t2117 = t8 * (t315 - dy * t321 / 0.24E2)
        t2118 = cc * t435
        t2120 = (-t2118 + t2090) * t69
        t2122 = (t2092 - t2120) * t69
        t2124 = (t2094 - t2122) * t69
        t2130 = t310 * (t2094 - dy * (t2096 - t2124) / 0.12E2) / 0.24E2
        t2131 = t2092 / 0.2E1
        t2138 = dy * (t2104 + t2131 - t310 * (t2096 / 0.2E1 + t2124 / 0.
     #2E1) / 0.6E1) / 0.4E1
        t2139 = t2075 / 0.2E1
        t2140 = -t2071 * t6 + t1622 + t1653 + t1745 + t1846 + t2102 - t2
     #111 - t2113 + t2117 - t2130 - t2138 + t2139
        t2141 = -t1874 + t1950 - t1990 + t1994 - t1999 - t2034 - t2036 +
     # t2045 - t2047 - t1037 - t1039 - t1043 - t1070
        t2147 = t56 * (t245 + t255 + t265 + t159 - t576 - t344 - t586 - 
     #t644) * t69
        t2152 = t538 * (t28 + t147 + t157 + t162 + t166 - t2007 - t454 -
     # t2017 - t2021 - t2025) * t69
        t2156 = t569 * t15
        t2159 = t572 * t15
        t2161 = (t2156 - t2159) * t15
        t2182 = j - 3
        t2098 = (t325 - u(i,t2182,k,n)) * t69
        t2196 = (-t2098 * t8 + t342) * t69
        t2204 = u(i,t71,t353,n)
        t2205 = t2204 - t577
        t2207 = t578 * t82
        t2210 = t582 * t82
        t2212 = (t2207 - t2210) * t82
        t2216 = u(i,t71,t366,n)
        t2217 = t581 - t2216
        t2229 = (t2205 * t8 * t82 - t580) * t82
        t2235 = (-t2217 * t8 * t82 + t584) * t82
        t2244 = t274 * (-t275 * ((t8 * ((t15 * t947 - t2156) * t15 - t21
     #61) * t15 - t8 * (t2161 - (-t1414 + t2159) * t15) * t15) * t15 + (
     #(t951 - t576) * t15 - (t576 - t1456) * t15) * t15) / 0.24E2 + t576
     # + t344 - t310 * ((t332 - t8 * (t329 - (-t2098 + t327) * t69) * t6
     #9) * t69 + (t346 - (t344 - t2196) * t69) * t69) / 0.24E2 + t586 - 
     #t352 * ((t8 * ((t2205 * t82 - t2207) * t82 - t2212) * t82 - t8 * (
     #t2212 - (-t2217 * t82 + t2210) * t82) * t82) * t82 + ((t2229 - t58
     #6) * t82 - (t586 - t2235) * t82) * t82) / 0.24E2 + t644)
        t2249 = t2000 * t15
        t2252 = t2003 * t15
        t2254 = (t2249 - t2252) * t15
        t2284 = ut(i,t2182,k,n)
        t2191 = (t435 - t2284) * t69
        t2297 = (-t2191 * t8 + t452) * t69
        t2305 = ut(i,t71,t353,n)
        t2306 = t2305 - t2008
        t2308 = t2009 * t82
        t2311 = t2013 * t82
        t2313 = (t2308 - t2311) * t82
        t2317 = ut(i,t71,t366,n)
        t2318 = t2012 - t2317
        t2195 = t15 * (t72 - t109)
        t2202 = t15 * (t670 - t1151)
        t2345 = t398 * (-t275 * ((t8 * ((t2195 - t2249) * t15 - t2254) *
     # t15 - t8 * (t2254 - (-t2202 + t2252) * t15) * t15) * t15 + (((t21
     #95 * t8 - t2002) * t15 - t2007) * t15 - (t2007 - (-t2202 * t8 + t2
     #005) * t15) * t15) * t15) / 0.24E2 + t2007 + t454 - t310 * ((t442 
     #- t8 * (t439 - (-t2191 + t437) * t69) * t69) * t69 + (t456 - (t454
     # - t2297) * t69) * t69) / 0.24E2 - t352 * ((t8 * ((t2306 * t82 - t
     #2308) * t82 - t2313) * t82 - t8 * (t2313 - (-t2318 * t82 + t2311) 
     #* t82) * t82) * t82 + (((t2306 * t8 * t82 - t2011) * t82 - t2017) 
     #* t82 - (t2017 - (-t2318 * t8 * t82 + t2015) * t82) * t82) * t82) 
     #/ 0.24E2 + t2017 + t2021 + t2025)
        t2355 = (t8 * (t749 - t325) * t15 - t8 * (t325 - t1257) * t15) *
     # t15
        t2356 = u(i,t324,t78,n)
        t2360 = u(i,t324,t84,n)
        t2365 = (t8 * (t2356 - t325) * t82 - t8 * (t325 - t2360) * t82) 
     #* t82
        t2366 = src(i,t324,k,nComp,n)
        t2370 = (t1611 - cc * (t2355 + t2196 + t2365 + t2366)) * t69
        t2373 = t1605 * (t1613 / 0.2E1 + t2370 / 0.2E1)
        t2396 = (t8 * (t952 - t577) * t15 - t8 * (t577 - t1457) * t15) *
     # t15
        t2322 = (t577 - t2356) * t69
        t2401 = (-t2322 * t8 + t605) * t69
        t2412 = (t8 * (t956 - t581) * t15 - t8 * (t581 - t1461) * t15) *
     # t15
        t2332 = (t581 - t2360) * t69
        t2417 = (-t2332 * t8 + t624) * t69
        t2436 = src(i,t71,t78,nComp,n)
        t2440 = src(i,t71,t84,nComp,n)
        t2449 = t539 * ((t8 * (t951 + t768 + t961 - t576 - t344 - t586) 
     #* t15 - t8 * (t576 + t344 + t586 - t1456 - t1276 - t1466) * t15) *
     # t15 + (t589 - t8 * (t576 + t344 + t586 - t2355 - t2196 - t2365) *
     # t69) * t69 + (t8 * (t2396 + t2401 + t2229 - t576 - t344 - t586) *
     # t82 - t8 * (t576 + t344 + t586 - t2412 - t2417 - t2235) * t82) * 
     #t82 + (t8 * (t1010 - t644) * t15 - t8 * (t644 - t1515) * t15) * t1
     #5 + (t647 - t8 * (t644 - t2366) * t69) * t69 + (t8 * (t2436 - t644
     #) * t82 - t8 * (t644 - t2440) * t82) * t82 + (t2020 - t2024) * t95
     #)
        t2460 = ut(i,t324,t78,n)
        t2464 = ut(i,t324,t84,n)
        t2484 = t1951 * (t2029 / 0.2E1 + (t2027 - cc * ((t8 * (t847 - t4
     #35) * t15 - t8 * (t435 - t1354) * t15) * t15 + t2297 + (t8 * (t246
     #0 - t435) * t82 - t8 * (t435 - t2464) * t82) * t82 + (src(i,t324,k
     #,nComp,t91) - t2366) * t95 / 0.2E1 + (t2366 - src(i,t324,k,nComp,t
     #98)) * t95 / 0.2E1)) * t69 / 0.2E1)
        t2488 = t1605 * (t1613 - t2370)
        t2492 = t708 * t2147 / 0.2E1
        t2494 = t715 * t2152 / 0.6E1
        t2496 = t721 * t2244 / 0.2E1
        t2498 = t819 * t2345 / 0.4E1
        t2500 = t721 * t2373 / 0.4E1
        t2502 = t923 * t2449 / 0.12E2
        t2504 = t819 * t2484 / 0.8E1
        t2506 = t721 * t2488 / 0.24E2
        t2507 = t1048 * t2147 / 0.2E1 + t1051 * t2152 / 0.6E1 - t176 * t
     #2244 / 0.2E1 - t55 * t2345 / 0.4E1 - t176 * t2373 / 0.4E1 - t537 *
     # t2449 / 0.12E2 - t55 * t2484 / 0.8E1 - t176 * t2488 / 0.24E2 - t2
     #492 - t2494 + t2496 + t2498 + t2500 + t2502 + t2504 + t2506
        t2511 = dt * (t429 - dy * t440 / 0.24E2)
        t2512 = t51 * t2511
        t2513 = dy * t455
        t2515 = t7 * t2513 / 0.24E2
        t2519 = t1617 + t1999 + t2034 - t2036 + t397 + t504 + t665 - t10
     #37 - t1039 - t1043 - t2512 + t2515 + t34 * t2511 - t48 * t2513 / 0
     #.24E2 - t2066 - t2068
        t2521 = (t2507 + t2519) * t4
        t2524 = t2090 / 0.2E1
        t2525 = t2130 - t2138 - t2524 + t2492 + t2494 - t2496 - t2498 - 
     #t2500 - t2502 - t2504 - t2506 - t1999
        t2527 = dy * t345 / 0.24E2
        t2535 = (t2122 - (t2120 - (-cc * t2284 + t2118) * t69) * t69) * 
     #t69
        t2542 = dy * (t2131 + t2120 / 0.2E1 - t310 * (t2124 / 0.2E1 + t2
     #535 / 0.2E1) / 0.6E1) / 0.4E1
        t2546 = t8 * (t318 - dy * t330 / 0.24E2)
        t2552 = t310 * (t2122 - dy * (t2124 - t2535) / 0.12E2) / 0.24E2
        t2554 = -t2521 * t6 + t1037 + t1039 + t1043 + t1070 - t2034 + t2
     #036 + t2512 - t2515 - t2527 - t2542 + t2546 - t2552
        t2560 = t56 * dz
        t2561 = t115 - t148
        t2563 = t8 * t2561 * t15
        t2564 = t148 - t676
        t2566 = t8 * t2564 * t15
        t2568 = (t2563 - t2566) * t15
        t2569 = t1631 - t148
        t2571 = t8 * t2569 * t69
        t2572 = t148 - t2008
        t2574 = t8 * t2572 * t69
        t2576 = (t2571 - t2574) * t69
        t2579 = (src(i,j,t78,nComp,t91) - t650) * t95
        t2580 = t2579 / 0.2E1
        t2583 = (t650 - src(i,j,t78,nComp,t98)) * t95
        t2584 = t2583 / 0.2E1
        t2586 = cc * (t2568 + t2576 + t487 + t2580 + t2584)
        t2588 = (t2586 - t168) * t82
        t2589 = t119 - t152
        t2591 = t8 * t2589 * t15
        t2592 = t152 - t680
        t2594 = t8 * t2592 * t15
        t2596 = (t2591 - t2594) * t15
        t2597 = t1635 - t152
        t2599 = t8 * t2597 * t69
        t2600 = t152 - t2012
        t2602 = t8 * t2600 * t69
        t2604 = (t2599 - t2602) * t69
        t2607 = (src(i,j,t84,nComp,t91) - t654) * t95
        t2608 = t2607 / 0.2E1
        t2611 = (t654 - src(i,j,t84,nComp,t98)) * t95
        t2612 = t2611 / 0.2E1
        t2614 = cc * (t2596 + t2604 + t493 + t2608 + t2612)
        t2616 = (t168 - t2614) * t82
        t2618 = t2588 / 0.2E1 + t2616 / 0.2E1
        t2619 = t2560 * t2618
        t2622 = dt * dz
        t2624 = cc * (t599 + t607 + t380 + t650)
        t2626 = (t2624 - t267) * t82
        t2628 = cc * (t618 + t626 + t386 + t654)
        t2630 = (t267 - t2628) * t82
        t2631 = t2626 - t2630
        t2632 = t2622 * t2631
        t2637 = t56 * (t599 + t607 + t380 + t650 - t245 - t255 - t265 - 
     #t159) * t82
        t2639 = t708 * t2637 / 0.2E1
        t2642 = t538 * (t2568 + t2576 + t487 + t2580 + t2584 - t28 - t14
     #7 - t157 - t162 - t166) * t82
        t2644 = t715 * t2642 / 0.6E1
        t2646 = t592 * t15
        t2649 = t595 * t15
        t2651 = (t2646 - t2649) * t15
        t2673 = t600 * t69
        t2676 = t603 * t69
        t2678 = (t2673 - t2676) * t69
        t2699 = k + 3
        t2701 = u(i,j,t2699,n) - t354
        t2713 = (t2701 * t8 * t82 - t378) * t82
        t2722 = t274 * (-t275 * ((t8 * ((t15 * t967 - t2646) * t15 - t26
     #51) * t15 - t8 * (t2651 - (-t1427 + t2649) * t15) * t15) * t15 + (
     #(t971 - t599) * t15 - (t599 - t1476) * t15) * t15) / 0.24E2 + t599
     # + t607 - t310 * ((t8 * ((t1839 - t2673) * t69 - t2678) * t69 - t8
     # * (t2678 - (-t2322 + t2676) * t69) * t69) * t69 + ((t1900 - t607)
     # * t69 - (t607 - t2401) * t69) * t69) / 0.24E2 + t380 - t352 * ((t
     #8 * ((t2701 * t82 - t356) * t82 - t359) * t82 - t365) * t82 + ((t2
     #713 - t380) * t82 - t382) * t82) / 0.24E2 + t650)
        t2724 = t721 * t2722 / 0.2E1
        t2727 = t2561 * t15
        t2730 = t2564 * t15
        t2732 = (t2727 - t2730) * t15
        t2764 = t2569 * t69
        t2767 = t2572 * t69
        t2769 = (t2764 - t2767) * t69
        t2799 = ut(i,j,t2699,n)
        t2800 = t2799 - t462
        t2812 = (t2800 * t8 * t82 - t485) * t82
        t2665 = t15 * (t79 - t115)
        t2671 = t15 * (t676 - t1157)
        t2697 = (t1960 - t1631) * t69
        t2705 = (t2008 - t2460) * t69
        t2821 = t398 * (-t275 * ((t8 * ((t2665 - t2727) * t15 - t2732) *
     # t15 - t8 * (t2732 - (-t2671 + t2730) * t15) * t15) * t15 + (((t26
     #65 * t8 - t2563) * t15 - t2568) * t15 - (t2568 - (-t2671 * t8 + t2
     #566) * t15) * t15) * t15) / 0.24E2 + t2568 + t2576 - t310 * ((t8 *
     # ((t2697 - t2764) * t69 - t2769) * t69 - t8 * (t2769 - (-t2705 + t
     #2767) * t69) * t69) * t69 + (((t2697 * t8 - t2571) * t69 - t2576) 
     #* t69 - (t2576 - (-t2705 * t8 + t2574) * t69) * t69) * t69) / 0.24
     #E2 - t352 * ((t8 * ((t2800 * t82 - t464) * t82 - t467) * t82 - t47
     #3) * t82 + ((t2812 - t487) * t82 - t489) * t82) / 0.24E2 + t487 + 
     #t2580 + t2584)
        t2823 = t819 * t2821 / 0.4E1
        t2831 = (t8 * (t776 - t354) * t15 - t8 * (t354 - t1284) * t15) *
     # t15
        t2839 = (t8 * (t1703 - t354) * t69 - t8 * (t354 - t2204) * t69) 
     #* t69
        t2840 = src(i,j,t353,nComp,n)
        t2844 = (cc * (t2831 + t2839 + t2713 + t2840) - t2624) * t82
        t2847 = t1605 * (t2844 / 0.2E1 + t2626 / 0.2E1)
        t2849 = t721 * t2847 / 0.4E1
        t2895 = t539 * ((t8 * (t971 + t979 + t801 - t599 - t607 - t380) 
     #* t15 - t8 * (t599 + t607 + t380 - t1476 - t1484 - t1309) * t15) *
     # t15 + (t8 * (t1895 + t1900 + t1728 - t599 - t607 - t380) * t69 - 
     #t8 * (t599 + t607 + t380 - t2396 - t2401 - t2229) * t69) * t69 + (
     #t8 * (t2831 + t2839 + t2713 - t599 - t607 - t380) * t82 - t610) * 
     #t82 + (t8 * (t1016 - t650) * t15 - t8 * (t650 - t1521) * t15) * t1
     #5 + (t8 * (t1935 - t650) * t69 - t8 * (t650 - t2436) * t69) * t69 
     #+ (t8 * (t2840 - t650) * t82 - t653) * t82 + (t2579 - t2583) * t95
     #)
        t2897 = t923 * t2895 / 0.12E2
        t2928 = t1951 * ((cc * ((t8 * (t874 - t462) * t15 - t8 * (t462 -
     # t1381) * t15) * t15 + (t8 * (t1804 - t462) * t69 - t8 * (t462 - t
     #2305) * t69) * t69 + t2812 + (src(i,j,t353,nComp,t91) - t2840) * t
     #95 / 0.2E1 + (t2840 - src(i,j,t353,nComp,t98)) * t95 / 0.2E1) - t2
     #586) * t82 / 0.2E1 + t2588 / 0.2E1)
        t2930 = t819 * t2928 / 0.8E1
        t2932 = t2626 / 0.2E1 + t2630 / 0.2E1
        t2933 = t2622 * t2932
        t2935 = t721 * t2933 / 0.4E1
        t2937 = t819 * t2619 / 0.8E1
        t2939 = t721 * t2632 / 0.24E2
        t2948 = -t55 * t2619 / 0.8E1 - t176 * t2632 / 0.24E2 - t2639 - t
     #2644 - t2724 - t2823 + t2849 - t2897 + t2930 + t2935 + t2937 + t29
     #39 + t1048 * t2637 / 0.2E1 + t1051 * t2642 / 0.6E1 + t176 * t2722 
     #/ 0.2E1 + t55 * t2821 / 0.4E1
        t2957 = dz * t488
        t2963 = dt * (t465 - dz * t471 / 0.24E2)
        t2964 = t51 * t2963
        t2966 = t7 * t2957 / 0.24E2
        t2968 = t721 * dt
        t2971 = t310 * (t2844 - t2626) * t82
        t2973 = t2968 * t2971 / 0.24E2
        t2974 = t176 * dt
        t2977 = -t176 * t2847 / 0.4E1 + t537 * t2895 / 0.12E2 - t55 * t2
     #928 / 0.8E1 - t176 * t2933 / 0.4E1 - t48 * t2957 / 0.24E2 - t2964 
     #+ t2966 + t34 * t2963 - t397 - t504 - t665 + t1037 + t1039 + t1043
     # - t2973 + t2974 * t2971 / 0.24E2
        t2979 = (t2948 + t2977) * t4
        t2982 = t2639 + t2644 + t2724 + t2823 - t2849 + t2897 - t2930 - 
     #t2935 - t2937 - t2939 + t2964 - t2966
        t2984 = dz * t381 / 0.24E2
        t2986 = cc * t148
        t2987 = t2986 / 0.2E1
        t2988 = cc * t462
        t2990 = (-t2986 + t2988) * t82
        t2993 = (-t1069 + t2986) * t82
        t2994 = t2993 / 0.2E1
        t3001 = (t2990 - t2993) * t82
        t3003 = (((cc * t2799 - t2988) * t82 - t2990) * t82 - t3001) * t
     #82
        t3004 = cc * t152
        t3006 = (t1069 - t3004) * t82
        t3008 = (t2993 - t3006) * t82
        t3010 = (t3001 - t3008) * t82
        t3017 = dy * (t2990 / 0.2E1 + t2994 - t352 * (t3003 / 0.2E1 + t3
     #010 / 0.2E1) / 0.6E1) / 0.4E1
        t3018 = t3006 / 0.2E1
        t3019 = cc * t474
        t3021 = (t3004 - t3019) * t82
        t3023 = (t3006 - t3021) * t82
        t3025 = (t3008 - t3023) * t82
        t3032 = dy * (t2994 + t3018 - t352 * (t3010 / 0.2E1 + t3025 / 0.
     #2E1) / 0.6E1) / 0.4E1
        t3036 = t3008 - dz * (t3010 - t3025) / 0.12E2
        t3038 = t352 * t3036 / 0.24E2
        t3044 = t310 * (t3001 - dz * (t3003 - t3010) / 0.12E2) / 0.24E2
        t3048 = t8 * (t357 - dz * t363 / 0.24E2)
        t3049 = -t2979 * t6 - t1037 - t1039 - t1043 - t1070 + t2973 - t2
     #984 + t2987 - t3017 - t3032 - t3038 + t3044 + t3048
        t3055 = t2589 * t15
        t3058 = t2592 * t15
        t3060 = (t3055 - t3058) * t15
        t3092 = t2597 * t69
        t3095 = t2600 * t69
        t3097 = (t3092 - t3095) * t69
        t3127 = k - 3
        t3128 = ut(i,j,t3127,n)
        t3129 = t474 - t3128
        t3141 = (-t3129 * t8 * t82 + t491) * t82
        t2961 = t15 * (t85 - t119)
        t2972 = t15 * (t680 - t1161)
        t3013 = (t1964 - t1635) * t69
        t3024 = (t2012 - t2464) * t69
        t3150 = t398 * (-t275 * ((t8 * ((t2961 - t3055) * t15 - t3060) *
     # t15 - t8 * (t3060 - (-t2972 + t3058) * t15) * t15) * t15 + (((t29
     #61 * t8 - t2591) * t15 - t2596) * t15 - (t2596 - (-t2972 * t8 + t2
     #594) * t15) * t15) * t15) / 0.24E2 + t2596 + t2604 - t310 * ((t8 *
     # ((t3013 - t3092) * t69 - t3097) * t69 - t8 * (t3097 - (-t3024 + t
     #3095) * t69) * t69) * t69 + (((t3013 * t8 - t2599) * t69 - t2604) 
     #* t69 - (t2604 - (-t3024 * t8 + t2602) * t69) * t69) * t69) / 0.24
     #E2 - t352 * ((t481 - t8 * (t478 - (-t3129 * t82 + t476) * t82) * t
     #82) * t82 + (t495 - (t493 - t3141) * t82) * t82) / 0.24E2 + t493 +
     # t2608 + t2612)
        t3152 = t819 * t3150 / 0.4E1
        t3160 = (t8 * (t788 - t367) * t15 - t8 * (t367 - t1296) * t15) *
     # t15
        t3168 = (t8 * (t1715 - t367) * t69 - t8 * (t367 - t2216) * t69) 
     #* t69
        t3170 = t367 - u(i,j,t3127,n)
        t3174 = (-t3170 * t8 * t82 + t384) * t82
        t3175 = src(i,j,t366,nComp,n)
        t3179 = (t2628 - cc * (t3160 + t3168 + t3174 + t3175)) * t82
        t3182 = t2622 * (t2630 / 0.2E1 + t3179 / 0.2E1)
        t3184 = t721 * t3182 / 0.4E1
        t3230 = t539 * ((t8 * (t987 + t995 + t807 - t618 - t626 - t386) 
     #* t15 - t8 * (t618 + t626 + t386 - t1492 - t1500 - t1315) * t15) *
     # t15 + (t8 * (t1911 + t1916 + t1734 - t618 - t626 - t386) * t69 - 
     #t8 * (t618 + t626 + t386 - t2412 - t2417 - t2235) * t69) * t69 + (
     #t629 - t8 * (t618 + t626 + t386 - t3160 - t3168 - t3174) * t82) * 
     #t82 + (t8 * (t1020 - t654) * t15 - t8 * (t654 - t1525) * t15) * t1
     #5 + (t8 * (t1939 - t654) * t69 - t8 * (t654 - t2440) * t69) * t69 
     #+ (t657 - t8 * (t654 - t3175) * t82) * t82 + (t2607 - t2611) * t95
     #)
        t3232 = t923 * t3230 / 0.12E2
        t3263 = t2560 * (t2616 / 0.2E1 + (t2614 - cc * ((t8 * (t886 - t4
     #74) * t15 - t8 * (t474 - t1393) * t15) * t15 + (t8 * (t1816 - t474
     #) * t69 - t8 * (t474 - t2317) * t69) * t69 + t3141 + (src(i,j,t366
     #,nComp,t91) - t3175) * t95 / 0.2E1 + (t3175 - src(i,j,t366,nComp,t
     #98)) * t95 / 0.2E1)) * t82 / 0.2E1)
        t3265 = t819 * t3263 / 0.8E1
        t3267 = t2622 * (t2630 - t3179)
        t3269 = t721 * t3267 / 0.24E2
        t3272 = t56 * (t245 + t255 + t265 + t159 - t618 - t626 - t386 - 
     #t654) * t82
        t3277 = t538 * (t28 + t147 + t157 + t162 + t166 - t2596 - t2604 
     #- t493 - t2608 - t2612) * t82
        t3280 = t1605 * t2932
        t3283 = t1951 * t2618
        t3287 = t611 * t15
        t3290 = t614 * t15
        t3292 = (t3287 - t3290) * t15
        t3314 = t619 * t69
        t3317 = t622 * t69
        t3319 = (t3314 - t3317) * t69
        t3356 = t274 * (-t275 * ((t8 * ((t15 * t983 - t3287) * t15 - t32
     #92) * t15 - t8 * (t3292 - (-t1441 + t3290) * t15) * t15) * t15 + (
     #(t987 - t618) * t15 - (t618 - t1492) * t15) * t15) / 0.24E2 + t618
     # + t626 - t310 * ((t8 * ((t1851 - t3314) * t69 - t3319) * t69 - t8
     # * (t3319 - (-t2332 + t3317) * t69) * t69) * t69 + ((t1916 - t626)
     # * t69 - (t626 - t2417) * t69) * t69) / 0.24E2 - t352 * ((t374 - t
     #8 * (t371 - (-t3170 * t82 + t369) * t82) * t82) * t82 + (t388 - (t
     #386 - t3174) * t82) * t82) / 0.24E2 + t386 + t654)
        t3370 = t708 * t3272 / 0.2E1
        t3371 = t3152 + t3184 + t3232 + t3265 + t3269 + t1048 * t3272 / 
     #0.2E1 + t1051 * t3277 / 0.6E1 - t176 * t3280 / 0.4E1 - t55 * t3283
     # / 0.8E1 - t176 * t3356 / 0.2E1 - t55 * t3150 / 0.4E1 - t176 * t31
     #82 / 0.4E1 - t537 * t3230 / 0.12E2 - t55 * t3263 / 0.8E1 - t176 * 
     #t3267 / 0.24E2 - t3370
        t3373 = t715 * t3277 / 0.6E1
        t3375 = t721 * t3280 / 0.4E1
        t3377 = t819 * t3283 / 0.8E1
        t3379 = t721 * t3356 / 0.2E1
        t3381 = t310 * t2631 * t82
        t3383 = t2968 * t3381 / 0.24E2
        t3389 = dt * (t468 - dz * t479 / 0.24E2)
        t3391 = dz * t494
        t3394 = t51 * t3389
        t3396 = t7 * t3391 / 0.24E2
        t3397 = -t3373 + t3375 + t3377 + t3379 + t397 + t504 + t665 - t1
     #037 - t1039 - t1043 - t3383 + t2974 * t3381 / 0.24E2 + t34 * t3389
     # - t48 * t3391 / 0.24E2 - t3394 + t3396
        t3399 = (t3371 + t3397) * t4
        t3409 = (t3023 - (t3021 - (-cc * t3128 + t3019) * t82) * t82) * 
     #t82
        t3416 = dy * (t3018 + t3021 / 0.2E1 - t352 * (t3025 / 0.2E1 + t3
     #409 / 0.2E1) / 0.6E1) / 0.4E1
        t3417 = t3004 / 0.2E1
        t3418 = -t3152 - t3184 - t3232 - t3265 - t3269 + t3370 + t3373 -
     # t3375 - t3377 - t3379 - t3416 - t3417
        t3420 = t310 * t3036 / 0.24E2
        t3426 = t352 * (t3023 - dz * (t3025 - t3409) / 0.12E2) / 0.24E2
        t3431 = t8 * (t360 - dz * t372 / 0.24E2)
        t3433 = dz * t387 / 0.24E2
        t3434 = -t3399 * t6 + t1037 + t1039 + t1043 + t1070 - t3032 + t3
     #383 + t3394 - t3396 + t3420 - t3426 + t3431 - t3433
        t3443 = src(i,j,k,nComp,n + 2)
        t3445 = (src(i,j,k,nComp,n + 3) - t3443) * t4
        t3455 = t1135 + t52 + t713 - t1131 + t720 - t32 + t1072 + t818 -
     # t1129 + t916 - t922 + t1121
        t3456 = t1031 - t1033 + t1035 - t1070 - t1037 - t1108 - t1039 - 
     #t1041 - t1099 - t1043 - t1045 - t1047
        t3462 = t1595 + t1561 + t1223 - t1591 + t1228 - t1563 + t1070 + 
     #t1037 - t1108 + t1039 - t1041 + t1099
        t3463 = t1043 - t1045 + t1047 - t1598 - t1326 - t1583 - t1423 - 
     #t1428 - t1589 - t1536 - t1538 - t1540
        t3471 = t2117 + t2045 + t1622 - t2113 + t1653 - t2047 + t2139 + 
     #t1745 - t2111 + t1846 - t1874 + t2102
        t3472 = t1950 - t1990 + t1994 - t1070 - t1037 - t2138 - t1039 - 
     #t1999 - t2130 - t1043 - t2034 - t2036
        t3478 = t2546 + t2512 + t2492 - t2527 + t2494 - t2515 + t1070 + 
     #t1037 - t2138 + t1039 - t1999 + t2130
        t3479 = t1043 - t2034 + t2036 - t2524 - t2496 - t2542 - t2498 - 
     #t2500 - t2552 - t2502 - t2504 - t2506
        t3487 = t3048 + t2964 + t2639 - t2984 + t2644 - t2966 + t2987 + 
     #t2724 - t3017 + t2823 - t2849 + t3044
        t3488 = t2897 - t2930 + t2973 - t1070 - t1037 - t3032 - t1039 - 
     #t2935 - t3038 - t1043 - t2937 - t2939
        t3494 = t3431 + t3394 + t3370 - t3433 + t3373 - t3396 + t1070 + 
     #t1037 - t3032 + t1039 - t3375 + t3420
        t3495 = t1043 - t3377 + t3383 - t3417 - t3379 - t3416 - t3152 - 
     #t3184 - t3426 - t3232 - t3265 - t3269


        unew(i,j,k) = t1 + dt * t2 + (t1064 * t56 / 0.6E1 + (t1067 + 
     #t1136) * t56 / 0.2E1 - t1566 * t56 / 0.6E1 - (t1597 + t1599) * t56
     # / 0.2E1) * t15 + (t2071 * t56 / 0.6E1 + (t2140 + t2141) * t56 / 0
     #.2E1 - t2521 * t56 / 0.6E1 - (t2525 + t2554) * t56 / 0.2E1) * t69 
     #+ (t2979 * t56 / 0.6E1 + (t2982 + t3049) * t56 / 0.2E1 - t3399 * t
     #56 / 0.6E1 - (t3418 + t3434) * t56 / 0.2E1) * t82 + t3445 * t56 / 
     #0.6E1 + (-t3445 * t6 + t3443) * t56 / 0.2E1

        utnew(i,j,k) = t2 + (t1064 * dt 
     #/ 0.2E1 + (t3455 + t3456) * dt - t1064 * t7 - t1566 * dt / 0.2E1 -
     # (t3462 + t3463) * dt + t1566 * t7) * t15 + (t2071 * dt / 0.2E1 + 
     #(t3471 + t3472) * dt - t2071 * t7 - t2521 * dt / 0.2E1 - (t3478 + 
     #t3479) * dt + t2521 * t7) * t69 + (t2979 * dt / 0.2E1 + (t3487 + t
     #3488) * dt - t2979 * t7 - t3399 * dt / 0.2E1 - (t3494 + t3495) * d
     #t + t3399 * t7) * t82 + t3445 * dt / 0.2E1 + t3443 * dt - t3445 * 
     #t7

        return
      end
