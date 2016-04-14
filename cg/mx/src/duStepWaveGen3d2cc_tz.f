      subroutine duStepWaveGen3d2cc_tz( 
     *   nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *   n1a,n1b,n2a,n2b,n3a,n3b,
     *   ndf4a,ndf4b,nComp,
     *   u,ut,unew,utnew,
     *   rx,src,
     *   dx,dy,dz,dt,cc,
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
      real rx   (nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:2,0:2)
      real src  (nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,ndf4a:ndf4b,1:*)
      real dx,dy,dz,dt,cc
c
c.. generated code to follow
c
        real t1
        real t100
        real t1001
        real t1005
        real t1010
        real t1013
        real t1027
        real t1028
        real t1030
        real t1040
        real t1041
        real t1043
        real t1045
        real t1047
        real t1049
        real t1050
        real t1054
        real t1056
        real t106
        real t1062
        real t1063
        real t107
        real t1075
        real t1077
        real t108
        real t1081
        real t1082
        real t1084
        real t1086
        real t1088
        real t109
        real t1090
        real t1091
        real t1095
        real t1097
        real t11
        real t110
        real t1103
        real t1104
        real t111
        real t1118
        real t1119
        real t1120
        real t1123
        real t1124
        real t1125
        real t1127
        real t1132
        real t1133
        real t1134
        real t1147
        real t115
        real t1150
        real t1153
        real t1154
        real t1162
        real t1165
        real t117
        real t1170
        real t1173
        real t1177
        real t1183
        real t1184
        real t1186
        real t1188
        real t1190
        real t1192
        real t1193
        real t1197
        real t1199
        real t120
        real t1205
        real t1206
        real t121
        real t1218
        real t1222
        real t1223
        real t1224
        real t1225
        real t1227
        real t1229
        real t1231
        real t1232
        real t1236
        real t1238
        real t1240
        real t1244
        real t1245
        integer t126
        real t1268
        real t127
        real t1272
        real t1285
        real t1288
        real t129
        real t1291
        real t1292
        real t1293
        real t1296
        real t1297
        real t1298
        real t13
        real t1300
        real t1305
        real t1306
        real t1307
        integer t131
        real t1316
        real t132
        real t1324
        real t1334
        real t1339
        real t1343
        real t1346
        real t135
        real t1354
        real t1359
        real t1363
        real t1366
        real t1374
        real t1383
        real t1384
        real t139
        real t1391
        real t1392
        real t1398
        real t1405
        real t1407
        real t1410
        real t1411
        real t1412
        real t1415
        real t1416
        real t1417
        real t1419
        real t1424
        real t1425
        real t1426
        real t143
        integer t1435
        real t1436
        real t144
        real t1443
        real t1445
        real t1447
        real t1449
        real t145
        real t1453
        real t146
        real t147
        real t1474
        real t1487
        real t1488
        real t149
        real t1490
        real t1492
        real t1494
        real t1496
        real t1497
        real t15
        real t1501
        real t1503
        real t1509
        real t151
        real t1510
        real t1526
        real t1527
        real t1528
        real t1529
        real t153
        real t1541
        real t1544
        real t1554
        real t1555
        real t1557
        real t1559
        real t1561
        real t1563
        real t1564
        real t1568
        real t157
        real t1570
        real t1576
        real t1577
        real t1587
        real t1591
        real t1595
        real t1596
        real t1598
        real t16
        real t1600
        real t1602
        real t1603
        real t1604
        real t1605
        real t1609
        real t161
        real t1611
        real t1616
        real t1617
        real t1618
        real t162
        real t1628
        real t164
        real t1645
        real t165
        real t1662
        real t1663
        real t1664
        real t1667
        real t1668
        real t1669
        real t167
        real t1671
        real t1676
        real t1677
        real t1678
        real t1687
        real t169
        real t1695
        real t1701
        real t1703
        real t1705
        real t171
        real t1710
        real t1714
        real t1716
        real t1718
        real t1720
        real t1724
        real t1725
        real t1728
        real t1731
        real t1735
        real t1737
        real t1738
        real t174
        real t1746
        real t1751
        real t1764
        real t1772
        real t1773
        real t1774
        real t1777
        real t1778
        real t1779
        real t1781
        real t1786
        real t1787
        real t1788
        integer t179
        integer t1797
        real t1798
        real t180
        real t1805
        real t1807
        real t1809
        real t1811
        real t1815
        real t182
        real t1836
        integer t184
        real t1849
        real t185
        real t1850
        real t1852
        real t1854
        real t1856
        real t1858
        real t1859
        real t1863
        real t1865
        real t1871
        real t1872
        real t1888
        real t1889
        real t1890
        real t1891
        real t1903
        real t1906
        real t1916
        real t1917
        real t1919
        real t1921
        real t1923
        real t1925
        real t1926
        real t1930
        real t1932
        real t1938
        real t1939
        real t1948
        real t1949
        real t195
        real t1953
        real t1957
        real t1958
        real t196
        real t1960
        real t1962
        real t1963
        real t1964
        real t1966
        real t1967
        real t1971
        real t1973
        real t1979
        real t198
        real t1980
        real t199
        real t1990
        real t2
        real t20
        real t2007
        real t201
        real t2024
        real t2025
        real t2026
        real t2029
        real t203
        real t2030
        real t2031
        real t2033
        real t2038
        real t2039
        real t2040
        real t2049
        real t205
        real t2057
        real t2067
        real t2072
        real t2076
        real t2079
        real t2085
        real t2087
        real t2096
        real t2109
        real t2114
        real t212
        real t2127
        real t213
        real t2134
        real t2136
        real t2139
        real t2140
        real t2141
        real t2144
        real t2145
        real t2146
        real t2148
        real t215
        real t2153
        real t2154
        real t2155
        real t216
        real t2175
        real t218
        integer t2188
        real t2189
        real t2196
        real t2198
        real t22
        real t220
        real t2200
        real t2202
        real t2206
        real t222
        real t2223
        real t2236
        real t2237
        real t2238
        real t2241
        real t2242
        real t2243
        real t2245
        real t225
        real t2250
        real t2251
        real t2252
        real t226
        real t2261
        real t2269
        real t227
        real t2273
        real t228
        real t2283
        real t2284
        real t2286
        real t2288
        real t229
        real t2290
        real t2292
        real t2293
        real t2297
        real t2299
        real t2305
        real t2306
        real t231
        real t233
        real t2337
        real t2338
        real t2339
        real t2340
        real t2348
        real t235
        real t2356
        real t2357
        real t236
        real t2362
        real t2364
        real t2366
        real t2371
        real t2375
        real t2377
        real t2379
        real t2381
        real t2385
        real t2386
        real t2391
        real t240
        real t2404
        real t2409
        real t242
        real t2422
        real t2430
        real t2431
        real t2432
        real t2435
        real t2436
        real t2437
        real t2439
        real t2444
        real t2445
        real t2446
        real t2466
        integer t2479
        real t248
        real t2480
        real t2487
        real t2489
        real t249
        real t2491
        real t2493
        real t2497
        real t2514
        real t2527
        real t2528
        real t2529
        real t2532
        real t2533
        real t2534
        real t2536
        real t2541
        real t2542
        real t2543
        real t2552
        real t2560
        real t2564
        real t257
        real t2574
        real t2575
        real t2577
        real t2579
        real t2581
        real t2583
        real t2584
        real t2588
        real t2590
        real t2596
        real t2597
        real t2628
        real t2629
        real t263
        real t2630
        real t2631
        real t2639
        real t2647
        real t265
        real t2657
        real t2664
        real t269
        real t27
        real t270
        real t272
        real t274
        real t276
        real t278
        real t279
        real t28
        real t283
        real t285
        real t29
        real t291
        real t292
        real t30
        real t300
        real t308
        real t309
        real t31
        real t310
        real t313
        real t314
        real t315
        real t317
        real t32
        real t322
        real t323
        real t324
        real t33
        real t337
        real t34
        real t340
        real t344
        real t35
        real t352
        real t357
        real t360
        real t363
        real t367
        real t369
        real t37
        real t373
        real t374
        real t376
        real t378
        real t380
        real t382
        real t383
        real t387
        real t389
        real t39
        real t395
        real t396
        real t4
        real t404
        real t41
        real t410
        real t414
        real t415
        real t417
        real t419
        real t421
        real t423
        real t424
        real t428
        real t43
        real t430
        real t433
        real t436
        real t437
        real t44
        real t445
        real t446
        real t462
        real t466
        real t479
        real t48
        real t485
        real t486
        real t487
        real t490
        real t491
        real t492
        real t494
        real t499
        integer t5
        real t50
        real t500
        real t501
        real t510
        real t518
        real t524
        real t525
        real t527
        real t528
        real t529
        real t534
        integer t536
        real t537
        real t538
        real t540
        real t542
        real t544
        real t545
        real t546
        real t547
        real t55
        real t551
        real t553
        real t558
        real t559
        real t56
        real t560
        real t561
        real t562
        real t563
        real t564
        real t567
        real t568
        real t57
        real t570
        real t571
        real t574
        real t578
        real t579
        real t58
        real t581
        real t582
        real t584
        real t586
        real t587
        real t588
        real t59
        real t591
        real t595
        real t596
        real t598
        real t599
        real t6
        real t60
        real t601
        real t603
        real t605
        real t608
        real t609
        real t61
        real t610
        real t612
        real t614
        real t616
        real t618
        real t619
        real t623
        real t625
        real t627
        real t630
        real t631
        real t632
        real t636
        real t638
        real t64
        real t640
        real t642
        real t644
        real t646
        real t649
        real t65
        real t650
        real t651
        real t652
        real t653
        real t655
        real t657
        real t659
        real t660
        real t661
        real t664
        real t666
        real t67
        real t671
        real t672
        real t673
        real t675
        real t677
        real t679
        real t68
        real t681
        real t683
        real t686
        real t687
        real t688
        real t689
        real t69
        real t690
        real t691
        real t692
        real t693
        real t694
        real t695
        real t696
        real t699
        real t7
        real t70
        real t700
        real t701
        real t702
        real t703
        real t704
        real t705
        real t708
        real t709
        real t715
        real t716
        real t718
        real t719
        real t721
        real t723
        real t725
        real t726
        real t729
        real t731
        real t734
        real t738
        real t739
        real t741
        real t742
        real t744
        real t746
        real t748
        real t751
        real t752
        real t753
        real t755
        real t757
        real t759
        real t761
        real t762
        real t764
        real t766
        real t768
        real t77
        real t773
        real t774
        real t775
        real t778
        real t779
        real t781
        real t783
        real t785
        real t787
        real t79
        real t790
        real t791
        real t792
        real t794
        real t796
        real t797
        real t798
        real t800
        real t801
        real t805
        real t807
        real t81
        real t812
        real t813
        real t814
        real t818
        real t820
        real t822
        real t823
        real t824
        real t827
        integer t83
        real t831
        real t833
        real t835
        real t837
        real t839
        real t84
        real t840
        real t841
        real t844
        real t848
        real t85
        real t850
        real t852
        real t854
        real t856
        real t859
        real t860
        real t861
        real t862
        real t863
        real t864
        real t865
        real t866
        real t867
        real t868
        real t869
        real t87
        real t872
        real t873
        real t874
        real t875
        real t876
        real t877
        real t878
        real t881
        real t882
        real t885
        real t888
        real t89
        real t891
        real t893
        real t894
        real t896
        real t898
        real t9
        real t902
        real t903
        real t904
        real t907
        real t91
        real t910
        real t914
        real t917
        real t920
        real t922
        real t923
        real t925
        real t93
        real t931
        real t934
        real t937
        real t94
        real t941
        real t944
        real t947
        real t949
        real t950
        real t952
        real t958
        integer t967
        real t968
        real t969
        real t971
        real t973
        real t975
        real t977
        real t978
        real t98
        real t982
        real t984
        real t990
        real t991
        real t992
        real t993
        real t994
        real t999
        t1 = u(i,j,k,n)
        t2 = ut(i,j,k,n)
        t4 = cc ** 2
        t5 = i + 1
        t6 = rx(t5,j,k,0,0)
        t7 = rx(t5,j,k,1,1)
        t9 = rx(t5,j,k,2,2)
        t11 = rx(t5,j,k,1,2)
        t13 = rx(t5,j,k,2,1)
        t15 = rx(t5,j,k,0,1)
        t16 = rx(t5,j,k,1,0)
        t20 = rx(t5,j,k,2,0)
        t22 = rx(t5,j,k,0,2)
        t27 = -t11 * t13 * t6 + t11 * t15 * t20 + t13 * t16 * t22 - t15 
     #* t16 * t9 - t20 * t22 * t7 + t6 * t7 * t9
        t28 = 0.1E1 / t27
        t29 = t6 ** 2
        t30 = t15 ** 2
        t31 = t22 ** 2
        t32 = t29 + t30 + t31
        t33 = t28 * t32
        t34 = rx(i,j,k,0,0)
        t35 = rx(i,j,k,1,1)
        t37 = rx(i,j,k,2,2)
        t39 = rx(i,j,k,1,2)
        t41 = rx(i,j,k,2,1)
        t43 = rx(i,j,k,0,1)
        t44 = rx(i,j,k,1,0)
        t48 = rx(i,j,k,2,0)
        t50 = rx(i,j,k,0,2)
        t55 = t34 * t35 * t37 - t34 * t39 * t41 - t35 * t48 * t50 - t37 
     #* t43 * t44 + t39 * t43 * t48 + t41 * t44 * t50
        t56 = 0.1E1 / t55
        t57 = t34 ** 2
        t58 = t43 ** 2
        t59 = t50 ** 2
        t60 = t57 + t58 + t59
        t61 = t56 * t60
        t64 = t4 * (t33 / 0.2E1 + t61 / 0.2E1)
        t65 = u(t5,j,k,n)
        t67 = 0.1E1 / dx
        t68 = (t65 - t1) * t67
        t69 = t64 * t68
        t70 = ut(t5,j,k,n)
        t77 = sqrt(t32)
        t79 = cc * t28 * t77 * t70
        t81 = dt * cc
        t83 = i + 2
        t84 = rx(t83,j,k,0,0)
        t85 = rx(t83,j,k,1,1)
        t87 = rx(t83,j,k,2,2)
        t89 = rx(t83,j,k,1,2)
        t91 = rx(t83,j,k,2,1)
        t93 = rx(t83,j,k,0,1)
        t94 = rx(t83,j,k,1,0)
        t98 = rx(t83,j,k,2,0)
        t100 = rx(t83,j,k,0,2)
        t106 = 0.1E1 / (-t100 * t85 * t98 + t100 * t91 * t94 + t84 * t85
     # * t87 - t84 * t89 * t91 - t87 * t93 * t94 + t89 * t93 * t98)
        t107 = t84 ** 2
        t108 = t93 ** 2
        t109 = t100 ** 2
        t110 = t107 + t108 + t109
        t115 = u(t83,j,k,n)
        t117 = (t115 - t65) * t67
        t121 = t4 * t106
        t126 = j + 1
        t127 = u(t83,t126,k,n)
        t129 = 0.1E1 / dy
        t131 = j - 1
        t132 = u(t83,t131,k,n)
        t139 = t4 * t28
        t143 = t11 * t22 + t15 * t7 + t16 * t6
        t144 = u(t5,t126,k,n)
        t146 = (t144 - t65) * t129
        t147 = u(t5,t131,k,n)
        t149 = (t65 - t147) * t129
        t151 = t146 / 0.2E1 + t149 / 0.2E1
        t111 = t139 * t143
        t153 = t111 * t151
        t157 = t4 * t56
        t161 = t34 * t44 + t35 * t43 + t39 * t50
        t162 = u(i,t126,k,n)
        t164 = (t162 - t1) * t129
        t165 = u(i,t131,k,n)
        t167 = (t1 - t165) * t129
        t169 = t164 / 0.2E1 + t167 / 0.2E1
        t120 = t157 * t161
        t171 = t120 * t169
        t174 = (t153 - t171) * t67 / 0.2E1
        t179 = k + 1
        t180 = u(t83,j,t179,n)
        t182 = 0.1E1 / dz
        t184 = k - 1
        t185 = u(t83,j,t184,n)
        t195 = t13 * t15 + t20 * t6 + t22 * t9
        t196 = u(t5,j,t179,n)
        t198 = (t196 - t65) * t182
        t199 = u(t5,j,t184,n)
        t201 = (t65 - t199) * t182
        t203 = t198 / 0.2E1 + t201 / 0.2E1
        t135 = t139 * t195
        t205 = t135 * t203
        t212 = t34 * t48 + t37 * t50 + t41 * t43
        t213 = u(i,j,t179,n)
        t215 = (t213 - t1) * t182
        t216 = u(i,j,t184,n)
        t218 = (t1 - t216) * t182
        t220 = t215 / 0.2E1 + t218 / 0.2E1
        t145 = t157 * t212
        t222 = t145 * t220
        t225 = (t205 - t222) * t67 / 0.2E1
        t226 = rx(t5,t126,k,0,0)
        t227 = rx(t5,t126,k,1,1)
        t229 = rx(t5,t126,k,2,2)
        t231 = rx(t5,t126,k,1,2)
        t233 = rx(t5,t126,k,2,1)
        t235 = rx(t5,t126,k,0,1)
        t236 = rx(t5,t126,k,1,0)
        t240 = rx(t5,t126,k,2,0)
        t242 = rx(t5,t126,k,0,2)
        t248 = 0.1E1 / (t226 * t227 * t229 - t226 * t231 * t233 - t227 *
     # t240 * t242 - t229 * t235 * t236 + t231 * t235 * t240 + t233 * t2
     #36 * t242)
        t249 = t4 * t248
        t257 = (t144 - t162) * t67
        t263 = t117 / 0.2E1 + t68 / 0.2E1
        t265 = t111 * t263
        t269 = rx(t5,t131,k,0,0)
        t270 = rx(t5,t131,k,1,1)
        t272 = rx(t5,t131,k,2,2)
        t274 = rx(t5,t131,k,1,2)
        t276 = rx(t5,t131,k,2,1)
        t278 = rx(t5,t131,k,0,1)
        t279 = rx(t5,t131,k,1,0)
        t283 = rx(t5,t131,k,2,0)
        t285 = rx(t5,t131,k,0,2)
        t291 = 0.1E1 / (t269 * t270 * t272 - t269 * t274 * t276 - t270 *
     # t283 * t285 - t272 * t278 * t279 + t274 * t278 * t283 + t276 * t2
     #79 * t285)
        t292 = t4 * t291
        t300 = (t147 - t165) * t67
        t308 = t236 ** 2
        t309 = t227 ** 2
        t310 = t231 ** 2
        t313 = t16 ** 2
        t314 = t7 ** 2
        t315 = t11 ** 2
        t317 = t28 * (t313 + t314 + t315)
        t322 = t279 ** 2
        t323 = t270 ** 2
        t324 = t274 ** 2
        t337 = u(t5,t126,t179,n)
        t340 = u(t5,t126,t184,n)
        t344 = (t337 - t144) * t182 / 0.2E1 + (t144 - t340) * t182 / 0.2
     #E1
        t228 = t139 * (t11 * t9 + t13 * t7 + t16 * t20)
        t352 = t228 * t203
        t360 = u(t5,t131,t179,n)
        t363 = u(t5,t131,t184,n)
        t367 = (t360 - t147) * t182 / 0.2E1 + (t147 - t363) * t182 / 0.2
     #E1
        t373 = rx(t5,j,t179,0,0)
        t374 = rx(t5,j,t179,1,1)
        t376 = rx(t5,j,t179,2,2)
        t378 = rx(t5,j,t179,1,2)
        t380 = rx(t5,j,t179,2,1)
        t382 = rx(t5,j,t179,0,1)
        t383 = rx(t5,j,t179,1,0)
        t387 = rx(t5,j,t179,2,0)
        t389 = rx(t5,j,t179,0,2)
        t395 = 0.1E1 / (t373 * t374 * t376 - t373 * t378 * t380 - t374 *
     # t387 * t389 - t376 * t382 * t383 + t378 * t382 * t387 + t380 * t3
     #83 * t389)
        t396 = t4 * t395
        t404 = (t196 - t213) * t67
        t410 = t135 * t263
        t414 = rx(t5,j,t184,0,0)
        t415 = rx(t5,j,t184,1,1)
        t417 = rx(t5,j,t184,2,2)
        t419 = rx(t5,j,t184,1,2)
        t421 = rx(t5,j,t184,2,1)
        t423 = rx(t5,j,t184,0,1)
        t424 = rx(t5,j,t184,1,0)
        t428 = rx(t5,j,t184,2,0)
        t430 = rx(t5,j,t184,0,2)
        t436 = 0.1E1 / (t414 * t415 * t417 - t414 * t419 * t421 - t415 *
     # t428 * t430 - t417 * t423 * t424 + t419 * t423 * t428 + t421 * t4
     #24 * t430)
        t437 = t4 * t436
        t445 = (t199 - t216) * t67
        t462 = (t337 - t196) * t129 / 0.2E1 + (t196 - t360) * t129 / 0.2
     #E1
        t466 = t228 * t151
        t479 = (t340 - t199) * t129 / 0.2E1 + (t199 - t363) * t129 / 0.2
     #E1
        t485 = t387 ** 2
        t486 = t380 ** 2
        t487 = t376 ** 2
        t490 = t20 ** 2
        t491 = t13 ** 2
        t492 = t9 ** 2
        t494 = t28 * (t490 + t491 + t492)
        t499 = t428 ** 2
        t500 = t421 ** 2
        t501 = t417 ** 2
        t357 = t249 * (t226 * t236 + t227 * t235 + t231 * t242)
        t369 = t292 * (t269 * t279 + t270 * t278 + t274 * t285)
        t433 = t396 * (t373 * t387 + t376 * t389 + t380 * t382)
        t446 = t437 * (t414 * t428 + t417 * t430 + t421 * t423)
        t510 = (t4 * (t106 * t110 / 0.2E1 + t33 / 0.2E1) * t117 - t69) *
     # t67 + (t121 * (t100 * t89 + t84 * t94 + t85 * t93) * ((t127 - t11
     #5) * t129 / 0.2E1 + (t115 - t132) * t129 / 0.2E1) - t153) * t67 / 
     #0.2E1 + t174 + (t121 * (t100 * t87 + t84 * t98 + t91 * t93) * ((t1
     #80 - t115) * t182 / 0.2E1 + (t115 - t185) * t182 / 0.2E1) - t205) 
     #* t67 / 0.2E1 + t225 + (t357 * ((t127 - t144) * t67 / 0.2E1 + t257
     # / 0.2E1) - t265) * t129 / 0.2E1 + (t265 - t369 * ((t132 - t147) *
     # t67 / 0.2E1 + t300 / 0.2E1)) * t129 / 0.2E1 + (t4 * (t248 * (t308
     # + t309 + t310) / 0.2E1 + t317 / 0.2E1) * t146 - t4 * (t317 / 0.2E
     #1 + t291 * (t322 + t323 + t324) / 0.2E1) * t149) * t129 + (t249 * 
     #(t227 * t233 + t229 * t231 + t236 * t240) * t344 - t352) * t129 / 
     #0.2E1 + (t352 - t292 * (t270 * t276 + t272 * t274 + t279 * t283) *
     # t367) * t129 / 0.2E1 + (t433 * ((t180 - t196) * t67 / 0.2E1 + t40
     #4 / 0.2E1) - t410) * t182 / 0.2E1 + (t410 - t446 * ((t185 - t199) 
     #* t67 / 0.2E1 + t445 / 0.2E1)) * t182 / 0.2E1 + (t396 * (t374 * t3
     #80 + t376 * t378 + t383 * t387) * t462 - t466) * t182 / 0.2E1 + (t
     #466 - t437 * (t415 * t421 + t417 * t419 + t424 * t428) * t479) * t
     #182 / 0.2E1 + (t4 * (t395 * (t485 + t486 + t487) / 0.2E1 + t494 / 
     #0.2E1) * t198 - t4 * (t494 / 0.2E1 + t436 * (t499 + t500 + t501) /
     # 0.2E1) * t201) * t182
        t518 = sqrt(t110)
        t524 = cc * t56
        t525 = sqrt(t60)
        t527 = t524 * t525 * t2
        t529 = (-t527 + t79) * t67
        t534 = t527 / 0.2E1
        t536 = i - 1
        t537 = rx(t536,j,k,0,0)
        t538 = rx(t536,j,k,1,1)
        t540 = rx(t536,j,k,2,2)
        t542 = rx(t536,j,k,1,2)
        t544 = rx(t536,j,k,2,1)
        t546 = rx(t536,j,k,0,1)
        t547 = rx(t536,j,k,1,0)
        t551 = rx(t536,j,k,2,0)
        t553 = rx(t536,j,k,0,2)
        t558 = t537 * t538 * t540 - t537 * t542 * t544 - t538 * t551 * t
     #553 - t540 * t546 * t547 + t542 * t546 * t551 + t544 * t547 * t553
        t559 = 0.1E1 / t558
        t560 = t537 ** 2
        t561 = t546 ** 2
        t562 = t553 ** 2
        t563 = t560 + t561 + t562
        t564 = t559 * t563
        t567 = t4 * (t61 / 0.2E1 + t564 / 0.2E1)
        t568 = u(t536,j,k,n)
        t570 = (t1 - t568) * t67
        t571 = t567 * t570
        t574 = t4 * t559
        t578 = t537 * t547 + t538 * t546 + t542 * t553
        t579 = u(t536,t126,k,n)
        t581 = (t579 - t568) * t129
        t582 = u(t536,t131,k,n)
        t584 = (t568 - t582) * t129
        t586 = t581 / 0.2E1 + t584 / 0.2E1
        t528 = t574 * t578
        t588 = t528 * t586
        t591 = (t171 - t588) * t67 / 0.2E1
        t595 = t537 * t551 + t540 * t553 + t544 * t546
        t596 = u(t536,j,t179,n)
        t598 = (t596 - t568) * t182
        t599 = u(t536,j,t184,n)
        t601 = (t568 - t599) * t182
        t603 = t598 / 0.2E1 + t601 / 0.2E1
        t545 = t574 * t595
        t605 = t545 * t603
        t608 = (t222 - t605) * t67 / 0.2E1
        t609 = rx(i,t126,k,0,0)
        t610 = rx(i,t126,k,1,1)
        t612 = rx(i,t126,k,2,2)
        t614 = rx(i,t126,k,1,2)
        t616 = rx(i,t126,k,2,1)
        t618 = rx(i,t126,k,0,1)
        t619 = rx(i,t126,k,1,0)
        t623 = rx(i,t126,k,2,0)
        t625 = rx(i,t126,k,0,2)
        t630 = t609 * t610 * t612 - t609 * t614 * t616 - t610 * t623 * t
     #625 - t612 * t618 * t619 + t614 * t618 * t623 + t616 * t619 * t625
        t631 = 0.1E1 / t630
        t632 = t4 * t631
        t636 = t609 * t619 + t610 * t618 + t614 * t625
        t638 = (t162 - t579) * t67
        t640 = t257 / 0.2E1 + t638 / 0.2E1
        t587 = t632 * t636
        t642 = t587 * t640
        t644 = t68 / 0.2E1 + t570 / 0.2E1
        t646 = t120 * t644
        t649 = (t642 - t646) * t129 / 0.2E1
        t650 = rx(i,t131,k,0,0)
        t651 = rx(i,t131,k,1,1)
        t653 = rx(i,t131,k,2,2)
        t655 = rx(i,t131,k,1,2)
        t657 = rx(i,t131,k,2,1)
        t659 = rx(i,t131,k,0,1)
        t660 = rx(i,t131,k,1,0)
        t664 = rx(i,t131,k,2,0)
        t666 = rx(i,t131,k,0,2)
        t671 = t650 * t651 * t653 - t650 * t655 * t657 - t651 * t664 * t
     #666 - t653 * t659 * t660 + t655 * t659 * t664 + t657 * t660 * t666
        t672 = 0.1E1 / t671
        t673 = t4 * t672
        t677 = t650 * t660 + t651 * t659 + t655 * t666
        t679 = (t165 - t582) * t67
        t681 = t300 / 0.2E1 + t679 / 0.2E1
        t627 = t673 * t677
        t683 = t627 * t681
        t686 = (t646 - t683) * t129 / 0.2E1
        t687 = t619 ** 2
        t688 = t610 ** 2
        t689 = t614 ** 2
        t690 = t687 + t688 + t689
        t691 = t631 * t690
        t692 = t44 ** 2
        t693 = t35 ** 2
        t694 = t39 ** 2
        t695 = t692 + t693 + t694
        t696 = t56 * t695
        t699 = t4 * (t691 / 0.2E1 + t696 / 0.2E1)
        t700 = t699 * t164
        t701 = t660 ** 2
        t702 = t651 ** 2
        t703 = t655 ** 2
        t704 = t701 + t702 + t703
        t705 = t672 * t704
        t708 = t4 * (t696 / 0.2E1 + t705 / 0.2E1)
        t709 = t708 * t167
        t715 = t610 * t616 + t612 * t614 + t619 * t623
        t716 = u(i,t126,t179,n)
        t718 = (t716 - t162) * t182
        t719 = u(i,t126,t184,n)
        t721 = (t162 - t719) * t182
        t723 = t718 / 0.2E1 + t721 / 0.2E1
        t652 = t632 * t715
        t725 = t652 * t723
        t729 = t35 * t41 + t37 * t39 + t44 * t48
        t661 = t157 * t729
        t731 = t661 * t220
        t734 = (t725 - t731) * t129 / 0.2E1
        t738 = t651 * t657 + t653 * t655 + t660 * t664
        t739 = u(i,t131,t179,n)
        t741 = (t739 - t165) * t182
        t742 = u(i,t131,t184,n)
        t744 = (t165 - t742) * t182
        t746 = t741 / 0.2E1 + t744 / 0.2E1
        t675 = t673 * t738
        t748 = t675 * t746
        t751 = (t731 - t748) * t129 / 0.2E1
        t752 = rx(i,j,t179,0,0)
        t753 = rx(i,j,t179,1,1)
        t755 = rx(i,j,t179,2,2)
        t757 = rx(i,j,t179,1,2)
        t759 = rx(i,j,t179,2,1)
        t761 = rx(i,j,t179,0,1)
        t762 = rx(i,j,t179,1,0)
        t766 = rx(i,j,t179,2,0)
        t768 = rx(i,j,t179,0,2)
        t773 = t752 * t753 * t755 - t752 * t757 * t759 - t753 * t766 * t
     #768 - t755 * t761 * t762 + t757 * t761 * t766 + t759 * t762 * t768
        t774 = 0.1E1 / t773
        t775 = t4 * t774
        t779 = t752 * t766 + t755 * t768 + t759 * t761
        t781 = (t213 - t596) * t67
        t783 = t404 / 0.2E1 + t781 / 0.2E1
        t726 = t775 * t779
        t785 = t726 * t783
        t787 = t145 * t644
        t790 = (t785 - t787) * t182 / 0.2E1
        t791 = rx(i,j,t184,0,0)
        t792 = rx(i,j,t184,1,1)
        t794 = rx(i,j,t184,2,2)
        t796 = rx(i,j,t184,1,2)
        t798 = rx(i,j,t184,2,1)
        t800 = rx(i,j,t184,0,1)
        t801 = rx(i,j,t184,1,0)
        t805 = rx(i,j,t184,2,0)
        t807 = rx(i,j,t184,0,2)
        t812 = t791 * t792 * t794 - t791 * t796 * t798 - t792 * t805 * t
     #807 - t794 * t800 * t801 + t796 * t800 * t805 + t798 * t801 * t807
        t813 = 0.1E1 / t812
        t814 = t4 * t813
        t818 = t791 * t805 + t794 * t807 + t798 * t800
        t820 = (t216 - t599) * t67
        t822 = t445 / 0.2E1 + t820 / 0.2E1
        t764 = t814 * t818
        t824 = t764 * t822
        t827 = (t787 - t824) * t182 / 0.2E1
        t831 = t753 * t759 + t755 * t757 + t762 * t766
        t833 = (t716 - t213) * t129
        t835 = (t213 - t739) * t129
        t837 = t833 / 0.2E1 + t835 / 0.2E1
        t778 = t775 * t831
        t839 = t778 * t837
        t841 = t661 * t169
        t844 = (t839 - t841) * t182 / 0.2E1
        t848 = t792 * t798 + t794 * t796 + t801 * t805
        t850 = (t719 - t216) * t129
        t852 = (t216 - t742) * t129
        t854 = t850 / 0.2E1 + t852 / 0.2E1
        t797 = t814 * t848
        t856 = t797 * t854
        t859 = (t841 - t856) * t182 / 0.2E1
        t860 = t766 ** 2
        t861 = t759 ** 2
        t862 = t755 ** 2
        t863 = t860 + t861 + t862
        t864 = t774 * t863
        t865 = t48 ** 2
        t866 = t41 ** 2
        t867 = t37 ** 2
        t868 = t865 + t866 + t867
        t869 = t56 * t868
        t872 = t4 * (t864 / 0.2E1 + t869 / 0.2E1)
        t873 = t872 * t215
        t874 = t805 ** 2
        t875 = t798 ** 2
        t876 = t794 ** 2
        t877 = t874 + t875 + t876
        t878 = t813 * t877
        t881 = t4 * (t869 / 0.2E1 + t878 / 0.2E1)
        t882 = t881 * t218
        t885 = (t69 - t571) * t67 + t174 + t591 + t225 + t608 + t649 + t
     #686 + (t700 - t709) * t129 + t734 + t751 + t790 + t827 + t844 + t8
     #59 + (t873 - t882) * t182
        t888 = t55 * t885 + src(i,j,k,nComp,n)
        t823 = t81 * t56
        t891 = t823 * t525 * t888 / 0.4E1
        t893 = sqrt(t563)
        t894 = ut(t536,j,k,n)
        t896 = cc * t559 * t893 * t894
        t898 = (t527 - t896) * t67
        t902 = dx * (t529 / 0.2E1 + t898 / 0.2E1) / 0.4E1
        t840 = (t70 - t2) * t67
        t903 = t69 + t64 * dt * t840 / 0.2E1 + t79 / 0.2E1 + t81 * t28 *
     # t77 * (t27 * t510 + src(t5,j,k,nComp,n)) / 0.4E1 - dx * ((cc * t1
     #06 * t518 * ut(t83,j,k,n) - t79) * t67 / 0.2E1 + t529 / 0.2E1) / 0
     #.4E1 - t534 - t891 - t902
        t904 = dt ** 2
        t907 = t56 * t161
        t910 = t4 * (t28 * t143 / 0.2E1 + t907 / 0.2E1)
        t914 = ut(t5,t126,k,n)
        t917 = ut(t5,t131,k,n)
        t920 = ut(i,t126,k,n)
        t922 = (t920 - t2) * t129
        t923 = ut(i,t131,k,n)
        t925 = (t2 - t923) * t129
        t931 = t910 * (t146 / 0.4E1 + t149 / 0.4E1 + t164 / 0.4E1 + t167
     # / 0.4E1) + t910 * dt * ((t914 - t70) * t129 / 0.4E1 + (t70 - t917
     #) * t129 / 0.4E1 + t922 / 0.4E1 + t925 / 0.4E1) / 0.2E1
        t934 = t56 * t212
        t937 = t4 * (t28 * t195 / 0.2E1 + t934 / 0.2E1)
        t941 = ut(t5,j,t179,n)
        t944 = ut(t5,j,t184,n)
        t947 = ut(i,j,t179,n)
        t949 = (t947 - t2) * t182
        t950 = ut(i,j,t184,n)
        t952 = (t2 - t950) * t182
        t958 = t937 * (t198 / 0.4E1 + t201 / 0.4E1 + t215 / 0.4E1 + t218
     # / 0.4E1) + t937 * dt * ((t941 - t70) * t182 / 0.4E1 + (t70 - t944
     #) * t182 / 0.4E1 + t949 / 0.4E1 + t952 / 0.4E1) / 0.2E1
        t967 = i - 2
        t968 = rx(t967,j,k,0,0)
        t969 = rx(t967,j,k,1,1)
        t971 = rx(t967,j,k,2,2)
        t973 = rx(t967,j,k,1,2)
        t975 = rx(t967,j,k,2,1)
        t977 = rx(t967,j,k,0,1)
        t978 = rx(t967,j,k,1,0)
        t982 = rx(t967,j,k,2,0)
        t984 = rx(t967,j,k,0,2)
        t990 = 0.1E1 / (t968 * t969 * t971 - t968 * t973 * t975 - t969 *
     # t982 * t984 - t971 * t977 * t978 + t973 * t977 * t982 + t975 * t9
     #78 * t984)
        t991 = t968 ** 2
        t992 = t977 ** 2
        t993 = t984 ** 2
        t994 = t991 + t992 + t993
        t999 = u(t967,j,k,n)
        t1001 = (t568 - t999) * t67
        t1005 = t4 * t990
        t1010 = u(t967,t126,k,n)
        t1013 = u(t967,t131,k,n)
        t1027 = u(t967,j,t179,n)
        t1030 = u(t967,j,t184,n)
        t1040 = rx(t536,t126,k,0,0)
        t1041 = rx(t536,t126,k,1,1)
        t1043 = rx(t536,t126,k,2,2)
        t1045 = rx(t536,t126,k,1,2)
        t1047 = rx(t536,t126,k,2,1)
        t1049 = rx(t536,t126,k,0,1)
        t1050 = rx(t536,t126,k,1,0)
        t1054 = rx(t536,t126,k,2,0)
        t1056 = rx(t536,t126,k,0,2)
        t1062 = 0.1E1 / (t1040 * t1041 * t1043 - t1040 * t1045 * t1047 -
     # t1041 * t1054 * t1056 - t1043 * t1049 * t1050 + t1045 * t1049 * t
     #1054 + t1047 * t1050 * t1056)
        t1063 = t4 * t1062
        t1075 = t570 / 0.2E1 + t1001 / 0.2E1
        t1077 = t528 * t1075
        t1081 = rx(t536,t131,k,0,0)
        t1082 = rx(t536,t131,k,1,1)
        t1084 = rx(t536,t131,k,2,2)
        t1086 = rx(t536,t131,k,1,2)
        t1088 = rx(t536,t131,k,2,1)
        t1090 = rx(t536,t131,k,0,1)
        t1091 = rx(t536,t131,k,1,0)
        t1095 = rx(t536,t131,k,2,0)
        t1097 = rx(t536,t131,k,0,2)
        t1103 = 0.1E1 / (t1081 * t1082 * t1084 - t1081 * t1086 * t1088 -
     # t1082 * t1095 * t1097 - t1084 * t1090 * t1091 + t1086 * t1090 * t
     #1095 + t1088 * t1091 * t1097)
        t1104 = t4 * t1103
        t1118 = t1050 ** 2
        t1119 = t1041 ** 2
        t1120 = t1045 ** 2
        t1123 = t547 ** 2
        t1124 = t538 ** 2
        t1125 = t542 ** 2
        t1127 = t559 * (t1123 + t1124 + t1125)
        t1132 = t1091 ** 2
        t1133 = t1082 ** 2
        t1134 = t1086 ** 2
        t1147 = u(t536,t126,t179,n)
        t1150 = u(t536,t126,t184,n)
        t1154 = (t1147 - t579) * t182 / 0.2E1 + (t579 - t1150) * t182 / 
     #0.2E1
        t1028 = t574 * (t538 * t544 + t540 * t542 + t547 * t551)
        t1162 = t1028 * t603
        t1170 = u(t536,t131,t179,n)
        t1173 = u(t536,t131,t184,n)
        t1177 = (t1170 - t582) * t182 / 0.2E1 + (t582 - t1173) * t182 / 
     #0.2E1
        t1183 = rx(t536,j,t179,0,0)
        t1184 = rx(t536,j,t179,1,1)
        t1186 = rx(t536,j,t179,2,2)
        t1188 = rx(t536,j,t179,1,2)
        t1190 = rx(t536,j,t179,2,1)
        t1192 = rx(t536,j,t179,0,1)
        t1193 = rx(t536,j,t179,1,0)
        t1197 = rx(t536,j,t179,2,0)
        t1199 = rx(t536,j,t179,0,2)
        t1205 = 0.1E1 / (t1183 * t1184 * t1186 - t1183 * t1188 * t1190 -
     # t1184 * t1197 * t1199 - t1186 * t1192 * t1193 + t1188 * t1192 * t
     #1197 + t1190 * t1193 * t1199)
        t1206 = t4 * t1205
        t1218 = t545 * t1075
        t1222 = rx(t536,j,t184,0,0)
        t1223 = rx(t536,j,t184,1,1)
        t1225 = rx(t536,j,t184,2,2)
        t1227 = rx(t536,j,t184,1,2)
        t1229 = rx(t536,j,t184,2,1)
        t1231 = rx(t536,j,t184,0,1)
        t1232 = rx(t536,j,t184,1,0)
        t1236 = rx(t536,j,t184,2,0)
        t1238 = rx(t536,j,t184,0,2)
        t1244 = 0.1E1 / (t1222 * t1223 * t1225 - t1222 * t1227 * t1229 -
     # t1223 * t1236 * t1238 - t1225 * t1231 * t1232 + t1227 * t1231 * t
     #1236 + t1229 * t1232 * t1238)
        t1245 = t4 * t1244
        t1268 = (t1147 - t596) * t129 / 0.2E1 + (t596 - t1170) * t129 / 
     #0.2E1
        t1272 = t1028 * t586
        t1285 = (t1150 - t599) * t129 / 0.2E1 + (t599 - t1173) * t129 / 
     #0.2E1
        t1291 = t1197 ** 2
        t1292 = t1190 ** 2
        t1293 = t1186 ** 2
        t1296 = t551 ** 2
        t1297 = t544 ** 2
        t1298 = t540 ** 2
        t1300 = t559 * (t1296 + t1297 + t1298)
        t1305 = t1236 ** 2
        t1306 = t1229 ** 2
        t1307 = t1225 ** 2
        t1153 = t1063 * (t1040 * t1050 + t1041 * t1049 + t1045 * t1056)
        t1165 = t1104 * (t1081 * t1091 + t1082 * t1090 + t1086 * t1097)
        t1224 = t1206 * (t1183 * t1197 + t1186 * t1199 + t1190 * t1192)
        t1240 = t1245 * (t1222 * t1236 + t1225 * t1238 + t1229 * t1231)
        t1316 = (t571 - t4 * (t990 * t994 / 0.2E1 + t564 / 0.2E1) * t100
     #1) * t67 + t591 + (t588 - t1005 * (t968 * t978 + t969 * t977 + t97
     #3 * t984) * ((t1010 - t999) * t129 / 0.2E1 + (t999 - t1013) * t129
     # / 0.2E1)) * t67 / 0.2E1 + t608 + (t605 - t1005 * (t968 * t982 + t
     #971 * t984 + t975 * t977) * ((t1027 - t999) * t182 / 0.2E1 + (t999
     # - t1030) * t182 / 0.2E1)) * t67 / 0.2E1 + (t1153 * (t638 / 0.2E1 
     #+ (t579 - t1010) * t67 / 0.2E1) - t1077) * t129 / 0.2E1 + (t1077 -
     # t1165 * (t679 / 0.2E1 + (t582 - t1013) * t67 / 0.2E1)) * t129 / 0
     #.2E1 + (t4 * (t1062 * (t1118 + t1119 + t1120) / 0.2E1 + t1127 / 0.
     #2E1) * t581 - t4 * (t1127 / 0.2E1 + t1103 * (t1132 + t1133 + t1134
     #) / 0.2E1) * t584) * t129 + (t1063 * (t1041 * t1047 + t1043 * t104
     #5 + t1050 * t1054) * t1154 - t1162) * t129 / 0.2E1 + (t1162 - t110
     #4 * (t1082 * t1088 + t1084 * t1086 + t1091 * t1095) * t1177) * t12
     #9 / 0.2E1 + (t1224 * (t781 / 0.2E1 + (t596 - t1027) * t67 / 0.2E1)
     # - t1218) * t182 / 0.2E1 + (t1218 - t1240 * (t820 / 0.2E1 + (t599 
     #- t1030) * t67 / 0.2E1)) * t182 / 0.2E1 + (t1206 * (t1184 * t1190 
     #+ t1186 * t1188 + t1193 * t1197) * t1268 - t1272) * t182 / 0.2E1 +
     # (t1272 - t1245 * (t1223 * t1229 + t1225 * t1227 + t1232 * t1236) 
     #* t1285) * t182 / 0.2E1 + (t4 * (t1205 * (t1291 + t1292 + t1293) /
     # 0.2E1 + t1300 / 0.2E1) * t598 - t4 * (t1300 / 0.2E1 + t1244 * (t1
     #305 + t1306 + t1307) / 0.2E1) * t601) * t182
        t1324 = sqrt(t994)
        t1288 = (t2 - t894) * t67
        t1334 = t571 + t567 * dt * t1288 / 0.2E1 + t534 + t891 - t902 - 
     #t896 / 0.2E1 - t81 * t559 * t893 * (t1316 * t558 + src(t536,j,k,nC
     #omp,n)) / 0.4E1 - dx * (t898 / 0.2E1 + (-cc * t1324 * t990 * ut(t9
     #67,j,k,n) + t896) * t67 / 0.2E1) / 0.4E1
        t1339 = t4 * (t559 * t578 / 0.2E1 + t907 / 0.2E1)
        t1343 = ut(t536,t126,k,n)
        t1346 = ut(t536,t131,k,n)
        t1354 = t1339 * (t164 / 0.4E1 + t167 / 0.4E1 + t581 / 0.4E1 + t5
     #84 / 0.4E1) + t1339 * dt * (t922 / 0.4E1 + t925 / 0.4E1 + (t1343 -
     # t894) * t129 / 0.4E1 + (t894 - t1346) * t129 / 0.4E1) / 0.2E1
        t1359 = t4 * (t559 * t595 / 0.2E1 + t934 / 0.2E1)
        t1363 = ut(t536,j,t179,n)
        t1366 = ut(t536,j,t184,n)
        t1374 = t1359 * (t215 / 0.4E1 + t218 / 0.4E1 + t598 / 0.4E1 + t6
     #01 / 0.4E1) + t1359 * dt * (t949 / 0.4E1 + t952 / 0.4E1 + (t1363 -
     # t894) * t182 / 0.4E1 + (t894 - t1366) * t182 / 0.4E1) / 0.2E1
        t1383 = t4 * (t631 * t636 / 0.2E1 + t907 / 0.2E1)
        t1391 = t840
        t1392 = t1288
        t1398 = t1383 * (t257 / 0.4E1 + t638 / 0.4E1 + t68 / 0.4E1 + t57
     #0 / 0.4E1) + t1383 * dt * ((t914 - t920) * t67 / 0.4E1 + (t920 - t
     #1343) * t67 / 0.4E1 + t1391 / 0.4E1 + t1392 / 0.4E1) / 0.2E1
        t1405 = sqrt(t690)
        t1407 = cc * t631 * t1405 * t920
        t1410 = t226 ** 2
        t1411 = t235 ** 2
        t1412 = t242 ** 2
        t1415 = t609 ** 2
        t1416 = t618 ** 2
        t1417 = t625 ** 2
        t1419 = t631 * (t1415 + t1416 + t1417)
        t1424 = t1040 ** 2
        t1425 = t1049 ** 2
        t1426 = t1056 ** 2
        t1435 = j + 2
        t1436 = u(t5,t1435,k,n)
        t1443 = u(i,t1435,k,n)
        t1445 = (t1443 - t162) * t129
        t1447 = t1445 / 0.2E1 + t164 / 0.2E1
        t1449 = t587 * t1447
        t1453 = u(t536,t1435,k,n)
        t1384 = t632 * (t609 * t623 + t612 * t625 + t616 * t618)
        t1474 = t1384 * t723
        t1487 = rx(i,t1435,k,0,0)
        t1488 = rx(i,t1435,k,1,1)
        t1490 = rx(i,t1435,k,2,2)
        t1492 = rx(i,t1435,k,1,2)
        t1494 = rx(i,t1435,k,2,1)
        t1496 = rx(i,t1435,k,0,1)
        t1497 = rx(i,t1435,k,1,0)
        t1501 = rx(i,t1435,k,2,0)
        t1503 = rx(i,t1435,k,0,2)
        t1509 = 0.1E1 / (t1487 * t1488 * t1490 - t1487 * t1492 * t1494 -
     # t1488 * t1501 * t1503 - t1490 * t1496 * t1497 + t1492 * t1496 * t
     #1501 + t1494 * t1497 * t1503)
        t1510 = t4 * t1509
        t1526 = t1497 ** 2
        t1527 = t1488 ** 2
        t1528 = t1492 ** 2
        t1529 = t1526 + t1527 + t1528
        t1541 = u(i,t1435,t179,n)
        t1544 = u(i,t1435,t184,n)
        t1554 = rx(i,t126,t179,0,0)
        t1555 = rx(i,t126,t179,1,1)
        t1557 = rx(i,t126,t179,2,2)
        t1559 = rx(i,t126,t179,1,2)
        t1561 = rx(i,t126,t179,2,1)
        t1563 = rx(i,t126,t179,0,1)
        t1564 = rx(i,t126,t179,1,0)
        t1568 = rx(i,t126,t179,2,0)
        t1570 = rx(i,t126,t179,0,2)
        t1576 = 0.1E1 / (t1554 * t1555 * t1557 - t1554 * t1559 * t1561 -
     # t1555 * t1568 * t1570 - t1557 * t1563 * t1564 + t1559 * t1563 * t
     #1568 + t1561 * t1564 * t1570)
        t1577 = t4 * t1576
        t1587 = (t337 - t716) * t67 / 0.2E1 + (t716 - t1147) * t67 / 0.2
     #E1
        t1591 = t1384 * t640
        t1595 = rx(i,t126,t184,0,0)
        t1596 = rx(i,t126,t184,1,1)
        t1598 = rx(i,t126,t184,2,2)
        t1600 = rx(i,t126,t184,1,2)
        t1602 = rx(i,t126,t184,2,1)
        t1604 = rx(i,t126,t184,0,1)
        t1605 = rx(i,t126,t184,1,0)
        t1609 = rx(i,t126,t184,2,0)
        t1611 = rx(i,t126,t184,0,2)
        t1617 = 0.1E1 / (t1595 * t1596 * t1598 - t1595 * t1600 * t1602 -
     # t1596 * t1609 * t1611 - t1598 * t1604 * t1605 + t1600 * t1604 * t
     #1609 + t1602 * t1605 * t1611)
        t1618 = t4 * t1617
        t1628 = (t340 - t719) * t67 / 0.2E1 + (t719 - t1150) * t67 / 0.2
     #E1
        t1645 = t652 * t1447
        t1662 = t1568 ** 2
        t1663 = t1561 ** 2
        t1664 = t1557 ** 2
        t1667 = t623 ** 2
        t1668 = t616 ** 2
        t1669 = t612 ** 2
        t1671 = t631 * (t1667 + t1668 + t1669)
        t1676 = t1609 ** 2
        t1677 = t1602 ** 2
        t1678 = t1598 ** 2
        t1603 = t1577 * (t1555 * t1561 + t1557 * t1559 + t1564 * t1568)
        t1616 = t1618 * (t1596 * t1602 + t1598 * t1600 + t1605 * t1609)
        t1687 = (t4 * (t248 * (t1410 + t1411 + t1412) / 0.2E1 + t1419 / 
     #0.2E1) * t257 - t4 * (t1419 / 0.2E1 + t1062 * (t1424 + t1425 + t14
     #26) / 0.2E1) * t638) * t67 + (t357 * ((t1436 - t144) * t129 / 0.2E
     #1 + t146 / 0.2E1) - t1449) * t67 / 0.2E1 + (t1449 - t1153 * ((t145
     #3 - t579) * t129 / 0.2E1 + t581 / 0.2E1)) * t67 / 0.2E1 + (t249 * 
     #(t226 * t240 + t229 * t242 + t233 * t235) * t344 - t1474) * t67 / 
     #0.2E1 + (t1474 - t1063 * (t1040 * t1054 + t1043 * t1056 + t1047 * 
     #t1049) * t1154) * t67 / 0.2E1 + (t1510 * (t1487 * t1497 + t1488 * 
     #t1496 + t1492 * t1503) * ((t1436 - t1443) * t67 / 0.2E1 + (t1443 -
     # t1453) * t67 / 0.2E1) - t642) * t129 / 0.2E1 + t649 + (t4 * (t150
     #9 * t1529 / 0.2E1 + t691 / 0.2E1) * t1445 - t700) * t129 + (t1510 
     #* (t1488 * t1494 + t1490 * t1492 + t1497 * t1501) * ((t1541 - t144
     #3) * t182 / 0.2E1 + (t1443 - t1544) * t182 / 0.2E1) - t725) * t129
     # / 0.2E1 + t734 + (t1577 * (t1554 * t1568 + t1557 * t1570 + t1561 
     #* t1563) * t1587 - t1591) * t182 / 0.2E1 + (t1591 - t1618 * (t1595
     # * t1609 + t1598 * t1611 + t1602 * t1604) * t1628) * t182 / 0.2E1 
     #+ (t1603 * ((t1541 - t716) * t129 / 0.2E1 + t833 / 0.2E1) - t1645)
     # * t182 / 0.2E1 + (t1645 - t1616 * ((t1544 - t719) * t129 / 0.2E1 
     #+ t850 / 0.2E1)) * t182 / 0.2E1 + (t4 * (t1576 * (t1662 + t1663 + 
     #t1664) / 0.2E1 + t1671 / 0.2E1) * t718 - t4 * (t1671 / 0.2E1 + t16
     #17 * (t1676 + t1677 + t1678) / 0.2E1) * t721) * t182
        t1695 = sqrt(t1529)
        t1701 = sqrt(t695)
        t1703 = t524 * t1701 * t2
        t1705 = (-t1703 + t1407) * t129
        t1710 = t1703 / 0.2E1
        t1714 = t823 * t1701 * t888 / 0.4E1
        t1716 = sqrt(t704)
        t1718 = cc * t672 * t1716 * t923
        t1720 = (t1703 - t1718) * t129
        t1724 = dy * (t1705 / 0.2E1 + t1720 / 0.2E1) / 0.4E1
        t1725 = t700 + t699 * dt * t922 / 0.2E1 + t1407 / 0.2E1 + t81 * 
     #t631 * t1405 * (t1687 * t630 + src(i,t126,k,nComp,n)) / 0.4E1 - dy
     # * ((cc * t1509 * t1695 * ut(i,t1435,k,n) - t1407) * t129 / 0.2E1 
     #+ t1705 / 0.2E1) / 0.4E1 - t1710 - t1714 - t1724
        t1728 = t56 * t729
        t1731 = t4 * (t631 * t715 / 0.2E1 + t1728 / 0.2E1)
        t1735 = ut(i,t126,t179,n)
        t1738 = ut(i,t126,t184,n)
        t1746 = t1731 * (t718 / 0.4E1 + t721 / 0.4E1 + t215 / 0.4E1 + t2
     #18 / 0.4E1) + t1731 * dt * ((t1735 - t920) * t182 / 0.4E1 + (t920 
     #- t1738) * t182 / 0.4E1 + t949 / 0.4E1 + t952 / 0.4E1) / 0.2E1
        t1751 = t4 * (t672 * t677 / 0.2E1 + t907 / 0.2E1)
        t1764 = t1751 * (t68 / 0.4E1 + t570 / 0.4E1 + t300 / 0.4E1 + t67
     #9 / 0.4E1) + t1751 * dt * (t1391 / 0.4E1 + t1392 / 0.4E1 + (t917 -
     # t923) * t67 / 0.4E1 + (t923 - t1346) * t67 / 0.4E1) / 0.2E1
        t1772 = t269 ** 2
        t1773 = t278 ** 2
        t1774 = t285 ** 2
        t1777 = t650 ** 2
        t1778 = t659 ** 2
        t1779 = t666 ** 2
        t1781 = t672 * (t1777 + t1778 + t1779)
        t1786 = t1081 ** 2
        t1787 = t1090 ** 2
        t1788 = t1097 ** 2
        t1797 = j - 2
        t1798 = u(t5,t1797,k,n)
        t1805 = u(i,t1797,k,n)
        t1807 = (t165 - t1805) * t129
        t1809 = t167 / 0.2E1 + t1807 / 0.2E1
        t1811 = t627 * t1809
        t1815 = u(t536,t1797,k,n)
        t1737 = t673 * (t650 * t664 + t653 * t666 + t657 * t659)
        t1836 = t1737 * t746
        t1849 = rx(i,t1797,k,0,0)
        t1850 = rx(i,t1797,k,1,1)
        t1852 = rx(i,t1797,k,2,2)
        t1854 = rx(i,t1797,k,1,2)
        t1856 = rx(i,t1797,k,2,1)
        t1858 = rx(i,t1797,k,0,1)
        t1859 = rx(i,t1797,k,1,0)
        t1863 = rx(i,t1797,k,2,0)
        t1865 = rx(i,t1797,k,0,2)
        t1871 = 0.1E1 / (t1849 * t1850 * t1852 - t1849 * t1854 * t1856 -
     # t1850 * t1863 * t1865 - t1852 * t1858 * t1859 + t1854 * t1858 * t
     #1863 + t1856 * t1859 * t1865)
        t1872 = t4 * t1871
        t1888 = t1859 ** 2
        t1889 = t1850 ** 2
        t1890 = t1854 ** 2
        t1891 = t1888 + t1889 + t1890
        t1903 = u(i,t1797,t179,n)
        t1906 = u(i,t1797,t184,n)
        t1916 = rx(i,t131,t179,0,0)
        t1917 = rx(i,t131,t179,1,1)
        t1919 = rx(i,t131,t179,2,2)
        t1921 = rx(i,t131,t179,1,2)
        t1923 = rx(i,t131,t179,2,1)
        t1925 = rx(i,t131,t179,0,1)
        t1926 = rx(i,t131,t179,1,0)
        t1930 = rx(i,t131,t179,2,0)
        t1932 = rx(i,t131,t179,0,2)
        t1938 = 0.1E1 / (t1916 * t1917 * t1919 - t1916 * t1921 * t1923 -
     # t1917 * t1930 * t1932 - t1919 * t1925 * t1926 + t1921 * t1925 * t
     #1930 + t1923 * t1926 * t1932)
        t1939 = t4 * t1938
        t1949 = (t360 - t739) * t67 / 0.2E1 + (t739 - t1170) * t67 / 0.2
     #E1
        t1953 = t1737 * t681
        t1957 = rx(i,t131,t184,0,0)
        t1958 = rx(i,t131,t184,1,1)
        t1960 = rx(i,t131,t184,2,2)
        t1962 = rx(i,t131,t184,1,2)
        t1964 = rx(i,t131,t184,2,1)
        t1966 = rx(i,t131,t184,0,1)
        t1967 = rx(i,t131,t184,1,0)
        t1971 = rx(i,t131,t184,2,0)
        t1973 = rx(i,t131,t184,0,2)
        t1979 = 0.1E1 / (t1957 * t1958 * t1960 - t1957 * t1962 * t1964 -
     # t1958 * t1971 * t1973 - t1960 * t1966 * t1967 + t1962 * t1966 * t
     #1971 + t1964 * t1967 * t1973)
        t1980 = t4 * t1979
        t1990 = (t363 - t742) * t67 / 0.2E1 + (t742 - t1173) * t67 / 0.2
     #E1
        t2007 = t675 * t1809
        t2024 = t1930 ** 2
        t2025 = t1923 ** 2
        t2026 = t1919 ** 2
        t2029 = t664 ** 2
        t2030 = t657 ** 2
        t2031 = t653 ** 2
        t2033 = t672 * (t2029 + t2030 + t2031)
        t2038 = t1971 ** 2
        t2039 = t1964 ** 2
        t2040 = t1960 ** 2
        t1948 = t1939 * (t1917 * t1923 + t1919 * t1921 + t1926 * t1930)
        t1963 = t1980 * (t1958 * t1964 + t1960 * t1962 + t1967 * t1971)
        t2049 = (t4 * (t291 * (t1772 + t1773 + t1774) / 0.2E1 + t1781 / 
     #0.2E1) * t300 - t4 * (t1781 / 0.2E1 + t1103 * (t1786 + t1787 + t17
     #88) / 0.2E1) * t679) * t67 + (t369 * (t149 / 0.2E1 + (t147 - t1798
     #) * t129 / 0.2E1) - t1811) * t67 / 0.2E1 + (t1811 - t1165 * (t584 
     #/ 0.2E1 + (t582 - t1815) * t129 / 0.2E1)) * t67 / 0.2E1 + (t292 * 
     #(t269 * t283 + t272 * t285 + t276 * t278) * t367 - t1836) * t67 / 
     #0.2E1 + (t1836 - t1104 * (t1081 * t1095 + t1084 * t1097 + t1088 * 
     #t1090) * t1177) * t67 / 0.2E1 + t686 + (t683 - t1872 * (t1849 * t1
     #859 + t1850 * t1858 + t1854 * t1865) * ((t1798 - t1805) * t67 / 0.
     #2E1 + (t1805 - t1815) * t67 / 0.2E1)) * t129 / 0.2E1 + (t709 - t4 
     #* (t1871 * t1891 / 0.2E1 + t705 / 0.2E1) * t1807) * t129 + t751 + 
     #(t748 - t1872 * (t1850 * t1856 + t1852 * t1854 + t1859 * t1863) * 
     #((t1903 - t1805) * t182 / 0.2E1 + (t1805 - t1906) * t182 / 0.2E1))
     # * t129 / 0.2E1 + (t1939 * (t1916 * t1930 + t1919 * t1932 + t1923 
     #* t1925) * t1949 - t1953) * t182 / 0.2E1 + (t1953 - t1980 * (t1957
     # * t1971 + t1960 * t1973 + t1964 * t1966) * t1990) * t182 / 0.2E1 
     #+ (t1948 * (t835 / 0.2E1 + (t739 - t1903) * t129 / 0.2E1) - t2007)
     # * t182 / 0.2E1 + (t2007 - t1963 * (t852 / 0.2E1 + (t742 - t1906) 
     #* t129 / 0.2E1)) * t182 / 0.2E1 + (t4 * (t1938 * (t2024 + t2025 + 
     #t2026) / 0.2E1 + t2033 / 0.2E1) * t741 - t4 * (t2033 / 0.2E1 + t19
     #79 * (t2038 + t2039 + t2040) / 0.2E1) * t744) * t182
        t2057 = sqrt(t1891)
        t2067 = t709 + t708 * dt * t925 / 0.2E1 + t1710 + t1714 - t1724 
     #- t1718 / 0.2E1 - t81 * t672 * t1716 * (t2049 * t671 + src(i,t131,
     #k,nComp,n)) / 0.4E1 - dy * (t1720 / 0.2E1 + (-cc * t1871 * t2057 *
     # ut(i,t1797,k,n) + t1718) * t129 / 0.2E1) / 0.4E1
        t2072 = t4 * (t672 * t738 / 0.2E1 + t1728 / 0.2E1)
        t2076 = ut(i,t131,t179,n)
        t2079 = ut(i,t131,t184,n)
        t2087 = t2072 * (t215 / 0.4E1 + t218 / 0.4E1 + t741 / 0.4E1 + t7
     #44 / 0.4E1) + t2072 * dt * (t949 / 0.4E1 + t952 / 0.4E1 + (t2076 -
     # t923) * t182 / 0.4E1 + (t923 - t2079) * t182 / 0.4E1) / 0.2E1
        t2096 = t4 * (t774 * t779 / 0.2E1 + t934 / 0.2E1)
        t2109 = t2096 * (t404 / 0.4E1 + t781 / 0.4E1 + t68 / 0.4E1 + t57
     #0 / 0.4E1) + t2096 * dt * ((t941 - t947) * t67 / 0.4E1 + (t947 - t
     #1363) * t67 / 0.4E1 + t1391 / 0.4E1 + t1392 / 0.4E1) / 0.2E1
        t2114 = t4 * (t774 * t831 / 0.2E1 + t1728 / 0.2E1)
        t2127 = t2114 * (t833 / 0.4E1 + t835 / 0.4E1 + t164 / 0.4E1 + t1
     #67 / 0.4E1) + t2114 * dt * ((t1735 - t947) * t129 / 0.4E1 + (t947 
     #- t2076) * t129 / 0.4E1 + t922 / 0.4E1 + t925 / 0.4E1) / 0.2E1
        t2134 = sqrt(t863)
        t2136 = cc * t774 * t2134 * t947
        t2139 = t373 ** 2
        t2140 = t382 ** 2
        t2141 = t389 ** 2
        t2144 = t752 ** 2
        t2145 = t761 ** 2
        t2146 = t768 ** 2
        t2148 = t774 * (t2144 + t2145 + t2146)
        t2153 = t1183 ** 2
        t2154 = t1192 ** 2
        t2155 = t1199 ** 2
        t2085 = t775 * (t752 * t762 + t753 * t761 + t757 * t768)
        t2175 = t2085 * t837
        t2188 = k + 2
        t2189 = u(t5,j,t2188,n)
        t2196 = u(i,j,t2188,n)
        t2198 = (t2196 - t213) * t182
        t2200 = t2198 / 0.2E1 + t215 / 0.2E1
        t2202 = t726 * t2200
        t2206 = u(t536,j,t2188,n)
        t2223 = t2085 * t783
        t2236 = t1564 ** 2
        t2237 = t1555 ** 2
        t2238 = t1559 ** 2
        t2241 = t762 ** 2
        t2242 = t753 ** 2
        t2243 = t757 ** 2
        t2245 = t774 * (t2241 + t2242 + t2243)
        t2250 = t1926 ** 2
        t2251 = t1917 ** 2
        t2252 = t1921 ** 2
        t2261 = u(i,t126,t2188,n)
        t2269 = t778 * t2200
        t2273 = u(i,t131,t2188,n)
        t2283 = rx(i,j,t2188,0,0)
        t2284 = rx(i,j,t2188,1,1)
        t2286 = rx(i,j,t2188,2,2)
        t2288 = rx(i,j,t2188,1,2)
        t2290 = rx(i,j,t2188,2,1)
        t2292 = rx(i,j,t2188,0,1)
        t2293 = rx(i,j,t2188,1,0)
        t2297 = rx(i,j,t2188,2,0)
        t2299 = rx(i,j,t2188,0,2)
        t2305 = 0.1E1 / (t2283 * t2284 * t2286 - t2283 * t2288 * t2290 -
     # t2284 * t2297 * t2299 - t2286 * t2292 * t2293 + t2288 * t2292 * t
     #2297 + t2290 * t2293 * t2299)
        t2306 = t4 * t2305
        t2337 = t2297 ** 2
        t2338 = t2290 ** 2
        t2339 = t2286 ** 2
        t2340 = t2337 + t2338 + t2339
        t2348 = (t4 * (t395 * (t2139 + t2140 + t2141) / 0.2E1 + t2148 / 
     #0.2E1) * t404 - t4 * (t2148 / 0.2E1 + t1205 * (t2153 + t2154 + t21
     #55) / 0.2E1) * t781) * t67 + (t396 * (t373 * t383 + t374 * t382 + 
     #t378 * t389) * t462 - t2175) * t67 / 0.2E1 + (t2175 - t1206 * (t11
     #83 * t1193 + t1184 * t1192 + t1188 * t1199) * t1268) * t67 / 0.2E1
     # + (t433 * ((t2189 - t196) * t182 / 0.2E1 + t198 / 0.2E1) - t2202)
     # * t67 / 0.2E1 + (t2202 - t1224 * ((t2206 - t596) * t182 / 0.2E1 +
     # t598 / 0.2E1)) * t67 / 0.2E1 + (t1577 * (t1554 * t1564 + t1555 * 
     #t1563 + t1559 * t1570) * t1587 - t2223) * t129 / 0.2E1 + (t2223 - 
     #t1939 * (t1916 * t1926 + t1917 * t1925 + t1921 * t1932) * t1949) *
     # t129 / 0.2E1 + (t4 * (t1576 * (t2236 + t2237 + t2238) / 0.2E1 + t
     #2245 / 0.2E1) * t833 - t4 * (t2245 / 0.2E1 + t1938 * (t2250 + t225
     #1 + t2252) / 0.2E1) * t835) * t129 + (t1603 * ((t2261 - t716) * t1
     #82 / 0.2E1 + t718 / 0.2E1) - t2269) * t129 / 0.2E1 + (t2269 - t194
     #8 * ((t2273 - t739) * t182 / 0.2E1 + t741 / 0.2E1)) * t129 / 0.2E1
     # + (t2306 * (t2283 * t2297 + t2286 * t2299 + t2290 * t2292) * ((t2
     #189 - t2196) * t67 / 0.2E1 + (t2196 - t2206) * t67 / 0.2E1) - t785
     #) * t182 / 0.2E1 + t790 + (t2306 * (t2284 * t2290 + t2286 * t2288 
     #+ t2293 * t2297) * ((t2261 - t2196) * t129 / 0.2E1 + (t2196 - t227
     #3) * t129 / 0.2E1) - t839) * t182 / 0.2E1 + t844 + (t4 * (t2305 * 
     #t2340 / 0.2E1 + t864 / 0.2E1) * t2198 - t873) * t182
        t2356 = sqrt(t2340)
        t2362 = sqrt(t868)
        t2364 = t524 * t2362 * t2
        t2366 = (-t2364 + t2136) * t182
        t2371 = t2364 / 0.2E1
        t2375 = t823 * t2362 * t888 / 0.4E1
        t2377 = sqrt(t877)
        t2379 = cc * t813 * t2377 * t950
        t2381 = (t2364 - t2379) * t182
        t2385 = dz * (t2366 / 0.2E1 + t2381 / 0.2E1) / 0.4E1
        t2386 = t873 + t872 * dt * t949 / 0.2E1 + t2136 / 0.2E1 + t81 * 
     #t774 * t2134 * (t2348 * t773 + src(i,j,t179,nComp,n)) / 0.4E1 - dz
     # * ((cc * t2305 * t2356 * ut(i,j,t2188,n) - t2136) * t182 / 0.2E1 
     #+ t2366 / 0.2E1) / 0.4E1 - t2371 - t2375 - t2385
        t2391 = t4 * (t813 * t818 / 0.2E1 + t934 / 0.2E1)
        t2404 = t2391 * (t68 / 0.4E1 + t570 / 0.4E1 + t445 / 0.4E1 + t82
     #0 / 0.4E1) + t2391 * dt * (t1391 / 0.4E1 + t1392 / 0.4E1 + (t944 -
     # t950) * t67 / 0.4E1 + (t950 - t1366) * t67 / 0.4E1) / 0.2E1
        t2409 = t4 * (t813 * t848 / 0.2E1 + t1728 / 0.2E1)
        t2422 = t2409 * (t164 / 0.4E1 + t167 / 0.4E1 + t850 / 0.4E1 + t8
     #52 / 0.4E1) + t2409 * dt * (t922 / 0.4E1 + t925 / 0.4E1 + (t1738 -
     # t950) * t129 / 0.4E1 + (t950 - t2079) * t129 / 0.4E1) / 0.2E1
        t2430 = t414 ** 2
        t2431 = t423 ** 2
        t2432 = t430 ** 2
        t2435 = t791 ** 2
        t2436 = t800 ** 2
        t2437 = t807 ** 2
        t2439 = t813 * (t2435 + t2436 + t2437)
        t2444 = t1222 ** 2
        t2445 = t1231 ** 2
        t2446 = t1238 ** 2
        t2357 = t814 * (t791 * t801 + t792 * t800 + t796 * t807)
        t2466 = t2357 * t854
        t2479 = k - 2
        t2480 = u(t5,j,t2479,n)
        t2487 = u(i,j,t2479,n)
        t2489 = (t216 - t2487) * t182
        t2491 = t218 / 0.2E1 + t2489 / 0.2E1
        t2493 = t764 * t2491
        t2497 = u(t536,j,t2479,n)
        t2514 = t2357 * t822
        t2527 = t1605 ** 2
        t2528 = t1596 ** 2
        t2529 = t1600 ** 2
        t2532 = t801 ** 2
        t2533 = t792 ** 2
        t2534 = t796 ** 2
        t2536 = t813 * (t2532 + t2533 + t2534)
        t2541 = t1967 ** 2
        t2542 = t1958 ** 2
        t2543 = t1962 ** 2
        t2552 = u(i,t126,t2479,n)
        t2560 = t797 * t2491
        t2564 = u(i,t131,t2479,n)
        t2574 = rx(i,j,t2479,0,0)
        t2575 = rx(i,j,t2479,1,1)
        t2577 = rx(i,j,t2479,2,2)
        t2579 = rx(i,j,t2479,1,2)
        t2581 = rx(i,j,t2479,2,1)
        t2583 = rx(i,j,t2479,0,1)
        t2584 = rx(i,j,t2479,1,0)
        t2588 = rx(i,j,t2479,2,0)
        t2590 = rx(i,j,t2479,0,2)
        t2596 = 0.1E1 / (t2574 * t2575 * t2577 - t2574 * t2579 * t2581 -
     # t2575 * t2588 * t2590 - t2577 * t2583 * t2584 + t2579 * t2583 * t
     #2588 + t2581 * t2584 * t2590)
        t2597 = t4 * t2596
        t2628 = t2588 ** 2
        t2629 = t2581 ** 2
        t2630 = t2577 ** 2
        t2631 = t2628 + t2629 + t2630
        t2639 = (t4 * (t436 * (t2430 + t2431 + t2432) / 0.2E1 + t2439 / 
     #0.2E1) * t445 - t4 * (t2439 / 0.2E1 + t1244 * (t2444 + t2445 + t24
     #46) / 0.2E1) * t820) * t67 + (t437 * (t414 * t424 + t415 * t423 + 
     #t419 * t430) * t479 - t2466) * t67 / 0.2E1 + (t2466 - t1245 * (t12
     #22 * t1232 + t1223 * t1231 + t1227 * t1238) * t1285) * t67 / 0.2E1
     # + (t446 * (t201 / 0.2E1 + (t199 - t2480) * t182 / 0.2E1) - t2493)
     # * t67 / 0.2E1 + (t2493 - t1240 * (t601 / 0.2E1 + (t599 - t2497) *
     # t182 / 0.2E1)) * t67 / 0.2E1 + (t1618 * (t1595 * t1605 + t1596 * 
     #t1604 + t1600 * t1611) * t1628 - t2514) * t129 / 0.2E1 + (t2514 - 
     #t1980 * (t1957 * t1967 + t1958 * t1966 + t1962 * t1973) * t1990) *
     # t129 / 0.2E1 + (t4 * (t1617 * (t2527 + t2528 + t2529) / 0.2E1 + t
     #2536 / 0.2E1) * t850 - t4 * (t2536 / 0.2E1 + t1979 * (t2541 + t254
     #2 + t2543) / 0.2E1) * t852) * t129 + (t1616 * (t721 / 0.2E1 + (t71
     #9 - t2552) * t182 / 0.2E1) - t2560) * t129 / 0.2E1 + (t2560 - t196
     #3 * (t744 / 0.2E1 + (t742 - t2564) * t182 / 0.2E1)) * t129 / 0.2E1
     # + t827 + (t824 - t2597 * (t2574 * t2588 + t2577 * t2590 + t2581 *
     # t2583) * ((t2480 - t2487) * t67 / 0.2E1 + (t2487 - t2497) * t67 /
     # 0.2E1)) * t182 / 0.2E1 + t859 + (t856 - t2597 * (t2575 * t2581 + 
     #t2577 * t2579 + t2584 * t2588) * ((t2552 - t2487) * t129 / 0.2E1 +
     # (t2487 - t2564) * t129 / 0.2E1)) * t182 / 0.2E1 + (t882 - t4 * (t
     #2596 * t2631 / 0.2E1 + t878 / 0.2E1) * t2489) * t182
        t2647 = sqrt(t2631)
        t2657 = t882 + t881 * dt * t952 / 0.2E1 + t2371 + t2375 - t2385 
     #- t2379 / 0.2E1 - t81 * t813 * t2377 * (t2639 * t812 + src(i,j,t18
     #4,nComp,n)) / 0.4E1 - dz * (t2381 / 0.2E1 + (-cc * t2596 * t2647 *
     # ut(i,j,t2479,n) + t2379) * t182 / 0.2E1) / 0.4E1
        t2664 = src(i,j,k,nComp,n + 1)

        unew(i,j,k) = t1 + dt * t2 + (-t1334 * t904 / 0.2E1 - t1354 *
     # t904 / 0.2E1 - t1374 * t904 / 0.2E1 + t903 * t904 / 0.2E1 + t931 
     #* t904 / 0.2E1 + t958 * t904 / 0.2E1) * t55 * t67 + (t1398 * t904 
     #/ 0.2E1 + t1725 * t904 / 0.2E1 + t1746 * t904 / 0.2E1 - t1764 * t9
     #04 / 0.2E1 - t2067 * t904 / 0.2E1 - t2087 * t904 / 0.2E1) * t55 * 
     #t129 + (t2109 * t904 / 0.2E1 + t2127 * t904 / 0.2E1 + t2386 * t904
     # / 0.2E1 - t2404 * t904 / 0.2E1 - t2422 * t904 / 0.2E1 - t2657 * t
     #904 / 0.2E1) * t55 * t182 + t2664 * t904 / 0.2E1

        utnew(i,j,k) = t2 + (-dt * 
     #t1334 - dt * t1354 - dt * t1374 + dt * t903 + dt * t931 + dt * t95
     #8) * t55 * t67 + (dt * t1398 + dt * t1725 + dt * t1746 - dt * t176
     #4 - dt * t2067 - dt * t2087) * t55 * t129 + (dt * t2109 + dt * t21
     #27 + dt * t2386 - dt * t2404 - dt * t2422 - dt * t2657) * t55 * t1
     #82 + t2664 * dt


        return
      end
