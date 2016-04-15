      subroutine duStepWaveGen3d4rc( 
     *   nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *   n1a,n1b,n2a,n2b,n3a,n3b,
     *   u,ut,unew,utnew,
     *   dx,dy,dz,dt,cc,beta,
     *   i,j,k,n )

      implicit none
c
c.. declarations of incoming variables      
      integer nd1a,nd1b,nd2a,nd2b,nd3a,nd3b
      integer n1a,n1b,n2a,n2b,n3a,n3b
      integer i,j,k,n

      real u    (nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,*)
      real ut   (nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,*)
      real unew (nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real utnew(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real dx,dy,dz,dt,cc,beta
c
c.. generated code to follow
c
        real t1
        real t10
        real t100
        real t1001
        real t1002
        real t1004
        real t1006
        real t1008
        real t1014
        real t1015
        real t102
        real t1021
        real t1023
        real t1024
        real t103
        real t1031
        real t1035
        real t1036
        real t1037
        real t1039
        integer t104
        integer t1043
        real t1044
        real t1045
        real t105
        real t1057
        real t106
        real t1065
        real t1066
        real t1068
        real t107
        real t1071
        real t1073
        real t1077
        real t1078
        real t108
        integer t11
        real t110
        real t1104
        real t1105
        real t1107
        real t111
        real t1110
        real t1112
        real t1116
        real t1117
        real t113
        real t114
        real t1144
        real t1148
        real t1152
        real t1153
        real t1157
        real t116
        real t1162
        real t1163
        real t1167
        integer t117
        real t1172
        real t1176
        real t1179
        real t118
        real t1187
        real t119
        real t1191
        real t1192
        real t1193
        real t1197
        real t1198
        real t12
        real t120
        real t1202
        real t1207
        real t1211
        real t1215
        real t1216
        real t1217
        real t122
        real t1221
        real t1222
        real t1226
        real t123
        real t1231
        real t1237
        real t1241
        real t1249
        real t125
        real t1250
        real t1251
        real t1255
        real t1259
        real t1263
        real t1271
        real t1272
        real t1273
        real t1277
        real t1284
        real t1287
        real t129
        real t1291
        real t1297
        real t13
        real t1301
        real t131
        real t1313
        real t1317
        real t132
        real t1321
        real t1323
        real t1326
        real t1328
        real t133
        real t1345
        real t1348
        real t135
        real t1350
        real t137
        real t1372
        real t1375
        real t1377
        real t138
        real t139
        real t1399
        real t1401
        real t1403
        real t1405
        real t1407
        real t1409
        real t1411
        real t1418
        real t1419
        real t1423
        real t1427
        real t1429
        real t1430
        real t1432
        real t1435
        real t1436
        real t1444
        integer t145
        real t1450
        real t1458
        real t146
        real t1460
        real t1464
        real t1465
        real t147
        real t1471
        real t1472
        real t1474
        real t1475
        real t1477
        real t1479
        real t1480
        real t1481
        real t1483
        real t1484
        real t1485
        real t1487
        real t1489
        real t149
        real t1491
        real t1493
        real t1494
        real t1496
        real t1497
        real t1499
        real t15
        real t150
        real t1501
        real t1502
        real t1503
        real t1505
        real t1506
        real t1507
        real t1509
        integer t151
        real t1511
        real t1513
        real t1515
        real t1518
        real t152
        real t1520
        real t1521
        real t1523
        real t1525
        real t1527
        real t1529
        real t153
        real t1531
        real t1533
        integer t1542
        real t1543
        real t1544
        real t1548
        real t1549
        real t155
        real t1553
        real t1565
        real t157
        real t1575
        real t1577
        real t158
        real t1581
        real t1582
        real t1586
        integer t159
        real t1591
        real t1595
        real t1597
        real t16
        real t160
        real t1602
        real t1604
        real t1606
        real t1608
        real t161
        real t1610
        real t1612
        real t1615
        real t1617
        real t1619
        real t162
        real t1622
        real t1624
        real t163
        real t165
        real t166
        real t1660
        real t1661
        real t1663
        real t1666
        real t1668
        real t1672
        real t1673
        real t168
        real t1685
        real t169
        real t1691
        real t17
        real t1700
        real t1702
        real t1703
        real t1705
        real t1708
        real t171
        real t1710
        real t1714
        integer t172
        real t173
        real t174
        real t175
        real t1755
        real t1756
        real t1758
        real t1761
        real t1763
        real t1767
        real t1768
        real t177
        real t178
        real t1795
        real t1797
        real t180
        real t1800
        real t1802
        real t1823
        real t1824
        real t1828
        real t1839
        real t184
        real t1840
        real t1844
        real t1851
        real t1853
        real t1855
        real t1857
        real t1859
        real t186
        real t1860
        real t187
        real t1876
        real t1878
        real t1879
        real t188
        real t1883
        real t1884
        real t1886
        real t1889
        real t1891
        real t1892
        real t1893
        real t1895
        real t1896
        real t1897
        real t1899
        real t19
        real t190
        real t1901
        real t1903
        real t1905
        real t1906
        real t1908
        real t1910
        real t1912
        real t1919
        real t192
        real t1920
        real t1928
        real t193
        real t1935
        real t1937
        real t194
        real t1943
        real t1944
        real t1950
        real t1954
        real t1956
        integer t1968
        real t1969
        real t1970
        real t1974
        real t1975
        real t1979
        real t1991
        real t1993
        real t2
        real t2001
        real t2003
        real t2007
        real t2008
        real t201
        real t2012
        real t2017
        real t2021
        real t2023
        real t2025
        real t2027
        real t203
        real t2030
        real t2032
        real t204
        real t205
        real t206
        real t2068
        real t2069
        real t207
        real t2071
        real t2074
        real t2076
        real t208
        real t2080
        real t2081
        real t209
        real t2093
        real t2099
        real t21
        real t210
        real t2108
        real t2110
        real t2111
        real t2113
        real t2116
        real t2118
        real t212
        real t2122
        real t213
        real t215
        real t216
        real t2163
        real t2164
        real t2166
        real t2169
        real t2171
        real t2175
        real t2176
        real t218
        real t219
        integer t22
        real t220
        real t2203
        real t2205
        real t2208
        real t221
        real t2210
        real t223
        real t2231
        real t2232
        real t2236
        real t224
        real t2247
        real t2248
        real t2252
        real t2259
        real t226
        real t2261
        real t2264
        real t2266
        real t2272
        real t2274
        real t2275
        real t229
        real t2291
        real t2293
        real t2294
        real t2298
        real t2299
        real t23
        real t2301
        real t2304
        real t2305
        real t231
        real t2312
        real t2318
        real t2320
        real t2324
        real t233
        real t2332
        real t2334
        real t234
        real t2340
        real t2341
        real t2343
        real t2344
        real t2346
        real t2348
        real t2349
        real t235
        real t2351
        real t2352
        real t2354
        real t2356
        real t2358
        real t2360
        real t2361
        real t2363
        real t2364
        real t2366
        real t2368
        real t2369
        real t2371
        real t2372
        real t2374
        real t2376
        real t2378
        real t2380
        real t2383
        real t2385
        real t2386
        real t2388
        real t2390
        real t2392
        real t2394
        real t2396
        real t2398
        real t24
        real t2400
        real t2405
        real t2409
        real t241
        real t2412
        real t2414
        real t242
        real t2436
        real t2439
        real t244
        real t2441
        real t245
        real t246
        integer t2462
        real t2464
        real t2476
        real t248
        real t2485
        real t2488
        real t2490
        real t2493
        real t2495
        real t2499
        real t250
        real t251
        real t252
        real t2525
        real t2527
        real t253
        real t2530
        real t2532
        real t2536
        real t254
        real t256
        real t2562
        real t2563
        real t257
        real t2575
        real t2584
        real t259
        real t2594
        real t26
        real t260
        real t2602
        real t2606
        real t2609
        real t262
        real t263
        real t2634
        real t264
        real t265
        real t2659
        real t2663
        real t2668
        real t267
        real t2670
        real t2672
        real t2674
        real t2676
        real t2678
        real t268
        real t2680
        real t2681
        real t2683
        real t2685
        real t2687
        real t2689
        real t2691
        real t2693
        real t2697
        real t2699
        real t270
        real t2700
        real t2702
        real t2705
        real t2707
        real t2710
        real t2712
        real t2714
        real t2715
        real t2716
        real t2718
        real t2719
        real t2720
        real t2722
        real t2724
        real t2726
        real t2728
        real t2729
        real t2731
        real t2733
        real t2735
        real t274
        real t2742
        real t2749
        real t2755
        real t2759
        real t276
        real t2760
        real t2766
        real t277
        real t2774
        real t2776
        real t2777
        real t278
        real t2782
        real t2785
        real t2787
        real t28
        real t280
        real t2809
        real t2812
        real t2814
        real t282
        real t283
        integer t2835
        real t2837
        real t284
        real t2849
        real t2858
        real t2861
        real t2863
        real t2866
        real t2868
        real t2872
        real t2898
        real t29
        real t290
        real t2900
        real t2903
        real t2905
        real t2909
        real t291
        real t293
        real t2935
        real t2936
        real t294
        real t2948
        real t295
        real t2957
        real t2967
        real t297
        real t2975
        real t2979
        real t2982
        real t299
        real t30
        real t300
        real t3007
        real t301
        real t302
        real t303
        real t3032
        real t3036
        real t3040
        real t3042
        real t3045
        real t3047
        real t3049
        real t305
        real t3051
        real t3052
        real t3054
        real t3056
        real t3058
        real t306
        real t3060
        real t3068
        real t3070
        real t3071
        real t3075
        real t3076
        real t3078
        real t308
        real t3082
        real t3083
        real t309
        real t3090
        real t3096
        real t3100
        real t3108
        real t311
        real t3110
        real t3111
        real t312
        real t3120
        real t3121
        real t3127
        real t3128
        real t313
        real t3136
        real t3137
        real t314
        real t3143
        real t3144
        real t3152
        real t3153
        real t3159
        real t316
        real t3160
        real t317
        real t319
        real t32
        real t323
        real t325
        real t326
        real t327
        real t329
        real t33
        real t331
        real t332
        real t333
        real t34
        real t340
        real t342
        real t343
        real t344
        real t345
        real t347
        real t348
        real t349
        real t351
        real t353
        real t354
        real t355
        real t357
        real t358
        real t359
        real t361
        real t363
        real t365
        real t367
        real t369
        real t37
        real t370
        real t371
        real t373
        real t374
        real t375
        real t377
        real t379
        real t38
        real t380
        real t381
        real t383
        real t384
        real t385
        real t387
        real t389
        real t39
        real t391
        real t393
        real t396
        real t398
        real t399
        real t4
        real t40
        real t400
        real t401
        real t402
        real t403
        real t404
        real t406
        real t407
        real t409
        real t412
        real t414
        real t415
        real t417
        real t419
        real t42
        real t420
        real t421
        real t423
        real t424
        real t425
        real t427
        real t429
        real t43
        real t430
        real t432
        real t433
        real t435
        real t436
        real t438
        real t44
        real t440
        real t441
        real t442
        real t444
        real t445
        real t446
        real t448
        real t450
        real t451
        real t453
        real t456
        real t458
        real t459
        real t46
        real t461
        real t463
        real t464
        real t466
        real t467
        real t469
        real t471
        real t472
        real t474
        real t475
        real t477
        real t478
        real t48
        real t480
        real t482
        real t483
        real t485
        real t486
        real t488
        real t49
        real t490
        real t491
        real t493
        real t497
        real t499
        real t5
        real t50
        real t500
        real t501
        real t502
        real t504
        real t505
        real t506
        real t508
        real t51
        real t510
        real t511
        real t512
        real t514
        real t515
        real t516
        real t518
        real t52
        real t520
        real t522
        real t524
        real t526
        real t527
        real t528
        real t53
        real t530
        real t531
        real t532
        real t534
        real t536
        real t537
        real t538
        real t540
        real t541
        real t542
        real t544
        real t546
        real t548
        real t55
        real t550
        real t553
        real t555
        real t557
        real t559
        real t56
        real t560
        real t561
        real t563
        real t566
        real t567
        real t570
        real t573
        integer t574
        real t576
        real t58
        real t588
        real t59
        real t596
        real t597
        real t599
        real t6
        real t602
        real t604
        real t608
        real t609
        real t61
        integer t62
        real t621
        real t627
        real t63
        real t635
        real t636
        real t638
        real t64
        real t641
        real t643
        real t647
        real t648
        real t65
        real t660
        real t666
        real t67
        real t675
        real t678
        real t679
        real t68
        real t680
        real t692
        real t7
        real t70
        real t700
        real t701
        real t703
        real t706
        real t708
        real t712
        real t713
        real t739
        real t74
        real t740
        real t742
        real t745
        real t747
        real t751
        real t752
        real t76
        real t77
        real t779
        real t78
        real t782
        real t786
        real t791
        real t792
        real t796
        real t8
        real t80
        real t801
        real t805
        real t808
        real t811
        real t817
        real t82
        real t821
        real t822
        real t826
        real t83
        real t831
        real t835
        real t839
        real t84
        real t840
        real t844
        real t849
        real t855
        real t859
        real t867
        real t871
        real t875
        real t883
        real t890
        real t893
        real t897
        integer t9
        integer t90
        real t903
        real t907
        real t91
        real t919
        real t92
        real t923
        real t926
        real t928
        real t930
        real t932
        real t934
        real t936
        real t938
        real t939
        real t94
        real t941
        real t942
        real t944
        real t946
        real t948
        real t95
        real t950
        real t952
        real t954
        real t956
        real t957
        integer t96
        real t961
        real t963
        real t964
        real t965
        real t967
        real t97
        real t970
        real t971
        real t973
        real t975
        real t977
        real t978
        real t98
        real t985
        real t987
        real t988
        real t990
        real t992
        real t994
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
        t34 = t33 * dt
        t37 = beta * t6
        t38 = dt * cc
        t39 = u(t11,j,k,n)
        t40 = t39 - t1
        t42 = t8 * t40 * t15
        t43 = u(t22,j,k,n)
        t44 = t1 - t43
        t46 = t8 * t44 * t15
        t48 = (t42 - t46) * t15
        t49 = dx ** 2
        t50 = u(t9,j,k,n)
        t51 = t50 - t39
        t52 = t51 * t15
        t53 = t40 * t15
        t55 = (t52 - t53) * t15
        t56 = t44 * t15
        t58 = (t53 - t56) * t15
        t59 = t55 - t58
        t61 = t8 * t59 * t15
        t62 = i - 2
        t63 = u(t62,j,k,n)
        t64 = t43 - t63
        t65 = t64 * t15
        t67 = (t56 - t65) * t15
        t68 = t58 - t67
        t70 = t8 * t68 * t15
        t74 = t8 * t51 * t15
        t76 = (t74 - t42) * t15
        t77 = t76 - t48
        t78 = t77 * t15
        t80 = t8 * t64 * t15
        t82 = (t46 - t80) * t15
        t83 = t48 - t82
        t84 = t83 * t15
        t90 = j + 1
        t91 = u(i,t90,k,n)
        t92 = t91 - t1
        t94 = 0.1E1 / dy
        t95 = t8 * t92 * t94
        t96 = j - 1
        t97 = u(i,t96,k,n)
        t98 = t1 - t97
        t100 = t8 * t98 * t94
        t102 = (t95 - t100) * t94
        t103 = dy ** 2
        t104 = j + 2
        t105 = u(i,t104,k,n)
        t106 = t105 - t91
        t107 = t106 * t94
        t108 = t92 * t94
        t110 = (t107 - t108) * t94
        t111 = t98 * t94
        t113 = (t108 - t111) * t94
        t114 = t110 - t113
        t116 = t8 * t114 * t94
        t117 = j - 2
        t118 = u(i,t117,k,n)
        t119 = t97 - t118
        t120 = t119 * t94
        t122 = (t111 - t120) * t94
        t123 = t113 - t122
        t125 = t8 * t123 * t94
        t129 = t8 * t106 * t94
        t131 = (t129 - t95) * t94
        t132 = t131 - t102
        t133 = t132 * t94
        t135 = t8 * t119 * t94
        t137 = (t100 - t135) * t94
        t138 = t102 - t137
        t139 = t138 * t94
        t145 = k + 1
        t146 = u(i,j,t145,n)
        t147 = t146 - t1
        t149 = 0.1E1 / dz
        t150 = t8 * t147 * t149
        t151 = k - 1
        t152 = u(i,j,t151,n)
        t153 = t1 - t152
        t155 = t8 * t153 * t149
        t157 = (t150 - t155) * t149
        t158 = dz ** 2
        t159 = k + 2
        t160 = u(i,j,t159,n)
        t161 = t160 - t146
        t162 = t161 * t149
        t163 = t147 * t149
        t165 = (t162 - t163) * t149
        t166 = t153 * t149
        t168 = (t163 - t166) * t149
        t169 = t165 - t168
        t171 = t8 * t169 * t149
        t172 = k - 2
        t173 = u(i,j,t172,n)
        t174 = t152 - t173
        t175 = t174 * t149
        t177 = (t166 - t175) * t149
        t178 = t168 - t177
        t180 = t8 * t178 * t149
        t184 = t8 * t161 * t149
        t186 = (t184 - t150) * t149
        t187 = t186 - t157
        t188 = t187 * t149
        t190 = t8 * t174 * t149
        t192 = (t155 - t190) * t149
        t193 = t157 - t192
        t194 = t193 * t149
        t201 = t38 * (t48 - t49 * ((t61 - t70) * t15 + (t78 - t84) * t15
     #) / 0.24E2 + t102 - t103 * ((t116 - t125) * t94 + (t133 - t139) * 
     #t94) / 0.24E2 + t157 - t158 * ((t171 - t180) * t149 + (t188 - t194
     #) * t149) / 0.24E2)
        t203 = t37 * t201 / 0.2E1
        t204 = beta ** 2
        t205 = t6 ** 2
        t206 = t204 * t205
        t207 = dt ** 2
        t208 = t207 * cc
        t209 = t13 * t15
        t210 = t17 * t15
        t212 = (t209 - t210) * t15
        t213 = t24 * t15
        t215 = (t210 - t213) * t15
        t216 = t212 - t215
        t218 = t8 * t216 * t15
        t219 = ut(t62,j,k,n)
        t220 = t23 - t219
        t221 = t220 * t15
        t223 = (t213 - t221) * t15
        t224 = t215 - t223
        t226 = t8 * t224 * t15
        t229 = t29 * t15
        t231 = t8 * t220 * t15
        t233 = (t26 - t231) * t15
        t234 = t28 - t233
        t235 = t234 * t15
        t241 = ut(i,t90,k,n)
        t242 = t241 - t2
        t244 = t8 * t242 * t94
        t245 = ut(i,t96,k,n)
        t246 = t2 - t245
        t248 = t8 * t246 * t94
        t250 = (t244 - t248) * t94
        t251 = ut(i,t104,k,n)
        t252 = t251 - t241
        t253 = t252 * t94
        t254 = t242 * t94
        t256 = (t253 - t254) * t94
        t257 = t246 * t94
        t259 = (t254 - t257) * t94
        t260 = t256 - t259
        t262 = t8 * t260 * t94
        t263 = ut(i,t117,k,n)
        t264 = t245 - t263
        t265 = t264 * t94
        t267 = (t257 - t265) * t94
        t268 = t259 - t267
        t270 = t8 * t268 * t94
        t274 = t8 * t252 * t94
        t276 = (t274 - t244) * t94
        t277 = t276 - t250
        t278 = t277 * t94
        t280 = t8 * t264 * t94
        t282 = (t248 - t280) * t94
        t283 = t250 - t282
        t284 = t283 * t94
        t290 = ut(i,j,t145,n)
        t291 = t290 - t2
        t293 = t8 * t291 * t149
        t294 = ut(i,j,t151,n)
        t295 = t2 - t294
        t297 = t8 * t295 * t149
        t299 = (t293 - t297) * t149
        t300 = ut(i,j,t159,n)
        t301 = t300 - t290
        t302 = t301 * t149
        t303 = t291 * t149
        t305 = (t302 - t303) * t149
        t306 = t295 * t149
        t308 = (t303 - t306) * t149
        t309 = t305 - t308
        t311 = t8 * t309 * t149
        t312 = ut(i,j,t172,n)
        t313 = t294 - t312
        t314 = t313 * t149
        t316 = (t306 - t314) * t149
        t317 = t308 - t316
        t319 = t8 * t317 * t149
        t323 = t8 * t301 * t149
        t325 = (t323 - t293) * t149
        t326 = t325 - t299
        t327 = t326 * t149
        t329 = t8 * t313 * t149
        t331 = (t297 - t329) * t149
        t332 = t299 - t331
        t333 = t332 * t149
        t340 = t208 * (t28 - t49 * ((t218 - t226) * t15 + (t229 - t235) 
     #* t15) / 0.24E2 + t250 - t103 * ((t262 - t270) * t94 + (t278 - t28
     #4) * t94) / 0.24E2 + t299 - t158 * ((t311 - t319) * t149 + (t327 -
     # t333) * t149) / 0.24E2)
        t342 = t206 * t340 / 0.4E1
        t343 = dt * dx
        t344 = u(t11,t90,k,n)
        t345 = t344 - t39
        t347 = t8 * t345 * t94
        t348 = u(t11,t96,k,n)
        t349 = t39 - t348
        t351 = t8 * t349 * t94
        t353 = (t347 - t351) * t94
        t354 = u(t11,j,t145,n)
        t355 = t354 - t39
        t357 = t8 * t355 * t149
        t358 = u(t11,j,t151,n)
        t359 = t39 - t358
        t361 = t8 * t359 * t149
        t363 = (t357 - t361) * t149
        t365 = cc * (t76 + t353 + t363)
        t367 = cc * (t48 + t102 + t157)
        t369 = (t365 - t367) * t15
        t370 = u(t22,t90,k,n)
        t371 = t370 - t43
        t373 = t8 * t371 * t94
        t374 = u(t22,t96,k,n)
        t375 = t43 - t374
        t377 = t8 * t375 * t94
        t379 = (t373 - t377) * t94
        t380 = u(t22,j,t145,n)
        t381 = t380 - t43
        t383 = t8 * t381 * t149
        t384 = u(t22,j,t151,n)
        t385 = t43 - t384
        t387 = t8 * t385 * t149
        t389 = (t383 - t387) * t149
        t391 = cc * (t82 + t379 + t389)
        t393 = (t367 - t391) * t15
        t396 = t343 * (t369 / 0.2E1 + t393 / 0.2E1)
        t398 = t37 * t396 / 0.4E1
        t399 = t204 * beta
        t400 = t205 * t6
        t401 = t399 * t400
        t402 = t207 * dt
        t403 = t402 * cc
        t404 = t76 + t353 + t363 - t48 - t102 - t157
        t406 = t8 * t404 * t15
        t407 = t48 + t102 + t157 - t82 - t379 - t389
        t409 = t8 * t407 * t15
        t412 = t344 - t91
        t414 = t8 * t412 * t15
        t415 = t91 - t370
        t417 = t8 * t415 * t15
        t419 = (t414 - t417) * t15
        t420 = u(i,t90,t145,n)
        t421 = t420 - t91
        t423 = t8 * t421 * t149
        t424 = u(i,t90,t151,n)
        t425 = t91 - t424
        t427 = t8 * t425 * t149
        t429 = (t423 - t427) * t149
        t430 = t419 + t131 + t429 - t48 - t102 - t157
        t432 = t8 * t430 * t94
        t433 = t348 - t97
        t435 = t8 * t433 * t15
        t436 = t97 - t374
        t438 = t8 * t436 * t15
        t440 = (t435 - t438) * t15
        t441 = u(i,t96,t145,n)
        t442 = t441 - t97
        t444 = t8 * t442 * t149
        t445 = u(i,t96,t151,n)
        t446 = t97 - t445
        t448 = t8 * t446 * t149
        t450 = (t444 - t448) * t149
        t451 = t48 + t102 + t157 - t440 - t137 - t450
        t453 = t8 * t451 * t94
        t456 = t354 - t146
        t458 = t8 * t456 * t15
        t459 = t146 - t380
        t461 = t8 * t459 * t15
        t463 = (t458 - t461) * t15
        t464 = t420 - t146
        t466 = t8 * t464 * t94
        t467 = t146 - t441
        t469 = t8 * t467 * t94
        t471 = (t466 - t469) * t94
        t472 = t463 + t471 + t186 - t48 - t102 - t157
        t474 = t8 * t472 * t149
        t475 = t358 - t152
        t477 = t8 * t475 * t15
        t478 = t152 - t384
        t480 = t8 * t478 * t15
        t482 = (t477 - t480) * t15
        t483 = t424 - t152
        t485 = t8 * t483 * t94
        t486 = t152 - t445
        t488 = t8 * t486 * t94
        t490 = (t485 - t488) * t94
        t491 = t48 + t102 + t157 - t482 - t490 - t192
        t493 = t8 * t491 * t149
        t497 = t403 * ((t406 - t409) * t15 + (t432 - t453) * t94 + (t474
     # - t493) * t149)
        t499 = t401 * t497 / 0.12E2
        t500 = t207 * dx
        t501 = ut(t11,t90,k,n)
        t502 = t501 - t12
        t504 = t8 * t502 * t94
        t505 = ut(t11,t96,k,n)
        t506 = t12 - t505
        t508 = t8 * t506 * t94
        t510 = (t504 - t508) * t94
        t511 = ut(t11,j,t145,n)
        t512 = t511 - t12
        t514 = t8 * t512 * t149
        t515 = ut(t11,j,t151,n)
        t516 = t12 - t515
        t518 = t8 * t516 * t149
        t520 = (t514 - t518) * t149
        t522 = cc * (t21 + t510 + t520)
        t524 = cc * (t28 + t250 + t299)
        t526 = (t522 - t524) * t15
        t527 = ut(t22,t90,k,n)
        t528 = t527 - t23
        t530 = t8 * t528 * t94
        t531 = ut(t22,t96,k,n)
        t532 = t23 - t531
        t534 = t8 * t532 * t94
        t536 = (t530 - t534) * t94
        t537 = ut(t22,j,t145,n)
        t538 = t537 - t23
        t540 = t8 * t538 * t149
        t541 = ut(t22,j,t151,n)
        t542 = t23 - t541
        t544 = t8 * t542 * t149
        t546 = (t540 - t544) * t149
        t548 = cc * (t233 + t536 + t546)
        t550 = (t524 - t548) * t15
        t553 = t500 * (t526 / 0.2E1 + t550 / 0.2E1)
        t555 = t206 * t553 / 0.8E1
        t557 = t343 * (t369 - t393)
        t559 = t37 * t557 / 0.24E2
        t560 = t33 ** 2
        t561 = t8 * t560
        t563 = t207 * t404 * t15
        t566 = t560 * t33
        t567 = t8 * t566
        t570 = t402 * (t21 + t510 + t520 - t28 - t250 - t299) * t15
        t573 = beta * t33
        t574 = i + 3
        t576 = u(t574,j,k,n) - t50
        t588 = (t15 * t576 * t8 - t74) * t15
        t596 = u(t11,t104,k,n)
        t597 = t596 - t344
        t599 = t345 * t94
        t602 = t349 * t94
        t604 = (t599 - t602) * t94
        t608 = u(t11,t117,k,n)
        t609 = t348 - t608
        t621 = (t597 * t8 * t94 - t347) * t94
        t627 = (-t609 * t8 * t94 + t351) * t94
        t635 = u(t11,j,t159,n)
        t636 = t635 - t354
        t638 = t355 * t149
        t641 = t359 * t149
        t643 = (t638 - t641) * t149
        t647 = u(t11,j,t172,n)
        t648 = t358 - t647
        t660 = (t149 * t636 * t8 - t357) * t149
        t666 = (-t149 * t648 * t8 + t361) * t149
        t675 = t38 * (t76 - t49 * ((t8 * ((t15 * t576 - t52) * t15 - t55
     #) * t15 - t61) * t15 + ((t588 - t76) * t15 - t78) * t15) / 0.24E2 
     #+ t353 - t103 * ((t8 * ((t597 * t94 - t599) * t94 - t604) * t94 - 
     #t8 * (t604 - (-t609 * t94 + t602) * t94) * t94) * t94 + ((t621 - t
     #353) * t94 - (t353 - t627) * t94) * t94) / 0.24E2 + t363 - t158 * 
     #((t8 * ((t149 * t636 - t638) * t149 - t643) * t149 - t8 * (t643 - 
     #(-t149 * t648 + t641) * t149) * t149) * t149 + ((t660 - t363) * t1
     #49 - (t363 - t666) * t149) * t149) / 0.24E2)
        t678 = t204 * t560
        t679 = ut(t574,j,k,n)
        t680 = t679 - t10
        t692 = (t15 * t680 * t8 - t16) * t15
        t700 = ut(t11,t104,k,n)
        t701 = t700 - t501
        t703 = t502 * t94
        t706 = t506 * t94
        t708 = (t703 - t706) * t94
        t712 = ut(t11,t117,k,n)
        t713 = t505 - t712
        t739 = ut(t11,j,t159,n)
        t740 = t739 - t511
        t742 = t512 * t149
        t745 = t516 * t149
        t747 = (t742 - t745) * t149
        t751 = ut(t11,j,t172,n)
        t752 = t515 - t751
        t779 = t208 * (t21 - t49 * ((t8 * ((t15 * t680 - t209) * t15 - t
     #212) * t15 - t218) * t15 + ((t692 - t21) * t15 - t229) * t15) / 0.
     #24E2 + t510 - t103 * ((t8 * ((t701 * t94 - t703) * t94 - t708) * t
     #94 - t8 * (t708 - (-t713 * t94 + t706) * t94) * t94) * t94 + (((t7
     #01 * t8 * t94 - t504) * t94 - t510) * t94 - (t510 - (-t713 * t8 * 
     #t94 + t508) * t94) * t94) * t94) / 0.24E2 + t520 - t158 * ((t8 * (
     #(t149 * t740 - t742) * t149 - t747) * t149 - t8 * (t747 - (-t149 *
     # t752 + t745) * t149) * t149) * t149 + (((t149 * t740 * t8 - t514)
     # * t149 - t520) * t149 - (t520 - (-t149 * t752 * t8 + t518) * t149
     #) * t149) * t149) / 0.24E2)
        t782 = u(t9,t90,k,n)
        t786 = u(t9,t96,k,n)
        t791 = (t8 * (t782 - t50) * t94 - t8 * (t50 - t786) * t94) * t94
        t792 = u(t9,j,t145,n)
        t796 = u(t9,j,t151,n)
        t801 = (t8 * (t792 - t50) * t149 - t8 * (t50 - t796) * t149) * t
     #149
        t805 = (cc * (t588 + t791 + t801) - t365) * t15
        t808 = t343 * (t805 / 0.2E1 + t369 / 0.2E1)
        t811 = t399 * t566
        t817 = t782 - t344
        t821 = (t15 * t8 * t817 - t414) * t15
        t822 = u(t11,t90,t145,n)
        t826 = u(t11,t90,t151,n)
        t831 = (t8 * (t822 - t344) * t149 - t8 * (t344 - t826) * t149) *
     # t149
        t835 = t786 - t348
        t839 = (t15 * t8 * t835 - t435) * t15
        t840 = u(t11,t96,t145,n)
        t844 = u(t11,t96,t151,n)
        t849 = (t8 * (t840 - t348) * t149 - t8 * (t348 - t844) * t149) *
     # t149
        t855 = t792 - t354
        t859 = (t15 * t8 * t855 - t458) * t15
        t867 = (t8 * (t822 - t354) * t94 - t8 * (t354 - t840) * t94) * t
     #94
        t871 = t796 - t358
        t875 = (t15 * t8 * t871 - t477) * t15
        t883 = (t8 * (t826 - t358) * t94 - t8 * (t358 - t844) * t94) * t
     #94
        t890 = t403 * ((t8 * (t588 + t791 + t801 - t76 - t353 - t363) * 
     #t15 - t406) * t15 + (t8 * (t821 + t621 + t831 - t76 - t353 - t363)
     # * t94 - t8 * (t76 + t353 + t363 - t839 - t627 - t849) * t94) * t9
     #4 + (t8 * (t859 + t867 + t660 - t76 - t353 - t363) * t149 - t8 * (
     #t76 + t353 + t363 - t875 - t883 - t666) * t149) * t149)
        t893 = ut(t9,t90,k,n)
        t897 = ut(t9,t96,k,n)
        t903 = ut(t9,j,t145,n)
        t907 = ut(t9,j,t151,n)
        t919 = t500 * ((cc * (t692 + (t8 * (t893 - t10) * t94 - t8 * (t1
     #0 - t897) * t94) * t94 + (t8 * (t903 - t10) * t149 - t8 * (t10 - t
     #907) * t149) * t149) - t522) * t15 / 0.2E1 + t526 / 0.2E1)
        t923 = t343 * (t805 - t369)
        t926 = t32 - t34 * t30 / 0.24E2 + t203 + t342 + t398 + t499 + t5
     #55 + t559 + t561 * t563 / 0.2E1 + t567 * t570 / 0.6E1 + t573 * t67
     #5 / 0.2E1 + t678 * t779 / 0.4E1 - t573 * t808 / 0.4E1 + t811 * t89
     #0 / 0.12E2 - t678 * t919 / 0.8E1 + t573 * t923 / 0.24E2
        t928 = t573 * t201 / 0.2E1
        t930 = t678 * t340 / 0.4E1
        t932 = t573 * t396 / 0.4E1
        t934 = t811 * t497 / 0.12E2
        t936 = t678 * t553 / 0.8E1
        t938 = t573 * t557 / 0.24E2
        t939 = t8 * t205
        t941 = t939 * t563 / 0.2E1
        t942 = t8 * t400
        t944 = t942 * t570 / 0.6E1
        t946 = t37 * t675 / 0.2E1
        t948 = t206 * t779 / 0.4E1
        t950 = t37 * t808 / 0.4E1
        t952 = t401 * t890 / 0.12E2
        t954 = t206 * t919 / 0.8E1
        t956 = t37 * t923 / 0.24E2
        t957 = t8 * t33
        t961 = dt * (t210 - dx * t216 / 0.24E2)
        t963 = t8 * t6
        t964 = t963 * t961
        t965 = t957 * t961 - t928 - t930 - t932 - t934 - t936 - t938 - t
     #941 - t944 - t946 - t948 + t950 - t952 + t954 - t956 - t964
        t967 = (t926 + t965) * t4
        t970 = cc * t12
        t971 = cc * t10
        t973 = (-t970 + t971) * t15
        t975 = cc * t2
        t977 = (-t975 + t970) * t15
        t978 = t977 / 0.2E1
        t985 = (t973 - t977) * t15
        t987 = (((cc * t679 - t971) * t15 - t973) * t15 - t985) * t15
        t988 = cc * t23
        t990 = (t975 - t988) * t15
        t992 = (t977 - t990) * t15
        t994 = (t985 - t992) * t15
        t1001 = dx * (t973 / 0.2E1 + t978 - t49 * (t987 / 0.2E1 + t994 /
     # 0.2E1) / 0.6E1) / 0.4E1
        t1002 = cc * t219
        t1004 = (-t1002 + t988) * t15
        t1006 = (t990 - t1004) * t15
        t1008 = (t992 - t1006) * t15
        t1014 = t49 * (t992 - dx * (t994 - t1008) / 0.12E2) / 0.24E2
        t1015 = -t32 - t1001 - t1014 - t203 - t342 - t398 - t499 - t555 
     #- t559 + t941 + t944 + t946
        t1021 = t49 * (t985 - dx * (t987 - t994) / 0.12E2) / 0.24E2
        t1023 = dx * t77 / 0.24E2
        t1024 = t990 / 0.2E1
        t1031 = dx * (t978 + t1024 - t49 * (t994 / 0.2E1 + t1008 / 0.2E1
     #) / 0.6E1) / 0.4E1
        t1035 = t8 * (t53 - dx * t59 / 0.24E2)
        t1036 = t975 / 0.2E1
        t1037 = t970 / 0.2E1
        t1039 = -t6 * t967 + t1021 - t1023 - t1031 + t1035 - t1036 + t10
     #37 + t948 - t950 + t952 - t954 + t956 + t964
        t1043 = i - 3
        t1044 = ut(t1043,j,k,n)
        t1045 = t219 - t1044
        t1057 = (-t1045 * t15 * t8 + t231) * t15
        t1065 = ut(t22,t104,k,n)
        t1066 = t1065 - t527
        t1068 = t528 * t94
        t1071 = t532 * t94
        t1073 = (t1068 - t1071) * t94
        t1077 = ut(t22,t117,k,n)
        t1078 = t531 - t1077
        t1104 = ut(t22,j,t159,n)
        t1105 = t1104 - t537
        t1107 = t538 * t149
        t1110 = t542 * t149
        t1112 = (t1107 - t1110) * t149
        t1116 = ut(t22,j,t172,n)
        t1117 = t541 - t1116
        t1144 = t208 * (t233 - t49 * ((t226 - t8 * (t223 - (-t1045 * t15
     # + t221) * t15) * t15) * t15 + (t235 - (t233 - t1057) * t15) * t15
     #) / 0.24E2 + t536 - t103 * ((t8 * ((t1066 * t94 - t1068) * t94 - t
     #1073) * t94 - t8 * (t1073 - (-t1078 * t94 + t1071) * t94) * t94) *
     # t94 + (((t1066 * t8 * t94 - t530) * t94 - t536) * t94 - (t536 - (
     #-t1078 * t8 * t94 + t534) * t94) * t94) * t94) / 0.24E2 + t546 - t
     #158 * ((t8 * ((t1105 * t149 - t1107) * t149 - t1112) * t149 - t8 *
     # (t1112 - (-t1117 * t149 + t1110) * t149) * t149) * t149 + (((t110
     #5 * t149 * t8 - t540) * t149 - t546) * t149 - (t546 - (-t1117 * t1
     #49 * t8 + t544) * t149) * t149) * t149) / 0.24E2)
        t1148 = t63 - u(t1043,j,k,n)
        t1152 = (-t1148 * t15 * t8 + t80) * t15
        t1153 = u(t62,t90,k,n)
        t1157 = u(t62,t96,k,n)
        t1162 = (t8 * (t1153 - t63) * t94 - t8 * (t63 - t1157) * t94) * 
     #t94
        t1163 = u(t62,j,t145,n)
        t1167 = u(t62,j,t151,n)
        t1172 = (t8 * (t1163 - t63) * t149 - t8 * (t63 - t1167) * t149) 
     #* t149
        t1176 = (t391 - cc * (t1152 + t1162 + t1172)) * t15
        t1179 = t343 * (t393 / 0.2E1 + t1176 / 0.2E1)
        t1187 = t370 - t1153
        t1191 = (-t1187 * t15 * t8 + t417) * t15
        t1192 = u(t22,t104,k,n)
        t1193 = t1192 - t370
        t1197 = (t1193 * t8 * t94 - t373) * t94
        t1198 = u(t22,t90,t145,n)
        t1202 = u(t22,t90,t151,n)
        t1207 = (t8 * (t1198 - t370) * t149 - t8 * (t370 - t1202) * t149
     #) * t149
        t1211 = t374 - t1157
        t1215 = (-t1211 * t15 * t8 + t438) * t15
        t1216 = u(t22,t117,k,n)
        t1217 = t374 - t1216
        t1221 = (-t1217 * t8 * t94 + t377) * t94
        t1222 = u(t22,t96,t145,n)
        t1226 = u(t22,t96,t151,n)
        t1231 = (t8 * (t1222 - t374) * t149 - t8 * (t374 - t1226) * t149
     #) * t149
        t1237 = t380 - t1163
        t1241 = (-t1237 * t15 * t8 + t461) * t15
        t1249 = (t8 * (t1198 - t380) * t94 - t8 * (t380 - t1222) * t94) 
     #* t94
        t1250 = u(t22,j,t159,n)
        t1251 = t1250 - t380
        t1255 = (t1251 * t149 * t8 - t383) * t149
        t1259 = t384 - t1167
        t1263 = (-t1259 * t15 * t8 + t480) * t15
        t1271 = (t8 * (t1202 - t384) * t94 - t8 * (t384 - t1226) * t94) 
     #* t94
        t1272 = u(t22,j,t172,n)
        t1273 = t384 - t1272
        t1277 = (-t1273 * t149 * t8 + t387) * t149
        t1284 = t403 * ((t409 - t8 * (t82 + t379 + t389 - t1152 - t1162 
     #- t1172) * t15) * t15 + (t8 * (t1191 + t1197 + t1207 - t82 - t379 
     #- t389) * t94 - t8 * (t82 + t379 + t389 - t1215 - t1221 - t1231) *
     # t94) * t94 + (t8 * (t1241 + t1249 + t1255 - t82 - t379 - t389) * 
     #t149 - t8 * (t82 + t379 + t389 - t1263 - t1271 - t1277) * t149) * 
     #t149)
        t1287 = ut(t62,t90,k,n)
        t1291 = ut(t62,t96,k,n)
        t1297 = ut(t62,j,t145,n)
        t1301 = ut(t62,j,t151,n)
        t1313 = t500 * (t550 / 0.2E1 + (t548 - cc * (t1057 + (t8 * (t128
     #7 - t219) * t94 - t8 * (t219 - t1291) * t94) * t94 + (t8 * (t1297 
     #- t219) * t149 - t8 * (t219 - t1301) * t149) * t149)) * t15 / 0.2E
     #1)
        t1317 = t343 * (t393 - t1176)
        t1321 = t207 * t407 * t15
        t1323 = t939 * t1321 / 0.2E1
        t1326 = t402 * (t28 + t250 + t299 - t233 - t536 - t546) * t15
        t1328 = t942 * t1326 / 0.6E1
        t1345 = t371 * t94
        t1348 = t375 * t94
        t1350 = (t1345 - t1348) * t94
        t1372 = t381 * t149
        t1375 = t385 * t149
        t1377 = (t1372 - t1375) * t149
        t1399 = t38 * (t82 - t49 * ((t70 - t8 * (t67 - (-t1148 * t15 + t
     #65) * t15) * t15) * t15 + (t84 - (t82 - t1152) * t15) * t15) / 0.2
     #4E2 + t379 - t103 * ((t8 * ((t1193 * t94 - t1345) * t94 - t1350) *
     # t94 - t8 * (t1350 - (-t1217 * t94 + t1348) * t94) * t94) * t94 + 
     #((t1197 - t379) * t94 - (t379 - t1221) * t94) * t94) / 0.24E2 + t3
     #89 - t158 * ((t8 * ((t1251 * t149 - t1372) * t149 - t1377) * t149 
     #- t8 * (t1377 - (-t1273 * t149 + t1375) * t149) * t149) * t149 + (
     #(t1255 - t389) * t149 - (t389 - t1277) * t149) * t149) / 0.24E2)
        t1401 = t37 * t1399 / 0.2E1
        t1403 = t206 * t1144 / 0.4E1
        t1405 = t37 * t1179 / 0.4E1
        t1407 = t401 * t1284 / 0.12E2
        t1409 = t206 * t1313 / 0.8E1
        t1411 = t37 * t1317 / 0.24E2
        t1418 = -t678 * t1144 / 0.4E1 - t573 * t1179 / 0.4E1 - t811 * t1
     #284 / 0.12E2 - t678 * t1313 / 0.8E1 - t573 * t1317 / 0.24E2 - t132
     #3 - t1328 + t1401 + t1403 + t1405 + t1407 + t1409 + t1411 + t561 *
     # t1321 / 0.2E1 + t567 * t1326 / 0.6E1 - t573 * t1399 / 0.2E1
        t1419 = dx * t234
        t1423 = t7 * t1419 / 0.24E2
        t1427 = dt * (t213 - dx * t224 / 0.24E2)
        t1429 = t963 * t1427
        t1430 = -t203 - t342 + t398 - t499 + t555 - t559 + t928 + t930 -
     # t932 + t934 - t936 + t938 - t34 * t1419 / 0.24E2 + t1423 + t957 *
     # t1427 - t1429
        t1432 = (t1418 + t1430) * t4
        t1435 = t988 / 0.2E1
        t1436 = t1323 + t1328 - t1401 - t1403 - t1405 - t1407 - t1409 - 
     #t1411 - t1435 + t1014 + t203 + t342
        t1444 = (t1006 - (t1004 - (-cc * t1044 + t1002) * t15) * t15) * 
     #t15
        t1450 = t49 * (t1006 - dx * (t1008 - t1444) / 0.12E2) / 0.24E2
        t1458 = dx * (t1024 + t1004 / 0.2E1 - t49 * (t1008 / 0.2E1 + t14
     #44 / 0.2E1) / 0.6E1) / 0.4E1
        t1460 = dx * t83 / 0.24E2
        t1464 = t8 * (t56 - dx * t68 / 0.24E2)
        t1465 = -t1432 * t6 - t1031 + t1036 - t1423 + t1429 - t1450 - t1
     #458 - t1460 + t1464 - t398 + t499 - t555 + t559
        t1471 = t207 * dy
        t1472 = t501 - t241
        t1474 = t8 * t1472 * t15
        t1475 = t241 - t527
        t1477 = t8 * t1475 * t15
        t1479 = (t1474 - t1477) * t15
        t1480 = ut(i,t90,t145,n)
        t1481 = t1480 - t241
        t1483 = t8 * t1481 * t149
        t1484 = ut(i,t90,t151,n)
        t1485 = t241 - t1484
        t1487 = t8 * t1485 * t149
        t1489 = (t1483 - t1487) * t149
        t1491 = cc * (t1479 + t276 + t1489)
        t1493 = (t1491 - t524) * t94
        t1494 = t505 - t245
        t1496 = t8 * t1494 * t15
        t1497 = t245 - t531
        t1499 = t8 * t1497 * t15
        t1501 = (t1496 - t1499) * t15
        t1502 = ut(i,t96,t145,n)
        t1503 = t1502 - t245
        t1505 = t8 * t1503 * t149
        t1506 = ut(i,t96,t151,n)
        t1507 = t245 - t1506
        t1509 = t8 * t1507 * t149
        t1511 = (t1505 - t1509) * t149
        t1513 = cc * (t1501 + t282 + t1511)
        t1515 = (t524 - t1513) * t94
        t1518 = t1471 * (t1493 / 0.2E1 + t1515 / 0.2E1)
        t1520 = t206 * t1518 / 0.8E1
        t1521 = dt * dy
        t1523 = cc * (t419 + t131 + t429)
        t1525 = (t1523 - t367) * t94
        t1527 = cc * (t440 + t137 + t450)
        t1529 = (t367 - t1527) * t94
        t1531 = t1521 * (t1525 - t1529)
        t1533 = t37 * t1531 / 0.24E2
        t1542 = j + 3
        t1543 = ut(i,t1542,k,n)
        t1544 = t1543 - t251
        t1548 = (t1544 * t8 * t94 - t274) * t94
        t1549 = ut(i,t104,t145,n)
        t1553 = ut(i,t104,t151,n)
        t1565 = t1471 * ((cc * ((t8 * (t700 - t251) * t15 - t8 * (t251 -
     # t1065) * t15) * t15 + t1548 + (t8 * (t1549 - t251) * t149 - t8 * 
     #(t251 - t1553) * t149) * t149) - t1491) * t94 / 0.2E1 + t1493 / 0.
     #2E1)
        t1575 = (t8 * (t596 - t105) * t15 - t8 * (t105 - t1192) * t15) *
     # t15
        t1577 = u(i,t1542,k,n) - t105
        t1581 = (t1577 * t8 * t94 - t129) * t94
        t1582 = u(i,t104,t145,n)
        t1586 = u(i,t104,t151,n)
        t1591 = (t8 * (t1582 - t105) * t149 - t8 * (t105 - t1586) * t149
     #) * t149
        t1595 = (cc * (t1575 + t1581 + t1591) - t1523) * t94
        t1597 = t1521 * (t1595 - t1525)
        t1602 = t1521 * (t1525 / 0.2E1 + t1529 / 0.2E1)
        t1604 = t573 * t1602 / 0.4E1
        t1606 = t678 * t1518 / 0.8E1
        t1608 = t573 * t1531 / 0.24E2
        t1610 = t207 * t430 * t94
        t1612 = t939 * t1610 / 0.2E1
        t1615 = t402 * (t1479 + t276 + t1489 - t28 - t250 - t299) * t94
        t1617 = t942 * t1615 / 0.6E1
        t1619 = t412 * t15
        t1622 = t415 * t15
        t1624 = (t1619 - t1622) * t15
        t1660 = u(i,t90,t159,n)
        t1661 = t1660 - t420
        t1663 = t421 * t149
        t1666 = t425 * t149
        t1668 = (t1663 - t1666) * t149
        t1672 = u(i,t90,t172,n)
        t1673 = t424 - t1672
        t1685 = (t149 * t1661 * t8 - t423) * t149
        t1691 = (-t149 * t1673 * t8 + t427) * t149
        t1700 = t38 * (t419 - t49 * ((t8 * ((t15 * t817 - t1619) * t15 -
     # t1624) * t15 - t8 * (t1624 - (-t1187 * t15 + t1622) * t15) * t15)
     # * t15 + ((t821 - t419) * t15 - (t419 - t1191) * t15) * t15) / 0.2
     #4E2 + t131 - t103 * ((t8 * ((t1577 * t94 - t107) * t94 - t110) * t
     #94 - t116) * t94 + ((t1581 - t131) * t94 - t133) * t94) / 0.24E2 +
     # t429 - t158 * ((t8 * ((t149 * t1661 - t1663) * t149 - t1668) * t1
     #49 - t8 * (t1668 - (-t149 * t1673 + t1666) * t149) * t149) * t149 
     #+ ((t1685 - t429) * t149 - (t429 - t1691) * t149) * t149) / 0.24E2
     #)
        t1702 = t37 * t1700 / 0.2E1
        t1703 = t893 - t501
        t1705 = t1472 * t15
        t1708 = t1475 * t15
        t1710 = (t1705 - t1708) * t15
        t1714 = t527 - t1287
        t1755 = ut(i,t90,t159,n)
        t1756 = t1755 - t1480
        t1758 = t1481 * t149
        t1761 = t1485 * t149
        t1763 = (t1758 - t1761) * t149
        t1767 = ut(i,t90,t172,n)
        t1768 = t1484 - t1767
        t1795 = t208 * (t1479 - t49 * ((t8 * ((t15 * t1703 - t1705) * t1
     #5 - t1710) * t15 - t8 * (t1710 - (-t15 * t1714 + t1708) * t15) * t
     #15) * t15 + (((t15 * t1703 * t8 - t1474) * t15 - t1479) * t15 - (t
     #1479 - (-t15 * t1714 * t8 + t1477) * t15) * t15) * t15) / 0.24E2 +
     # t276 - t103 * ((t8 * ((t1544 * t94 - t253) * t94 - t256) * t94 - 
     #t262) * t94 + ((t1548 - t276) * t94 - t278) * t94) / 0.24E2 + t148
     #9 - t158 * ((t8 * ((t149 * t1756 - t1758) * t149 - t1763) * t149 -
     # t8 * (t1763 - (-t149 * t1768 + t1761) * t149) * t149) * t149 + ((
     #(t149 * t1756 * t8 - t1483) * t149 - t1489) * t149 - (t1489 - (-t1
     #49 * t1768 * t8 + t1487) * t149) * t149) * t149) / 0.24E2)
        t1797 = t206 * t1795 / 0.4E1
        t1800 = t1521 * (t1595 / 0.2E1 + t1525 / 0.2E1)
        t1802 = t37 * t1800 / 0.4E1
        t1823 = (t8 * (t822 - t420) * t15 - t8 * (t420 - t1198) * t15) *
     # t15
        t1824 = t1582 - t420
        t1828 = (t1824 * t8 * t94 - t466) * t94
        t1839 = (t8 * (t826 - t424) * t15 - t8 * (t424 - t1202) * t15) *
     # t15
        t1840 = t1586 - t424
        t1844 = (t1840 * t8 * t94 - t485) * t94
        t1851 = t403 * ((t8 * (t821 + t621 + t831 - t419 - t131 - t429) 
     #* t15 - t8 * (t419 + t131 + t429 - t1191 - t1197 - t1207) * t15) *
     # t15 + (t8 * (t1575 + t1581 + t1591 - t419 - t131 - t429) * t94 - 
     #t432) * t94 + (t8 * (t1823 + t1828 + t1685 - t419 - t131 - t429) *
     # t149 - t8 * (t419 + t131 + t429 - t1839 - t1844 - t1691) * t149) 
     #* t149)
        t1853 = t401 * t1851 / 0.12E2
        t1855 = t206 * t1565 / 0.8E1
        t1857 = t37 * t1597 / 0.24E2
        t1859 = t37 * t1602 / 0.4E1
        t1860 = t1520 + t1533 - t678 * t1565 / 0.8E1 + t573 * t1597 / 0.
     #24E2 - t1604 - t1606 - t1608 - t1612 - t1617 - t1702 - t1797 + t18
     #02 - t1853 + t1855 - t1857 + t1859
        t1876 = dt * (t254 - dy * t260 / 0.24E2)
        t1878 = t963 * t1876
        t1879 = dy * t277
        t1883 = t7 * t1879 / 0.24E2
        t1884 = t561 * t1610 / 0.2E1 + t567 * t1615 / 0.6E1 + t573 * t17
     #00 / 0.2E1 + t678 * t1795 / 0.4E1 - t573 * t1800 / 0.4E1 + t811 * 
     #t1851 / 0.12E2 + t203 + t342 + t499 - t928 - t930 - t934 + t957 * 
     #t1876 - t1878 - t34 * t1879 / 0.24E2 + t1883
        t1886 = (t1860 + t1884) * t4
        t1889 = cc * t241
        t1891 = (-t975 + t1889) * t94
        t1892 = t1891 / 0.2E1
        t1893 = cc * t245
        t1895 = (t975 - t1893) * t94
        t1896 = t1895 / 0.2E1
        t1897 = cc * t251
        t1899 = (-t1889 + t1897) * t94
        t1901 = (t1899 - t1891) * t94
        t1903 = (t1891 - t1895) * t94
        t1905 = (t1901 - t1903) * t94
        t1906 = cc * t263
        t1908 = (-t1906 + t1893) * t94
        t1910 = (t1895 - t1908) * t94
        t1912 = (t1903 - t1910) * t94
        t1919 = dy * (t1892 + t1896 - t103 * (t1905 / 0.2E1 + t1912 / 0.
     #2E1) / 0.6E1) / 0.4E1
        t1920 = -t1520 - t1533 + t1612 + t1617 + t1702 + t1797 - t1802 +
     # t1853 - t1855 + t1857 - t1859 - t1919
        t1928 = (((cc * t1543 - t1897) * t94 - t1899) * t94 - t1901) * t
     #94
        t1935 = dy * (t1899 / 0.2E1 + t1892 - t103 * (t1928 / 0.2E1 + t1
     #905 / 0.2E1) / 0.6E1) / 0.4E1
        t1937 = dy * t132 / 0.24E2
        t1943 = t103 * (t1903 - dy * (t1905 - t1912) / 0.12E2) / 0.24E2
        t1944 = t1889 / 0.2E1
        t1950 = t103 * (t1901 - dy * (t1928 - t1905) / 0.12E2) / 0.24E2
        t1954 = t8 * (t108 - dy * t114 / 0.24E2)
        t1956 = -t1886 * t6 - t1036 + t1878 - t1883 - t1935 - t1937 - t1
     #943 + t1944 + t1950 + t1954 - t203 - t342 - t499
        t1968 = j - 3
        t1969 = ut(i,t1968,k,n)
        t1970 = t263 - t1969
        t1974 = (-t1970 * t8 * t94 + t280) * t94
        t1975 = ut(i,t117,t145,n)
        t1979 = ut(i,t117,t151,n)
        t1991 = t1471 * (t1515 / 0.2E1 + (t1513 - cc * ((t8 * (t712 - t2
     #63) * t15 - t8 * (t263 - t1077) * t15) * t15 + t1974 + (t8 * (t197
     #5 - t263) * t149 - t8 * (t263 - t1979) * t149) * t149)) * t94 / 0.
     #2E1)
        t1993 = t206 * t1991 / 0.8E1
        t2001 = (t8 * (t608 - t118) * t15 - t8 * (t118 - t1216) * t15) *
     # t15
        t2003 = t118 - u(i,t1968,k,n)
        t2007 = (-t2003 * t8 * t94 + t135) * t94
        t2008 = u(i,t117,t145,n)
        t2012 = u(i,t117,t151,n)
        t2017 = (t8 * (t2008 - t118) * t149 - t8 * (t118 - t2012) * t149
     #) * t149
        t2021 = (t1527 - cc * (t2001 + t2007 + t2017)) * t94
        t2023 = t1521 * (t1529 - t2021)
        t2025 = t37 * t2023 / 0.24E2
        t2027 = t433 * t15
        t2030 = t436 * t15
        t2032 = (t2027 - t2030) * t15
        t2068 = u(i,t96,t159,n)
        t2069 = t2068 - t441
        t2071 = t442 * t149
        t2074 = t446 * t149
        t2076 = (t2071 - t2074) * t149
        t2080 = u(i,t96,t172,n)
        t2081 = t445 - t2080
        t2093 = (t149 * t2069 * t8 - t444) * t149
        t2099 = (-t149 * t2081 * t8 + t448) * t149
        t2108 = t38 * (t440 - t49 * ((t8 * ((t15 * t835 - t2027) * t15 -
     # t2032) * t15 - t8 * (t2032 - (-t1211 * t15 + t2030) * t15) * t15)
     # * t15 + ((t839 - t440) * t15 - (t440 - t1215) * t15) * t15) / 0.2
     #4E2 + t137 - t103 * ((t125 - t8 * (t122 - (-t2003 * t94 + t120) * 
     #t94) * t94) * t94 + (t139 - (t137 - t2007) * t94) * t94) / 0.24E2 
     #+ t450 - t158 * ((t8 * ((t149 * t2069 - t2071) * t149 - t2076) * t
     #149 - t8 * (t2076 - (-t149 * t2081 + t2074) * t149) * t149) * t149
     # + ((t2093 - t450) * t149 - (t450 - t2099) * t149) * t149) / 0.24E
     #2)
        t2110 = t37 * t2108 / 0.2E1
        t2111 = t897 - t505
        t2113 = t1494 * t15
        t2116 = t1497 * t15
        t2118 = (t2113 - t2116) * t15
        t2122 = t531 - t1291
        t2163 = ut(i,t96,t159,n)
        t2164 = t2163 - t1502
        t2166 = t1503 * t149
        t2169 = t1507 * t149
        t2171 = (t2166 - t2169) * t149
        t2175 = ut(i,t96,t172,n)
        t2176 = t1506 - t2175
        t2203 = t208 * (t1501 - t49 * ((t8 * ((t15 * t2111 - t2113) * t1
     #5 - t2118) * t15 - t8 * (t2118 - (-t15 * t2122 + t2116) * t15) * t
     #15) * t15 + (((t15 * t2111 * t8 - t1496) * t15 - t1501) * t15 - (t
     #1501 - (-t15 * t2122 * t8 + t1499) * t15) * t15) * t15) / 0.24E2 +
     # t282 - t103 * ((t270 - t8 * (t267 - (-t1970 * t94 + t265) * t94) 
     #* t94) * t94 + (t284 - (t282 - t1974) * t94) * t94) / 0.24E2 + t15
     #11 - t158 * ((t8 * ((t149 * t2164 - t2166) * t149 - t2171) * t149 
     #- t8 * (t2171 - (-t149 * t2176 + t2169) * t149) * t149) * t149 + (
     #((t149 * t2164 * t8 - t1505) * t149 - t1511) * t149 - (t1511 - (-t
     #149 * t2176 * t8 + t1509) * t149) * t149) * t149) / 0.24E2)
        t2205 = t206 * t2203 / 0.4E1
        t2208 = t1521 * (t1529 / 0.2E1 + t2021 / 0.2E1)
        t2210 = t37 * t2208 / 0.4E1
        t2231 = (t8 * (t840 - t441) * t15 - t8 * (t441 - t1222) * t15) *
     # t15
        t2232 = t441 - t2008
        t2236 = (-t2232 * t8 * t94 + t469) * t94
        t2247 = (t8 * (t844 - t445) * t15 - t8 * (t445 - t1226) * t15) *
     # t15
        t2248 = t445 - t2012
        t2252 = (-t2248 * t8 * t94 + t488) * t94
        t2259 = t403 * ((t8 * (t839 + t627 + t849 - t440 - t137 - t450) 
     #* t15 - t8 * (t440 + t137 + t450 - t1215 - t1221 - t1231) * t15) *
     # t15 + (t453 - t8 * (t440 + t137 + t450 - t2001 - t2007 - t2017) *
     # t94) * t94 + (t8 * (t2231 + t2236 + t2093 - t440 - t137 - t450) *
     # t149 - t8 * (t440 + t137 + t450 - t2247 - t2252 - t2099) * t149) 
     #* t149)
        t2261 = t401 * t2259 / 0.12E2
        t2264 = t402 * (t28 + t250 + t299 - t1501 - t282 - t1511) * t94
        t2266 = t942 * t2264 / 0.6E1
        t2272 = t207 * t451 * t94
        t2274 = t939 * t2272 / 0.2E1
        t2275 = t1520 - t1533 - t1604 - t1606 + t1608 + t1859 + t1993 + 
     #t2025 + t2110 + t2205 + t2210 + t2261 - t2266 - t678 * t1991 / 0.8
     #E1 - t573 * t2023 / 0.24E2 - t2274
        t2291 = dt * (t257 - dy * t268 / 0.24E2)
        t2293 = t963 * t2291
        t2294 = dy * t283
        t2298 = t7 * t2294 / 0.24E2
        t2299 = -t573 * t2108 / 0.2E1 - t678 * t2203 / 0.4E1 - t573 * t2
     #208 / 0.4E1 - t811 * t2259 / 0.12E2 + t561 * t2272 / 0.2E1 + t567 
     #* t2264 / 0.6E1 - t203 - t342 - t499 + t928 + t930 + t934 + t957 *
     # t2291 - t2293 - t34 * t2294 / 0.24E2 + t2298
        t2301 = (t2275 + t2299) * t4
        t2304 = t1893 / 0.2E1
        t2305 = -t1520 + t1533 - t1859 - t1993 - t2025 - t2110 - t2205 -
     # t2210 - t2261 + t2266 + t2274 - t2304
        t2312 = (t1910 - (t1908 - (-cc * t1969 + t1906) * t94) * t94) * 
     #t94
        t2318 = t103 * (t1910 - dy * (t1912 - t2312) / 0.12E2) / 0.24E2
        t2320 = dy * t138 / 0.24E2
        t2324 = t8 * (t111 - dy * t123 / 0.24E2)
        t2332 = dy * (t1896 + t1908 / 0.2E1 - t103 * (t1912 / 0.2E1 + t2
     #312 / 0.2E1) / 0.6E1) / 0.4E1
        t2334 = -t2301 * t6 + t1036 - t1919 + t1943 + t203 + t2293 - t22
     #98 - t2318 - t2320 + t2324 - t2332 + t342 + t499
        t2340 = t207 * dz
        t2341 = t511 - t290
        t2343 = t8 * t2341 * t15
        t2344 = t290 - t537
        t2346 = t8 * t2344 * t15
        t2348 = (t2343 - t2346) * t15
        t2349 = t1480 - t290
        t2351 = t8 * t2349 * t94
        t2352 = t290 - t1502
        t2354 = t8 * t2352 * t94
        t2356 = (t2351 - t2354) * t94
        t2358 = cc * (t2348 + t2356 + t325)
        t2360 = (t2358 - t524) * t149
        t2361 = t515 - t294
        t2363 = t8 * t2361 * t15
        t2364 = t294 - t541
        t2366 = t8 * t2364 * t15
        t2368 = (t2363 - t2366) * t15
        t2369 = t1484 - t294
        t2371 = t8 * t2369 * t94
        t2372 = t294 - t1506
        t2374 = t8 * t2372 * t94
        t2376 = (t2371 - t2374) * t94
        t2378 = cc * (t2368 + t2376 + t331)
        t2380 = (t524 - t2378) * t149
        t2383 = t2340 * (t2360 / 0.2E1 + t2380 / 0.2E1)
        t2385 = t206 * t2383 / 0.8E1
        t2386 = dt * dz
        t2388 = cc * (t463 + t471 + t186)
        t2390 = (t2388 - t367) * t149
        t2392 = cc * (t482 + t490 + t192)
        t2394 = (t367 - t2392) * t149
        t2396 = t2386 * (t2390 - t2394)
        t2398 = t37 * t2396 / 0.24E2
        t2400 = t207 * t472 * t149
        t2405 = t402 * (t2348 + t2356 + t325 - t28 - t250 - t299) * t149
        t2409 = t456 * t15
        t2412 = t459 * t15
        t2414 = (t2409 - t2412) * t15
        t2436 = t464 * t94
        t2439 = t467 * t94
        t2441 = (t2436 - t2439) * t94
        t2462 = k + 3
        t2464 = u(i,j,t2462,n) - t160
        t2476 = (t149 * t2464 * t8 - t184) * t149
        t2485 = t38 * (t463 - t49 * ((t8 * ((t15 * t855 - t2409) * t15 -
     # t2414) * t15 - t8 * (t2414 - (-t1237 * t15 + t2412) * t15) * t15)
     # * t15 + ((t859 - t463) * t15 - (t463 - t1241) * t15) * t15) / 0.2
     #4E2 + t471 - t103 * ((t8 * ((t1824 * t94 - t2436) * t94 - t2441) *
     # t94 - t8 * (t2441 - (-t2232 * t94 + t2439) * t94) * t94) * t94 + 
     #((t1828 - t471) * t94 - (t471 - t2236) * t94) * t94) / 0.24E2 + t1
     #86 - t158 * ((t8 * ((t149 * t2464 - t162) * t149 - t165) * t149 - 
     #t171) * t149 + ((t2476 - t186) * t149 - t188) * t149) / 0.24E2)
        t2488 = t903 - t511
        t2490 = t2341 * t15
        t2493 = t2344 * t15
        t2495 = (t2490 - t2493) * t15
        t2499 = t537 - t1297
        t2525 = t1549 - t1480
        t2527 = t2349 * t94
        t2530 = t2352 * t94
        t2532 = (t2527 - t2530) * t94
        t2536 = t1502 - t1975
        t2562 = ut(i,j,t2462,n)
        t2563 = t2562 - t300
        t2575 = (t149 * t2563 * t8 - t323) * t149
        t2584 = t208 * (t2348 - t49 * ((t8 * ((t15 * t2488 - t2490) * t1
     #5 - t2495) * t15 - t8 * (t2495 - (-t15 * t2499 + t2493) * t15) * t
     #15) * t15 + (((t15 * t2488 * t8 - t2343) * t15 - t2348) * t15 - (t
     #2348 - (-t15 * t2499 * t8 + t2346) * t15) * t15) * t15) / 0.24E2 +
     # t2356 - t103 * ((t8 * ((t2525 * t94 - t2527) * t94 - t2532) * t94
     # - t8 * (t2532 - (-t2536 * t94 + t2530) * t94) * t94) * t94 + (((t
     #2525 * t8 * t94 - t2351) * t94 - t2356) * t94 - (t2356 - (-t2536 *
     # t8 * t94 + t2354) * t94) * t94) * t94) / 0.24E2 + t325 - t158 * (
     #(t8 * ((t149 * t2563 - t302) * t149 - t305) * t149 - t311) * t149 
     #+ ((t2575 - t325) * t149 - t327) * t149) / 0.24E2)
        t2594 = (t8 * (t635 - t160) * t15 - t8 * (t160 - t1250) * t15) *
     # t15
        t2602 = (t8 * (t1660 - t160) * t94 - t8 * (t160 - t2068) * t94) 
     #* t94
        t2606 = (cc * (t2594 + t2602 + t2476) - t2388) * t149
        t2609 = t2386 * (t2606 / 0.2E1 + t2390 / 0.2E1)
        t2634 = t403 * ((t8 * (t859 + t867 + t660 - t463 - t471 - t186) 
     #* t15 - t8 * (t463 + t471 + t186 - t1241 - t1249 - t1255) * t15) *
     # t15 + (t8 * (t1823 + t1828 + t1685 - t463 - t471 - t186) * t94 - 
     #t8 * (t463 + t471 + t186 - t2231 - t2236 - t2093) * t94) * t94 + (
     #t8 * (t2594 + t2602 + t2476 - t463 - t471 - t186) * t149 - t474) *
     # t149)
        t2659 = t2340 * ((cc * ((t8 * (t739 - t300) * t15 - t8 * (t300 -
     # t1104) * t15) * t15 + (t8 * (t1755 - t300) * t94 - t8 * (t300 - t
     #2163) * t94) * t94 + t2575) - t2358) * t149 / 0.2E1 + t2360 / 0.2E
     #1)
        t2663 = t2386 * (t2606 - t2390)
        t2668 = t2386 * (t2390 / 0.2E1 + t2394 / 0.2E1)
        t2670 = t573 * t2668 / 0.4E1
        t2672 = t678 * t2383 / 0.8E1
        t2674 = t573 * t2396 / 0.24E2
        t2676 = t939 * t2400 / 0.2E1
        t2678 = t942 * t2405 / 0.6E1
        t2680 = t37 * t2485 / 0.2E1
        t2681 = t2385 + t2398 + t561 * t2400 / 0.2E1 + t567 * t2405 / 0.
     #6E1 + t573 * t2485 / 0.2E1 + t678 * t2584 / 0.4E1 - t573 * t2609 /
     # 0.4E1 + t811 * t2634 / 0.12E2 - t678 * t2659 / 0.8E1 + t573 * t26
     #63 / 0.24E2 - t2670 - t2672 - t2674 - t2676 - t2678 - t2680
        t2683 = t206 * t2584 / 0.4E1
        t2685 = t37 * t2609 / 0.4E1
        t2687 = t401 * t2634 / 0.12E2
        t2689 = t206 * t2659 / 0.8E1
        t2691 = t37 * t2663 / 0.24E2
        t2693 = t37 * t2668 / 0.4E1
        t2697 = dt * (t303 - dz * t309 / 0.24E2)
        t2699 = t963 * t2697
        t2700 = dz * t326
        t2702 = t7 * t2700 / 0.24E2
        t2705 = -t2683 + t2685 - t2687 + t2689 - t2691 + t2693 + t957 * 
     #t2697 - t2699 + t2702 - t34 * t2700 / 0.24E2 + t203 + t342 + t499 
     #- t928 - t930 - t934
        t2707 = (t2681 + t2705) * t4
        t2710 = -t2385 - t2398 + t2676 + t2678 + t2680 + t2683 - t2685 +
     # t2687 - t2689 + t2691 - t2693 + t2699
        t2712 = cc * t290
        t2714 = (-t975 + t2712) * t149
        t2715 = t2714 / 0.2E1
        t2716 = cc * t294
        t2718 = (t975 - t2716) * t149
        t2719 = t2718 / 0.2E1
        t2720 = cc * t300
        t2722 = (-t2712 + t2720) * t149
        t2724 = (t2722 - t2714) * t149
        t2726 = (t2714 - t2718) * t149
        t2728 = (t2724 - t2726) * t149
        t2729 = cc * t312
        t2731 = (-t2729 + t2716) * t149
        t2733 = (t2718 - t2731) * t149
        t2735 = (t2726 - t2733) * t149
        t2742 = dz * (t2715 + t2719 - t158 * (t2728 / 0.2E1 + t2735 / 0.
     #2E1) / 0.6E1) / 0.4E1
        t2749 = (((cc * t2562 - t2720) * t149 - t2722) * t149 - t2724) *
     # t149
        t2755 = t158 * (t2724 - dz * (t2749 - t2728) / 0.12E2) / 0.24E2
        t2759 = t8 * (t163 - dz * t169 / 0.24E2)
        t2760 = t2712 / 0.2E1
        t2766 = t158 * (t2726 - dz * (t2728 - t2735) / 0.12E2) / 0.24E2
        t2774 = dz * (t2722 / 0.2E1 + t2715 - t158 * (t2749 / 0.2E1 + t2
     #728 / 0.2E1) / 0.6E1) / 0.4E1
        t2776 = dz * t187 / 0.24E2
        t2777 = -t2707 * t6 - t1036 - t203 - t2702 - t2742 + t2755 + t27
     #59 + t2760 - t2766 - t2774 - t2776 - t342 - t499
        t2782 = t475 * t15
        t2785 = t478 * t15
        t2787 = (t2782 - t2785) * t15
        t2809 = t483 * t94
        t2812 = t486 * t94
        t2814 = (t2809 - t2812) * t94
        t2835 = k - 3
        t2837 = t173 - u(i,j,t2835,n)
        t2849 = (-t149 * t2837 * t8 + t190) * t149
        t2858 = t38 * (t482 - t49 * ((t8 * ((t15 * t871 - t2782) * t15 -
     # t2787) * t15 - t8 * (t2787 - (-t1259 * t15 + t2785) * t15) * t15)
     # * t15 + ((t875 - t482) * t15 - (t482 - t1263) * t15) * t15) / 0.2
     #4E2 + t490 - t103 * ((t8 * ((t1840 * t94 - t2809) * t94 - t2814) *
     # t94 - t8 * (t2814 - (-t2248 * t94 + t2812) * t94) * t94) * t94 + 
     #((t1844 - t490) * t94 - (t490 - t2252) * t94) * t94) / 0.24E2 + t1
     #92 - t158 * ((t180 - t8 * (t177 - (-t149 * t2837 + t175) * t149) *
     # t149) * t149 + (t194 - (t192 - t2849) * t149) * t149) / 0.24E2)
        t2861 = t907 - t515
        t2863 = t2361 * t15
        t2866 = t2364 * t15
        t2868 = (t2863 - t2866) * t15
        t2872 = t541 - t1301
        t2898 = t1553 - t1484
        t2900 = t2369 * t94
        t2903 = t2372 * t94
        t2905 = (t2900 - t2903) * t94
        t2909 = t1506 - t1979
        t2935 = ut(i,j,t2835,n)
        t2936 = t312 - t2935
        t2948 = (-t149 * t2936 * t8 + t329) * t149
        t2957 = t208 * (t2368 - t49 * ((t8 * ((t15 * t2861 - t2863) * t1
     #5 - t2868) * t15 - t8 * (t2868 - (-t15 * t2872 + t2866) * t15) * t
     #15) * t15 + (((t15 * t2861 * t8 - t2363) * t15 - t2368) * t15 - (t
     #2368 - (-t15 * t2872 * t8 + t2366) * t15) * t15) * t15) / 0.24E2 +
     # t2376 - t103 * ((t8 * ((t2898 * t94 - t2900) * t94 - t2905) * t94
     # - t8 * (t2905 - (-t2909 * t94 + t2903) * t94) * t94) * t94 + (((t
     #2898 * t8 * t94 - t2371) * t94 - t2376) * t94 - (t2376 - (-t2909 *
     # t8 * t94 + t2374) * t94) * t94) * t94) / 0.24E2 + t331 - t158 * (
     #(t319 - t8 * (t316 - (-t149 * t2936 + t314) * t149) * t149) * t149
     # + (t333 - (t331 - t2948) * t149) * t149) / 0.24E2)
        t2967 = (t8 * (t647 - t173) * t15 - t8 * (t173 - t1272) * t15) *
     # t15
        t2975 = (t8 * (t1672 - t173) * t94 - t8 * (t173 - t2080) * t94) 
     #* t94
        t2979 = (t2392 - cc * (t2967 + t2975 + t2849)) * t149
        t2982 = t2386 * (t2394 / 0.2E1 + t2979 / 0.2E1)
        t3007 = t403 * ((t8 * (t875 + t883 + t666 - t482 - t490 - t192) 
     #* t15 - t8 * (t482 + t490 + t192 - t1263 - t1271 - t1277) * t15) *
     # t15 + (t8 * (t1839 + t1844 + t1691 - t482 - t490 - t192) * t94 - 
     #t8 * (t482 + t490 + t192 - t2247 - t2252 - t2099) * t94) * t94 + (
     #t493 - t8 * (t482 + t490 + t192 - t2967 - t2975 - t2849) * t149) *
     # t149)
        t3032 = t2340 * (t2380 / 0.2E1 + (t2378 - cc * ((t8 * (t751 - t3
     #12) * t15 - t8 * (t312 - t1116) * t15) * t15 + (t8 * (t1767 - t312
     #) * t94 - t8 * (t312 - t2175) * t94) * t94 + t2948)) * t149 / 0.2E
     #1)
        t3036 = t2386 * (t2394 - t2979)
        t3040 = t207 * t491 * t149
        t3042 = t939 * t3040 / 0.2E1
        t3045 = t402 * (t28 + t250 + t299 - t2368 - t2376 - t331) * t149
        t3047 = t942 * t3045 / 0.6E1
        t3049 = t37 * t2858 / 0.2E1
        t3051 = t206 * t2957 / 0.4E1
        t3052 = t2385 - t2398 - t2670 - t2672 + t2674 + t2693 - t573 * t
     #2858 / 0.2E1 - t678 * t2957 / 0.4E1 - t573 * t2982 / 0.4E1 - t811 
     #* t3007 / 0.12E2 - t678 * t3032 / 0.8E1 - t573 * t3036 / 0.24E2 - 
     #t3042 - t3047 + t3049 + t3051
        t3054 = t37 * t2982 / 0.4E1
        t3056 = t401 * t3007 / 0.12E2
        t3058 = t206 * t3032 / 0.8E1
        t3060 = t37 * t3036 / 0.24E2
        t3068 = dt * (t306 - dz * t317 / 0.24E2)
        t3070 = t963 * t3068
        t3071 = dz * t332
        t3075 = t7 * t3071 / 0.24E2
        t3076 = t3054 + t3056 + t3058 + t3060 + t561 * t3040 / 0.2E1 + t
     #567 * t3045 / 0.6E1 - t203 - t342 - t499 + t928 + t930 + t934 + t9
     #57 * t3068 - t3070 - t34 * t3071 / 0.24E2 + t3075
        t3078 = (t3052 + t3076) * t4
        t3082 = dz * t193 / 0.24E2
        t3083 = -t2385 + t2398 - t2693 + t3042 + t3047 - t3049 - t3051 -
     # t3054 - t3056 - t3058 - t3060 - t3082
        t3090 = (t2733 - (t2731 - (-cc * t2935 + t2729) * t149) * t149) 
     #* t149
        t3096 = t158 * (t2733 - dz * (t2735 - t3090) / 0.12E2) / 0.24E2
        t3100 = t8 * (t166 - dz * t178 / 0.24E2)
        t3108 = dz * (t2719 + t2731 / 0.2E1 - t158 * (t2735 / 0.2E1 + t3
     #090 / 0.2E1) / 0.6E1) / 0.4E1
        t3110 = t2716 / 0.2E1
        t3111 = -t3078 * t6 + t1036 + t203 - t2742 + t2766 + t3070 - t30
     #75 - t3096 + t3100 - t3108 - t3110 + t342 + t499
        t3120 = t1035 + t964 + t941 - t1023 + t944 - t32 + t1037 + t946 
     #- t1001 + t948 - t950 + t1021
        t3121 = t952 - t954 + t956 - t1036 - t203 - t1031 - t342 - t398 
     #- t1014 - t499 - t555 - t559
        t3127 = t1464 + t1429 + t1323 - t1460 + t1328 - t1423 + t1036 + 
     #t203 - t1031 + t342 - t398 + t1014
        t3128 = t499 - t555 + t559 - t1435 - t1401 - t1458 - t1403 - t14
     #05 - t1450 - t1407 - t1409 - t1411
        t3136 = t1954 + t1878 + t1612 - t1937 + t1617 - t1883 + t1944 + 
     #t1702 - t1935 + t1797 - t1802 + t1950
        t3137 = t1853 - t1855 + t1857 - t1036 - t203 - t1919 - t342 - t1
     #859 - t1943 - t499 - t1520 - t1533
        t3143 = t2324 + t2293 + t2274 - t2320 + t2266 - t2298 + t1036 + 
     #t203 - t1919 + t342 - t1859 + t1943
        t3144 = t499 - t1520 + t1533 - t2304 - t2110 - t2332 - t2205 - t
     #2210 - t2318 - t2261 - t1993 - t2025
        t3152 = t2759 + t2699 + t2676 - t2776 + t2678 - t2702 + t2760 + 
     #t2680 - t2774 + t2683 - t2685 + t2755
        t3153 = t2687 - t2689 + t2691 - t1036 - t203 - t2742 - t342 - t2
     #693 - t2766 - t499 - t2385 - t2398
        t3159 = t3100 + t3070 + t3042 - t3082 + t3047 - t3075 + t1036 + 
     #t203 - t2742 + t342 - t2693 + t2766
        t3160 = t499 - t2385 + t2398 - t3110 - t3049 - t3108 - t3051 - t
     #3054 - t3096 - t3056 - t3058 - t3060

        unew(i,j,k) = t1 + dt * t2 + (t967 * t207 / 0.6E1 + (t1015 + 
     #t1039) * t207 / 0.2E1 - t1432 * t207 / 0.6E1 - (t1436 + t1465) * t
     #207 / 0.2E1) * t15 + (t1886 * t207 / 0.6E1 + (t1920 + t1956) * t20
     #7 / 0.2E1 - t2301 * t207 / 0.6E1 - (t2305 + t2334) * t207 / 0.2E1)
     # * t94 + (t2707 * t207 / 0.6E1 + (t2710 + t2777) * t207 / 0.2E1 - 
     #t3078 * t207 / 0.6E1 - (t3083 + t3111) * t207 / 0.2E1) * t149

      utnew(i,j,k) = t2 + (t967 * dt / 0.2E1 + (t3120 + t3121) * dt - 
     #t967 * t7 - t143
     #2 * dt / 0.2E1 - (t3127 + t3128) * dt + t1432 * t7) * t15 + (t1886
     # * dt / 0.2E1 + (t3136 + t3137) * dt - t1886 * t7 - t2301 * dt / 0
     #.2E1 - (t3143 + t3144) * dt + t2301 * t7) * t94 + (t2707 * dt / 0.
     #2E1 + (t3152 + t3153) * dt - t2707 * t7 - t3078 * dt / 0.2E1 - (t3
     #159 + t3160) * dt + t3078 * t7) * t149 
        return
      end
