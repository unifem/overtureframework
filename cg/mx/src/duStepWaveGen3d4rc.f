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
        real t1008
        real t1009
        real t1010
        real t1011
        real t1012
        real t1014
        real t1016
        real t1018
        real t102
        real t1024
        real t1025
        real t103
        real t1032
        real t1038
        real t1039
        real t104
        real t1044
        real t1049
        integer t1052
        real t1054
        real t106
        real t1066
        real t107
        real t1074
        real t1075
        real t1077
        real t108
        real t1080
        real t1082
        real t1086
        real t1087
        real t1099
        integer t11
        real t110
        real t1105
        real t1113
        real t1114
        real t1116
        real t1119
        real t112
        real t1121
        real t1125
        real t1126
        real t113
        real t1138
        real t114
        real t1144
        real t1153
        real t1156
        real t1157
        real t116
        real t1169
        real t117
        real t1177
        real t1178
        real t118
        real t1180
        real t1183
        real t1185
        real t1189
        real t1190
        real t12
        real t120
        real t1216
        real t1217
        real t1219
        real t122
        real t1222
        real t1224
        real t1228
        real t1229
        real t123
        real t125
        real t1256
        real t1259
        integer t126
        real t1263
        real t1268
        real t1269
        real t127
        real t1273
        real t1278
        real t128
        real t1282
        real t1285
        real t1293
        real t1297
        real t1298
        real t13
        real t130
        real t1302
        real t1307
        real t1311
        real t1315
        real t1316
        real t132
        real t1320
        real t1325
        real t133
        real t1331
        real t1335
        real t134
        real t1343
        real t1347
        real t1351
        real t1359
        real t136
        real t1366
        real t1369
        real t137
        real t1373
        real t1379
        real t138
        real t1383
        real t1395
        real t1399
        real t140
        real t1403
        real t1405
        real t1407
        real t1409
        real t1411
        real t1413
        real t1415
        real t1417
        real t1418
        real t142
        real t1422
        real t1424
        real t1427
        real t1429
        real t143
        real t1430
        real t1432
        real t144
        real t1442
        real t1449
        real t1455
        real t1457
        real t146
        real t1461
        real t1463
        real t1464
        real t1465
        real t147
        real t1474
        real t1476
        real t1479
        real t148
        real t1481
        real t1482
        integer t1491
        real t1492
        real t1493
        real t1497
        real t1498
        real t15
        real t150
        real t1502
        real t1510
        real t1512
        real t1513
        real t1515
        real t1517
        real t1518
        real t1519
        real t152
        real t1521
        real t1522
        real t1523
        real t1525
        real t1527
        real t1529
        real t153
        real t1533
        real t1536
        real t1538
        real t1539
        real t1547
        real t1549
        real t155
        real t1553
        real t1554
        real t1558
        real t1563
        real t1567
        real t1569
        real t1571
        real t1573
        real t1575
        real t1577
        real t1579
        real t158
        real t1582
        real t1584
        real t1586
        real t1588
        real t1590
        real t1592
        real t1595
        real t1597
        real t1599
        real t16
        real t160
        real t1602
        real t1604
        real t161
        real t163
        real t1640
        real t1641
        real t1643
        real t1646
        real t1648
        real t165
        real t1652
        real t1653
        integer t166
        real t1665
        real t167
        real t1671
        real t168
        real t1680
        real t1682
        real t1683
        real t1685
        real t1688
        real t1690
        real t1694
        real t17
        real t170
        real t172
        real t173
        real t1735
        real t1736
        real t1738
        real t174
        real t1741
        real t1743
        real t1747
        real t1748
        real t176
        real t177
        real t1775
        real t1777
        real t178
        real t1780
        real t1782
        real t180
        real t1803
        real t1804
        real t1808
        real t1819
        real t182
        real t1820
        real t1824
        real t183
        real t1831
        real t1833
        real t1838
        real t185
        real t1852
        real t1853
        real t1855
        real t1856
        real t1858
        real t186
        real t1860
        real t1861
        real t1862
        real t1864
        real t1865
        real t1866
        real t1868
        real t1870
        real t1872
        real t1874
        real t1877
        real t1879
        real t188
        real t1881
        real t1883
        real t1884
        real t1886
        real t189
        real t1890
        real t1891
        real t1892
        real t1894
        real t1897
        real t1898
        real t19
        real t1905
        real t1907
        real t1908
        real t191
        real t1910
        real t1912
        real t1914
        real t1921
        real t1923
        real t1927
        real t1928
        real t1929
        real t193
        real t1931
        real t1933
        real t1935
        integer t194
        real t1942
        real t1948
        real t1949
        real t195
        real t1955
        real t1956
        real t196
        integer t1968
        real t1969
        real t1970
        real t1974
        real t1975
        real t1979
        real t198
        real t1991
        real t2
        real t200
        real t2001
        real t2003
        real t2007
        real t2008
        real t201
        real t2012
        real t2017
        real t202
        real t2021
        real t2023
        real t2027
        real t2029
        real t2032
        real t2034
        real t2036
        real t2039
        real t204
        real t2041
        real t205
        real t206
        real t2077
        real t2078
        real t208
        real t2080
        real t2083
        real t2085
        real t2089
        real t2090
        real t21
        real t210
        real t2102
        real t2108
        real t211
        real t2117
        real t2119
        real t2120
        real t2122
        real t2125
        real t2127
        real t213
        real t2131
        real t216
        real t2172
        real t2173
        real t2175
        real t2178
        real t218
        real t2180
        real t2184
        real t2185
        real t219
        integer t22
        real t221
        real t2212
        real t2214
        real t2215
        real t2218
        real t2220
        real t223
        real t224
        real t2241
        real t2242
        real t2246
        real t2257
        real t2258
        real t226
        real t2262
        real t2269
        real t227
        real t2271
        real t2273
        real t2275
        real t229
        real t2291
        real t2292
        real t2293
        real t2295
        real t2299
        real t23
        real t2301
        real t2305
        real t2306
        real t2308
        real t231
        real t2312
        real t2319
        integer t232
        real t2325
        real t233
        real t2333
        real t2334
        real t234
        real t2340
        real t2348
        real t2356
        integer t2357
        real t2359
        real t236
        real t2363
        real t2367
        real t2369
        real t2371
        real t2374
        real t2377
        real t2379
        real t238
        real t239
        real t24
        real t2402
        real t2404
        real t241
        real t242
        real t2421
        real t2422
        real t2426
        real t2429
        real t2431
        real t2432
        real t2434
        real t2436
        real t2437
        real t2439
        real t244
        real t2440
        real t2442
        real t2444
        real t2446
        real t245
        real t2450
        real t2453
        real t2455
        real t2456
        real t2458
        real t2460
        real t2462
        real t2463
        real t2465
        real t2468
        real t247
        real t2470
        real t2472
        real t2477
        real t2481
        real t2484
        real t2486
        real t249
        real t250
        real t2508
        real t2511
        real t2513
        real t252
        real t253
        real t255
        real t2550
        real t2553
        real t2555
        real t2558
        real t2560
        real t2564
        real t257
        integer t258
        real t259
        real t2590
        real t2592
        real t2595
        real t2597
        real t26
        real t260
        real t2601
        real t262
        real t264
        real t2643
        real t265
        real t2654
        real t2655
        real t2657
        real t2658
        real t2660
        real t2662
        real t2663
        real t2665
        real t2666
        real t2668
        real t267
        real t2670
        real t2672
        real t2674
        real t2676
        real t2677
        real t2680
        real t2681
        real t2684
        real t2686
        real t2688
        real t2690
        real t2692
        real t2694
        real t2696
        real t2700
        real t2702
        real t2705
        real t2707
        real t2708
        real t271
        real t2710
        real t2714
        real t2715
        real t2716
        real t2718
        real t2721
        real t2722
        real t2729
        real t273
        real t2731
        real t2732
        real t2734
        real t2736
        real t2738
        real t274
        real t2745
        real t2746
        real t275
        real t2752
        real t2753
        real t2754
        real t2756
        real t2758
        real t276
        real t2760
        real t2767
        real t2769
        real t277
        real t2773
        real t2775
        real t2779
        real t2780
        real t2787
        real t2789
        real t279
        real t2792
        real t2794
        real t2795
        real t2798
        real t28
        real t280
        real t2802
        real t2805
        real t2807
        real t281
        real t2829
        real t283
        real t2832
        real t2834
        real t285
        integer t2855
        real t2857
        real t286
        real t2869
        real t287
        real t2878
        real t2881
        real t2883
        real t2886
        real t2888
        real t289
        real t2892
        real t29
        real t290
        real t291
        real t2918
        real t2920
        real t2923
        real t2925
        real t2929
        real t293
        real t295
        real t2955
        real t2956
        real t2968
        real t297
        real t2977
        real t298
        real t2987
        real t299
        real t2995
        real t2999
        real t30
        real t3002
        real t301
        real t302
        real t3027
        real t303
        real t3031
        real t3036
        real t3039
        real t305
        real t3062
        real t3066
        real t307
        real t3070
        real t3072
        real t3076
        real t3078
        real t308
        real t3080
        real t3082
        real t3084
        real t3086
        real t3088
        real t309
        real t3090
        real t3092
        real t3094
        real t3095
        real t3097
        real t3100
        real t3102
        real t3104
        real t311
        real t3112
        real t3119
        real t312
        real t3123
        real t3125
        real t313
        real t3131
        real t3132
        real t3141
        real t3142
        real t3148
        real t3149
        real t315
        real t3157
        real t3158
        real t3164
        real t3165
        real t317
        real t3173
        real t3174
        real t3180
        real t3181
        real t319
        real t32
        real t321
        real t322
        real t323
        real t325
        real t327
        real t328
        real t329
        real t33
        real t331
        real t332
        real t333
        real t335
        real t337
        real t338
        real t339
        real t34
        real t341
        real t342
        real t343
        real t345
        real t347
        real t349
        real t35
        real t351
        real t354
        real t356
        real t357
        real t358
        real t36
        real t360
        real t362
        real t364
        real t366
        real t368
        real t370
        real t372
        real t373
        real t374
        real t376
        real t379
        real t38
        real t380
        real t383
        real t386
        real t387
        real t388
        integer t389
        real t39
        real t391
        real t393
        real t396
        real t398
        real t4
        real t402
        real t404
        real t405
        real t407
        real t41
        real t413
        real t416
        real t417
        real t42
        real t423
        real t424
        real t425
        real t427
        real t430
        real t432
        real t436
        real t437
        real t449
        real t455
        real t46
        real t463
        real t464
        real t465
        real t467
        real t470
        real t472
        real t476
        real t477
        real t48
        real t489
        real t495
        real t5
        real t504
        real t507
        real t508
        real t509
        real t51
        real t510
        real t518
        real t52
        real t524
        real t527
        real t53
        real t533
        real t534
        real t536
        real t539
        real t54
        real t541
        real t545
        real t546
        real t55
        real t56
        real t57
        real t572
        real t573
        real t575
        real t578
        real t58
        real t580
        real t584
        real t585
        real t59
        real t6
        real t60
        real t61
        real t612
        real t615
        real t619
        real t62
        real t624
        real t625
        real t629
        real t63
        real t634
        real t638
        real t641
        real t644
        real t65
        real t650
        real t654
        real t655
        real t659
        real t66
        real t664
        real t668
        real t672
        real t673
        real t677
        real t68
        real t682
        real t688
        real t692
        real t7
        real t70
        real t700
        real t704
        real t708
        integer t71
        real t716
        real t72
        real t723
        real t726
        real t73
        real t730
        real t736
        real t740
        real t75
        real t752
        real t756
        real t759
        real t76
        real t761
        real t762
        real t764
        real t767
        real t768
        integer t77
        real t774
        real t775
        real t777
        real t778
        real t78
        real t780
        real t781
        real t783
        real t784
        real t786
        real t787
        real t789
        real t79
        real t792
        real t793
        real t794
        real t795
        real t8
        real t801
        real t802
        real t804
        real t805
        real t807
        real t808
        real t81
        real t810
        real t811
        real t813
        real t814
        real t816
        real t819
        real t820
        real t821
        real t822
        real t829
        real t83
        real t831
        real t832
        real t833
        real t835
        real t836
        real t838
        integer t84
        real t841
        real t842
        real t848
        real t849
        real t85
        real t850
        real t851
        real t853
        real t854
        real t856
        real t857
        real t859
        real t86
        real t860
        real t861
        real t862
        real t864
        real t865
        real t867
        real t871
        real t873
        real t874
        real t875
        real t877
        real t879
        real t88
        real t880
        real t881
        real t887
        real t888
        real t889
        real t89
        real t890
        real t892
        real t893
        real t895
        real t896
        real t898
        real t899
        integer t9
        integer t90
        real t900
        real t901
        real t903
        real t904
        real t906
        real t91
        real t910
        real t912
        real t913
        real t914
        real t916
        real t918
        real t919
        real t92
        real t920
        real t927
        real t929
        real t932
        real t934
        real t936
        real t938
        real t94
        real t940
        real t941
        real t943
        real t944
        real t946
        real t948
        real t950
        real t952
        real t954
        real t956
        real t958
        real t96
        real t960
        real t962
        real t964
        real t965
        real t967
        real t97
        real t971
        real t975
        real t977
        real t978
        real t98
        real t980
        real t982
        real t984
        real t985
        real t992
        real t994
        real t995
        real t997
        real t999
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
        t54 = t53 * beta
        t55 = t6 ** 2
        t56 = t55 * t6
        t57 = t54 * t56
        t58 = dt ** 2
        t59 = t58 * dt
        t60 = t59 * cc
        t61 = u(t9,j,k,n)
        t62 = u(t11,j,k,n)
        t63 = t61 - t62
        t65 = t8 * t63 * t15
        t66 = t62 - t1
        t68 = t8 * t66 * t15
        t70 = (t65 - t68) * t15
        t71 = j + 1
        t72 = u(t11,t71,k,n)
        t73 = t72 - t62
        t75 = 0.1E1 / dy
        t76 = t8 * t73 * t75
        t77 = j - 1
        t78 = u(t11,t77,k,n)
        t79 = t62 - t78
        t81 = t8 * t79 * t75
        t83 = (t76 - t81) * t75
        t84 = k + 1
        t85 = u(t11,j,t84,n)
        t86 = t85 - t62
        t88 = 0.1E1 / dz
        t89 = t8 * t86 * t88
        t90 = k - 1
        t91 = u(t11,j,t90,n)
        t92 = t62 - t91
        t94 = t8 * t92 * t88
        t96 = (t89 - t94) * t88
        t97 = u(t22,j,k,n)
        t98 = t1 - t97
        t100 = t8 * t98 * t15
        t102 = (t68 - t100) * t15
        t103 = u(i,t71,k,n)
        t104 = t103 - t1
        t106 = t8 * t104 * t75
        t107 = u(i,t77,k,n)
        t108 = t1 - t107
        t110 = t8 * t108 * t75
        t112 = (t106 - t110) * t75
        t113 = u(i,j,t84,n)
        t114 = t113 - t1
        t116 = t8 * t114 * t88
        t117 = u(i,j,t90,n)
        t118 = t1 - t117
        t120 = t8 * t118 * t88
        t122 = (t116 - t120) * t88
        t123 = t70 + t83 + t96 - t102 - t112 - t122
        t125 = t8 * t123 * t15
        t126 = i - 2
        t127 = u(t126,j,k,n)
        t128 = t97 - t127
        t130 = t8 * t128 * t15
        t132 = (t100 - t130) * t15
        t133 = u(t22,t71,k,n)
        t134 = t133 - t97
        t136 = t8 * t134 * t75
        t137 = u(t22,t77,k,n)
        t138 = t97 - t137
        t140 = t8 * t138 * t75
        t142 = (t136 - t140) * t75
        t143 = u(t22,j,t84,n)
        t144 = t143 - t97
        t146 = t8 * t144 * t88
        t147 = u(t22,j,t90,n)
        t148 = t97 - t147
        t150 = t8 * t148 * t88
        t152 = (t146 - t150) * t88
        t153 = t102 + t112 + t122 - t132 - t142 - t152
        t155 = t8 * t153 * t15
        t158 = t72 - t103
        t160 = t8 * t158 * t15
        t161 = t103 - t133
        t163 = t8 * t161 * t15
        t165 = (t160 - t163) * t15
        t166 = j + 2
        t167 = u(i,t166,k,n)
        t168 = t167 - t103
        t170 = t8 * t168 * t75
        t172 = (t170 - t106) * t75
        t173 = u(i,t71,t84,n)
        t174 = t173 - t103
        t176 = t8 * t174 * t88
        t177 = u(i,t71,t90,n)
        t178 = t103 - t177
        t180 = t8 * t178 * t88
        t182 = (t176 - t180) * t88
        t183 = t165 + t172 + t182 - t102 - t112 - t122
        t185 = t8 * t183 * t75
        t186 = t78 - t107
        t188 = t8 * t186 * t15
        t189 = t107 - t137
        t191 = t8 * t189 * t15
        t193 = (t188 - t191) * t15
        t194 = j - 2
        t195 = u(i,t194,k,n)
        t196 = t107 - t195
        t198 = t8 * t196 * t75
        t200 = (t110 - t198) * t75
        t201 = u(i,t77,t84,n)
        t202 = t201 - t107
        t204 = t8 * t202 * t88
        t205 = u(i,t77,t90,n)
        t206 = t107 - t205
        t208 = t8 * t206 * t88
        t210 = (t204 - t208) * t88
        t211 = t102 + t112 + t122 - t193 - t200 - t210
        t213 = t8 * t211 * t75
        t216 = t85 - t113
        t218 = t8 * t216 * t15
        t219 = t113 - t143
        t221 = t8 * t219 * t15
        t223 = (t218 - t221) * t15
        t224 = t173 - t113
        t226 = t8 * t224 * t75
        t227 = t113 - t201
        t229 = t8 * t227 * t75
        t231 = (t226 - t229) * t75
        t232 = k + 2
        t233 = u(i,j,t232,n)
        t234 = t233 - t113
        t236 = t8 * t234 * t88
        t238 = (t236 - t116) * t88
        t239 = t223 + t231 + t238 - t102 - t112 - t122
        t241 = t8 * t239 * t88
        t242 = t91 - t117
        t244 = t8 * t242 * t15
        t245 = t117 - t147
        t247 = t8 * t245 * t15
        t249 = (t244 - t247) * t15
        t250 = t177 - t117
        t252 = t8 * t250 * t75
        t253 = t117 - t205
        t255 = t8 * t253 * t75
        t257 = (t252 - t255) * t75
        t258 = k - 2
        t259 = u(i,j,t258,n)
        t260 = t117 - t259
        t262 = t8 * t260 * t88
        t264 = (t120 - t262) * t88
        t265 = t102 + t112 + t122 - t249 - t257 - t264
        t267 = t8 * t265 * t88
        t271 = t60 * ((t125 - t155) * t15 + (t185 - t213) * t75 + (t241 
     #- t267) * t88)
        t273 = t57 * t271 / 0.12E2
        t274 = t53 * t55
        t275 = t58 * dx
        t276 = ut(t11,t71,k,n)
        t277 = t276 - t12
        t279 = t8 * t277 * t75
        t280 = ut(t11,t77,k,n)
        t281 = t12 - t280
        t283 = t8 * t281 * t75
        t285 = (t279 - t283) * t75
        t286 = ut(t11,j,t84,n)
        t287 = t286 - t12
        t289 = t8 * t287 * t88
        t290 = ut(t11,j,t90,n)
        t291 = t12 - t290
        t293 = t8 * t291 * t88
        t295 = (t289 - t293) * t88
        t297 = cc * (t21 + t285 + t295)
        t298 = ut(i,t71,k,n)
        t299 = t298 - t2
        t301 = t8 * t299 * t75
        t302 = ut(i,t77,k,n)
        t303 = t2 - t302
        t305 = t8 * t303 * t75
        t307 = (t301 - t305) * t75
        t308 = ut(i,j,t84,n)
        t309 = t308 - t2
        t311 = t8 * t309 * t88
        t312 = ut(i,j,t90,n)
        t313 = t2 - t312
        t315 = t8 * t313 * t88
        t317 = (t311 - t315) * t88
        t319 = cc * (t28 + t307 + t317)
        t321 = (t297 - t319) * t15
        t322 = ut(t126,j,k,n)
        t323 = t23 - t322
        t325 = t8 * t323 * t15
        t327 = (t26 - t325) * t15
        t328 = ut(t22,t71,k,n)
        t329 = t328 - t23
        t331 = t8 * t329 * t75
        t332 = ut(t22,t77,k,n)
        t333 = t23 - t332
        t335 = t8 * t333 * t75
        t337 = (t331 - t335) * t75
        t338 = ut(t22,j,t84,n)
        t339 = t338 - t23
        t341 = t8 * t339 * t88
        t342 = ut(t22,j,t90,n)
        t343 = t23 - t342
        t345 = t8 * t343 * t88
        t347 = (t341 - t345) * t88
        t349 = cc * (t327 + t337 + t347)
        t351 = (t319 - t349) * t15
        t354 = t275 * (t321 / 0.2E1 + t351 / 0.2E1)
        t356 = t274 * t354 / 0.8E1
        t357 = beta * t6
        t358 = dt * dx
        t360 = cc * (t70 + t83 + t96)
        t362 = cc * (t102 + t112 + t122)
        t364 = (t360 - t362) * t15
        t366 = cc * (t132 + t142 + t152)
        t368 = (t362 - t366) * t15
        t370 = t358 * (t364 - t368)
        t372 = t357 * t370 / 0.24E2
        t373 = t33 ** 2
        t374 = t8 * t373
        t376 = t58 * t123 * t15
        t379 = t373 * t33
        t380 = t8 * t379
        t383 = t59 * (t21 + t285 + t295 - t28 - t307 - t317) * t15
        t386 = beta * t33
        t387 = dt * cc
        t388 = dx ** 2
        t389 = i + 3
        t391 = u(t389,j,k,n) - t61
        t393 = t63 * t15
        t396 = t66 * t15
        t398 = (t393 - t396) * t15
        t402 = t98 * t15
        t404 = (t396 - t402) * t15
        t405 = t398 - t404
        t407 = t8 * t405 * t15
        t413 = (t15 * t391 * t8 - t65) * t15
        t416 = t70 - t102
        t417 = t416 * t15
        t423 = dy ** 2
        t424 = u(t11,t166,k,n)
        t425 = t424 - t72
        t427 = t73 * t75
        t430 = t79 * t75
        t432 = (t427 - t430) * t75
        t436 = u(t11,t194,k,n)
        t437 = t78 - t436
        t449 = (t425 * t75 * t8 - t76) * t75
        t455 = (-t437 * t75 * t8 + t81) * t75
        t463 = dz ** 2
        t464 = u(t11,j,t232,n)
        t465 = t464 - t85
        t467 = t86 * t88
        t470 = t92 * t88
        t472 = (t467 - t470) * t88
        t476 = u(t11,j,t258,n)
        t477 = t91 - t476
        t489 = (t465 * t8 * t88 - t89) * t88
        t495 = (-t477 * t8 * t88 + t94) * t88
        t504 = t387 * (-t388 * ((t8 * ((t15 * t391 - t393) * t15 - t398)
     # * t15 - t407) * t15 + ((t413 - t70) * t15 - t417) * t15) / 0.24E2
     # + t70 + t83 - t423 * ((t8 * ((t425 * t75 - t427) * t75 - t432) * 
     #t75 - t8 * (t432 - (-t437 * t75 + t430) * t75) * t75) * t75 + ((t4
     #49 - t83) * t75 - (t83 - t455) * t75) * t75) / 0.24E2 + t96 - t463
     # * ((t8 * ((t465 * t88 - t467) * t88 - t472) * t88 - t8 * (t472 - 
     #(-t477 * t88 + t470) * t88) * t88) * t88 + ((t489 - t96) * t88 - (
     #t96 - t495) * t88) * t88) / 0.24E2)
        t507 = t53 * t373
        t508 = cc * t58
        t509 = ut(t389,j,k,n)
        t510 = t509 - t10
        t518 = t8 * t42 * t15
        t524 = (t15 * t510 * t8 - t16) * t15
        t527 = t29 * t15
        t533 = ut(t11,t166,k,n)
        t534 = t533 - t276
        t536 = t277 * t75
        t539 = t281 * t75
        t541 = (t536 - t539) * t75
        t545 = ut(t11,t194,k,n)
        t546 = t280 - t545
        t572 = ut(t11,j,t232,n)
        t573 = t572 - t286
        t575 = t287 * t88
        t578 = t291 * t88
        t580 = (t575 - t578) * t88
        t584 = ut(t11,j,t258,n)
        t585 = t290 - t584
        t612 = t508 * (-t388 * ((t8 * ((t15 * t510 - t36) * t15 - t38) *
     # t15 - t518) * t15 + ((t524 - t21) * t15 - t527) * t15) / 0.24E2 +
     # t21 + t285 - t423 * ((t8 * ((t534 * t75 - t536) * t75 - t541) * t
     #75 - t8 * (t541 - (-t546 * t75 + t539) * t75) * t75) * t75 + (((t5
     #34 * t75 * t8 - t279) * t75 - t285) * t75 - (t285 - (-t546 * t75 *
     # t8 + t283) * t75) * t75) * t75) / 0.24E2 - t463 * ((t8 * ((t573 *
     # t88 - t575) * t88 - t580) * t88 - t8 * (t580 - (-t585 * t88 + t57
     #8) * t88) * t88) * t88 + (((t573 * t8 * t88 - t289) * t88 - t295) 
     #* t88 - (t295 - (-t585 * t8 * t88 + t293) * t88) * t88) * t88) / 0
     #.24E2 + t295)
        t615 = u(t9,t71,k,n)
        t619 = u(t9,t77,k,n)
        t624 = (t8 * (t615 - t61) * t75 - t8 * (t61 - t619) * t75) * t75
        t625 = u(t9,j,t84,n)
        t629 = u(t9,j,t90,n)
        t634 = (t8 * (t625 - t61) * t88 - t8 * (t61 - t629) * t88) * t88
        t638 = (cc * (t413 + t624 + t634) - t360) * t15
        t641 = t358 * (t638 / 0.2E1 + t364 / 0.2E1)
        t644 = t54 * t379
        t650 = t615 - t72
        t654 = (t15 * t650 * t8 - t160) * t15
        t655 = u(t11,t71,t84,n)
        t659 = u(t11,t71,t90,n)
        t664 = (t8 * (t655 - t72) * t88 - t8 * (t72 - t659) * t88) * t88
        t668 = t619 - t78
        t672 = (t15 * t668 * t8 - t188) * t15
        t673 = u(t11,t77,t84,n)
        t677 = u(t11,t77,t90,n)
        t682 = (t8 * (t673 - t78) * t88 - t8 * (t78 - t677) * t88) * t88
        t688 = t625 - t85
        t692 = (t15 * t688 * t8 - t218) * t15
        t700 = (t8 * (t655 - t85) * t75 - t8 * (t85 - t673) * t75) * t75
        t704 = t629 - t91
        t708 = (t15 * t704 * t8 - t244) * t15
        t716 = (t8 * (t659 - t91) * t75 - t8 * (t91 - t677) * t75) * t75
        t723 = t60 * ((t8 * (t413 + t624 + t634 - t70 - t83 - t96) * t15
     # - t125) * t15 + (t8 * (t654 + t449 + t664 - t70 - t83 - t96) * t7
     #5 - t8 * (t70 + t83 + t96 - t672 - t455 - t682) * t75) * t75 + (t8
     # * (t692 + t700 + t489 - t70 - t83 - t96) * t88 - t8 * (t70 + t83 
     #+ t96 - t708 - t716 - t495) * t88) * t88)
        t726 = ut(t9,t71,k,n)
        t730 = ut(t9,t77,k,n)
        t736 = ut(t9,j,t84,n)
        t740 = ut(t9,j,t90,n)
        t752 = t275 * ((cc * (t524 + (t8 * (t726 - t10) * t75 - t8 * (t1
     #0 - t730) * t75) * t75 + (t8 * (t736 - t10) * t88 - t8 * (t10 - t7
     #40) * t88) * t88) - t297) * t15 / 0.2E1 + t321 / 0.2E1)
        t756 = t358 * (t638 - t364)
        t759 = t128 * t15
        t761 = (t402 - t759) * t15
        t762 = t404 - t761
        t764 = t8 * t762 * t15
        t767 = t102 - t132
        t768 = t767 * t15
        t774 = t168 * t75
        t775 = t104 * t75
        t777 = (t774 - t775) * t75
        t778 = t108 * t75
        t780 = (t775 - t778) * t75
        t781 = t777 - t780
        t783 = t8 * t781 * t75
        t784 = t196 * t75
        t786 = (t778 - t784) * t75
        t787 = t780 - t786
        t789 = t8 * t787 * t75
        t792 = t172 - t112
        t793 = t792 * t75
        t794 = t112 - t200
        t795 = t794 * t75
        t801 = t234 * t88
        t802 = t114 * t88
        t804 = (t801 - t802) * t88
        t805 = t118 * t88
        t807 = (t802 - t805) * t88
        t808 = t804 - t807
        t810 = t8 * t808 * t88
        t811 = t260 * t88
        t813 = (t805 - t811) * t88
        t814 = t807 - t813
        t816 = t8 * t814 * t88
        t819 = t238 - t122
        t820 = t819 * t88
        t821 = t122 - t264
        t822 = t821 * t88
        t829 = t387 * (-t388 * ((t407 - t764) * t15 + (t417 - t768) * t1
     #5) / 0.24E2 + t102 - t423 * ((t783 - t789) * t75 + (t793 - t795) *
     # t75) / 0.24E2 + t112 - t463 * ((t810 - t816) * t88 + (t820 - t822
     #) * t88) / 0.24E2 + t122)
        t831 = t386 * t829 / 0.2E1
        t832 = t32 + t34 * t46 - t48 * t30 / 0.24E2 - t52 + t273 + t356 
     #+ t372 + t374 * t376 / 0.2E1 + t380 * t383 / 0.6E1 + t386 * t504 /
     # 0.2E1 + t507 * t612 / 0.4E1 - t386 * t641 / 0.4E1 + t644 * t723 /
     # 0.12E2 - t507 * t752 / 0.8E1 + t386 * t756 / 0.24E2 - t831
        t833 = t323 * t15
        t835 = (t39 - t833) * t15
        t836 = t41 - t835
        t838 = t8 * t836 * t15
        t841 = t28 - t327
        t842 = t841 * t15
        t848 = ut(i,t166,k,n)
        t849 = t848 - t298
        t850 = t849 * t75
        t851 = t299 * t75
        t853 = (t850 - t851) * t75
        t854 = t303 * t75
        t856 = (t851 - t854) * t75
        t857 = t853 - t856
        t859 = t8 * t857 * t75
        t860 = ut(i,t194,k,n)
        t861 = t302 - t860
        t862 = t861 * t75
        t864 = (t854 - t862) * t75
        t865 = t856 - t864
        t867 = t8 * t865 * t75
        t871 = t8 * t849 * t75
        t873 = (t871 - t301) * t75
        t874 = t873 - t307
        t875 = t874 * t75
        t877 = t8 * t861 * t75
        t879 = (t305 - t877) * t75
        t880 = t307 - t879
        t881 = t880 * t75
        t887 = ut(i,j,t232,n)
        t888 = t887 - t308
        t889 = t888 * t88
        t890 = t309 * t88
        t892 = (t889 - t890) * t88
        t893 = t313 * t88
        t895 = (t890 - t893) * t88
        t896 = t892 - t895
        t898 = t8 * t896 * t88
        t899 = ut(i,j,t258,n)
        t900 = t312 - t899
        t901 = t900 * t88
        t903 = (t893 - t901) * t88
        t904 = t895 - t903
        t906 = t8 * t904 * t88
        t910 = t8 * t888 * t88
        t912 = (t910 - t311) * t88
        t913 = t912 - t317
        t914 = t913 * t88
        t916 = t8 * t900 * t88
        t918 = (t315 - t916) * t88
        t919 = t317 - t918
        t920 = t919 * t88
        t927 = t508 * (-t388 * ((t518 - t838) * t15 + (t527 - t842) * t1
     #5) / 0.24E2 + t28 - t423 * ((t859 - t867) * t75 + (t875 - t881) * 
     #t75) / 0.24E2 + t307 - t463 * ((t898 - t906) * t88 + (t914 - t920)
     # * t88) / 0.24E2 + t317)
        t929 = t507 * t927 / 0.4E1
        t932 = t358 * (t364 / 0.2E1 + t368 / 0.2E1)
        t934 = t386 * t932 / 0.4E1
        t936 = t644 * t271 / 0.12E2
        t938 = t507 * t354 / 0.8E1
        t940 = t386 * t370 / 0.24E2
        t941 = t8 * t55
        t943 = t941 * t376 / 0.2E1
        t944 = t8 * t56
        t946 = t944 * t383 / 0.6E1
        t948 = t357 * t504 / 0.2E1
        t950 = t274 * t612 / 0.4E1
        t952 = t357 * t641 / 0.4E1
        t954 = t57 * t723 / 0.12E2
        t956 = t274 * t752 / 0.8E1
        t958 = t357 * t756 / 0.24E2
        t960 = t357 * t829 / 0.2E1
        t962 = t274 * t927 / 0.4E1
        t964 = t357 * t932 / 0.4E1
        t965 = -t929 - t934 - t936 - t938 - t940 - t943 - t946 - t948 - 
     #t950 + t952 - t954 + t956 - t958 + t960 + t962 + t964
        t967 = (t832 + t965) * t4
        t971 = dx * t416 / 0.24E2
        t975 = t8 * (t396 - dx * t405 / 0.24E2)
        t977 = cc * t12
        t978 = cc * t10
        t980 = (-t977 + t978) * t15
        t982 = cc * t2
        t984 = (-t982 + t977) * t15
        t985 = t984 / 0.2E1
        t992 = (t980 - t984) * t15
        t994 = (((cc * t509 - t978) * t15 - t980) * t15 - t992) * t15
        t995 = cc * t23
        t997 = (t982 - t995) * t15
        t999 = (t984 - t997) * t15
        t1001 = (t992 - t999) * t15
        t1008 = dx * (t980 / 0.2E1 + t985 - t388 * (t994 / 0.2E1 + t1001
     # / 0.2E1) / 0.6E1) / 0.4E1
        t1009 = -t6 * t967 - t1008 - t273 - t32 - t356 - t372 + t52 + t9
     #43 + t946 + t948 - t971 + t975
        t1010 = t982 / 0.2E1
        t1011 = t977 / 0.2E1
        t1012 = cc * t322
        t1014 = (-t1012 + t995) * t15
        t1016 = (t997 - t1014) * t15
        t1018 = (t999 - t1016) * t15
        t1024 = t388 * (t999 - dx * (t1001 - t1018) / 0.12E2) / 0.24E2
        t1025 = t997 / 0.2E1
        t1032 = dx * (t985 + t1025 - t388 * (t1001 / 0.2E1 + t1018 / 0.2
     #E1) / 0.6E1) / 0.4E1
        t1038 = t388 * (t992 - dx * (t994 - t1001) / 0.12E2) / 0.24E2
        t1039 = t950 - t952 + t954 - t956 + t958 - t960 - t962 - t964 - 
     #t1010 + t1011 - t1024 - t1032 + t1038
        t1044 = t58 * t153 * t15
        t1049 = t59 * (t28 + t307 + t317 - t327 - t337 - t347) * t15
        t1052 = i - 3
        t1054 = t127 - u(t1052,j,k,n)
        t1066 = (-t1054 * t15 * t8 + t130) * t15
        t1074 = u(t22,t166,k,n)
        t1075 = t1074 - t133
        t1077 = t134 * t75
        t1080 = t138 * t75
        t1082 = (t1077 - t1080) * t75
        t1086 = u(t22,t194,k,n)
        t1087 = t137 - t1086
        t1099 = (t1075 * t75 * t8 - t136) * t75
        t1105 = (-t1087 * t75 * t8 + t140) * t75
        t1113 = u(t22,j,t232,n)
        t1114 = t1113 - t143
        t1116 = t144 * t88
        t1119 = t148 * t88
        t1121 = (t1116 - t1119) * t88
        t1125 = u(t22,j,t258,n)
        t1126 = t147 - t1125
        t1138 = (t1114 * t8 * t88 - t146) * t88
        t1144 = (-t1126 * t8 * t88 + t150) * t88
        t1153 = t387 * (-t388 * ((t764 - t8 * (t761 - (-t1054 * t15 + t7
     #59) * t15) * t15) * t15 + (t768 - (t132 - t1066) * t15) * t15) / 0
     #.24E2 + t132 + t142 - t423 * ((t8 * ((t1075 * t75 - t1077) * t75 -
     # t1082) * t75 - t8 * (t1082 - (-t1087 * t75 + t1080) * t75) * t75)
     # * t75 + ((t1099 - t142) * t75 - (t142 - t1105) * t75) * t75) / 0.
     #24E2 + t152 - t463 * ((t8 * ((t1114 * t88 - t1116) * t88 - t1121) 
     #* t88 - t8 * (t1121 - (-t1126 * t88 + t1119) * t88) * t88) * t88 +
     # ((t1138 - t152) * t88 - (t152 - t1144) * t88) * t88) / 0.24E2)
        t1156 = ut(t1052,j,k,n)
        t1157 = t322 - t1156
        t1169 = (-t1157 * t15 * t8 + t325) * t15
        t1177 = ut(t22,t166,k,n)
        t1178 = t1177 - t328
        t1180 = t329 * t75
        t1183 = t333 * t75
        t1185 = (t1180 - t1183) * t75
        t1189 = ut(t22,t194,k,n)
        t1190 = t332 - t1189
        t1216 = ut(t22,j,t232,n)
        t1217 = t1216 - t338
        t1219 = t339 * t88
        t1222 = t343 * t88
        t1224 = (t1219 - t1222) * t88
        t1228 = ut(t22,j,t258,n)
        t1229 = t342 - t1228
        t1256 = t508 * (-t388 * ((t838 - t8 * (t835 - (-t1157 * t15 + t8
     #33) * t15) * t15) * t15 + (t842 - (t327 - t1169) * t15) * t15) / 0
     #.24E2 + t327 - t423 * ((t8 * ((t1178 * t75 - t1180) * t75 - t1185)
     # * t75 - t8 * (t1185 - (-t1190 * t75 + t1183) * t75) * t75) * t75 
     #+ (((t1178 * t75 * t8 - t331) * t75 - t337) * t75 - (t337 - (-t119
     #0 * t75 * t8 + t335) * t75) * t75) * t75) / 0.24E2 + t337 - t463 *
     # ((t8 * ((t1217 * t88 - t1219) * t88 - t1224) * t88 - t8 * (t1224 
     #- (-t1229 * t88 + t1222) * t88) * t88) * t88 + (((t1217 * t8 * t88
     # - t341) * t88 - t347) * t88 - (t347 - (-t1229 * t8 * t88 + t345) 
     #* t88) * t88) * t88) / 0.24E2 + t347)
        t1259 = u(t126,t71,k,n)
        t1263 = u(t126,t77,k,n)
        t1268 = (t8 * (t1259 - t127) * t75 - t8 * (t127 - t1263) * t75) 
     #* t75
        t1269 = u(t126,j,t84,n)
        t1273 = u(t126,j,t90,n)
        t1278 = (t8 * (t1269 - t127) * t88 - t8 * (t127 - t1273) * t88) 
     #* t88
        t1282 = (t366 - cc * (t1066 + t1268 + t1278)) * t15
        t1285 = t358 * (t368 / 0.2E1 + t1282 / 0.2E1)
        t1293 = t133 - t1259
        t1297 = (-t1293 * t15 * t8 + t163) * t15
        t1298 = u(t22,t71,t84,n)
        t1302 = u(t22,t71,t90,n)
        t1307 = (t8 * (t1298 - t133) * t88 - t8 * (t133 - t1302) * t88) 
     #* t88
        t1311 = t137 - t1263
        t1315 = (-t1311 * t15 * t8 + t191) * t15
        t1316 = u(t22,t77,t84,n)
        t1320 = u(t22,t77,t90,n)
        t1325 = (t8 * (t1316 - t137) * t88 - t8 * (t137 - t1320) * t88) 
     #* t88
        t1331 = t143 - t1269
        t1335 = (-t1331 * t15 * t8 + t221) * t15
        t1343 = (t8 * (t1298 - t143) * t75 - t8 * (t143 - t1316) * t75) 
     #* t75
        t1347 = t147 - t1273
        t1351 = (-t1347 * t15 * t8 + t247) * t15
        t1359 = (t8 * (t1302 - t147) * t75 - t8 * (t147 - t1320) * t75) 
     #* t75
        t1366 = t60 * ((t155 - t8 * (t132 + t142 + t152 - t1066 - t1268 
     #- t1278) * t15) * t15 + (t8 * (t1297 + t1099 + t1307 - t132 - t142
     # - t152) * t75 - t8 * (t132 + t142 + t152 - t1315 - t1105 - t1325)
     # * t75) * t75 + (t8 * (t1335 + t1343 + t1138 - t132 - t142 - t152)
     # * t88 - t8 * (t132 + t142 + t152 - t1351 - t1359 - t1144) * t88) 
     #* t88)
        t1369 = ut(t126,t71,k,n)
        t1373 = ut(t126,t77,k,n)
        t1379 = ut(t126,j,t84,n)
        t1383 = ut(t126,j,t90,n)
        t1395 = t275 * (t351 / 0.2E1 + (t349 - cc * (t1169 + (t8 * (t136
     #9 - t322) * t75 - t8 * (t322 - t1373) * t75) * t75 + (t8 * (t1379 
     #- t322) * t88 - t8 * (t322 - t1383) * t88) * t88)) * t15 / 0.2E1)
        t1399 = t358 * (t368 - t1282)
        t1403 = t941 * t1044 / 0.2E1
        t1405 = t944 * t1049 / 0.6E1
        t1407 = t357 * t1153 / 0.2E1
        t1409 = t274 * t1256 / 0.4E1
        t1411 = t357 * t1285 / 0.4E1
        t1413 = t57 * t1366 / 0.12E2
        t1415 = t274 * t1395 / 0.8E1
        t1417 = t357 * t1399 / 0.24E2
        t1418 = t374 * t1044 / 0.2E1 + t380 * t1049 / 0.6E1 - t386 * t11
     #53 / 0.2E1 - t507 * t1256 / 0.4E1 - t386 * t1285 / 0.4E1 - t644 * 
     #t1366 / 0.12E2 - t507 * t1395 / 0.8E1 - t386 * t1399 / 0.24E2 - t1
     #403 - t1405 + t1407 + t1409 + t1411 + t1413 + t1415 + t1417
        t1422 = dt * (t39 - dx * t836 / 0.24E2)
        t1424 = dx * t841
        t1427 = t51 * t1422
        t1429 = t7 * t1424 / 0.24E2
        t1430 = -t273 + t356 - t372 + t831 + t929 - t934 + t936 - t938 +
     # t940 - t960 - t962 + t964 + t34 * t1422 - t48 * t1424 / 0.24E2 - 
     #t1427 + t1429
        t1432 = (t1418 + t1430) * t4
        t1442 = (t1016 - (t1014 - (-cc * t1156 + t1012) * t15) * t15) * 
     #t15
        t1449 = dx * (t1025 + t1014 / 0.2E1 - t388 * (t1018 / 0.2E1 + t1
     #442 / 0.2E1) / 0.6E1) / 0.4E1
        t1455 = t388 * (t1016 - dx * (t1018 - t1442) / 0.12E2) / 0.24E2
        t1457 = dx * t767 / 0.24E2
        t1461 = t8 * (t402 - dx * t762 / 0.24E2)
        t1463 = -t1432 * t6 + t1403 + t1405 - t1407 - t1409 - t1411 - t1
     #413 - t1415 - t1449 - t1455 - t1457 + t1461
        t1464 = t995 / 0.2E1
        t1465 = -t1417 + t273 - t356 + t372 + t960 + t962 - t964 + t1427
     # - t1429 + t1010 - t1464 + t1024 - t1032
        t1474 = dt * (t851 - dy * t857 / 0.24E2)
        t1476 = dy * t874
        t1479 = t51 * t1474
        t1481 = t7 * t1476 / 0.24E2
        t1482 = t58 * dy
        t1491 = j + 3
        t1492 = ut(i,t1491,k,n)
        t1493 = t1492 - t848
        t1497 = (t1493 * t75 * t8 - t871) * t75
        t1498 = ut(i,t166,t84,n)
        t1502 = ut(i,t166,t90,n)
        t1510 = t276 - t298
        t1512 = t8 * t1510 * t15
        t1513 = t298 - t328
        t1515 = t8 * t1513 * t15
        t1517 = (t1512 - t1515) * t15
        t1518 = ut(i,t71,t84,n)
        t1519 = t1518 - t298
        t1521 = t8 * t1519 * t88
        t1522 = ut(i,t71,t90,n)
        t1523 = t298 - t1522
        t1525 = t8 * t1523 * t88
        t1527 = (t1521 - t1525) * t88
        t1529 = cc * (t1517 + t873 + t1527)
        t1533 = (t1529 - t319) * t75
        t1536 = t1482 * ((cc * ((t8 * (t533 - t848) * t15 - t8 * (t848 -
     # t1177) * t15) * t15 + t1497 + (t8 * (t1498 - t848) * t88 - t8 * (
     #t848 - t1502) * t88) * t88) - t1529) * t75 / 0.2E1 + t1533 / 0.2E1
     #)
        t1538 = t274 * t1536 / 0.8E1
        t1539 = dt * dy
        t1547 = (t8 * (t424 - t167) * t15 - t8 * (t167 - t1074) * t15) *
     # t15
        t1549 = u(i,t1491,k,n) - t167
        t1553 = (t1549 * t75 * t8 - t170) * t75
        t1554 = u(i,t166,t84,n)
        t1558 = u(i,t166,t90,n)
        t1563 = (t8 * (t1554 - t167) * t88 - t8 * (t167 - t1558) * t88) 
     #* t88
        t1567 = cc * (t165 + t172 + t182)
        t1569 = (cc * (t1547 + t1553 + t1563) - t1567) * t75
        t1571 = (t1567 - t362) * t75
        t1573 = t1539 * (t1569 - t1571)
        t1575 = t357 * t1573 / 0.24E2
        t1577 = cc * (t193 + t200 + t210)
        t1579 = (t362 - t1577) * t75
        t1582 = t1539 * (t1571 / 0.2E1 + t1579 / 0.2E1)
        t1584 = t357 * t1582 / 0.4E1
        t1586 = t1539 * (t1571 - t1579)
        t1588 = t386 * t1586 / 0.24E2
        t1590 = t58 * t183 * t75
        t1592 = t941 * t1590 / 0.2E1
        t1595 = t59 * (t1517 + t873 + t1527 - t28 - t307 - t317) * t75
        t1597 = t944 * t1595 / 0.6E1
        t1599 = t158 * t15
        t1602 = t161 * t15
        t1604 = (t1599 - t1602) * t15
        t1640 = u(i,t71,t232,n)
        t1641 = t1640 - t173
        t1643 = t174 * t88
        t1646 = t178 * t88
        t1648 = (t1643 - t1646) * t88
        t1652 = u(i,t71,t258,n)
        t1653 = t177 - t1652
        t1665 = (t1641 * t8 * t88 - t176) * t88
        t1671 = (-t1653 * t8 * t88 + t180) * t88
        t1680 = t387 * (-t388 * ((t8 * ((t15 * t650 - t1599) * t15 - t16
     #04) * t15 - t8 * (t1604 - (-t1293 * t15 + t1602) * t15) * t15) * t
     #15 + ((t654 - t165) * t15 - (t165 - t1297) * t15) * t15) / 0.24E2 
     #+ t165 - t423 * ((t8 * ((t1549 * t75 - t774) * t75 - t777) * t75 -
     # t783) * t75 + ((t1553 - t172) * t75 - t793) * t75) / 0.24E2 + t17
     #2 - t463 * ((t8 * ((t1641 * t88 - t1643) * t88 - t1648) * t88 - t8
     # * (t1648 - (-t1653 * t88 + t1646) * t88) * t88) * t88 + ((t1665 -
     # t182) * t88 - (t182 - t1671) * t88) * t88) / 0.24E2 + t182)
        t1682 = t357 * t1680 / 0.2E1
        t1683 = t726 - t276
        t1685 = t1510 * t15
        t1688 = t1513 * t15
        t1690 = (t1685 - t1688) * t15
        t1694 = t328 - t1369
        t1735 = ut(i,t71,t232,n)
        t1736 = t1735 - t1518
        t1738 = t1519 * t88
        t1741 = t1523 * t88
        t1743 = (t1738 - t1741) * t88
        t1747 = ut(i,t71,t258,n)
        t1748 = t1522 - t1747
        t1775 = t508 * (-t388 * ((t8 * ((t15 * t1683 - t1685) * t15 - t1
     #690) * t15 - t8 * (t1690 - (-t15 * t1694 + t1688) * t15) * t15) * 
     #t15 + (((t15 * t1683 * t8 - t1512) * t15 - t1517) * t15 - (t1517 -
     # (-t15 * t1694 * t8 + t1515) * t15) * t15) * t15) / 0.24E2 + t1517
     # + t873 - t423 * ((t8 * ((t1493 * t75 - t850) * t75 - t853) * t75 
     #- t859) * t75 + ((t1497 - t873) * t75 - t875) * t75) / 0.24E2 - t4
     #63 * ((t8 * ((t1736 * t88 - t1738) * t88 - t1743) * t88 - t8 * (t1
     #743 - (-t1748 * t88 + t1741) * t88) * t88) * t88 + (((t1736 * t8 *
     # t88 - t1521) * t88 - t1527) * t88 - (t1527 - (-t1748 * t8 * t88 +
     # t1525) * t88) * t88) * t88) / 0.24E2 + t1527)
        t1777 = t274 * t1775 / 0.4E1
        t1780 = t1539 * (t1569 / 0.2E1 + t1571 / 0.2E1)
        t1782 = t357 * t1780 / 0.4E1
        t1803 = (t8 * (t655 - t173) * t15 - t8 * (t173 - t1298) * t15) *
     # t15
        t1804 = t1554 - t173
        t1808 = (t1804 * t75 * t8 - t226) * t75
        t1819 = (t8 * (t659 - t177) * t15 - t8 * (t177 - t1302) * t15) *
     # t15
        t1820 = t1558 - t177
        t1824 = (t1820 * t75 * t8 - t252) * t75
        t1831 = t60 * ((t8 * (t654 + t449 + t664 - t165 - t172 - t182) *
     # t15 - t8 * (t165 + t172 + t182 - t1297 - t1099 - t1307) * t15) * 
     #t15 + (t8 * (t1547 + t1553 + t1563 - t165 - t172 - t182) * t75 - t
     #185) * t75 + (t8 * (t1803 + t1808 + t1665 - t165 - t172 - t182) * 
     #t88 - t8 * (t165 + t172 + t182 - t1819 - t1824 - t1671) * t88) * t
     #88)
        t1833 = t57 * t1831 / 0.12E2
        t1838 = t34 * t1474 - t48 * t1476 / 0.24E2 - t1479 + t1481 + t15
     #38 - t1575 + t1584 - t1588 - t1592 - t1597 - t1682 - t1777 + t1782
     # - t1833 + t374 * t1590 / 0.2E1 + t380 * t1595 / 0.6E1
        t1852 = t386 * t1582 / 0.4E1
        t1853 = t280 - t302
        t1855 = t8 * t1853 * t15
        t1856 = t302 - t332
        t1858 = t8 * t1856 * t15
        t1860 = (t1855 - t1858) * t15
        t1861 = ut(i,t77,t84,n)
        t1862 = t1861 - t302
        t1864 = t8 * t1862 * t88
        t1865 = ut(i,t77,t90,n)
        t1866 = t302 - t1865
        t1868 = t8 * t1866 * t88
        t1870 = (t1864 - t1868) * t88
        t1872 = cc * (t1860 + t879 + t1870)
        t1874 = (t319 - t1872) * t75
        t1877 = t1482 * (t1533 / 0.2E1 + t1874 / 0.2E1)
        t1879 = t507 * t1877 / 0.8E1
        t1881 = t274 * t1877 / 0.8E1
        t1883 = t357 * t1586 / 0.24E2
        t1884 = t386 * t1680 / 0.2E1 + t507 * t1775 / 0.4E1 - t386 * t17
     #80 / 0.4E1 + t644 * t1831 / 0.12E2 - t507 * t1536 / 0.8E1 + t386 *
     # t1573 / 0.24E2 - t1852 - t1879 + t273 - t831 - t929 - t936 + t960
     # + t962 + t1881 + t1883
        t1886 = (t1838 + t1884) * t4
        t1890 = -t1886 * t6 + t1479 - t1481 - t1538 + t1575 - t1584 + t1
     #592 + t1597 + t1682 + t1777 - t1782 + t1833
        t1891 = cc * t298
        t1892 = cc * t848
        t1894 = (-t1891 + t1892) * t75
        t1897 = (-t982 + t1891) * t75
        t1898 = t1897 / 0.2E1
        t1905 = (t1894 - t1897) * t75
        t1907 = (((cc * t1492 - t1892) * t75 - t1894) * t75 - t1905) * t
     #75
        t1908 = cc * t302
        t1910 = (t982 - t1908) * t75
        t1912 = (t1897 - t1910) * t75
        t1914 = (t1905 - t1912) * t75
        t1921 = dy * (t1894 / 0.2E1 + t1898 - t423 * (t1907 / 0.2E1 + t1
     #914 / 0.2E1) / 0.6E1) / 0.4E1
        t1923 = dy * t792 / 0.24E2
        t1927 = t8 * (t775 - dy * t781 / 0.24E2)
        t1928 = t1910 / 0.2E1
        t1929 = cc * t860
        t1931 = (-t1929 + t1908) * t75
        t1933 = (t1910 - t1931) * t75
        t1935 = (t1912 - t1933) * t75
        t1942 = dy * (t1898 + t1928 - t423 * (t1914 / 0.2E1 + t1935 / 0.
     #2E1) / 0.6E1) / 0.4E1
        t1948 = t423 * (t1905 - dy * (t1907 - t1914) / 0.12E2) / 0.24E2
        t1949 = t1891 / 0.2E1
        t1955 = t423 * (t1912 - dy * (t1914 - t1935) / 0.12E2) / 0.24E2
        t1956 = -t273 - t1921 - t1923 + t1927 - t960 - t962 - t1942 + t1
     #948 + t1949 - t1955 - t1881 - t1883 - t1010
        t1968 = j - 3
        t1969 = ut(i,t1968,k,n)
        t1970 = t860 - t1969
        t1974 = (-t1970 * t75 * t8 + t877) * t75
        t1975 = ut(i,t194,t84,n)
        t1979 = ut(i,t194,t90,n)
        t1991 = t1482 * (t1874 / 0.2E1 + (t1872 - cc * ((t8 * (t545 - t8
     #60) * t15 - t8 * (t860 - t1189) * t15) * t15 + t1974 + (t8 * (t197
     #5 - t860) * t88 - t8 * (t860 - t1979) * t88) * t88)) * t75 / 0.2E1
     #)
        t2001 = (t8 * (t436 - t195) * t15 - t8 * (t195 - t1086) * t15) *
     # t15
        t2003 = t195 - u(i,t1968,k,n)
        t2007 = (-t2003 * t75 * t8 + t198) * t75
        t2008 = u(i,t194,t84,n)
        t2012 = u(i,t194,t90,n)
        t2017 = (t8 * (t2008 - t195) * t88 - t8 * (t195 - t2012) * t88) 
     #* t88
        t2021 = (t1577 - cc * (t2001 + t2007 + t2017)) * t75
        t2023 = t1539 * (t1579 - t2021)
        t2027 = t58 * t211 * t75
        t2029 = t941 * t2027 / 0.2E1
        t2032 = t59 * (t28 + t307 + t317 - t1860 - t879 - t1870) * t75
        t2034 = t944 * t2032 / 0.6E1
        t2036 = t186 * t15
        t2039 = t189 * t15
        t2041 = (t2036 - t2039) * t15
        t2077 = u(i,t77,t232,n)
        t2078 = t2077 - t201
        t2080 = t202 * t88
        t2083 = t206 * t88
        t2085 = (t2080 - t2083) * t88
        t2089 = u(i,t77,t258,n)
        t2090 = t205 - t2089
        t2102 = (t2078 * t8 * t88 - t204) * t88
        t2108 = (-t2090 * t8 * t88 + t208) * t88
        t2117 = t387 * (-t388 * ((t8 * ((t15 * t668 - t2036) * t15 - t20
     #41) * t15 - t8 * (t2041 - (-t1311 * t15 + t2039) * t15) * t15) * t
     #15 + ((t672 - t193) * t15 - (t193 - t1315) * t15) * t15) / 0.24E2 
     #+ t193 + t200 - t423 * ((t789 - t8 * (t786 - (-t2003 * t75 + t784)
     # * t75) * t75) * t75 + (t795 - (t200 - t2007) * t75) * t75) / 0.24
     #E2 + t210 - t463 * ((t8 * ((t2078 * t88 - t2080) * t88 - t2085) * 
     #t88 - t8 * (t2085 - (-t2090 * t88 + t2083) * t88) * t88) * t88 + (
     #(t2102 - t210) * t88 - (t210 - t2108) * t88) * t88) / 0.24E2)
        t2119 = t357 * t2117 / 0.2E1
        t2120 = t730 - t280
        t2122 = t1853 * t15
        t2125 = t1856 * t15
        t2127 = (t2122 - t2125) * t15
        t2131 = t332 - t1373
        t2172 = ut(i,t77,t232,n)
        t2173 = t2172 - t1861
        t2175 = t1862 * t88
        t2178 = t1866 * t88
        t2180 = (t2175 - t2178) * t88
        t2184 = ut(i,t77,t258,n)
        t2185 = t1865 - t2184
        t2212 = t508 * (-t388 * ((t8 * ((t15 * t2120 - t2122) * t15 - t2
     #127) * t15 - t8 * (t2127 - (-t15 * t2131 + t2125) * t15) * t15) * 
     #t15 + (((t15 * t2120 * t8 - t1855) * t15 - t1860) * t15 - (t1860 -
     # (-t15 * t2131 * t8 + t1858) * t15) * t15) * t15) / 0.24E2 + t1860
     # + t879 - t423 * ((t867 - t8 * (t864 - (-t1970 * t75 + t862) * t75
     #) * t75) * t75 + (t881 - (t879 - t1974) * t75) * t75) / 0.24E2 - t
     #463 * ((t8 * ((t2173 * t88 - t2175) * t88 - t2180) * t88 - t8 * (t
     #2180 - (-t2185 * t88 + t2178) * t88) * t88) * t88 + (((t2173 * t8 
     #* t88 - t1864) * t88 - t1870) * t88 - (t1870 - (-t2185 * t8 * t88 
     #+ t1868) * t88) * t88) * t88) / 0.24E2 + t1870)
        t2214 = t274 * t2212 / 0.4E1
        t2215 = t1584 + t1588 - t1852 - t1879 - t273 + t831 + t929 + t93
     #6 - t960 - t962 - t507 * t1991 / 0.8E1 - t386 * t2023 / 0.24E2 - t
     #2029 - t2034 + t2119 + t2214
        t2218 = t1539 * (t1579 / 0.2E1 + t2021 / 0.2E1)
        t2220 = t357 * t2218 / 0.4E1
        t2241 = (t8 * (t673 - t201) * t15 - t8 * (t201 - t1316) * t15) *
     # t15
        t2242 = t201 - t2008
        t2246 = (-t2242 * t75 * t8 + t229) * t75
        t2257 = (t8 * (t677 - t205) * t15 - t8 * (t205 - t1320) * t15) *
     # t15
        t2258 = t205 - t2012
        t2262 = (-t2258 * t75 * t8 + t255) * t75
        t2269 = t60 * ((t8 * (t672 + t455 + t682 - t193 - t200 - t210) *
     # t15 - t8 * (t193 + t200 + t210 - t1315 - t1105 - t1325) * t15) * 
     #t15 + (t213 - t8 * (t193 + t200 + t210 - t2001 - t2007 - t2017) * 
     #t75) * t75 + (t8 * (t2241 + t2246 + t2102 - t193 - t200 - t210) * 
     #t88 - t8 * (t193 + t200 + t210 - t2257 - t2262 - t2108) * t88) * t
     #88)
        t2271 = t57 * t2269 / 0.12E2
        t2273 = t274 * t1991 / 0.8E1
        t2275 = t357 * t2023 / 0.24E2
        t2291 = dt * (t854 - dy * t865 / 0.24E2)
        t2292 = t51 * t2291
        t2293 = dy * t880
        t2295 = t7 * t2293 / 0.24E2
        t2299 = t2220 + t2271 + t2273 + t2275 + t374 * t2027 / 0.2E1 + t
     #380 * t2032 / 0.6E1 - t386 * t2117 / 0.2E1 - t507 * t2212 / 0.4E1 
     #- t386 * t2218 / 0.4E1 - t644 * t2269 / 0.12E2 - t2292 + t2295 + t
     #34 * t2291 - t48 * t2293 / 0.24E2 + t1881 - t1883
        t2301 = (t2215 + t2299) * t4
        t2305 = -t2301 * t6 - t1584 + t2029 + t2034 - t2119 - t2214 - t2
     #220 - t2271 - t2273 + t273 + t960 + t962
        t2306 = t1908 / 0.2E1
        t2308 = dy * t794 / 0.24E2
        t2312 = t8 * (t778 - dy * t787 / 0.24E2)
        t2319 = (t1933 - (t1931 - (-cc * t1969 + t1929) * t75) * t75) * 
     #t75
        t2325 = t423 * (t1933 - dy * (t1935 - t2319) / 0.12E2) / 0.24E2
        t2333 = dy * (t1928 + t1931 / 0.2E1 - t423 * (t1935 / 0.2E1 + t2
     #319 / 0.2E1) / 0.6E1) / 0.4E1
        t2334 = -t2275 - t1942 - t2306 + t1955 + t2292 - t2295 - t1881 +
     # t1883 + t1010 - t2308 + t2312 - t2325 - t2333
        t2340 = t386 * dt
        t2348 = (t8 * (t464 - t233) * t15 - t8 * (t233 - t1113) * t15) *
     # t15
        t2356 = (t8 * (t1640 - t233) * t75 - t8 * (t233 - t2077) * t75) 
     #* t75
        t2357 = k + 3
        t2359 = u(i,j,t2357,n) - t233
        t2363 = (t2359 * t8 * t88 - t236) * t88
        t2367 = cc * (t223 + t231 + t238)
        t2369 = (cc * (t2348 + t2356 + t2363) - t2367) * t88
        t2371 = (t2367 - t362) * t88
        t2374 = t423 * (t2369 - t2371) * t88
        t2377 = t357 * dt
        t2379 = t2377 * t2374 / 0.24E2
        t2402 = t60 * ((t8 * (t692 + t700 + t489 - t223 - t231 - t238) *
     # t15 - t8 * (t223 + t231 + t238 - t1335 - t1343 - t1138) * t15) * 
     #t15 + (t8 * (t1803 + t1808 + t1665 - t223 - t231 - t238) * t75 - t
     #8 * (t223 + t231 + t238 - t2241 - t2246 - t2102) * t75) * t75 + (t
     #8 * (t2348 + t2356 + t2363 - t223 - t231 - t238) * t88 - t241) * t
     #88)
        t2404 = t57 * t2402 / 0.12E2
        t2421 = ut(i,j,t2357,n)
        t2422 = t2421 - t887
        t2426 = (t2422 * t8 * t88 - t910) * t88
        t2429 = t286 - t308
        t2431 = t8 * t2429 * t15
        t2432 = t308 - t338
        t2434 = t8 * t2432 * t15
        t2436 = (t2431 - t2434) * t15
        t2437 = t1518 - t308
        t2439 = t8 * t2437 * t75
        t2440 = t308 - t1861
        t2442 = t8 * t2440 * t75
        t2444 = (t2439 - t2442) * t75
        t2446 = cc * (t2436 + t2444 + t912)
        t2450 = (t2446 - t319) * t88
        t2453 = t1482 * ((cc * ((t8 * (t572 - t887) * t15 - t8 * (t887 -
     # t1216) * t15) * t15 + (t8 * (t1735 - t887) * t75 - t8 * (t887 - t
     #2172) * t75) * t75 + t2426) - t2446) * t88 / 0.2E1 + t2450 / 0.2E1
     #)
        t2455 = t274 * t2453 / 0.8E1
        t2456 = dt * dz
        t2458 = cc * (t249 + t257 + t264)
        t2460 = (t362 - t2458) * t88
        t2462 = t2371 / 0.2E1 + t2460 / 0.2E1
        t2463 = t2456 * t2462
        t2465 = t357 * t2463 / 0.4E1
        t2468 = t1539 * (t2369 / 0.2E1 + t2371 / 0.2E1)
        t2470 = t357 * t2468 / 0.4E1
        t2472 = t58 * t239 * t88
        t2477 = t59 * (t2436 + t2444 + t912 - t28 - t307 - t317) * t88
        t2481 = t216 * t15
        t2484 = t219 * t15
        t2486 = (t2481 - t2484) * t15
        t2508 = t224 * t75
        t2511 = t227 * t75
        t2513 = (t2508 - t2511) * t75
        t2550 = t387 * (-t388 * ((t8 * ((t15 * t688 - t2481) * t15 - t24
     #86) * t15 - t8 * (t2486 - (-t1331 * t15 + t2484) * t15) * t15) * t
     #15 + ((t692 - t223) * t15 - (t223 - t1335) * t15) * t15) / 0.24E2 
     #+ t223 + t231 - t423 * ((t8 * ((t1804 * t75 - t2508) * t75 - t2513
     #) * t75 - t8 * (t2513 - (-t2242 * t75 + t2511) * t75) * t75) * t75
     # + ((t1808 - t231) * t75 - (t231 - t2246) * t75) * t75) / 0.24E2 +
     # t238 - t463 * ((t8 * ((t2359 * t88 - t801) * t88 - t804) * t88 - 
     #t810) * t88 + ((t2363 - t238) * t88 - t820) * t88) / 0.24E2)
        t2553 = t736 - t286
        t2555 = t2429 * t15
        t2558 = t2432 * t15
        t2560 = (t2555 - t2558) * t15
        t2564 = t338 - t1379
        t2590 = t1498 - t1518
        t2592 = t2437 * t75
        t2595 = t2440 * t75
        t2597 = (t2592 - t2595) * t75
        t2601 = t1861 - t1975
        t2643 = t508 * (-t388 * ((t8 * ((t15 * t2553 - t2555) * t15 - t2
     #560) * t15 - t8 * (t2560 - (-t15 * t2564 + t2558) * t15) * t15) * 
     #t15 + (((t15 * t2553 * t8 - t2431) * t15 - t2436) * t15 - (t2436 -
     # (-t15 * t2564 * t8 + t2434) * t15) * t15) * t15) / 0.24E2 + t2436
     # + t2444 - t423 * ((t8 * ((t2590 * t75 - t2592) * t75 - t2597) * t
     #75 - t8 * (t2597 - (-t2601 * t75 + t2595) * t75) * t75) * t75 + ((
     #(t2590 * t75 * t8 - t2439) * t75 - t2444) * t75 - (t2444 - (-t2601
     # * t75 * t8 + t2442) * t75) * t75) * t75) / 0.24E2 - t463 * ((t8 *
     # ((t2422 * t88 - t889) * t88 - t892) * t88 - t898) * t88 + ((t2426
     # - t912) * t88 - t914) * t88) / 0.24E2 + t912)
        t2654 = t58 * dz
        t2655 = t290 - t312
        t2657 = t8 * t2655 * t15
        t2658 = t312 - t342
        t2660 = t8 * t2658 * t15
        t2662 = (t2657 - t2660) * t15
        t2663 = t1522 - t312
        t2665 = t8 * t2663 * t75
        t2666 = t312 - t1865
        t2668 = t8 * t2666 * t75
        t2670 = (t2665 - t2668) * t75
        t2672 = cc * (t2662 + t2670 + t918)
        t2674 = (t319 - t2672) * t88
        t2676 = t2450 / 0.2E1 + t2674 / 0.2E1
        t2677 = t2654 * t2676
        t2680 = t2371 - t2460
        t2681 = t2456 * t2680
        t2684 = t2340 * t2374 / 0.24E2 - t2379 - t2404 + t2455 + t2465 +
     # t2470 + t374 * t2472 / 0.2E1 + t380 * t2477 / 0.6E1 + t386 * t255
     #0 / 0.2E1 + t507 * t2643 / 0.4E1 - t386 * t2468 / 0.4E1 + t644 * t
     #2402 / 0.12E2 - t507 * t2453 / 0.8E1 - t386 * t2463 / 0.4E1 - t507
     # * t2677 / 0.8E1 - t386 * t2681 / 0.24E2
        t2686 = t941 * t2472 / 0.2E1
        t2688 = t944 * t2477 / 0.6E1
        t2690 = t357 * t2550 / 0.2E1
        t2692 = t274 * t2643 / 0.4E1
        t2694 = t274 * t2677 / 0.8E1
        t2696 = t357 * t2681 / 0.24E2
        t2700 = dt * (t890 - dz * t896 / 0.24E2)
        t2702 = dz * t913
        t2705 = t51 * t2700
        t2707 = t7 * t2702 / 0.24E2
        t2708 = -t2686 - t2688 - t2690 - t2692 + t2694 + t2696 + t273 - 
     #t831 - t929 - t936 + t960 + t962 + t34 * t2700 - t48 * t2702 / 0.2
     #4E2 - t2705 + t2707
        t2710 = (t2684 + t2708) * t4
        t2714 = cc * t308
        t2715 = t2714 / 0.2E1
        t2716 = cc * t887
        t2718 = (-t2714 + t2716) * t88
        t2721 = (-t982 + t2714) * t88
        t2722 = t2721 / 0.2E1
        t2729 = (t2718 - t2721) * t88
        t2731 = (((cc * t2421 - t2716) * t88 - t2718) * t88 - t2729) * t
     #88
        t2732 = cc * t312
        t2734 = (t982 - t2732) * t88
        t2736 = (t2721 - t2734) * t88
        t2738 = (t2729 - t2736) * t88
        t2745 = dy * (t2718 / 0.2E1 + t2722 - t463 * (t2731 / 0.2E1 + t2
     #738 / 0.2E1) / 0.6E1) / 0.4E1
        t2746 = -t2710 * t6 + t2379 + t2404 - t2455 - t2465 - t2470 + t2
     #686 + t2688 + t2690 + t2692 + t2715 - t2745
        t2752 = t423 * (t2729 - dz * (t2731 - t2738) / 0.12E2) / 0.24E2
        t2753 = t2734 / 0.2E1
        t2754 = cc * t899
        t2756 = (-t2754 + t2732) * t88
        t2758 = (t2734 - t2756) * t88
        t2760 = (t2736 - t2758) * t88
        t2767 = dy * (t2722 + t2753 - t463 * (t2738 / 0.2E1 + t2760 / 0.
     #2E1) / 0.6E1) / 0.4E1
        t2769 = t819 * dz / 0.24E2
        t2773 = t2736 - dz * (t2738 - t2760) / 0.12E2
        t2775 = t463 * t2773 / 0.24E2
        t2779 = t8 * (t802 - dz * t808 / 0.24E2)
        t2780 = -t2694 - t2696 - t273 + t2752 - t960 - t962 - t2767 - t2
     #769 + t2705 - t2707 - t1010 - t2775 + t2779
        t2787 = dt * (t893 - dz * t904 / 0.24E2)
        t2789 = dz * t919
        t2792 = t51 * t2787
        t2794 = t7 * t2789 / 0.24E2
        t2795 = t1539 * t2462
        t2798 = t1482 * t2676
        t2802 = t242 * t15
        t2805 = t245 * t15
        t2807 = (t2802 - t2805) * t15
        t2829 = t250 * t75
        t2832 = t253 * t75
        t2834 = (t2829 - t2832) * t75
        t2855 = k - 3
        t2857 = t259 - u(i,j,t2855,n)
        t2869 = (-t2857 * t8 * t88 + t262) * t88
        t2878 = t387 * (-t388 * ((t8 * ((t15 * t704 - t2802) * t15 - t28
     #07) * t15 - t8 * (t2807 - (-t1347 * t15 + t2805) * t15) * t15) * t
     #15 + ((t708 - t249) * t15 - (t249 - t1351) * t15) * t15) / 0.24E2 
     #+ t249 + t257 - t423 * ((t8 * ((t1820 * t75 - t2829) * t75 - t2834
     #) * t75 - t8 * (t2834 - (-t2258 * t75 + t2832) * t75) * t75) * t75
     # + ((t1824 - t257) * t75 - (t257 - t2262) * t75) * t75) / 0.24E2 -
     # t463 * ((t816 - t8 * (t813 - (-t2857 * t88 + t811) * t88) * t88) 
     #* t88 + (t822 - (t264 - t2869) * t88) * t88) / 0.24E2 + t264)
        t2881 = t740 - t290
        t2883 = t2655 * t15
        t2886 = t2658 * t15
        t2888 = (t2883 - t2886) * t15
        t2892 = t342 - t1383
        t2918 = t1502 - t1522
        t2920 = t2663 * t75
        t2923 = t2666 * t75
        t2925 = (t2920 - t2923) * t75
        t2929 = t1865 - t1979
        t2955 = ut(i,j,t2855,n)
        t2956 = t899 - t2955
        t2968 = (-t2956 * t8 * t88 + t916) * t88
        t2977 = t508 * (-t388 * ((t8 * ((t15 * t2881 - t2883) * t15 - t2
     #888) * t15 - t8 * (t2888 - (-t15 * t2892 + t2886) * t15) * t15) * 
     #t15 + (((t15 * t2881 * t8 - t2657) * t15 - t2662) * t15 - (t2662 -
     # (-t15 * t2892 * t8 + t2660) * t15) * t15) * t15) / 0.24E2 + t2662
     # + t2670 - t423 * ((t8 * ((t2918 * t75 - t2920) * t75 - t2925) * t
     #75 - t8 * (t2925 - (-t2929 * t75 + t2923) * t75) * t75) * t75 + ((
     #(t2918 * t75 * t8 - t2665) * t75 - t2670) * t75 - (t2670 - (-t2929
     # * t75 * t8 + t2668) * t75) * t75) * t75) / 0.24E2 - t463 * ((t906
     # - t8 * (t903 - (-t2956 * t88 + t901) * t88) * t88) * t88 + (t920 
     #- (t918 - t2968) * t88) * t88) / 0.24E2 + t918)
        t2987 = (t8 * (t476 - t259) * t15 - t8 * (t259 - t1125) * t15) *
     # t15
        t2995 = (t8 * (t1652 - t259) * t75 - t8 * (t259 - t2089) * t75) 
     #* t75
        t2999 = (t2458 - cc * (t2987 + t2995 + t2869)) * t88
        t3002 = t2456 * (t2460 / 0.2E1 + t2999 / 0.2E1)
        t3027 = t60 * ((t8 * (t708 + t716 + t495 - t249 - t257 - t264) *
     # t15 - t8 * (t249 + t257 + t264 - t1351 - t1359 - t1144) * t15) * 
     #t15 + (t8 * (t1819 + t1824 + t1671 - t249 - t257 - t264) * t75 - t
     #8 * (t249 + t257 + t264 - t2257 - t2262 - t2108) * t75) * t75 + (t
     #267 - t8 * (t249 + t257 + t264 - t2987 - t2995 - t2869) * t88) * t
     #88)
        t3031 = t58 * t265 * t88
        t3036 = t59 * (t28 + t307 + t317 - t2662 - t2670 - t918) * t88
        t3039 = t34 * t2787 - t48 * t2789 / 0.24E2 - t2792 + t2794 - t27
     #3 + t831 + t929 + t936 - t386 * t2795 / 0.4E1 - t507 * t2798 / 0.8
     #E1 - t386 * t2878 / 0.2E1 - t507 * t2977 / 0.4E1 - t386 * t3002 / 
     #0.4E1 - t644 * t3027 / 0.12E2 + t374 * t3031 / 0.2E1 + t380 * t303
     #6 / 0.6E1
        t3062 = t2654 * (t2674 / 0.2E1 + (t2672 - cc * ((t8 * (t584 - t8
     #99) * t15 - t8 * (t899 - t1228) * t15) * t15 + (t8 * (t1747 - t899
     #) * t75 - t8 * (t899 - t2184) * t75) * t75 + t2968)) * t88 / 0.2E1
     #)
        t3066 = t2456 * (t2460 - t2999)
        t3070 = t941 * t3031 / 0.2E1
        t3072 = t423 * t2680 * t88
        t3076 = t2377 * t3072 / 0.24E2
        t3078 = t944 * t3036 / 0.6E1
        t3080 = t357 * t2795 / 0.4E1
        t3082 = t274 * t2798 / 0.8E1
        t3084 = t357 * t2878 / 0.2E1
        t3086 = t274 * t2977 / 0.4E1
        t3088 = t357 * t3002 / 0.4E1
        t3090 = t57 * t3027 / 0.12E2
        t3092 = t274 * t3062 / 0.8E1
        t3094 = t357 * t3066 / 0.24E2
        t3095 = -t507 * t3062 / 0.8E1 - t386 * t3066 / 0.24E2 - t3070 - 
     #t960 - t962 + t2340 * t3072 / 0.24E2 - t3076 - t3078 + t3080 + t30
     #82 + t3084 + t3086 + t3088 + t3090 + t3092 + t3094
        t3097 = (t3039 + t3095) * t4
        t3100 = t2732 / 0.2E1
        t3102 = dz * t821 / 0.24E2
        t3104 = t423 * t2773 / 0.24E2
        t3112 = (t2758 - (t2756 - (-cc * t2955 + t2754) * t88) * t88) * 
     #t88
        t3119 = dy * (t2753 + t2756 / 0.2E1 - t463 * (t2760 / 0.2E1 + t3
     #112 / 0.2E1) / 0.6E1) / 0.4E1
        t3123 = t8 * (t805 - dz * t814 / 0.24E2)
        t3125 = -t3097 * t6 + t273 + t2792 - t2794 + t3070 - t3100 - t31
     #02 + t3104 - t3119 + t3123 + t960 + t962
        t3131 = t463 * (t2758 - dz * (t2760 - t3112) / 0.12E2) / 0.24E2
        t3132 = -t2767 + t3076 + t3078 - t3080 - t3082 - t3084 - t3086 -
     # t3088 - t3090 - t3092 - t3094 + t1010 - t3131
        t3141 = t975 + t52 + t943 - t971 + t946 - t32 + t1011 + t948 - t
     #1008 + t950 - t952 + t1038
        t3142 = t954 - t956 + t958 - t1010 - t960 - t1032 - t962 - t964 
     #- t1024 - t273 - t356 - t372
        t3148 = t1461 + t1427 + t1403 - t1457 + t1405 - t1429 + t1010 + 
     #t960 - t1032 + t962 - t964 + t1024
        t3149 = t273 - t356 + t372 - t1464 - t1407 - t1449 - t1409 - t14
     #11 - t1455 - t1413 - t1415 - t1417
        t3157 = t1927 + t1479 + t1592 - t1923 + t1597 - t1481 + t1949 + 
     #t1682 - t1921 + t1777 - t1782 + t1948
        t3158 = t1833 - t1538 + t1575 - t1010 - t960 - t1942 - t962 - t1
     #584 - t1955 - t273 - t1881 - t1883
        t3164 = t2312 + t2292 + t2029 - t2308 + t2034 - t2295 + t1010 + 
     #t960 - t1942 + t962 - t1584 + t1955
        t3165 = t273 - t1881 + t1883 - t2306 - t2119 - t2333 - t2214 - t
     #2220 - t2325 - t2271 - t2273 - t2275
        t3173 = t2779 + t2705 + t2686 - t2769 + t2688 - t2707 + t2715 + 
     #t2690 - t2745 + t2692 - t2470 + t2752
        t3174 = t2404 - t2455 + t2379 - t1010 - t960 - t2767 - t962 - t2
     #465 - t2775 - t273 - t2694 - t2696
        t3180 = t3123 + t2792 + t3070 - t3102 + t3078 - t2794 + t1010 + 
     #t960 - t2767 + t962 - t3080 + t3104
        t3181 = t273 - t3082 + t3076 - t3100 - t3084 - t3119 - t3086 - t
     #3088 - t3131 - t3090 - t3092 - t3094


        unew(i,j,k) = t1 + dt * t2 + (t967 * t58 / 0.6E1 + (t1009 + t
     #1039) * t58 / 0.2E1 - t1432 * t58 / 0.6E1 - (t1463 + t1465) * t58 
     #/ 0.2E1) * t15 + (t1886 * t58 / 0.6E1 + (t1890 + t1956) * t58 / 0.
     #2E1 - t2301 * t58 / 0.6E1 - (t2305 + t2334) * t58 / 0.2E1) * t75 +
     # (t2710 * t58 / 0.6E1 + (t2746 + t2780) * t58 / 0.2E1 - t3097 * t5
     #8 / 0.6E1 - (t3125 + t3132) * t58 / 0.2E1) * t88

        utnew(i,j,k) = t2 + (t967 *
     # dt / 0.2E1 + (t3141 + t3142) * dt - t967 * t7 - t1432 * dt / 0.2E
     #1 - (t3148 + t3149) * dt + t1432 * t7) * t15 + (t1886 * dt / 0.2E1
     # + (t3157 + t3158) * dt - t1886 * t7 - t2301 * dt / 0.2E1 - (t3164
     # + t3165) * dt + t2301 * t7) * t75 + (t2710 * dt / 0.2E1 + (t3173 
     #+ t3174) * dt - t2710 * t7 - t3097 * dt / 0.2E1 - (t3180 + t3181) 
     #* dt + t3097 * t7) * t88

        return
      end
