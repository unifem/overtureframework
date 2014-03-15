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
        real t100
        real t1004
        real t1005
        real t1008
        real t101
        real t1018
        real t1019
        real t102
        real t1021
        real t1023
        real t1025
        real t1027
        real t1029
        real t103
        real t1031
        real t1034
        real t1040
        real t1041
        real t105
        real t1053
        real t1055
        real t1059
        real t1060
        real t1062
        real t1064
        real t1066
        real t1068
        real t1070
        real t1072
        real t1075
        real t1081
        real t1082
        real t109
        real t1096
        real t1097
        real t1098
        real t11
        real t1101
        real t1102
        real t1103
        real t1105
        real t111
        real t1110
        real t1111
        real t1112
        real t1125
        real t1128
        real t1131
        real t1132
        real t114
        real t1140
        real t1143
        real t1148
        real t115
        real t1151
        real t1155
        real t1161
        real t1162
        real t1164
        real t1166
        real t1168
        real t1170
        real t1172
        real t1174
        real t1177
        real t1183
        real t1184
        real t1196
        integer t120
        real t1200
        real t1201
        real t1202
        real t1203
        real t1205
        real t1207
        real t1209
        real t121
        real t1211
        real t1213
        real t1216
        real t1218
        real t1222
        real t1223
        real t123
        real t1246
        integer t125
        real t1250
        real t126
        real t1263
        real t1269
        real t1270
        real t1271
        real t1274
        real t1275
        real t1276
        real t1278
        real t1283
        real t1284
        real t1285
        real t129
        real t1294
        real t13
        real t1308
        real t1312
        real t1317
        real t1321
        real t1324
        real t133
        real t1332
        real t1337
        real t1341
        real t1344
        real t1352
        real t1357
        real t1361
        real t137
        real t1374
        real t138
        real t1381
        real t1382
        real t1383
        real t1386
        real t1387
        real t1388
        real t139
        real t1390
        real t1395
        real t1396
        real t1397
        real t140
        integer t1406
        real t1407
        real t141
        real t1414
        real t1416
        real t1418
        real t1420
        real t1424
        real t143
        real t1445
        real t145
        real t1458
        real t1459
        real t1461
        real t1463
        real t1465
        real t1467
        real t1469
        real t147
        real t1471
        real t1474
        real t1480
        real t1481
        real t1497
        real t1498
        real t1499
        real t15
        real t151
        real t1512
        real t1515
        real t1525
        real t1526
        real t1528
        real t1530
        real t1532
        real t1534
        real t1536
        real t1538
        real t1541
        real t1547
        real t1548
        real t155
        real t1558
        real t156
        real t1562
        real t1566
        real t1567
        real t1569
        real t1571
        real t1572
        real t1573
        real t1575
        real t1577
        real t1579
        real t158
        real t1582
        real t1586
        real t1588
        real t1589
        real t159
        real t1599
        real t161
        real t1616
        real t163
        real t1633
        real t1634
        real t1635
        real t1638
        real t1639
        real t1640
        real t1642
        real t1647
        real t1648
        real t1649
        real t165
        real t1658
        real t1672
        real t1676
        real t168
        real t1680
        real t1683
        real t1686
        real t1690
        real t1691
        real t1693
        real t17
        real t1701
        real t1706
        real t1719
        real t1726
        real t1727
        real t1728
        integer t173
        real t1731
        real t1732
        real t1733
        real t1735
        real t174
        real t1740
        real t1741
        real t1742
        integer t1751
        real t1752
        real t1759
        real t176
        real t1761
        real t1763
        real t1765
        real t1769
        integer t178
        real t179
        real t1790
        real t1803
        real t1804
        real t1806
        real t1808
        real t1810
        real t1812
        real t1814
        real t1816
        real t1819
        real t1825
        real t1826
        real t1842
        real t1843
        real t1844
        real t1857
        real t1860
        real t1870
        real t1871
        real t1873
        real t1875
        real t1877
        real t1879
        real t1881
        real t1883
        real t1886
        real t189
        real t1892
        real t1893
        real t19
        real t190
        real t1902
        real t1903
        real t1907
        real t1911
        real t1912
        real t1914
        real t1916
        real t1917
        real t1918
        real t192
        real t1920
        real t1922
        real t1924
        real t1927
        real t193
        real t1933
        real t1934
        real t1944
        real t195
        real t1961
        real t197
        real t1978
        real t1979
        real t1980
        real t1983
        real t1984
        real t1985
        real t1987
        real t199
        real t1992
        real t1993
        real t1994
        real t2
        real t2003
        real t2017
        real t2021
        real t2026
        real t2030
        real t2033
        real t2036
        real t2041
        real t2050
        real t206
        real t2063
        real t2068
        real t207
        real t2081
        real t2088
        real t2089
        real t209
        real t2090
        real t2093
        real t2094
        real t2095
        real t2097
        real t210
        real t2102
        real t2103
        real t2104
        real t212
        real t2124
        integer t2137
        real t2138
        real t214
        real t2145
        real t2147
        real t2149
        real t2151
        real t2155
        real t216
        real t2172
        real t2185
        real t2186
        real t2187
        real t219
        real t2190
        real t2191
        real t2192
        real t2194
        real t2199
        real t22
        real t220
        real t2200
        real t2201
        real t221
        real t2210
        real t2218
        real t222
        real t2222
        real t223
        real t2232
        real t2233
        real t2235
        real t2237
        real t2239
        real t2241
        real t2243
        real t2245
        real t2248
        real t225
        real t2254
        real t2255
        real t227
        real t2286
        real t2287
        real t2288
        real t229
        real t2296
        real t2297
        real t231
        real t2311
        real t2315
        real t2319
        real t2324
        real t233
        real t2337
        real t2342
        real t2355
        real t236
        real t2362
        real t2363
        real t2364
        real t2367
        real t2368
        real t2369
        real t2371
        real t2376
        real t2377
        real t2378
        real t2398
        integer t2411
        real t2412
        real t2419
        real t242
        real t2421
        real t2423
        real t2425
        real t2429
        real t243
        real t2446
        real t2459
        real t2460
        real t2461
        real t2464
        real t2465
        real t2466
        real t2468
        real t2473
        real t2474
        real t2475
        real t2484
        real t2492
        real t2496
        real t2506
        real t2507
        real t2509
        real t251
        real t2511
        real t2513
        real t2515
        real t2517
        real t2519
        real t2522
        real t2528
        real t2529
        real t2560
        real t2561
        real t2562
        real t257
        real t2571
        real t2585
        real t2589
        real t259
        real t263
        real t264
        real t266
        real t268
        real t27
        real t270
        real t272
        real t274
        real t276
        real t279
        real t28
        real t285
        real t286
        real t29
        real t294
        real t30
        real t302
        real t303
        real t304
        real t307
        real t308
        real t309
        real t31
        real t311
        real t316
        real t317
        real t318
        real t33
        real t331
        real t334
        real t338
        real t34
        real t346
        real t35
        real t352
        real t354
        real t357
        real t361
        real t364
        real t367
        real t368
        real t37
        real t370
        real t372
        real t374
        real t376
        real t378
        real t380
        real t383
        real t389
        real t39
        real t390
        real t398
        real t4
        real t404
        real t408
        real t409
        real t41
        real t411
        real t413
        real t415
        real t417
        real t419
        real t421
        real t424
        real t428
        real t43
        real t430
        real t431
        real t439
        real t441
        real t45
        real t456
        real t460
        real t47
        real t473
        real t479
        real t480
        real t481
        real t484
        real t485
        real t486
        real t488
        real t493
        real t494
        real t495
        integer t5
        real t50
        real t504
        real t511
        integer t516
        real t517
        real t518
        real t520
        real t522
        real t523
        real t524
        real t526
        real t528
        real t530
        real t533
        real t537
        real t538
        real t539
        real t540
        real t541
        real t542
        real t544
        real t546
        real t547
        real t548
        real t55
        real t550
        real t551
        real t554
        real t558
        real t559
        real t56
        real t561
        real t562
        real t564
        real t566
        real t568
        real t57
        real t571
        real t575
        real t576
        real t578
        real t579
        real t58
        real t581
        real t582
        real t583
        real t585
        real t588
        real t589
        real t59
        real t590
        real t592
        real t594
        real t596
        real t598
        real t6
        real t600
        real t602
        real t605
        real t61
        real t610
        real t611
        real t612
        real t616
        real t618
        real t620
        real t621
        real t622
        real t624
        real t626
        real t629
        real t63
        real t630
        real t631
        real t633
        real t635
        real t637
        real t639
        real t64
        real t641
        real t643
        real t646
        real t648
        real t65
        real t651
        real t652
        real t653
        real t655
        real t657
        real t659
        real t661
        real t663
        real t666
        real t667
        real t668
        real t669
        real t67
        real t671
        real t672
        real t673
        real t674
        real t676
        real t677
        real t678
        real t679
        real t68
        real t680
        real t681
        real t682
        real t683
        real t685
        real t687
        real t688
        real t689
        real t69
        real t695
        real t696
        real t698
        real t699
        real t7
        real t70
        real t701
        real t703
        real t705
        real t709
        real t711
        real t714
        real t717
        real t718
        real t719
        real t721
        real t722
        real t724
        real t726
        real t728
        real t731
        real t732
        real t733
        real t735
        real t737
        real t739
        real t741
        real t743
        real t745
        real t748
        real t753
        real t754
        real t755
        real t757
        real t759
        real t761
        real t763
        real t765
        real t767
        integer t77
        real t770
        real t771
        real t772
        real t774
        real t775
        real t776
        real t778
        real t78
        real t780
        real t782
        real t784
        real t787
        real t79
        real t790
        real t792
        real t793
        real t794
        real t798
        real t800
        real t802
        real t804
        real t807
        real t81
        real t811
        real t813
        real t815
        real t817
        real t819
        real t821
        real t824
        real t828
        real t83
        real t830
        real t832
        real t834
        real t836
        real t839
        real t840
        real t841
        real t842
        real t844
        real t845
        real t846
        real t847
        real t849
        real t85
        real t851
        real t852
        real t853
        real t854
        real t855
        real t856
        real t858
        real t860
        real t861
        real t862
        real t865
        real t868
        real t869
        real t87
        real t871
        real t875
        real t879
        real t883
        real t884
        real t887
        real t89
        real t890
        real t894
        real t897
        real t9
        real t900
        real t902
        real t903
        real t905
        real t91
        real t911
        real t914
        real t917
        real t921
        real t924
        real t927
        real t929
        real t930
        real t932
        real t938
        real t94
        integer t945
        real t946
        real t947
        real t949
        real t951
        real t953
        real t955
        real t957
        real t959
        real t962
        real t968
        real t969
        real t970
        real t971
        real t977
        real t979
        real t983
        real t988
        real t991
        t1 = u(i,j,k,n)
        t2 = ut(i,j,k,n)
        t4 = cc ** 2
        t5 = i + 1
        t6 = rx(t5,j,k,0,0)
        t7 = rx(t5,j,k,1,1)
        t9 = rx(t5,j,k,2,2)
        t11 = rx(t5,j,k,1,2)
        t13 = rx(t5,j,k,2,1)
        t15 = rx(t5,j,k,1,0)
        t17 = rx(t5,j,k,0,2)
        t19 = rx(t5,j,k,0,1)
        t22 = rx(t5,j,k,2,0)
        t27 = t6 * t7 * t9 - t6 * t11 * t13 + t15 * t13 * t17 - t15 * t1
     #9 * t9 + t22 * t19 * t11 - t22 * t7 * t17
        t28 = 0.1E1 / t27
        t29 = t6 ** 2
        t30 = t19 ** 2
        t31 = t17 ** 2
        t33 = t28 * (t29 + t30 + t31)
        t34 = rx(i,j,k,0,0)
        t35 = rx(i,j,k,1,1)
        t37 = rx(i,j,k,2,2)
        t39 = rx(i,j,k,1,2)
        t41 = rx(i,j,k,2,1)
        t43 = rx(i,j,k,1,0)
        t45 = rx(i,j,k,0,2)
        t47 = rx(i,j,k,0,1)
        t50 = rx(i,j,k,2,0)
        t55 = t34 * t35 * t37 - t34 * t39 * t41 + t43 * t41 * t45 - t43 
     #* t47 * t37 + t50 * t47 * t39 - t50 * t35 * t45
        t56 = 0.1E1 / t55
        t57 = t34 ** 2
        t58 = t47 ** 2
        t59 = t45 ** 2
        t61 = t56 * (t57 + t58 + t59)
        t63 = t33 / 0.2E1 + t61 / 0.2E1
        t64 = t4 * t63
        t65 = u(t5,j,k,n)
        t67 = 0.1E1 / dx
        t68 = (t65 - t1) * t67
        t69 = t64 * t68
        t70 = ut(t5,j,k,n)
        t77 = i + 2
        t78 = rx(t77,j,k,0,0)
        t79 = rx(t77,j,k,1,1)
        t81 = rx(t77,j,k,2,2)
        t83 = rx(t77,j,k,1,2)
        t85 = rx(t77,j,k,2,1)
        t87 = rx(t77,j,k,1,0)
        t89 = rx(t77,j,k,0,2)
        t91 = rx(t77,j,k,0,1)
        t94 = rx(t77,j,k,2,0)
        t100 = 0.1E1 / (t78 * t79 * t81 - t78 * t83 * t85 + t87 * t85 * 
     #t89 - t87 * t91 * t81 + t94 * t91 * t83 - t94 * t79 * t89)
        t101 = t78 ** 2
        t102 = t91 ** 2
        t103 = t89 ** 2
        t109 = u(t77,j,k,n)
        t111 = (t109 - t65) * t67
        t115 = t4 * t100
        t120 = j + 1
        t121 = u(t77,t120,k,n)
        t123 = 0.1E1 / dy
        t125 = j - 1
        t126 = u(t77,t125,k,n)
        t133 = t4 * t28
        t137 = t6 * t15 + t19 * t7 + t17 * t11
        t138 = u(t5,t120,k,n)
        t140 = (t138 - t65) * t123
        t141 = u(t5,t125,k,n)
        t143 = (t65 - t141) * t123
        t145 = t140 / 0.2E1 + t143 / 0.2E1
        t105 = t133 * t137
        t147 = t105 * t145
        t151 = t4 * t56
        t155 = t34 * t43 + t47 * t35 + t45 * t39
        t156 = u(i,t120,k,n)
        t158 = (t156 - t1) * t123
        t159 = u(i,t125,k,n)
        t161 = (t1 - t159) * t123
        t163 = t158 / 0.2E1 + t161 / 0.2E1
        t114 = t151 * t155
        t165 = t114 * t163
        t168 = (t147 - t165) * t67 / 0.2E1
        t173 = k + 1
        t174 = u(t77,j,t173,n)
        t176 = 0.1E1 / dz
        t178 = k - 1
        t179 = u(t77,j,t178,n)
        t189 = t6 * t22 + t19 * t13 + t17 * t9
        t190 = u(t5,j,t173,n)
        t192 = (t190 - t65) * t176
        t193 = u(t5,j,t178,n)
        t195 = (t65 - t193) * t176
        t197 = t192 / 0.2E1 + t195 / 0.2E1
        t129 = t133 * t189
        t199 = t129 * t197
        t206 = t34 * t50 + t47 * t41 + t45 * t37
        t207 = u(i,j,t173,n)
        t209 = (t207 - t1) * t176
        t210 = u(i,j,t178,n)
        t212 = (t1 - t210) * t176
        t214 = t209 / 0.2E1 + t212 / 0.2E1
        t139 = t151 * t206
        t216 = t139 * t214
        t219 = (t199 - t216) * t67 / 0.2E1
        t220 = rx(t5,t120,k,0,0)
        t221 = rx(t5,t120,k,1,1)
        t223 = rx(t5,t120,k,2,2)
        t225 = rx(t5,t120,k,1,2)
        t227 = rx(t5,t120,k,2,1)
        t229 = rx(t5,t120,k,1,0)
        t231 = rx(t5,t120,k,0,2)
        t233 = rx(t5,t120,k,0,1)
        t236 = rx(t5,t120,k,2,0)
        t242 = 0.1E1 / (t220 * t221 * t223 - t220 * t225 * t227 + t229 *
     # t227 * t231 - t229 * t233 * t223 + t236 * t233 * t225 - t236 * t2
     #21 * t231)
        t243 = t4 * t242
        t251 = (t138 - t156) * t67
        t257 = t111 / 0.2E1 + t68 / 0.2E1
        t259 = t105 * t257
        t263 = rx(t5,t125,k,0,0)
        t264 = rx(t5,t125,k,1,1)
        t266 = rx(t5,t125,k,2,2)
        t268 = rx(t5,t125,k,1,2)
        t270 = rx(t5,t125,k,2,1)
        t272 = rx(t5,t125,k,1,0)
        t274 = rx(t5,t125,k,0,2)
        t276 = rx(t5,t125,k,0,1)
        t279 = rx(t5,t125,k,2,0)
        t285 = 0.1E1 / (t263 * t264 * t266 - t263 * t268 * t270 + t272 *
     # t270 * t274 - t272 * t276 * t266 + t279 * t276 * t268 - t279 * t2
     #64 * t274)
        t286 = t4 * t285
        t294 = (t141 - t159) * t67
        t302 = t229 ** 2
        t303 = t221 ** 2
        t304 = t225 ** 2
        t307 = t15 ** 2
        t308 = t7 ** 2
        t309 = t11 ** 2
        t311 = t28 * (t307 + t308 + t309)
        t316 = t272 ** 2
        t317 = t264 ** 2
        t318 = t268 ** 2
        t331 = u(t5,t120,t173,n)
        t334 = u(t5,t120,t178,n)
        t338 = (t331 - t138) * t176 / 0.2E1 + (t138 - t334) * t176 / 0.2
     #E1
        t222 = t133 * (t15 * t22 + t7 * t13 + t11 * t9)
        t346 = t222 * t197
        t354 = u(t5,t125,t173,n)
        t357 = u(t5,t125,t178,n)
        t361 = (t354 - t141) * t176 / 0.2E1 + (t141 - t357) * t176 / 0.2
     #E1
        t367 = rx(t5,j,t173,0,0)
        t368 = rx(t5,j,t173,1,1)
        t370 = rx(t5,j,t173,2,2)
        t372 = rx(t5,j,t173,1,2)
        t374 = rx(t5,j,t173,2,1)
        t376 = rx(t5,j,t173,1,0)
        t378 = rx(t5,j,t173,0,2)
        t380 = rx(t5,j,t173,0,1)
        t383 = rx(t5,j,t173,2,0)
        t389 = 0.1E1 / (t367 * t368 * t370 - t374 * t372 * t367 + t376 *
     # t374 * t378 - t376 * t380 * t370 + t383 * t380 * t372 - t383 * t3
     #68 * t378)
        t390 = t4 * t389
        t398 = (t190 - t207) * t67
        t404 = t129 * t257
        t408 = rx(t5,j,t178,0,0)
        t409 = rx(t5,j,t178,1,1)
        t411 = rx(t5,j,t178,2,2)
        t413 = rx(t5,j,t178,1,2)
        t415 = rx(t5,j,t178,2,1)
        t417 = rx(t5,j,t178,1,0)
        t419 = rx(t5,j,t178,0,2)
        t421 = rx(t5,j,t178,0,1)
        t424 = rx(t5,j,t178,2,0)
        t430 = 0.1E1 / (t408 * t409 * t411 - t408 * t413 * t415 + t417 *
     # t415 * t419 - t417 * t421 * t411 + t424 * t421 * t413 - t424 * t4
     #09 * t419)
        t431 = t4 * t430
        t439 = (t193 - t210) * t67
        t456 = (t331 - t190) * t123 / 0.2E1 + (t190 - t354) * t123 / 0.2
     #E1
        t460 = t222 * t145
        t473 = (t334 - t193) * t123 / 0.2E1 + (t193 - t357) * t123 / 0.2
     #E1
        t479 = t383 ** 2
        t480 = t374 ** 2
        t481 = t370 ** 2
        t484 = t22 ** 2
        t485 = t13 ** 2
        t486 = t9 ** 2
        t488 = t28 * (t484 + t485 + t486)
        t493 = t424 ** 2
        t494 = t415 ** 2
        t495 = t411 ** 2
        t352 = t243 * (t220 * t229 + t233 * t221 + t231 * t225)
        t364 = t286 * (t263 * t272 + t276 * t264 + t274 * t268)
        t428 = t390 * (t367 * t383 + t380 * t374 + t378 * t370)
        t441 = t431 * (t408 * t424 + t421 * t415 + t419 * t411)
        t504 = (t4 * (t100 * (t101 + t102 + t103) / 0.2E1 + t33 / 0.2E1)
     # * t111 - t69) * t67 + (t115 * (t78 * t87 + t91 * t79 + t89 * t83)
     # * ((t121 - t109) * t123 / 0.2E1 + (t109 - t126) * t123 / 0.2E1) -
     # t147) * t67 / 0.2E1 + t168 + (t115 * (t78 * t94 + t91 * t85 + t89
     # * t81) * ((t174 - t109) * t176 / 0.2E1 + (t109 - t179) * t176 / 0
     #.2E1) - t199) * t67 / 0.2E1 + t219 + (t352 * ((t121 - t138) * t67 
     #/ 0.2E1 + t251 / 0.2E1) - t259) * t123 / 0.2E1 + (t259 - t364 * ((
     #t126 - t141) * t67 / 0.2E1 + t294 / 0.2E1)) * t123 / 0.2E1 + (t4 *
     # (t242 * (t302 + t303 + t304) / 0.2E1 + t311 / 0.2E1) * t140 - t4 
     #* (t311 / 0.2E1 + t285 * (t316 + t317 + t318) / 0.2E1) * t143) * t
     #123 + (t243 * (t229 * t236 + t221 * t227 + t225 * t223) * t338 - t
     #346) * t123 / 0.2E1 + (t346 - t286 * (t272 * t279 + t264 * t270 + 
     #t268 * t266) * t361) * t123 / 0.2E1 + (t428 * ((t174 - t190) * t67
     # / 0.2E1 + t398 / 0.2E1) - t404) * t176 / 0.2E1 + (t404 - t441 * (
     #(t179 - t193) * t67 / 0.2E1 + t439 / 0.2E1)) * t176 / 0.2E1 + (t39
     #0 * (t376 * t383 + t368 * t374 + t372 * t370) * t456 - t460) * t17
     #6 / 0.2E1 + (t460 - t431 * (t417 * t424 + t409 * t415 + t413 * t41
     #1) * t473) * t176 / 0.2E1 + (t4 * (t389 * (t479 + t480 + t481) / 0
     #.2E1 + t488 / 0.2E1) * t192 - t4 * (t488 / 0.2E1 + t430 * (t493 + 
     #t494 + t495) / 0.2E1) * t195) * t176
        t511 = (t70 - t2) * t67
        t516 = i - 1
        t517 = rx(t516,j,k,0,0)
        t518 = rx(t516,j,k,1,1)
        t520 = rx(t516,j,k,2,2)
        t522 = rx(t516,j,k,1,2)
        t524 = rx(t516,j,k,2,1)
        t526 = rx(t516,j,k,1,0)
        t528 = rx(t516,j,k,0,2)
        t530 = rx(t516,j,k,0,1)
        t533 = rx(t516,j,k,2,0)
        t538 = t518 * t517 * t520 - t517 * t522 * t524 + t526 * t524 * t
     #528 - t526 * t530 * t520 + t533 * t530 * t522 - t533 * t518 * t528
        t539 = 0.1E1 / t538
        t540 = t517 ** 2
        t541 = t530 ** 2
        t542 = t528 ** 2
        t544 = t539 * (t540 + t541 + t542)
        t546 = t61 / 0.2E1 + t544 / 0.2E1
        t547 = t4 * t546
        t548 = u(t516,j,k,n)
        t550 = (t1 - t548) * t67
        t551 = t547 * t550
        t554 = t4 * t539
        t558 = t517 * t526 + t530 * t518 + t528 * t522
        t559 = u(t516,t120,k,n)
        t561 = (t559 - t548) * t123
        t562 = u(t516,t125,k,n)
        t564 = (t548 - t562) * t123
        t566 = t561 / 0.2E1 + t564 / 0.2E1
        t523 = t554 * t558
        t568 = t523 * t566
        t571 = (t165 - t568) * t67 / 0.2E1
        t575 = t517 * t533 + t530 * t524 + t528 * t520
        t576 = u(t516,j,t173,n)
        t578 = (t576 - t548) * t176
        t579 = u(t516,j,t178,n)
        t581 = (t548 - t579) * t176
        t583 = t578 / 0.2E1 + t581 / 0.2E1
        t537 = t554 * t575
        t585 = t537 * t583
        t588 = (t216 - t585) * t67 / 0.2E1
        t589 = rx(i,t120,k,0,0)
        t590 = rx(i,t120,k,1,1)
        t592 = rx(i,t120,k,2,2)
        t594 = rx(i,t120,k,1,2)
        t596 = rx(i,t120,k,2,1)
        t598 = rx(i,t120,k,1,0)
        t600 = rx(i,t120,k,0,2)
        t602 = rx(i,t120,k,0,1)
        t605 = rx(i,t120,k,2,0)
        t610 = t589 * t590 * t592 - t589 * t594 * t596 + t598 * t596 * t
     #600 - t598 * t602 * t592 + t605 * t602 * t594 - t605 * t590 * t600
        t611 = 0.1E1 / t610
        t612 = t4 * t611
        t616 = t589 * t598 + t602 * t590 + t600 * t594
        t618 = (t156 - t559) * t67
        t620 = t251 / 0.2E1 + t618 / 0.2E1
        t582 = t612 * t616
        t622 = t582 * t620
        t624 = t68 / 0.2E1 + t550 / 0.2E1
        t626 = t114 * t624
        t629 = (t622 - t626) * t123 / 0.2E1
        t630 = rx(i,t125,k,0,0)
        t631 = rx(i,t125,k,1,1)
        t633 = rx(i,t125,k,2,2)
        t635 = rx(i,t125,k,1,2)
        t637 = rx(i,t125,k,2,1)
        t639 = rx(i,t125,k,1,0)
        t641 = rx(i,t125,k,0,2)
        t643 = rx(i,t125,k,0,1)
        t646 = rx(i,t125,k,2,0)
        t651 = t630 * t631 * t633 - t630 * t635 * t637 + t639 * t637 * t
     #641 - t639 * t643 * t633 + t646 * t643 * t635 - t646 * t631 * t641
        t652 = 0.1E1 / t651
        t653 = t4 * t652
        t657 = t630 * t639 + t643 * t631 + t641 * t635
        t659 = (t159 - t562) * t67
        t661 = t294 / 0.2E1 + t659 / 0.2E1
        t621 = t653 * t657
        t663 = t621 * t661
        t666 = (t626 - t663) * t123 / 0.2E1
        t667 = t598 ** 2
        t668 = t590 ** 2
        t669 = t594 ** 2
        t671 = t611 * (t667 + t668 + t669)
        t672 = t43 ** 2
        t673 = t35 ** 2
        t674 = t39 ** 2
        t676 = t56 * (t672 + t673 + t674)
        t678 = t671 / 0.2E1 + t676 / 0.2E1
        t679 = t4 * t678
        t680 = t679 * t158
        t681 = t639 ** 2
        t682 = t631 ** 2
        t683 = t635 ** 2
        t685 = t652 * (t681 + t682 + t683)
        t687 = t676 / 0.2E1 + t685 / 0.2E1
        t688 = t4 * t687
        t689 = t688 * t161
        t695 = t598 * t605 + t590 * t596 + t594 * t592
        t696 = u(i,t120,t173,n)
        t698 = (t696 - t156) * t176
        t699 = u(i,t120,t178,n)
        t701 = (t156 - t699) * t176
        t703 = t698 / 0.2E1 + t701 / 0.2E1
        t648 = t612 * t695
        t705 = t648 * t703
        t709 = t43 * t50 + t35 * t41 + t39 * t37
        t655 = t151 * t709
        t711 = t655 * t214
        t714 = (t705 - t711) * t123 / 0.2E1
        t718 = t639 * t646 + t631 * t637 + t635 * t633
        t719 = u(i,t125,t173,n)
        t721 = (t719 - t159) * t176
        t722 = u(i,t125,t178,n)
        t724 = (t159 - t722) * t176
        t726 = t721 / 0.2E1 + t724 / 0.2E1
        t677 = t653 * t718
        t728 = t677 * t726
        t731 = (t711 - t728) * t123 / 0.2E1
        t732 = rx(i,j,t173,0,0)
        t733 = rx(i,j,t173,1,1)
        t735 = rx(i,j,t173,2,2)
        t737 = rx(i,j,t173,1,2)
        t739 = rx(i,j,t173,2,1)
        t741 = rx(i,j,t173,1,0)
        t743 = rx(i,j,t173,0,2)
        t745 = rx(i,j,t173,0,1)
        t748 = rx(i,j,t173,2,0)
        t753 = t732 * t733 * t735 - t732 * t737 * t739 + t741 * t739 * t
     #743 - t741 * t745 * t735 + t748 * t745 * t737 - t748 * t733 * t743
        t754 = 0.1E1 / t753
        t755 = t4 * t754
        t759 = t732 * t748 + t745 * t739 + t743 * t735
        t761 = (t207 - t576) * t67
        t763 = t398 / 0.2E1 + t761 / 0.2E1
        t717 = t755 * t759
        t765 = t717 * t763
        t767 = t139 * t624
        t770 = (t765 - t767) * t176 / 0.2E1
        t771 = rx(i,j,t178,0,0)
        t772 = rx(i,j,t178,1,1)
        t774 = rx(i,j,t178,2,2)
        t776 = rx(i,j,t178,1,2)
        t778 = rx(i,j,t178,2,1)
        t780 = rx(i,j,t178,1,0)
        t782 = rx(i,j,t178,0,2)
        t784 = rx(i,j,t178,0,1)
        t787 = rx(i,j,t178,2,0)
        t792 = t771 * t772 * t774 - t771 * t776 * t778 + t780 * t778 * t
     #782 - t780 * t784 * t774 + t787 * t784 * t776 - t787 * t772 * t782
        t793 = 0.1E1 / t792
        t794 = t4 * t793
        t798 = t771 * t787 + t784 * t778 + t782 * t774
        t800 = (t210 - t579) * t67
        t802 = t439 / 0.2E1 + t800 / 0.2E1
        t757 = t794 * t798
        t804 = t757 * t802
        t807 = (t767 - t804) * t176 / 0.2E1
        t811 = t741 * t748 + t733 * t739 + t737 * t735
        t813 = (t696 - t207) * t123
        t815 = (t207 - t719) * t123
        t817 = t813 / 0.2E1 + t815 / 0.2E1
        t775 = t755 * t811
        t819 = t775 * t817
        t821 = t655 * t163
        t824 = (t819 - t821) * t176 / 0.2E1
        t828 = t780 * t787 + t772 * t778 + t776 * t774
        t830 = (t699 - t210) * t123
        t832 = (t210 - t722) * t123
        t834 = t830 / 0.2E1 + t832 / 0.2E1
        t790 = t794 * t828
        t836 = t790 * t834
        t839 = (t821 - t836) * t176 / 0.2E1
        t840 = t748 ** 2
        t841 = t739 ** 2
        t842 = t735 ** 2
        t844 = t754 * (t840 + t841 + t842)
        t845 = t50 ** 2
        t846 = t41 ** 2
        t847 = t37 ** 2
        t849 = t56 * (t845 + t846 + t847)
        t851 = t844 / 0.2E1 + t849 / 0.2E1
        t852 = t4 * t851
        t853 = t852 * t209
        t854 = t787 ** 2
        t855 = t778 ** 2
        t856 = t774 ** 2
        t858 = t793 * (t854 + t855 + t856)
        t860 = t849 / 0.2E1 + t858 / 0.2E1
        t861 = t4 * t860
        t862 = t861 * t212
        t865 = (t69 - t551) * t67 + t168 + t571 + t219 + t588 + t629 + t
     #666 + (t680 - t689) * t123 + t714 + t731 + t770 + t807 + t824 + t8
     #39 + (t853 - t862) * t176
        t868 = dt * t865 * t55 / 0.2E1
        t869 = ut(t516,j,k,n)
        t871 = (t2 - t869) * t67
        t875 = dx * (t511 / 0.2E1 + t871 / 0.2E1) / 0.2E1
        t879 = sqrt(0.2E1 * t29 + 0.2E1 * t30 + 0.2E1 * t31 + 0.2E1 * t5
     #7 + 0.2E1 * t58 + 0.2E1 * t59)
        t883 = t69 + t64 * dt * t511 / 0.2E1 + cc * t63 * (t70 + dt * t5
     #04 * t27 / 0.2E1 - dx * ((ut(t77,j,k,n) - t70) * t67 / 0.2E1 + t51
     #1 / 0.2E1) / 0.2E1 - t2 - t868 - t875) / t879
        t884 = dt ** 2
        t887 = t56 * t155
        t890 = t4 * (t28 * t137 / 0.2E1 + t887 / 0.2E1)
        t894 = ut(t5,t120,k,n)
        t897 = ut(t5,t125,k,n)
        t900 = ut(i,t120,k,n)
        t902 = (t900 - t2) * t123
        t903 = ut(i,t125,k,n)
        t905 = (t2 - t903) * t123
        t911 = t890 * (t140 / 0.4E1 + t143 / 0.4E1 + t158 / 0.4E1 + t161
     # / 0.4E1) + t890 * dt * ((t894 - t70) * t123 / 0.4E1 + (t70 - t897
     #) * t123 / 0.4E1 + t902 / 0.4E1 + t905 / 0.4E1) / 0.2E1
        t914 = t56 * t206
        t917 = t4 * (t28 * t189 / 0.2E1 + t914 / 0.2E1)
        t921 = ut(t5,j,t173,n)
        t924 = ut(t5,j,t178,n)
        t927 = ut(i,j,t173,n)
        t929 = (t927 - t2) * t176
        t930 = ut(i,j,t178,n)
        t932 = (t2 - t930) * t176
        t938 = t917 * (t192 / 0.4E1 + t195 / 0.4E1 + t209 / 0.4E1 + t212
     # / 0.4E1) + t917 * dt * ((t921 - t70) * t176 / 0.4E1 + (t70 - t924
     #) * t176 / 0.4E1 + t929 / 0.4E1 + t932 / 0.4E1) / 0.2E1
        t945 = i - 2
        t946 = rx(t945,j,k,0,0)
        t947 = rx(t945,j,k,1,1)
        t949 = rx(t945,j,k,2,2)
        t951 = rx(t945,j,k,1,2)
        t953 = rx(t945,j,k,2,1)
        t955 = rx(t945,j,k,1,0)
        t957 = rx(t945,j,k,0,2)
        t959 = rx(t945,j,k,0,1)
        t962 = rx(t945,j,k,2,0)
        t968 = 0.1E1 / (t946 * t947 * t949 - t946 * t951 * t953 + t955 *
     # t953 * t957 - t955 * t959 * t949 + t962 * t959 * t951 - t962 * t9
     #47 * t957)
        t969 = t946 ** 2
        t970 = t959 ** 2
        t971 = t957 ** 2
        t977 = u(t945,j,k,n)
        t979 = (t548 - t977) * t67
        t983 = t4 * t968
        t988 = u(t945,t120,k,n)
        t991 = u(t945,t125,k,n)
        t1005 = u(t945,j,t173,n)
        t1008 = u(t945,j,t178,n)
        t1018 = rx(t516,t120,k,0,0)
        t1019 = rx(t516,t120,k,1,1)
        t1021 = rx(t516,t120,k,2,2)
        t1023 = rx(t516,t120,k,1,2)
        t1025 = rx(t516,t120,k,2,1)
        t1027 = rx(t516,t120,k,1,0)
        t1029 = rx(t516,t120,k,0,2)
        t1031 = rx(t516,t120,k,0,1)
        t1034 = rx(t516,t120,k,2,0)
        t1040 = 0.1E1 / (t1018 * t1019 * t1021 - t1018 * t1023 * t1025 +
     # t1027 * t1025 * t1029 - t1027 * t1031 * t1021 + t1034 * t1031 * t
     #1023 - t1034 * t1019 * t1029)
        t1041 = t4 * t1040
        t1053 = t550 / 0.2E1 + t979 / 0.2E1
        t1055 = t523 * t1053
        t1059 = rx(t516,t125,k,0,0)
        t1060 = rx(t516,t125,k,1,1)
        t1062 = rx(t516,t125,k,2,2)
        t1064 = rx(t516,t125,k,1,2)
        t1066 = rx(t516,t125,k,2,1)
        t1068 = rx(t516,t125,k,1,0)
        t1070 = rx(t516,t125,k,0,2)
        t1072 = rx(t516,t125,k,0,1)
        t1075 = rx(t516,t125,k,2,0)
        t1081 = 0.1E1 / (t1059 * t1060 * t1062 - t1059 * t1064 * t1066 +
     # t1068 * t1066 * t1070 - t1068 * t1072 * t1062 + t1075 * t1072 * t
     #1064 - t1075 * t1060 * t1070)
        t1082 = t4 * t1081
        t1096 = t1027 ** 2
        t1097 = t1019 ** 2
        t1098 = t1023 ** 2
        t1101 = t526 ** 2
        t1102 = t518 ** 2
        t1103 = t522 ** 2
        t1105 = t539 * (t1101 + t1102 + t1103)
        t1110 = t1068 ** 2
        t1111 = t1060 ** 2
        t1112 = t1064 ** 2
        t1125 = u(t516,t120,t173,n)
        t1128 = u(t516,t120,t178,n)
        t1132 = (t1125 - t559) * t176 / 0.2E1 + (t559 - t1128) * t176 / 
     #0.2E1
        t1004 = t554 * (t526 * t533 + t518 * t524 + t522 * t520)
        t1140 = t1004 * t583
        t1148 = u(t516,t125,t173,n)
        t1151 = u(t516,t125,t178,n)
        t1155 = (t1148 - t562) * t176 / 0.2E1 + (t562 - t1151) * t176 / 
     #0.2E1
        t1161 = rx(t516,j,t173,0,0)
        t1162 = rx(t516,j,t173,1,1)
        t1164 = rx(t516,j,t173,2,2)
        t1166 = rx(t516,j,t173,1,2)
        t1168 = rx(t516,j,t173,2,1)
        t1170 = rx(t516,j,t173,1,0)
        t1172 = rx(t516,j,t173,0,2)
        t1174 = rx(t516,j,t173,0,1)
        t1177 = rx(t516,j,t173,2,0)
        t1183 = 0.1E1 / (t1161 * t1162 * t1164 - t1161 * t1166 * t1168 +
     # t1170 * t1168 * t1172 - t1170 * t1174 * t1164 + t1177 * t1174 * t
     #1166 - t1177 * t1162 * t1172)
        t1184 = t4 * t1183
        t1196 = t537 * t1053
        t1200 = rx(t516,j,t178,0,0)
        t1201 = rx(t516,j,t178,1,1)
        t1203 = rx(t516,j,t178,2,2)
        t1205 = rx(t516,j,t178,1,2)
        t1207 = rx(t516,j,t178,2,1)
        t1209 = rx(t516,j,t178,1,0)
        t1211 = rx(t516,j,t178,0,2)
        t1213 = rx(t516,j,t178,0,1)
        t1216 = rx(t516,j,t178,2,0)
        t1222 = 0.1E1 / (t1200 * t1201 * t1203 - t1200 * t1205 * t1207 +
     # t1209 * t1207 * t1211 - t1209 * t1213 * t1203 + t1216 * t1213 * t
     #1205 - t1216 * t1201 * t1211)
        t1223 = t4 * t1222
        t1246 = (t1125 - t576) * t123 / 0.2E1 + (t576 - t1148) * t123 / 
     #0.2E1
        t1250 = t1004 * t566
        t1263 = (t1128 - t579) * t123 / 0.2E1 + (t579 - t1151) * t123 / 
     #0.2E1
        t1269 = t1177 ** 2
        t1270 = t1168 ** 2
        t1271 = t1164 ** 2
        t1274 = t533 ** 2
        t1275 = t524 ** 2
        t1276 = t520 ** 2
        t1278 = t539 * (t1274 + t1275 + t1276)
        t1283 = t1216 ** 2
        t1284 = t1207 ** 2
        t1285 = t1203 ** 2
        t1131 = t1041 * (t1018 * t1027 + t1031 * t1019 + t1029 * t1023)
        t1143 = t1082 * (t1059 * t1068 + t1072 * t1060 + t1070 * t1064)
        t1202 = t1184 * (t1161 * t1177 + t1174 * t1168 + t1172 * t1164)
        t1218 = t1223 * (t1200 * t1216 + t1213 * t1207 + t1211 * t1203)
        t1294 = (t551 - t4 * (t544 / 0.2E1 + t968 * (t969 + t970 + t971)
     # / 0.2E1) * t979) * t67 + t571 + (t568 - t983 * (t946 * t955 + t95
     #9 * t947 + t957 * t951) * ((t988 - t977) * t123 / 0.2E1 + (t977 - 
     #t991) * t123 / 0.2E1)) * t67 / 0.2E1 + t588 + (t585 - t983 * (t946
     # * t962 + t959 * t953 + t957 * t949) * ((t1005 - t977) * t176 / 0.
     #2E1 + (t977 - t1008) * t176 / 0.2E1)) * t67 / 0.2E1 + (t1131 * (t6
     #18 / 0.2E1 + (t559 - t988) * t67 / 0.2E1) - t1055) * t123 / 0.2E1 
     #+ (t1055 - t1143 * (t659 / 0.2E1 + (t562 - t991) * t67 / 0.2E1)) *
     # t123 / 0.2E1 + (t4 * (t1040 * (t1096 + t1097 + t1098) / 0.2E1 + t
     #1105 / 0.2E1) * t561 - t4 * (t1105 / 0.2E1 + t1081 * (t1110 + t111
     #1 + t1112) / 0.2E1) * t564) * t123 + (t1041 * (t1027 * t1034 + t10
     #19 * t1025 + t1023 * t1021) * t1132 - t1140) * t123 / 0.2E1 + (t11
     #40 - t1082 * (t1068 * t1075 + t1060 * t1066 + t1064 * t1062) * t11
     #55) * t123 / 0.2E1 + (t1202 * (t761 / 0.2E1 + (t576 - t1005) * t67
     # / 0.2E1) - t1196) * t176 / 0.2E1 + (t1196 - t1218 * (t800 / 0.2E1
     # + (t579 - t1008) * t67 / 0.2E1)) * t176 / 0.2E1 + (t1184 * (t1170
     # * t1177 + t1168 * t1162 + t1166 * t1164) * t1246 - t1250) * t176 
     #/ 0.2E1 + (t1250 - t1223 * (t1209 * t1216 + t1201 * t1207 + t1205 
     #* t1203) * t1263) * t176 / 0.2E1 + (t4 * (t1183 * (t1269 + t1270 +
     # t1271) / 0.2E1 + t1278 / 0.2E1) * t578 - t4 * (t1278 / 0.2E1 + t1
     #222 * (t1283 + t1284 + t1285) / 0.2E1) * t581) * t176
        t1308 = sqrt(0.2E1 * t57 + 0.2E1 * t58 + 0.2E1 * t59 + 0.2E1 * t
     #540 + 0.2E1 * t541 + 0.2E1 * t542)
        t1312 = t551 + t547 * dt * t871 / 0.2E1 + cc * t546 * (t2 + t868
     # - t875 - t869 - dt * t1294 * t538 / 0.2E1 - dx * (t871 / 0.2E1 + 
     #(t869 - ut(t945,j,k,n)) * t67 / 0.2E1) / 0.2E1) / t1308
        t1317 = t4 * (t887 / 0.2E1 + t539 * t558 / 0.2E1)
        t1321 = ut(t516,t120,k,n)
        t1324 = ut(t516,t125,k,n)
        t1332 = t1317 * (t158 / 0.4E1 + t161 / 0.4E1 + t561 / 0.4E1 + t5
     #64 / 0.4E1) + t1317 * dt * (t902 / 0.4E1 + t905 / 0.4E1 + (t1321 -
     # t869) * t123 / 0.4E1 + (t869 - t1324) * t123 / 0.4E1) / 0.2E1
        t1337 = t4 * (t914 / 0.2E1 + t539 * t575 / 0.2E1)
        t1341 = ut(t516,j,t173,n)
        t1344 = ut(t516,j,t178,n)
        t1352 = t1337 * (t209 / 0.4E1 + t212 / 0.4E1 + t578 / 0.4E1 + t5
     #81 / 0.4E1) + t1337 * dt * (t929 / 0.4E1 + t932 / 0.4E1 + (t1341 -
     # t869) * t176 / 0.4E1 + (t869 - t1344) * t176 / 0.4E1) / 0.2E1
        t1361 = t4 * (t611 * t616 / 0.2E1 + t887 / 0.2E1)
        t1374 = t1361 * (t251 / 0.4E1 + t618 / 0.4E1 + t68 / 0.4E1 + t55
     #0 / 0.4E1) + t1361 * dt * ((t894 - t900) * t67 / 0.4E1 + (t900 - t
     #1321) * t67 / 0.4E1 + t511 / 0.4E1 + t871 / 0.4E1) / 0.2E1
        t1381 = t220 ** 2
        t1382 = t233 ** 2
        t1383 = t231 ** 2
        t1386 = t589 ** 2
        t1387 = t602 ** 2
        t1388 = t600 ** 2
        t1390 = t611 * (t1386 + t1387 + t1388)
        t1395 = t1018 ** 2
        t1396 = t1031 ** 2
        t1397 = t1029 ** 2
        t1406 = j + 2
        t1407 = u(t5,t1406,k,n)
        t1414 = u(i,t1406,k,n)
        t1416 = (t1414 - t156) * t123
        t1418 = t1416 / 0.2E1 + t158 / 0.2E1
        t1420 = t582 * t1418
        t1424 = u(t516,t1406,k,n)
        t1357 = t612 * (t589 * t605 + t602 * t596 + t600 * t592)
        t1445 = t1357 * t703
        t1458 = rx(i,t1406,k,0,0)
        t1459 = rx(i,t1406,k,1,1)
        t1461 = rx(i,t1406,k,2,2)
        t1463 = rx(i,t1406,k,1,2)
        t1465 = rx(i,t1406,k,2,1)
        t1467 = rx(i,t1406,k,1,0)
        t1469 = rx(i,t1406,k,0,2)
        t1471 = rx(i,t1406,k,0,1)
        t1474 = rx(i,t1406,k,2,0)
        t1480 = 0.1E1 / (t1459 * t1458 * t1461 - t1458 * t1463 * t1465 +
     # t1467 * t1465 * t1469 - t1467 * t1471 * t1461 + t1474 * t1471 * t
     #1463 - t1474 * t1459 * t1469)
        t1481 = t4 * t1480
        t1497 = t1467 ** 2
        t1498 = t1459 ** 2
        t1499 = t1463 ** 2
        t1512 = u(i,t1406,t173,n)
        t1515 = u(i,t1406,t178,n)
        t1525 = rx(i,t120,t173,0,0)
        t1526 = rx(i,t120,t173,1,1)
        t1528 = rx(i,t120,t173,2,2)
        t1530 = rx(i,t120,t173,1,2)
        t1532 = rx(i,t120,t173,2,1)
        t1534 = rx(i,t120,t173,1,0)
        t1536 = rx(i,t120,t173,0,2)
        t1538 = rx(i,t120,t173,0,1)
        t1541 = rx(i,t120,t173,2,0)
        t1547 = 0.1E1 / (t1526 * t1525 * t1528 - t1525 * t1530 * t1532 +
     # t1534 * t1532 * t1536 - t1534 * t1538 * t1528 + t1541 * t1538 * t
     #1530 - t1541 * t1526 * t1536)
        t1548 = t4 * t1547
        t1558 = (t331 - t696) * t67 / 0.2E1 + (t696 - t1125) * t67 / 0.2
     #E1
        t1562 = t1357 * t620
        t1566 = rx(i,t120,t178,0,0)
        t1567 = rx(i,t120,t178,1,1)
        t1569 = rx(i,t120,t178,2,2)
        t1571 = rx(i,t120,t178,1,2)
        t1573 = rx(i,t120,t178,2,1)
        t1575 = rx(i,t120,t178,1,0)
        t1577 = rx(i,t120,t178,0,2)
        t1579 = rx(i,t120,t178,0,1)
        t1582 = rx(i,t120,t178,2,0)
        t1588 = 0.1E1 / (t1567 * t1566 * t1569 - t1566 * t1571 * t1573 +
     # t1575 * t1573 * t1577 - t1575 * t1579 * t1569 + t1582 * t1579 * t
     #1571 - t1582 * t1567 * t1577)
        t1589 = t4 * t1588
        t1599 = (t334 - t699) * t67 / 0.2E1 + (t699 - t1128) * t67 / 0.2
     #E1
        t1616 = t648 * t1418
        t1633 = t1541 ** 2
        t1634 = t1532 ** 2
        t1635 = t1528 ** 2
        t1638 = t605 ** 2
        t1639 = t596 ** 2
        t1640 = t592 ** 2
        t1642 = t611 * (t1638 + t1639 + t1640)
        t1647 = t1582 ** 2
        t1648 = t1573 ** 2
        t1649 = t1569 ** 2
        t1572 = t1548 * (t1534 * t1541 + t1526 * t1532 + t1530 * t1528)
        t1586 = t1589 * (t1575 * t1582 + t1567 * t1573 + t1571 * t1569)
        t1658 = (t4 * (t242 * (t1381 + t1382 + t1383) / 0.2E1 + t1390 / 
     #0.2E1) * t251 - t4 * (t1390 / 0.2E1 + t1040 * (t1395 + t1396 + t13
     #97) / 0.2E1) * t618) * t67 + (t352 * ((t1407 - t138) * t123 / 0.2E
     #1 + t140 / 0.2E1) - t1420) * t67 / 0.2E1 + (t1420 - t1131 * ((t142
     #4 - t559) * t123 / 0.2E1 + t561 / 0.2E1)) * t67 / 0.2E1 + (t243 * 
     #(t220 * t236 + t233 * t227 + t231 * t223) * t338 - t1445) * t67 / 
     #0.2E1 + (t1445 - t1041 * (t1018 * t1034 + t1025 * t1031 + t1029 * 
     #t1021) * t1132) * t67 / 0.2E1 + (t1481 * (t1458 * t1467 + t1471 * 
     #t1459 + t1469 * t1463) * ((t1407 - t1414) * t67 / 0.2E1 + (t1414 -
     # t1424) * t67 / 0.2E1) - t622) * t123 / 0.2E1 + t629 + (t4 * (t148
     #0 * (t1497 + t1498 + t1499) / 0.2E1 + t671 / 0.2E1) * t1416 - t680
     #) * t123 + (t1481 * (t1467 * t1474 + t1459 * t1465 + t1463 * t1461
     #) * ((t1512 - t1414) * t176 / 0.2E1 + (t1414 - t1515) * t176 / 0.2
     #E1) - t705) * t123 / 0.2E1 + t714 + (t1548 * (t1525 * t1541 + t153
     #8 * t1532 + t1536 * t1528) * t1558 - t1562) * t176 / 0.2E1 + (t156
     #2 - t1589 * (t1566 * t1582 + t1579 * t1573 + t1577 * t1569) * t159
     #9) * t176 / 0.2E1 + (t1572 * ((t1512 - t696) * t123 / 0.2E1 + t813
     # / 0.2E1) - t1616) * t176 / 0.2E1 + (t1616 - t1586 * ((t1515 - t69
     #9) * t123 / 0.2E1 + t830 / 0.2E1)) * t176 / 0.2E1 + (t4 * (t1547 *
     # (t1633 + t1634 + t1635) / 0.2E1 + t1642 / 0.2E1) * t698 - t4 * (t
     #1642 / 0.2E1 + t1588 * (t1647 + t1648 + t1649) / 0.2E1) * t701) * 
     #t176
        t1672 = dy * (t902 / 0.2E1 + t905 / 0.2E1) / 0.2E1
        t1676 = sqrt(0.2E1 * t667 + 0.2E1 * t668 + 0.2E1 * t669 + 0.2E1 
     #* t672 + 0.2E1 * t673 + 0.2E1 * t674)
        t1680 = t680 + t679 * dt * t902 / 0.2E1 + cc * t678 * (t900 + dt
     # * t1658 * t610 / 0.2E1 - dy * ((ut(i,t1406,k,n) - t900) * t123 / 
     #0.2E1 + t902 / 0.2E1) / 0.2E1 - t2 - t868 - t1672) / t1676
        t1683 = t56 * t709
        t1686 = t4 * (t611 * t695 / 0.2E1 + t1683 / 0.2E1)
        t1690 = ut(i,t120,t173,n)
        t1693 = ut(i,t120,t178,n)
        t1701 = t1686 * (t698 / 0.4E1 + t701 / 0.4E1 + t209 / 0.4E1 + t2
     #12 / 0.4E1) + t1686 * dt * ((t1690 - t900) * t176 / 0.4E1 + (t900 
     #- t1693) * t176 / 0.4E1 + t929 / 0.4E1 + t932 / 0.4E1) / 0.2E1
        t1706 = t4 * (t887 / 0.2E1 + t652 * t657 / 0.2E1)
        t1719 = t1706 * (t68 / 0.4E1 + t550 / 0.4E1 + t294 / 0.4E1 + t65
     #9 / 0.4E1) + t1706 * dt * (t511 / 0.4E1 + t871 / 0.4E1 + (t897 - t
     #903) * t67 / 0.4E1 + (t903 - t1324) * t67 / 0.4E1) / 0.2E1
        t1726 = t263 ** 2
        t1727 = t276 ** 2
        t1728 = t274 ** 2
        t1731 = t630 ** 2
        t1732 = t643 ** 2
        t1733 = t641 ** 2
        t1735 = t652 * (t1731 + t1732 + t1733)
        t1740 = t1059 ** 2
        t1741 = t1072 ** 2
        t1742 = t1070 ** 2
        t1751 = j - 2
        t1752 = u(t5,t1751,k,n)
        t1759 = u(i,t1751,k,n)
        t1761 = (t159 - t1759) * t123
        t1763 = t161 / 0.2E1 + t1761 / 0.2E1
        t1765 = t621 * t1763
        t1769 = u(t516,t1751,k,n)
        t1691 = t653 * (t630 * t646 + t643 * t637 + t641 * t633)
        t1790 = t1691 * t726
        t1803 = rx(i,t1751,k,0,0)
        t1804 = rx(i,t1751,k,1,1)
        t1806 = rx(i,t1751,k,2,2)
        t1808 = rx(i,t1751,k,1,2)
        t1810 = rx(i,t1751,k,2,1)
        t1812 = rx(i,t1751,k,1,0)
        t1814 = rx(i,t1751,k,0,2)
        t1816 = rx(i,t1751,k,0,1)
        t1819 = rx(i,t1751,k,2,0)
        t1825 = 0.1E1 / (t1804 * t1803 * t1806 - t1803 * t1808 * t1810 +
     # t1812 * t1810 * t1814 - t1812 * t1816 * t1806 + t1819 * t1816 * t
     #1808 - t1819 * t1804 * t1814)
        t1826 = t4 * t1825
        t1842 = t1812 ** 2
        t1843 = t1804 ** 2
        t1844 = t1808 ** 2
        t1857 = u(i,t1751,t173,n)
        t1860 = u(i,t1751,t178,n)
        t1870 = rx(i,t125,t173,0,0)
        t1871 = rx(i,t125,t173,1,1)
        t1873 = rx(i,t125,t173,2,2)
        t1875 = rx(i,t125,t173,1,2)
        t1877 = rx(i,t125,t173,2,1)
        t1879 = rx(i,t125,t173,1,0)
        t1881 = rx(i,t125,t173,0,2)
        t1883 = rx(i,t125,t173,0,1)
        t1886 = rx(i,t125,t173,2,0)
        t1892 = 0.1E1 / (t1871 * t1870 * t1873 - t1870 * t1875 * t1877 +
     # t1879 * t1877 * t1881 - t1879 * t1883 * t1873 + t1886 * t1883 * t
     #1875 - t1886 * t1871 * t1881)
        t1893 = t4 * t1892
        t1903 = (t354 - t719) * t67 / 0.2E1 + (t719 - t1148) * t67 / 0.2
     #E1
        t1907 = t1691 * t661
        t1911 = rx(i,t125,t178,0,0)
        t1912 = rx(i,t125,t178,1,1)
        t1914 = rx(i,t125,t178,2,2)
        t1916 = rx(i,t125,t178,1,2)
        t1918 = rx(i,t125,t178,2,1)
        t1920 = rx(i,t125,t178,1,0)
        t1922 = rx(i,t125,t178,0,2)
        t1924 = rx(i,t125,t178,0,1)
        t1927 = rx(i,t125,t178,2,0)
        t1933 = 0.1E1 / (t1911 * t1912 * t1914 - t1911 * t1916 * t1918 +
     # t1920 * t1918 * t1922 - t1920 * t1924 * t1914 + t1927 * t1924 * t
     #1916 - t1927 * t1912 * t1922)
        t1934 = t4 * t1933
        t1944 = (t357 - t722) * t67 / 0.2E1 + (t722 - t1151) * t67 / 0.2
     #E1
        t1961 = t677 * t1763
        t1978 = t1886 ** 2
        t1979 = t1877 ** 2
        t1980 = t1873 ** 2
        t1983 = t646 ** 2
        t1984 = t637 ** 2
        t1985 = t633 ** 2
        t1987 = t652 * (t1983 + t1984 + t1985)
        t1992 = t1927 ** 2
        t1993 = t1918 ** 2
        t1994 = t1914 ** 2
        t1902 = t1893 * (t1879 * t1886 + t1871 * t1877 + t1875 * t1873)
        t1917 = t1934 * (t1920 * t1927 + t1912 * t1918 + t1916 * t1914)
        t2003 = (t4 * (t285 * (t1726 + t1727 + t1728) / 0.2E1 + t1735 / 
     #0.2E1) * t294 - t4 * (t1735 / 0.2E1 + t1081 * (t1740 + t1741 + t17
     #42) / 0.2E1) * t659) * t67 + (t364 * (t143 / 0.2E1 + (t141 - t1752
     #) * t123 / 0.2E1) - t1765) * t67 / 0.2E1 + (t1765 - t1143 * (t564 
     #/ 0.2E1 + (t562 - t1769) * t123 / 0.2E1)) * t67 / 0.2E1 + (t286 * 
     #(t263 * t279 + t276 * t270 + t274 * t266) * t361 - t1790) * t67 / 
     #0.2E1 + (t1790 - t1082 * (t1059 * t1075 + t1066 * t1072 + t1070 * 
     #t1062) * t1155) * t67 / 0.2E1 + t666 + (t663 - t1826 * (t1803 * t1
     #812 + t1816 * t1804 + t1814 * t1808) * ((t1752 - t1759) * t67 / 0.
     #2E1 + (t1759 - t1769) * t67 / 0.2E1)) * t123 / 0.2E1 + (t689 - t4 
     #* (t685 / 0.2E1 + t1825 * (t1842 + t1843 + t1844) / 0.2E1) * t1761
     #) * t123 + t731 + (t728 - t1826 * (t1812 * t1819 + t1804 * t1810 +
     # t1808 * t1806) * ((t1857 - t1759) * t176 / 0.2E1 + (t1759 - t1860
     #) * t176 / 0.2E1)) * t123 / 0.2E1 + (t1893 * (t1870 * t1886 + t188
     #3 * t1877 + t1881 * t1873) * t1903 - t1907) * t176 / 0.2E1 + (t190
     #7 - t1934 * (t1911 * t1927 + t1924 * t1918 + t1922 * t1914) * t194
     #4) * t176 / 0.2E1 + (t1902 * (t815 / 0.2E1 + (t719 - t1857) * t123
     # / 0.2E1) - t1961) * t176 / 0.2E1 + (t1961 - t1917 * (t832 / 0.2E1
     # + (t722 - t1860) * t123 / 0.2E1)) * t176 / 0.2E1 + (t4 * (t1892 *
     # (t1978 + t1979 + t1980) / 0.2E1 + t1987 / 0.2E1) * t721 - t4 * (t
     #1987 / 0.2E1 + t1933 * (t1992 + t1993 + t1994) / 0.2E1) * t724) * 
     #t176
        t2017 = sqrt(0.2E1 * t672 + 0.2E1 * t673 + 0.2E1 * t674 + 0.2E1 
     #* t681 + 0.2E1 * t682 + 0.2E1 * t683)
        t2021 = t689 + t688 * dt * t905 / 0.2E1 + cc * t687 * (t2 + t868
     # - t1672 - t903 - dt * t2003 * t651 / 0.2E1 - dy * (t905 / 0.2E1 +
     # (t903 - ut(i,t1751,k,n)) * t123 / 0.2E1) / 0.2E1) / t2017
        t2026 = t4 * (t1683 / 0.2E1 + t652 * t718 / 0.2E1)
        t2030 = ut(i,t125,t173,n)
        t2033 = ut(i,t125,t178,n)
        t2041 = t2026 * (t209 / 0.4E1 + t212 / 0.4E1 + t721 / 0.4E1 + t7
     #24 / 0.4E1) + t2026 * dt * (t929 / 0.4E1 + t932 / 0.4E1 + (t2030 -
     # t903) * t176 / 0.4E1 + (t903 - t2033) * t176 / 0.4E1) / 0.2E1
        t2050 = t4 * (t754 * t759 / 0.2E1 + t914 / 0.2E1)
        t2063 = t2050 * (t398 / 0.4E1 + t761 / 0.4E1 + t68 / 0.4E1 + t55
     #0 / 0.4E1) + t2050 * dt * ((t921 - t927) * t67 / 0.4E1 + (t927 - t
     #1341) * t67 / 0.4E1 + t511 / 0.4E1 + t871 / 0.4E1) / 0.2E1
        t2068 = t4 * (t754 * t811 / 0.2E1 + t1683 / 0.2E1)
        t2081 = t2068 * (t813 / 0.4E1 + t815 / 0.4E1 + t158 / 0.4E1 + t1
     #61 / 0.4E1) + t2068 * dt * ((t1690 - t927) * t123 / 0.4E1 + (t927 
     #- t2030) * t123 / 0.4E1 + t902 / 0.4E1 + t905 / 0.4E1) / 0.2E1
        t2088 = t367 ** 2
        t2089 = t380 ** 2
        t2090 = t378 ** 2
        t2093 = t732 ** 2
        t2094 = t745 ** 2
        t2095 = t743 ** 2
        t2097 = t754 * (t2093 + t2094 + t2095)
        t2102 = t1161 ** 2
        t2103 = t1174 ** 2
        t2104 = t1172 ** 2
        t2036 = t755 * (t732 * t741 + t745 * t733 + t743 * t737)
        t2124 = t2036 * t817
        t2137 = k + 2
        t2138 = u(t5,j,t2137,n)
        t2145 = u(i,j,t2137,n)
        t2147 = (t2145 - t207) * t176
        t2149 = t2147 / 0.2E1 + t209 / 0.2E1
        t2151 = t717 * t2149
        t2155 = u(t516,j,t2137,n)
        t2172 = t2036 * t763
        t2185 = t1534 ** 2
        t2186 = t1526 ** 2
        t2187 = t1530 ** 2
        t2190 = t741 ** 2
        t2191 = t733 ** 2
        t2192 = t737 ** 2
        t2194 = t754 * (t2190 + t2191 + t2192)
        t2199 = t1879 ** 2
        t2200 = t1871 ** 2
        t2201 = t1875 ** 2
        t2210 = u(i,t120,t2137,n)
        t2218 = t775 * t2149
        t2222 = u(i,t125,t2137,n)
        t2232 = rx(i,j,t2137,0,0)
        t2233 = rx(i,j,t2137,1,1)
        t2235 = rx(i,j,t2137,2,2)
        t2237 = rx(i,j,t2137,1,2)
        t2239 = rx(i,j,t2137,2,1)
        t2241 = rx(i,j,t2137,1,0)
        t2243 = rx(i,j,t2137,0,2)
        t2245 = rx(i,j,t2137,0,1)
        t2248 = rx(i,j,t2137,2,0)
        t2254 = 0.1E1 / (t2232 * t2233 * t2235 - t2232 * t2237 * t2239 +
     # t2241 * t2239 * t2243 - t2241 * t2245 * t2235 + t2248 * t2245 * t
     #2237 - t2248 * t2233 * t2243)
        t2255 = t4 * t2254
        t2286 = t2248 ** 2
        t2287 = t2239 ** 2
        t2288 = t2235 ** 2
        t2297 = (t4 * (t389 * (t2088 + t2089 + t2090) / 0.2E1 + t2097 / 
     #0.2E1) * t398 - t4 * (t2097 / 0.2E1 + t1183 * (t2102 + t2103 + t21
     #04) / 0.2E1) * t761) * t67 + (t390 * (t367 * t376 + t380 * t368 + 
     #t378 * t372) * t456 - t2124) * t67 / 0.2E1 + (t2124 - t1184 * (t11
     #61 * t1170 + t1174 * t1162 + t1166 * t1172) * t1246) * t67 / 0.2E1
     # + (t428 * ((t2138 - t190) * t176 / 0.2E1 + t192 / 0.2E1) - t2151)
     # * t67 / 0.2E1 + (t2151 - t1202 * ((t2155 - t576) * t176 / 0.2E1 +
     # t578 / 0.2E1)) * t67 / 0.2E1 + (t1548 * (t1525 * t1534 + t1538 * 
     #t1526 + t1536 * t1530) * t1558 - t2172) * t123 / 0.2E1 + (t2172 - 
     #t1893 * (t1870 * t1879 + t1883 * t1871 + t1881 * t1875) * t1903) *
     # t123 / 0.2E1 + (t4 * (t1547 * (t2185 + t2186 + t2187) / 0.2E1 + t
     #2194 / 0.2E1) * t813 - t4 * (t2194 / 0.2E1 + t1892 * (t2199 + t220
     #0 + t2201) / 0.2E1) * t815) * t123 + (t1572 * ((t2210 - t696) * t1
     #76 / 0.2E1 + t698 / 0.2E1) - t2218) * t123 / 0.2E1 + (t2218 - t190
     #2 * ((t2222 - t719) * t176 / 0.2E1 + t721 / 0.2E1)) * t123 / 0.2E1
     # + (t2255 * (t2232 * t2248 + t2245 * t2239 + t2243 * t2235) * ((t2
     #138 - t2145) * t67 / 0.2E1 + (t2145 - t2155) * t67 / 0.2E1) - t765
     #) * t176 / 0.2E1 + t770 + (t2255 * (t2241 * t2248 + t2233 * t2239 
     #+ t2237 * t2235) * ((t2210 - t2145) * t123 / 0.2E1 + (t2145 - t222
     #2) * t123 / 0.2E1) - t819) * t176 / 0.2E1 + t824 + (t4 * (t2254 * 
     #(t2286 + t2287 + t2288) / 0.2E1 + t844 / 0.2E1) * t2147 - t853) * 
     #t176
        t2311 = dz * (t929 / 0.2E1 + t932 / 0.2E1) / 0.2E1
        t2315 = sqrt(0.2E1 * t840 + 0.2E1 * t841 + 0.2E1 * t842 + 0.2E1 
     #* t845 + 0.2E1 * t846 + 0.2E1 * t847)
        t2319 = t853 + t852 * dt * t929 / 0.2E1 + cc * t851 * (t927 + dt
     # * t2297 * t753 / 0.2E1 - dz * ((ut(i,j,t2137,n) - t927) * t176 / 
     #0.2E1 + t929 / 0.2E1) / 0.2E1 - t2 - t868 - t2311) / t2315
        t2324 = t4 * (t914 / 0.2E1 + t793 * t798 / 0.2E1)
        t2337 = t2324 * (t68 / 0.4E1 + t550 / 0.4E1 + t439 / 0.4E1 + t80
     #0 / 0.4E1) + t2324 * dt * (t511 / 0.4E1 + t871 / 0.4E1 + (t924 - t
     #930) * t67 / 0.4E1 + (t930 - t1344) * t67 / 0.4E1) / 0.2E1
        t2342 = t4 * (t1683 / 0.2E1 + t793 * t828 / 0.2E1)
        t2355 = t2342 * (t158 / 0.4E1 + t161 / 0.4E1 + t830 / 0.4E1 + t8
     #32 / 0.4E1) + t2342 * dt * (t902 / 0.4E1 + t905 / 0.4E1 + (t1693 -
     # t930) * t123 / 0.4E1 + (t930 - t2033) * t123 / 0.4E1) / 0.2E1
        t2362 = t408 ** 2
        t2363 = t421 ** 2
        t2364 = t419 ** 2
        t2367 = t771 ** 2
        t2368 = t784 ** 2
        t2369 = t782 ** 2
        t2371 = t793 * (t2367 + t2368 + t2369)
        t2376 = t1200 ** 2
        t2377 = t1213 ** 2
        t2378 = t1211 ** 2
        t2296 = t794 * (t771 * t780 + t784 * t772 + t782 * t776)
        t2398 = t2296 * t834
        t2411 = k - 2
        t2412 = u(t5,j,t2411,n)
        t2419 = u(i,j,t2411,n)
        t2421 = (t210 - t2419) * t176
        t2423 = t212 / 0.2E1 + t2421 / 0.2E1
        t2425 = t757 * t2423
        t2429 = u(t516,j,t2411,n)
        t2446 = t2296 * t802
        t2459 = t1575 ** 2
        t2460 = t1567 ** 2
        t2461 = t1571 ** 2
        t2464 = t780 ** 2
        t2465 = t772 ** 2
        t2466 = t776 ** 2
        t2468 = t793 * (t2464 + t2465 + t2466)
        t2473 = t1920 ** 2
        t2474 = t1912 ** 2
        t2475 = t1916 ** 2
        t2484 = u(i,t120,t2411,n)
        t2492 = t790 * t2423
        t2496 = u(i,t125,t2411,n)
        t2506 = rx(i,j,t2411,0,0)
        t2507 = rx(i,j,t2411,1,1)
        t2509 = rx(i,j,t2411,2,2)
        t2511 = rx(i,j,t2411,1,2)
        t2513 = rx(i,j,t2411,2,1)
        t2515 = rx(i,j,t2411,1,0)
        t2517 = rx(i,j,t2411,0,2)
        t2519 = rx(i,j,t2411,0,1)
        t2522 = rx(i,j,t2411,2,0)
        t2528 = 0.1E1 / (t2506 * t2507 * t2509 - t2513 * t2511 * t2506 +
     # t2515 * t2513 * t2517 - t2515 * t2519 * t2509 + t2522 * t2519 * t
     #2511 - t2522 * t2507 * t2517)
        t2529 = t4 * t2528
        t2560 = t2522 ** 2
        t2561 = t2513 ** 2
        t2562 = t2509 ** 2
        t2571 = (t4 * (t430 * (t2362 + t2363 + t2364) / 0.2E1 + t2371 / 
     #0.2E1) * t439 - t4 * (t2371 / 0.2E1 + t1222 * (t2376 + t2377 + t23
     #78) / 0.2E1) * t800) * t67 + (t431 * (t408 * t417 + t421 * t409 + 
     #t419 * t413) * t473 - t2398) * t67 / 0.2E1 + (t2398 - t1223 * (t12
     #00 * t1209 + t1213 * t1201 + t1205 * t1211) * t1263) * t67 / 0.2E1
     # + (t441 * (t195 / 0.2E1 + (t193 - t2412) * t176 / 0.2E1) - t2425)
     # * t67 / 0.2E1 + (t2425 - t1218 * (t581 / 0.2E1 + (t579 - t2429) *
     # t176 / 0.2E1)) * t67 / 0.2E1 + (t1589 * (t1566 * t1575 + t1579 * 
     #t1567 + t1577 * t1571) * t1599 - t2446) * t123 / 0.2E1 + (t2446 - 
     #t1934 * (t1911 * t1920 + t1924 * t1912 + t1922 * t1916) * t1944) *
     # t123 / 0.2E1 + (t4 * (t1588 * (t2459 + t2460 + t2461) / 0.2E1 + t
     #2468 / 0.2E1) * t830 - t4 * (t2468 / 0.2E1 + t1933 * (t2473 + t247
     #4 + t2475) / 0.2E1) * t832) * t123 + (t1586 * (t701 / 0.2E1 + (t69
     #9 - t2484) * t176 / 0.2E1) - t2492) * t123 / 0.2E1 + (t2492 - t191
     #7 * (t724 / 0.2E1 + (t722 - t2496) * t176 / 0.2E1)) * t123 / 0.2E1
     # + t807 + (t804 - t2529 * (t2506 * t2522 + t2519 * t2513 + t2517 *
     # t2509) * ((t2412 - t2419) * t67 / 0.2E1 + (t2419 - t2429) * t67 /
     # 0.2E1)) * t176 / 0.2E1 + t839 + (t836 - t2529 * (t2515 * t2522 + 
     #t2507 * t2513 + t2511 * t2509) * ((t2484 - t2419) * t123 / 0.2E1 +
     # (t2419 - t2496) * t123 / 0.2E1)) * t176 / 0.2E1 + (t862 - t4 * (t
     #858 / 0.2E1 + t2528 * (t2560 + t2561 + t2562) / 0.2E1) * t2421) * 
     #t176
        t2585 = sqrt(0.2E1 * t845 + 0.2E1 * t846 + 0.2E1 * t847 + 0.2E1 
     #* t854 + 0.2E1 * t855 + 0.2E1 * t856)
        t2589 = t862 + t861 * dt * t932 / 0.2E1 + cc * t860 * (t2 + t868
     # - t2311 - t930 - dt * t2571 * t792 / 0.2E1 - dz * (t932 / 0.2E1 +
     # (t930 - ut(i,j,t2411,n)) * t176 / 0.2E1) / 0.2E1) / t2585

        unew(i,j,k) = t1 + dt * t2 + (t883 * t884 / 0.2E1 + t911 * t8
     #84 / 0.2E1 + t938 * t884 / 0.2E1 - t1312 * t884 / 0.2E1 - t1332 * 
     #t884 / 0.2E1 - t1352 * t884 / 0.2E1) * t55 * t67 + (t1374 * t884 /
     # 0.2E1 + t1680 * t884 / 0.2E1 + t1701 * t884 / 0.2E1 - t1719 * t88
     #4 / 0.2E1 - t2021 * t884 / 0.2E1 - t2041 * t884 / 0.2E1) * t55 * t
     #123 + (t2063 * t884 / 0.2E1 + t2081 * t884 / 0.2E1 + t2319 * t884 
     #/ 0.2E1 - t2337 * t884 / 0.2E1 - t2355 * t884 / 0.2E1 - t2589 * t8
     #84 / 0.2E1) * t55 * t176

        utnew(i,j,k) = t2 + (t883 * dt + t911 * dt + t938 *
     # dt - t1312 * dt - t1332 * dt - t1352 * dt) * t55 * t67 + (t1374 *
     # dt + t1680 * dt + t1701 * dt - t1719 * dt - t2021 * dt - t2041 * 
     #dt) * t55 * t123 + (t2063 * dt + t2081 * dt + t2319 * dt - t2337 *
     # dt - t2355 * dt - t2589 * dt) * t55 * t176

c        blah = array(int(t1 + dt * t2 + (t883 * t884 / 0.2E1 + t911 * t8
c     #84 / 0.2E1 + t938 * t884 / 0.2E1 - t1312 * t884 / 0.2E1 - t1332 * 
c     #t884 / 0.2E1 - t1352 * t884 / 0.2E1) * t55 * t67 + (t1374 * t884 /
c     # 0.2E1 + t1680 * t884 / 0.2E1 + t1701 * t884 / 0.2E1 - t1719 * t88
c     #4 / 0.2E1 - t2021 * t884 / 0.2E1 - t2041 * t884 / 0.2E1) * t55 * t
c     #123 + (t2063 * t884 / 0.2E1 + t2081 * t884 / 0.2E1 + t2319 * t884 
c     #/ 0.2E1 - t2337 * t884 / 0.2E1 - t2355 * t884 / 0.2E1 - t2589 * t8
c     #84 / 0.2E1) * t55 * t176),int(t2 + (t883 * dt + t911 * dt + t938 *
c     # dt - t1312 * dt - t1332 * dt - t1352 * dt) * t55 * t67 + (t1374 *
c     # dt + t1680 * dt + t1701 * dt - t1719 * dt - t2021 * dt - t2041 * 
c     #dt) * t55 * t123 + (t2063 * dt + t2081 * dt + t2319 * dt - t2337 *
c     # dt - t2355 * dt - t2589 * dt) * t55 * t176))

        return
      end
