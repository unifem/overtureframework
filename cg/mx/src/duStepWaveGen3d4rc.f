      subroutine duStepWaveGen3d4rc( 
     *   nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *   n1a,n1b,n2a,n2b,n3a,n3b,
     *   u,ut,unew,utnew,
     *   dx,dy,dz,dt,cc,
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
      real dx,dy,dz,dt,cc
c
c.. generated code to follow
c
        real t1
        real t10
        real t1002
        real t1004
        real t1008
        real t1009
        real t102
        real t1021
        real t1027
        real t103
        real t1035
        real t1036
        real t1038
        real t104
        real t1041
        real t1043
        real t1047
        real t1048
        real t106
        real t1060
        real t1066
        integer t1074
        real t1076
        real t108
        real t1088
        real t1096
        real t11
        real t110
        real t1100
        real t1104
        real t1105
        real t111
        real t1112
        real t112
        real t1120
        real t1128
        real t1129
        real t1131
        real t1134
        real t1136
        real t114
        real t1140
        real t1141
        real t115
        real t116
        real t1167
        real t1168
        real t1170
        real t1173
        real t1175
        real t1179
        real t118
        real t1180
        real t12
        real t120
        real t1206
        real t1209
        real t121
        real t1213
        real t1219
        real t122
        real t1223
        real t1229
        real t1230
        real t1233
        real t124
        real t1241
        real t1246
        real t125
        real t1250
        real t1251
        real t1255
        real t126
        real t1260
        real t1264
        real t1268
        real t1269
        real t1273
        real t1278
        real t128
        real t1284
        real t1288
        real t1296
        real t13
        real t130
        real t1300
        real t1304
        real t1312
        real t1318
        real t132
        real t1321
        real t1325
        real t1331
        real t1335
        real t134
        real t1345
        real t1349
        real t135
        real t1352
        real t1356
        real t1358
        real t136
        real t1360
        real t1362
        real t1374
        real t1377
        real t1379
        real t138
        real t1385
        real t1387
        real t139
        real t1397
        integer t14
        real t140
        real t1400
        real t1403
        real t1405
        real t1406
        real t1408
        real t1410
        real t1411
        real t1412
        real t1414
        real t1415
        real t1416
        real t1418
        real t142
        real t1420
        real t1421
        real t1423
        real t1426
        real t1429
        real t1430
        real t1432
        real t1435
        real t1437
        real t144
        real t1441
        real t1442
        real t145
        real t1454
        real t146
        real t1460
        real t1469
        real t1472
        real t1474
        real t148
        real t149
        integer t1495
        real t1497
        real t15
        real t150
        real t1509
        real t1517
        real t152
        real t1520
        real t1522
        real t1526
        real t1527
        real t1528
        real t1535
        real t1536
        real t1537
        real t1539
        real t154
        real t1542
        real t1544
        real t1548
        real t1549
        real t155
        real t157
        real t1575
        real t1577
        real t1580
        real t1582
        real t1586
        real t16
        real t160
        real t161
        real t1619
        real t162
        real t1627
        real t1638
        real t1642
        real t1648
        real t1649
        real t165
        real t1650
        real t1653
        integer t166
        real t1661
        real t167
        real t168
        real t1681
        real t1682
        real t1686
        real t1697
        real t1698
        real t17
        real t170
        real t1702
        real t1708
        real t1719
        real t1723
        real t173
        real t1731
        real t1734
        real t1738
        real t1741
        real t1742
        real t1749
        real t175
        real t1750
        real t1753
        real t1755
        real t1761
        real t1762
        real t1764
        real t1765
        real t1767
        real t1769
        real t1770
        real t1771
        real t1773
        real t1774
        real t1775
        real t1777
        real t1779
        real t1780
        real t1781
        real t1784
        real t1786
        real t1788
        integer t179
        real t1790
        real t1791
        real t1795
        real t1797
        real t1799
        real t180
        real t1801
        real t181
        real t1814
        real t1816
        real t1818
        real t1819
        real t1822
        real t1824
        real t1830
        real t1832
        real t1840
        real t1843
        real t1847
        real t1850
        real t1854
        real t1857
        real t1859
        real t1880
        real t1881
        real t1883
        real t1886
        real t1888
        real t1892
        real t1893
        real t19
        real t1905
        real t1911
        integer t1919
        real t1921
        real t193
        real t1933
        real t1941
        real t1945
        real t1949
        real t1950
        real t1957
        real t1958
        real t1960
        real t1963
        real t1965
        real t1969
        real t199
        real t2
        integer t20
        real t2002
        real t2010
        real t2011
        real t2013
        real t2016
        real t2018
        real t2022
        real t2023
        real t2049
        real t2060
        real t2064
        real t207
        real t2070
        real t2071
        real t2074
        integer t208
        real t2082
        real t209
        real t21
        real t210
        real t2102
        real t2103
        real t2107
        real t2118
        real t2119
        real t212
        real t2123
        real t2129
        real t2140
        real t2144
        real t215
        real t2154
        real t2158
        real t2161
        real t2165
        real t2167
        real t2169
        real t217
        real t2171
        real t2183
        real t2186
        real t2188
        real t2194
        real t2196
        real t22
        real t2206
        real t2209
        integer t221
        real t2212
        real t2214
        real t2215
        real t2217
        real t2219
        real t222
        real t2220
        real t2222
        real t2223
        real t2225
        real t2227
        real t2228
        real t223
        real t2230
        real t2233
        real t2237
        real t2240
        real t2242
        integer t2263
        real t2265
        real t2277
        real t2286
        real t2289
        real t2291
        real t23
        real t2312
        real t2315
        real t2317
        real t2321
        real t2322
        real t2323
        real t2330
        real t2331
        real t2333
        real t2336
        real t2338
        real t2342
        real t235
        real t2368
        real t2370
        real t2373
        real t2375
        real t2379
        real t241
        real t2412
        real t2420
        real t2439
        real t2440
        real t2441
        real t2444
        real t2452
        real t2473
        real t249
        real t2494
        real t2497
        real t25
        integer t250
        real t2501
        real t2504
        real t2505
        real t2512
        real t2513
        real t2516
        real t2518
        real t252
        real t2524
        real t2525
        real t2527
        real t2528
        real t2530
        real t2532
        real t2533
        real t2535
        real t2536
        real t2538
        real t254
        real t2540
        real t2541
        real t2542
        real t2545
        real t2547
        real t2549
        real t2551
        real t2552
        real t2556
        real t2558
        real t2560
        real t2562
        real t257
        real t2575
        real t2577
        real t2579
        real t2580
        real t2583
        real t2585
        real t259
        real t2591
        real t2593
        real t26
        real t2601
        real t2604
        real t2608
        real t2611
        real t2615
        real t2618
        real t2620
        real t263
        integer t2641
        real t2643
        real t265
        real t2655
        real t266
        real t2664
        real t2667
        real t2669
        real t268
        real t2690
        real t2694
        real t2698
        real t2699
        real t2706
        real t2714
        real t2722
        real t2724
        real t2727
        real t2729
        real t2733
        real t274
        real t2759
        real t2761
        real t2764
        real t2766
        real t277
        real t2770
        real t278
        real t2796
        real t2815
        real t2816
        real t2819
        real t2827
        real t284
        real t2848
        real t287
        real t2871
        real t2875
        real t2878
        real t2882
        real t2884
        real t2886
        real t2888
        real t289
        real t2900
        real t2903
        real t2905
        real t2911
        real t2913
        real t293
        real t294
        real t295
        real t30
        real t302
        real t303
        real t304
        real t305
        real t307
        real t310
        real t312
        real t316
        real t317
        real t32
        real t33
        real t34
        real t346
        real t35
        real t352
        real t355
        real t36
        real t361
        real t362
        real t364
        real t367
        real t369
        real t37
        real t373
        real t374
        real t39
        real t4
        real t40
        real t400
        real t403
        real t407
        real t413
        real t417
        real t42
        real t423
        real t424
        real t425
        real t428
        real t436
        real t437
        real t44
        real t441
        real t444
        real t447
        real t449
        integer t45
        real t451
        real t452
        real t456
        real t46
        real t461
        real t465
        real t468
        real t47
        real t470
        real t472
        real t473
        real t477
        real t482
        real t488
        real t49
        real t491
        real t493
        real t495
        real t5
        real t50
        real t503
        real t507
        integer t51
        real t510
        real t512
        real t514
        real t52
        real t522
        real t528
        real t53
        real t531
        real t535
        real t541
        real t545
        real t55
        real t553
        real t556
        real t560
        integer t563
        real t564
        real t565
        real t566
        real t568
        real t569
        real t57
        real t571
        real t575
        real t577
        real t578
        real t579
        integer t58
        real t585
        real t586
        real t587
        real t588
        real t59
        real t590
        real t591
        real t593
        real t594
        real t596
        real t597
        real t598
        real t599
        real t6
        real t60
        real t601
        real t602
        real t604
        real t608
        real t610
        real t611
        real t612
        real t614
        real t616
        real t617
        real t618
        real t62
        real t624
        real t625
        real t626
        real t627
        real t629
        real t63
        real t630
        real t632
        real t633
        real t635
        real t636
        real t637
        real t638
        integer t64
        real t640
        real t641
        real t643
        real t647
        real t649
        real t65
        real t650
        real t651
        real t653
        real t655
        real t656
        real t657
        real t66
        real t663
        real t664
        real t665
        real t666
        real t667
        real t668
        real t670
        real t671
        real t672
        real t679
        real t68
        real t681
        real t685
        real t687
        real t688
        real t689
        real t695
        real t696
        real t697
        real t698
        real t7
        real t70
        real t700
        real t701
        real t703
        real t704
        real t706
        real t707
        real t708
        real t709
        real t71
        real t711
        real t712
        real t714
        real t718
        real t72
        real t720
        real t721
        real t722
        real t724
        real t726
        real t727
        real t728
        real t734
        real t735
        real t736
        real t737
        real t739
        real t74
        real t740
        real t742
        real t743
        real t745
        real t746
        real t747
        real t748
        real t750
        real t751
        real t753
        real t757
        real t759
        real t76
        real t760
        real t761
        real t763
        real t765
        real t766
        real t767
        real t77
        real t773
        real t775
        real t776
        real t777
        real t779
        real t78
        real t780
        real t781
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
        real t8
        real t80
        real t800
        real t802
        real t808
        real t81
        real t810
        real t813
        real t815
        real t817
        real t818
        real t819
        real t82
        real t821
        real t822
        real t823
        real t825
        real t827
        real t828
        real t830
        real t831
        real t833
        real t835
        real t836
        real t837
        real t839
        real t84
        real t840
        real t841
        real t843
        real t845
        real t846
        real t848
        real t851
        real t853
        real t855
        real t856
        real t858
        real t859
        real t86
        real t861
        real t863
        real t864
        real t866
        real t867
        real t869
        real t87
        real t871
        real t872
        real t874
        real t875
        real t877
        real t879
        real t88
        real t880
        real t882
        real t885
        real t887
        real t888
        real t889
        real t891
        real t892
        real t893
        real t895
        real t897
        real t898
        real t899
        integer t9
        real t90
        real t901
        real t902
        real t903
        real t905
        real t907
        real t908
        real t909
        real t91
        real t912
        real t914
        real t916
        real t918
        real t919
        real t92
        real t921
        real t924
        real t925
        real t926
        real t927
        real t928
        real t930
        real t931
        real t932
        real t934
        real t935
        real t937
        real t939
        real t94
        real t944
        real t951
        real t953
        real t955
        real t957
        real t959
        real t96
        real t961
        real t962
        real t965
        real t967
        real t97
        real t973
        real t975
        real t983
        real t986
        real t99
        real t990
        real t993
        real t996
        real t997
        real t999
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
        t165 = dz ** 2
        t166 = k + 2
        t167 = u(t9,j,t166,n)
        t168 = t167 - t59
        t170 = t60 * t62
        t173 = t66 * t62
        t175 = (t170 - t173) * t62
        t179 = k - 2
        t180 = u(t9,j,t179,n)
        t181 = t65 - t180
        t193 = (t4 * t168 * t62 - t63) * t62
        t199 = (t68 - t4 * t181 * t62) * t62
        t207 = dy ** 2
        t208 = j + 2
        t209 = u(t9,t208,k,n)
        t210 = t209 - t46
        t212 = t47 * t49
        t215 = t53 * t49
        t217 = (t212 - t215) * t49
        t221 = j - 2
        t222 = u(t9,t221,k,n)
        t223 = t52 - t222
        t235 = (t4 * t210 * t49 - t50) * t49
        t241 = (t55 - t4 * t223 * t49) * t49
        t249 = dx ** 2
        t250 = i + 3
        t252 = u(t250,j,k,n) - t35
        t254 = t37 * t12
        t257 = t40 * t12
        t259 = (t254 - t257) * t12
        t263 = t72 * t12
        t265 = (t257 - t263) * t12
        t266 = t259 - t265
        t268 = t4 * t266 * t12
        t274 = (t4 * t252 * t12 - t39) * t12
        t277 = t44 - t76
        t278 = t277 * t12
        t284 = t44 + t57 + t70 - t165 * ((t4 * ((t168 * t62 - t170) * t6
     #2 - t175) * t62 - t4 * (t175 - (t173 - t181 * t62) * t62) * t62) *
     # t62 + ((t193 - t70) * t62 - (t70 - t199) * t62) * t62) / 0.24E2 -
     # t207 * ((t4 * ((t210 * t49 - t212) * t49 - t217) * t49 - t4 * (t2
     #17 - (t215 - t223 * t49) * t49) * t49) * t49 + ((t235 - t57) * t49
     # - (t57 - t241) * t49) * t49) / 0.24E2 - t249 * ((t4 * ((t252 * t1
     #2 - t254) * t12 - t259) * t12 - t268) * t12 + ((t274 - t44) * t12 
     #- t278) * t12) / 0.24E2
        t287 = t13 / 0.2E1
        t289 = ut(t250,j,k,n) - t15
        t293 = (t289 * t12 - t17) * t12 - t19
        t294 = t293 * t12
        t295 = t26 * t12
        t302 = dx * (t17 / 0.2E1 + t287 - t249 * (t294 / 0.2E1 + t295 / 
     #0.2E1) / 0.6E1) / 0.2E1
        t303 = t32 * t34
        t304 = ut(t9,j,t166,n)
        t305 = t304 - t121
        t307 = t122 * t62
        t310 = t126 * t62
        t312 = (t307 - t310) * t62
        t316 = ut(t9,j,t179,n)
        t317 = t125 - t316
        t346 = t4 * t26 * t12
        t352 = (t4 * t289 * t12 - t106) * t12
        t355 = t161 * t12
        t361 = ut(t9,t208,k,n)
        t362 = t361 - t111
        t364 = t112 * t49
        t367 = t116 * t49
        t369 = (t364 - t367) * t49
        t373 = ut(t9,t221,k,n)
        t374 = t115 - t373
        t400 = t110 + t120 + t130 - t165 * ((t4 * ((t305 * t62 - t307) *
     # t62 - t312) * t62 - t4 * (t312 - (t310 - t317 * t62) * t62) * t62
     #) * t62 + (((t4 * t305 * t62 - t124) * t62 - t130) * t62 - (t130 -
     # (t128 - t4 * t317 * t62) * t62) * t62) * t62) / 0.24E2 - t249 * (
     #(t4 * t293 * t12 - t346) * t12 + ((t352 - t110) * t12 - t355) * t1
     #2) / 0.24E2 - t207 * ((t4 * ((t362 * t49 - t364) * t49 - t369) * t
     #49 - t4 * (t369 - (t367 - t374 * t49) * t49) * t49) * t49 + (((t4 
     #* t362 * t49 - t114) * t49 - t120) * t49 - (t120 - (t118 - t4 * t3
     #74 * t49) * t49) * t49) * t49) / 0.24E2
        t403 = u(t14,t45,k,n)
        t407 = u(t14,t51,k,n)
        t413 = u(t14,j,t58,n)
        t417 = u(t14,j,t64,n)
        t423 = t274 + (t4 * (t403 - t35) * t49 - t4 * (t35 - t407) * t49
     #) * t49 + (t4 * (t413 - t35) * t62 - t4 * (t35 - t417) * t62) * t6
     #2 - t44 - t57 - t70
        t424 = t423 * t12
        t425 = t97 * t12
        t428 = dx * (t424 / 0.2E1 + t425 / 0.2E1)
        t436 = t249 * (t19 - dx * (t294 - t295) / 0.12E2) / 0.12E2
        t437 = t102 * t104
        t441 = t4 * t97 * t12
        t444 = t403 - t46
        t447 = t46 - t77
        t449 = t4 * t447 * t12
        t451 = (t4 * t444 * t12 - t449) * t12
        t452 = u(t9,t45,t58,n)
        t456 = u(t9,t45,t64,n)
        t461 = (t4 * (t452 - t46) * t62 - t4 * (t46 - t456) * t62) * t62
        t465 = t407 - t52
        t468 = t52 - t81
        t470 = t4 * t468 * t12
        t472 = (t4 * t465 * t12 - t470) * t12
        t473 = u(t9,t51,t58,n)
        t477 = u(t9,t51,t64,n)
        t482 = (t4 * (t473 - t52) * t62 - t4 * (t52 - t477) * t62) * t62
        t488 = t413 - t59
        t491 = t59 - t87
        t493 = t4 * t491 * t12
        t495 = (t4 * t488 * t12 - t493) * t12
        t503 = (t4 * (t452 - t59) * t49 - t4 * (t59 - t473) * t49) * t49
        t507 = t417 - t65
        t510 = t65 - t91
        t512 = t4 * t510 * t12
        t514 = (t4 * t507 * t12 - t512) * t12
        t522 = (t4 * (t456 - t65) * t49 - t4 * (t65 - t477) * t49) * t49
        t528 = (t4 * t423 * t12 - t441) * t12 + (t4 * (t451 + t235 + t46
     #1 - t44 - t57 - t70) * t49 - t4 * (t44 + t57 + t70 - t472 - t241 -
     # t482) * t49) * t49 + (t4 * (t495 + t503 + t193 - t44 - t57 - t70)
     # * t62 - t4 * (t44 + t57 + t70 - t514 - t522 - t199) * t62) * t62
        t531 = ut(t14,t45,k,n)
        t535 = ut(t14,t51,k,n)
        t541 = ut(t14,j,t58,n)
        t545 = ut(t14,j,t64,n)
        t553 = t155 * t12
        t556 = dx * ((t352 + (t4 * (t531 - t15) * t49 - t4 * (t15 - t535
     #) * t49) * t49 + (t4 * (t541 - t15) * t62 - t4 * (t15 - t545) * t6
     #2) * t62 - t110 - t120 - t130) * t12 / 0.2E1 + t553 / 0.2E1)
        t560 = dx * (t424 - t425)
        t563 = i - 2
        t564 = u(t563,j,k,n)
        t565 = t71 - t564
        t566 = t565 * t12
        t568 = (t263 - t566) * t12
        t569 = t265 - t568
        t571 = t4 * t569 * t12
        t575 = t4 * t565 * t12
        t577 = (t74 - t575) * t12
        t578 = t76 - t577
        t579 = t578 * t12
        t585 = u(i,t208,k,n)
        t586 = t585 - t77
        t587 = t586 * t49
        t588 = t78 * t49
        t590 = (t587 - t588) * t49
        t591 = t82 * t49
        t593 = (t588 - t591) * t49
        t594 = t590 - t593
        t596 = t4 * t594 * t49
        t597 = u(i,t221,k,n)
        t598 = t81 - t597
        t599 = t598 * t49
        t601 = (t591 - t599) * t49
        t602 = t593 - t601
        t604 = t4 * t602 * t49
        t608 = t4 * t586 * t49
        t610 = (t608 - t80) * t49
        t611 = t610 - t86
        t612 = t611 * t49
        t614 = t4 * t598 * t49
        t616 = (t84 - t614) * t49
        t617 = t86 - t616
        t618 = t617 * t49
        t624 = u(i,j,t166,n)
        t625 = t624 - t87
        t626 = t625 * t62
        t627 = t88 * t62
        t629 = (t626 - t627) * t62
        t630 = t92 * t62
        t632 = (t627 - t630) * t62
        t633 = t629 - t632
        t635 = t4 * t633 * t62
        t636 = u(i,j,t179,n)
        t637 = t91 - t636
        t638 = t637 * t62
        t640 = (t630 - t638) * t62
        t641 = t632 - t640
        t643 = t4 * t641 * t62
        t647 = t4 * t625 * t62
        t649 = (t647 - t90) * t62
        t650 = t649 - t96
        t651 = t650 * t62
        t653 = t4 * t637 * t62
        t655 = (t94 - t653) * t62
        t656 = t96 - t655
        t657 = t656 * t62
        t663 = t76 - t249 * ((t268 - t571) * t12 + (t278 - t579) * t12) 
     #/ 0.24E2 + t86 - t207 * ((t596 - t604) * t49 + (t612 - t618) * t49
     #) / 0.24E2 + t96 - t165 * ((t635 - t643) * t62 + (t651 - t657) * t
     #62) / 0.24E2
        t664 = t160 * t663
        t665 = t23 / 0.2E1
        t666 = ut(t563,j,k,n)
        t667 = t21 - t666
        t668 = t667 * t12
        t670 = (t23 - t668) * t12
        t671 = t25 - t670
        t672 = t671 * t12
        t679 = dx * (t287 + t665 - t249 * (t295 / 0.2E1 + t672 / 0.2E1) 
     #/ 0.6E1) / 0.2E1
        t681 = t4 * t671 * t12
        t685 = t4 * t667 * t12
        t687 = (t132 - t685) * t12
        t688 = t134 - t687
        t689 = t688 * t12
        t695 = ut(i,t208,k,n)
        t696 = t695 - t135
        t697 = t696 * t49
        t698 = t136 * t49
        t700 = (t697 - t698) * t49
        t701 = t140 * t49
        t703 = (t698 - t701) * t49
        t704 = t700 - t703
        t706 = t4 * t704 * t49
        t707 = ut(i,t221,k,n)
        t708 = t139 - t707
        t709 = t708 * t49
        t711 = (t701 - t709) * t49
        t712 = t703 - t711
        t714 = t4 * t712 * t49
        t718 = t4 * t696 * t49
        t720 = (t718 - t138) * t49
        t721 = t720 - t144
        t722 = t721 * t49
        t724 = t4 * t708 * t49
        t726 = (t142 - t724) * t49
        t727 = t144 - t726
        t728 = t727 * t49
        t734 = ut(i,j,t166,n)
        t735 = t734 - t145
        t736 = t735 * t62
        t737 = t146 * t62
        t739 = (t736 - t737) * t62
        t740 = t150 * t62
        t742 = (t737 - t740) * t62
        t743 = t739 - t742
        t745 = t4 * t743 * t62
        t746 = ut(i,j,t179,n)
        t747 = t149 - t746
        t748 = t747 * t62
        t750 = (t740 - t748) * t62
        t751 = t742 - t750
        t753 = t4 * t751 * t62
        t757 = t4 * t735 * t62
        t759 = (t757 - t148) * t62
        t760 = t759 - t154
        t761 = t760 * t62
        t763 = t4 * t747 * t62
        t765 = (t152 - t763) * t62
        t766 = t154 - t765
        t767 = t766 * t62
        t773 = t134 + t144 + t154 - t249 * ((t346 - t681) * t12 + (t355 
     #- t689) * t12) / 0.24E2 - t207 * ((t706 - t714) * t49 + (t722 - t7
     #28) * t49) / 0.24E2 - t165 * ((t745 - t753) * t62 + (t761 - t767) 
     #* t62) / 0.24E2
        t775 = t303 * t773 / 0.2E1
        t776 = u(t20,t45,k,n)
        t777 = t776 - t71
        t779 = t4 * t777 * t49
        t780 = u(t20,t51,k,n)
        t781 = t71 - t780
        t783 = t4 * t781 * t49
        t785 = (t779 - t783) * t49
        t786 = u(t20,j,t58,n)
        t787 = t786 - t71
        t789 = t4 * t787 * t62
        t790 = u(t20,j,t64,n)
        t791 = t71 - t790
        t793 = t4 * t791 * t62
        t795 = (t789 - t793) * t62
        t796 = t76 + t86 + t96 - t577 - t785 - t795
        t797 = t796 * t12
        t800 = dx * (t425 / 0.2E1 + t797 / 0.2E1)
        t802 = t160 * t800 / 0.2E1
        t808 = t249 * (t25 - dx * (t295 - t672) / 0.12E2) / 0.12E2
        t810 = t4 * t796 * t12
        t813 = t77 - t776
        t815 = t4 * t813 * t12
        t817 = (t449 - t815) * t12
        t818 = u(i,t45,t58,n)
        t819 = t818 - t77
        t821 = t4 * t819 * t62
        t822 = u(i,t45,t64,n)
        t823 = t77 - t822
        t825 = t4 * t823 * t62
        t827 = (t821 - t825) * t62
        t828 = t817 + t610 + t827 - t76 - t86 - t96
        t830 = t4 * t828 * t49
        t831 = t81 - t780
        t833 = t4 * t831 * t12
        t835 = (t470 - t833) * t12
        t836 = u(i,t51,t58,n)
        t837 = t836 - t81
        t839 = t4 * t837 * t62
        t840 = u(i,t51,t64,n)
        t841 = t81 - t840
        t843 = t4 * t841 * t62
        t845 = (t839 - t843) * t62
        t846 = t76 + t86 + t96 - t835 - t616 - t845
        t848 = t4 * t846 * t49
        t851 = t87 - t786
        t853 = t4 * t851 * t12
        t855 = (t493 - t853) * t12
        t856 = t818 - t87
        t858 = t4 * t856 * t49
        t859 = t87 - t836
        t861 = t4 * t859 * t49
        t863 = (t858 - t861) * t49
        t864 = t855 + t863 + t649 - t76 - t86 - t96
        t866 = t4 * t864 * t62
        t867 = t91 - t790
        t869 = t4 * t867 * t12
        t871 = (t512 - t869) * t12
        t872 = t822 - t91
        t874 = t4 * t872 * t49
        t875 = t91 - t840
        t877 = t4 * t875 * t49
        t879 = (t874 - t877) * t49
        t880 = t76 + t86 + t96 - t871 - t879 - t655
        t882 = t4 * t880 * t62
        t885 = (t441 - t810) * t12 + (t830 - t848) * t49 + (t866 - t882)
     # * t62
        t887 = t437 * t885 / 0.6E1
        t888 = ut(t20,t45,k,n)
        t889 = t888 - t21
        t891 = t4 * t889 * t49
        t892 = ut(t20,t51,k,n)
        t893 = t21 - t892
        t895 = t4 * t893 * t49
        t897 = (t891 - t895) * t49
        t898 = ut(t20,j,t58,n)
        t899 = t898 - t21
        t901 = t4 * t899 * t62
        t902 = ut(t20,j,t64,n)
        t903 = t21 - t902
        t905 = t4 * t903 * t62
        t907 = (t901 - t905) * t62
        t908 = t134 + t144 + t154 - t687 - t897 - t907
        t909 = t908 * t12
        t912 = dx * (t553 / 0.2E1 + t909 / 0.2E1)
        t914 = t303 * t912 / 0.4E1
        t916 = dx * (t425 - t797)
        t918 = t160 * t916 / 0.12E2
        t919 = t10 + t160 * t284 - t302 + t303 * t400 / 0.2E1 - t160 * t
     #428 / 0.2E1 + t436 + t437 * t528 / 0.6E1 - t303 * t556 / 0.4E1 + t
     #160 * t560 / 0.12E2 - t2 - t664 - t679 - t775 - t802 - t808 - t887
     # - t914 - t918
        t921 = sqrt(0.16E2)
        t924 = 0.1E1 / 0.2E1 - t6
        t925 = t4 * t924
        t926 = t925 * t30
        t927 = t924 ** 2
        t928 = t4 * t927
        t930 = t928 * t99 / 0.2E1
        t931 = t927 * t924
        t932 = t4 * t931
        t934 = t932 * t157 / 0.6E1
        t935 = t924 * dt
        t937 = t935 * t162 / 0.24E2
        t939 = t927 * t34
        t944 = t931 * t104
        t951 = t935 * t663
        t953 = t939 * t773 / 0.2E1
        t955 = t935 * t800 / 0.2E1
        t957 = t944 * t885 / 0.6E1
        t959 = t939 * t912 / 0.4E1
        t961 = t935 * t916 / 0.12E2
        t962 = t10 + t935 * t284 - t302 + t939 * t400 / 0.2E1 - t935 * t
     #428 / 0.2E1 + t436 + t944 * t528 / 0.6E1 - t939 * t556 / 0.4E1 + t
     #935 * t560 / 0.12E2 - t2 - t951 - t679 - t953 - t955 - t808 - t957
     # - t959 - t961
        t965 = cc * t962 * t921 / 0.8E1
        t967 = (t8 * t30 + t33 * t99 / 0.2E1 + t103 * t157 / 0.6E1 - t16
     #0 * t162 / 0.24E2 + cc * t919 * t921 / 0.8E1 - t926 - t930 - t934 
     #+ t937 - t965) * t5
        t973 = t4 * (t257 - dx * t266 / 0.24E2)
        t975 = dx * t277 / 0.24E2
        t983 = dt * (t23 - dx * t671 / 0.24E2)
        t986 = t34 * t796 * t12
        t990 = t104 * t908 * t12
        t993 = dx * t688
        t996 = u(t20,t208,k,n)
        t997 = t996 - t776
        t999 = t777 * t49
        t1002 = t781 * t49
        t1004 = (t999 - t1002) * t49
        t1008 = u(t20,t221,k,n)
        t1009 = t780 - t1008
        t1021 = (t4 * t997 * t49 - t779) * t49
        t1027 = (t783 - t4 * t1009 * t49) * t49
        t1035 = u(t20,j,t166,n)
        t1036 = t1035 - t786
        t1038 = t787 * t62
        t1041 = t791 * t62
        t1043 = (t1038 - t1041) * t62
        t1047 = u(t20,j,t179,n)
        t1048 = t790 - t1047
        t1060 = (t4 * t1036 * t62 - t789) * t62
        t1066 = (t793 - t4 * t1048 * t62) * t62
        t1074 = i - 3
        t1076 = t564 - u(t1074,j,k,n)
        t1088 = (t575 - t4 * t1076 * t12) * t12
        t1096 = -t207 * ((t4 * ((t997 * t49 - t999) * t49 - t1004) * t49
     # - t4 * (t1004 - (t1002 - t1009 * t49) * t49) * t49) * t49 + ((t10
     #21 - t785) * t49 - (t785 - t1027) * t49) * t49) / 0.24E2 - t165 * 
     #((t4 * ((t1036 * t62 - t1038) * t62 - t1043) * t62 - t4 * (t1043 -
     # (t1041 - t1048 * t62) * t62) * t62) * t62 + ((t1060 - t795) * t62
     # - (t795 - t1066) * t62) * t62) / 0.24E2 - t249 * ((t571 - t4 * (t
     #568 - (t566 - t1076 * t12) * t12) * t12) * t12 + (t579 - (t577 - t
     #1088) * t12) * t12) / 0.24E2 + t577 + t785 + t795
        t1100 = t666 - ut(t1074,j,k,n)
        t1104 = t670 - (t668 - t1100 * t12) * t12
        t1105 = t1104 * t12
        t1112 = dx * (t665 + t668 / 0.2E1 - t249 * (t672 / 0.2E1 + t1105
     # / 0.2E1) / 0.6E1) / 0.2E1
        t1120 = (t685 - t4 * t1100 * t12) * t12
        t1128 = ut(t20,t208,k,n)
        t1129 = t1128 - t888
        t1131 = t889 * t49
        t1134 = t893 * t49
        t1136 = (t1131 - t1134) * t49
        t1140 = ut(t20,t221,k,n)
        t1141 = t892 - t1140
        t1167 = ut(t20,j,t166,n)
        t1168 = t1167 - t898
        t1170 = t899 * t62
        t1173 = t903 * t62
        t1175 = (t1170 - t1173) * t62
        t1179 = ut(t20,j,t179,n)
        t1180 = t902 - t1179
        t1206 = -t249 * ((t681 - t4 * t1104 * t12) * t12 + (t689 - (t687
     # - t1120) * t12) * t12) / 0.24E2 - t207 * ((t4 * ((t1129 * t49 - t
     #1131) * t49 - t1136) * t49 - t4 * (t1136 - (t1134 - t1141 * t49) *
     # t49) * t49) * t49 + (((t4 * t1129 * t49 - t891) * t49 - t897) * t
     #49 - (t897 - (t895 - t4 * t1141 * t49) * t49) * t49) * t49) / 0.24
     #E2 - t165 * ((t4 * ((t1168 * t62 - t1170) * t62 - t1175) * t62 - t
     #4 * (t1175 - (t1173 - t1180 * t62) * t62) * t62) * t62 + (((t4 * t
     #1168 * t62 - t901) * t62 - t907) * t62 - (t907 - (t905 - t4 * t118
     #0 * t62) * t62) * t62) * t62) / 0.24E2 + t687 + t897 + t907
        t1209 = u(t563,t45,k,n)
        t1213 = u(t563,t51,k,n)
        t1219 = u(t563,j,t58,n)
        t1223 = u(t563,j,t64,n)
        t1229 = t577 + t785 + t795 - t1088 - (t4 * (t1209 - t564) * t49 
     #- t4 * (t564 - t1213) * t49) * t49 - (t4 * (t1219 - t564) * t62 - 
     #t4 * (t564 - t1223) * t62) * t62
        t1230 = t1229 * t12
        t1233 = dx * (t797 / 0.2E1 + t1230 / 0.2E1)
        t1241 = t249 * (t670 - dx * (t672 - t1105) / 0.12E2) / 0.12E2
        t1246 = t776 - t1209
        t1250 = (t815 - t4 * t1246 * t12) * t12
        t1251 = u(t20,t45,t58,n)
        t1255 = u(t20,t45,t64,n)
        t1260 = (t4 * (t1251 - t776) * t62 - t4 * (t776 - t1255) * t62) 
     #* t62
        t1264 = t780 - t1213
        t1268 = (t833 - t4 * t1264 * t12) * t12
        t1269 = u(t20,t51,t58,n)
        t1273 = u(t20,t51,t64,n)
        t1278 = (t4 * (t1269 - t780) * t62 - t4 * (t780 - t1273) * t62) 
     #* t62
        t1284 = t786 - t1219
        t1288 = (t853 - t4 * t1284 * t12) * t12
        t1296 = (t4 * (t1251 - t786) * t49 - t4 * (t786 - t1269) * t49) 
     #* t49
        t1300 = t790 - t1223
        t1304 = (t869 - t4 * t1300 * t12) * t12
        t1312 = (t4 * (t1255 - t790) * t49 - t4 * (t790 - t1273) * t49) 
     #* t49
        t1318 = (t810 - t4 * t1229 * t12) * t12 + (t4 * (t1250 + t1021 +
     # t1260 - t577 - t785 - t795) * t49 - t4 * (t577 + t785 + t795 - t1
     #268 - t1027 - t1278) * t49) * t49 + (t4 * (t1288 + t1296 + t1060 -
     # t577 - t785 - t795) * t62 - t4 * (t577 + t785 + t795 - t1304 - t1
     #312 - t1066) * t62) * t62
        t1321 = ut(t563,t45,k,n)
        t1325 = ut(t563,t51,k,n)
        t1331 = ut(t563,j,t58,n)
        t1335 = ut(t563,j,t64,n)
        t1345 = dx * (t909 / 0.2E1 + (t687 + t897 + t907 - t1120 - (t4 *
     # (t1321 - t666) * t49 - t4 * (t666 - t1325) * t49) * t49 - (t4 * (
     #t1331 - t666) * t62 - t4 * (t666 - t1335) * t62) * t62) * t12 / 0.
     #2E1)
        t1349 = dx * (t797 - t1230)
        t1352 = t2 + t664 - t679 + t775 - t802 + t808 + t887 - t914 + t9
     #18 - t21 - t160 * t1096 - t1112 - t303 * t1206 / 0.2E1 - t160 * t1
     #233 / 0.2E1 - t1241 - t437 * t1318 / 0.6E1 - t303 * t1345 / 0.4E1 
     #- t160 * t1349 / 0.12E2
        t1356 = t925 * t983
        t1358 = t928 * t986 / 0.2E1
        t1360 = t932 * t990 / 0.6E1
        t1362 = t935 * t993 / 0.24E2
        t1374 = t2 + t951 - t679 + t953 - t955 + t808 + t957 - t959 + t9
     #61 - t21 - t935 * t1096 - t1112 - t939 * t1206 / 0.2E1 - t935 * t1
     #233 / 0.2E1 - t1241 - t944 * t1318 / 0.6E1 - t939 * t1345 / 0.4E1 
     #- t935 * t1349 / 0.12E2
        t1377 = cc * t1374 * t921 / 0.8E1
        t1379 = (t8 * t983 + t33 * t986 / 0.2E1 + t103 * t990 / 0.6E1 - 
     #t160 * t993 / 0.24E2 + cc * t1352 * t921 / 0.8E1 - t1356 - t1358 -
     # t1360 + t1362 - t1377) * t5
        t1385 = t4 * (t263 - dx * t569 / 0.24E2)
        t1387 = dx * t578 / 0.24E2
        t1397 = dt * (t698 - dy * t704 / 0.24E2)
        t1400 = t34 * t828 * t49
        t1403 = t111 - t135
        t1405 = t4 * t1403 * t12
        t1406 = t135 - t888
        t1408 = t4 * t1406 * t12
        t1410 = (t1405 - t1408) * t12
        t1411 = ut(i,t45,t58,n)
        t1412 = t1411 - t135
        t1414 = t4 * t1412 * t62
        t1415 = ut(i,t45,t64,n)
        t1416 = t135 - t1415
        t1418 = t4 * t1416 * t62
        t1420 = (t1414 - t1418) * t62
        t1421 = t1410 + t720 + t1420 - t134 - t144 - t154
        t1423 = t104 * t1421 * t49
        t1426 = dy * t721
        t1429 = u(i,t45,t166,n)
        t1430 = t1429 - t818
        t1432 = t819 * t62
        t1435 = t823 * t62
        t1437 = (t1432 - t1435) * t62
        t1441 = u(i,t45,t179,n)
        t1442 = t822 - t1441
        t1454 = (t4 * t1430 * t62 - t821) * t62
        t1460 = (t825 - t4 * t1442 * t62) * t62
        t1469 = t447 * t12
        t1472 = t813 * t12
        t1474 = (t1469 - t1472) * t12
        t1495 = j + 3
        t1497 = u(i,t1495,k,n) - t585
        t1509 = (t4 * t1497 * t49 - t608) * t49
        t1517 = -t165 * ((t4 * ((t1430 * t62 - t1432) * t62 - t1437) * t
     #62 - t4 * (t1437 - (t1435 - t1442 * t62) * t62) * t62) * t62 + ((t
     #1454 - t827) * t62 - (t827 - t1460) * t62) * t62) / 0.24E2 - t249 
     #* ((t4 * ((t444 * t12 - t1469) * t12 - t1474) * t12 - t4 * (t1474 
     #- (t1472 - t1246 * t12) * t12) * t12) * t12 + ((t451 - t817) * t12
     # - (t817 - t1250) * t12) * t12) / 0.24E2 + t817 + t610 + t827 - t2
     #07 * ((t4 * ((t1497 * t49 - t587) * t49 - t590) * t49 - t596) * t4
     #9 + ((t1509 - t610) * t49 - t612) * t49) / 0.24E2
        t1520 = t698 / 0.2E1
        t1522 = ut(i,t1495,k,n) - t695
        t1526 = (t1522 * t49 - t697) * t49 - t700
        t1527 = t1526 * t49
        t1528 = t704 * t49
        t1535 = dy * (t697 / 0.2E1 + t1520 - t207 * (t1527 / 0.2E1 + t15
     #28 / 0.2E1) / 0.6E1) / 0.2E1
        t1536 = ut(i,t45,t166,n)
        t1537 = t1536 - t1411
        t1539 = t1412 * t62
        t1542 = t1416 * t62
        t1544 = (t1539 - t1542) * t62
        t1548 = ut(i,t45,t179,n)
        t1549 = t1415 - t1548
        t1575 = t531 - t111
        t1577 = t1403 * t12
        t1580 = t1406 * t12
        t1582 = (t1577 - t1580) * t12
        t1586 = t888 - t1321
        t1619 = (t4 * t1522 * t49 - t718) * t49
        t1627 = -t165 * ((t4 * ((t1537 * t62 - t1539) * t62 - t1544) * t
     #62 - t4 * (t1544 - (t1542 - t1549 * t62) * t62) * t62) * t62 + (((
     #t4 * t1537 * t62 - t1414) * t62 - t1420) * t62 - (t1420 - (t1418 -
     # t4 * t1549 * t62) * t62) * t62) * t62) / 0.24E2 - t249 * ((t4 * (
     #(t1575 * t12 - t1577) * t12 - t1582) * t12 - t4 * (t1582 - (t1580 
     #- t1586 * t12) * t12) * t12) * t12 + (((t4 * t1575 * t12 - t1405) 
     #* t12 - t1410) * t12 - (t1410 - (t1408 - t4 * t1586 * t12) * t12) 
     #* t12) * t12) / 0.24E2 - t207 * ((t4 * t1526 * t49 - t706) * t49 +
     # ((t1619 - t720) * t49 - t722) * t49) / 0.24E2 + t1410 + t1420 + t
     #720
        t1638 = u(i,t208,t58,n)
        t1642 = u(i,t208,t64,n)
        t1648 = (t4 * (t209 - t585) * t12 - t4 * (t585 - t996) * t12) * 
     #t12 + t1509 + (t4 * (t1638 - t585) * t62 - t4 * (t585 - t1642) * t
     #62) * t62 - t817 - t610 - t827
        t1649 = t1648 * t49
        t1650 = t828 * t49
        t1653 = dy * (t1649 / 0.2E1 + t1650 / 0.2E1)
        t1661 = t207 * (t700 - dy * (t1527 - t1528) / 0.12E2) / 0.12E2
        t1681 = (t4 * (t452 - t818) * t12 - t4 * (t818 - t1251) * t12) *
     # t12
        t1682 = t1638 - t818
        t1686 = (t4 * t1682 * t49 - t858) * t49
        t1697 = (t4 * (t456 - t822) * t12 - t4 * (t822 - t1255) * t12) *
     # t12
        t1698 = t1642 - t822
        t1702 = (t4 * t1698 * t49 - t874) * t49
        t1708 = (t4 * (t451 + t235 + t461 - t817 - t610 - t827) * t12 - 
     #t4 * (t817 + t610 + t827 - t1250 - t1021 - t1260) * t12) * t12 + (
     #t4 * t1648 * t49 - t830) * t49 + (t4 * (t1681 + t1686 + t1454 - t8
     #17 - t610 - t827) * t62 - t4 * (t817 + t610 + t827 - t1697 - t1702
     # - t1460) * t62) * t62
        t1719 = ut(i,t208,t58,n)
        t1723 = ut(i,t208,t64,n)
        t1731 = t1421 * t49
        t1734 = dy * (((t4 * (t361 - t695) * t12 - t4 * (t695 - t1128) *
     # t12) * t12 + t1619 + (t4 * (t1719 - t695) * t62 - t4 * (t695 - t1
     #723) * t62) * t62 - t1410 - t720 - t1420) * t49 / 0.2E1 + t1731 / 
     #0.2E1)
        t1738 = dy * (t1649 - t1650)
        t1741 = t701 / 0.2E1
        t1742 = t712 * t49
        t1749 = dy * (t1520 + t1741 - t207 * (t1528 / 0.2E1 + t1742 / 0.
     #2E1) / 0.6E1) / 0.2E1
        t1750 = t846 * t49
        t1753 = dy * (t1650 / 0.2E1 + t1750 / 0.2E1)
        t1755 = t160 * t1753 / 0.2E1
        t1761 = t207 * (t703 - dy * (t1528 - t1742) / 0.12E2) / 0.12E2
        t1762 = t115 - t139
        t1764 = t4 * t1762 * t12
        t1765 = t139 - t892
        t1767 = t4 * t1765 * t12
        t1769 = (t1764 - t1767) * t12
        t1770 = ut(i,t51,t58,n)
        t1771 = t1770 - t139
        t1773 = t4 * t1771 * t62
        t1774 = ut(i,t51,t64,n)
        t1775 = t139 - t1774
        t1777 = t4 * t1775 * t62
        t1779 = (t1773 - t1777) * t62
        t1780 = t134 + t144 + t154 - t1769 - t726 - t1779
        t1781 = t1780 * t49
        t1784 = dy * (t1731 / 0.2E1 + t1781 / 0.2E1)
        t1786 = t303 * t1784 / 0.4E1
        t1788 = dy * (t1650 - t1750)
        t1790 = t160 * t1788 / 0.12E2
        t1791 = t135 + t160 * t1517 - t1535 + t303 * t1627 / 0.2E1 - t16
     #0 * t1653 / 0.2E1 + t1661 + t437 * t1708 / 0.6E1 - t303 * t1734 / 
     #0.4E1 + t160 * t1738 / 0.12E2 - t2 - t664 - t1749 - t775 - t1755 -
     # t1761 - t887 - t1786 - t1790
        t1795 = t925 * t1397
        t1797 = t928 * t1400 / 0.2E1
        t1799 = t932 * t1423 / 0.6E1
        t1801 = t935 * t1426 / 0.24E2
        t1814 = t935 * t1753 / 0.2E1
        t1816 = t939 * t1784 / 0.4E1
        t1818 = t935 * t1788 / 0.12E2
        t1819 = t135 + t935 * t1517 - t1535 + t939 * t1627 / 0.2E1 - t93
     #5 * t1653 / 0.2E1 + t1661 + t944 * t1708 / 0.6E1 - t939 * t1734 / 
     #0.4E1 + t935 * t1738 / 0.12E2 - t2 - t951 - t1749 - t953 - t1814 -
     # t1761 - t957 - t1816 - t1818
        t1822 = cc * t1819 * t921 / 0.8E1
        t1824 = (t8 * t1397 + t33 * t1400 / 0.2E1 + t103 * t1423 / 0.6E1
     # - t160 * t1426 / 0.24E2 + cc * t1791 * t921 / 0.8E1 - t1795 - t17
     #97 - t1799 + t1801 - t1822) * t5
        t1830 = t4 * (t588 - dy * t594 / 0.24E2)
        t1832 = dy * t611 / 0.24E2
        t1840 = dt * (t701 - dy * t712 / 0.24E2)
        t1843 = t34 * t846 * t49
        t1847 = t104 * t1780 * t49
        t1850 = dy * t727
        t1854 = t468 * t12
        t1857 = t831 * t12
        t1859 = (t1854 - t1857) * t12
        t1880 = u(i,t51,t166,n)
        t1881 = t1880 - t836
        t1883 = t837 * t62
        t1886 = t841 * t62
        t1888 = (t1883 - t1886) * t62
        t1892 = u(i,t51,t179,n)
        t1893 = t840 - t1892
        t1905 = (t4 * t1881 * t62 - t839) * t62
        t1911 = (t843 - t4 * t1893 * t62) * t62
        t1919 = j - 3
        t1921 = t597 - u(i,t1919,k,n)
        t1933 = (t614 - t4 * t1921 * t49) * t49
        t1941 = -t249 * ((t4 * ((t465 * t12 - t1854) * t12 - t1859) * t1
     #2 - t4 * (t1859 - (t1857 - t1264 * t12) * t12) * t12) * t12 + ((t4
     #72 - t835) * t12 - (t835 - t1268) * t12) * t12) / 0.24E2 - t165 * 
     #((t4 * ((t1881 * t62 - t1883) * t62 - t1888) * t62 - t4 * (t1888 -
     # (t1886 - t1893 * t62) * t62) * t62) * t62 + ((t1905 - t845) * t62
     # - (t845 - t1911) * t62) * t62) / 0.24E2 - t207 * ((t604 - t4 * (t
     #601 - (t599 - t1921 * t49) * t49) * t49) * t49 + (t618 - (t616 - t
     #1933) * t49) * t49) / 0.24E2 + t835 + t616 + t845
        t1945 = t707 - ut(i,t1919,k,n)
        t1949 = t711 - (t709 - t1945 * t49) * t49
        t1950 = t1949 * t49
        t1957 = dy * (t1741 + t709 / 0.2E1 - t207 * (t1742 / 0.2E1 + t19
     #50 / 0.2E1) / 0.6E1) / 0.2E1
        t1958 = t535 - t115
        t1960 = t1762 * t12
        t1963 = t1765 * t12
        t1965 = (t1960 - t1963) * t12
        t1969 = t892 - t1325
        t2002 = (t724 - t4 * t1945 * t49) * t49
        t2010 = ut(i,t51,t166,n)
        t2011 = t2010 - t1770
        t2013 = t1771 * t62
        t2016 = t1775 * t62
        t2018 = (t2013 - t2016) * t62
        t2022 = ut(i,t51,t179,n)
        t2023 = t1774 - t2022
        t2049 = -t249 * ((t4 * ((t1958 * t12 - t1960) * t12 - t1965) * t
     #12 - t4 * (t1965 - (t1963 - t1969 * t12) * t12) * t12) * t12 + (((
     #t4 * t1958 * t12 - t1764) * t12 - t1769) * t12 - (t1769 - (t1767 -
     # t4 * t1969 * t12) * t12) * t12) * t12) / 0.24E2 - t207 * ((t714 -
     # t4 * t1949 * t49) * t49 + (t728 - (t726 - t2002) * t49) * t49) / 
     #0.24E2 - t165 * ((t4 * ((t2011 * t62 - t2013) * t62 - t2018) * t62
     # - t4 * (t2018 - (t2016 - t2023 * t62) * t62) * t62) * t62 + (((t4
     # * t2011 * t62 - t1773) * t62 - t1779) * t62 - (t1779 - (t1777 - t
     #4 * t2023 * t62) * t62) * t62) * t62) / 0.24E2 + t1769 + t1779 + t
     #726
        t2060 = u(i,t221,t58,n)
        t2064 = u(i,t221,t64,n)
        t2070 = t835 + t616 + t845 - (t4 * (t222 - t597) * t12 - t4 * (t
     #597 - t1008) * t12) * t12 - t1933 - (t4 * (t2060 - t597) * t62 - t
     #4 * (t597 - t2064) * t62) * t62
        t2071 = t2070 * t49
        t2074 = dy * (t1750 / 0.2E1 + t2071 / 0.2E1)
        t2082 = t207 * (t711 - dy * (t1742 - t1950) / 0.12E2) / 0.12E2
        t2102 = (t4 * (t473 - t836) * t12 - t4 * (t836 - t1269) * t12) *
     # t12
        t2103 = t836 - t2060
        t2107 = (t861 - t4 * t2103 * t49) * t49
        t2118 = (t4 * (t477 - t840) * t12 - t4 * (t840 - t1273) * t12) *
     # t12
        t2119 = t840 - t2064
        t2123 = (t877 - t4 * t2119 * t49) * t49
        t2129 = (t4 * (t472 + t241 + t482 - t835 - t616 - t845) * t12 - 
     #t4 * (t835 + t616 + t845 - t1268 - t1027 - t1278) * t12) * t12 + (
     #t848 - t4 * t2070 * t49) * t49 + (t4 * (t2102 + t2107 + t1905 - t8
     #35 - t616 - t845) * t62 - t4 * (t835 + t616 + t845 - t2118 - t2123
     # - t1911) * t62) * t62
        t2140 = ut(i,t221,t58,n)
        t2144 = ut(i,t221,t64,n)
        t2154 = dy * (t1781 / 0.2E1 + (t1769 + t726 + t1779 - (t4 * (t37
     #3 - t707) * t12 - t4 * (t707 - t1140) * t12) * t12 - t2002 - (t4 *
     # (t2140 - t707) * t62 - t4 * (t707 - t2144) * t62) * t62) * t49 / 
     #0.2E1)
        t2158 = dy * (t1750 - t2071)
        t2161 = t2 + t664 - t1749 + t775 - t1755 + t1761 + t887 - t1786 
     #+ t1790 - t139 - t160 * t1941 - t1957 - t303 * t2049 / 0.2E1 - t16
     #0 * t2074 / 0.2E1 - t2082 - t437 * t2129 / 0.6E1 - t303 * t2154 / 
     #0.4E1 - t160 * t2158 / 0.12E2
        t2165 = t925 * t1840
        t2167 = t928 * t1843 / 0.2E1
        t2169 = t932 * t1847 / 0.6E1
        t2171 = t935 * t1850 / 0.24E2
        t2183 = t2 + t951 - t1749 + t953 - t1814 + t1761 + t957 - t1816 
     #+ t1818 - t139 - t935 * t1941 - t1957 - t939 * t2049 / 0.2E1 - t93
     #5 * t2074 / 0.2E1 - t2082 - t944 * t2129 / 0.6E1 - t939 * t2154 / 
     #0.4E1 - t935 * t2158 / 0.12E2
        t2186 = cc * t2183 * t921 / 0.8E1
        t2188 = (t8 * t1840 + t33 * t1843 / 0.2E1 + t103 * t1847 / 0.6E1
     # - t160 * t1850 / 0.24E2 + cc * t2161 * t921 / 0.8E1 - t2165 - t21
     #67 - t2169 + t2171 - t2186) * t5
        t2194 = t4 * (t591 - dy * t602 / 0.24E2)
        t2196 = dy * t617 / 0.24E2
        t2206 = dt * (t737 - dz * t743 / 0.24E2)
        t2209 = t34 * t864 * t62
        t2212 = t121 - t145
        t2214 = t4 * t2212 * t12
        t2215 = t145 - t898
        t2217 = t4 * t2215 * t12
        t2219 = (t2214 - t2217) * t12
        t2220 = t1411 - t145
        t2222 = t4 * t2220 * t49
        t2223 = t145 - t1770
        t2225 = t4 * t2223 * t49
        t2227 = (t2222 - t2225) * t49
        t2228 = t2219 + t2227 + t759 - t134 - t144 - t154
        t2230 = t104 * t2228 * t62
        t2233 = dz * t760
        t2237 = t491 * t12
        t2240 = t851 * t12
        t2242 = (t2237 - t2240) * t12
        t2263 = k + 3
        t2265 = u(i,j,t2263,n) - t624
        t2277 = (t4 * t2265 * t62 - t647) * t62
        t2286 = t856 * t49
        t2289 = t859 * t49
        t2291 = (t2286 - t2289) * t49
        t2312 = t855 + t863 + t649 - t249 * ((t4 * ((t488 * t12 - t2237)
     # * t12 - t2242) * t12 - t4 * (t2242 - (t2240 - t1284 * t12) * t12)
     # * t12) * t12 + ((t495 - t855) * t12 - (t855 - t1288) * t12) * t12
     #) / 0.24E2 - t165 * ((t4 * ((t2265 * t62 - t626) * t62 - t629) * t
     #62 - t635) * t62 + ((t2277 - t649) * t62 - t651) * t62) / 0.24E2 -
     # t207 * ((t4 * ((t1682 * t49 - t2286) * t49 - t2291) * t49 - t4 * 
     #(t2291 - (t2289 - t2103 * t49) * t49) * t49) * t49 + ((t1686 - t86
     #3) * t49 - (t863 - t2107) * t49) * t49) / 0.24E2
        t2315 = t737 / 0.2E1
        t2317 = ut(i,j,t2263,n) - t734
        t2321 = (t2317 * t62 - t736) * t62 - t739
        t2322 = t2321 * t62
        t2323 = t743 * t62
        t2330 = dz * (t736 / 0.2E1 + t2315 - t165 * (t2322 / 0.2E1 + t23
     #23 / 0.2E1) / 0.6E1) / 0.2E1
        t2331 = t541 - t121
        t2333 = t2212 * t12
        t2336 = t2215 * t12
        t2338 = (t2333 - t2336) * t12
        t2342 = t898 - t1331
        t2368 = t1719 - t1411
        t2370 = t2220 * t49
        t2373 = t2223 * t49
        t2375 = (t2370 - t2373) * t49
        t2379 = t1770 - t2140
        t2412 = (t4 * t2317 * t62 - t757) * t62
        t2420 = t2219 + t2227 - t249 * ((t4 * ((t2331 * t12 - t2333) * t
     #12 - t2338) * t12 - t4 * (t2338 - (t2336 - t2342 * t12) * t12) * t
     #12) * t12 + (((t4 * t2331 * t12 - t2214) * t12 - t2219) * t12 - (t
     #2219 - (t2217 - t4 * t2342 * t12) * t12) * t12) * t12) / 0.24E2 - 
     #t207 * ((t4 * ((t2368 * t49 - t2370) * t49 - t2375) * t49 - t4 * (
     #t2375 - (t2373 - t2379 * t49) * t49) * t49) * t49 + (((t4 * t2368 
     #* t49 - t2222) * t49 - t2227) * t49 - (t2227 - (t2225 - t4 * t2379
     # * t49) * t49) * t49) * t49) / 0.24E2 - t165 * ((t4 * t2321 * t62 
     #- t745) * t62 + ((t2412 - t759) * t62 - t761) * t62) / 0.24E2 + t7
     #59
        t2439 = (t4 * (t167 - t624) * t12 - t4 * (t624 - t1035) * t12) *
     # t12 + (t4 * (t1429 - t624) * t49 - t4 * (t624 - t1880) * t49) * t
     #49 + t2277 - t855 - t863 - t649
        t2440 = t2439 * t62
        t2441 = t864 * t62
        t2444 = dz * (t2440 / 0.2E1 + t2441 / 0.2E1)
        t2452 = t165 * (t739 - dz * (t2322 - t2323) / 0.12E2) / 0.12E2
        t2473 = (t4 * (t495 + t503 + t193 - t855 - t863 - t649) * t12 - 
     #t4 * (t855 + t863 + t649 - t1288 - t1296 - t1060) * t12) * t12 + (
     #t4 * (t1681 + t1686 + t1454 - t855 - t863 - t649) * t49 - t4 * (t8
     #55 + t863 + t649 - t2102 - t2107 - t1905) * t49) * t49 + (t4 * t24
     #39 * t62 - t866) * t62
        t2494 = t2228 * t62
        t2497 = dz * (((t4 * (t304 - t734) * t12 - t4 * (t734 - t1167) *
     # t12) * t12 + (t4 * (t1536 - t734) * t49 - t4 * (t734 - t2010) * t
     #49) * t49 + t2412 - t2219 - t2227 - t759) * t62 / 0.2E1 + t2494 / 
     #0.2E1)
        t2501 = dz * (t2440 - t2441)
        t2504 = t740 / 0.2E1
        t2505 = t751 * t62
        t2512 = dz * (t2315 + t2504 - t165 * (t2323 / 0.2E1 + t2505 / 0.
     #2E1) / 0.6E1) / 0.2E1
        t2513 = t880 * t62
        t2516 = dz * (t2441 / 0.2E1 + t2513 / 0.2E1)
        t2518 = t160 * t2516 / 0.2E1
        t2524 = t165 * (t742 - dz * (t2323 - t2505) / 0.12E2) / 0.12E2
        t2525 = t125 - t149
        t2527 = t4 * t2525 * t12
        t2528 = t149 - t902
        t2530 = t4 * t2528 * t12
        t2532 = (t2527 - t2530) * t12
        t2533 = t1415 - t149
        t2535 = t4 * t2533 * t49
        t2536 = t149 - t1774
        t2538 = t4 * t2536 * t49
        t2540 = (t2535 - t2538) * t49
        t2541 = t134 + t144 + t154 - t2532 - t2540 - t765
        t2542 = t2541 * t62
        t2545 = dz * (t2494 / 0.2E1 + t2542 / 0.2E1)
        t2547 = t303 * t2545 / 0.4E1
        t2549 = dz * (t2441 - t2513)
        t2551 = t160 * t2549 / 0.12E2
        t2552 = t145 + t160 * t2312 - t2330 + t303 * t2420 / 0.2E1 - t16
     #0 * t2444 / 0.2E1 + t2452 + t437 * t2473 / 0.6E1 - t303 * t2497 / 
     #0.4E1 + t160 * t2501 / 0.12E2 - t2 - t664 - t2512 - t775 - t2518 -
     # t2524 - t887 - t2547 - t2551
        t2556 = t925 * t2206
        t2558 = t928 * t2209 / 0.2E1
        t2560 = t932 * t2230 / 0.6E1
        t2562 = t935 * t2233 / 0.24E2
        t2575 = t935 * t2516 / 0.2E1
        t2577 = t939 * t2545 / 0.4E1
        t2579 = t935 * t2549 / 0.12E2
        t2580 = t145 + t935 * t2312 - t2330 + t939 * t2420 / 0.2E1 - t93
     #5 * t2444 / 0.2E1 + t2452 + t944 * t2473 / 0.6E1 - t939 * t2497 / 
     #0.4E1 + t935 * t2501 / 0.12E2 - t2 - t951 - t2512 - t953 - t2575 -
     # t2524 - t957 - t2577 - t2579
        t2583 = cc * t2580 * t921 / 0.8E1
        t2585 = (t8 * t2206 + t33 * t2209 / 0.2E1 + t103 * t2230 / 0.6E1
     # - t160 * t2233 / 0.24E2 + cc * t2552 * t921 / 0.8E1 - t2556 - t25
     #58 - t2560 + t2562 - t2583) * t5
        t2591 = t4 * (t627 - dz * t633 / 0.24E2)
        t2593 = dz * t650 / 0.24E2
        t2601 = dt * (t740 - dz * t751 / 0.24E2)
        t2604 = t34 * t880 * t62
        t2608 = t104 * t2541 * t62
        t2611 = dz * t766
        t2615 = t872 * t49
        t2618 = t875 * t49
        t2620 = (t2615 - t2618) * t49
        t2641 = k - 3
        t2643 = t636 - u(i,j,t2641,n)
        t2655 = (t653 - t4 * t2643 * t62) * t62
        t2664 = t510 * t12
        t2667 = t867 * t12
        t2669 = (t2664 - t2667) * t12
        t2690 = t879 - t207 * ((t4 * ((t1698 * t49 - t2615) * t49 - t262
     #0) * t49 - t4 * (t2620 - (t2618 - t2119 * t49) * t49) * t49) * t49
     # + ((t1702 - t879) * t49 - (t879 - t2123) * t49) * t49) / 0.24E2 +
     # t871 + t655 - t165 * ((t643 - t4 * (t640 - (t638 - t2643 * t62) *
     # t62) * t62) * t62 + (t657 - (t655 - t2655) * t62) * t62) / 0.24E2
     # - t249 * ((t4 * ((t507 * t12 - t2664) * t12 - t2669) * t12 - t4 *
     # (t2669 - (t2667 - t1300 * t12) * t12) * t12) * t12 + ((t514 - t87
     #1) * t12 - (t871 - t1304) * t12) * t12) / 0.24E2
        t2694 = t746 - ut(i,j,t2641,n)
        t2698 = t750 - (t748 - t2694 * t62) * t62
        t2699 = t2698 * t62
        t2706 = dz * (t2504 + t748 / 0.2E1 - t165 * (t2505 / 0.2E1 + t26
     #99 / 0.2E1) / 0.6E1) / 0.2E1
        t2714 = (t763 - t4 * t2694 * t62) * t62
        t2722 = t545 - t125
        t2724 = t2525 * t12
        t2727 = t2528 * t12
        t2729 = (t2724 - t2727) * t12
        t2733 = t902 - t1335
        t2759 = t1723 - t1415
        t2761 = t2533 * t49
        t2764 = t2536 * t49
        t2766 = (t2761 - t2764) * t49
        t2770 = t1774 - t2144
        t2796 = t2532 + t2540 + t765 - t165 * ((t753 - t4 * t2698 * t62)
     # * t62 + (t767 - (t765 - t2714) * t62) * t62) / 0.24E2 - t249 * ((
     #t4 * ((t2722 * t12 - t2724) * t12 - t2729) * t12 - t4 * (t2729 - (
     #t2727 - t2733 * t12) * t12) * t12) * t12 + (((t4 * t2722 * t12 - t
     #2527) * t12 - t2532) * t12 - (t2532 - (t2530 - t4 * t2733 * t12) *
     # t12) * t12) * t12) / 0.24E2 - t207 * ((t4 * ((t2759 * t49 - t2761
     #) * t49 - t2766) * t49 - t4 * (t2766 - (t2764 - t2770 * t49) * t49
     #) * t49) * t49 + (((t4 * t2759 * t49 - t2535) * t49 - t2540) * t49
     # - (t2540 - (t2538 - t4 * t2770 * t49) * t49) * t49) * t49) / 0.24
     #E2
        t2815 = t871 + t879 + t655 - (t4 * (t180 - t636) * t12 - t4 * (t
     #636 - t1047) * t12) * t12 - (t4 * (t1441 - t636) * t49 - t4 * (t63
     #6 - t1892) * t49) * t49 - t2655
        t2816 = t2815 * t62
        t2819 = dz * (t2513 / 0.2E1 + t2816 / 0.2E1)
        t2827 = t165 * (t750 - dz * (t2505 - t2699) / 0.12E2) / 0.12E2
        t2848 = (t4 * (t514 + t522 + t199 - t871 - t879 - t655) * t12 - 
     #t4 * (t871 + t879 + t655 - t1304 - t1312 - t1066) * t12) * t12 + (
     #t4 * (t1697 + t1702 + t1460 - t871 - t879 - t655) * t49 - t4 * (t8
     #71 + t879 + t655 - t2118 - t2123 - t1911) * t49) * t49 + (t882 - t
     #4 * t2815 * t62) * t62
        t2871 = dz * (t2542 / 0.2E1 + (t2532 + t2540 + t765 - (t4 * (t31
     #6 - t746) * t12 - t4 * (t746 - t1179) * t12) * t12 - (t4 * (t1548 
     #- t746) * t49 - t4 * (t746 - t2022) * t49) * t49 - t2714) * t62 / 
     #0.2E1)
        t2875 = dz * (t2513 - t2816)
        t2878 = t2 + t664 - t2512 + t775 - t2518 + t2524 + t887 - t2547 
     #+ t2551 - t149 - t160 * t2690 - t2706 - t303 * t2796 / 0.2E1 - t16
     #0 * t2819 / 0.2E1 - t2827 - t437 * t2848 / 0.6E1 - t303 * t2871 / 
     #0.4E1 - t160 * t2875 / 0.12E2
        t2882 = t925 * t2601
        t2884 = t928 * t2604 / 0.2E1
        t2886 = t932 * t2608 / 0.6E1
        t2888 = t935 * t2611 / 0.24E2
        t2900 = t2 + t951 - t2512 + t953 - t2575 + t2524 + t957 - t2577 
     #+ t2579 - t149 - t935 * t2690 - t2706 - t939 * t2796 / 0.2E1 - t93
     #5 * t2819 / 0.2E1 - t2827 - t944 * t2848 / 0.6E1 - t939 * t2871 / 
     #0.4E1 - t935 * t2875 / 0.12E2
        t2903 = cc * t2900 * t921 / 0.8E1
        t2905 = (t8 * t2601 + t33 * t2604 / 0.2E1 + t103 * t2608 / 0.6E1
     # - t160 * t2611 / 0.24E2 + cc * t2878 * t921 / 0.8E1 - t2882 - t28
     #84 - t2886 + t2888 - t2903) * t5
        t2911 = t4 * (t630 - dz * t641 / 0.24E2)
        t2913 = dz * t656 / 0.24E2

        unew(i,j,k) = t1 + dt * t2 + (t967 * t34 / 0.6E1 + (t973 + t9
     #26 + t930 - t975 + t934 - t937 + t965 - t967 * t924) * t34 / 0.2E1
     # - t1379 * t34 / 0.6E1 - (t1385 + t1356 + t1358 - t1387 + t1360 - 
     #t1362 + t1377 - t1379 * t924) * t34 / 0.2E1) * t12 + (t1824 * t34 
     #/ 0.6E1 + (t1830 + t1795 + t1797 - t1832 + t1799 - t1801 + t1822 -
     # t1824 * t924) * t34 / 0.2E1 - t2188 * t34 / 0.6E1 - (t2194 + t216
     #5 + t2167 - t2196 + t2169 - t2171 + t2186 - t2188 * t924) * t34 / 
     #0.2E1) * t49 + (t2585 * t34 / 0.6E1 + (t2591 + t2556 + t2558 - t25
     #93 + t2560 - t2562 + t2583 - t2585 * t924) * t34 / 0.2E1 - t2905 *
     # t34 / 0.6E1 - (t2911 + t2882 + t2884 - t2913 + t2886 - t2888 + t2
     #903 - t2905 * t924) * t34 / 0.2E1) * t62

        utnew(i,j,k) = t2 + (t967 * dt / 0.
     #2E1 + (t973 + t926 + t930 - t975 + t934 - t937 + t965) * dt - t967
     # * t935 - t1379 * dt / 0.2E1 - (t1385 + t1356 + t1358 - t1387 + t1
     #360 - t1362 + t1377) * dt + t1379 * t935) * t12 + (t1824 * dt / 0.
     #2E1 + (t1830 + t1795 + t1797 - t1832 + t1799 - t1801 + t1822) * dt
     # - t1824 * t935 - t2188 * dt / 0.2E1 - (t2194 + t2165 + t2167 - t2
     #196 + t2169 - t2171 + t2186) * dt + t2188 * t935) * t49 + (t2585 *
     # dt / 0.2E1 + (t2591 + t2556 + t2558 - t2593 + t2560 - t2562 + t25
     #83) * dt - t2585 * t935 - t2905 * dt / 0.2E1 - (t2911 + t2882 + t2
     #884 - t2913 + t2886 - t2888 + t2903) * dt + t2905 * t935) * t62

c        blah = array(int(t1 + dt * t2 + (t967 * t34 / 0.6E1 + (t973 + t9
c     #26 + t930 - t975 + t934 - t937 + t965 - t967 * t924) * t34 / 0.2E1
c     # - t1379 * t34 / 0.6E1 - (t1385 + t1356 + t1358 - t1387 + t1360 - 
c     #t1362 + t1377 - t1379 * t924) * t34 / 0.2E1) * t12 + (t1824 * t34 
c     #/ 0.6E1 + (t1830 + t1795 + t1797 - t1832 + t1799 - t1801 + t1822 -
c     # t1824 * t924) * t34 / 0.2E1 - t2188 * t34 / 0.6E1 - (t2194 + t216
c     #5 + t2167 - t2196 + t2169 - t2171 + t2186 - t2188 * t924) * t34 / 
c     #0.2E1) * t49 + (t2585 * t34 / 0.6E1 + (t2591 + t2556 + t2558 - t25
c     #93 + t2560 - t2562 + t2583 - t2585 * t924) * t34 / 0.2E1 - t2905 *
c     # t34 / 0.6E1 - (t2911 + t2882 + t2884 - t2913 + t2886 - t2888 + t2
c     #903 - t2905 * t924) * t34 / 0.2E1) * t62),int(t2 + (t967 * dt / 0.
c     #2E1 + (t973 + t926 + t930 - t975 + t934 - t937 + t965) * dt - t967
c     # * t935 - t1379 * dt / 0.2E1 - (t1385 + t1356 + t1358 - t1387 + t1
c     #360 - t1362 + t1377) * dt + t1379 * t935) * t12 + (t1824 * dt / 0.
c     #2E1 + (t1830 + t1795 + t1797 - t1832 + t1799 - t1801 + t1822) * dt
c     # - t1824 * t935 - t2188 * dt / 0.2E1 - (t2194 + t2165 + t2167 - t2
c     #196 + t2169 - t2171 + t2186) * dt + t2188 * t935) * t49 + (t2585 *
c     # dt / 0.2E1 + (t2591 + t2556 + t2558 - t2593 + t2560 - t2562 + t25
c     #83) * dt - t2585 * t935 - t2905 * dt / 0.2E1 - (t2911 + t2882 + t2
c     #884 - t2913 + t2886 - t2888 + t2903) * dt + t2905 * t935) * t62))

        return
      end
