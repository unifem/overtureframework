      subroutine duStepWaveGen3d4rcOLD( 
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
        real t1002
        real t1005
        real t1008
        real t1009
        real t1011
        real t1014
        real t1016
        real t102
        real t1020
        real t1021
        real t103
        real t1033
        real t1039
        real t104
        integer t1047
        real t1049
        real t106
        real t1061
        real t1069
        real t1070
        real t1072
        real t1075
        real t1077
        real t108
        real t1081
        real t1082
        real t1094
        real t11
        real t110
        real t1100
        real t1109
        real t111
        real t1113
        real t1117
        real t1118
        real t112
        real t1125
        real t1126
        real t1127
        real t1129
        real t1132
        real t1134
        real t1138
        real t1139
        real t114
        real t115
        real t116
        real t1165
        real t1166
        real t1168
        real t1171
        real t1173
        real t1177
        real t1178
        real t118
        real t12
        real t120
        real t121
        real t1211
        real t122
        real t1220
        real t1223
        real t1227
        real t1233
        real t1237
        real t124
        real t1243
        real t1244
        real t1247
        real t125
        real t1255
        real t126
        real t1260
        real t1264
        real t1265
        real t1269
        real t1274
        real t1278
        real t128
        real t1282
        real t1283
        real t1287
        real t1292
        real t1298
        real t13
        real t130
        real t1302
        real t1310
        real t1314
        real t1318
        real t132
        real t1326
        real t1333
        real t1336
        real t134
        real t1340
        real t1346
        real t135
        real t1350
        real t136
        real t1360
        real t1364
        real t1367
        real t1371
        real t1373
        real t1375
        real t1377
        real t138
        real t1389
        real t139
        real t1392
        real t1394
        integer t14
        real t140
        real t1400
        real t1402
        real t1412
        real t1415
        real t1418
        real t142
        real t1420
        real t1421
        real t1423
        real t1425
        real t1426
        real t1427
        real t1429
        real t1430
        real t1431
        real t1433
        real t1435
        real t1436
        real t1438
        real t144
        real t1441
        integer t1444
        real t1446
        real t145
        real t1458
        real t146
        real t1466
        real t1467
        real t1469
        real t1472
        real t1474
        real t1478
        real t1479
        real t148
        real t149
        real t1491
        real t1497
        real t15
        real t150
        real t1506
        real t1509
        real t1511
        real t152
        real t1533
        real t1536
        real t1538
        real t154
        real t1542
        real t1543
        real t1544
        real t155
        real t1551
        real t1552
        real t1554
        real t1557
        real t1559
        real t1563
        real t157
        real t1596
        real t16
        real t160
        real t1604
        real t1605
        real t1607
        real t161
        real t1610
        real t1612
        real t1616
        real t1617
        real t162
        real t1644
        real t1647
        real t165
        real t1656
        real t166
        real t1660
        real t1666
        real t1667
        real t1668
        integer t167
        real t1671
        real t1679
        real t169
        real t1699
        real t17
        real t1700
        real t1704
        real t171
        real t1715
        real t1716
        real t1720
        real t1727
        real t1730
        real t1739
        real t174
        real t1743
        real t1751
        real t1754
        real t1758
        real t176
        real t1761
        real t1762
        real t1769
        real t1770
        real t1773
        real t1775
        real t1781
        real t1782
        real t1784
        real t1785
        real t1787
        real t1789
        real t1790
        real t1791
        real t1793
        real t1794
        real t1795
        real t1797
        real t1799
        real t180
        real t1800
        real t1801
        real t1804
        real t1806
        real t1808
        real t1810
        real t1811
        real t1815
        real t1817
        real t1819
        real t182
        real t1821
        real t183
        real t1834
        real t1836
        real t1838
        real t1839
        real t1842
        real t1844
        real t185
        real t1850
        real t1852
        real t1860
        real t1863
        real t1867
        real t1870
        integer t1873
        real t1875
        real t1887
        real t1896
        real t1899
        real t19
        real t1901
        real t191
        real t1922
        real t1923
        real t1925
        real t1928
        real t1930
        real t1934
        real t1935
        real t194
        real t1947
        real t195
        real t1953
        real t1962
        real t1966
        real t1970
        real t1971
        real t1978
        real t1979
        real t1981
        real t1984
        real t1986
        real t1990
        real t2
        integer t20
        real t201
        real t2016
        real t2017
        real t2019
        integer t202
        real t2022
        real t2024
        real t2028
        real t2029
        real t203
        real t204
        real t206
        real t2062
        real t2071
        real t2082
        real t2086
        real t209
        real t2092
        real t2093
        real t2096
        real t21
        real t2104
        real t211
        real t2124
        real t2125
        real t2129
        real t2140
        real t2141
        real t2145
        integer t215
        real t2152
        real t216
        real t2163
        real t2167
        real t217
        real t2177
        real t2181
        real t2184
        real t2188
        real t2190
        real t2192
        real t2194
        real t22
        real t2206
        real t2209
        real t2211
        real t2217
        real t2219
        real t2229
        real t2232
        real t2235
        real t2237
        real t2238
        real t2240
        real t2242
        real t2243
        real t2245
        real t2246
        real t2248
        real t2250
        real t2251
        real t2253
        real t2256
        real t2260
        real t2263
        real t2265
        real t2287
        real t229
        real t2290
        real t2292
        real t23
        integer t2313
        real t2315
        real t2327
        real t2336
        real t2339
        real t2341
        real t2345
        real t2346
        real t2347
        real t235
        real t2354
        real t2355
        real t2357
        real t2360
        real t2362
        real t2366
        real t2392
        real t2394
        real t2397
        real t2399
        real t2403
        real t243
        real t2436
        integer t244
        real t2445
        real t2448
        real t245
        real t246
        real t2465
        real t2466
        real t2467
        real t2470
        real t2478
        real t248
        real t25
        real t2500
        real t2503
        real t251
        real t2522
        real t2525
        real t2529
        real t253
        real t2532
        real t2533
        real t2540
        real t2541
        real t2544
        real t2546
        real t2552
        real t2553
        real t2555
        real t2556
        real t2558
        real t2560
        real t2561
        real t2563
        real t2564
        real t2566
        real t2568
        real t2569
        integer t257
        real t2570
        real t2573
        real t2575
        real t2577
        real t2579
        real t258
        real t2580
        real t2584
        real t2586
        real t2588
        real t259
        real t2590
        real t26
        real t2603
        real t2605
        real t2607
        real t2608
        real t2611
        real t2613
        real t2619
        real t2621
        real t2629
        real t2632
        real t2636
        real t2639
        real t2643
        real t2646
        real t2648
        integer t2669
        real t2671
        real t2683
        real t2692
        real t2695
        real t2697
        real t271
        real t2719
        real t2723
        real t2727
        real t2728
        real t2735
        real t2736
        real t2738
        real t2741
        real t2743
        real t2747
        real t277
        real t2780
        real t2788
        real t2790
        real t2793
        real t2795
        real t2799
        real t2826
        real t2845
        real t2846
        real t2849
        real t2857
        real t286
        real t2879
        real t289
        real t2902
        real t2906
        real t2909
        real t291
        real t2913
        real t2915
        real t2917
        real t2919
        real t2931
        real t2934
        real t2936
        real t2942
        real t2944
        real t295
        real t296
        real t297
        real t30
        real t304
        real t305
        real t306
        real t307
        real t308
        real t310
        real t313
        real t315
        real t319
        real t32
        real t320
        real t33
        real t34
        real t346
        real t347
        real t349
        real t35
        real t352
        real t354
        real t358
        real t359
        real t36
        real t37
        real t388
        real t39
        real t394
        real t397
        real t4
        real t40
        real t404
        real t407
        real t408
        real t412
        real t418
        real t42
        real t422
        real t428
        real t429
        real t430
        real t433
        real t44
        real t441
        real t442
        real t443
        real t447
        integer t45
        real t450
        real t453
        real t455
        real t457
        real t458
        real t46
        real t462
        real t467
        real t47
        real t471
        real t474
        real t476
        real t478
        real t479
        real t483
        real t488
        real t49
        real t494
        real t497
        real t499
        real t5
        real t50
        real t501
        real t509
        integer t51
        real t513
        real t516
        real t518
        real t52
        real t520
        real t528
        real t53
        real t535
        real t538
        real t539
        real t543
        real t549
        real t55
        real t553
        real t561
        real t564
        real t568
        real t57
        integer t571
        real t572
        real t573
        real t574
        real t576
        real t577
        real t579
        integer t58
        real t583
        real t585
        real t586
        real t587
        real t59
        real t593
        real t594
        real t595
        real t596
        real t598
        real t599
        real t6
        real t60
        real t601
        real t602
        real t604
        real t605
        real t606
        real t607
        real t609
        real t610
        real t612
        real t616
        real t618
        real t619
        real t62
        real t620
        real t622
        real t624
        real t625
        real t626
        real t63
        real t632
        real t633
        real t634
        real t635
        real t637
        real t638
        integer t64
        real t640
        real t641
        real t643
        real t644
        real t645
        real t646
        real t648
        real t649
        real t65
        real t651
        real t655
        real t657
        real t658
        real t659
        real t66
        real t661
        real t663
        real t664
        real t665
        real t672
        real t673
        real t674
        real t675
        real t676
        real t677
        real t679
        real t68
        real t680
        real t681
        real t688
        real t689
        real t690
        real t691
        real t692
        real t694
        real t695
        real t697
        real t698
        real t7
        real t70
        real t700
        real t701
        real t702
        real t703
        real t705
        real t706
        real t708
        real t71
        real t712
        real t714
        real t715
        real t716
        real t718
        real t72
        real t720
        real t721
        real t722
        real t729
        real t733
        real t735
        real t736
        real t737
        real t74
        real t743
        real t744
        real t745
        real t746
        real t748
        real t749
        real t751
        real t752
        real t754
        real t755
        real t756
        real t757
        real t759
        real t76
        real t760
        real t762
        real t766
        real t768
        real t769
        real t77
        real t770
        real t772
        real t774
        real t775
        real t776
        real t78
        real t783
        real t785
        real t786
        real t787
        real t789
        real t790
        real t791
        real t793
        real t795
        real t796
        real t797
        real t799
        real t8
        real t80
        real t800
        real t801
        real t803
        real t805
        real t806
        real t807
        real t81
        real t810
        real t812
        real t818
        real t82
        real t820
        real t823
        real t825
        real t827
        real t828
        real t829
        real t831
        real t832
        real t833
        real t835
        real t837
        real t838
        real t84
        real t840
        real t841
        real t843
        real t845
        real t846
        real t847
        real t849
        real t850
        real t851
        real t853
        real t855
        real t856
        real t858
        real t86
        real t861
        real t863
        real t865
        real t866
        real t868
        real t869
        real t87
        real t871
        real t873
        real t874
        real t876
        real t877
        real t879
        real t88
        real t881
        real t882
        real t884
        real t885
        real t887
        real t889
        real t890
        real t892
        real t896
        real t898
        real t899
        integer t9
        real t90
        real t900
        real t902
        real t903
        real t904
        real t906
        real t908
        real t909
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
        real t923
        real t925
        real t927
        real t929
        real t930
        real t932
        real t935
        real t936
        real t937
        real t938
        real t939
        real t94
        real t941
        real t942
        real t943
        real t945
        real t946
        real t948
        real t949
        real t951
        real t956
        real t96
        real t963
        real t965
        real t967
        real t969
        real t97
        real t971
        real t973
        real t974
        real t977
        real t979
        real t985
        real t987
        real t99
        real t995
        real t998
        t1 = u(i,j,k,n)
        t2 = ut(i,j,k,n)
        t4 = cc ** 2
        t5 = sqrt(0.3E1)
        t6 = t5 / 0.6E1
        t7 = 0.1E1 / 0.2E1 + t6
        t8 = t4 * t7
        t9 = i + 1
        t10 = ut(t9,j,k,n)
        t11 = t10 - t2
        t12 = 0.1E1 / dx
        t13 = t11 * t12
        t14 = i + 2
        t15 = ut(t14,j,k,n)
        t16 = t15 - t10
        t17 = t16 * t12
        t19 = (t17 - t13) * t12
        t20 = i - 1
        t21 = ut(t20,j,k,n)
        t22 = t2 - t21
        t23 = t22 * t12
        t25 = (t13 - t23) * t12
        t26 = t19 - t25
        t30 = dt * (t13 - dx * t26 / 0.24E2)
        t32 = t7 ** 2
        t33 = t4 * t32
        t34 = dt ** 2
        t35 = u(t14,j,k,n)
        t36 = u(t9,j,k,n)
        t37 = t35 - t36
        t39 = t4 * t37 * t12
        t40 = t36 - t1
        t42 = t4 * t40 * t12
        t44 = (t39 - t42) * t12
        t45 = j + 1
        t46 = u(t9,t45,k,n)
        t47 = t46 - t36
        t49 = 0.1E1 / dy
        t50 = t4 * t47 * t49
        t51 = j - 1
        t52 = u(t9,t51,k,n)
        t53 = t36 - t52
        t55 = t4 * t53 * t49
        t57 = (t50 - t55) * t49
        t58 = k + 1
        t59 = u(t9,j,t58,n)
        t60 = t59 - t36
        t62 = 0.1E1 / dz
        t63 = t4 * t60 * t62
        t64 = k - 1
        t65 = u(t9,j,t64,n)
        t66 = t36 - t65
        t68 = t4 * t66 * t62
        t70 = (t63 - t68) * t62
        t71 = u(t20,j,k,n)
        t72 = t1 - t71
        t74 = t4 * t72 * t12
        t76 = (t42 - t74) * t12
        t77 = u(i,t45,k,n)
        t78 = t77 - t1
        t80 = t4 * t78 * t49
        t81 = u(i,t51,k,n)
        t82 = t1 - t81
        t84 = t4 * t82 * t49
        t86 = (t80 - t84) * t49
        t87 = u(i,j,t58,n)
        t88 = t87 - t1
        t90 = t4 * t88 * t62
        t91 = u(i,j,t64,n)
        t92 = t1 - t91
        t94 = t4 * t92 * t62
        t96 = (t90 - t94) * t62
        t97 = t44 + t57 + t70 - t76 - t86 - t96
        t99 = t34 * t97 * t12
        t102 = t32 * t7
        t103 = t4 * t102
        t104 = t34 * dt
        t106 = t4 * t16 * t12
        t108 = t4 * t11 * t12
        t110 = (t106 - t108) * t12
        t111 = ut(t9,t45,k,n)
        t112 = t111 - t10
        t114 = t4 * t112 * t49
        t115 = ut(t9,t51,k,n)
        t116 = t10 - t115
        t118 = t4 * t116 * t49
        t120 = (t114 - t118) * t49
        t121 = ut(t9,j,t58,n)
        t122 = t121 - t10
        t124 = t4 * t122 * t62
        t125 = ut(t9,j,t64,n)
        t126 = t10 - t125
        t128 = t4 * t126 * t62
        t130 = (t124 - t128) * t62
        t132 = t4 * t22 * t12
        t134 = (t108 - t132) * t12
        t135 = ut(i,t45,k,n)
        t136 = t135 - t2
        t138 = t4 * t136 * t49
        t139 = ut(i,t51,k,n)
        t140 = t2 - t139
        t142 = t4 * t140 * t49
        t144 = (t138 - t142) * t49
        t145 = ut(i,j,t58,n)
        t146 = t145 - t2
        t148 = t4 * t146 * t62
        t149 = ut(i,j,t64,n)
        t150 = t2 - t149
        t152 = t4 * t150 * t62
        t154 = (t148 - t152) * t62
        t155 = t110 + t120 + t130 - t134 - t144 - t154
        t157 = t104 * t155 * t12
        t160 = t7 * dt
        t161 = t110 - t134
        t162 = dx * t161
        t165 = beta * t7
        t166 = dx ** 2
        t167 = i + 3
        t169 = u(t167,j,k,n) - t35
        t171 = t37 * t12
        t174 = t40 * t12
        t176 = (t171 - t174) * t12
        t180 = t72 * t12
        t182 = (t174 - t180) * t12
        t183 = t176 - t182
        t185 = t4 * t183 * t12
        t191 = (t4 * t169 * t12 - t39) * t12
        t194 = t44 - t76
        t195 = t194 * t12
        t201 = dy ** 2
        t202 = j + 2
        t203 = u(t9,t202,k,n)
        t204 = t203 - t46
        t206 = t47 * t49
        t209 = t53 * t49
        t211 = (t206 - t209) * t49
        t215 = j - 2
        t216 = u(t9,t215,k,n)
        t217 = t52 - t216
        t229 = (t4 * t204 * t49 - t50) * t49
        t235 = (t55 - t4 * t217 * t49) * t49
        t243 = dz ** 2
        t244 = k + 2
        t245 = u(t9,j,t244,n)
        t246 = t245 - t59
        t248 = t60 * t62
        t251 = t66 * t62
        t253 = (t248 - t251) * t62
        t257 = k - 2
        t258 = u(t9,j,t257,n)
        t259 = t65 - t258
        t271 = (t4 * t246 * t62 - t63) * t62
        t277 = (t68 - t4 * t259 * t62) * t62
        t286 = dt * (-t166 * ((t4 * ((t169 * t12 - t171) * t12 - t176) *
     # t12 - t185) * t12 + ((t191 - t44) * t12 - t195) * t12) / 0.24E2 +
     # t70 + t57 - t201 * ((t4 * ((t204 * t49 - t206) * t49 - t211) * t4
     #9 - t4 * (t211 - (t209 - t217 * t49) * t49) * t49) * t49 + ((t229 
     #- t57) * t49 - (t57 - t235) * t49) * t49) / 0.24E2 - t243 * ((t4 *
     # ((t246 * t62 - t248) * t62 - t253) * t62 - t4 * (t253 - (t251 - t
     #259 * t62) * t62) * t62) * t62 + ((t271 - t70) * t62 - (t70 - t277
     #) * t62) * t62) / 0.24E2 + t44)
        t289 = t13 / 0.2E1
        t291 = ut(t167,j,k,n) - t15
        t295 = (t291 * t12 - t17) * t12 - t19
        t296 = t295 * t12
        t297 = t26 * t12
        t304 = dx * (t17 / 0.2E1 + t289 - t166 * (t296 / 0.2E1 + t297 / 
     #0.2E1) / 0.6E1) / 0.2E1
        t305 = beta ** 2
        t306 = t305 * t32
        t307 = ut(t9,j,t244,n)
        t308 = t307 - t121
        t310 = t122 * t62
        t313 = t126 * t62
        t315 = (t310 - t313) * t62
        t319 = ut(t9,j,t257,n)
        t320 = t125 - t319
        t346 = ut(t9,t202,k,n)
        t347 = t346 - t111
        t349 = t112 * t49
        t352 = t116 * t49
        t354 = (t349 - t352) * t49
        t358 = ut(t9,t215,k,n)
        t359 = t115 - t358
        t388 = t4 * t26 * t12
        t394 = (t4 * t291 * t12 - t106) * t12
        t397 = t161 * t12
        t404 = t34 * (t130 - t243 * ((t4 * ((t308 * t62 - t310) * t62 - 
     #t315) * t62 - t4 * (t315 - (t313 - t320 * t62) * t62) * t62) * t62
     # + (((t4 * t308 * t62 - t124) * t62 - t130) * t62 - (t130 - (t128 
     #- t4 * t320 * t62) * t62) * t62) * t62) / 0.24E2 - t201 * ((t4 * (
     #(t347 * t49 - t349) * t49 - t354) * t49 - t4 * (t354 - (t352 - t35
     #9 * t49) * t49) * t49) * t49 + (((t4 * t347 * t49 - t114) * t49 - 
     #t120) * t49 - (t120 - (t118 - t4 * t359 * t49) * t49) * t49) * t49
     #) / 0.24E2 - t166 * ((t4 * t295 * t12 - t388) * t12 + ((t394 - t11
     #0) * t12 - t397) * t12) / 0.24E2 + t120 + t110)
        t407 = dt * dx
        t408 = u(t14,t45,k,n)
        t412 = u(t14,t51,k,n)
        t418 = u(t14,j,t58,n)
        t422 = u(t14,j,t64,n)
        t428 = t191 + (t4 * (t408 - t35) * t49 - t4 * (t35 - t412) * t49
     #) * t49 + (t4 * (t418 - t35) * t62 - t4 * (t35 - t422) * t62) * t6
     #2 - t44 - t57 - t70
        t429 = t428 * t12
        t430 = t97 * t12
        t433 = t407 * (t429 / 0.2E1 + t430 / 0.2E1)
        t441 = t166 * (t19 - dx * (t296 - t297) / 0.12E2) / 0.12E2
        t442 = t305 * beta
        t443 = t442 * t102
        t447 = t4 * t97 * t12
        t450 = t408 - t46
        t453 = t46 - t77
        t455 = t4 * t453 * t12
        t457 = (t4 * t450 * t12 - t455) * t12
        t458 = u(t9,t45,t58,n)
        t462 = u(t9,t45,t64,n)
        t467 = (t4 * (t458 - t46) * t62 - t4 * (t46 - t462) * t62) * t62
        t471 = t412 - t52
        t474 = t52 - t81
        t476 = t4 * t474 * t12
        t478 = (t4 * t471 * t12 - t476) * t12
        t479 = u(t9,t51,t58,n)
        t483 = u(t9,t51,t64,n)
        t488 = (t4 * (t479 - t52) * t62 - t4 * (t52 - t483) * t62) * t62
        t494 = t418 - t59
        t497 = t59 - t87
        t499 = t4 * t497 * t12
        t501 = (t4 * t494 * t12 - t499) * t12
        t509 = (t4 * (t458 - t59) * t49 - t4 * (t59 - t479) * t49) * t49
        t513 = t422 - t65
        t516 = t65 - t91
        t518 = t4 * t516 * t12
        t520 = (t4 * t513 * t12 - t518) * t12
        t528 = (t4 * (t462 - t65) * t49 - t4 * (t65 - t483) * t49) * t49
        t535 = t104 * ((t4 * t428 * t12 - t447) * t12 + (t4 * (t457 + t2
     #29 + t467 - t44 - t57 - t70) * t49 - t4 * (t44 + t57 + t70 - t478 
     #- t235 - t488) * t49) * t49 + (t4 * (t501 + t509 + t271 - t44 - t5
     #7 - t70) * t62 - t4 * (t44 + t57 + t70 - t520 - t528 - t277) * t62
     #) * t62)
        t538 = t34 * dx
        t539 = ut(t14,t45,k,n)
        t543 = ut(t14,t51,k,n)
        t549 = ut(t14,j,t58,n)
        t553 = ut(t14,j,t64,n)
        t561 = t155 * t12
        t564 = t538 * ((t394 + (t4 * (t539 - t15) * t49 - t4 * (t15 - t5
     #43) * t49) * t49 + (t4 * (t549 - t15) * t62 - t4 * (t15 - t553) * 
     #t62) * t62 - t110 - t120 - t130) * t12 / 0.2E1 + t561 / 0.2E1)
        t568 = t407 * (t429 - t430)
        t571 = i - 2
        t572 = u(t571,j,k,n)
        t573 = t71 - t572
        t574 = t573 * t12
        t576 = (t180 - t574) * t12
        t577 = t182 - t576
        t579 = t4 * t577 * t12
        t583 = t4 * t573 * t12
        t585 = (t74 - t583) * t12
        t586 = t76 - t585
        t587 = t586 * t12
        t593 = u(i,t202,k,n)
        t594 = t593 - t77
        t595 = t594 * t49
        t596 = t78 * t49
        t598 = (t595 - t596) * t49
        t599 = t82 * t49
        t601 = (t596 - t599) * t49
        t602 = t598 - t601
        t604 = t4 * t602 * t49
        t605 = u(i,t215,k,n)
        t606 = t81 - t605
        t607 = t606 * t49
        t609 = (t599 - t607) * t49
        t610 = t601 - t609
        t612 = t4 * t610 * t49
        t616 = t4 * t594 * t49
        t618 = (t616 - t80) * t49
        t619 = t618 - t86
        t620 = t619 * t49
        t622 = t4 * t606 * t49
        t624 = (t84 - t622) * t49
        t625 = t86 - t624
        t626 = t625 * t49
        t632 = u(i,j,t244,n)
        t633 = t632 - t87
        t634 = t633 * t62
        t635 = t88 * t62
        t637 = (t634 - t635) * t62
        t638 = t92 * t62
        t640 = (t635 - t638) * t62
        t641 = t637 - t640
        t643 = t4 * t641 * t62
        t644 = u(i,j,t257,n)
        t645 = t91 - t644
        t646 = t645 * t62
        t648 = (t638 - t646) * t62
        t649 = t640 - t648
        t651 = t4 * t649 * t62
        t655 = t4 * t633 * t62
        t657 = (t655 - t90) * t62
        t658 = t657 - t96
        t659 = t658 * t62
        t661 = t4 * t645 * t62
        t663 = (t94 - t661) * t62
        t664 = t96 - t663
        t665 = t664 * t62
        t672 = dt * (t86 + t76 - t166 * ((t185 - t579) * t12 + (t195 - t
     #587) * t12) / 0.24E2 - t201 * ((t604 - t612) * t49 + (t620 - t626)
     # * t49) / 0.24E2 + t96 - t243 * ((t643 - t651) * t62 + (t659 - t66
     #5) * t62) / 0.24E2)
        t673 = t165 * t672
        t674 = t23 / 0.2E1
        t675 = ut(t571,j,k,n)
        t676 = t21 - t675
        t677 = t676 * t12
        t679 = (t23 - t677) * t12
        t680 = t25 - t679
        t681 = t680 * t12
        t688 = dx * (t289 + t674 - t166 * (t297 / 0.2E1 + t681 / 0.2E1) 
     #/ 0.6E1) / 0.2E1
        t689 = ut(i,t202,k,n)
        t690 = t689 - t135
        t691 = t690 * t49
        t692 = t136 * t49
        t694 = (t691 - t692) * t49
        t695 = t140 * t49
        t697 = (t692 - t695) * t49
        t698 = t694 - t697
        t700 = t4 * t698 * t49
        t701 = ut(i,t215,k,n)
        t702 = t139 - t701
        t703 = t702 * t49
        t705 = (t695 - t703) * t49
        t706 = t697 - t705
        t708 = t4 * t706 * t49
        t712 = t4 * t690 * t49
        t714 = (t712 - t138) * t49
        t715 = t714 - t144
        t716 = t715 * t49
        t718 = t4 * t702 * t49
        t720 = (t142 - t718) * t49
        t721 = t144 - t720
        t722 = t721 * t49
        t729 = t4 * t680 * t12
        t733 = t4 * t676 * t12
        t735 = (t132 - t733) * t12
        t736 = t134 - t735
        t737 = t736 * t12
        t743 = ut(i,j,t244,n)
        t744 = t743 - t145
        t745 = t744 * t62
        t746 = t146 * t62
        t748 = (t745 - t746) * t62
        t749 = t150 * t62
        t751 = (t746 - t749) * t62
        t752 = t748 - t751
        t754 = t4 * t752 * t62
        t755 = ut(i,j,t257,n)
        t756 = t149 - t755
        t757 = t756 * t62
        t759 = (t749 - t757) * t62
        t760 = t751 - t759
        t762 = t4 * t760 * t62
        t766 = t4 * t744 * t62
        t768 = (t766 - t148) * t62
        t769 = t768 - t154
        t770 = t769 * t62
        t772 = t4 * t756 * t62
        t774 = (t152 - t772) * t62
        t775 = t154 - t774
        t776 = t775 * t62
        t783 = t34 * (-t201 * ((t700 - t708) * t49 + (t716 - t722) * t49
     #) / 0.24E2 + t154 - t166 * ((t388 - t729) * t12 + (t397 - t737) * 
     #t12) / 0.24E2 + t134 + t144 - t243 * ((t754 - t762) * t62 + (t770 
     #- t776) * t62) / 0.24E2)
        t785 = t306 * t783 / 0.2E1
        t786 = u(t20,t45,k,n)
        t787 = t786 - t71
        t789 = t4 * t787 * t49
        t790 = u(t20,t51,k,n)
        t791 = t71 - t790
        t793 = t4 * t791 * t49
        t795 = (t789 - t793) * t49
        t796 = u(t20,j,t58,n)
        t797 = t796 - t71
        t799 = t4 * t797 * t62
        t800 = u(t20,j,t64,n)
        t801 = t71 - t800
        t803 = t4 * t801 * t62
        t805 = (t799 - t803) * t62
        t806 = t76 + t86 + t96 - t585 - t795 - t805
        t807 = t806 * t12
        t810 = t407 * (t430 / 0.2E1 + t807 / 0.2E1)
        t812 = t165 * t810 / 0.2E1
        t818 = t166 * (t25 - dx * (t297 - t681) / 0.12E2) / 0.12E2
        t820 = t4 * t806 * t12
        t823 = t77 - t786
        t825 = t4 * t823 * t12
        t827 = (t455 - t825) * t12
        t828 = u(i,t45,t58,n)
        t829 = t828 - t77
        t831 = t4 * t829 * t62
        t832 = u(i,t45,t64,n)
        t833 = t77 - t832
        t835 = t4 * t833 * t62
        t837 = (t831 - t835) * t62
        t838 = t827 + t618 + t837 - t76 - t86 - t96
        t840 = t4 * t838 * t49
        t841 = t81 - t790
        t843 = t4 * t841 * t12
        t845 = (t476 - t843) * t12
        t846 = u(i,t51,t58,n)
        t847 = t846 - t81
        t849 = t4 * t847 * t62
        t850 = u(i,t51,t64,n)
        t851 = t81 - t850
        t853 = t4 * t851 * t62
        t855 = (t849 - t853) * t62
        t856 = t76 + t86 + t96 - t845 - t624 - t855
        t858 = t4 * t856 * t49
        t861 = t87 - t796
        t863 = t4 * t861 * t12
        t865 = (t499 - t863) * t12
        t866 = t828 - t87
        t868 = t4 * t866 * t49
        t869 = t87 - t846
        t871 = t4 * t869 * t49
        t873 = (t868 - t871) * t49
        t874 = t865 + t873 + t657 - t76 - t86 - t96
        t876 = t4 * t874 * t62
        t877 = t91 - t800
        t879 = t4 * t877 * t12
        t881 = (t518 - t879) * t12
        t882 = t832 - t91
        t884 = t4 * t882 * t49
        t885 = t91 - t850
        t887 = t4 * t885 * t49
        t889 = (t884 - t887) * t49
        t890 = t76 + t86 + t96 - t881 - t889 - t663
        t892 = t4 * t890 * t62
        t896 = t104 * ((t447 - t820) * t12 + (t840 - t858) * t49 + (t876
     # - t892) * t62)
        t898 = t443 * t896 / 0.6E1
        t899 = ut(t20,t45,k,n)
        t900 = t899 - t21
        t902 = t4 * t900 * t49
        t903 = ut(t20,t51,k,n)
        t904 = t21 - t903
        t906 = t4 * t904 * t49
        t908 = (t902 - t906) * t49
        t909 = ut(t20,j,t58,n)
        t910 = t909 - t21
        t912 = t4 * t910 * t62
        t913 = ut(t20,j,t64,n)
        t914 = t21 - t913
        t916 = t4 * t914 * t62
        t918 = (t912 - t916) * t62
        t919 = t134 + t144 + t154 - t735 - t908 - t918
        t920 = t919 * t12
        t923 = t538 * (t561 / 0.2E1 + t920 / 0.2E1)
        t925 = t306 * t923 / 0.4E1
        t927 = t407 * (t430 - t807)
        t929 = t165 * t927 / 0.12E2
        t930 = t10 + t165 * t286 - t304 + t306 * t404 / 0.2E1 - t165 * t
     #433 / 0.2E1 + t441 + t443 * t535 / 0.6E1 - t306 * t564 / 0.4E1 + t
     #165 * t568 / 0.12E2 - t2 - t673 - t688 - t785 - t812 - t818 - t898
     # - t925 - t929
        t932 = sqrt(0.16E2)
        t935 = 0.1E1 / 0.2E1 - t6
        t936 = t4 * t935
        t937 = t936 * t30
        t938 = t935 ** 2
        t939 = t4 * t938
        t941 = t939 * t99 / 0.2E1
        t942 = t938 * t935
        t943 = t4 * t942
        t945 = t943 * t157 / 0.6E1
        t946 = t935 * dt
        t948 = t946 * t162 / 0.24E2
        t949 = beta * t935
        t951 = t305 * t938
        t956 = t442 * t942
        t963 = t949 * t672
        t965 = t951 * t783 / 0.2E1
        t967 = t949 * t810 / 0.2E1
        t969 = t956 * t896 / 0.6E1
        t971 = t951 * t923 / 0.4E1
        t973 = t949 * t927 / 0.12E2
        t974 = t10 + t949 * t286 - t304 + t951 * t404 / 0.2E1 - t949 * t
     #433 / 0.2E1 + t441 + t956 * t535 / 0.6E1 - t951 * t564 / 0.4E1 + t
     #949 * t568 / 0.12E2 - t2 - t963 - t688 - t965 - t967 - t818 - t969
     # - t971 - t973
        t977 = cc * t974 * t932 / 0.8E1
        t979 = (t8 * t30 + t33 * t99 / 0.2E1 + t103 * t157 / 0.6E1 - t16
     #0 * t162 / 0.24E2 + cc * t930 * t932 / 0.8E1 - t937 - t941 - t945 
     #+ t948 - t977) * t5
        t985 = t4 * (t174 - dx * t183 / 0.24E2)
        t987 = dx * t194 / 0.24E2
        t995 = dt * (t23 - dx * t680 / 0.24E2)
        t998 = t34 * t806 * t12
        t1002 = t104 * t919 * t12
        t1005 = dx * t736
        t1008 = u(t20,j,t244,n)
        t1009 = t1008 - t796
        t1011 = t797 * t62
        t1014 = t801 * t62
        t1016 = (t1011 - t1014) * t62
        t1020 = u(t20,j,t257,n)
        t1021 = t800 - t1020
        t1033 = (t4 * t1009 * t62 - t799) * t62
        t1039 = (t803 - t4 * t1021 * t62) * t62
        t1047 = i - 3
        t1049 = t572 - u(t1047,j,k,n)
        t1061 = (t583 - t4 * t1049 * t12) * t12
        t1069 = u(t20,t202,k,n)
        t1070 = t1069 - t786
        t1072 = t787 * t49
        t1075 = t791 * t49
        t1077 = (t1072 - t1075) * t49
        t1081 = u(t20,t215,k,n)
        t1082 = t790 - t1081
        t1094 = (t4 * t1070 * t49 - t789) * t49
        t1100 = (t793 - t4 * t1082 * t49) * t49
        t1109 = dt * (-t243 * ((t4 * ((t1009 * t62 - t1011) * t62 - t101
     #6) * t62 - t4 * (t1016 - (t1014 - t1021 * t62) * t62) * t62) * t62
     # + ((t1033 - t805) * t62 - (t805 - t1039) * t62) * t62) / 0.24E2 +
     # t585 - t166 * ((t579 - t4 * (t576 - (t574 - t1049 * t12) * t12) *
     # t12) * t12 + (t587 - (t585 - t1061) * t12) * t12) / 0.24E2 + t805
     # + t795 - t201 * ((t4 * ((t1070 * t49 - t1072) * t49 - t1077) * t4
     #9 - t4 * (t1077 - (t1075 - t1082 * t49) * t49) * t49) * t49 + ((t1
     #094 - t795) * t49 - (t795 - t1100) * t49) * t49) / 0.24E2)
        t1113 = t675 - ut(t1047,j,k,n)
        t1117 = t679 - (t677 - t1113 * t12) * t12
        t1118 = t1117 * t12
        t1125 = dx * (t674 + t677 / 0.2E1 - t166 * (t681 / 0.2E1 + t1118
     # / 0.2E1) / 0.6E1) / 0.2E1
        t1126 = ut(t20,t202,k,n)
        t1127 = t1126 - t899
        t1129 = t900 * t49
        t1132 = t904 * t49
        t1134 = (t1129 - t1132) * t49
        t1138 = ut(t20,t215,k,n)
        t1139 = t903 - t1138
        t1165 = ut(t20,j,t244,n)
        t1166 = t1165 - t909
        t1168 = t910 * t62
        t1171 = t914 * t62
        t1173 = (t1168 - t1171) * t62
        t1177 = ut(t20,j,t257,n)
        t1178 = t913 - t1177
        t1211 = (t733 - t4 * t1113 * t12) * t12
        t1220 = t34 * (t735 + t918 + t908 - t201 * ((t4 * ((t1127 * t49 
     #- t1129) * t49 - t1134) * t49 - t4 * (t1134 - (t1132 - t1139 * t49
     #) * t49) * t49) * t49 + (((t4 * t1127 * t49 - t902) * t49 - t908) 
     #* t49 - (t908 - (t906 - t4 * t1139 * t49) * t49) * t49) * t49) / 0
     #.24E2 - t243 * ((t4 * ((t1166 * t62 - t1168) * t62 - t1173) * t62 
     #- t4 * (t1173 - (t1171 - t1178 * t62) * t62) * t62) * t62 + (((t4 
     #* t1166 * t62 - t912) * t62 - t918) * t62 - (t918 - (t916 - t4 * t
     #1178 * t62) * t62) * t62) * t62) / 0.24E2 - t166 * ((t729 - t4 * t
     #1117 * t12) * t12 + (t737 - (t735 - t1211) * t12) * t12) / 0.24E2)
        t1223 = u(t571,t45,k,n)
        t1227 = u(t571,t51,k,n)
        t1233 = u(t571,j,t58,n)
        t1237 = u(t571,j,t64,n)
        t1243 = t585 + t795 + t805 - t1061 - (t4 * (t1223 - t572) * t49 
     #- t4 * (t572 - t1227) * t49) * t49 - (t4 * (t1233 - t572) * t62 - 
     #t4 * (t572 - t1237) * t62) * t62
        t1244 = t1243 * t12
        t1247 = t407 * (t807 / 0.2E1 + t1244 / 0.2E1)
        t1255 = t166 * (t679 - dx * (t681 - t1118) / 0.12E2) / 0.12E2
        t1260 = t786 - t1223
        t1264 = (t825 - t4 * t1260 * t12) * t12
        t1265 = u(t20,t45,t58,n)
        t1269 = u(t20,t45,t64,n)
        t1274 = (t4 * (t1265 - t786) * t62 - t4 * (t786 - t1269) * t62) 
     #* t62
        t1278 = t790 - t1227
        t1282 = (t843 - t4 * t1278 * t12) * t12
        t1283 = u(t20,t51,t58,n)
        t1287 = u(t20,t51,t64,n)
        t1292 = (t4 * (t1283 - t790) * t62 - t4 * (t790 - t1287) * t62) 
     #* t62
        t1298 = t796 - t1233
        t1302 = (t863 - t4 * t1298 * t12) * t12
        t1310 = (t4 * (t1265 - t796) * t49 - t4 * (t796 - t1283) * t49) 
     #* t49
        t1314 = t800 - t1237
        t1318 = (t879 - t4 * t1314 * t12) * t12
        t1326 = (t4 * (t1269 - t800) * t49 - t4 * (t800 - t1287) * t49) 
     #* t49
        t1333 = t104 * ((t820 - t4 * t1243 * t12) * t12 + (t4 * (t1264 +
     # t1094 + t1274 - t585 - t795 - t805) * t49 - t4 * (t585 + t795 + t
     #805 - t1282 - t1100 - t1292) * t49) * t49 + (t4 * (t1302 + t1310 +
     # t1033 - t585 - t795 - t805) * t62 - t4 * (t585 + t795 + t805 - t1
     #318 - t1326 - t1039) * t62) * t62)
        t1336 = ut(t571,t45,k,n)
        t1340 = ut(t571,t51,k,n)
        t1346 = ut(t571,j,t58,n)
        t1350 = ut(t571,j,t64,n)
        t1360 = t538 * (t920 / 0.2E1 + (t735 + t908 + t918 - t1211 - (t4
     # * (t1336 - t675) * t49 - t4 * (t675 - t1340) * t49) * t49 - (t4 *
     # (t1346 - t675) * t62 - t4 * (t675 - t1350) * t62) * t62) * t12 / 
     #0.2E1)
        t1364 = t407 * (t807 - t1244)
        t1367 = t2 + t673 - t688 + t785 - t812 + t818 + t898 - t925 + t9
     #29 - t21 - t165 * t1109 - t1125 - t306 * t1220 / 0.2E1 - t165 * t1
     #247 / 0.2E1 - t1255 - t443 * t1333 / 0.6E1 - t306 * t1360 / 0.4E1 
     #- t165 * t1364 / 0.12E2
        t1371 = t936 * t995
        t1373 = t939 * t998 / 0.2E1
        t1375 = t943 * t1002 / 0.6E1
        t1377 = t946 * t1005 / 0.24E2
        t1389 = t2 + t963 - t688 + t965 - t967 + t818 + t969 - t971 + t9
     #73 - t21 - t949 * t1109 - t1125 - t951 * t1220 / 0.2E1 - t949 * t1
     #247 / 0.2E1 - t1255 - t956 * t1333 / 0.6E1 - t951 * t1360 / 0.4E1 
     #- t949 * t1364 / 0.12E2
        t1392 = cc * t1389 * t932 / 0.8E1
        t1394 = (t8 * t995 + t33 * t998 / 0.2E1 + t103 * t1002 / 0.6E1 -
     # t160 * t1005 / 0.24E2 + cc * t1367 * t932 / 0.8E1 - t1371 - t1373
     # - t1375 + t1377 - t1392) * t5
        t1400 = t4 * (t180 - dx * t577 / 0.24E2)
        t1402 = dx * t586 / 0.24E2
        t1412 = dt * (t692 - dy * t698 / 0.24E2)
        t1415 = t34 * t838 * t49
        t1418 = t111 - t135
        t1420 = t4 * t1418 * t12
        t1421 = t135 - t899
        t1423 = t4 * t1421 * t12
        t1425 = (t1420 - t1423) * t12
        t1426 = ut(i,t45,t58,n)
        t1427 = t1426 - t135
        t1429 = t4 * t1427 * t62
        t1430 = ut(i,t45,t64,n)
        t1431 = t135 - t1430
        t1433 = t4 * t1431 * t62
        t1435 = (t1429 - t1433) * t62
        t1436 = t1425 + t714 + t1435 - t134 - t144 - t154
        t1438 = t104 * t1436 * t49
        t1441 = dy * t715
        t1444 = j + 3
        t1446 = u(i,t1444,k,n) - t593
        t1458 = (t4 * t1446 * t49 - t616) * t49
        t1466 = u(i,t45,t244,n)
        t1467 = t1466 - t828
        t1469 = t829 * t62
        t1472 = t833 * t62
        t1474 = (t1469 - t1472) * t62
        t1478 = u(i,t45,t257,n)
        t1479 = t832 - t1478
        t1491 = (t4 * t1467 * t62 - t831) * t62
        t1497 = (t835 - t4 * t1479 * t62) * t62
        t1506 = t453 * t12
        t1509 = t823 * t12
        t1511 = (t1506 - t1509) * t12
        t1533 = dt * (-t201 * ((t4 * ((t1446 * t49 - t595) * t49 - t598)
     # * t49 - t604) * t49 + ((t1458 - t618) * t49 - t620) * t49) / 0.24
     #E2 - t243 * ((t4 * ((t1467 * t62 - t1469) * t62 - t1474) * t62 - t
     #4 * (t1474 - (t1472 - t1479 * t62) * t62) * t62) * t62 + ((t1491 -
     # t837) * t62 - (t837 - t1497) * t62) * t62) / 0.24E2 - t166 * ((t4
     # * ((t450 * t12 - t1506) * t12 - t1511) * t12 - t4 * (t1511 - (t15
     #09 - t1260 * t12) * t12) * t12) * t12 + ((t457 - t827) * t12 - (t8
     #27 - t1264) * t12) * t12) / 0.24E2 + t827 + t618 + t837)
        t1536 = t692 / 0.2E1
        t1538 = ut(i,t1444,k,n) - t689
        t1542 = (t1538 * t49 - t691) * t49 - t694
        t1543 = t1542 * t49
        t1544 = t698 * t49
        t1551 = dy * (t691 / 0.2E1 + t1536 - t201 * (t1543 / 0.2E1 + t15
     #44 / 0.2E1) / 0.6E1) / 0.2E1
        t1552 = t539 - t111
        t1554 = t1418 * t12
        t1557 = t1421 * t12
        t1559 = (t1554 - t1557) * t12
        t1563 = t899 - t1336
        t1596 = (t4 * t1538 * t49 - t712) * t49
        t1604 = ut(i,t45,t244,n)
        t1605 = t1604 - t1426
        t1607 = t1427 * t62
        t1610 = t1431 * t62
        t1612 = (t1607 - t1610) * t62
        t1616 = ut(i,t45,t257,n)
        t1617 = t1430 - t1616
        t1644 = t34 * (t714 + t1425 - t166 * ((t4 * ((t1552 * t12 - t155
     #4) * t12 - t1559) * t12 - t4 * (t1559 - (t1557 - t1563 * t12) * t1
     #2) * t12) * t12 + (((t4 * t1552 * t12 - t1420) * t12 - t1425) * t1
     #2 - (t1425 - (t1423 - t4 * t1563 * t12) * t12) * t12) * t12) / 0.2
     #4E2 - t201 * ((t4 * t1542 * t49 - t700) * t49 + ((t1596 - t714) * 
     #t49 - t716) * t49) / 0.24E2 + t1435 - t243 * ((t4 * ((t1605 * t62 
     #- t1607) * t62 - t1612) * t62 - t4 * (t1612 - (t1610 - t1617 * t62
     #) * t62) * t62) * t62 + (((t4 * t1605 * t62 - t1429) * t62 - t1435
     #) * t62 - (t1435 - (t1433 - t4 * t1617 * t62) * t62) * t62) * t62)
     # / 0.24E2)
        t1647 = dt * dy
        t1656 = u(i,t202,t58,n)
        t1660 = u(i,t202,t64,n)
        t1666 = (t4 * (t203 - t593) * t12 - t4 * (t593 - t1069) * t12) *
     # t12 + t1458 + (t4 * (t1656 - t593) * t62 - t4 * (t593 - t1660) * 
     #t62) * t62 - t827 - t618 - t837
        t1667 = t1666 * t49
        t1668 = t838 * t49
        t1671 = t1647 * (t1667 / 0.2E1 + t1668 / 0.2E1)
        t1679 = t201 * (t694 - dy * (t1543 - t1544) / 0.12E2) / 0.12E2
        t1699 = (t4 * (t458 - t828) * t12 - t4 * (t828 - t1265) * t12) *
     # t12
        t1700 = t1656 - t828
        t1704 = (t4 * t1700 * t49 - t868) * t49
        t1715 = (t4 * (t462 - t832) * t12 - t4 * (t832 - t1269) * t12) *
     # t12
        t1716 = t1660 - t832
        t1720 = (t4 * t1716 * t49 - t884) * t49
        t1727 = t104 * ((t4 * (t457 + t229 + t467 - t827 - t618 - t837) 
     #* t12 - t4 * (t827 + t618 + t837 - t1264 - t1094 - t1274) * t12) *
     # t12 + (t4 * t1666 * t49 - t840) * t49 + (t4 * (t1699 + t1704 + t1
     #491 - t827 - t618 - t837) * t62 - t4 * (t827 + t618 + t837 - t1715
     # - t1720 - t1497) * t62) * t62)
        t1730 = t34 * dy
        t1739 = ut(i,t202,t58,n)
        t1743 = ut(i,t202,t64,n)
        t1751 = t1436 * t49
        t1754 = t1730 * (((t4 * (t346 - t689) * t12 - t4 * (t689 - t1126
     #) * t12) * t12 + t1596 + (t4 * (t1739 - t689) * t62 - t4 * (t689 -
     # t1743) * t62) * t62 - t1425 - t714 - t1435) * t49 / 0.2E1 + t1751
     # / 0.2E1)
        t1758 = t1647 * (t1667 - t1668)
        t1761 = t695 / 0.2E1
        t1762 = t706 * t49
        t1769 = dy * (t1536 + t1761 - t201 * (t1544 / 0.2E1 + t1762 / 0.
     #2E1) / 0.6E1) / 0.2E1
        t1770 = t856 * t49
        t1773 = t1647 * (t1668 / 0.2E1 + t1770 / 0.2E1)
        t1775 = t165 * t1773 / 0.2E1
        t1781 = t201 * (t697 - dy * (t1544 - t1762) / 0.12E2) / 0.12E2
        t1782 = t115 - t139
        t1784 = t4 * t1782 * t12
        t1785 = t139 - t903
        t1787 = t4 * t1785 * t12
        t1789 = (t1784 - t1787) * t12
        t1790 = ut(i,t51,t58,n)
        t1791 = t1790 - t139
        t1793 = t4 * t1791 * t62
        t1794 = ut(i,t51,t64,n)
        t1795 = t139 - t1794
        t1797 = t4 * t1795 * t62
        t1799 = (t1793 - t1797) * t62
        t1800 = t134 + t144 + t154 - t1789 - t720 - t1799
        t1801 = t1800 * t49
        t1804 = t1730 * (t1751 / 0.2E1 + t1801 / 0.2E1)
        t1806 = t306 * t1804 / 0.4E1
        t1808 = t1647 * (t1668 - t1770)
        t1810 = t165 * t1808 / 0.12E2
        t1811 = t135 + t165 * t1533 - t1551 + t306 * t1644 / 0.2E1 - t16
     #5 * t1671 / 0.2E1 + t1679 + t443 * t1727 / 0.6E1 - t306 * t1754 / 
     #0.4E1 + t165 * t1758 / 0.12E2 - t2 - t673 - t1769 - t785 - t1775 -
     # t1781 - t898 - t1806 - t1810
        t1815 = t936 * t1412
        t1817 = t939 * t1415 / 0.2E1
        t1819 = t943 * t1438 / 0.6E1
        t1821 = t946 * t1441 / 0.24E2
        t1834 = t949 * t1773 / 0.2E1
        t1836 = t951 * t1804 / 0.4E1
        t1838 = t949 * t1808 / 0.12E2
        t1839 = t135 + t949 * t1533 - t1551 + t951 * t1644 / 0.2E1 - t94
     #9 * t1671 / 0.2E1 + t1679 + t956 * t1727 / 0.6E1 - t951 * t1754 / 
     #0.4E1 + t949 * t1758 / 0.12E2 - t2 - t963 - t1769 - t965 - t1834 -
     # t1781 - t969 - t1836 - t1838
        t1842 = cc * t1839 * t932 / 0.8E1
        t1844 = (t8 * t1412 + t33 * t1415 / 0.2E1 + t103 * t1438 / 0.6E1
     # - t160 * t1441 / 0.24E2 + cc * t1811 * t932 / 0.8E1 - t1815 - t18
     #17 - t1819 + t1821 - t1842) * t5
        t1850 = t4 * (t596 - dy * t602 / 0.24E2)
        t1852 = dy * t619 / 0.24E2
        t1860 = dt * (t695 - dy * t706 / 0.24E2)
        t1863 = t34 * t856 * t49
        t1867 = t104 * t1800 * t49
        t1870 = dy * t721
        t1873 = j - 3
        t1875 = t605 - u(i,t1873,k,n)
        t1887 = (t622 - t4 * t1875 * t49) * t49
        t1896 = t474 * t12
        t1899 = t841 * t12
        t1901 = (t1896 - t1899) * t12
        t1922 = u(i,t51,t244,n)
        t1923 = t1922 - t846
        t1925 = t847 * t62
        t1928 = t851 * t62
        t1930 = (t1925 - t1928) * t62
        t1934 = u(i,t51,t257,n)
        t1935 = t850 - t1934
        t1947 = (t4 * t1923 * t62 - t849) * t62
        t1953 = (t853 - t4 * t1935 * t62) * t62
        t1962 = dt * (-t201 * ((t612 - t4 * (t609 - (t607 - t1875 * t49)
     # * t49) * t49) * t49 + (t626 - (t624 - t1887) * t49) * t49) / 0.24
     #E2 - t166 * ((t4 * ((t471 * t12 - t1896) * t12 - t1901) * t12 - t4
     # * (t1901 - (t1899 - t1278 * t12) * t12) * t12) * t12 + ((t478 - t
     #845) * t12 - (t845 - t1282) * t12) * t12) / 0.24E2 - t243 * ((t4 *
     # ((t1923 * t62 - t1925) * t62 - t1930) * t62 - t4 * (t1930 - (t192
     #8 - t1935 * t62) * t62) * t62) * t62 + ((t1947 - t855) * t62 - (t8
     #55 - t1953) * t62) * t62) / 0.24E2 + t845 + t624 + t855)
        t1966 = t701 - ut(i,t1873,k,n)
        t1970 = t705 - (t703 - t1966 * t49) * t49
        t1971 = t1970 * t49
        t1978 = dy * (t1761 + t703 / 0.2E1 - t201 * (t1762 / 0.2E1 + t19
     #71 / 0.2E1) / 0.6E1) / 0.2E1
        t1979 = t543 - t115
        t1981 = t1782 * t12
        t1984 = t1785 * t12
        t1986 = (t1981 - t1984) * t12
        t1990 = t903 - t1340
        t2016 = ut(i,t51,t244,n)
        t2017 = t2016 - t1790
        t2019 = t1791 * t62
        t2022 = t1795 * t62
        t2024 = (t2019 - t2022) * t62
        t2028 = ut(i,t51,t257,n)
        t2029 = t1794 - t2028
        t2062 = (t718 - t4 * t1966 * t49) * t49
        t2071 = t34 * (t1799 + t1789 - t166 * ((t4 * ((t1979 * t12 - t19
     #81) * t12 - t1986) * t12 - t4 * (t1986 - (t1984 - t1990 * t12) * t
     #12) * t12) * t12 + (((t4 * t1979 * t12 - t1784) * t12 - t1789) * t
     #12 - (t1789 - (t1787 - t4 * t1990 * t12) * t12) * t12) * t12) / 0.
     #24E2 - t243 * ((t4 * ((t2017 * t62 - t2019) * t62 - t2024) * t62 -
     # t4 * (t2024 - (t2022 - t2029 * t62) * t62) * t62) * t62 + (((t4 *
     # t2017 * t62 - t1793) * t62 - t1799) * t62 - (t1799 - (t1797 - t4 
     #* t2029 * t62) * t62) * t62) * t62) / 0.24E2 + t720 - t201 * ((t70
     #8 - t4 * t1970 * t49) * t49 + (t722 - (t720 - t2062) * t49) * t49)
     # / 0.24E2)
        t2082 = u(i,t215,t58,n)
        t2086 = u(i,t215,t64,n)
        t2092 = t845 + t624 + t855 - (t4 * (t216 - t605) * t12 - t4 * (t
     #605 - t1081) * t12) * t12 - t1887 - (t4 * (t2082 - t605) * t62 - t
     #4 * (t605 - t2086) * t62) * t62
        t2093 = t2092 * t49
        t2096 = t1647 * (t1770 / 0.2E1 + t2093 / 0.2E1)
        t2104 = t201 * (t705 - dy * (t1762 - t1971) / 0.12E2) / 0.12E2
        t2124 = (t4 * (t479 - t846) * t12 - t4 * (t846 - t1283) * t12) *
     # t12
        t2125 = t846 - t2082
        t2129 = (t871 - t4 * t2125 * t49) * t49
        t2140 = (t4 * (t483 - t850) * t12 - t4 * (t850 - t1287) * t12) *
     # t12
        t2141 = t850 - t2086
        t2145 = (t887 - t4 * t2141 * t49) * t49
        t2152 = t104 * ((t4 * (t478 + t235 + t488 - t845 - t624 - t855) 
     #* t12 - t4 * (t845 + t624 + t855 - t1282 - t1100 - t1292) * t12) *
     # t12 + (t858 - t4 * t2092 * t49) * t49 + (t4 * (t2124 + t2129 + t1
     #947 - t845 - t624 - t855) * t62 - t4 * (t845 + t624 + t855 - t2140
     # - t2145 - t1953) * t62) * t62)
        t2163 = ut(i,t215,t58,n)
        t2167 = ut(i,t215,t64,n)
        t2177 = t1730 * (t1801 / 0.2E1 + (t1789 + t720 + t1799 - (t4 * (
     #t358 - t701) * t12 - t4 * (t701 - t1138) * t12) * t12 - t2062 - (t
     #4 * (t2163 - t701) * t62 - t4 * (t701 - t2167) * t62) * t62) * t49
     # / 0.2E1)
        t2181 = t1647 * (t1770 - t2093)
        t2184 = t2 + t673 - t1769 + t785 - t1775 + t1781 + t898 - t1806 
     #+ t1810 - t139 - t165 * t1962 - t1978 - t306 * t2071 / 0.2E1 - t16
     #5 * t2096 / 0.2E1 - t2104 - t443 * t2152 / 0.6E1 - t306 * t2177 / 
     #0.4E1 - t165 * t2181 / 0.12E2
        t2188 = t936 * t1860
        t2190 = t939 * t1863 / 0.2E1
        t2192 = t943 * t1867 / 0.6E1
        t2194 = t946 * t1870 / 0.24E2
        t2206 = t2 + t963 - t1769 + t965 - t1834 + t1781 + t969 - t1836 
     #+ t1838 - t139 - t949 * t1962 - t1978 - t951 * t2071 / 0.2E1 - t94
     #9 * t2096 / 0.2E1 - t2104 - t956 * t2152 / 0.6E1 - t951 * t2177 / 
     #0.4E1 - t949 * t2181 / 0.12E2
        t2209 = cc * t2206 * t932 / 0.8E1
        t2211 = (t8 * t1860 + t33 * t1863 / 0.2E1 + t103 * t1867 / 0.6E1
     # - t160 * t1870 / 0.24E2 + cc * t2184 * t932 / 0.8E1 - t2188 - t21
     #90 - t2192 + t2194 - t2209) * t5
        t2217 = t4 * (t599 - dy * t610 / 0.24E2)
        t2219 = dy * t625 / 0.24E2
        t2229 = dt * (t746 - dz * t752 / 0.24E2)
        t2232 = t34 * t874 * t62
        t2235 = t121 - t145
        t2237 = t4 * t2235 * t12
        t2238 = t145 - t909
        t2240 = t4 * t2238 * t12
        t2242 = (t2237 - t2240) * t12
        t2243 = t1426 - t145
        t2245 = t4 * t2243 * t49
        t2246 = t145 - t1790
        t2248 = t4 * t2246 * t49
        t2250 = (t2245 - t2248) * t49
        t2251 = t2242 + t2250 + t768 - t134 - t144 - t154
        t2253 = t104 * t2251 * t62
        t2256 = dz * t769
        t2260 = t497 * t12
        t2263 = t861 * t12
        t2265 = (t2260 - t2263) * t12
        t2287 = t866 * t49
        t2290 = t869 * t49
        t2292 = (t2287 - t2290) * t49
        t2313 = k + 3
        t2315 = u(i,j,t2313,n) - t632
        t2327 = (t4 * t2315 * t62 - t655) * t62
        t2336 = dt * (-t166 * ((t4 * ((t494 * t12 - t2260) * t12 - t2265
     #) * t12 - t4 * (t2265 - (t2263 - t1298 * t12) * t12) * t12) * t12 
     #+ ((t501 - t865) * t12 - (t865 - t1302) * t12) * t12) / 0.24E2 - t
     #201 * ((t4 * ((t1700 * t49 - t2287) * t49 - t2292) * t49 - t4 * (t
     #2292 - (t2290 - t2125 * t49) * t49) * t49) * t49 + ((t1704 - t873)
     # * t49 - (t873 - t2129) * t49) * t49) / 0.24E2 - t243 * ((t4 * ((t
     #2315 * t62 - t634) * t62 - t637) * t62 - t643) * t62 + ((t2327 - t
     #657) * t62 - t659) * t62) / 0.24E2 + t865 + t873 + t657)
        t2339 = t746 / 0.2E1
        t2341 = ut(i,j,t2313,n) - t743
        t2345 = (t2341 * t62 - t745) * t62 - t748
        t2346 = t2345 * t62
        t2347 = t752 * t62
        t2354 = dz * (t745 / 0.2E1 + t2339 - t243 * (t2346 / 0.2E1 + t23
     #47 / 0.2E1) / 0.6E1) / 0.2E1
        t2355 = t549 - t121
        t2357 = t2235 * t12
        t2360 = t2238 * t12
        t2362 = (t2357 - t2360) * t12
        t2366 = t909 - t1346
        t2392 = t1739 - t1426
        t2394 = t2243 * t49
        t2397 = t2246 * t49
        t2399 = (t2394 - t2397) * t49
        t2403 = t1790 - t2163
        t2436 = (t4 * t2341 * t62 - t766) * t62
        t2445 = t34 * (t2242 - t166 * ((t4 * ((t2355 * t12 - t2357) * t1
     #2 - t2362) * t12 - t4 * (t2362 - (t2360 - t2366 * t12) * t12) * t1
     #2) * t12 + (((t4 * t2355 * t12 - t2237) * t12 - t2242) * t12 - (t2
     #242 - (t2240 - t4 * t2366 * t12) * t12) * t12) * t12) / 0.24E2 + t
     #2250 - t201 * ((t4 * ((t2392 * t49 - t2394) * t49 - t2399) * t49 -
     # t4 * (t2399 - (t2397 - t2403 * t49) * t49) * t49) * t49 + (((t4 *
     # t2392 * t49 - t2245) * t49 - t2250) * t49 - (t2250 - (t2248 - t4 
     #* t2403 * t49) * t49) * t49) * t49) / 0.24E2 + t768 - t243 * ((t4 
     #* t2345 * t62 - t754) * t62 + ((t2436 - t768) * t62 - t770) * t62)
     # / 0.24E2)
        t2448 = dt * dz
        t2465 = (t4 * (t245 - t632) * t12 - t4 * (t632 - t1008) * t12) *
     # t12 + (t4 * (t1466 - t632) * t49 - t4 * (t632 - t1922) * t49) * t
     #49 + t2327 - t865 - t873 - t657
        t2466 = t2465 * t62
        t2467 = t874 * t62
        t2470 = t2448 * (t2466 / 0.2E1 + t2467 / 0.2E1)
        t2478 = t243 * (t748 - dz * (t2346 - t2347) / 0.12E2) / 0.12E2
        t2500 = t104 * ((t4 * (t501 + t509 + t271 - t865 - t873 - t657) 
     #* t12 - t4 * (t865 + t873 + t657 - t1302 - t1310 - t1033) * t12) *
     # t12 + (t4 * (t1699 + t1704 + t1491 - t865 - t873 - t657) * t49 - 
     #t4 * (t865 + t873 + t657 - t2124 - t2129 - t1947) * t49) * t49 + (
     #t4 * t2465 * t62 - t876) * t62)
        t2503 = t34 * dz
        t2522 = t2251 * t62
        t2525 = t2503 * (((t4 * (t307 - t743) * t12 - t4 * (t743 - t1165
     #) * t12) * t12 + (t4 * (t1604 - t743) * t49 - t4 * (t743 - t2016) 
     #* t49) * t49 + t2436 - t2242 - t2250 - t768) * t62 / 0.2E1 + t2522
     # / 0.2E1)
        t2529 = t2448 * (t2466 - t2467)
        t2532 = t749 / 0.2E1
        t2533 = t760 * t62
        t2540 = dz * (t2339 + t2532 - t243 * (t2347 / 0.2E1 + t2533 / 0.
     #2E1) / 0.6E1) / 0.2E1
        t2541 = t890 * t62
        t2544 = t2448 * (t2467 / 0.2E1 + t2541 / 0.2E1)
        t2546 = t165 * t2544 / 0.2E1
        t2552 = t243 * (t751 - dz * (t2347 - t2533) / 0.12E2) / 0.12E2
        t2553 = t125 - t149
        t2555 = t4 * t2553 * t12
        t2556 = t149 - t913
        t2558 = t4 * t2556 * t12
        t2560 = (t2555 - t2558) * t12
        t2561 = t1430 - t149
        t2563 = t4 * t2561 * t49
        t2564 = t149 - t1794
        t2566 = t4 * t2564 * t49
        t2568 = (t2563 - t2566) * t49
        t2569 = t134 + t144 + t154 - t2560 - t2568 - t774
        t2570 = t2569 * t62
        t2573 = t2503 * (t2522 / 0.2E1 + t2570 / 0.2E1)
        t2575 = t306 * t2573 / 0.4E1
        t2577 = t2448 * (t2467 - t2541)
        t2579 = t165 * t2577 / 0.12E2
        t2580 = t145 + t165 * t2336 - t2354 + t306 * t2445 / 0.2E1 - t16
     #5 * t2470 / 0.2E1 + t2478 + t443 * t2500 / 0.6E1 - t306 * t2525 / 
     #0.4E1 + t165 * t2529 / 0.12E2 - t2 - t673 - t2540 - t785 - t2546 -
     # t2552 - t898 - t2575 - t2579
        t2584 = t936 * t2229
        t2586 = t939 * t2232 / 0.2E1
        t2588 = t943 * t2253 / 0.6E1
        t2590 = t946 * t2256 / 0.24E2
        t2603 = t949 * t2544 / 0.2E1
        t2605 = t951 * t2573 / 0.4E1
        t2607 = t949 * t2577 / 0.12E2
        t2608 = t145 + t949 * t2336 - t2354 + t951 * t2445 / 0.2E1 - t94
     #9 * t2470 / 0.2E1 + t2478 + t956 * t2500 / 0.6E1 - t951 * t2525 / 
     #0.4E1 + t949 * t2529 / 0.12E2 - t2 - t963 - t2540 - t965 - t2603 -
     # t2552 - t969 - t2605 - t2607
        t2611 = cc * t2608 * t932 / 0.8E1
        t2613 = (t8 * t2229 + t33 * t2232 / 0.2E1 + t103 * t2253 / 0.6E1
     # - t160 * t2256 / 0.24E2 + cc * t2580 * t932 / 0.8E1 - t2584 - t25
     #86 - t2588 + t2590 - t2611) * t5
        t2619 = t4 * (t635 - dz * t641 / 0.24E2)
        t2621 = dz * t658 / 0.24E2
        t2629 = dt * (t749 - dz * t760 / 0.24E2)
        t2632 = t34 * t890 * t62
        t2636 = t104 * t2569 * t62
        t2639 = dz * t775
        t2643 = t516 * t12
        t2646 = t877 * t12
        t2648 = (t2643 - t2646) * t12
        t2669 = k - 3
        t2671 = t644 - u(i,j,t2669,n)
        t2683 = (t661 - t4 * t2671 * t62) * t62
        t2692 = t882 * t49
        t2695 = t885 * t49
        t2697 = (t2692 - t2695) * t49
        t2719 = dt * (-t166 * ((t4 * ((t513 * t12 - t2643) * t12 - t2648
     #) * t12 - t4 * (t2648 - (t2646 - t1314 * t12) * t12) * t12) * t12 
     #+ ((t520 - t881) * t12 - (t881 - t1318) * t12) * t12) / 0.24E2 + t
     #663 - t243 * ((t651 - t4 * (t648 - (t646 - t2671 * t62) * t62) * t
     #62) * t62 + (t665 - (t663 - t2683) * t62) * t62) / 0.24E2 + t889 -
     # t201 * ((t4 * ((t1716 * t49 - t2692) * t49 - t2697) * t49 - t4 * 
     #(t2697 - (t2695 - t2141 * t49) * t49) * t49) * t49 + ((t1720 - t88
     #9) * t49 - (t889 - t2145) * t49) * t49) / 0.24E2 + t881)
        t2723 = t755 - ut(i,j,t2669,n)
        t2727 = t759 - (t757 - t2723 * t62) * t62
        t2728 = t2727 * t62
        t2735 = dz * (t2532 + t757 / 0.2E1 - t243 * (t2533 / 0.2E1 + t27
     #28 / 0.2E1) / 0.6E1) / 0.2E1
        t2736 = t1743 - t1430
        t2738 = t2561 * t49
        t2741 = t2564 * t49
        t2743 = (t2738 - t2741) * t49
        t2747 = t1794 - t2167
        t2780 = (t772 - t4 * t2723 * t62) * t62
        t2788 = t553 - t125
        t2790 = t2553 * t12
        t2793 = t2556 * t12
        t2795 = (t2790 - t2793) * t12
        t2799 = t913 - t1350
        t2826 = t34 * (-t201 * ((t4 * ((t2736 * t49 - t2738) * t49 - t27
     #43) * t49 - t4 * (t2743 - (t2741 - t2747 * t49) * t49) * t49) * t4
     #9 + (((t4 * t2736 * t49 - t2563) * t49 - t2568) * t49 - (t2568 - (
     #t2566 - t4 * t2747 * t49) * t49) * t49) * t49) / 0.24E2 + t774 - t
     #243 * ((t762 - t4 * t2727 * t62) * t62 + (t776 - (t774 - t2780) * 
     #t62) * t62) / 0.24E2 + t2568 + t2560 - t166 * ((t4 * ((t2788 * t12
     # - t2790) * t12 - t2795) * t12 - t4 * (t2795 - (t2793 - t2799 * t1
     #2) * t12) * t12) * t12 + (((t4 * t2788 * t12 - t2555) * t12 - t256
     #0) * t12 - (t2560 - (t2558 - t4 * t2799 * t12) * t12) * t12) * t12
     #) / 0.24E2)
        t2845 = t881 + t889 + t663 - (t4 * (t258 - t644) * t12 - t4 * (t
     #644 - t1020) * t12) * t12 - (t4 * (t1478 - t644) * t49 - t4 * (t64
     #4 - t1934) * t49) * t49 - t2683
        t2846 = t2845 * t62
        t2849 = t2448 * (t2541 / 0.2E1 + t2846 / 0.2E1)
        t2857 = t243 * (t759 - dz * (t2533 - t2728) / 0.12E2) / 0.12E2
        t2879 = t104 * ((t4 * (t520 + t528 + t277 - t881 - t889 - t663) 
     #* t12 - t4 * (t881 + t889 + t663 - t1318 - t1326 - t1039) * t12) *
     # t12 + (t4 * (t1715 + t1720 + t1497 - t881 - t889 - t663) * t49 - 
     #t4 * (t881 + t889 + t663 - t2140 - t2145 - t1953) * t49) * t49 + (
     #t892 - t4 * t2845 * t62) * t62)
        t2902 = t2503 * (t2570 / 0.2E1 + (t2560 + t2568 + t774 - (t4 * (
     #t319 - t755) * t12 - t4 * (t755 - t1177) * t12) * t12 - (t4 * (t16
     #16 - t755) * t49 - t4 * (t755 - t2028) * t49) * t49 - t2780) * t62
     # / 0.2E1)
        t2906 = t2448 * (t2541 - t2846)
        t2909 = t2 + t673 - t2540 + t785 - t2546 + t2552 + t898 - t2575 
     #+ t2579 - t149 - t165 * t2719 - t2735 - t306 * t2826 / 0.2E1 - t16
     #5 * t2849 / 0.2E1 - t2857 - t443 * t2879 / 0.6E1 - t306 * t2902 / 
     #0.4E1 - t165 * t2906 / 0.12E2
        t2913 = t936 * t2629
        t2915 = t939 * t2632 / 0.2E1
        t2917 = t943 * t2636 / 0.6E1
        t2919 = t946 * t2639 / 0.24E2
        t2931 = t2 + t963 - t2540 + t965 - t2603 + t2552 + t969 - t2605 
     #+ t2607 - t149 - t949 * t2719 - t2735 - t951 * t2826 / 0.2E1 - t94
     #9 * t2849 / 0.2E1 - t2857 - t956 * t2879 / 0.6E1 - t951 * t2902 / 
     #0.4E1 - t949 * t2906 / 0.12E2
        t2934 = cc * t2931 * t932 / 0.8E1
        t2936 = (t8 * t2629 + t33 * t2632 / 0.2E1 + t103 * t2636 / 0.6E1
     # - t160 * t2639 / 0.24E2 + cc * t2909 * t932 / 0.8E1 - t2913 - t29
     #15 - t2917 + t2919 - t2934) * t5
        t2942 = t4 * (t638 - dz * t649 / 0.24E2)
        t2944 = dz * t664 / 0.24E2

        unew(i,j,k) = t1 + dt * t2 + (t979 * t34 / 0.6E1 + (t985 + t9
     #37 + t941 - t987 + t945 - t948 + t977 - t979 * t935) * t34 / 0.2E1
     # - t1394 * t34 / 0.6E1 - (t1400 + t1371 + t1373 - t1402 + t1375 - 
     #t1377 + t1392 - t1394 * t935) * t34 / 0.2E1) * t12 + (t1844 * t34 
     #/ 0.6E1 + (t1850 + t1815 + t1817 - t1852 + t1819 - t1821 + t1842 -
     # t1844 * t935) * t34 / 0.2E1 - t2211 * t34 / 0.6E1 - (t2217 + t218
     #8 + t2190 - t2219 + t2192 - t2194 + t2209 - t2211 * t935) * t34 / 
     #0.2E1) * t49 + (t2613 * t34 / 0.6E1 + (t2619 + t2584 + t2586 - t26
     #21 + t2588 - t2590 + t2611 - t2613 * t935) * t34 / 0.2E1 - t2936 *
     # t34 / 0.6E1 - (t2942 + t2913 + t2915 - t2944 + t2917 - t2919 + t2
     #934 - t2936 * t935) * t34 / 0.2E1) * t62

        utnew(i,j,k) = t2 + (t979 * dt / 0.
     #2E1 + (t985 + t937 + t941 - t987 + t945 - t948 + t977) * dt - t979
     # * t946 - t1394 * dt / 0.2E1 - (t1400 + t1371 + t1373 - t1402 + t1
     #375 - t1377 + t1392) * dt + t1394 * t946) * t12 + (t1844 * dt / 0.
     #2E1 + (t1850 + t1815 + t1817 - t1852 + t1819 - t1821 + t1842) * dt
     # - t1844 * t946 - t2211 * dt / 0.2E1 - (t2217 + t2188 + t2190 - t2
     #219 + t2192 - t2194 + t2209) * dt + t2211 * t946) * t49 + (t2613 *
     # dt / 0.2E1 + (t2619 + t2584 + t2586 - t2621 + t2588 - t2590 + t26
     #11) * dt - t2613 * t946 - t2936 * dt / 0.2E1 - (t2942 + t2913 + t2
     #915 - t2944 + t2917 - t2919 + t2934) * dt + t2936 * t946) * t62

c        blah = array(int(t1 + dt * t2 + (t979 * t34 / 0.6E1 + (t985 + t9
c     #37 + t941 - t987 + t945 - t948 + t977 - t979 * t935) * t34 / 0.2E1
c     # - t1394 * t34 / 0.6E1 - (t1400 + t1371 + t1373 - t1402 + t1375 - 
c     #t1377 + t1392 - t1394 * t935) * t34 / 0.2E1) * t12 + (t1844 * t34 
c     #/ 0.6E1 + (t1850 + t1815 + t1817 - t1852 + t1819 - t1821 + t1842 -
c     # t1844 * t935) * t34 / 0.2E1 - t2211 * t34 / 0.6E1 - (t2217 + t218
c     #8 + t2190 - t2219 + t2192 - t2194 + t2209 - t2211 * t935) * t34 / 
c     #0.2E1) * t49 + (t2613 * t34 / 0.6E1 + (t2619 + t2584 + t2586 - t26
c     #21 + t2588 - t2590 + t2611 - t2613 * t935) * t34 / 0.2E1 - t2936 *
c     # t34 / 0.6E1 - (t2942 + t2913 + t2915 - t2944 + t2917 - t2919 + t2
c     #934 - t2936 * t935) * t34 / 0.2E1) * t62),int(t2 + (t979 * dt / 0.
c     #2E1 + (t985 + t937 + t941 - t987 + t945 - t948 + t977) * dt - t979
c     # * t946 - t1394 * dt / 0.2E1 - (t1400 + t1371 + t1373 - t1402 + t1
c     #375 - t1377 + t1392) * dt + t1394 * t946) * t12 + (t1844 * dt / 0.
c     #2E1 + (t1850 + t1815 + t1817 - t1852 + t1819 - t1821 + t1842) * dt
c     # - t1844 * t946 - t2211 * dt / 0.2E1 - (t2217 + t2188 + t2190 - t2
c     #219 + t2192 - t2194 + t2209) * dt + t2211 * t946) * t49 + (t2613 *
c     # dt / 0.2E1 + (t2619 + t2584 + t2586 - t2621 + t2588 - t2590 + t26
c     #11) * dt - t2613 * t946 - t2936 * dt / 0.2E1 - (t2942 + t2913 + t2
c     #915 - t2944 + t2917 - t2919 + t2934) * dt + t2936 * t946) * t62))

        return
      end
