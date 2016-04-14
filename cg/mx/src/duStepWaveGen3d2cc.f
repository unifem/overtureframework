      subroutine duStepWaveGen3d2cc( 
     *   nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *   n1a,n1b,n2a,n2b,n3a,n3b,
     *   u,ut,unew,utnew,rx,
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
      real rx   (nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:2,0:2)
      real dx,dy,dz,dt,cc
c
c.. generated code to follow
c
        real t1
        real t1001
        real t1004
        real t1018
        real t1020
        real t1021
        real t1031
        real t1032
        real t1034
        real t1036
        real t1038
        real t1040
        real t1041
        real t1045
        real t1047
        real t105
        real t1053
        real t1054
        real t106
        real t1066
        real t1068
        real t107
        real t1072
        real t1073
        real t1075
        real t1077
        real t1079
        real t108
        real t1081
        real t1082
        real t1086
        real t1088
        real t109
        real t1094
        real t1095
        real t11
        real t1109
        real t111
        real t1110
        real t1111
        real t1114
        real t1115
        real t1116
        real t1118
        real t1123
        real t1124
        real t1125
        real t1138
        real t114
        real t1141
        real t1145
        real t1146
        real t1153
        real t1157
        real t116
        real t1161
        real t1164
        real t1168
        real t1174
        real t1175
        real t1177
        real t1179
        real t1181
        real t1183
        real t1184
        real t1188
        real t1190
        real t1196
        real t1197
        real t120
        real t1209
        real t121
        real t1213
        real t1214
        real t1216
        real t1217
        real t1218
        real t1220
        real t1222
        real t1223
        real t1227
        real t1229
        real t1232
        real t1235
        real t1236
        integer t125
        real t1259
        real t126
        real t1263
        real t1276
        real t128
        real t1280
        real t1282
        real t1283
        real t1284
        real t1287
        real t1288
        real t1289
        real t1291
        real t1296
        real t1297
        real t1298
        real t13
        integer t130
        real t1307
        real t131
        real t1312
        real t1322
        real t1327
        real t1331
        real t1334
        real t1342
        real t1347
        real t135
        real t1351
        real t1354
        real t1362
        real t1371
        real t1372
        real t1379
        real t138
        real t1380
        real t1386
        real t1393
        real t1395
        real t1397
        real t1398
        real t1399
        real t1402
        real t1403
        real t1404
        real t1406
        real t1411
        real t1412
        real t1413
        real t142
        integer t1422
        real t1423
        real t143
        real t1430
        real t1432
        real t1434
        real t1436
        real t1440
        real t145
        real t146
        real t1461
        real t147
        real t1474
        real t1475
        real t1477
        real t1479
        real t148
        real t1481
        real t1483
        real t1484
        real t1488
        real t1490
        real t1496
        real t1497
        real t15
        real t150
        real t1513
        real t1514
        real t1515
        real t1516
        real t152
        real t1528
        real t1531
        real t1541
        real t1542
        real t1544
        real t1546
        real t1548
        real t1550
        real t1551
        real t1555
        real t1557
        real t156
        real t1563
        real t1564
        real t1574
        real t1578
        real t1582
        real t1583
        real t1585
        real t1587
        real t1589
        real t1591
        real t1592
        real t1593
        real t1596
        real t1598
        real t16
        real t160
        real t1604
        real t1605
        real t1606
        real t161
        real t1615
        real t163
        real t1632
        real t164
        real t1649
        real t1650
        real t1651
        real t1654
        real t1655
        real t1656
        real t1658
        real t166
        real t1663
        real t1664
        real t1665
        real t1674
        real t1679
        real t168
        real t1685
        real t1687
        real t1689
        real t1694
        real t1697
        real t1699
        real t170
        real t1701
        real t1703
        real t1707
        real t1708
        real t1711
        real t1714
        real t1718
        real t1721
        real t1722
        real t1729
        real t173
        real t1734
        real t1747
        real t1754
        real t1755
        real t1756
        real t1759
        real t1760
        real t1761
        real t1763
        real t1768
        real t1769
        real t1770
        integer t1779
        integer t178
        real t1780
        real t1787
        real t1789
        real t179
        real t1791
        real t1793
        real t1797
        real t181
        real t1818
        integer t183
        real t1831
        real t1832
        real t1834
        real t1836
        real t1838
        real t184
        real t1840
        real t1841
        real t1845
        real t1847
        real t1853
        real t1854
        real t1870
        real t1871
        real t1872
        real t1873
        real t1885
        real t1888
        real t1898
        real t1899
        real t1901
        real t1903
        real t1905
        real t1907
        real t1908
        real t1912
        real t1914
        real t1920
        real t1921
        real t1931
        real t1933
        real t1935
        real t1939
        real t194
        real t1940
        real t1942
        real t1944
        real t1946
        real t1948
        real t1949
        real t195
        real t1950
        real t1953
        real t1955
        real t1961
        real t1962
        real t197
        real t1972
        real t198
        real t1989
        real t2
        real t20
        real t200
        real t2006
        real t2007
        real t2008
        real t2011
        real t2012
        real t2013
        real t2015
        real t202
        real t2020
        real t2021
        real t2022
        real t2031
        real t2036
        real t204
        real t2046
        real t2051
        real t2055
        real t2058
        real t2065
        real t2066
        real t2075
        real t2088
        real t2093
        real t2106
        real t211
        real t2113
        real t2115
        real t2117
        real t2118
        real t2119
        real t212
        real t2122
        real t2123
        real t2124
        real t2126
        real t2131
        real t2132
        real t2133
        real t214
        real t215
        real t2153
        integer t2166
        real t2167
        real t217
        real t2174
        real t2176
        real t2178
        real t2180
        real t2184
        real t219
        real t22
        real t2201
        real t221
        real t2214
        real t2215
        real t2216
        real t2219
        real t2220
        real t2221
        real t2223
        real t2228
        real t2229
        real t2230
        real t2239
        real t224
        real t2247
        real t225
        real t2251
        real t226
        real t2261
        real t2262
        real t2264
        real t2266
        real t2268
        real t2270
        real t2271
        real t2275
        real t2277
        real t228
        real t2283
        real t2284
        real t229
        real t230
        real t2315
        real t2316
        real t2317
        real t2318
        real t232
        real t2326
        real t2331
        real t2333
        real t2337
        real t2339
        real t234
        real t2341
        real t2346
        real t2349
        real t235
        real t2351
        real t2353
        real t2355
        real t2359
        real t2360
        real t2365
        real t2378
        real t2383
        real t239
        real t2396
        real t2403
        real t2404
        real t2405
        real t2408
        real t2409
        real t241
        real t2410
        real t2412
        real t2417
        real t2418
        real t2419
        real t2439
        integer t2452
        real t2453
        real t2460
        real t2462
        real t2464
        real t2466
        real t247
        real t2470
        real t248
        real t2487
        real t2500
        real t2501
        real t2502
        real t2505
        real t2506
        real t2507
        real t2509
        real t2514
        real t2515
        real t2516
        real t2525
        real t2533
        real t2537
        real t2547
        real t2548
        real t2550
        real t2552
        real t2554
        real t2556
        real t2557
        real t256
        real t2561
        real t2563
        real t2569
        real t2570
        real t2601
        real t2602
        real t2603
        real t2604
        real t2612
        real t2617
        real t262
        real t2627
        real t264
        real t268
        real t269
        real t271
        real t273
        real t275
        real t277
        real t278
        real t28
        real t282
        real t284
        real t29
        real t290
        real t291
        real t299
        real t30
        real t307
        real t308
        real t309
        real t31
        real t312
        real t313
        real t314
        real t316
        real t32
        real t321
        real t322
        real t323
        real t33
        real t336
        real t339
        real t34
        real t343
        real t35
        real t351
        real t357
        real t359
        real t362
        real t366
        real t369
        real t37
        real t372
        real t373
        real t375
        real t377
        real t379
        real t381
        real t382
        real t386
        real t388
        real t39
        real t394
        real t395
        real t4
        real t403
        real t409
        real t41
        real t413
        real t414
        real t416
        real t418
        real t420
        real t422
        real t423
        real t427
        real t429
        real t43
        real t433
        real t435
        real t436
        real t44
        real t444
        real t446
        real t461
        real t465
        real t478
        real t48
        real t484
        real t485
        real t486
        real t489
        real t490
        real t491
        real t493
        real t498
        real t499
        integer t5
        real t50
        real t500
        real t509
        real t514
        real t520
        real t521
        real t523
        real t525
        real t530
        integer t531
        real t532
        real t533
        real t534
        real t535
        real t537
        real t539
        real t541
        real t542
        real t546
        real t548
        real t55
        real t550
        real t554
        real t555
        real t556
        real t557
        real t558
        real t559
        real t56
        real t562
        real t563
        real t565
        real t566
        real t569
        real t57
        real t573
        real t574
        real t576
        real t577
        real t579
        real t58
        real t581
        real t583
        real t586
        real t59
        real t590
        real t591
        real t592
        real t593
        real t594
        real t596
        real t598
        real t6
        real t60
        real t600
        real t603
        real t604
        real t605
        real t607
        real t609
        real t61
        real t611
        real t613
        real t614
        real t618
        real t620
        real t626
        real t627
        real t631
        real t632
        real t633
        real t635
        real t637
        real t639
        real t64
        real t641
        real t644
        real t645
        real t646
        real t648
        real t65
        real t650
        real t652
        real t654
        real t655
        real t658
        real t659
        real t661
        real t664
        real t667
        real t668
        real t67
        real t672
        real t674
        real t676
        real t678
        real t679
        real t68
        real t681
        real t682
        real t683
        real t684
        real t685
        real t686
        real t687
        real t688
        real t689
        real t69
        real t690
        real t691
        real t694
        real t695
        real t696
        real t697
        real t698
        real t699
        real t7
        real t70
        real t700
        real t703
        real t704
        real t710
        real t711
        real t713
        real t714
        real t716
        real t718
        real t720
        real t724
        real t726
        real t729
        real t730
        real t733
        real t734
        real t736
        real t737
        real t739
        real t741
        real t743
        real t746
        real t747
        real t748
        real t750
        real t752
        real t754
        real t756
        real t757
        real t761
        real t763
        real t768
        real t769
        real t77
        real t770
        real t774
        real t776
        real t778
        real t780
        real t782
        real t784
        real t785
        real t786
        real t787
        real t789
        real t79
        real t791
        real t793
        real t795
        real t796
        real t800
        real t802
        real t803
        real t808
        real t809
        real t81
        real t813
        real t815
        real t817
        real t819
        integer t82
        real t822
        real t826
        real t828
        real t83
        real t830
        real t832
        real t834
        real t836
        real t838
        real t839
        real t84
        real t843
        real t845
        real t847
        real t849
        real t851
        real t854
        real t855
        real t856
        real t857
        real t858
        real t859
        real t86
        real t860
        real t861
        real t862
        real t863
        real t864
        real t867
        real t868
        real t869
        real t870
        real t871
        real t872
        real t873
        real t876
        real t877
        real t88
        real t880
        real t883
        real t885
        real t886
        real t888
        real t890
        real t894
        real t895
        real t896
        real t899
        real t9
        real t90
        real t902
        real t906
        real t909
        real t912
        real t914
        real t915
        real t917
        real t92
        real t923
        real t926
        real t929
        real t93
        real t933
        real t936
        real t939
        real t941
        real t942
        real t944
        real t950
        integer t958
        real t959
        real t960
        real t962
        real t964
        real t966
        real t968
        real t969
        real t97
        real t973
        real t975
        real t981
        real t982
        real t983
        real t984
        real t985
        real t99
        real t990
        real t992
        real t996
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
        t28 = 0.1E1 / (-t11 * t13 * t6 + t11 * t15 * t20 + t13 * t16 * t
     #22 - t15 * t16 * t9 - t20 * t22 * t7 + t6 * t7 * t9)
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
        t82 = i + 2
        t83 = rx(t82,j,k,0,0)
        t84 = rx(t82,j,k,1,1)
        t86 = rx(t82,j,k,2,2)
        t88 = rx(t82,j,k,1,2)
        t90 = rx(t82,j,k,2,1)
        t92 = rx(t82,j,k,0,1)
        t93 = rx(t82,j,k,1,0)
        t97 = rx(t82,j,k,2,0)
        t99 = rx(t82,j,k,0,2)
        t105 = 0.1E1 / (t83 * t84 * t86 - t83 * t88 * t90 - t84 * t97 * 
     #t99 - t86 * t92 * t93 + t88 * t92 * t97 + t90 * t93 * t99)
        t106 = t83 ** 2
        t107 = t92 ** 2
        t108 = t99 ** 2
        t109 = t106 + t107 + t108
        t114 = u(t82,j,k,n)
        t116 = (t114 - t65) * t67
        t120 = t4 * t105
        t125 = j + 1
        t126 = u(t82,t125,k,n)
        t128 = 0.1E1 / dy
        t130 = j - 1
        t131 = u(t82,t130,k,n)
        t138 = t4 * t28
        t142 = t11 * t22 + t15 * t7 + t16 * t6
        t143 = u(t5,t125,k,n)
        t145 = (t143 - t65) * t128
        t146 = u(t5,t130,k,n)
        t148 = (t65 - t146) * t128
        t150 = t145 / 0.2E1 + t148 / 0.2E1
        t111 = t138 * t142
        t152 = t111 * t150
        t156 = t4 * t56
        t160 = t34 * t44 + t35 * t43 + t39 * t50
        t161 = u(i,t125,k,n)
        t163 = (t161 - t1) * t128
        t164 = u(i,t130,k,n)
        t166 = (t1 - t164) * t128
        t168 = t163 / 0.2E1 + t166 / 0.2E1
        t121 = t156 * t160
        t170 = t121 * t168
        t173 = (t152 - t170) * t67 / 0.2E1
        t178 = k + 1
        t179 = u(t82,j,t178,n)
        t181 = 0.1E1 / dz
        t183 = k - 1
        t184 = u(t82,j,t183,n)
        t194 = t13 * t15 + t20 * t6 + t22 * t9
        t195 = u(t5,j,t178,n)
        t197 = (t195 - t65) * t181
        t198 = u(t5,j,t183,n)
        t200 = (t65 - t198) * t181
        t202 = t197 / 0.2E1 + t200 / 0.2E1
        t135 = t138 * t194
        t204 = t135 * t202
        t211 = t34 * t48 + t37 * t50 + t41 * t43
        t212 = u(i,j,t178,n)
        t214 = (t212 - t1) * t181
        t215 = u(i,j,t183,n)
        t217 = (t1 - t215) * t181
        t219 = t214 / 0.2E1 + t217 / 0.2E1
        t147 = t156 * t211
        t221 = t147 * t219
        t224 = (t204 - t221) * t67 / 0.2E1
        t225 = rx(t5,t125,k,0,0)
        t226 = rx(t5,t125,k,1,1)
        t228 = rx(t5,t125,k,2,2)
        t230 = rx(t5,t125,k,1,2)
        t232 = rx(t5,t125,k,2,1)
        t234 = rx(t5,t125,k,0,1)
        t235 = rx(t5,t125,k,1,0)
        t239 = rx(t5,t125,k,2,0)
        t241 = rx(t5,t125,k,0,2)
        t247 = 0.1E1 / (t225 * t226 * t228 - t225 * t230 * t232 - t226 *
     # t239 * t241 - t228 * t234 * t235 + t230 * t234 * t239 + t232 * t2
     #35 * t241)
        t248 = t4 * t247
        t256 = (t143 - t161) * t67
        t262 = t116 / 0.2E1 + t68 / 0.2E1
        t264 = t111 * t262
        t268 = rx(t5,t130,k,0,0)
        t269 = rx(t5,t130,k,1,1)
        t271 = rx(t5,t130,k,2,2)
        t273 = rx(t5,t130,k,1,2)
        t275 = rx(t5,t130,k,2,1)
        t277 = rx(t5,t130,k,0,1)
        t278 = rx(t5,t130,k,1,0)
        t282 = rx(t5,t130,k,2,0)
        t284 = rx(t5,t130,k,0,2)
        t290 = 0.1E1 / (t268 * t269 * t271 - t268 * t273 * t275 - t269 *
     # t282 * t284 - t271 * t277 * t278 + t273 * t277 * t282 + t275 * t2
     #78 * t284)
        t291 = t4 * t290
        t299 = (t146 - t164) * t67
        t307 = t235 ** 2
        t308 = t226 ** 2
        t309 = t230 ** 2
        t312 = t16 ** 2
        t313 = t7 ** 2
        t314 = t11 ** 2
        t316 = t28 * (t312 + t313 + t314)
        t321 = t278 ** 2
        t322 = t269 ** 2
        t323 = t273 ** 2
        t336 = u(t5,t125,t178,n)
        t339 = u(t5,t125,t183,n)
        t343 = (t336 - t143) * t181 / 0.2E1 + (t143 - t339) * t181 / 0.2
     #E1
        t229 = t138 * (t11 * t9 + t13 * t7 + t16 * t20)
        t351 = t229 * t202
        t359 = u(t5,t130,t178,n)
        t362 = u(t5,t130,t183,n)
        t366 = (t359 - t146) * t181 / 0.2E1 + (t146 - t362) * t181 / 0.2
     #E1
        t372 = rx(t5,j,t178,0,0)
        t373 = rx(t5,j,t178,1,1)
        t375 = rx(t5,j,t178,2,2)
        t377 = rx(t5,j,t178,1,2)
        t379 = rx(t5,j,t178,2,1)
        t381 = rx(t5,j,t178,0,1)
        t382 = rx(t5,j,t178,1,0)
        t386 = rx(t5,j,t178,2,0)
        t388 = rx(t5,j,t178,0,2)
        t394 = 0.1E1 / (t372 * t373 * t375 - t372 * t377 * t379 - t373 *
     # t386 * t388 - t375 * t381 * t382 + t377 * t381 * t386 + t379 * t3
     #82 * t388)
        t395 = t4 * t394
        t403 = (t195 - t212) * t67
        t409 = t135 * t262
        t413 = rx(t5,j,t183,0,0)
        t414 = rx(t5,j,t183,1,1)
        t416 = rx(t5,j,t183,2,2)
        t418 = rx(t5,j,t183,1,2)
        t420 = rx(t5,j,t183,2,1)
        t422 = rx(t5,j,t183,0,1)
        t423 = rx(t5,j,t183,1,0)
        t427 = rx(t5,j,t183,2,0)
        t429 = rx(t5,j,t183,0,2)
        t435 = 0.1E1 / (t413 * t414 * t416 - t413 * t418 * t420 - t414 *
     # t427 * t429 - t416 * t422 * t423 + t418 * t422 * t427 + t420 * t4
     #23 * t429)
        t436 = t4 * t435
        t444 = (t198 - t215) * t67
        t461 = (t336 - t195) * t128 / 0.2E1 + (t195 - t359) * t128 / 0.2
     #E1
        t465 = t229 * t150
        t478 = (t339 - t198) * t128 / 0.2E1 + (t198 - t362) * t128 / 0.2
     #E1
        t484 = t386 ** 2
        t485 = t379 ** 2
        t486 = t375 ** 2
        t489 = t20 ** 2
        t490 = t13 ** 2
        t491 = t9 ** 2
        t493 = t28 * (t489 + t490 + t491)
        t498 = t427 ** 2
        t499 = t420 ** 2
        t500 = t416 ** 2
        t357 = t248 * (t225 * t235 + t226 * t234 + t230 * t241)
        t369 = t291 * (t268 * t278 + t269 * t277 + t273 * t284)
        t433 = t395 * (t372 * t386 + t375 * t388 + t379 * t381)
        t446 = t436 * (t413 * t427 + t416 * t429 + t420 * t422)
        t509 = (t4 * (t105 * t109 / 0.2E1 + t33 / 0.2E1) * t116 - t69) *
     # t67 + (t120 * (t83 * t93 + t84 * t92 + t88 * t99) * ((t126 - t114
     #) * t128 / 0.2E1 + (t114 - t131) * t128 / 0.2E1) - t152) * t67 / 0
     #.2E1 + t173 + (t120 * (t83 * t97 + t86 * t99 + t90 * t92) * ((t179
     # - t114) * t181 / 0.2E1 + (t114 - t184) * t181 / 0.2E1) - t204) * 
     #t67 / 0.2E1 + t224 + (t357 * ((t126 - t143) * t67 / 0.2E1 + t256 /
     # 0.2E1) - t264) * t128 / 0.2E1 + (t264 - t369 * ((t131 - t146) * t
     #67 / 0.2E1 + t299 / 0.2E1)) * t128 / 0.2E1 + (t4 * (t247 * (t307 +
     # t308 + t309) / 0.2E1 + t316 / 0.2E1) * t145 - t4 * (t316 / 0.2E1 
     #+ t290 * (t321 + t322 + t323) / 0.2E1) * t148) * t128 + (t248 * (t
     #226 * t232 + t228 * t230 + t235 * t239) * t343 - t351) * t128 / 0.
     #2E1 + (t351 - t291 * (t269 * t275 + t271 * t273 + t278 * t282) * t
     #366) * t128 / 0.2E1 + (t433 * ((t179 - t195) * t67 / 0.2E1 + t403 
     #/ 0.2E1) - t409) * t181 / 0.2E1 + (t409 - t446 * ((t184 - t198) * 
     #t67 / 0.2E1 + t444 / 0.2E1)) * t181 / 0.2E1 + (t395 * (t373 * t379
     # + t375 * t377 + t382 * t386) * t461 - t465) * t181 / 0.2E1 + (t46
     #5 - t436 * (t414 * t420 + t416 * t418 + t423 * t427) * t478) * t18
     #1 / 0.2E1 + (t4 * (t394 * (t484 + t485 + t486) / 0.2E1 + t493 / 0.
     #2E1) * t197 - t4 * (t493 / 0.2E1 + t435 * (t498 + t499 + t500) / 0
     #.2E1) * t200) * t181
        t514 = sqrt(t109)
        t520 = cc * t56
        t521 = sqrt(t60)
        t523 = t520 * t521 * t2
        t525 = (-t523 + t79) * t67
        t530 = t523 / 0.2E1
        t531 = i - 1
        t532 = rx(t531,j,k,0,0)
        t533 = rx(t531,j,k,1,1)
        t535 = rx(t531,j,k,2,2)
        t537 = rx(t531,j,k,1,2)
        t539 = rx(t531,j,k,2,1)
        t541 = rx(t531,j,k,0,1)
        t542 = rx(t531,j,k,1,0)
        t546 = rx(t531,j,k,2,0)
        t548 = rx(t531,j,k,0,2)
        t554 = 0.1E1 / (t532 * t533 * t535 - t532 * t537 * t539 - t533 *
     # t546 * t548 - t535 * t541 * t542 + t537 * t541 * t546 + t539 * t5
     #42 * t548)
        t555 = t532 ** 2
        t556 = t541 ** 2
        t557 = t548 ** 2
        t558 = t555 + t556 + t557
        t559 = t554 * t558
        t562 = t4 * (t61 / 0.2E1 + t559 / 0.2E1)
        t563 = u(t531,j,k,n)
        t565 = (t1 - t563) * t67
        t566 = t562 * t565
        t569 = t4 * t554
        t573 = t532 * t542 + t533 * t541 + t537 * t548
        t574 = u(t531,t125,k,n)
        t576 = (t574 - t563) * t128
        t577 = u(t531,t130,k,n)
        t579 = (t563 - t577) * t128
        t581 = t576 / 0.2E1 + t579 / 0.2E1
        t534 = t569 * t573
        t583 = t534 * t581
        t586 = (t170 - t583) * t67 / 0.2E1
        t590 = t532 * t546 + t535 * t548 + t539 * t541
        t591 = u(t531,j,t178,n)
        t593 = (t591 - t563) * t181
        t594 = u(t531,j,t183,n)
        t596 = (t563 - t594) * t181
        t598 = t593 / 0.2E1 + t596 / 0.2E1
        t550 = t569 * t590
        t600 = t550 * t598
        t603 = (t221 - t600) * t67 / 0.2E1
        t604 = rx(i,t125,k,0,0)
        t605 = rx(i,t125,k,1,1)
        t607 = rx(i,t125,k,2,2)
        t609 = rx(i,t125,k,1,2)
        t611 = rx(i,t125,k,2,1)
        t613 = rx(i,t125,k,0,1)
        t614 = rx(i,t125,k,1,0)
        t618 = rx(i,t125,k,2,0)
        t620 = rx(i,t125,k,0,2)
        t626 = 0.1E1 / (t604 * t605 * t607 - t604 * t609 * t611 - t605 *
     # t618 * t620 - t607 * t613 * t614 + t609 * t613 * t618 + t611 * t6
     #14 * t620)
        t627 = t4 * t626
        t631 = t604 * t614 + t605 * t613 + t609 * t620
        t633 = (t161 - t574) * t67
        t635 = t256 / 0.2E1 + t633 / 0.2E1
        t592 = t627 * t631
        t637 = t592 * t635
        t639 = t68 / 0.2E1 + t565 / 0.2E1
        t641 = t121 * t639
        t644 = (t637 - t641) * t128 / 0.2E1
        t645 = rx(i,t130,k,0,0)
        t646 = rx(i,t130,k,1,1)
        t648 = rx(i,t130,k,2,2)
        t650 = rx(i,t130,k,1,2)
        t652 = rx(i,t130,k,2,1)
        t654 = rx(i,t130,k,0,1)
        t655 = rx(i,t130,k,1,0)
        t659 = rx(i,t130,k,2,0)
        t661 = rx(i,t130,k,0,2)
        t667 = 0.1E1 / (t645 * t646 * t648 - t645 * t650 * t652 - t646 *
     # t659 * t661 - t648 * t654 * t655 + t650 * t654 * t659 + t652 * t6
     #55 * t661)
        t668 = t4 * t667
        t672 = t645 * t655 + t646 * t654 + t650 * t661
        t674 = (t164 - t577) * t67
        t676 = t299 / 0.2E1 + t674 / 0.2E1
        t632 = t668 * t672
        t678 = t632 * t676
        t681 = (t641 - t678) * t128 / 0.2E1
        t682 = t614 ** 2
        t683 = t605 ** 2
        t684 = t609 ** 2
        t685 = t682 + t683 + t684
        t686 = t626 * t685
        t687 = t44 ** 2
        t688 = t35 ** 2
        t689 = t39 ** 2
        t690 = t687 + t688 + t689
        t691 = t56 * t690
        t694 = t4 * (t686 / 0.2E1 + t691 / 0.2E1)
        t695 = t694 * t163
        t696 = t655 ** 2
        t697 = t646 ** 2
        t698 = t650 ** 2
        t699 = t696 + t697 + t698
        t700 = t667 * t699
        t703 = t4 * (t691 / 0.2E1 + t700 / 0.2E1)
        t704 = t703 * t166
        t710 = t605 * t611 + t607 * t609 + t614 * t618
        t711 = u(i,t125,t178,n)
        t713 = (t711 - t161) * t181
        t714 = u(i,t125,t183,n)
        t716 = (t161 - t714) * t181
        t718 = t713 / 0.2E1 + t716 / 0.2E1
        t658 = t627 * t710
        t720 = t658 * t718
        t724 = t35 * t41 + t37 * t39 + t44 * t48
        t664 = t156 * t724
        t726 = t664 * t219
        t729 = (t720 - t726) * t128 / 0.2E1
        t733 = t646 * t652 + t648 * t650 + t655 * t659
        t734 = u(i,t130,t178,n)
        t736 = (t734 - t164) * t181
        t737 = u(i,t130,t183,n)
        t739 = (t164 - t737) * t181
        t741 = t736 / 0.2E1 + t739 / 0.2E1
        t679 = t668 * t733
        t743 = t679 * t741
        t746 = (t726 - t743) * t128 / 0.2E1
        t747 = rx(i,j,t178,0,0)
        t748 = rx(i,j,t178,1,1)
        t750 = rx(i,j,t178,2,2)
        t752 = rx(i,j,t178,1,2)
        t754 = rx(i,j,t178,2,1)
        t756 = rx(i,j,t178,0,1)
        t757 = rx(i,j,t178,1,0)
        t761 = rx(i,j,t178,2,0)
        t763 = rx(i,j,t178,0,2)
        t769 = 0.1E1 / (t747 * t748 * t750 - t747 * t752 * t754 - t748 *
     # t761 * t763 - t750 * t756 * t757 + t752 * t756 * t761 + t754 * t7
     #57 * t763)
        t770 = t4 * t769
        t774 = t747 * t761 + t750 * t763 + t754 * t756
        t776 = (t212 - t591) * t67
        t778 = t403 / 0.2E1 + t776 / 0.2E1
        t730 = t770 * t774
        t780 = t730 * t778
        t782 = t147 * t639
        t785 = (t780 - t782) * t181 / 0.2E1
        t786 = rx(i,j,t183,0,0)
        t787 = rx(i,j,t183,1,1)
        t789 = rx(i,j,t183,2,2)
        t791 = rx(i,j,t183,1,2)
        t793 = rx(i,j,t183,2,1)
        t795 = rx(i,j,t183,0,1)
        t796 = rx(i,j,t183,1,0)
        t800 = rx(i,j,t183,2,0)
        t802 = rx(i,j,t183,0,2)
        t808 = 0.1E1 / (t786 * t787 * t789 - t786 * t791 * t793 - t787 *
     # t800 * t802 - t789 * t795 * t796 + t791 * t795 * t800 + t793 * t7
     #96 * t802)
        t809 = t4 * t808
        t813 = t786 * t800 + t789 * t802 + t793 * t795
        t815 = (t215 - t594) * t67
        t817 = t444 / 0.2E1 + t815 / 0.2E1
        t768 = t809 * t813
        t819 = t768 * t817
        t822 = (t782 - t819) * t181 / 0.2E1
        t826 = t748 * t754 + t750 * t752 + t757 * t761
        t828 = (t711 - t212) * t128
        t830 = (t212 - t734) * t128
        t832 = t828 / 0.2E1 + t830 / 0.2E1
        t784 = t770 * t826
        t834 = t784 * t832
        t836 = t664 * t168
        t839 = (t834 - t836) * t181 / 0.2E1
        t843 = t787 * t793 + t789 * t791 + t796 * t800
        t845 = (t714 - t215) * t128
        t847 = (t215 - t737) * t128
        t849 = t845 / 0.2E1 + t847 / 0.2E1
        t803 = t809 * t843
        t851 = t803 * t849
        t854 = (t836 - t851) * t181 / 0.2E1
        t855 = t761 ** 2
        t856 = t754 ** 2
        t857 = t750 ** 2
        t858 = t855 + t856 + t857
        t859 = t769 * t858
        t860 = t48 ** 2
        t861 = t41 ** 2
        t862 = t37 ** 2
        t863 = t860 + t861 + t862
        t864 = t56 * t863
        t867 = t4 * (t859 / 0.2E1 + t864 / 0.2E1)
        t868 = t867 * t214
        t869 = t800 ** 2
        t870 = t793 ** 2
        t871 = t789 ** 2
        t872 = t869 + t870 + t871
        t873 = t808 * t872
        t876 = t4 * (t864 / 0.2E1 + t873 / 0.2E1)
        t877 = t876 * t217
        t880 = (t69 - t566) * t67 + t173 + t586 + t224 + t603 + t644 + t
     #681 + (t695 - t704) * t128 + t729 + t746 + t785 + t822 + t839 + t8
     #54 + (t868 - t877) * t181
        t883 = t81 * t521 * t880 / 0.4E1
        t885 = sqrt(t558)
        t886 = ut(t531,j,k,n)
        t888 = cc * t554 * t885 * t886
        t890 = (t523 - t888) * t67
        t894 = dx * (t525 / 0.2E1 + t890 / 0.2E1) / 0.4E1
        t838 = (t70 - t2) * t67
        t895 = t69 + t64 * dt * t838 / 0.2E1 + t79 / 0.2E1 + t81 * t77 *
     # t509 / 0.4E1 - dx * ((cc * t105 * t514 * ut(t82,j,k,n) - t79) * t
     #67 / 0.2E1 + t525 / 0.2E1) / 0.4E1 - t530 - t883 - t894
        t896 = dt ** 2
        t899 = t56 * t160
        t902 = t4 * (t28 * t142 / 0.2E1 + t899 / 0.2E1)
        t906 = ut(t5,t125,k,n)
        t909 = ut(t5,t130,k,n)
        t912 = ut(i,t125,k,n)
        t914 = (t912 - t2) * t128
        t915 = ut(i,t130,k,n)
        t917 = (t2 - t915) * t128
        t923 = t902 * (t145 / 0.4E1 + t148 / 0.4E1 + t163 / 0.4E1 + t166
     # / 0.4E1) + t902 * dt * ((t906 - t70) * t128 / 0.4E1 + (t70 - t909
     #) * t128 / 0.4E1 + t914 / 0.4E1 + t917 / 0.4E1) / 0.2E1
        t926 = t56 * t211
        t929 = t4 * (t28 * t194 / 0.2E1 + t926 / 0.2E1)
        t933 = ut(t5,j,t178,n)
        t936 = ut(t5,j,t183,n)
        t939 = ut(i,j,t178,n)
        t941 = (t939 - t2) * t181
        t942 = ut(i,j,t183,n)
        t944 = (t2 - t942) * t181
        t950 = t929 * (t197 / 0.4E1 + t200 / 0.4E1 + t214 / 0.4E1 + t217
     # / 0.4E1) + t929 * dt * ((t933 - t70) * t181 / 0.4E1 + (t70 - t936
     #) * t181 / 0.4E1 + t941 / 0.4E1 + t944 / 0.4E1) / 0.2E1
        t958 = i - 2
        t959 = rx(t958,j,k,0,0)
        t960 = rx(t958,j,k,1,1)
        t962 = rx(t958,j,k,2,2)
        t964 = rx(t958,j,k,1,2)
        t966 = rx(t958,j,k,2,1)
        t968 = rx(t958,j,k,0,1)
        t969 = rx(t958,j,k,1,0)
        t973 = rx(t958,j,k,2,0)
        t975 = rx(t958,j,k,0,2)
        t981 = 0.1E1 / (t959 * t960 * t962 - t959 * t964 * t966 - t960 *
     # t973 * t975 - t962 * t968 * t969 + t964 * t968 * t973 + t966 * t9
     #69 * t975)
        t982 = t959 ** 2
        t983 = t968 ** 2
        t984 = t975 ** 2
        t985 = t982 + t983 + t984
        t990 = u(t958,j,k,n)
        t992 = (t563 - t990) * t67
        t996 = t4 * t981
        t1001 = u(t958,t125,k,n)
        t1004 = u(t958,t130,k,n)
        t1018 = u(t958,j,t178,n)
        t1021 = u(t958,j,t183,n)
        t1031 = rx(t531,t125,k,0,0)
        t1032 = rx(t531,t125,k,1,1)
        t1034 = rx(t531,t125,k,2,2)
        t1036 = rx(t531,t125,k,1,2)
        t1038 = rx(t531,t125,k,2,1)
        t1040 = rx(t531,t125,k,0,1)
        t1041 = rx(t531,t125,k,1,0)
        t1045 = rx(t531,t125,k,2,0)
        t1047 = rx(t531,t125,k,0,2)
        t1053 = 0.1E1 / (t1031 * t1032 * t1034 - t1031 * t1036 * t1038 -
     # t1032 * t1045 * t1047 - t1034 * t1040 * t1041 + t1036 * t1040 * t
     #1045 + t1038 * t1041 * t1047)
        t1054 = t4 * t1053
        t1066 = t565 / 0.2E1 + t992 / 0.2E1
        t1068 = t534 * t1066
        t1072 = rx(t531,t130,k,0,0)
        t1073 = rx(t531,t130,k,1,1)
        t1075 = rx(t531,t130,k,2,2)
        t1077 = rx(t531,t130,k,1,2)
        t1079 = rx(t531,t130,k,2,1)
        t1081 = rx(t531,t130,k,0,1)
        t1082 = rx(t531,t130,k,1,0)
        t1086 = rx(t531,t130,k,2,0)
        t1088 = rx(t531,t130,k,0,2)
        t1094 = 0.1E1 / (t1072 * t1073 * t1075 - t1072 * t1077 * t1079 -
     # t1073 * t1086 * t1088 - t1075 * t1081 * t1082 + t1077 * t1081 * t
     #1086 + t1079 * t1082 * t1088)
        t1095 = t4 * t1094
        t1109 = t1041 ** 2
        t1110 = t1032 ** 2
        t1111 = t1036 ** 2
        t1114 = t542 ** 2
        t1115 = t533 ** 2
        t1116 = t537 ** 2
        t1118 = t554 * (t1114 + t1115 + t1116)
        t1123 = t1082 ** 2
        t1124 = t1073 ** 2
        t1125 = t1077 ** 2
        t1138 = u(t531,t125,t178,n)
        t1141 = u(t531,t125,t183,n)
        t1145 = (t1138 - t574) * t181 / 0.2E1 + (t574 - t1141) * t181 / 
     #0.2E1
        t1020 = t569 * (t533 * t539 + t535 * t537 + t542 * t546)
        t1153 = t1020 * t598
        t1161 = u(t531,t130,t178,n)
        t1164 = u(t531,t130,t183,n)
        t1168 = (t1161 - t577) * t181 / 0.2E1 + (t577 - t1164) * t181 / 
     #0.2E1
        t1174 = rx(t531,j,t178,0,0)
        t1175 = rx(t531,j,t178,1,1)
        t1177 = rx(t531,j,t178,2,2)
        t1179 = rx(t531,j,t178,1,2)
        t1181 = rx(t531,j,t178,2,1)
        t1183 = rx(t531,j,t178,0,1)
        t1184 = rx(t531,j,t178,1,0)
        t1188 = rx(t531,j,t178,2,0)
        t1190 = rx(t531,j,t178,0,2)
        t1196 = 0.1E1 / (t1174 * t1175 * t1177 - t1174 * t1179 * t1181 -
     # t1175 * t1188 * t1190 - t1177 * t1183 * t1184 + t1179 * t1183 * t
     #1188 + t1181 * t1184 * t1190)
        t1197 = t4 * t1196
        t1209 = t550 * t1066
        t1213 = rx(t531,j,t183,0,0)
        t1214 = rx(t531,j,t183,1,1)
        t1216 = rx(t531,j,t183,2,2)
        t1218 = rx(t531,j,t183,1,2)
        t1220 = rx(t531,j,t183,2,1)
        t1222 = rx(t531,j,t183,0,1)
        t1223 = rx(t531,j,t183,1,0)
        t1227 = rx(t531,j,t183,2,0)
        t1229 = rx(t531,j,t183,0,2)
        t1235 = 0.1E1 / (t1213 * t1214 * t1216 - t1213 * t1218 * t1220 -
     # t1214 * t1227 * t1229 - t1216 * t1222 * t1223 + t1218 * t1222 * t
     #1227 + t1220 * t1223 * t1229)
        t1236 = t4 * t1235
        t1259 = (t1138 - t591) * t128 / 0.2E1 + (t591 - t1161) * t128 / 
     #0.2E1
        t1263 = t1020 * t581
        t1276 = (t1141 - t594) * t128 / 0.2E1 + (t594 - t1164) * t128 / 
     #0.2E1
        t1282 = t1188 ** 2
        t1283 = t1181 ** 2
        t1284 = t1177 ** 2
        t1287 = t546 ** 2
        t1288 = t539 ** 2
        t1289 = t535 ** 2
        t1291 = t554 * (t1287 + t1288 + t1289)
        t1296 = t1227 ** 2
        t1297 = t1220 ** 2
        t1298 = t1216 ** 2
        t1146 = t1054 * (t1031 * t1041 + t1032 * t1040 + t1036 * t1047)
        t1157 = t1095 * (t1072 * t1082 + t1073 * t1081 + t1077 * t1088)
        t1217 = t1197 * (t1174 * t1188 + t1177 * t1190 + t1181 * t1183)
        t1232 = t1236 * (t1213 * t1227 + t1216 * t1229 + t1220 * t1222)
        t1307 = (t566 - t4 * (t981 * t985 / 0.2E1 + t559 / 0.2E1) * t992
     #) * t67 + t586 + (t583 - t996 * (t959 * t969 + t960 * t968 + t964 
     #* t975) * ((t1001 - t990) * t128 / 0.2E1 + (t990 - t1004) * t128 /
     # 0.2E1)) * t67 / 0.2E1 + t603 + (t600 - t996 * (t959 * t973 + t962
     # * t975 + t966 * t968) * ((t1018 - t990) * t181 / 0.2E1 + (t990 - 
     #t1021) * t181 / 0.2E1)) * t67 / 0.2E1 + (t1146 * (t633 / 0.2E1 + (
     #t574 - t1001) * t67 / 0.2E1) - t1068) * t128 / 0.2E1 + (t1068 - t1
     #157 * (t674 / 0.2E1 + (t577 - t1004) * t67 / 0.2E1)) * t128 / 0.2E
     #1 + (t4 * (t1053 * (t1109 + t1110 + t1111) / 0.2E1 + t1118 / 0.2E1
     #) * t576 - t4 * (t1118 / 0.2E1 + t1094 * (t1123 + t1124 + t1125) /
     # 0.2E1) * t579) * t128 + (t1054 * (t1032 * t1038 + t1034 * t1036 +
     # t1041 * t1045) * t1145 - t1153) * t128 / 0.2E1 + (t1153 - t1095 *
     # (t1073 * t1079 + t1075 * t1077 + t1082 * t1086) * t1168) * t128 /
     # 0.2E1 + (t1217 * (t776 / 0.2E1 + (t591 - t1018) * t67 / 0.2E1) - 
     #t1209) * t181 / 0.2E1 + (t1209 - t1232 * (t815 / 0.2E1 + (t594 - t
     #1021) * t67 / 0.2E1)) * t181 / 0.2E1 + (t1197 * (t1175 * t1181 + t
     #1177 * t1179 + t1184 * t1188) * t1259 - t1263) * t181 / 0.2E1 + (t
     #1263 - t1236 * (t1214 * t1220 + t1216 * t1218 + t1223 * t1227) * t
     #1276) * t181 / 0.2E1 + (t4 * (t1196 * (t1282 + t1283 + t1284) / 0.
     #2E1 + t1291 / 0.2E1) * t593 - t4 * (t1291 / 0.2E1 + t1235 * (t1296
     # + t1297 + t1298) / 0.2E1) * t596) * t181
        t1312 = sqrt(t985)
        t1280 = (t2 - t886) * t67
        t1322 = t566 + t562 * dt * t1280 / 0.2E1 + t530 + t883 - t894 - 
     #t888 / 0.2E1 - t81 * t885 * t1307 / 0.4E1 - dx * (t890 / 0.2E1 + (
     #-cc * t1312 * t981 * ut(t958,j,k,n) + t888) * t67 / 0.2E1) / 0.4E1
        t1327 = t4 * (t554 * t573 / 0.2E1 + t899 / 0.2E1)
        t1331 = ut(t531,t125,k,n)
        t1334 = ut(t531,t130,k,n)
        t1342 = t1327 * (t163 / 0.4E1 + t166 / 0.4E1 + t576 / 0.4E1 + t5
     #79 / 0.4E1) + t1327 * dt * (t914 / 0.4E1 + t917 / 0.4E1 + (t1331 -
     # t886) * t128 / 0.4E1 + (t886 - t1334) * t128 / 0.4E1) / 0.2E1
        t1347 = t4 * (t554 * t590 / 0.2E1 + t926 / 0.2E1)
        t1351 = ut(t531,j,t178,n)
        t1354 = ut(t531,j,t183,n)
        t1362 = t1347 * (t214 / 0.4E1 + t217 / 0.4E1 + t593 / 0.4E1 + t5
     #96 / 0.4E1) + t1347 * dt * (t941 / 0.4E1 + t944 / 0.4E1 + (t1351 -
     # t886) * t181 / 0.4E1 + (t886 - t1354) * t181 / 0.4E1) / 0.2E1
        t1371 = t4 * (t626 * t631 / 0.2E1 + t899 / 0.2E1)
        t1379 = t838
        t1380 = t1280
        t1386 = t1371 * (t256 / 0.4E1 + t633 / 0.4E1 + t68 / 0.4E1 + t56
     #5 / 0.4E1) + t1371 * dt * ((t906 - t912) * t67 / 0.4E1 + (t912 - t
     #1331) * t67 / 0.4E1 + t1379 / 0.4E1 + t1380 / 0.4E1) / 0.2E1
        t1393 = sqrt(t685)
        t1395 = cc * t626 * t1393 * t912
        t1397 = t225 ** 2
        t1398 = t234 ** 2
        t1399 = t241 ** 2
        t1402 = t604 ** 2
        t1403 = t613 ** 2
        t1404 = t620 ** 2
        t1406 = t626 * (t1402 + t1403 + t1404)
        t1411 = t1031 ** 2
        t1412 = t1040 ** 2
        t1413 = t1047 ** 2
        t1422 = j + 2
        t1423 = u(t5,t1422,k,n)
        t1430 = u(i,t1422,k,n)
        t1432 = (t1430 - t161) * t128
        t1434 = t1432 / 0.2E1 + t163 / 0.2E1
        t1436 = t592 * t1434
        t1440 = u(t531,t1422,k,n)
        t1372 = t627 * (t604 * t618 + t607 * t620 + t611 * t613)
        t1461 = t1372 * t718
        t1474 = rx(i,t1422,k,0,0)
        t1475 = rx(i,t1422,k,1,1)
        t1477 = rx(i,t1422,k,2,2)
        t1479 = rx(i,t1422,k,1,2)
        t1481 = rx(i,t1422,k,2,1)
        t1483 = rx(i,t1422,k,0,1)
        t1484 = rx(i,t1422,k,1,0)
        t1488 = rx(i,t1422,k,2,0)
        t1490 = rx(i,t1422,k,0,2)
        t1496 = 0.1E1 / (t1474 * t1475 * t1477 - t1474 * t1479 * t1481 -
     # t1475 * t1488 * t1490 - t1477 * t1483 * t1484 + t1479 * t1483 * t
     #1488 + t1481 * t1484 * t1490)
        t1497 = t4 * t1496
        t1513 = t1484 ** 2
        t1514 = t1475 ** 2
        t1515 = t1479 ** 2
        t1516 = t1513 + t1514 + t1515
        t1528 = u(i,t1422,t178,n)
        t1531 = u(i,t1422,t183,n)
        t1541 = rx(i,t125,t178,0,0)
        t1542 = rx(i,t125,t178,1,1)
        t1544 = rx(i,t125,t178,2,2)
        t1546 = rx(i,t125,t178,1,2)
        t1548 = rx(i,t125,t178,2,1)
        t1550 = rx(i,t125,t178,0,1)
        t1551 = rx(i,t125,t178,1,0)
        t1555 = rx(i,t125,t178,2,0)
        t1557 = rx(i,t125,t178,0,2)
        t1563 = 0.1E1 / (t1541 * t1542 * t1544 - t1541 * t1546 * t1548 -
     # t1542 * t1555 * t1557 - t1544 * t1550 * t1551 + t1546 * t1550 * t
     #1555 + t1548 * t1551 * t1557)
        t1564 = t4 * t1563
        t1574 = (t336 - t711) * t67 / 0.2E1 + (t711 - t1138) * t67 / 0.2
     #E1
        t1578 = t1372 * t635
        t1582 = rx(i,t125,t183,0,0)
        t1583 = rx(i,t125,t183,1,1)
        t1585 = rx(i,t125,t183,2,2)
        t1587 = rx(i,t125,t183,1,2)
        t1589 = rx(i,t125,t183,2,1)
        t1591 = rx(i,t125,t183,0,1)
        t1592 = rx(i,t125,t183,1,0)
        t1596 = rx(i,t125,t183,2,0)
        t1598 = rx(i,t125,t183,0,2)
        t1604 = 0.1E1 / (t1582 * t1583 * t1585 - t1582 * t1587 * t1589 -
     # t1583 * t1596 * t1598 - t1585 * t1591 * t1592 + t1587 * t1591 * t
     #1596 + t1589 * t1592 * t1598)
        t1605 = t4 * t1604
        t1615 = (t339 - t714) * t67 / 0.2E1 + (t714 - t1141) * t67 / 0.2
     #E1
        t1632 = t658 * t1434
        t1649 = t1555 ** 2
        t1650 = t1548 ** 2
        t1651 = t1544 ** 2
        t1654 = t618 ** 2
        t1655 = t611 ** 2
        t1656 = t607 ** 2
        t1658 = t626 * (t1654 + t1655 + t1656)
        t1663 = t1596 ** 2
        t1664 = t1589 ** 2
        t1665 = t1585 ** 2
        t1593 = t1564 * (t1542 * t1548 + t1544 * t1546 + t1551 * t1555)
        t1606 = t1605 * (t1583 * t1589 + t1585 * t1587 + t1592 * t1596)
        t1674 = (t4 * (t247 * (t1397 + t1398 + t1399) / 0.2E1 + t1406 / 
     #0.2E1) * t256 - t4 * (t1406 / 0.2E1 + t1053 * (t1411 + t1412 + t14
     #13) / 0.2E1) * t633) * t67 + (t357 * ((t1423 - t143) * t128 / 0.2E
     #1 + t145 / 0.2E1) - t1436) * t67 / 0.2E1 + (t1436 - t1146 * ((t144
     #0 - t574) * t128 / 0.2E1 + t576 / 0.2E1)) * t67 / 0.2E1 + (t248 * 
     #(t225 * t239 + t228 * t241 + t232 * t234) * t343 - t1461) * t67 / 
     #0.2E1 + (t1461 - t1054 * (t1031 * t1045 + t1034 * t1047 + t1038 * 
     #t1040) * t1145) * t67 / 0.2E1 + (t1497 * (t1474 * t1484 + t1475 * 
     #t1483 + t1479 * t1490) * ((t1423 - t1430) * t67 / 0.2E1 + (t1430 -
     # t1440) * t67 / 0.2E1) - t637) * t128 / 0.2E1 + t644 + (t4 * (t149
     #6 * t1516 / 0.2E1 + t686 / 0.2E1) * t1432 - t695) * t128 + (t1497 
     #* (t1475 * t1481 + t1477 * t1479 + t1484 * t1488) * ((t1528 - t143
     #0) * t181 / 0.2E1 + (t1430 - t1531) * t181 / 0.2E1) - t720) * t128
     # / 0.2E1 + t729 + (t1564 * (t1541 * t1555 + t1544 * t1557 + t1548 
     #* t1550) * t1574 - t1578) * t181 / 0.2E1 + (t1578 - t1605 * (t1582
     # * t1596 + t1585 * t1598 + t1589 * t1591) * t1615) * t181 / 0.2E1 
     #+ (t1593 * ((t1528 - t711) * t128 / 0.2E1 + t828 / 0.2E1) - t1632)
     # * t181 / 0.2E1 + (t1632 - t1606 * ((t1531 - t714) * t128 / 0.2E1 
     #+ t845 / 0.2E1)) * t181 / 0.2E1 + (t4 * (t1563 * (t1649 + t1650 + 
     #t1651) / 0.2E1 + t1658 / 0.2E1) * t713 - t4 * (t1658 / 0.2E1 + t16
     #04 * (t1663 + t1664 + t1665) / 0.2E1) * t716) * t181
        t1679 = sqrt(t1516)
        t1685 = sqrt(t690)
        t1687 = t520 * t1685 * t2
        t1689 = (-t1687 + t1395) * t128
        t1694 = t1687 / 0.2E1
        t1697 = t81 * t1685 * t880 / 0.4E1
        t1699 = sqrt(t699)
        t1701 = cc * t667 * t1699 * t915
        t1703 = (t1687 - t1701) * t128
        t1707 = dy * (t1689 / 0.2E1 + t1703 / 0.2E1) / 0.4E1
        t1708 = t695 + t694 * dt * t914 / 0.2E1 + t1395 / 0.2E1 + t81 * 
     #t1393 * t1674 / 0.4E1 - dy * ((cc * t1496 * t1679 * ut(i,t1422,k,n
     #) - t1395) * t128 / 0.2E1 + t1689 / 0.2E1) / 0.4E1 - t1694 - t1697
     # - t1707
        t1711 = t56 * t724
        t1714 = t4 * (t626 * t710 / 0.2E1 + t1711 / 0.2E1)
        t1718 = ut(i,t125,t178,n)
        t1721 = ut(i,t125,t183,n)
        t1729 = t1714 * (t713 / 0.4E1 + t716 / 0.4E1 + t214 / 0.4E1 + t2
     #17 / 0.4E1) + t1714 * dt * ((t1718 - t912) * t181 / 0.4E1 + (t912 
     #- t1721) * t181 / 0.4E1 + t941 / 0.4E1 + t944 / 0.4E1) / 0.2E1
        t1734 = t4 * (t667 * t672 / 0.2E1 + t899 / 0.2E1)
        t1747 = t1734 * (t68 / 0.4E1 + t565 / 0.4E1 + t299 / 0.4E1 + t67
     #4 / 0.4E1) + t1734 * dt * (t1379 / 0.4E1 + t1380 / 0.4E1 + (t909 -
     # t915) * t67 / 0.4E1 + (t915 - t1334) * t67 / 0.4E1) / 0.2E1
        t1754 = t268 ** 2
        t1755 = t277 ** 2
        t1756 = t284 ** 2
        t1759 = t645 ** 2
        t1760 = t654 ** 2
        t1761 = t661 ** 2
        t1763 = t667 * (t1759 + t1760 + t1761)
        t1768 = t1072 ** 2
        t1769 = t1081 ** 2
        t1770 = t1088 ** 2
        t1779 = j - 2
        t1780 = u(t5,t1779,k,n)
        t1787 = u(i,t1779,k,n)
        t1789 = (t164 - t1787) * t128
        t1791 = t166 / 0.2E1 + t1789 / 0.2E1
        t1793 = t632 * t1791
        t1797 = u(t531,t1779,k,n)
        t1722 = t668 * (t645 * t659 + t648 * t661 + t652 * t654)
        t1818 = t1722 * t741
        t1831 = rx(i,t1779,k,0,0)
        t1832 = rx(i,t1779,k,1,1)
        t1834 = rx(i,t1779,k,2,2)
        t1836 = rx(i,t1779,k,1,2)
        t1838 = rx(i,t1779,k,2,1)
        t1840 = rx(i,t1779,k,0,1)
        t1841 = rx(i,t1779,k,1,0)
        t1845 = rx(i,t1779,k,2,0)
        t1847 = rx(i,t1779,k,0,2)
        t1853 = 0.1E1 / (t1831 * t1832 * t1834 - t1831 * t1836 * t1838 -
     # t1832 * t1845 * t1847 - t1834 * t1840 * t1841 + t1836 * t1840 * t
     #1845 + t1838 * t1841 * t1847)
        t1854 = t4 * t1853
        t1870 = t1841 ** 2
        t1871 = t1832 ** 2
        t1872 = t1836 ** 2
        t1873 = t1870 + t1871 + t1872
        t1885 = u(i,t1779,t178,n)
        t1888 = u(i,t1779,t183,n)
        t1898 = rx(i,t130,t178,0,0)
        t1899 = rx(i,t130,t178,1,1)
        t1901 = rx(i,t130,t178,2,2)
        t1903 = rx(i,t130,t178,1,2)
        t1905 = rx(i,t130,t178,2,1)
        t1907 = rx(i,t130,t178,0,1)
        t1908 = rx(i,t130,t178,1,0)
        t1912 = rx(i,t130,t178,2,0)
        t1914 = rx(i,t130,t178,0,2)
        t1920 = 0.1E1 / (t1898 * t1899 * t1901 - t1898 * t1903 * t1905 -
     # t1899 * t1912 * t1914 - t1901 * t1907 * t1908 + t1903 * t1907 * t
     #1912 + t1905 * t1908 * t1914)
        t1921 = t4 * t1920
        t1931 = (t359 - t734) * t67 / 0.2E1 + (t734 - t1161) * t67 / 0.2
     #E1
        t1935 = t1722 * t676
        t1939 = rx(i,t130,t183,0,0)
        t1940 = rx(i,t130,t183,1,1)
        t1942 = rx(i,t130,t183,2,2)
        t1944 = rx(i,t130,t183,1,2)
        t1946 = rx(i,t130,t183,2,1)
        t1948 = rx(i,t130,t183,0,1)
        t1949 = rx(i,t130,t183,1,0)
        t1953 = rx(i,t130,t183,2,0)
        t1955 = rx(i,t130,t183,0,2)
        t1961 = 0.1E1 / (t1939 * t1940 * t1942 - t1939 * t1944 * t1946 -
     # t1940 * t1953 * t1955 - t1942 * t1948 * t1949 + t1944 * t1948 * t
     #1953 + t1946 * t1949 * t1955)
        t1962 = t4 * t1961
        t1972 = (t362 - t737) * t67 / 0.2E1 + (t737 - t1164) * t67 / 0.2
     #E1
        t1989 = t679 * t1791
        t2006 = t1912 ** 2
        t2007 = t1905 ** 2
        t2008 = t1901 ** 2
        t2011 = t659 ** 2
        t2012 = t652 ** 2
        t2013 = t648 ** 2
        t2015 = t667 * (t2011 + t2012 + t2013)
        t2020 = t1953 ** 2
        t2021 = t1946 ** 2
        t2022 = t1942 ** 2
        t1933 = t1921 * (t1899 * t1905 + t1901 * t1903 + t1908 * t1912)
        t1950 = t1962 * (t1940 * t1946 + t1942 * t1944 + t1949 * t1953)
        t2031 = (t4 * (t290 * (t1754 + t1755 + t1756) / 0.2E1 + t1763 / 
     #0.2E1) * t299 - t4 * (t1763 / 0.2E1 + t1094 * (t1768 + t1769 + t17
     #70) / 0.2E1) * t674) * t67 + (t369 * (t148 / 0.2E1 + (t146 - t1780
     #) * t128 / 0.2E1) - t1793) * t67 / 0.2E1 + (t1793 - t1157 * (t579 
     #/ 0.2E1 + (t577 - t1797) * t128 / 0.2E1)) * t67 / 0.2E1 + (t291 * 
     #(t268 * t282 + t271 * t284 + t275 * t277) * t366 - t1818) * t67 / 
     #0.2E1 + (t1818 - t1095 * (t1072 * t1086 + t1075 * t1088 + t1079 * 
     #t1081) * t1168) * t67 / 0.2E1 + t681 + (t678 - t1854 * (t1831 * t1
     #841 + t1832 * t1840 + t1836 * t1847) * ((t1780 - t1787) * t67 / 0.
     #2E1 + (t1787 - t1797) * t67 / 0.2E1)) * t128 / 0.2E1 + (t704 - t4 
     #* (t1853 * t1873 / 0.2E1 + t700 / 0.2E1) * t1789) * t128 + t746 + 
     #(t743 - t1854 * (t1832 * t1838 + t1834 * t1836 + t1841 * t1845) * 
     #((t1885 - t1787) * t181 / 0.2E1 + (t1787 - t1888) * t181 / 0.2E1))
     # * t128 / 0.2E1 + (t1921 * (t1898 * t1912 + t1901 * t1914 + t1905 
     #* t1907) * t1931 - t1935) * t181 / 0.2E1 + (t1935 - t1962 * (t1939
     # * t1953 + t1942 * t1955 + t1946 * t1948) * t1972) * t181 / 0.2E1 
     #+ (t1933 * (t830 / 0.2E1 + (t734 - t1885) * t128 / 0.2E1) - t1989)
     # * t181 / 0.2E1 + (t1989 - t1950 * (t847 / 0.2E1 + (t737 - t1888) 
     #* t128 / 0.2E1)) * t181 / 0.2E1 + (t4 * (t1920 * (t2006 + t2007 + 
     #t2008) / 0.2E1 + t2015 / 0.2E1) * t736 - t4 * (t2015 / 0.2E1 + t19
     #61 * (t2020 + t2021 + t2022) / 0.2E1) * t739) * t181
        t2036 = sqrt(t1873)
        t2046 = t704 + t703 * dt * t917 / 0.2E1 + t1694 + t1697 - t1707 
     #- t1701 / 0.2E1 - t81 * t1699 * t2031 / 0.4E1 - dy * (t1703 / 0.2E
     #1 + (-cc * t1853 * t2036 * ut(i,t1779,k,n) + t1701) * t128 / 0.2E1
     #) / 0.4E1
        t2051 = t4 * (t667 * t733 / 0.2E1 + t1711 / 0.2E1)
        t2055 = ut(i,t130,t178,n)
        t2058 = ut(i,t130,t183,n)
        t2066 = t2051 * (t214 / 0.4E1 + t217 / 0.4E1 + t736 / 0.4E1 + t7
     #39 / 0.4E1) + t2051 * dt * (t941 / 0.4E1 + t944 / 0.4E1 + (t2055 -
     # t915) * t181 / 0.4E1 + (t915 - t2058) * t181 / 0.4E1) / 0.2E1
        t2075 = t4 * (t769 * t774 / 0.2E1 + t926 / 0.2E1)
        t2088 = t2075 * (t403 / 0.4E1 + t776 / 0.4E1 + t68 / 0.4E1 + t56
     #5 / 0.4E1) + t2075 * dt * ((t933 - t939) * t67 / 0.4E1 + (t939 - t
     #1351) * t67 / 0.4E1 + t1379 / 0.4E1 + t1380 / 0.4E1) / 0.2E1
        t2093 = t4 * (t769 * t826 / 0.2E1 + t1711 / 0.2E1)
        t2106 = t2093 * (t828 / 0.4E1 + t830 / 0.4E1 + t163 / 0.4E1 + t1
     #66 / 0.4E1) + t2093 * dt * ((t1718 - t939) * t128 / 0.4E1 + (t939 
     #- t2055) * t128 / 0.4E1 + t914 / 0.4E1 + t917 / 0.4E1) / 0.2E1
        t2113 = sqrt(t858)
        t2115 = cc * t769 * t2113 * t939
        t2117 = t372 ** 2
        t2118 = t381 ** 2
        t2119 = t388 ** 2
        t2122 = t747 ** 2
        t2123 = t756 ** 2
        t2124 = t763 ** 2
        t2126 = t769 * (t2122 + t2123 + t2124)
        t2131 = t1174 ** 2
        t2132 = t1183 ** 2
        t2133 = t1190 ** 2
        t2065 = t770 * (t747 * t757 + t748 * t756 + t752 * t763)
        t2153 = t2065 * t832
        t2166 = k + 2
        t2167 = u(t5,j,t2166,n)
        t2174 = u(i,j,t2166,n)
        t2176 = (t2174 - t212) * t181
        t2178 = t2176 / 0.2E1 + t214 / 0.2E1
        t2180 = t730 * t2178
        t2184 = u(t531,j,t2166,n)
        t2201 = t2065 * t778
        t2214 = t1551 ** 2
        t2215 = t1542 ** 2
        t2216 = t1546 ** 2
        t2219 = t757 ** 2
        t2220 = t748 ** 2
        t2221 = t752 ** 2
        t2223 = t769 * (t2219 + t2220 + t2221)
        t2228 = t1908 ** 2
        t2229 = t1899 ** 2
        t2230 = t1903 ** 2
        t2239 = u(i,t125,t2166,n)
        t2247 = t784 * t2178
        t2251 = u(i,t130,t2166,n)
        t2261 = rx(i,j,t2166,0,0)
        t2262 = rx(i,j,t2166,1,1)
        t2264 = rx(i,j,t2166,2,2)
        t2266 = rx(i,j,t2166,1,2)
        t2268 = rx(i,j,t2166,2,1)
        t2270 = rx(i,j,t2166,0,1)
        t2271 = rx(i,j,t2166,1,0)
        t2275 = rx(i,j,t2166,2,0)
        t2277 = rx(i,j,t2166,0,2)
        t2283 = 0.1E1 / (t2261 * t2262 * t2264 - t2261 * t2266 * t2268 -
     # t2262 * t2275 * t2277 - t2264 * t2270 * t2271 + t2266 * t2270 * t
     #2275 + t2268 * t2271 * t2277)
        t2284 = t4 * t2283
        t2315 = t2275 ** 2
        t2316 = t2268 ** 2
        t2317 = t2264 ** 2
        t2318 = t2315 + t2316 + t2317
        t2326 = (t4 * (t394 * (t2117 + t2118 + t2119) / 0.2E1 + t2126 / 
     #0.2E1) * t403 - t4 * (t2126 / 0.2E1 + t1196 * (t2131 + t2132 + t21
     #33) / 0.2E1) * t776) * t67 + (t395 * (t372 * t382 + t373 * t381 + 
     #t377 * t388) * t461 - t2153) * t67 / 0.2E1 + (t2153 - t1197 * (t11
     #74 * t1184 + t1175 * t1183 + t1179 * t1190) * t1259) * t67 / 0.2E1
     # + (t433 * ((t2167 - t195) * t181 / 0.2E1 + t197 / 0.2E1) - t2180)
     # * t67 / 0.2E1 + (t2180 - t1217 * ((t2184 - t591) * t181 / 0.2E1 +
     # t593 / 0.2E1)) * t67 / 0.2E1 + (t1564 * (t1541 * t1551 + t1542 * 
     #t1550 + t1546 * t1557) * t1574 - t2201) * t128 / 0.2E1 + (t2201 - 
     #t1921 * (t1898 * t1908 + t1899 * t1907 + t1903 * t1914) * t1931) *
     # t128 / 0.2E1 + (t4 * (t1563 * (t2214 + t2215 + t2216) / 0.2E1 + t
     #2223 / 0.2E1) * t828 - t4 * (t2223 / 0.2E1 + t1920 * (t2228 + t222
     #9 + t2230) / 0.2E1) * t830) * t128 + (t1593 * ((t2239 - t711) * t1
     #81 / 0.2E1 + t713 / 0.2E1) - t2247) * t128 / 0.2E1 + (t2247 - t193
     #3 * ((t2251 - t734) * t181 / 0.2E1 + t736 / 0.2E1)) * t128 / 0.2E1
     # + (t2284 * (t2261 * t2275 + t2264 * t2277 + t2268 * t2270) * ((t2
     #167 - t2174) * t67 / 0.2E1 + (t2174 - t2184) * t67 / 0.2E1) - t780
     #) * t181 / 0.2E1 + t785 + (t2284 * (t2262 * t2268 + t2264 * t2266 
     #+ t2271 * t2275) * ((t2239 - t2174) * t128 / 0.2E1 + (t2174 - t225
     #1) * t128 / 0.2E1) - t834) * t181 / 0.2E1 + t839 + (t4 * (t2283 * 
     #t2318 / 0.2E1 + t859 / 0.2E1) * t2176 - t868) * t181
        t2331 = sqrt(t2318)
        t2337 = sqrt(t863)
        t2339 = t520 * t2337 * t2
        t2341 = (-t2339 + t2115) * t181
        t2346 = t2339 / 0.2E1
        t2349 = t81 * t2337 * t880 / 0.4E1
        t2351 = sqrt(t872)
        t2353 = cc * t808 * t2351 * t942
        t2355 = (t2339 - t2353) * t181
        t2359 = dz * (t2341 / 0.2E1 + t2355 / 0.2E1) / 0.4E1
        t2360 = t868 + t867 * dt * t941 / 0.2E1 + t2115 / 0.2E1 + t81 * 
     #t2113 * t2326 / 0.4E1 - dz * ((cc * t2283 * t2331 * ut(i,j,t2166,n
     #) - t2115) * t181 / 0.2E1 + t2341 / 0.2E1) / 0.4E1 - t2346 - t2349
     # - t2359
        t2365 = t4 * (t808 * t813 / 0.2E1 + t926 / 0.2E1)
        t2378 = t2365 * (t68 / 0.4E1 + t565 / 0.4E1 + t444 / 0.4E1 + t81
     #5 / 0.4E1) + t2365 * dt * (t1379 / 0.4E1 + t1380 / 0.4E1 + (t936 -
     # t942) * t67 / 0.4E1 + (t942 - t1354) * t67 / 0.4E1) / 0.2E1
        t2383 = t4 * (t808 * t843 / 0.2E1 + t1711 / 0.2E1)
        t2396 = t2383 * (t163 / 0.4E1 + t166 / 0.4E1 + t845 / 0.4E1 + t8
     #47 / 0.4E1) + t2383 * dt * (t914 / 0.4E1 + t917 / 0.4E1 + (t1721 -
     # t942) * t128 / 0.4E1 + (t942 - t2058) * t128 / 0.4E1) / 0.2E1
        t2403 = t413 ** 2
        t2404 = t422 ** 2
        t2405 = t429 ** 2
        t2408 = t786 ** 2
        t2409 = t795 ** 2
        t2410 = t802 ** 2
        t2412 = t808 * (t2408 + t2409 + t2410)
        t2417 = t1213 ** 2
        t2418 = t1222 ** 2
        t2419 = t1229 ** 2
        t2333 = t809 * (t786 * t796 + t787 * t795 + t791 * t802)
        t2439 = t2333 * t849
        t2452 = k - 2
        t2453 = u(t5,j,t2452,n)
        t2460 = u(i,j,t2452,n)
        t2462 = (t215 - t2460) * t181
        t2464 = t217 / 0.2E1 + t2462 / 0.2E1
        t2466 = t768 * t2464
        t2470 = u(t531,j,t2452,n)
        t2487 = t2333 * t817
        t2500 = t1592 ** 2
        t2501 = t1583 ** 2
        t2502 = t1587 ** 2
        t2505 = t796 ** 2
        t2506 = t787 ** 2
        t2507 = t791 ** 2
        t2509 = t808 * (t2505 + t2506 + t2507)
        t2514 = t1949 ** 2
        t2515 = t1940 ** 2
        t2516 = t1944 ** 2
        t2525 = u(i,t125,t2452,n)
        t2533 = t803 * t2464
        t2537 = u(i,t130,t2452,n)
        t2547 = rx(i,j,t2452,0,0)
        t2548 = rx(i,j,t2452,1,1)
        t2550 = rx(i,j,t2452,2,2)
        t2552 = rx(i,j,t2452,1,2)
        t2554 = rx(i,j,t2452,2,1)
        t2556 = rx(i,j,t2452,0,1)
        t2557 = rx(i,j,t2452,1,0)
        t2561 = rx(i,j,t2452,2,0)
        t2563 = rx(i,j,t2452,0,2)
        t2569 = 0.1E1 / (t2547 * t2548 * t2550 - t2547 * t2552 * t2554 -
     # t2548 * t2561 * t2563 - t2550 * t2556 * t2557 + t2552 * t2556 * t
     #2561 + t2554 * t2557 * t2563)
        t2570 = t4 * t2569
        t2601 = t2561 ** 2
        t2602 = t2554 ** 2
        t2603 = t2550 ** 2
        t2604 = t2601 + t2602 + t2603
        t2612 = (t4 * (t435 * (t2403 + t2404 + t2405) / 0.2E1 + t2412 / 
     #0.2E1) * t444 - t4 * (t2412 / 0.2E1 + t1235 * (t2417 + t2418 + t24
     #19) / 0.2E1) * t815) * t67 + (t436 * (t413 * t423 + t414 * t422 + 
     #t418 * t429) * t478 - t2439) * t67 / 0.2E1 + (t2439 - t1236 * (t12
     #13 * t1223 + t1214 * t1222 + t1218 * t1229) * t1276) * t67 / 0.2E1
     # + (t446 * (t200 / 0.2E1 + (t198 - t2453) * t181 / 0.2E1) - t2466)
     # * t67 / 0.2E1 + (t2466 - t1232 * (t596 / 0.2E1 + (t594 - t2470) *
     # t181 / 0.2E1)) * t67 / 0.2E1 + (t1605 * (t1582 * t1592 + t1583 * 
     #t1591 + t1587 * t1598) * t1615 - t2487) * t128 / 0.2E1 + (t2487 - 
     #t1962 * (t1939 * t1949 + t1940 * t1948 + t1944 * t1955) * t1972) *
     # t128 / 0.2E1 + (t4 * (t1604 * (t2500 + t2501 + t2502) / 0.2E1 + t
     #2509 / 0.2E1) * t845 - t4 * (t2509 / 0.2E1 + t1961 * (t2514 + t251
     #5 + t2516) / 0.2E1) * t847) * t128 + (t1606 * (t716 / 0.2E1 + (t71
     #4 - t2525) * t181 / 0.2E1) - t2533) * t128 / 0.2E1 + (t2533 - t195
     #0 * (t739 / 0.2E1 + (t737 - t2537) * t181 / 0.2E1)) * t128 / 0.2E1
     # + t822 + (t819 - t2570 * (t2547 * t2561 + t2550 * t2563 + t2554 *
     # t2556) * ((t2453 - t2460) * t67 / 0.2E1 + (t2460 - t2470) * t67 /
     # 0.2E1)) * t181 / 0.2E1 + t854 + (t851 - t2570 * (t2548 * t2554 + 
     #t2550 * t2552 + t2557 * t2561) * ((t2525 - t2460) * t128 / 0.2E1 +
     # (t2460 - t2537) * t128 / 0.2E1)) * t181 / 0.2E1 + (t877 - t4 * (t
     #2569 * t2604 / 0.2E1 + t873 / 0.2E1) * t2462) * t181
        t2617 = sqrt(t2604)
        t2627 = t877 + t876 * dt * t944 / 0.2E1 + t2346 + t2349 - t2359 
     #- t2353 / 0.2E1 - t81 * t2351 * t2612 / 0.4E1 - dz * (t2355 / 0.2E
     #1 + (-cc * t2569 * t2617 * ut(i,j,t2452,n) + t2353) * t181 / 0.2E1
     #) / 0.4E1
        unew(i,j,k) = t1 + dt * t2 + (-t1322 * t896 / 0.2E1 - t1342 *
     # t896 / 0.2E1 - t1362 * t896 / 0.2E1 + t895 * t896 / 0.2E1 + t923 
     #* t896 / 0.2E1 + t950 * t896 / 0.2E1) * t55 * t67 + (t1386 * t896 
     #/ 0.2E1 + t1708 * t896 / 0.2E1 + t1729 * t896 / 0.2E1 - t1747 * t8
     #96 / 0.2E1 - t2046 * t896 / 0.2E1 - t2066 * t896 / 0.2E1) * t55 * 
     #t128 + (t2088 * t896 / 0.2E1 + t2106 * t896 / 0.2E1 + t2360 * t896
     # / 0.2E1 - t2378 * t896 / 0.2E1 - t2396 * t896 / 0.2E1 - t2627 * t
     #896 / 0.2E1) * t55 * t181

        utnew(i,j,k) = t2 + (-dt * t1322 - dt * t1342 - dt
     # * t1362 + dt * t895 + dt * t923 + dt * t950) * t55 * t67 + (dt * 
     #t1386 + dt * t1708 + dt * t1729 - dt * t1747 - dt * t2046 - dt * t
     #2066) * t55 * t128 + (dt * t2088 + dt * t2106 + dt * t2360 - dt * 
     #t2378 - dt * t2396 - t2627 * dt) * t55 * t181

        return
      end
