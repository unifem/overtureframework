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
        integer t10
        real t100
        real t1005
        real t1009
        real t101
        real t102
        real t1029
        real t1035
        real t1037
        real t1039
        real t104
        real t1041
        real t1043
        real t1045
        real t1046
        real t1048
        real t1049
        real t105
        real t1051
        real t1053
        real t1055
        real t1057
        real t1059
        real t106
        real t1061
        real t1062
        real t1064
        real t1067
        real t1068
        real t1070
        real t1071
        real t1073
        real t1075
        real t108
        real t1082
        real t1083
        real t1085
        real t1087
        real t1089
        real t1095
        real t1097
        real t110
        real t1101
        real t1103
        real t111
        real t1110
        real t1111
        real t1113
        real t1115
        real t1117
        real t1123
        real t1124
        real t1125
        real t113
        real t1132
        real t1134
        real t1135
        real t1136
        integer t1140
        real t1142
        real t1146
        real t1147
        real t115
        real t1151
        real t1156
        real t1157
        real t1161
        real t1166
        real t1167
        real t117
        real t1171
        real t1174
        real t1176
        real t1182
        real t1186
        real t1187
        real t1188
        real t119
        real t1192
        real t1193
        real t1197
        integer t12
        real t120
        real t1202
        real t1206
        real t121
        real t1210
        real t1211
        real t1212
        real t1216
        real t1217
        real t122
        real t1221
        real t1226
        real t123
        real t1232
        real t1236
        real t1244
        real t1245
        real t1246
        real t125
        real t1250
        real t1254
        real t1258
        real t126
        real t1266
        real t1267
        real t1268
        real t1272
        real t128
        real t1283
        real t1287
        real t129
        real t1293
        real t1297
        real t13
        real t1306
        real t1308
        real t1309
        real t131
        real t1310
        real t1314
        real t1315
        real t1319
        integer t132
        real t1325
        real t1329
        real t133
        real t134
        real t1349
        real t135
        real t1351
        real t1353
        real t1355
        real t1358
        real t1363
        real t137
        real t138
        real t1382
        real t1385
        real t1387
        real t14
        real t140
        real t1409
        real t1412
        real t1414
        real t143
        real t1436
        real t1439
        real t144
        real t1440
        real t1442
        real t1445
        real t1447
        real t1451
        real t1452
        real t146
        real t1478
        real t1479
        real t148
        real t1481
        real t1484
        real t1486
        real t149
        real t1490
        real t1491
        real t150
        real t1533
        real t1545
        real t1547
        real t1549
        real t1551
        real t1552
        real t1553
        real t1557
        real t156
        real t1561
        real t1563
        real t1564
        real t1566
        integer t157
        real t1572
        real t1579
        real t158
        real t1585
        real t159
        real t1593
        real t1595
        real t1596
        real t1598
        real t1599
        real t16
        real t160
        real t1605
        real t161
        integer t1614
        real t1615
        real t1616
        real t1620
        real t1621
        real t1625
        real t163
        real t1632
        real t164
        real t1642
        real t1644
        real t1645
        real t1647
        real t1649
        real t1650
        real t1651
        real t1653
        real t1654
        real t1655
        real t1657
        real t1659
        real t166
        real t1662
        real t1663
        real t1666
        real t1667
        real t1669
        real t167
        real t1673
        real t1676
        real t1678
        real t1679
        real t1687
        real t1689
        real t169
        real t1693
        real t1694
        real t1698
        integer t170
        real t1703
        real t1707
        real t1709
        real t171
        real t1711
        real t1713
        real t1715
        real t1717
        real t1719
        real t172
        real t1722
        real t1724
        real t1725
        real t1727
        real t1728
        real t173
        real t1730
        real t1732
        real t1733
        real t1734
        real t1736
        real t1737
        real t1738
        real t1740
        real t1742
        real t1745
        real t1746
        real t1749
        real t175
        real t1750
        real t1752
        real t1754
        real t1757
        real t1759
        real t176
        real t1761
        real t1763
        real t1766
        real t1771
        real t1774
        real t1775
        real t1777
        real t178
        real t1780
        real t1782
        real t1786
        real t1787
        real t1799
        integer t18
        real t1805
        real t182
        real t1829
        real t1832
        real t1834
        real t184
        real t185
        real t1856
        real t1859
        real t186
        real t1860
        real t1862
        real t1865
        real t1867
        real t1871
        real t1872
        real t188
        real t19
        real t190
        real t191
        real t1913
        real t1915
        real t1918
        real t192
        real t1920
        real t1924
        real t1951
        real t1956
        real t1979
        real t198
        real t1980
        real t1984
        integer t199
        real t1995
        real t1996
        real t2
        real t20
        real t200
        real t2000
        real t201
        real t2019
        real t202
        real t2023
        real t203
        real t2032
        real t2039
        real t2041
        real t2043
        real t2045
        real t2047
        real t2049
        real t205
        real t2051
        real t2055
        real t2057
        real t2058
        real t206
        real t2062
        real t2064
        real t2066
        real t2068
        real t2069
        real t2071
        real t2075
        real t2076
        real t2078
        real t2079
        real t208
        real t2080
        real t2082
        real t2083
        real t2084
        real t2086
        real t2088
        real t209
        real t2090
        real t2092
        real t2093
        real t2095
        real t2097
        real t2099
        real t2106
        real t211
        real t2112
        real t2119
        integer t212
        real t2125
        real t2129
        real t213
        real t2130
        real t2138
        real t214
        real t2140
        real t2141
        real t215
        real t2152
        integer t2153
        real t2155
        real t2159
        real t2160
        real t2164
        real t2169
        real t217
        real t2170
        real t2174
        real t2176
        real t2178
        real t2179
        real t218
        real t2180
        real t2182
        real t2185
        real t2187
        real t2191
        real t2192
        real t22
        real t220
        real t2204
        real t2210
        real t2234
        real t2237
        real t2239
        real t224
        real t226
        real t2261
        real t2264
        real t2265
        real t2267
        real t227
        real t2270
        real t2272
        real t2276
        real t2277
        real t228
        real t230
        real t2303
        real t2304
        real t2316
        real t232
        real t2324
        real t2326
        real t2329
        real t233
        real t2331
        real t2335
        real t234
        real t2362
        real t2367
        real t2390
        real t2391
        real t2395
        real t24
        real t2406
        real t2407
        real t241
        real t2411
        real t243
        real t2430
        real t2434
        real t244
        real t2443
        real t245
        real t2454
        real t2458
        real t246
        real t247
        real t2478
        real t248
        real t2485
        real t2487
        real t249
        real t2490
        real t2492
        real t2494
        real t2496
        real t2498
        integer t25
        real t250
        real t2500
        real t2501
        real t2503
        real t2511
        real t2513
        real t2514
        real t2518
        real t2519
        real t252
        real t2521
        real t2524
        real t253
        real t2533
        real t254
        real t2540
        real t2546
        real t2548
        real t2549
        real t2553
        real t2554
        real t256
        real t2563
        real t2565
        real t2566
        real t2568
        real t2571
        real t2574
        real t2576
        real t2577
        real t2579
        real t258
        real t2581
        real t2582
        real t2584
        real t2585
        real t2587
        real t2589
        real t259
        real t2592
        real t2593
        real t2596
        real t2597
        real t26
        real t260
        real t2600
        integer t2603
        real t2605
        real t261
        real t2617
        real t262
        real t2626
        real t2629
        real t2631
        real t264
        real t265
        real t2653
        real t2656
        real t2658
        real t267
        real t268
        real t2680
        real t2683
        real t2684
        real t2696
        real t270
        real t2704
        real t2706
        real t2709
        real t271
        real t2711
        real t2715
        real t272
        real t273
        real t2741
        real t2743
        real t2746
        real t2748
        real t275
        real t2752
        real t276
        real t2779
        real t278
        real t2782
        real t2790
        real t2798
        real t2799
        real t2803
        real t2805
        real t2807
        real t2810
        real t282
        real t284
        real t285
        real t2858
        real t286
        real t2861
        real t288
        real t2889
        real t2893
        real t2896
        real t29
        real t290
        real t2900
        real t2904
        real t2906
        real t2909
        real t291
        real t2911
        real t2912
        real t2914
        real t2915
        real t2917
        real t2919
        real t292
        real t2920
        real t2922
        real t2923
        real t2925
        real t2927
        real t2930
        real t2931
        real t2934
        real t2935
        real t2937
        real t2939
        real t2942
        real t2944
        real t2945
        real t2947
        real t2949
        real t2951
        real t2953
        real t2955
        real t2959
        real t2961
        real t2963
        real t2965
        real t2967
        real t2969
        real t2971
        real t2973
        real t2974
        real t2976
        real t2979
        real t298
        real t2980
        real t2982
        real t2984
        real t2986
        real t299
        real t2993
        real t2994
        real t2996
        real t2998
        real t3000
        real t3006
        real t3008
        real t301
        real t3015
        real t3016
        real t302
        real t3020
        real t3021
        real t3022
        real t3024
        real t3026
        real t3028
        real t303
        real t3035
        real t3036
        real t3039
        real t3045
        real t3046
        real t305
        real t3052
        real t3054
        real t3056
        real t3059
        real t3061
        real t307
        real t308
        integer t3082
        real t3084
        real t309
        real t3096
        integer t31
        real t310
        real t3105
        real t3108
        real t311
        real t3110
        real t313
        real t3132
        real t3134
        real t3135
        real t3137
        real t314
        real t3140
        real t3142
        real t3146
        real t316
        real t317
        real t3172
        real t3173
        real t3185
        real t319
        real t3193
        real t3195
        real t3198
        real t32
        real t320
        real t3200
        real t3204
        real t321
        real t322
        real t3231
        real t3233
        real t324
        real t3241
        real t3249
        real t325
        real t3250
        real t3254
        real t3257
        real t3259
        real t327
        real t3305
        real t3307
        real t331
        real t333
        real t3338
        real t334
        real t3340
        real t3342
        real t3344
        real t3347
        real t335
        real t3365
        real t3366
        real t3367
        real t337
        real t3371
        real t3375
        real t3377
        real t3378
        real t3380
        real t3383
        real t3384
        real t3386
        real t339
        real t3393
        real t3399
        real t340
        real t3403
        real t341
        real t3411
        real t3413
        real t3422
        real t3424
        real t3434
        real t3435
        real t3441
        real t3442
        real t3450
        real t3451
        real t3457
        real t3458
        real t3466
        real t3467
        real t347
        real t3473
        real t3474
        real t348
        real t350
        real t351
        real t352
        real t354
        real t356
        real t357
        real t358
        real t359
        real t360
        real t362
        real t363
        real t365
        real t366
        real t368
        real t369
        real t37
        real t370
        real t371
        real t373
        real t374
        real t376
        integer t38
        real t380
        real t382
        real t383
        real t384
        real t386
        real t388
        real t389
        real t39
        real t390
        integer t396
        real t399
        real t4
        real t400
        real t401
        integer t402
        real t405
        real t406
        real t408
        real t410
        real t411
        real t412
        real t414
        real t415
        real t416
        real t418
        real t42
        real t420
        real t421
        real t422
        real t424
        real t425
        real t426
        real t428
        real t430
        real t431
        real t433
        real t435
        real t438
        integer t44
        real t440
        real t441
        real t442
        real t443
        real t444
        real t445
        real t448
        real t45
        real t451
        real t454
        real t456
        real t457
        real t459
        real t461
        real t462
        real t463
        real t465
        real t466
        real t467
        real t469
        real t471
        real t474
        real t475
        real t477
        real t478
        real t480
        real t482
        real t483
        real t484
        real t486
        real t487
        real t488
        real t490
        real t492
        real t495
        real t498
        real t5
        real t50
        real t500
        real t501
        real t503
        real t505
        real t506
        real t508
        real t509
        real t51
        real t511
        real t513
        real t516
        real t517
        real t519
        real t520
        real t522
        real t524
        real t525
        real t527
        real t528
        real t530
        real t532
        real t535
        real t54
        real t540
        real t543
        real t546
        real t549
        real t550
        real t553
        real t556
        real t559
        real t56
        real t560
        real t563
        real t569
        real t571
        real t572
        real t573
        real t575
        real t576
        real t577
        real t58
        real t580
        real t584
        real t586
        real t587
        real t588
        real t589
        real t59
        real t590
        real t592
        real t593
        real t594
        real t596
        real t598
        real t599
        real t6
        real t60
        real t600
        real t602
        real t603
        real t604
        real t606
        real t608
        real t611
        real t612
        real t615
        real t616
        real t618
        real t62
        real t620
        real t622
        real t623
        real t624
        real t626
        real t627
        real t628
        real t63
        real t630
        real t632
        real t633
        real t634
        real t636
        real t637
        real t638
        real t64
        real t640
        real t642
        real t645
        real t646
        real t649
        real t650
        real t652
        real t654
        real t657
        real t659
        real t66
        real t661
        real t663
        real t664
        real t665
        real t668
        real t671
        real t672
        real t675
        real t678
        real t68
        real t69
        real t694
        real t695
        real t697
        real t7
        real t70
        real t700
        real t702
        real t706
        real t707
        real t719
        real t72
        real t725
        real t73
        real t733
        real t734
        real t736
        real t739
        real t74
        real t741
        real t745
        real t746
        real t758
        real t76
        real t764
        real t773
        real t776
        real t777
        real t778
        real t78
        real t780
        real t783
        real t785
        real t789
        real t79
        real t790
        real t8
        real t81
        real t816
        real t817
        real t819
        real t822
        real t824
        real t828
        real t829
        real t83
        integer t84
        real t85
        real t855
        real t856
        real t86
        real t868
        real t877
        real t88
        real t882
        real t885
        real t886
        real t892
        real t896
        real t897
        real t9
        real t90
        real t901
        real t906
        real t91
        real t910
        real t914
        real t915
        real t919
        real t92
        real t924
        real t930
        real t934
        real t94
        real t942
        real t946
        real t95
        real t950
        real t958
        real t96
        real t969
        real t973
        real t979
        real t98
        real t983
        real t992
        real t995
        real t999
        t1 = u(i,j,k,n)
        t2 = ut(i,j,k,n)
        t4 = sqrt(0.3E1)
        t5 = t4 / 0.6E1
        t6 = 0.1E1 / 0.2E1 - t5
        t7 = beta * t6
        t8 = dt * dx
        t9 = cc ** 2
        t10 = i + 3
        t12 = i + 2
        t13 = u(t12,j,k,n)
        t14 = u(t10,j,k,n) - t13
        t16 = 0.1E1 / dx
        t18 = i + 1
        t19 = u(t18,j,k,n)
        t20 = t13 - t19
        t22 = t9 * t20 * t16
        t24 = (t14 * t16 * t9 - t22) * t16
        t25 = j + 1
        t26 = u(t12,t25,k,n)
        t29 = 0.1E1 / dy
        t31 = j - 1
        t32 = u(t12,t31,k,n)
        t37 = (t9 * (t26 - t13) * t29 - t9 * (t13 - t32) * t29) * t29
        t38 = k + 1
        t39 = u(t12,j,t38,n)
        t42 = 0.1E1 / dz
        t44 = k - 1
        t45 = u(t12,j,t44,n)
        t50 = (t9 * (t39 - t13) * t42 - t9 * (t13 - t45) * t42) * t42
        t51 = src(t12,j,k,nComp,n)
        t54 = t19 - t1
        t56 = t9 * t54 * t16
        t58 = (t22 - t56) * t16
        t59 = u(t18,t25,k,n)
        t60 = t59 - t19
        t62 = t9 * t60 * t29
        t63 = u(t18,t31,k,n)
        t64 = t19 - t63
        t66 = t9 * t64 * t29
        t68 = (t62 - t66) * t29
        t69 = u(t18,j,t38,n)
        t70 = t69 - t19
        t72 = t9 * t70 * t42
        t73 = u(t18,j,t44,n)
        t74 = t19 - t73
        t76 = t9 * t74 * t42
        t78 = (t72 - t76) * t42
        t79 = src(t18,j,k,nComp,n)
        t81 = cc * (t58 + t68 + t78 + t79)
        t83 = (cc * (t24 + t37 + t50 + t51) - t81) * t16
        t84 = i - 1
        t85 = u(t84,j,k,n)
        t86 = t1 - t85
        t88 = t9 * t86 * t16
        t90 = (t56 - t88) * t16
        t91 = u(i,t25,k,n)
        t92 = t91 - t1
        t94 = t9 * t92 * t29
        t95 = u(i,t31,k,n)
        t96 = t1 - t95
        t98 = t9 * t96 * t29
        t100 = (t94 - t98) * t29
        t101 = u(i,j,t38,n)
        t102 = t101 - t1
        t104 = t9 * t102 * t42
        t105 = u(i,j,t44,n)
        t106 = t1 - t105
        t108 = t9 * t106 * t42
        t110 = (t104 - t108) * t42
        t111 = src(i,j,k,nComp,n)
        t113 = cc * (t90 + t100 + t110 + t111)
        t115 = (t81 - t113) * t16
        t117 = t8 * (t83 - t115)
        t119 = t7 * t117 / 0.24E2
        t120 = dt * cc
        t121 = dx ** 2
        t122 = t20 * t16
        t123 = t54 * t16
        t125 = (t122 - t123) * t16
        t126 = t86 * t16
        t128 = (t123 - t126) * t16
        t129 = t125 - t128
        t131 = t9 * t129 * t16
        t132 = i - 2
        t133 = u(t132,j,k,n)
        t134 = t85 - t133
        t135 = t134 * t16
        t137 = (t126 - t135) * t16
        t138 = t128 - t137
        t140 = t9 * t138 * t16
        t143 = t58 - t90
        t144 = t143 * t16
        t146 = t9 * t134 * t16
        t148 = (t88 - t146) * t16
        t149 = t90 - t148
        t150 = t149 * t16
        t156 = dy ** 2
        t157 = j + 2
        t158 = u(i,t157,k,n)
        t159 = t158 - t91
        t160 = t159 * t29
        t161 = t92 * t29
        t163 = (t160 - t161) * t29
        t164 = t96 * t29
        t166 = (t161 - t164) * t29
        t167 = t163 - t166
        t169 = t9 * t167 * t29
        t170 = j - 2
        t171 = u(i,t170,k,n)
        t172 = t95 - t171
        t173 = t172 * t29
        t175 = (t164 - t173) * t29
        t176 = t166 - t175
        t178 = t9 * t176 * t29
        t182 = t9 * t159 * t29
        t184 = (t182 - t94) * t29
        t185 = t184 - t100
        t186 = t185 * t29
        t188 = t9 * t172 * t29
        t190 = (t98 - t188) * t29
        t191 = t100 - t190
        t192 = t191 * t29
        t198 = dz ** 2
        t199 = k + 2
        t200 = u(i,j,t199,n)
        t201 = t200 - t101
        t202 = t201 * t42
        t203 = t102 * t42
        t205 = (t202 - t203) * t42
        t206 = t106 * t42
        t208 = (t203 - t206) * t42
        t209 = t205 - t208
        t211 = t9 * t209 * t42
        t212 = k - 2
        t213 = u(i,j,t212,n)
        t214 = t105 - t213
        t215 = t214 * t42
        t217 = (t206 - t215) * t42
        t218 = t208 - t217
        t220 = t9 * t218 * t42
        t224 = t9 * t201 * t42
        t226 = (t224 - t104) * t42
        t227 = t226 - t110
        t228 = t227 * t42
        t230 = t9 * t214 * t42
        t232 = (t108 - t230) * t42
        t233 = t110 - t232
        t234 = t233 * t42
        t241 = t120 * (t90 - t121 * ((t131 - t140) * t16 + (t144 - t150)
     # * t16) / 0.24E2 + t100 - t156 * ((t169 - t178) * t29 + (t186 - t1
     #92) * t29) / 0.24E2 + t110 - t198 * ((t211 - t220) * t42 + (t228 -
     # t234) * t42) / 0.24E2 + t111)
        t243 = t7 * t241 / 0.2E1
        t244 = beta ** 2
        t245 = t6 ** 2
        t246 = t244 * t245
        t247 = dt ** 2
        t248 = t247 * cc
        t249 = ut(t18,j,k,n)
        t250 = t249 - t2
        t252 = t9 * t250 * t16
        t253 = ut(t84,j,k,n)
        t254 = t2 - t253
        t256 = t9 * t254 * t16
        t258 = (t252 - t256) * t16
        t259 = ut(t12,j,k,n)
        t260 = t259 - t249
        t261 = t260 * t16
        t262 = t250 * t16
        t264 = (t261 - t262) * t16
        t265 = t254 * t16
        t267 = (t262 - t265) * t16
        t268 = t264 - t267
        t270 = t9 * t268 * t16
        t271 = ut(t132,j,k,n)
        t272 = t253 - t271
        t273 = t272 * t16
        t275 = (t265 - t273) * t16
        t276 = t267 - t275
        t278 = t9 * t276 * t16
        t282 = t9 * t260 * t16
        t284 = (t282 - t252) * t16
        t285 = t284 - t258
        t286 = t285 * t16
        t288 = t9 * t272 * t16
        t290 = (t256 - t288) * t16
        t291 = t258 - t290
        t292 = t291 * t16
        t298 = ut(i,t25,k,n)
        t299 = t298 - t2
        t301 = t9 * t299 * t29
        t302 = ut(i,t31,k,n)
        t303 = t2 - t302
        t305 = t9 * t303 * t29
        t307 = (t301 - t305) * t29
        t308 = ut(i,t157,k,n)
        t309 = t308 - t298
        t310 = t309 * t29
        t311 = t299 * t29
        t313 = (t310 - t311) * t29
        t314 = t303 * t29
        t316 = (t311 - t314) * t29
        t317 = t313 - t316
        t319 = t9 * t317 * t29
        t320 = ut(i,t170,k,n)
        t321 = t302 - t320
        t322 = t321 * t29
        t324 = (t314 - t322) * t29
        t325 = t316 - t324
        t327 = t9 * t325 * t29
        t331 = t9 * t309 * t29
        t333 = (t331 - t301) * t29
        t334 = t333 - t307
        t335 = t334 * t29
        t337 = t9 * t321 * t29
        t339 = (t305 - t337) * t29
        t340 = t307 - t339
        t341 = t340 * t29
        t347 = ut(i,j,t38,n)
        t348 = t347 - t2
        t350 = t9 * t348 * t42
        t351 = ut(i,j,t44,n)
        t352 = t2 - t351
        t354 = t9 * t352 * t42
        t356 = (t350 - t354) * t42
        t357 = ut(i,j,t199,n)
        t358 = t357 - t347
        t359 = t358 * t42
        t360 = t348 * t42
        t362 = (t359 - t360) * t42
        t363 = t352 * t42
        t365 = (t360 - t363) * t42
        t366 = t362 - t365
        t368 = t9 * t366 * t42
        t369 = ut(i,j,t212,n)
        t370 = t351 - t369
        t371 = t370 * t42
        t373 = (t363 - t371) * t42
        t374 = t365 - t373
        t376 = t9 * t374 * t42
        t380 = t9 * t358 * t42
        t382 = (t380 - t350) * t42
        t383 = t382 - t356
        t384 = t383 * t42
        t386 = t9 * t370 * t42
        t388 = (t354 - t386) * t42
        t389 = t356 - t388
        t390 = t389 * t42
        t396 = n + 1
        t399 = 0.1E1 / dt
        t400 = (src(i,j,k,nComp,t396) - t111) * t399
        t401 = t400 / 0.2E1
        t402 = n - 1
        t405 = (t111 - src(i,j,k,nComp,t402)) * t399
        t406 = t405 / 0.2E1
        t408 = t248 * (t258 - t121 * ((t270 - t278) * t16 + (t286 - t292
     #) * t16) / 0.24E2 + t307 - t156 * ((t319 - t327) * t29 + (t335 - t
     #341) * t29) / 0.24E2 + t356 - t198 * ((t368 - t376) * t42 + (t384 
     #- t390) * t42) / 0.24E2 + t401 + t406)
        t410 = t246 * t408 / 0.4E1
        t411 = u(t84,t25,k,n)
        t412 = t411 - t85
        t414 = t9 * t412 * t29
        t415 = u(t84,t31,k,n)
        t416 = t85 - t415
        t418 = t9 * t416 * t29
        t420 = (t414 - t418) * t29
        t421 = u(t84,j,t38,n)
        t422 = t421 - t85
        t424 = t9 * t422 * t42
        t425 = u(t84,j,t44,n)
        t426 = t85 - t425
        t428 = t9 * t426 * t42
        t430 = (t424 - t428) * t42
        t431 = src(t84,j,k,nComp,n)
        t433 = cc * (t148 + t420 + t430 + t431)
        t435 = (t113 - t433) * t16
        t438 = t8 * (t115 / 0.2E1 + t435 / 0.2E1)
        t440 = t7 * t438 / 0.4E1
        t441 = t244 * beta
        t442 = t245 * t6
        t443 = t441 * t442
        t444 = t247 * dt
        t445 = t444 * cc
        t448 = t9 * (t58 + t68 + t78 - t90 - t100 - t110) * t16
        t451 = t9 * (t90 + t100 + t110 - t148 - t420 - t430) * t16
        t454 = t59 - t91
        t456 = t9 * t454 * t16
        t457 = t91 - t411
        t459 = t9 * t457 * t16
        t461 = (t456 - t459) * t16
        t462 = u(i,t25,t38,n)
        t463 = t462 - t91
        t465 = t9 * t463 * t42
        t466 = u(i,t25,t44,n)
        t467 = t91 - t466
        t469 = t9 * t467 * t42
        t471 = (t465 - t469) * t42
        t474 = t9 * (t461 + t184 + t471 - t90 - t100 - t110) * t29
        t475 = t63 - t95
        t477 = t9 * t475 * t16
        t478 = t95 - t415
        t480 = t9 * t478 * t16
        t482 = (t477 - t480) * t16
        t483 = u(i,t31,t38,n)
        t484 = t483 - t95
        t486 = t9 * t484 * t42
        t487 = u(i,t31,t44,n)
        t488 = t95 - t487
        t490 = t9 * t488 * t42
        t492 = (t486 - t490) * t42
        t495 = t9 * (t90 + t100 + t110 - t482 - t190 - t492) * t29
        t498 = t69 - t101
        t500 = t9 * t498 * t16
        t501 = t101 - t421
        t503 = t9 * t501 * t16
        t505 = (t500 - t503) * t16
        t506 = t462 - t101
        t508 = t9 * t506 * t29
        t509 = t101 - t483
        t511 = t9 * t509 * t29
        t513 = (t508 - t511) * t29
        t516 = t9 * (t505 + t513 + t226 - t90 - t100 - t110) * t42
        t517 = t73 - t105
        t519 = t9 * t517 * t16
        t520 = t105 - t425
        t522 = t9 * t520 * t16
        t524 = (t519 - t522) * t16
        t525 = t466 - t105
        t527 = t9 * t525 * t29
        t528 = t105 - t487
        t530 = t9 * t528 * t29
        t532 = (t527 - t530) * t29
        t535 = t9 * (t90 + t100 + t110 - t524 - t532 - t232) * t42
        t540 = t9 * (t79 - t111) * t16
        t543 = t9 * (t111 - t431) * t16
        t546 = src(i,t25,k,nComp,n)
        t549 = t9 * (t546 - t111) * t29
        t550 = src(i,t31,k,nComp,n)
        t553 = t9 * (t111 - t550) * t29
        t556 = src(i,j,t38,nComp,n)
        t559 = t9 * (t556 - t111) * t42
        t560 = src(i,j,t44,nComp,n)
        t563 = t9 * (t111 - t560) * t42
        t569 = t445 * ((t448 - t451) * t16 + (t474 - t495) * t29 + (t516
     # - t535) * t42 + (t540 - t543) * t16 + (t549 - t553) * t29 + (t559
     # - t563) * t42 + (t400 - t405) * t399)
        t571 = t443 * t569 / 0.12E2
        t572 = t6 * dt
        t573 = dx * t285
        t575 = t572 * t573 / 0.24E2
        t576 = 0.1E1 / 0.2E1 + t5
        t577 = t576 * dt
        t580 = t9 * t576
        t584 = dt * (t262 - dx * t268 / 0.24E2)
        t586 = t9 * t6
        t587 = t584 * t586
        t588 = t247 * dx
        t589 = ut(t18,t25,k,n)
        t590 = t589 - t249
        t592 = t9 * t590 * t29
        t593 = ut(t18,t31,k,n)
        t594 = t249 - t593
        t596 = t9 * t594 * t29
        t598 = (t592 - t596) * t29
        t599 = ut(t18,j,t38,n)
        t600 = t599 - t249
        t602 = t9 * t600 * t42
        t603 = ut(t18,j,t44,n)
        t604 = t249 - t603
        t606 = t9 * t604 * t42
        t608 = (t602 - t606) * t42
        t611 = (src(t18,j,k,nComp,t396) - t79) * t399
        t612 = t611 / 0.2E1
        t615 = (t79 - src(t18,j,k,nComp,t402)) * t399
        t616 = t615 / 0.2E1
        t618 = cc * (t284 + t598 + t608 + t612 + t616)
        t620 = cc * (t258 + t307 + t356 + t401 + t406)
        t622 = (t618 - t620) * t16
        t623 = ut(t84,t25,k,n)
        t624 = t623 - t253
        t626 = t9 * t624 * t29
        t627 = ut(t84,t31,k,n)
        t628 = t253 - t627
        t630 = t9 * t628 * t29
        t632 = (t626 - t630) * t29
        t633 = ut(t84,j,t38,n)
        t634 = t633 - t253
        t636 = t9 * t634 * t42
        t637 = ut(t84,j,t44,n)
        t638 = t253 - t637
        t640 = t9 * t638 * t42
        t642 = (t636 - t640) * t42
        t645 = (src(t84,j,k,nComp,t396) - t431) * t399
        t646 = t645 / 0.2E1
        t649 = (t431 - src(t84,j,k,nComp,t402)) * t399
        t650 = t649 / 0.2E1
        t652 = cc * (t290 + t632 + t642 + t646 + t650)
        t654 = (t620 - t652) * t16
        t657 = t588 * (t622 / 0.2E1 + t654 / 0.2E1)
        t659 = t246 * t657 / 0.8E1
        t661 = t8 * (t115 - t435)
        t663 = t7 * t661 / 0.24E2
        t664 = t576 ** 2
        t665 = t9 * t664
        t668 = t247 * (t58 + t68 + t78 + t79 - t90 - t100 - t110 - t111)
     # * t16
        t671 = t664 * t576
        t672 = t9 * t671
        t675 = t444 * (t284 + t598 + t608 + t612 + t616 - t258 - t307 - 
     #t356 - t401 - t406) * t16
        t678 = beta * t576
        t694 = u(t18,t157,k,n)
        t695 = t694 - t59
        t697 = t60 * t29
        t700 = t64 * t29
        t702 = (t697 - t700) * t29
        t706 = u(t18,t170,k,n)
        t707 = t63 - t706
        t719 = (t29 * t695 * t9 - t62) * t29
        t725 = (-t29 * t707 * t9 + t66) * t29
        t733 = u(t18,j,t199,n)
        t734 = t733 - t69
        t736 = t70 * t42
        t739 = t74 * t42
        t741 = (t736 - t739) * t42
        t745 = u(t18,j,t212,n)
        t746 = t73 - t745
        t758 = (t42 * t734 * t9 - t72) * t42
        t764 = (-t42 * t746 * t9 + t76) * t42
        t773 = t120 * (t58 - t121 * ((t9 * ((t14 * t16 - t122) * t16 - t
     #125) * t16 - t131) * t16 + ((t24 - t58) * t16 - t144) * t16) / 0.2
     #4E2 + t68 - t156 * ((t9 * ((t29 * t695 - t697) * t29 - t702) * t29
     # - t9 * (t702 - (-t29 * t707 + t700) * t29) * t29) * t29 + ((t719 
     #- t68) * t29 - (t68 - t725) * t29) * t29) / 0.24E2 + t78 - t198 * 
     #((t9 * ((t42 * t734 - t736) * t42 - t741) * t42 - t9 * (t741 - (-t
     #42 * t746 + t739) * t42) * t42) * t42 + ((t758 - t78) * t42 - (t78
     # - t764) * t42) * t42) / 0.24E2 + t79)
        t776 = t244 * t664
        t777 = ut(t18,t157,k,n)
        t778 = t777 - t589
        t780 = t590 * t29
        t783 = t594 * t29
        t785 = (t780 - t783) * t29
        t789 = ut(t18,t170,k,n)
        t790 = t593 - t789
        t816 = ut(t18,j,t199,n)
        t817 = t816 - t599
        t819 = t600 * t42
        t822 = t604 * t42
        t824 = (t819 - t822) * t42
        t828 = ut(t18,j,t212,n)
        t829 = t603 - t828
        t855 = ut(t10,j,k,n)
        t856 = t855 - t259
        t868 = (t16 * t856 * t9 - t282) * t16
        t877 = t248 * (-t156 * ((t9 * ((t29 * t778 - t780) * t29 - t785)
     # * t29 - t9 * (t785 - (-t29 * t790 + t783) * t29) * t29) * t29 + (
     #((t29 * t778 * t9 - t592) * t29 - t598) * t29 - (t598 - (-t29 * t7
     #90 * t9 + t596) * t29) * t29) * t29) / 0.24E2 + t608 - t198 * ((t9
     # * ((t42 * t817 - t819) * t42 - t824) * t42 - t9 * (t824 - (-t42 *
     # t829 + t822) * t42) * t42) * t42 + (((t42 * t817 * t9 - t602) * t
     #42 - t608) * t42 - (t608 - (-t42 * t829 * t9 + t606) * t42) * t42)
     # * t42) / 0.24E2 + t598 + t284 - t121 * ((t9 * ((t16 * t856 - t261
     #) * t16 - t264) * t16 - t270) * t16 + ((t868 - t284) * t16 - t286)
     # * t16) / 0.24E2 + t612 + t616)
        t882 = t8 * (t83 / 0.2E1 + t115 / 0.2E1)
        t885 = -t119 + t243 + t410 + t440 + t571 + t575 - t577 * t573 / 
     #0.24E2 + t580 * t584 - t587 + t659 + t663 + t665 * t668 / 0.2E1 + 
     #t672 * t675 / 0.6E1 + t678 * t773 / 0.2E1 + t776 * t877 / 0.4E1 - 
     #t678 * t882 / 0.4E1
        t886 = t441 * t671
        t892 = t26 - t59
        t896 = (t16 * t892 * t9 - t456) * t16
        t897 = u(t18,t25,t38,n)
        t901 = u(t18,t25,t44,n)
        t906 = (t9 * (t897 - t59) * t42 - t9 * (t59 - t901) * t42) * t42
        t910 = t32 - t63
        t914 = (t16 * t9 * t910 - t477) * t16
        t915 = u(t18,t31,t38,n)
        t919 = u(t18,t31,t44,n)
        t924 = (t9 * (t915 - t63) * t42 - t9 * (t63 - t919) * t42) * t42
        t930 = t39 - t69
        t934 = (t16 * t9 * t930 - t500) * t16
        t942 = (t9 * (t897 - t69) * t29 - t9 * (t69 - t915) * t29) * t29
        t946 = t45 - t73
        t950 = (t16 * t9 * t946 - t519) * t16
        t958 = (t9 * (t901 - t73) * t29 - t9 * (t73 - t919) * t29) * t29
        t969 = src(t18,t25,k,nComp,n)
        t973 = src(t18,t31,k,nComp,n)
        t979 = src(t18,j,t38,nComp,n)
        t983 = src(t18,j,t44,nComp,n)
        t992 = t445 * ((t9 * (t24 + t37 + t50 - t58 - t68 - t78) * t16 -
     # t448) * t16 + (t9 * (t896 + t719 + t906 - t58 - t68 - t78) * t29 
     #- t9 * (t58 + t68 + t78 - t914 - t725 - t924) * t29) * t29 + (t9 *
     # (t934 + t942 + t758 - t58 - t68 - t78) * t42 - t9 * (t58 + t68 + 
     #t78 - t950 - t958 - t764) * t42) * t42 + (t9 * (t51 - t79) * t16 -
     # t540) * t16 + (t9 * (t969 - t79) * t29 - t9 * (t79 - t973) * t29)
     # * t29 + (t9 * (t979 - t79) * t42 - t9 * (t79 - t983) * t42) * t42
     # + (t611 - t615) * t399)
        t995 = ut(t12,t25,k,n)
        t999 = ut(t12,t31,k,n)
        t1005 = ut(t12,j,t38,n)
        t1009 = ut(t12,j,t44,n)
        t1029 = t588 * ((cc * (t868 + (t9 * (t995 - t259) * t29 - t9 * (
     #t259 - t999) * t29) * t29 + (t9 * (t1005 - t259) * t42 - t9 * (t25
     #9 - t1009) * t42) * t42 + (src(t12,j,k,nComp,t396) - t51) * t399 /
     # 0.2E1 + (t51 - src(t12,j,k,nComp,t402)) * t399 / 0.2E1) - t618) *
     # t16 / 0.2E1 + t622 / 0.2E1)
        t1035 = t678 * t241 / 0.2E1
        t1037 = t776 * t408 / 0.4E1
        t1039 = t678 * t438 / 0.4E1
        t1041 = t886 * t569 / 0.12E2
        t1043 = t776 * t657 / 0.8E1
        t1045 = t678 * t661 / 0.24E2
        t1046 = t9 * t245
        t1048 = t1046 * t668 / 0.2E1
        t1049 = t9 * t442
        t1051 = t1049 * t675 / 0.6E1
        t1053 = t7 * t773 / 0.2E1
        t1055 = t246 * t877 / 0.4E1
        t1057 = t7 * t882 / 0.4E1
        t1059 = t443 * t992 / 0.12E2
        t1061 = t246 * t1029 / 0.8E1
        t1062 = t886 * t992 / 0.12E2 - t776 * t1029 / 0.8E1 + t678 * t11
     #7 / 0.24E2 - t1035 - t1037 - t1039 - t1041 - t1043 - t1045 - t1048
     # - t1051 - t1053 - t1055 + t1057 - t1059 + t1061
        t1064 = (t885 + t1062) * t4
        t1067 = cc * t249
        t1068 = cc * t259
        t1070 = (-t1067 + t1068) * t16
        t1071 = cc * t2
        t1073 = (-t1071 + t1067) * t16
        t1075 = (t1070 - t1073) * t16
        t1082 = (((cc * t855 - t1068) * t16 - t1070) * t16 - t1075) * t1
     #6
        t1083 = cc * t253
        t1085 = (t1071 - t1083) * t16
        t1087 = (t1073 - t1085) * t16
        t1089 = (t1075 - t1087) * t16
        t1095 = t121 * (t1075 - dx * (t1082 - t1089) / 0.12E2) / 0.24E2
        t1097 = dx * t143 / 0.24E2
        t1101 = t9 * (t123 - dx * t129 / 0.24E2)
        t1103 = t1073 / 0.2E1
        t1110 = dx * (t1070 / 0.2E1 + t1103 - t121 * (t1082 / 0.2E1 + t1
     #089 / 0.2E1) / 0.6E1) / 0.4E1
        t1111 = cc * t271
        t1113 = (-t1111 + t1083) * t16
        t1115 = (t1085 - t1113) * t16
        t1117 = (t1087 - t1115) * t16
        t1123 = t121 * (t1087 - dx * (t1089 - t1117) / 0.12E2) / 0.24E2
        t1124 = t119 - t243 - t410 - t440 - t571 - t575 + t587 + t1095 -
     # t1097 + t1101 - t1110 - t1123
        t1125 = t1085 / 0.2E1
        t1132 = dx * (t1103 + t1125 - t121 * (t1089 / 0.2E1 + t1117 / 0.
     #2E1) / 0.6E1) / 0.4E1
        t1134 = t1071 / 0.2E1
        t1135 = t1067 / 0.2E1
        t1136 = -t1064 * t6 + t1048 + t1051 + t1053 + t1055 - t1057 + t1
     #059 - t1061 - t1132 - t1134 + t1135 - t659 - t663
        t1140 = i - 3
        t1142 = t133 - u(t1140,j,k,n)
        t1146 = (-t1142 * t16 * t9 + t146) * t16
        t1147 = u(t132,t25,k,n)
        t1151 = u(t132,t31,k,n)
        t1156 = (t9 * (t1147 - t133) * t29 - t9 * (t133 - t1151) * t29) 
     #* t29
        t1157 = u(t132,j,t38,n)
        t1161 = u(t132,j,t44,n)
        t1166 = (t9 * (t1157 - t133) * t42 - t9 * (t133 - t1161) * t42) 
     #* t42
        t1167 = src(t132,j,k,nComp,n)
        t1171 = (t433 - cc * (t1146 + t1156 + t1166 + t1167)) * t16
        t1174 = t8 * (t435 / 0.2E1 + t1171 / 0.2E1)
        t1176 = t7 * t1174 / 0.4E1
        t1182 = t411 - t1147
        t1186 = (-t1182 * t16 * t9 + t459) * t16
        t1187 = u(t84,t157,k,n)
        t1188 = t1187 - t411
        t1192 = (t1188 * t29 * t9 - t414) * t29
        t1193 = u(t84,t25,t38,n)
        t1197 = u(t84,t25,t44,n)
        t1202 = (t9 * (t1193 - t411) * t42 - t9 * (t411 - t1197) * t42) 
     #* t42
        t1206 = t415 - t1151
        t1210 = (-t1206 * t16 * t9 + t480) * t16
        t1211 = u(t84,t170,k,n)
        t1212 = t415 - t1211
        t1216 = (-t1212 * t29 * t9 + t418) * t29
        t1217 = u(t84,t31,t38,n)
        t1221 = u(t84,t31,t44,n)
        t1226 = (t9 * (t1217 - t415) * t42 - t9 * (t415 - t1221) * t42) 
     #* t42
        t1232 = t421 - t1157
        t1236 = (-t1232 * t16 * t9 + t503) * t16
        t1244 = (t9 * (t1193 - t421) * t29 - t9 * (t421 - t1217) * t29) 
     #* t29
        t1245 = u(t84,j,t199,n)
        t1246 = t1245 - t421
        t1250 = (t1246 * t42 * t9 - t424) * t42
        t1254 = t425 - t1161
        t1258 = (-t1254 * t16 * t9 + t522) * t16
        t1266 = (t9 * (t1197 - t425) * t29 - t9 * (t425 - t1221) * t29) 
     #* t29
        t1267 = u(t84,j,t212,n)
        t1268 = t425 - t1267
        t1272 = (-t1268 * t42 * t9 + t428) * t42
        t1283 = src(t84,t25,k,nComp,n)
        t1287 = src(t84,t31,k,nComp,n)
        t1293 = src(t84,j,t38,nComp,n)
        t1297 = src(t84,j,t44,nComp,n)
        t1306 = t445 * ((t451 - t9 * (t148 + t420 + t430 - t1146 - t1156
     # - t1166) * t16) * t16 + (t9 * (t1186 + t1192 + t1202 - t148 - t42
     #0 - t430) * t29 - t9 * (t148 + t420 + t430 - t1210 - t1216 - t1226
     #) * t29) * t29 + (t9 * (t1236 + t1244 + t1250 - t148 - t420 - t430
     #) * t42 - t9 * (t148 + t420 + t430 - t1258 - t1266 - t1272) * t42)
     # * t42 + (t543 - t9 * (t431 - t1167) * t16) * t16 + (t9 * (t1283 -
     # t431) * t29 - t9 * (t431 - t1287) * t29) * t29 + (t9 * (t1293 - t
     #431) * t42 - t9 * (t431 - t1297) * t42) * t42 + (t645 - t649) * t3
     #99)
        t1308 = t443 * t1306 / 0.12E2
        t1309 = ut(t1140,j,k,n)
        t1310 = t271 - t1309
        t1314 = (-t1310 * t16 * t9 + t288) * t16
        t1315 = ut(t132,t25,k,n)
        t1319 = ut(t132,t31,k,n)
        t1325 = ut(t132,j,t38,n)
        t1329 = ut(t132,j,t44,n)
        t1349 = t588 * (t654 / 0.2E1 + (t652 - cc * (t1314 + (t9 * (t131
     #5 - t271) * t29 - t9 * (t271 - t1319) * t29) * t29 + (t9 * (t1325 
     #- t271) * t42 - t9 * (t271 - t1329) * t42) * t42 + (src(t132,j,k,n
     #Comp,t396) - t1167) * t399 / 0.2E1 + (t1167 - src(t132,j,k,nComp,t
     #402)) * t399 / 0.2E1)) * t16 / 0.2E1)
        t1351 = t246 * t1349 / 0.8E1
        t1353 = t8 * (t435 - t1171)
        t1355 = t7 * t1353 / 0.24E2
        t1358 = t247 * (t90 + t100 + t110 + t111 - t148 - t420 - t430 - 
     #t431) * t16
        t1363 = t444 * (t258 + t307 + t356 + t401 + t406 - t290 - t632 -
     # t642 - t646 - t650) * t16
        t1382 = t422 * t42
        t1385 = t426 * t42
        t1387 = (t1382 - t1385) * t42
        t1409 = t412 * t29
        t1412 = t416 * t29
        t1414 = (t1409 - t1412) * t29
        t1436 = t120 * (-t121 * ((t140 - t9 * (t137 - (-t1142 * t16 + t1
     #35) * t16) * t16) * t16 + (t150 - (t148 - t1146) * t16) * t16) / 0
     #.24E2 + t148 + t430 - t198 * ((t9 * ((t1246 * t42 - t1382) * t42 -
     # t1387) * t42 - t9 * (t1387 - (-t1268 * t42 + t1385) * t42) * t42)
     # * t42 + ((t1250 - t430) * t42 - (t430 - t1272) * t42) * t42) / 0.
     #24E2 - t156 * ((t9 * ((t1188 * t29 - t1409) * t29 - t1414) * t29 -
     # t9 * (t1414 - (-t1212 * t29 + t1412) * t29) * t29) * t29 + ((t119
     #2 - t420) * t29 - (t420 - t1216) * t29) * t29) / 0.24E2 + t420 + t
     #431)
        t1439 = ut(t84,j,t199,n)
        t1440 = t1439 - t633
        t1442 = t634 * t42
        t1445 = t638 * t42
        t1447 = (t1442 - t1445) * t42
        t1451 = ut(t84,j,t212,n)
        t1452 = t637 - t1451
        t1478 = ut(t84,t157,k,n)
        t1479 = t1478 - t623
        t1481 = t624 * t29
        t1484 = t628 * t29
        t1486 = (t1481 - t1484) * t29
        t1490 = ut(t84,t170,k,n)
        t1491 = t627 - t1490
        t1533 = t248 * (-t198 * ((t9 * ((t1440 * t42 - t1442) * t42 - t1
     #447) * t42 - t9 * (t1447 - (-t1452 * t42 + t1445) * t42) * t42) * 
     #t42 + (((t1440 * t42 * t9 - t636) * t42 - t642) * t42 - (t642 - (-
     #t1452 * t42 * t9 + t640) * t42) * t42) * t42) / 0.24E2 + t642 + t6
     #32 - t156 * ((t9 * ((t1479 * t29 - t1481) * t29 - t1486) * t29 - t
     #9 * (t1486 - (-t1491 * t29 + t1484) * t29) * t29) * t29 + (((t1479
     # * t29 * t9 - t626) * t29 - t632) * t29 - (t632 - (-t1491 * t29 * 
     #t9 + t630) * t29) * t29) * t29) / 0.24E2 - t121 * ((t278 - t9 * (t
     #275 - (-t1310 * t16 + t273) * t16) * t16) * t16 + (t292 - (t290 - 
     #t1314) * t16) * t16) / 0.24E2 + t290 + t646 + t650)
        t1545 = t1046 * t1358 / 0.2E1
        t1547 = t1049 * t1363 / 0.6E1
        t1549 = t7 * t1436 / 0.2E1
        t1551 = t246 * t1533 / 0.4E1
        t1552 = t1176 + t1308 + t1351 + t1355 + t665 * t1358 / 0.2E1 + t
     #672 * t1363 / 0.6E1 - t678 * t1436 / 0.2E1 - t776 * t1533 / 0.4E1 
     #- t678 * t1174 / 0.4E1 - t886 * t1306 / 0.12E2 - t776 * t1349 / 0.
     #8E1 - t678 * t1353 / 0.24E2 - t1545 - t1547 + t1549 + t1551
        t1553 = dx * t291
        t1557 = t572 * t1553 / 0.24E2
        t1561 = dt * (t265 - dx * t276 / 0.24E2)
        t1563 = t586 * t1561
        t1564 = -t243 - t410 + t440 - t571 - t577 * t1553 / 0.24E2 + t15
     #57 + t580 * t1561 - t1563 + t659 - t663 + t1035 + t1037 - t1039 + 
     #t1041 - t1043 + t1045
        t1566 = (t1552 + t1564) * t4
        t1572 = t9 * (t126 - dx * t138 / 0.24E2)
        t1579 = (t1115 - (t1113 - (-cc * t1309 + t1111) * t16) * t16) * 
     #t16
        t1585 = t121 * (t1115 - dx * (t1117 - t1579) / 0.12E2) / 0.24E2
        t1593 = dx * (t1125 + t1113 / 0.2E1 - t121 * (t1117 / 0.2E1 + t1
     #579 / 0.2E1) / 0.6E1) / 0.4E1
        t1595 = dx * t149 / 0.24E2
        t1596 = t1572 - t1585 - t1593 - t1595 - t1176 - t1308 - t1351 - 
     #t1355 + t1545 + t1547 - t1549 - t1551
        t1598 = t1083 / 0.2E1
        t1599 = -t1566 * t6 + t1123 - t1132 + t1134 - t1557 + t1563 - t1
     #598 + t243 + t410 - t440 + t571 - t659 + t663
        t1605 = t247 * dy
        t1614 = j + 3
        t1615 = ut(i,t1614,k,n)
        t1616 = t1615 - t308
        t1620 = (t1616 * t29 * t9 - t331) * t29
        t1621 = ut(i,t157,t38,n)
        t1625 = ut(i,t157,t44,n)
        t1632 = src(i,t157,k,nComp,n)
        t1642 = t589 - t298
        t1644 = t9 * t1642 * t16
        t1645 = t298 - t623
        t1647 = t9 * t1645 * t16
        t1649 = (t1644 - t1647) * t16
        t1650 = ut(i,t25,t38,n)
        t1651 = t1650 - t298
        t1653 = t9 * t1651 * t42
        t1654 = ut(i,t25,t44,n)
        t1655 = t298 - t1654
        t1657 = t9 * t1655 * t42
        t1659 = (t1653 - t1657) * t42
        t1662 = (src(i,t25,k,nComp,t396) - t546) * t399
        t1663 = t1662 / 0.2E1
        t1666 = (t546 - src(i,t25,k,nComp,t402)) * t399
        t1667 = t1666 / 0.2E1
        t1669 = cc * (t1649 + t333 + t1659 + t1663 + t1667)
        t1673 = (t1669 - t620) * t29
        t1676 = t1605 * ((cc * ((t9 * (t777 - t308) * t16 - t9 * (t308 -
     # t1478) * t16) * t16 + t1620 + (t9 * (t1621 - t308) * t42 - t9 * (
     #t308 - t1625) * t42) * t42 + (src(i,t157,k,nComp,t396) - t1632) * 
     #t399 / 0.2E1 + (t1632 - src(i,t157,k,nComp,t402)) * t399 / 0.2E1) 
     #- t1669) * t29 / 0.2E1 + t1673 / 0.2E1)
        t1678 = t246 * t1676 / 0.8E1
        t1679 = dt * dy
        t1687 = (t9 * (t694 - t158) * t16 - t9 * (t158 - t1187) * t16) *
     # t16
        t1689 = u(i,t1614,k,n) - t158
        t1693 = (t1689 * t29 * t9 - t182) * t29
        t1694 = u(i,t157,t38,n)
        t1698 = u(i,t157,t44,n)
        t1703 = (t9 * (t1694 - t158) * t42 - t9 * (t158 - t1698) * t42) 
     #* t42
        t1707 = cc * (t461 + t184 + t471 + t546)
        t1709 = (cc * (t1687 + t1693 + t1703 + t1632) - t1707) * t29
        t1711 = (t1707 - t113) * t29
        t1713 = t1679 * (t1709 - t1711)
        t1715 = t7 * t1713 / 0.24E2
        t1717 = cc * (t482 + t190 + t492 + t550)
        t1719 = (t113 - t1717) * t29
        t1722 = t1679 * (t1711 / 0.2E1 + t1719 / 0.2E1)
        t1724 = t7 * t1722 / 0.4E1
        t1725 = t593 - t302
        t1727 = t9 * t1725 * t16
        t1728 = t302 - t627
        t1730 = t9 * t1728 * t16
        t1732 = (t1727 - t1730) * t16
        t1733 = ut(i,t31,t38,n)
        t1734 = t1733 - t302
        t1736 = t9 * t1734 * t42
        t1737 = ut(i,t31,t44,n)
        t1738 = t302 - t1737
        t1740 = t9 * t1738 * t42
        t1742 = (t1736 - t1740) * t42
        t1745 = (src(i,t31,k,nComp,t396) - t550) * t399
        t1746 = t1745 / 0.2E1
        t1749 = (t550 - src(i,t31,k,nComp,t402)) * t399
        t1750 = t1749 / 0.2E1
        t1752 = cc * (t1732 + t339 + t1742 + t1746 + t1750)
        t1754 = (t620 - t1752) * t29
        t1757 = t1605 * (t1673 / 0.2E1 + t1754 / 0.2E1)
        t1759 = t246 * t1757 / 0.8E1
        t1761 = t1679 * (t1711 - t1719)
        t1763 = t7 * t1761 / 0.24E2
        t1766 = t247 * (t461 + t184 + t471 + t546 - t90 - t100 - t110 - 
     #t111) * t29
        t1771 = t444 * (t1649 + t333 + t1659 + t1663 + t1667 - t258 - t3
     #07 - t356 - t401 - t406) * t29
        t1774 = u(i,t25,t199,n)
        t1775 = t1774 - t462
        t1777 = t463 * t42
        t1780 = t467 * t42
        t1782 = (t1777 - t1780) * t42
        t1786 = u(i,t25,t212,n)
        t1787 = t466 - t1786
        t1799 = (t1775 * t42 * t9 - t465) * t42
        t1805 = (-t1787 * t42 * t9 + t469) * t42
        t1829 = t454 * t16
        t1832 = t457 * t16
        t1834 = (t1829 - t1832) * t16
        t1856 = t120 * (t471 - t198 * ((t9 * ((t1775 * t42 - t1777) * t4
     #2 - t1782) * t42 - t9 * (t1782 - (-t1787 * t42 + t1780) * t42) * t
     #42) * t42 + ((t1799 - t471) * t42 - (t471 - t1805) * t42) * t42) /
     # 0.24E2 + t184 - t156 * ((t9 * ((t1689 * t29 - t160) * t29 - t163)
     # * t29 - t169) * t29 + ((t1693 - t184) * t29 - t186) * t29) / 0.24
     #E2 - t121 * ((t9 * ((t16 * t892 - t1829) * t16 - t1834) * t16 - t9
     # * (t1834 - (-t1182 * t16 + t1832) * t16) * t16) * t16 + ((t896 - 
     #t461) * t16 - (t461 - t1186) * t16) * t16) / 0.24E2 + t461 + t546)
        t1859 = ut(i,t25,t199,n)
        t1860 = t1859 - t1650
        t1862 = t1651 * t42
        t1865 = t1655 * t42
        t1867 = (t1862 - t1865) * t42
        t1871 = ut(i,t25,t212,n)
        t1872 = t1654 - t1871
        t1913 = t995 - t589
        t1915 = t1642 * t16
        t1918 = t1645 * t16
        t1920 = (t1915 - t1918) * t16
        t1924 = t623 - t1315
        t1951 = t248 * (t1659 - t198 * ((t9 * ((t1860 * t42 - t1862) * t
     #42 - t1867) * t42 - t9 * (t1867 - (-t1872 * t42 + t1865) * t42) * 
     #t42) * t42 + (((t1860 * t42 * t9 - t1653) * t42 - t1659) * t42 - (
     #t1659 - (-t1872 * t42 * t9 + t1657) * t42) * t42) * t42) / 0.24E2 
     #- t156 * ((t9 * ((t1616 * t29 - t310) * t29 - t313) * t29 - t319) 
     #* t29 + ((t1620 - t333) * t29 - t335) * t29) / 0.24E2 + t333 - t12
     #1 * ((t9 * ((t16 * t1913 - t1915) * t16 - t1920) * t16 - t9 * (t19
     #20 - (-t16 * t1924 + t1918) * t16) * t16) * t16 + (((t16 * t1913 *
     # t9 - t1644) * t16 - t1649) * t16 - (t1649 - (-t16 * t1924 * t9 + 
     #t1647) * t16) * t16) * t16) / 0.24E2 + t1649 + t1663 + t1667)
        t1956 = t1679 * (t1709 / 0.2E1 + t1711 / 0.2E1)
        t1979 = (t9 * (t897 - t462) * t16 - t9 * (t462 - t1193) * t16) *
     # t16
        t1980 = t1694 - t462
        t1984 = (t1980 * t29 * t9 - t508) * t29
        t1995 = (t9 * (t901 - t466) * t16 - t9 * (t466 - t1197) * t16) *
     # t16
        t1996 = t1698 - t466
        t2000 = (t1996 * t29 * t9 - t527) * t29
        t2019 = src(i,t25,t38,nComp,n)
        t2023 = src(i,t25,t44,nComp,n)
        t2032 = t445 * ((t9 * (t896 + t719 + t906 - t461 - t184 - t471) 
     #* t16 - t9 * (t461 + t184 + t471 - t1186 - t1192 - t1202) * t16) *
     # t16 + (t9 * (t1687 + t1693 + t1703 - t461 - t184 - t471) * t29 - 
     #t474) * t29 + (t9 * (t1979 + t1984 + t1799 - t461 - t184 - t471) *
     # t42 - t9 * (t461 + t184 + t471 - t1995 - t2000 - t1805) * t42) * 
     #t42 + (t9 * (t969 - t546) * t16 - t9 * (t546 - t1283) * t16) * t16
     # + (t9 * (t1632 - t546) * t29 - t549) * t29 + (t9 * (t2019 - t546)
     # * t42 - t9 * (t546 - t2023) * t42) * t42 + (t1662 - t1666) * t399
     #)
        t2039 = t243 + t410 + t571 + t1678 - t1715 + t1724 + t1759 + t17
     #63 + t665 * t1766 / 0.2E1 + t672 * t1771 / 0.6E1 + t678 * t1856 / 
     #0.2E1 + t776 * t1951 / 0.4E1 - t678 * t1956 / 0.4E1 + t886 * t2032
     # / 0.12E2 - t776 * t1676 / 0.8E1 + t678 * t1713 / 0.24E2
        t2041 = t678 * t1722 / 0.4E1
        t2043 = t776 * t1757 / 0.8E1
        t2045 = t678 * t1761 / 0.24E2
        t2047 = t1046 * t1766 / 0.2E1
        t2049 = t1049 * t1771 / 0.6E1
        t2051 = t7 * t1856 / 0.2E1
        t2055 = dt * (t311 - dy * t317 / 0.24E2)
        t2057 = t586 * t2055
        t2058 = dy * t334
        t2062 = t572 * t2058 / 0.24E2
        t2064 = t246 * t1951 / 0.4E1
        t2066 = t7 * t1956 / 0.4E1
        t2068 = t443 * t2032 / 0.12E2
        t2069 = -t2041 - t2043 - t2045 - t2047 - t2049 - t2051 + t580 * 
     #t2055 - t2057 - t577 * t2058 / 0.24E2 + t2062 - t2064 + t2066 - t2
     #068 - t1035 - t1037 - t1041
        t2071 = (t2039 + t2069) * t4
        t2075 = cc * t298
        t2076 = t2075 / 0.2E1
        t2078 = (-t1071 + t2075) * t29
        t2079 = t2078 / 0.2E1
        t2080 = cc * t302
        t2082 = (t1071 - t2080) * t29
        t2083 = t2082 / 0.2E1
        t2084 = cc * t308
        t2086 = (-t2075 + t2084) * t29
        t2088 = (t2086 - t2078) * t29
        t2090 = (t2078 - t2082) * t29
        t2092 = (t2088 - t2090) * t29
        t2093 = cc * t320
        t2095 = (-t2093 + t2080) * t29
        t2097 = (t2082 - t2095) * t29
        t2099 = (t2090 - t2097) * t29
        t2106 = dy * (t2079 + t2083 - t156 * (t2092 / 0.2E1 + t2099 / 0.
     #2E1) / 0.6E1) / 0.4E1
        t2112 = t156 * (t2090 - dy * (t2092 - t2099) / 0.12E2) / 0.24E2
        t2119 = (((cc * t1615 - t2084) * t29 - t2086) * t29 - t2088) * t
     #29
        t2125 = t156 * (t2088 - dy * (t2119 - t2092) / 0.12E2) / 0.24E2
        t2129 = t9 * (t161 - dy * t167 / 0.24E2)
        t2130 = -t2071 * t6 - t1678 + t1715 - t1724 + t2076 - t2106 - t2
     #112 + t2125 + t2129 - t243 - t410 - t571
        t2138 = dy * (t2086 / 0.2E1 + t2079 - t156 * (t2119 / 0.2E1 + t2
     #092 / 0.2E1) / 0.6E1) / 0.4E1
        t2140 = dy * t185 / 0.24E2
        t2141 = -t1759 - t1763 + t2047 + t2049 + t2051 + t2057 - t2062 +
     # t2064 - t2066 + t2068 - t2138 - t2140 - t1134
        t2152 = (t9 * (t706 - t171) * t16 - t9 * (t171 - t1211) * t16) *
     # t16
        t2153 = j - 3
        t2155 = t171 - u(i,t2153,k,n)
        t2159 = (-t2155 * t29 * t9 + t188) * t29
        t2160 = u(i,t170,t38,n)
        t2164 = u(i,t170,t44,n)
        t2169 = (t9 * (t2160 - t171) * t42 - t9 * (t171 - t2164) * t42) 
     #* t42
        t2170 = src(i,t170,k,nComp,n)
        t2174 = (t1717 - cc * (t2152 + t2159 + t2169 + t2170)) * t29
        t2176 = t1679 * (t1719 - t2174)
        t2178 = t7 * t2176 / 0.24E2
        t2179 = u(i,t31,t199,n)
        t2180 = t2179 - t483
        t2182 = t484 * t42
        t2185 = t488 * t42
        t2187 = (t2182 - t2185) * t42
        t2191 = u(i,t31,t212,n)
        t2192 = t487 - t2191
        t2204 = (t2180 * t42 * t9 - t486) * t42
        t2210 = (-t2192 * t42 * t9 + t490) * t42
        t2234 = t475 * t16
        t2237 = t478 * t16
        t2239 = (t2234 - t2237) * t16
        t2261 = t120 * (t492 - t198 * ((t9 * ((t2180 * t42 - t2182) * t4
     #2 - t2187) * t42 - t9 * (t2187 - (-t2192 * t42 + t2185) * t42) * t
     #42) * t42 + ((t2204 - t492) * t42 - (t492 - t2210) * t42) * t42) /
     # 0.24E2 - t156 * ((t178 - t9 * (t175 - (-t2155 * t29 + t173) * t29
     #) * t29) * t29 + (t192 - (t190 - t2159) * t29) * t29) / 0.24E2 + t
     #190 + t482 - t121 * ((t9 * ((t16 * t910 - t2234) * t16 - t2239) * 
     #t16 - t9 * (t2239 - (-t1206 * t16 + t2237) * t16) * t16) * t16 + (
     #(t914 - t482) * t16 - (t482 - t1210) * t16) * t16) / 0.24E2 + t550
     #)
        t2264 = ut(i,t31,t199,n)
        t2265 = t2264 - t1733
        t2267 = t1734 * t42
        t2270 = t1738 * t42
        t2272 = (t2267 - t2270) * t42
        t2276 = ut(i,t31,t212,n)
        t2277 = t1737 - t2276
        t2303 = ut(i,t2153,k,n)
        t2304 = t320 - t2303
        t2316 = (-t2304 * t29 * t9 + t337) * t29
        t2324 = t999 - t593
        t2326 = t1725 * t16
        t2329 = t1728 * t16
        t2331 = (t2326 - t2329) * t16
        t2335 = t627 - t1319
        t2362 = t248 * (-t198 * ((t9 * ((t2265 * t42 - t2267) * t42 - t2
     #272) * t42 - t9 * (t2272 - (-t2277 * t42 + t2270) * t42) * t42) * 
     #t42 + (((t2265 * t42 * t9 - t1736) * t42 - t1742) * t42 - (t1742 -
     # (-t2277 * t42 * t9 + t1740) * t42) * t42) * t42) / 0.24E2 + t1742
     # - t156 * ((t327 - t9 * (t324 - (-t2304 * t29 + t322) * t29) * t29
     #) * t29 + (t341 - (t339 - t2316) * t29) * t29) / 0.24E2 + t339 + t
     #1732 - t121 * ((t9 * ((t16 * t2324 - t2326) * t16 - t2331) * t16 -
     # t9 * (t2331 - (-t16 * t2335 + t2329) * t16) * t16) * t16 + (((t16
     # * t2324 * t9 - t1727) * t16 - t1732) * t16 - (t1732 - (-t16 * t23
     #35 * t9 + t1730) * t16) * t16) * t16) / 0.24E2 + t1746 + t1750)
        t2367 = t1679 * (t1719 / 0.2E1 + t2174 / 0.2E1)
        t2390 = (t9 * (t915 - t483) * t16 - t9 * (t483 - t1217) * t16) *
     # t16
        t2391 = t483 - t2160
        t2395 = (-t2391 * t29 * t9 + t511) * t29
        t2406 = (t9 * (t919 - t487) * t16 - t9 * (t487 - t1221) * t16) *
     # t16
        t2407 = t487 - t2164
        t2411 = (-t2407 * t29 * t9 + t530) * t29
        t2430 = src(i,t31,t38,nComp,n)
        t2434 = src(i,t31,t44,nComp,n)
        t2443 = t445 * ((t9 * (t914 + t725 + t924 - t482 - t190 - t492) 
     #* t16 - t9 * (t482 + t190 + t492 - t1210 - t1216 - t1226) * t16) *
     # t16 + (t495 - t9 * (t482 + t190 + t492 - t2152 - t2159 - t2169) *
     # t29) * t29 + (t9 * (t2390 + t2395 + t2204 - t482 - t190 - t492) *
     # t42 - t9 * (t482 + t190 + t492 - t2406 - t2411 - t2210) * t42) * 
     #t42 + (t9 * (t973 - t550) * t16 - t9 * (t550 - t1287) * t16) * t16
     # + (t553 - t9 * (t550 - t2170) * t29) * t29 + (t9 * (t2430 - t550)
     # * t42 - t9 * (t550 - t2434) * t42) * t42 + (t1745 - t1749) * t399
     #)
        t2454 = ut(i,t170,t38,n)
        t2458 = ut(i,t170,t44,n)
        t2478 = t1605 * (t1754 / 0.2E1 + (t1752 - cc * ((t9 * (t789 - t3
     #20) * t16 - t9 * (t320 - t1490) * t16) * t16 + t2316 + (t9 * (t245
     #4 - t320) * t42 - t9 * (t320 - t2458) * t42) * t42 + (src(i,t170,k
     #,nComp,t396) - t2170) * t399 / 0.2E1 + (t2170 - src(i,t170,k,nComp
     #,t402)) * t399 / 0.2E1)) * t29 / 0.2E1)
        t2485 = t247 * (t90 + t100 + t110 + t111 - t482 - t190 - t492 - 
     #t550) * t29
        t2487 = t1046 * t2485 / 0.2E1
        t2490 = t444 * (t258 + t307 + t356 + t401 + t406 - t1732 - t339 
     #- t1742 - t1746 - t1750) * t29
        t2492 = t1049 * t2490 / 0.6E1
        t2494 = t7 * t2261 / 0.2E1
        t2496 = t246 * t2362 / 0.4E1
        t2498 = t7 * t2367 / 0.4E1
        t2500 = t443 * t2443 / 0.12E2
        t2501 = -t243 - t410 - t571 + t2178 - t678 * t2261 / 0.2E1 - t77
     #6 * t2362 / 0.4E1 - t678 * t2367 / 0.4E1 - t886 * t2443 / 0.12E2 -
     # t776 * t2478 / 0.8E1 - t678 * t2176 / 0.24E2 - t2487 - t2492 + t2
     #494 + t2496 + t2498 + t2500
        t2503 = t246 * t2478 / 0.8E1
        t2511 = dt * (t314 - dy * t325 / 0.24E2)
        t2513 = t586 * t2511
        t2514 = dy * t340
        t2518 = t572 * t2514 / 0.24E2
        t2519 = t2503 + t665 * t2485 / 0.2E1 + t672 * t2490 / 0.6E1 + t1
     #724 + t1759 - t1763 - t2041 - t2043 + t2045 + t580 * t2511 - t2513
     # - t577 * t2514 / 0.24E2 + t2518 + t1035 + t1037 + t1041
        t2521 = (t2501 + t2519) * t4
        t2524 = t2080 / 0.2E1
        t2533 = (t2097 - (t2095 - (-cc * t2303 + t2093) * t29) * t29) * 
     #t29
        t2540 = dy * (t2083 + t2095 / 0.2E1 - t156 * (t2099 / 0.2E1 + t2
     #533 / 0.2E1) / 0.6E1) / 0.4E1
        t2546 = t156 * (t2097 - dy * (t2099 - t2533) / 0.12E2) / 0.24E2
        t2548 = dy * t191 / 0.24E2
        t2549 = -t2521 * t6 - t2106 + t2112 - t2178 + t243 + t2487 - t25
     #24 - t2540 - t2546 - t2548 + t410 + t571
        t2553 = t9 * (t164 - dy * t176 / 0.24E2)
        t2554 = t2492 - t2494 - t2496 - t2498 - t2500 - t2503 - t1724 - 
     #t1759 + t1763 + t2553 + t1134 + t2513 - t2518
        t2563 = dt * (t360 - dz * t366 / 0.24E2)
        t2565 = t586 * t2563
        t2566 = dz * t383
        t2568 = t572 * t2566 / 0.24E2
        t2571 = t247 * (t505 + t513 + t226 + t556 - t90 - t100 - t110 - 
     #t111) * t42
        t2574 = t599 - t347
        t2576 = t9 * t2574 * t16
        t2577 = t347 - t633
        t2579 = t9 * t2577 * t16
        t2581 = (t2576 - t2579) * t16
        t2582 = t1650 - t347
        t2584 = t9 * t2582 * t29
        t2585 = t347 - t1733
        t2587 = t9 * t2585 * t29
        t2589 = (t2584 - t2587) * t29
        t2592 = (src(i,j,t38,nComp,t396) - t556) * t399
        t2593 = t2592 / 0.2E1
        t2596 = (t556 - src(i,j,t38,nComp,t402)) * t399
        t2597 = t2596 / 0.2E1
        t2600 = t444 * (t2581 + t2589 + t382 + t2593 + t2597 - t258 - t3
     #07 - t356 - t401 - t406) * t42
        t2603 = k + 3
        t2605 = u(i,j,t2603,n) - t200
        t2617 = (t2605 * t42 * t9 - t224) * t42
        t2626 = t506 * t29
        t2629 = t509 * t29
        t2631 = (t2626 - t2629) * t29
        t2653 = t498 * t16
        t2656 = t501 * t16
        t2658 = (t2653 - t2656) * t16
        t2680 = t120 * (t226 - t198 * ((t9 * ((t2605 * t42 - t202) * t42
     # - t205) * t42 - t211) * t42 + ((t2617 - t226) * t42 - t228) * t42
     #) / 0.24E2 - t156 * ((t9 * ((t1980 * t29 - t2626) * t29 - t2631) *
     # t29 - t9 * (t2631 - (-t2391 * t29 + t2629) * t29) * t29) * t29 + 
     #((t1984 - t513) * t29 - (t513 - t2395) * t29) * t29) / 0.24E2 + t5
     #13 - t121 * ((t9 * ((t16 * t930 - t2653) * t16 - t2658) * t16 - t9
     # * (t2658 - (-t1232 * t16 + t2656) * t16) * t16) * t16 + ((t934 - 
     #t505) * t16 - (t505 - t1236) * t16) * t16) / 0.24E2 + t505 + t556)
        t2683 = ut(i,j,t2603,n)
        t2684 = t2683 - t357
        t2696 = (t2684 * t42 * t9 - t380) * t42
        t2704 = t1621 - t1650
        t2706 = t2582 * t29
        t2709 = t2585 * t29
        t2711 = (t2706 - t2709) * t29
        t2715 = t1733 - t2454
        t2741 = t1005 - t599
        t2743 = t2574 * t16
        t2746 = t2577 * t16
        t2748 = (t2743 - t2746) * t16
        t2752 = t633 - t1325
        t2779 = t248 * (t382 - t198 * ((t9 * ((t2684 * t42 - t359) * t42
     # - t362) * t42 - t368) * t42 + ((t2696 - t382) * t42 - t384) * t42
     #) / 0.24E2 - t156 * ((t9 * ((t2704 * t29 - t2706) * t29 - t2711) *
     # t29 - t9 * (t2711 - (-t2715 * t29 + t2709) * t29) * t29) * t29 + 
     #(((t2704 * t29 * t9 - t2584) * t29 - t2589) * t29 - (t2589 - (-t27
     #15 * t29 * t9 + t2587) * t29) * t29) * t29) / 0.24E2 + t2589 - t12
     #1 * ((t9 * ((t16 * t2741 - t2743) * t16 - t2748) * t16 - t9 * (t27
     #48 - (-t16 * t2752 + t2746) * t16) * t16) * t16 + (((t16 * t2741 *
     # t9 - t2576) * t16 - t2581) * t16 - (t2581 - (-t16 * t2752 * t9 + 
     #t2579) * t16) * t16) * t16) / 0.24E2 + t2581 + t2593 + t2597)
        t2782 = dt * dz
        t2790 = (t9 * (t733 - t200) * t16 - t9 * (t200 - t1245) * t16) *
     # t16
        t2798 = (t9 * (t1774 - t200) * t29 - t9 * (t200 - t2179) * t29) 
     #* t29
        t2799 = src(i,j,t199,nComp,n)
        t2803 = cc * (t505 + t513 + t226 + t556)
        t2805 = (cc * (t2790 + t2798 + t2617 + t2799) - t2803) * t42
        t2807 = (t2803 - t113) * t42
        t2810 = t2782 * (t2805 / 0.2E1 + t2807 / 0.2E1)
        t2858 = t445 * ((t9 * (t934 + t942 + t758 - t505 - t513 - t226) 
     #* t16 - t9 * (t505 + t513 + t226 - t1236 - t1244 - t1250) * t16) *
     # t16 + (t9 * (t1979 + t1984 + t1799 - t505 - t513 - t226) * t29 - 
     #t9 * (t505 + t513 + t226 - t2390 - t2395 - t2204) * t29) * t29 + (
     #t9 * (t2790 + t2798 + t2617 - t505 - t513 - t226) * t42 - t516) * 
     #t42 + (t9 * (t979 - t556) * t16 - t9 * (t556 - t1293) * t16) * t16
     # + (t9 * (t2019 - t556) * t29 - t9 * (t556 - t2430) * t29) * t29 +
     # (t9 * (t2799 - t556) * t42 - t559) * t42 + (t2592 - t2596) * t399
     #)
        t2861 = t247 * dz
        t2889 = cc * (t2581 + t2589 + t382 + t2593 + t2597)
        t2893 = (t2889 - t620) * t42
        t2896 = t2861 * ((cc * ((t9 * (t816 - t357) * t16 - t9 * (t357 -
     # t1439) * t16) * t16 + (t9 * (t1859 - t357) * t29 - t9 * (t357 - t
     #2264) * t29) * t29 + t2696 + (src(i,j,t199,nComp,t396) - t2799) * 
     #t399 / 0.2E1 + (t2799 - src(i,j,t199,nComp,t402)) * t399 / 0.2E1) 
     #- t2889) * t42 / 0.2E1 + t2893 / 0.2E1)
        t2900 = t2782 * (t2805 - t2807)
        t2904 = cc * (t524 + t532 + t232 + t560)
        t2906 = (t113 - t2904) * t42
        t2909 = t2782 * (t2807 / 0.2E1 + t2906 / 0.2E1)
        t2911 = t678 * t2909 / 0.4E1
        t2912 = t603 - t351
        t2914 = t9 * t2912 * t16
        t2915 = t351 - t637
        t2917 = t9 * t2915 * t16
        t2919 = (t2914 - t2917) * t16
        t2920 = t1654 - t351
        t2922 = t9 * t2920 * t29
        t2923 = t351 - t1737
        t2925 = t9 * t2923 * t29
        t2927 = (t2922 - t2925) * t29
        t2930 = (src(i,j,t44,nComp,t396) - t560) * t399
        t2931 = t2930 / 0.2E1
        t2934 = (t560 - src(i,j,t44,nComp,t402)) * t399
        t2935 = t2934 / 0.2E1
        t2937 = cc * (t2919 + t2927 + t388 + t2931 + t2935)
        t2939 = (t620 - t2937) * t42
        t2942 = t2861 * (t2893 / 0.2E1 + t2939 / 0.2E1)
        t2944 = t776 * t2942 / 0.8E1
        t2945 = t243 + t410 + t571 + t580 * t2563 - t2565 + t2568 + t665
     # * t2571 / 0.2E1 + t672 * t2600 / 0.6E1 + t678 * t2680 / 0.2E1 + t
     #776 * t2779 / 0.4E1 - t678 * t2810 / 0.4E1 + t886 * t2858 / 0.12E2
     # - t776 * t2896 / 0.8E1 + t678 * t2900 / 0.24E2 - t2911 - t2944
        t2947 = t2782 * (t2807 - t2906)
        t2949 = t678 * t2947 / 0.24E2
        t2951 = t1046 * t2571 / 0.2E1
        t2953 = t1049 * t2600 / 0.6E1
        t2955 = t7 * t2680 / 0.2E1
        t2959 = t246 * t2779 / 0.4E1
        t2961 = t7 * t2810 / 0.4E1
        t2963 = t443 * t2858 / 0.12E2
        t2965 = t246 * t2896 / 0.8E1
        t2967 = t7 * t2900 / 0.24E2
        t2969 = t7 * t2909 / 0.4E1
        t2971 = t246 * t2942 / 0.8E1
        t2973 = t7 * t2947 / 0.24E2
        t2974 = -t2949 - t2951 - t2953 - t2955 - t577 * t2566 / 0.24E2 -
     # t2959 + t2961 - t2963 + t2965 - t2967 + t2969 + t2971 + t2973 - t
     #1035 - t1037 - t1041
        t2976 = (t2945 + t2974) * t4
        t2979 = cc * t347
        t2980 = cc * t357
        t2982 = (-t2979 + t2980) * t42
        t2984 = (-t1071 + t2979) * t42
        t2986 = (t2982 - t2984) * t42
        t2993 = (((cc * t2683 - t2980) * t42 - t2982) * t42 - t2986) * t
     #42
        t2994 = cc * t351
        t2996 = (t1071 - t2994) * t42
        t2998 = (t2984 - t2996) * t42
        t3000 = (t2986 - t2998) * t42
        t3006 = t198 * (t2986 - dz * (t2993 - t3000) / 0.12E2) / 0.24E2
        t3008 = t2984 / 0.2E1
        t3015 = dz * (t2982 / 0.2E1 + t3008 - t198 * (t2993 / 0.2E1 + t3
     #000 / 0.2E1) / 0.6E1) / 0.4E1
        t3016 = t2979 / 0.2E1
        t3020 = t9 * (t203 - dz * t209 / 0.24E2)
        t3021 = t2996 / 0.2E1
        t3022 = cc * t369
        t3024 = (-t3022 + t2994) * t42
        t3026 = (t2996 - t3024) * t42
        t3028 = (t2998 - t3026) * t42
        t3035 = dz * (t3008 + t3021 - t198 * (t3000 / 0.2E1 + t3028 / 0.
     #2E1) / 0.6E1) / 0.4E1
        t3036 = t3006 - t3015 + t3016 + t3020 - t3035 - t243 - t410 - t5
     #71 + t2565 - t2568 + t2951 + t2953
        t3039 = dz * t227 / 0.24E2
        t3045 = t198 * (t2998 - dz * (t3000 - t3028) / 0.12E2) / 0.24E2
        t3046 = -t2976 * t6 - t1134 + t2955 + t2959 - t2961 + t2963 - t2
     #965 + t2967 - t2969 - t2971 - t2973 - t3039 - t3045
        t3052 = t444 * (t258 + t307 + t356 + t401 + t406 - t2919 - t2927
     # - t388 - t2931 - t2935) * t42
        t3054 = t1049 * t3052 / 0.6E1
        t3056 = t517 * t16
        t3059 = t520 * t16
        t3061 = (t3056 - t3059) * t16
        t3082 = k - 3
        t3084 = t213 - u(i,j,t3082,n)
        t3096 = (-t3084 * t42 * t9 + t230) * t42
        t3105 = t525 * t29
        t3108 = t528 * t29
        t3110 = (t3105 - t3108) * t29
        t3132 = t120 * (t524 - t121 * ((t9 * ((t16 * t946 - t3056) * t16
     # - t3061) * t16 - t9 * (t3061 - (-t1254 * t16 + t3059) * t16) * t1
     #6) * t16 + ((t950 - t524) * t16 - (t524 - t1258) * t16) * t16) / 0
     #.24E2 + t232 - t198 * ((t220 - t9 * (t217 - (-t3084 * t42 + t215) 
     #* t42) * t42) * t42 + (t234 - (t232 - t3096) * t42) * t42) / 0.24E
     #2 - t156 * ((t9 * ((t1996 * t29 - t3105) * t29 - t3110) * t29 - t9
     # * (t3110 - (-t2407 * t29 + t3108) * t29) * t29) * t29 + ((t2000 -
     # t532) * t29 - (t532 - t2411) * t29) * t29) / 0.24E2 + t532 + t560
     #)
        t3134 = t7 * t3132 / 0.2E1
        t3135 = t1009 - t603
        t3137 = t2912 * t16
        t3140 = t2915 * t16
        t3142 = (t3137 - t3140) * t16
        t3146 = t637 - t1329
        t3172 = ut(i,j,t3082,n)
        t3173 = t369 - t3172
        t3185 = (-t3173 * t42 * t9 + t386) * t42
        t3193 = t1625 - t1654
        t3195 = t2920 * t29
        t3198 = t2923 * t29
        t3200 = (t3195 - t3198) * t29
        t3204 = t1737 - t2458
        t3231 = t248 * (t2919 - t121 * ((t9 * ((t16 * t3135 - t3137) * t
     #16 - t3142) * t16 - t9 * (t3142 - (-t16 * t3146 + t3140) * t16) * 
     #t16) * t16 + (((t16 * t3135 * t9 - t2914) * t16 - t2919) * t16 - (
     #t2919 - (-t16 * t3146 * t9 + t2917) * t16) * t16) * t16) / 0.24E2 
     #- t198 * ((t376 - t9 * (t373 - (-t3173 * t42 + t371) * t42) * t42)
     # * t42 + (t390 - (t388 - t3185) * t42) * t42) / 0.24E2 + t388 + t2
     #927 - t156 * ((t9 * ((t29 * t3193 - t3195) * t29 - t3200) * t29 - 
     #t9 * (t3200 - (-t29 * t3204 + t3198) * t29) * t29) * t29 + (((t29 
     #* t3193 * t9 - t2922) * t29 - t2927) * t29 - (t2927 - (-t29 * t320
     #4 * t9 + t2925) * t29) * t29) * t29) / 0.24E2 + t2931 + t2935)
        t3233 = t246 * t3231 / 0.4E1
        t3241 = (t9 * (t745 - t213) * t16 - t9 * (t213 - t1267) * t16) *
     # t16
        t3249 = (t9 * (t1786 - t213) * t29 - t9 * (t213 - t2191) * t29) 
     #* t29
        t3250 = src(i,j,t212,nComp,n)
        t3254 = (t2904 - cc * (t3241 + t3249 + t3096 + t3250)) * t42
        t3257 = t2782 * (t2906 / 0.2E1 + t3254 / 0.2E1)
        t3259 = t7 * t3257 / 0.4E1
        t3305 = t445 * ((t9 * (t950 + t958 + t764 - t524 - t532 - t232) 
     #* t16 - t9 * (t524 + t532 + t232 - t1258 - t1266 - t1272) * t16) *
     # t16 + (t9 * (t1995 + t2000 + t1805 - t524 - t532 - t232) * t29 - 
     #t9 * (t524 + t532 + t232 - t2406 - t2411 - t2210) * t29) * t29 + (
     #t535 - t9 * (t524 + t532 + t232 - t3241 - t3249 - t3096) * t42) * 
     #t42 + (t9 * (t983 - t560) * t16 - t9 * (t560 - t1297) * t16) * t16
     # + (t9 * (t2023 - t560) * t29 - t9 * (t560 - t2434) * t29) * t29 +
     # (t563 - t9 * (t560 - t3250) * t42) * t42 + (t2930 - t2934) * t399
     #)
        t3307 = t443 * t3305 / 0.12E2
        t3338 = t2861 * (t2939 / 0.2E1 + (t2937 - cc * ((t9 * (t828 - t3
     #69) * t16 - t9 * (t369 - t1451) * t16) * t16 + (t9 * (t1871 - t369
     #) * t29 - t9 * (t369 - t2276) * t29) * t29 + t3185 + (src(i,j,t212
     #,nComp,t396) - t3250) * t399 / 0.2E1 + (t3250 - src(i,j,t212,nComp
     #,t402)) * t399 / 0.2E1)) * t42 / 0.2E1)
        t3340 = t246 * t3338 / 0.8E1
        t3342 = t2782 * (t2906 - t3254)
        t3344 = t7 * t3342 / 0.24E2
        t3347 = t247 * (t90 + t100 + t110 + t111 - t524 - t532 - t232 - 
     #t560) * t42
        t3365 = t1046 * t3347 / 0.2E1
        t3366 = -t3054 + t3134 + t3233 + t3259 + t3307 + t3340 + t3344 +
     # t665 * t3347 / 0.2E1 + t672 * t3052 / 0.6E1 - t678 * t3132 / 0.2E
     #1 - t776 * t3231 / 0.4E1 - t678 * t3257 / 0.4E1 - t886 * t3305 / 0
     #.12E2 - t776 * t3338 / 0.8E1 - t678 * t3342 / 0.24E2 - t3365
        t3367 = dz * t389
        t3371 = t572 * t3367 / 0.24E2
        t3375 = dt * (t363 - dz * t374 / 0.24E2)
        t3377 = t586 * t3375
        t3378 = -t243 - t410 - t571 - t2911 - t2944 + t2949 + t2969 + t2
     #971 - t2973 + t1035 + t1037 + t1041 - t577 * t3367 / 0.24E2 + t337
     #1 + t580 * t3375 - t3377
        t3380 = (t3366 + t3378) * t4
        t3383 = -t3035 + t3054 - t3134 - t3233 - t3259 - t3307 - t3340 -
     # t3344 + t3365 + t243 + t410 + t571
        t3384 = t2994 / 0.2E1
        t3386 = dz * t233 / 0.24E2
        t3393 = (t3026 - (t3024 - (-cc * t3172 + t3022) * t42) * t42) * 
     #t42
        t3399 = t198 * (t3026 - dz * (t3028 - t3393) / 0.12E2) / 0.24E2
        t3403 = t9 * (t206 - dz * t218 / 0.24E2)
        t3411 = dz * (t3021 + t3024 / 0.2E1 - t198 * (t3028 / 0.2E1 + t3
     #393 / 0.2E1) / 0.6E1) / 0.4E1
        t3413 = -t3380 * t6 + t1134 - t2969 - t2971 + t2973 + t3045 - t3
     #371 + t3377 - t3384 - t3386 - t3399 + t3403 - t3411
        t3422 = src(i,j,k,nComp,n + 2)
        t3424 = (src(i,j,k,nComp,n + 3) - t3422) * t4
        t3434 = t1101 + t587 + t1048 - t1097 + t1051 - t575 + t1135 + t1
     #053 - t1110 + t1055 - t1057 + t1095
        t3435 = t1059 - t1061 + t119 - t1134 - t243 - t1132 - t410 - t44
     #0 - t1123 - t571 - t659 - t663
        t3441 = t1572 + t1563 + t1545 - t1595 + t1547 - t1557 + t1134 + 
     #t243 - t1132 + t410 - t440 + t1123
        t3442 = t571 - t659 + t663 - t1598 - t1549 - t1593 - t1551 - t11
     #76 - t1585 - t1308 - t1351 - t1355
        t3450 = t2129 + t2057 + t2047 - t2140 + t2049 - t2062 + t2076 + 
     #t2051 - t2138 + t2064 - t2066 + t2125
        t3451 = t2068 - t1678 + t1715 - t1134 - t243 - t2106 - t410 - t1
     #724 - t2112 - t571 - t1759 - t1763
        t3457 = t2553 + t2513 + t2487 - t2548 + t2492 - t2518 + t1134 + 
     #t243 - t2106 + t410 - t1724 + t2112
        t3458 = t571 - t1759 + t1763 - t2524 - t2494 - t2540 - t2496 - t
     #2498 - t2546 - t2500 - t2503 - t2178
        t3466 = t3020 + t2565 + t2951 - t3039 + t2953 - t2568 + t3016 + 
     #t2955 - t3015 + t2959 - t2961 + t3006
        t3467 = t2963 - t2965 + t2967 - t1134 - t243 - t3035 - t410 - t2
     #969 - t3045 - t571 - t2971 - t2973
        t3473 = t3403 + t3377 + t3365 - t3386 + t3054 - t3371 + t1134 + 
     #t243 - t3035 + t410 - t2969 + t3045
        t3474 = t571 - t2971 + t2973 - t3384 - t3134 - t3411 - t3233 - t
     #3259 - t3399 - t3307 - t3340 - t3344

        unew(i,j,k) = t1 + dt * t2 + (t1064 * t247 / 0.6E1 + (t1124 +
     # t1136) * t247 / 0.2E1 - t1566 * t247 / 0.6E1 - (t1596 + t1599) * 
     #t247 / 0.2E1) * t16 + (t2071 * t247 / 0.6E1 + (t2130 + t2141) * t2
     #47 / 0.2E1 - t2521 * t247 / 0.6E1 - (t2549 + t2554) * t247 / 0.2E1
     #) * t29 + (t2976 * t247 / 0.6E1 + (t3036 + t3046) * t247 / 0.2E1 -
     # t3380 * t247 / 0.6E1 - (t3383 + t3413) * t247 / 0.2E1) * t42 + t3
     #424 * t247 / 0.6E1 + (-t3424 * t6 + t3422) * t247 / 0.2E1
        utnew(i,j,k) = t2 
     #+ (t1064 * dt / 0.2E1 + (t3434 + t3435) * dt - t1064 * t572 - t156
     #6 * dt / 0.2E1 - (t3441 + t3442) * dt + t1566 * t572) * t16 + (t20
     #71 * dt / 0.2E1 + (t3450 + t3451) * dt - t2071 * t572 - t2521 * dt
     # / 0.2E1 - (t3457 + t3458) * dt + t2521 * t572) * t29 + (t2976 * d
     #t / 0.2E1 + (t3466 + t3467) * dt - t2976 * t572 - t3380 * dt / 0.2
     #E1 - (t3473 + t3474) * dt + t3380 * t572) * t42 + t3424 * dt / 0.2
     #E1 + t3422 * dt - t3424 * t572

        return
      end
