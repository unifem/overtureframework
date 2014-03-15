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
        real t1008
        real t1009
        real t101
        real t1012
        real t102
        real t1022
        real t1023
        real t1025
        real t1027
        real t1029
        real t103
        real t1031
        real t1033
        real t1035
        real t1038
        real t1044
        real t1045
        real t105
        real t1057
        real t1059
        real t1063
        real t1064
        real t1066
        real t1068
        real t1070
        real t1072
        real t1074
        real t1076
        real t1079
        real t1085
        real t1086
        real t109
        real t11
        real t1100
        real t1101
        real t1102
        real t1105
        real t1106
        real t1107
        real t1109
        real t111
        real t1114
        real t1115
        real t1116
        real t1129
        real t1132
        real t1135
        real t1136
        real t114
        real t1144
        real t1147
        real t115
        real t1152
        real t1155
        real t1159
        real t1165
        real t1166
        real t1168
        real t1170
        real t1172
        real t1174
        real t1176
        real t1178
        real t1181
        real t1187
        real t1188
        integer t120
        real t1200
        real t1204
        real t1205
        real t1206
        real t1207
        real t1209
        real t121
        real t1211
        real t1213
        real t1215
        real t1217
        real t1220
        real t1222
        real t1226
        real t1227
        real t123
        integer t125
        real t1250
        real t1254
        real t126
        real t1267
        real t1273
        real t1274
        real t1275
        real t1278
        real t1279
        real t1280
        real t1282
        real t1287
        real t1288
        real t1289
        real t129
        real t1298
        real t13
        real t1314
        real t1318
        real t1323
        real t1327
        real t133
        real t1330
        real t1338
        real t1343
        real t1347
        real t1350
        real t1358
        real t1363
        real t1367
        real t137
        real t138
        real t1380
        real t1387
        real t1388
        real t1389
        real t139
        real t1392
        real t1393
        real t1394
        real t1396
        real t140
        real t1401
        real t1402
        real t1403
        real t141
        integer t1412
        real t1413
        real t1420
        real t1422
        real t1424
        real t1426
        real t143
        real t1430
        real t145
        real t1451
        real t1464
        real t1465
        real t1467
        real t1469
        real t147
        real t1471
        real t1473
        real t1475
        real t1477
        real t1480
        real t1486
        real t1487
        real t15
        real t1503
        real t1504
        real t1505
        real t151
        real t1518
        real t1521
        real t1531
        real t1532
        real t1534
        real t1536
        real t1538
        real t1540
        real t1542
        real t1544
        real t1547
        real t155
        real t1553
        real t1554
        real t156
        real t1564
        real t1568
        real t1572
        real t1573
        real t1575
        real t1577
        real t1578
        real t1579
        real t158
        real t1581
        real t1583
        real t1585
        real t1588
        real t159
        real t1592
        real t1594
        real t1595
        real t1605
        real t161
        real t1622
        real t163
        real t1639
        real t1640
        real t1641
        real t1644
        real t1645
        real t1646
        real t1648
        real t165
        real t1653
        real t1654
        real t1655
        real t1664
        real t168
        real t1680
        real t1684
        real t1688
        real t1691
        real t1694
        real t1698
        real t1699
        real t17
        real t1701
        real t1709
        real t1714
        real t1727
        integer t173
        real t1734
        real t1735
        real t1736
        real t1739
        real t174
        real t1740
        real t1741
        real t1743
        real t1748
        real t1749
        real t1750
        integer t1759
        real t176
        real t1760
        real t1767
        real t1769
        real t1771
        real t1773
        real t1777
        integer t178
        real t179
        real t1798
        real t1811
        real t1812
        real t1814
        real t1816
        real t1818
        real t1820
        real t1822
        real t1824
        real t1827
        real t1833
        real t1834
        real t1850
        real t1851
        real t1852
        real t1865
        real t1868
        real t1878
        real t1879
        real t1881
        real t1883
        real t1885
        real t1887
        real t1889
        real t189
        real t1891
        real t1894
        real t19
        real t190
        real t1900
        real t1901
        real t1910
        real t1911
        real t1915
        real t1919
        real t192
        real t1920
        real t1922
        real t1924
        real t1925
        real t1926
        real t1928
        real t193
        real t1930
        real t1932
        real t1935
        real t1941
        real t1942
        real t195
        real t1952
        real t1969
        real t197
        real t1986
        real t1987
        real t1988
        real t199
        real t1991
        real t1992
        real t1993
        real t1995
        real t2
        real t2000
        real t2001
        real t2002
        real t2011
        real t2027
        real t2031
        real t2036
        real t2040
        real t2043
        real t2046
        real t2051
        real t206
        real t2060
        real t207
        real t2073
        real t2078
        real t209
        real t2091
        real t2098
        real t2099
        real t210
        real t2100
        real t2103
        real t2104
        real t2105
        real t2107
        real t2112
        real t2113
        real t2114
        real t212
        real t2134
        real t214
        integer t2147
        real t2148
        real t2155
        real t2157
        real t2159
        real t216
        real t2161
        real t2165
        real t2182
        real t219
        real t2195
        real t2196
        real t2197
        real t22
        real t220
        real t2200
        real t2201
        real t2202
        real t2204
        real t2209
        real t221
        real t2210
        real t2211
        real t222
        real t2220
        real t2228
        real t223
        real t2232
        real t2242
        real t2243
        real t2245
        real t2247
        real t2249
        real t225
        real t2251
        real t2253
        real t2255
        real t2258
        real t2264
        real t2265
        real t227
        real t229
        real t2296
        real t2297
        real t2298
        real t2307
        real t2309
        real t231
        real t2323
        real t2327
        real t233
        real t2331
        real t2336
        real t2349
        real t2354
        real t236
        real t2367
        real t2374
        real t2375
        real t2376
        real t2379
        real t2380
        real t2381
        real t2383
        real t2388
        real t2389
        real t2390
        real t2410
        real t242
        integer t2423
        real t2424
        real t243
        real t2431
        real t2433
        real t2435
        real t2437
        real t2441
        real t2458
        real t2471
        real t2472
        real t2473
        real t2476
        real t2477
        real t2478
        real t2480
        real t2485
        real t2486
        real t2487
        real t2496
        real t2504
        real t2508
        real t251
        real t2518
        real t2519
        real t2521
        real t2523
        real t2525
        real t2527
        real t2529
        real t2531
        real t2534
        real t2540
        real t2541
        real t257
        real t2572
        real t2573
        real t2574
        real t2583
        real t259
        real t2599
        real t2603
        real t2610
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
        real t513
        integer t518
        real t519
        real t520
        real t521
        real t522
        real t524
        real t526
        real t528
        real t530
        real t532
        real t535
        real t537
        real t540
        real t541
        real t542
        real t543
        real t544
        real t546
        real t548
        real t549
        real t55
        real t550
        real t552
        real t553
        real t556
        real t56
        real t560
        real t561
        real t563
        real t564
        real t566
        real t568
        real t57
        real t570
        real t573
        real t577
        real t578
        real t579
        real t58
        real t580
        real t581
        real t583
        real t585
        real t587
        real t59
        real t590
        real t591
        real t592
        real t594
        real t596
        real t598
        real t6
        real t600
        real t602
        real t604
        real t607
        real t61
        real t612
        real t613
        real t614
        real t618
        real t619
        real t620
        real t622
        real t624
        real t626
        real t628
        real t63
        real t631
        real t632
        real t633
        real t635
        real t637
        real t639
        real t64
        real t641
        real t643
        real t645
        real t647
        real t648
        real t65
        real t652
        real t653
        real t654
        real t655
        real t659
        real t661
        real t663
        real t665
        real t668
        real t669
        real t67
        real t670
        real t671
        real t672
        real t673
        real t674
        real t675
        real t676
        real t678
        real t68
        real t680
        real t681
        real t682
        real t683
        real t684
        real t685
        real t687
        real t689
        real t69
        real t690
        real t691
        real t697
        real t698
        real t7
        real t70
        real t700
        real t701
        real t703
        real t705
        real t707
        real t711
        real t713
        real t716
        real t717
        real t720
        real t721
        real t723
        real t724
        real t726
        real t728
        real t730
        real t733
        real t734
        real t735
        real t737
        real t739
        real t741
        real t743
        real t745
        real t747
        real t750
        real t754
        real t755
        real t756
        real t757
        real t761
        real t763
        real t765
        real t767
        real t769
        integer t77
        real t771
        real t772
        real t773
        real t774
        real t776
        real t778
        real t78
        real t780
        real t782
        real t784
        real t786
        real t789
        real t79
        real t790
        real t794
        real t795
        real t796
        real t800
        real t802
        real t804
        real t806
        real t809
        real t81
        real t813
        real t815
        real t817
        real t819
        real t821
        real t823
        real t826
        real t83
        real t830
        real t832
        real t834
        real t836
        real t838
        real t841
        real t842
        real t843
        real t844
        real t846
        real t847
        real t848
        real t849
        real t85
        real t851
        real t853
        real t854
        real t855
        real t856
        real t857
        real t858
        real t860
        real t862
        real t863
        real t864
        real t867
        real t87
        real t872
        real t873
        real t875
        real t879
        real t883
        real t887
        real t888
        real t89
        real t891
        real t894
        real t898
        real t9
        real t901
        real t904
        real t906
        real t907
        real t909
        real t91
        real t915
        real t918
        real t921
        real t925
        real t928
        real t931
        real t933
        real t934
        real t936
        real t94
        real t942
        integer t949
        real t950
        real t951
        real t953
        real t955
        real t957
        real t959
        real t961
        real t963
        real t966
        real t972
        real t973
        real t974
        real t975
        real t981
        real t983
        real t987
        real t992
        real t995
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
        t513 = (t70 - t2) * t67
        t518 = i - 1
        t519 = rx(t518,j,k,0,0)
        t520 = rx(t518,j,k,1,1)
        t522 = rx(t518,j,k,2,2)
        t524 = rx(t518,j,k,1,2)
        t526 = rx(t518,j,k,2,1)
        t528 = rx(t518,j,k,1,0)
        t530 = rx(t518,j,k,0,2)
        t532 = rx(t518,j,k,0,1)
        t535 = rx(t518,j,k,2,0)
        t540 = t519 * t520 * t522 - t519 * t524 * t526 + t528 * t526 * t
     #530 - t528 * t532 * t522 + t535 * t532 * t524 - t535 * t520 * t530
        t541 = 0.1E1 / t540
        t542 = t519 ** 2
        t543 = t532 ** 2
        t544 = t530 ** 2
        t546 = t541 * (t542 + t543 + t544)
        t548 = t61 / 0.2E1 + t546 / 0.2E1
        t549 = t4 * t548
        t550 = u(t518,j,k,n)
        t552 = (t1 - t550) * t67
        t553 = t549 * t552
        t556 = t4 * t541
        t560 = t519 * t528 + t532 * t520 + t530 * t524
        t561 = u(t518,t120,k,n)
        t563 = (t561 - t550) * t123
        t564 = u(t518,t125,k,n)
        t566 = (t550 - t564) * t123
        t568 = t563 / 0.2E1 + t566 / 0.2E1
        t521 = t556 * t560
        t570 = t521 * t568
        t573 = (t165 - t570) * t67 / 0.2E1
        t577 = t519 * t535 + t532 * t526 + t530 * t522
        t578 = u(t518,j,t173,n)
        t580 = (t578 - t550) * t176
        t581 = u(t518,j,t178,n)
        t583 = (t550 - t581) * t176
        t585 = t580 / 0.2E1 + t583 / 0.2E1
        t537 = t556 * t577
        t587 = t537 * t585
        t590 = (t216 - t587) * t67 / 0.2E1
        t591 = rx(i,t120,k,0,0)
        t592 = rx(i,t120,k,1,1)
        t594 = rx(i,t120,k,2,2)
        t596 = rx(i,t120,k,1,2)
        t598 = rx(i,t120,k,2,1)
        t600 = rx(i,t120,k,1,0)
        t602 = rx(i,t120,k,0,2)
        t604 = rx(i,t120,k,0,1)
        t607 = rx(i,t120,k,2,0)
        t612 = t591 * t592 * t594 - t591 * t596 * t598 + t600 * t598 * t
     #602 - t600 * t604 * t594 + t607 * t604 * t596 - t607 * t592 * t602
        t613 = 0.1E1 / t612
        t614 = t4 * t613
        t618 = t591 * t600 + t604 * t592 + t602 * t596
        t620 = (t156 - t561) * t67
        t622 = t251 / 0.2E1 + t620 / 0.2E1
        t579 = t614 * t618
        t624 = t579 * t622
        t626 = t68 / 0.2E1 + t552 / 0.2E1
        t628 = t114 * t626
        t631 = (t624 - t628) * t123 / 0.2E1
        t632 = rx(i,t125,k,0,0)
        t633 = rx(i,t125,k,1,1)
        t635 = rx(i,t125,k,2,2)
        t637 = rx(i,t125,k,1,2)
        t639 = rx(i,t125,k,2,1)
        t641 = rx(i,t125,k,1,0)
        t643 = rx(i,t125,k,0,2)
        t645 = rx(i,t125,k,0,1)
        t648 = rx(i,t125,k,2,0)
        t653 = t632 * t633 * t635 - t632 * t637 * t639 + t641 * t639 * t
     #643 - t641 * t645 * t635 + t648 * t645 * t637 - t648 * t633 * t643
        t654 = 0.1E1 / t653
        t655 = t4 * t654
        t659 = t632 * t641 + t645 * t633 + t643 * t637
        t661 = (t159 - t564) * t67
        t663 = t294 / 0.2E1 + t661 / 0.2E1
        t619 = t655 * t659
        t665 = t619 * t663
        t668 = (t628 - t665) * t123 / 0.2E1
        t669 = t600 ** 2
        t670 = t592 ** 2
        t671 = t596 ** 2
        t673 = t613 * (t669 + t670 + t671)
        t674 = t43 ** 2
        t675 = t35 ** 2
        t676 = t39 ** 2
        t678 = t56 * (t674 + t675 + t676)
        t680 = t673 / 0.2E1 + t678 / 0.2E1
        t681 = t4 * t680
        t682 = t681 * t158
        t683 = t641 ** 2
        t684 = t633 ** 2
        t685 = t637 ** 2
        t687 = t654 * (t683 + t684 + t685)
        t689 = t678 / 0.2E1 + t687 / 0.2E1
        t690 = t4 * t689
        t691 = t690 * t161
        t697 = t600 * t607 + t592 * t598 + t596 * t594
        t698 = u(i,t120,t173,n)
        t700 = (t698 - t156) * t176
        t701 = u(i,t120,t178,n)
        t703 = (t156 - t701) * t176
        t705 = t700 / 0.2E1 + t703 / 0.2E1
        t647 = t614 * t697
        t707 = t647 * t705
        t711 = t43 * t50 + t35 * t41 + t39 * t37
        t652 = t151 * t711
        t713 = t652 * t214
        t716 = (t707 - t713) * t123 / 0.2E1
        t720 = t641 * t648 + t633 * t639 + t637 * t635
        t721 = u(i,t125,t173,n)
        t723 = (t721 - t159) * t176
        t724 = u(i,t125,t178,n)
        t726 = (t159 - t724) * t176
        t728 = t723 / 0.2E1 + t726 / 0.2E1
        t672 = t655 * t720
        t730 = t672 * t728
        t733 = (t713 - t730) * t123 / 0.2E1
        t734 = rx(i,j,t173,0,0)
        t735 = rx(i,j,t173,1,1)
        t737 = rx(i,j,t173,2,2)
        t739 = rx(i,j,t173,1,2)
        t741 = rx(i,j,t173,2,1)
        t743 = rx(i,j,t173,1,0)
        t745 = rx(i,j,t173,0,2)
        t747 = rx(i,j,t173,0,1)
        t750 = rx(i,j,t173,2,0)
        t755 = t734 * t735 * t737 - t734 * t739 * t741 + t743 * t741 * t
     #745 - t743 * t747 * t737 + t750 * t747 * t739 - t750 * t735 * t745
        t756 = 0.1E1 / t755
        t757 = t4 * t756
        t761 = t734 * t750 + t747 * t741 + t745 * t737
        t763 = (t207 - t578) * t67
        t765 = t398 / 0.2E1 + t763 / 0.2E1
        t717 = t757 * t761
        t767 = t717 * t765
        t769 = t139 * t626
        t772 = (t767 - t769) * t176 / 0.2E1
        t773 = rx(i,j,t178,0,0)
        t774 = rx(i,j,t178,1,1)
        t776 = rx(i,j,t178,2,2)
        t778 = rx(i,j,t178,1,2)
        t780 = rx(i,j,t178,2,1)
        t782 = rx(i,j,t178,1,0)
        t784 = rx(i,j,t178,0,2)
        t786 = rx(i,j,t178,0,1)
        t789 = rx(i,j,t178,2,0)
        t794 = t773 * t774 * t776 - t773 * t778 * t780 + t782 * t780 * t
     #784 - t782 * t786 * t776 + t789 * t786 * t778 - t789 * t774 * t784
        t795 = 0.1E1 / t794
        t796 = t4 * t795
        t800 = t773 * t789 + t786 * t780 + t784 * t776
        t802 = (t210 - t581) * t67
        t804 = t439 / 0.2E1 + t802 / 0.2E1
        t754 = t796 * t800
        t806 = t754 * t804
        t809 = (t769 - t806) * t176 / 0.2E1
        t813 = t743 * t750 + t735 * t741 + t739 * t737
        t815 = (t698 - t207) * t123
        t817 = (t207 - t721) * t123
        t819 = t815 / 0.2E1 + t817 / 0.2E1
        t771 = t757 * t813
        t821 = t771 * t819
        t823 = t652 * t163
        t826 = (t821 - t823) * t176 / 0.2E1
        t830 = t782 * t789 + t774 * t780 + t778 * t776
        t832 = (t701 - t210) * t123
        t834 = (t210 - t724) * t123
        t836 = t832 / 0.2E1 + t834 / 0.2E1
        t790 = t796 * t830
        t838 = t790 * t836
        t841 = (t823 - t838) * t176 / 0.2E1
        t842 = t750 ** 2
        t843 = t741 ** 2
        t844 = t737 ** 2
        t846 = t756 * (t842 + t843 + t844)
        t847 = t50 ** 2
        t848 = t41 ** 2
        t849 = t37 ** 2
        t851 = t56 * (t847 + t848 + t849)
        t853 = t846 / 0.2E1 + t851 / 0.2E1
        t854 = t4 * t853
        t855 = t854 * t209
        t856 = t789 ** 2
        t857 = t780 ** 2
        t858 = t776 ** 2
        t860 = t795 * (t856 + t857 + t858)
        t862 = t851 / 0.2E1 + t860 / 0.2E1
        t863 = t4 * t862
        t864 = t863 * t212
        t867 = (t69 - t553) * t67 + t168 + t573 + t219 + t590 + t631 + t
     #668 + (t682 - t691) * t123 + t716 + t733 + t772 + t809 + t826 + t8
     #41 + (t855 - t864) * t176
        t872 = dt * (t867 * t55 + src(i,j,k,nComp,n)) / 0.2E1
        t873 = ut(t518,j,k,n)
        t875 = (t2 - t873) * t67
        t879 = dx * (t513 / 0.2E1 + t875 / 0.2E1) / 0.2E1
        t883 = sqrt(0.2E1 * t29 + 0.2E1 * t30 + 0.2E1 * t31 + 0.2E1 * t5
     #7 + 0.2E1 * t58 + 0.2E1 * t59)
        t887 = t69 + t64 * dt * t513 / 0.2E1 + cc * t63 * (t70 + dt * (t
     #504 * t27 + src(t5,j,k,nComp,n)) / 0.2E1 - dx * ((ut(t77,j,k,n) - 
     #t70) * t67 / 0.2E1 + t513 / 0.2E1) / 0.2E1 - t2 - t872 - t879) / t
     #883
        t888 = dt ** 2
        t891 = t56 * t155
        t894 = t4 * (t28 * t137 / 0.2E1 + t891 / 0.2E1)
        t898 = ut(t5,t120,k,n)
        t901 = ut(t5,t125,k,n)
        t904 = ut(i,t120,k,n)
        t906 = (t904 - t2) * t123
        t907 = ut(i,t125,k,n)
        t909 = (t2 - t907) * t123
        t915 = t894 * (t140 / 0.4E1 + t143 / 0.4E1 + t158 / 0.4E1 + t161
     # / 0.4E1) + t894 * dt * ((t898 - t70) * t123 / 0.4E1 + (t70 - t901
     #) * t123 / 0.4E1 + t906 / 0.4E1 + t909 / 0.4E1) / 0.2E1
        t918 = t56 * t206
        t921 = t4 * (t28 * t189 / 0.2E1 + t918 / 0.2E1)
        t925 = ut(t5,j,t173,n)
        t928 = ut(t5,j,t178,n)
        t931 = ut(i,j,t173,n)
        t933 = (t931 - t2) * t176
        t934 = ut(i,j,t178,n)
        t936 = (t2 - t934) * t176
        t942 = t921 * (t192 / 0.4E1 + t195 / 0.4E1 + t209 / 0.4E1 + t212
     # / 0.4E1) + t921 * dt * ((t925 - t70) * t176 / 0.4E1 + (t70 - t928
     #) * t176 / 0.4E1 + t933 / 0.4E1 + t936 / 0.4E1) / 0.2E1
        t949 = i - 2
        t950 = rx(t949,j,k,0,0)
        t951 = rx(t949,j,k,1,1)
        t953 = rx(t949,j,k,2,2)
        t955 = rx(t949,j,k,1,2)
        t957 = rx(t949,j,k,2,1)
        t959 = rx(t949,j,k,1,0)
        t961 = rx(t949,j,k,0,2)
        t963 = rx(t949,j,k,0,1)
        t966 = rx(t949,j,k,2,0)
        t972 = 0.1E1 / (t950 * t951 * t953 - t950 * t955 * t957 + t959 *
     # t957 * t961 - t959 * t963 * t953 + t966 * t963 * t955 - t966 * t9
     #51 * t961)
        t973 = t950 ** 2
        t974 = t963 ** 2
        t975 = t961 ** 2
        t981 = u(t949,j,k,n)
        t983 = (t550 - t981) * t67
        t987 = t4 * t972
        t992 = u(t949,t120,k,n)
        t995 = u(t949,t125,k,n)
        t1009 = u(t949,j,t173,n)
        t1012 = u(t949,j,t178,n)
        t1022 = rx(t518,t120,k,0,0)
        t1023 = rx(t518,t120,k,1,1)
        t1025 = rx(t518,t120,k,2,2)
        t1027 = rx(t518,t120,k,1,2)
        t1029 = rx(t518,t120,k,2,1)
        t1031 = rx(t518,t120,k,1,0)
        t1033 = rx(t518,t120,k,0,2)
        t1035 = rx(t518,t120,k,0,1)
        t1038 = rx(t518,t120,k,2,0)
        t1044 = 0.1E1 / (t1022 * t1023 * t1025 - t1022 * t1027 * t1029 +
     # t1031 * t1029 * t1033 - t1031 * t1035 * t1025 + t1038 * t1035 * t
     #1027 - t1038 * t1023 * t1033)
        t1045 = t4 * t1044
        t1057 = t552 / 0.2E1 + t983 / 0.2E1
        t1059 = t521 * t1057
        t1063 = rx(t518,t125,k,0,0)
        t1064 = rx(t518,t125,k,1,1)
        t1066 = rx(t518,t125,k,2,2)
        t1068 = rx(t518,t125,k,1,2)
        t1070 = rx(t518,t125,k,2,1)
        t1072 = rx(t518,t125,k,1,0)
        t1074 = rx(t518,t125,k,0,2)
        t1076 = rx(t518,t125,k,0,1)
        t1079 = rx(t518,t125,k,2,0)
        t1085 = 0.1E1 / (t1063 * t1064 * t1066 - t1063 * t1068 * t1070 +
     # t1072 * t1070 * t1074 - t1072 * t1076 * t1066 + t1079 * t1076 * t
     #1068 - t1079 * t1064 * t1074)
        t1086 = t4 * t1085
        t1100 = t1031 ** 2
        t1101 = t1023 ** 2
        t1102 = t1027 ** 2
        t1105 = t528 ** 2
        t1106 = t520 ** 2
        t1107 = t524 ** 2
        t1109 = t541 * (t1105 + t1106 + t1107)
        t1114 = t1072 ** 2
        t1115 = t1064 ** 2
        t1116 = t1068 ** 2
        t1129 = u(t518,t120,t173,n)
        t1132 = u(t518,t120,t178,n)
        t1136 = (t1129 - t561) * t176 / 0.2E1 + (t561 - t1132) * t176 / 
     #0.2E1
        t1008 = t556 * (t528 * t535 + t520 * t526 + t524 * t522)
        t1144 = t1008 * t585
        t1152 = u(t518,t125,t173,n)
        t1155 = u(t518,t125,t178,n)
        t1159 = (t1152 - t564) * t176 / 0.2E1 + (t564 - t1155) * t176 / 
     #0.2E1
        t1165 = rx(t518,j,t173,0,0)
        t1166 = rx(t518,j,t173,1,1)
        t1168 = rx(t518,j,t173,2,2)
        t1170 = rx(t518,j,t173,1,2)
        t1172 = rx(t518,j,t173,2,1)
        t1174 = rx(t518,j,t173,1,0)
        t1176 = rx(t518,j,t173,0,2)
        t1178 = rx(t518,j,t173,0,1)
        t1181 = rx(t518,j,t173,2,0)
        t1187 = 0.1E1 / (t1165 * t1166 * t1168 - t1165 * t1170 * t1172 +
     # t1174 * t1172 * t1176 - t1174 * t1178 * t1168 + t1181 * t1178 * t
     #1170 - t1181 * t1166 * t1176)
        t1188 = t4 * t1187
        t1200 = t537 * t1057
        t1204 = rx(t518,j,t178,0,0)
        t1205 = rx(t518,j,t178,1,1)
        t1207 = rx(t518,j,t178,2,2)
        t1209 = rx(t518,j,t178,1,2)
        t1211 = rx(t518,j,t178,2,1)
        t1213 = rx(t518,j,t178,1,0)
        t1215 = rx(t518,j,t178,0,2)
        t1217 = rx(t518,j,t178,0,1)
        t1220 = rx(t518,j,t178,2,0)
        t1226 = 0.1E1 / (t1204 * t1205 * t1207 - t1204 * t1209 * t1211 +
     # t1213 * t1211 * t1215 - t1213 * t1217 * t1207 + t1220 * t1217 * t
     #1209 - t1220 * t1205 * t1215)
        t1227 = t4 * t1226
        t1250 = (t1129 - t578) * t123 / 0.2E1 + (t578 - t1152) * t123 / 
     #0.2E1
        t1254 = t1008 * t568
        t1267 = (t1132 - t581) * t123 / 0.2E1 + (t581 - t1155) * t123 / 
     #0.2E1
        t1273 = t1181 ** 2
        t1274 = t1172 ** 2
        t1275 = t1168 ** 2
        t1278 = t535 ** 2
        t1279 = t526 ** 2
        t1280 = t522 ** 2
        t1282 = t541 * (t1278 + t1279 + t1280)
        t1287 = t1220 ** 2
        t1288 = t1211 ** 2
        t1289 = t1207 ** 2
        t1135 = t1045 * (t1022 * t1031 + t1035 * t1023 + t1033 * t1027)
        t1147 = t1086 * (t1063 * t1072 + t1076 * t1064 + t1074 * t1068)
        t1206 = t1188 * (t1165 * t1181 + t1178 * t1172 + t1176 * t1168)
        t1222 = t1227 * (t1204 * t1220 + t1217 * t1211 + t1215 * t1207)
        t1298 = (t553 - t4 * (t546 / 0.2E1 + t972 * (t973 + t974 + t975)
     # / 0.2E1) * t983) * t67 + t573 + (t570 - t987 * (t950 * t959 + t96
     #3 * t951 + t961 * t955) * ((t992 - t981) * t123 / 0.2E1 + (t981 - 
     #t995) * t123 / 0.2E1)) * t67 / 0.2E1 + t590 + (t587 - t987 * (t950
     # * t966 + t963 * t957 + t961 * t953) * ((t1009 - t981) * t176 / 0.
     #2E1 + (t981 - t1012) * t176 / 0.2E1)) * t67 / 0.2E1 + (t1135 * (t6
     #20 / 0.2E1 + (t561 - t992) * t67 / 0.2E1) - t1059) * t123 / 0.2E1 
     #+ (t1059 - t1147 * (t661 / 0.2E1 + (t564 - t995) * t67 / 0.2E1)) *
     # t123 / 0.2E1 + (t4 * (t1044 * (t1100 + t1101 + t1102) / 0.2E1 + t
     #1109 / 0.2E1) * t563 - t4 * (t1109 / 0.2E1 + t1085 * (t1114 + t111
     #5 + t1116) / 0.2E1) * t566) * t123 + (t1045 * (t1031 * t1038 + t10
     #23 * t1029 + t1027 * t1025) * t1136 - t1144) * t123 / 0.2E1 + (t11
     #44 - t1086 * (t1072 * t1079 + t1064 * t1070 + t1068 * t1066) * t11
     #59) * t123 / 0.2E1 + (t1206 * (t763 / 0.2E1 + (t578 - t1009) * t67
     # / 0.2E1) - t1200) * t176 / 0.2E1 + (t1200 - t1222 * (t802 / 0.2E1
     # + (t581 - t1012) * t67 / 0.2E1)) * t176 / 0.2E1 + (t1188 * (t1174
     # * t1181 + t1172 * t1166 + t1170 * t1168) * t1250 - t1254) * t176 
     #/ 0.2E1 + (t1254 - t1227 * (t1213 * t1220 + t1211 * t1205 + t1209 
     #* t1207) * t1267) * t176 / 0.2E1 + (t4 * (t1187 * (t1273 + t1274 +
     # t1275) / 0.2E1 + t1282 / 0.2E1) * t580 - t4 * (t1282 / 0.2E1 + t1
     #226 * (t1287 + t1288 + t1289) / 0.2E1) * t583) * t176
        t1314 = sqrt(0.2E1 * t57 + 0.2E1 * t58 + 0.2E1 * t59 + 0.2E1 * t
     #542 + 0.2E1 * t543 + 0.2E1 * t544)
        t1318 = t553 + t549 * dt * t875 / 0.2E1 + cc * t548 * (t2 + t872
     # - t879 - t873 - dt * (t1298 * t540 + src(t518,j,k,nComp,n)) / 0.2
     #E1 - dx * (t875 / 0.2E1 + (t873 - ut(t949,j,k,n)) * t67 / 0.2E1) /
     # 0.2E1) / t1314
        t1323 = t4 * (t891 / 0.2E1 + t541 * t560 / 0.2E1)
        t1327 = ut(t518,t120,k,n)
        t1330 = ut(t518,t125,k,n)
        t1338 = t1323 * (t158 / 0.4E1 + t161 / 0.4E1 + t563 / 0.4E1 + t5
     #66 / 0.4E1) + t1323 * dt * (t906 / 0.4E1 + t909 / 0.4E1 + (t1327 -
     # t873) * t123 / 0.4E1 + (t873 - t1330) * t123 / 0.4E1) / 0.2E1
        t1343 = t4 * (t918 / 0.2E1 + t541 * t577 / 0.2E1)
        t1347 = ut(t518,j,t173,n)
        t1350 = ut(t518,j,t178,n)
        t1358 = t1343 * (t209 / 0.4E1 + t212 / 0.4E1 + t580 / 0.4E1 + t5
     #83 / 0.4E1) + t1343 * dt * (t933 / 0.4E1 + t936 / 0.4E1 + (t1347 -
     # t873) * t176 / 0.4E1 + (t873 - t1350) * t176 / 0.4E1) / 0.2E1
        t1367 = t4 * (t613 * t618 / 0.2E1 + t891 / 0.2E1)
        t1380 = t1367 * (t251 / 0.4E1 + t620 / 0.4E1 + t68 / 0.4E1 + t55
     #2 / 0.4E1) + t1367 * dt * ((t898 - t904) * t67 / 0.4E1 + (t904 - t
     #1327) * t67 / 0.4E1 + t513 / 0.4E1 + t875 / 0.4E1) / 0.2E1
        t1387 = t220 ** 2
        t1388 = t233 ** 2
        t1389 = t231 ** 2
        t1392 = t591 ** 2
        t1393 = t604 ** 2
        t1394 = t602 ** 2
        t1396 = t613 * (t1392 + t1393 + t1394)
        t1401 = t1022 ** 2
        t1402 = t1035 ** 2
        t1403 = t1033 ** 2
        t1412 = j + 2
        t1413 = u(t5,t1412,k,n)
        t1420 = u(i,t1412,k,n)
        t1422 = (t1420 - t156) * t123
        t1424 = t1422 / 0.2E1 + t158 / 0.2E1
        t1426 = t579 * t1424
        t1430 = u(t518,t1412,k,n)
        t1363 = t614 * (t591 * t607 + t604 * t598 + t602 * t594)
        t1451 = t1363 * t705
        t1464 = rx(i,t1412,k,0,0)
        t1465 = rx(i,t1412,k,1,1)
        t1467 = rx(i,t1412,k,2,2)
        t1469 = rx(i,t1412,k,1,2)
        t1471 = rx(i,t1412,k,2,1)
        t1473 = rx(i,t1412,k,1,0)
        t1475 = rx(i,t1412,k,0,2)
        t1477 = rx(i,t1412,k,0,1)
        t1480 = rx(i,t1412,k,2,0)
        t1486 = 0.1E1 / (t1464 * t1465 * t1467 - t1464 * t1469 * t1471 +
     # t1473 * t1471 * t1475 - t1473 * t1477 * t1467 + t1480 * t1477 * t
     #1469 - t1480 * t1465 * t1475)
        t1487 = t4 * t1486
        t1503 = t1473 ** 2
        t1504 = t1465 ** 2
        t1505 = t1469 ** 2
        t1518 = u(i,t1412,t173,n)
        t1521 = u(i,t1412,t178,n)
        t1531 = rx(i,t120,t173,0,0)
        t1532 = rx(i,t120,t173,1,1)
        t1534 = rx(i,t120,t173,2,2)
        t1536 = rx(i,t120,t173,1,2)
        t1538 = rx(i,t120,t173,2,1)
        t1540 = rx(i,t120,t173,1,0)
        t1542 = rx(i,t120,t173,0,2)
        t1544 = rx(i,t120,t173,0,1)
        t1547 = rx(i,t120,t173,2,0)
        t1553 = 0.1E1 / (t1532 * t1531 * t1534 - t1531 * t1536 * t1538 +
     # t1540 * t1538 * t1542 - t1540 * t1544 * t1534 + t1547 * t1544 * t
     #1536 - t1547 * t1532 * t1542)
        t1554 = t4 * t1553
        t1564 = (t331 - t698) * t67 / 0.2E1 + (t698 - t1129) * t67 / 0.2
     #E1
        t1568 = t1363 * t622
        t1572 = rx(i,t120,t178,0,0)
        t1573 = rx(i,t120,t178,1,1)
        t1575 = rx(i,t120,t178,2,2)
        t1577 = rx(i,t120,t178,1,2)
        t1579 = rx(i,t120,t178,2,1)
        t1581 = rx(i,t120,t178,1,0)
        t1583 = rx(i,t120,t178,0,2)
        t1585 = rx(i,t120,t178,0,1)
        t1588 = rx(i,t120,t178,2,0)
        t1594 = 0.1E1 / (t1572 * t1573 * t1575 - t1572 * t1577 * t1579 +
     # t1581 * t1579 * t1583 - t1581 * t1585 * t1575 + t1588 * t1585 * t
     #1577 - t1588 * t1573 * t1583)
        t1595 = t4 * t1594
        t1605 = (t334 - t701) * t67 / 0.2E1 + (t701 - t1132) * t67 / 0.2
     #E1
        t1622 = t647 * t1424
        t1639 = t1547 ** 2
        t1640 = t1538 ** 2
        t1641 = t1534 ** 2
        t1644 = t607 ** 2
        t1645 = t598 ** 2
        t1646 = t594 ** 2
        t1648 = t613 * (t1644 + t1645 + t1646)
        t1653 = t1588 ** 2
        t1654 = t1579 ** 2
        t1655 = t1575 ** 2
        t1578 = t1554 * (t1540 * t1547 + t1532 * t1538 + t1536 * t1534)
        t1592 = t1595 * (t1581 * t1588 + t1573 * t1579 + t1577 * t1575)
        t1664 = (t4 * (t242 * (t1387 + t1388 + t1389) / 0.2E1 + t1396 / 
     #0.2E1) * t251 - t4 * (t1396 / 0.2E1 + t1044 * (t1401 + t1402 + t14
     #03) / 0.2E1) * t620) * t67 + (t352 * ((t1413 - t138) * t123 / 0.2E
     #1 + t140 / 0.2E1) - t1426) * t67 / 0.2E1 + (t1426 - t1135 * ((t143
     #0 - t561) * t123 / 0.2E1 + t563 / 0.2E1)) * t67 / 0.2E1 + (t243 * 
     #(t220 * t236 + t233 * t227 + t231 * t223) * t338 - t1451) * t67 / 
     #0.2E1 + (t1451 - t1045 * (t1022 * t1038 + t1035 * t1029 + t1033 * 
     #t1025) * t1136) * t67 / 0.2E1 + (t1487 * (t1464 * t1473 + t1477 * 
     #t1465 + t1475 * t1469) * ((t1413 - t1420) * t67 / 0.2E1 + (t1420 -
     # t1430) * t67 / 0.2E1) - t624) * t123 / 0.2E1 + t631 + (t4 * (t148
     #6 * (t1503 + t1504 + t1505) / 0.2E1 + t673 / 0.2E1) * t1422 - t682
     #) * t123 + (t1487 * (t1473 * t1480 + t1465 * t1471 + t1469 * t1467
     #) * ((t1518 - t1420) * t176 / 0.2E1 + (t1420 - t1521) * t176 / 0.2
     #E1) - t707) * t123 / 0.2E1 + t716 + (t1554 * (t1531 * t1547 + t154
     #4 * t1538 + t1542 * t1534) * t1564 - t1568) * t176 / 0.2E1 + (t156
     #8 - t1595 * (t1572 * t1588 + t1585 * t1579 + t1583 * t1575) * t160
     #5) * t176 / 0.2E1 + (t1578 * ((t1518 - t698) * t123 / 0.2E1 + t815
     # / 0.2E1) - t1622) * t176 / 0.2E1 + (t1622 - t1592 * ((t1521 - t70
     #1) * t123 / 0.2E1 + t832 / 0.2E1)) * t176 / 0.2E1 + (t4 * (t1553 *
     # (t1639 + t1640 + t1641) / 0.2E1 + t1648 / 0.2E1) * t700 - t4 * (t
     #1648 / 0.2E1 + t1594 * (t1653 + t1654 + t1655) / 0.2E1) * t703) * 
     #t176
        t1680 = dy * (t906 / 0.2E1 + t909 / 0.2E1) / 0.2E1
        t1684 = sqrt(0.2E1 * t669 + 0.2E1 * t670 + 0.2E1 * t671 + 0.2E1 
     #* t674 + 0.2E1 * t675 + 0.2E1 * t676)
        t1688 = t682 + t681 * dt * t906 / 0.2E1 + cc * t680 * (t904 + dt
     # * (t1664 * t612 + src(i,t120,k,nComp,n)) / 0.2E1 - dy * ((ut(i,t1
     #412,k,n) - t904) * t123 / 0.2E1 + t906 / 0.2E1) / 0.2E1 - t2 - t87
     #2 - t1680) / t1684
        t1691 = t56 * t711
        t1694 = t4 * (t613 * t697 / 0.2E1 + t1691 / 0.2E1)
        t1698 = ut(i,t120,t173,n)
        t1701 = ut(i,t120,t178,n)
        t1709 = t1694 * (t700 / 0.4E1 + t703 / 0.4E1 + t209 / 0.4E1 + t2
     #12 / 0.4E1) + t1694 * dt * ((t1698 - t904) * t176 / 0.4E1 + (t904 
     #- t1701) * t176 / 0.4E1 + t933 / 0.4E1 + t936 / 0.4E1) / 0.2E1
        t1714 = t4 * (t891 / 0.2E1 + t654 * t659 / 0.2E1)
        t1727 = t1714 * (t68 / 0.4E1 + t552 / 0.4E1 + t294 / 0.4E1 + t66
     #1 / 0.4E1) + t1714 * dt * (t513 / 0.4E1 + t875 / 0.4E1 + (t901 - t
     #907) * t67 / 0.4E1 + (t907 - t1330) * t67 / 0.4E1) / 0.2E1
        t1734 = t263 ** 2
        t1735 = t276 ** 2
        t1736 = t274 ** 2
        t1739 = t632 ** 2
        t1740 = t645 ** 2
        t1741 = t643 ** 2
        t1743 = t654 * (t1739 + t1740 + t1741)
        t1748 = t1063 ** 2
        t1749 = t1076 ** 2
        t1750 = t1074 ** 2
        t1759 = j - 2
        t1760 = u(t5,t1759,k,n)
        t1767 = u(i,t1759,k,n)
        t1769 = (t159 - t1767) * t123
        t1771 = t161 / 0.2E1 + t1769 / 0.2E1
        t1773 = t619 * t1771
        t1777 = u(t518,t1759,k,n)
        t1699 = t655 * (t632 * t648 + t645 * t639 + t643 * t635)
        t1798 = t1699 * t728
        t1811 = rx(i,t1759,k,0,0)
        t1812 = rx(i,t1759,k,1,1)
        t1814 = rx(i,t1759,k,2,2)
        t1816 = rx(i,t1759,k,1,2)
        t1818 = rx(i,t1759,k,2,1)
        t1820 = rx(i,t1759,k,1,0)
        t1822 = rx(i,t1759,k,0,2)
        t1824 = rx(i,t1759,k,0,1)
        t1827 = rx(i,t1759,k,2,0)
        t1833 = 0.1E1 / (t1812 * t1811 * t1814 - t1811 * t1816 * t1818 +
     # t1820 * t1818 * t1822 - t1820 * t1824 * t1814 + t1827 * t1824 * t
     #1816 - t1827 * t1812 * t1822)
        t1834 = t4 * t1833
        t1850 = t1820 ** 2
        t1851 = t1812 ** 2
        t1852 = t1816 ** 2
        t1865 = u(i,t1759,t173,n)
        t1868 = u(i,t1759,t178,n)
        t1878 = rx(i,t125,t173,0,0)
        t1879 = rx(i,t125,t173,1,1)
        t1881 = rx(i,t125,t173,2,2)
        t1883 = rx(i,t125,t173,1,2)
        t1885 = rx(i,t125,t173,2,1)
        t1887 = rx(i,t125,t173,1,0)
        t1889 = rx(i,t125,t173,0,2)
        t1891 = rx(i,t125,t173,0,1)
        t1894 = rx(i,t125,t173,2,0)
        t1900 = 0.1E1 / (t1879 * t1878 * t1881 - t1878 * t1883 * t1885 +
     # t1887 * t1885 * t1889 - t1887 * t1891 * t1881 + t1894 * t1891 * t
     #1883 - t1894 * t1879 * t1889)
        t1901 = t4 * t1900
        t1911 = (t354 - t721) * t67 / 0.2E1 + (t721 - t1152) * t67 / 0.2
     #E1
        t1915 = t1699 * t663
        t1919 = rx(i,t125,t178,0,0)
        t1920 = rx(i,t125,t178,1,1)
        t1922 = rx(i,t125,t178,2,2)
        t1924 = rx(i,t125,t178,1,2)
        t1926 = rx(i,t125,t178,2,1)
        t1928 = rx(i,t125,t178,1,0)
        t1930 = rx(i,t125,t178,0,2)
        t1932 = rx(i,t125,t178,0,1)
        t1935 = rx(i,t125,t178,2,0)
        t1941 = 0.1E1 / (t1919 * t1920 * t1922 - t1919 * t1924 * t1926 +
     # t1928 * t1926 * t1930 - t1928 * t1932 * t1922 + t1935 * t1932 * t
     #1924 - t1935 * t1920 * t1930)
        t1942 = t4 * t1941
        t1952 = (t357 - t724) * t67 / 0.2E1 + (t724 - t1155) * t67 / 0.2
     #E1
        t1969 = t672 * t1771
        t1986 = t1894 ** 2
        t1987 = t1885 ** 2
        t1988 = t1881 ** 2
        t1991 = t648 ** 2
        t1992 = t639 ** 2
        t1993 = t635 ** 2
        t1995 = t654 * (t1991 + t1992 + t1993)
        t2000 = t1935 ** 2
        t2001 = t1926 ** 2
        t2002 = t1922 ** 2
        t1910 = t1901 * (t1887 * t1894 + t1879 * t1885 + t1883 * t1881)
        t1925 = t1942 * (t1928 * t1935 + t1920 * t1926 + t1924 * t1922)
        t2011 = (t4 * (t285 * (t1734 + t1735 + t1736) / 0.2E1 + t1743 / 
     #0.2E1) * t294 - t4 * (t1743 / 0.2E1 + t1085 * (t1748 + t1749 + t17
     #50) / 0.2E1) * t661) * t67 + (t364 * (t143 / 0.2E1 + (t141 - t1760
     #) * t123 / 0.2E1) - t1773) * t67 / 0.2E1 + (t1773 - t1147 * (t566 
     #/ 0.2E1 + (t564 - t1777) * t123 / 0.2E1)) * t67 / 0.2E1 + (t286 * 
     #(t263 * t279 + t276 * t270 + t274 * t266) * t361 - t1798) * t67 / 
     #0.2E1 + (t1798 - t1086 * (t1063 * t1079 + t1070 * t1076 + t1074 * 
     #t1066) * t1159) * t67 / 0.2E1 + t668 + (t665 - t1834 * (t1811 * t1
     #820 + t1824 * t1812 + t1822 * t1816) * ((t1760 - t1767) * t67 / 0.
     #2E1 + (t1767 - t1777) * t67 / 0.2E1)) * t123 / 0.2E1 + (t691 - t4 
     #* (t687 / 0.2E1 + t1833 * (t1850 + t1851 + t1852) / 0.2E1) * t1769
     #) * t123 + t733 + (t730 - t1834 * (t1820 * t1827 + t1812 * t1818 +
     # t1816 * t1814) * ((t1865 - t1767) * t176 / 0.2E1 + (t1767 - t1868
     #) * t176 / 0.2E1)) * t123 / 0.2E1 + (t1901 * (t1878 * t1894 + t189
     #1 * t1885 + t1889 * t1881) * t1911 - t1915) * t176 / 0.2E1 + (t191
     #5 - t1942 * (t1919 * t1935 + t1932 * t1926 + t1930 * t1922) * t195
     #2) * t176 / 0.2E1 + (t1910 * (t817 / 0.2E1 + (t721 - t1865) * t123
     # / 0.2E1) - t1969) * t176 / 0.2E1 + (t1969 - t1925 * (t834 / 0.2E1
     # + (t724 - t1868) * t123 / 0.2E1)) * t176 / 0.2E1 + (t4 * (t1900 *
     # (t1986 + t1987 + t1988) / 0.2E1 + t1995 / 0.2E1) * t723 - t4 * (t
     #1995 / 0.2E1 + t1941 * (t2000 + t2001 + t2002) / 0.2E1) * t726) * 
     #t176
        t2027 = sqrt(0.2E1 * t674 + 0.2E1 * t675 + 0.2E1 * t676 + 0.2E1 
     #* t683 + 0.2E1 * t684 + 0.2E1 * t685)
        t2031 = t691 + t690 * dt * t909 / 0.2E1 + cc * t689 * (t2 + t872
     # - t1680 - t907 - dt * (t2011 * t653 + src(i,t125,k,nComp,n)) / 0.
     #2E1 - dy * (t909 / 0.2E1 + (t907 - ut(i,t1759,k,n)) * t123 / 0.2E1
     #) / 0.2E1) / t2027
        t2036 = t4 * (t1691 / 0.2E1 + t654 * t720 / 0.2E1)
        t2040 = ut(i,t125,t173,n)
        t2043 = ut(i,t125,t178,n)
        t2051 = t2036 * (t209 / 0.4E1 + t212 / 0.4E1 + t723 / 0.4E1 + t7
     #26 / 0.4E1) + t2036 * dt * (t933 / 0.4E1 + t936 / 0.4E1 + (t2040 -
     # t907) * t176 / 0.4E1 + (t907 - t2043) * t176 / 0.4E1) / 0.2E1
        t2060 = t4 * (t756 * t761 / 0.2E1 + t918 / 0.2E1)
        t2073 = t2060 * (t398 / 0.4E1 + t763 / 0.4E1 + t68 / 0.4E1 + t55
     #2 / 0.4E1) + t2060 * dt * ((t925 - t931) * t67 / 0.4E1 + (t931 - t
     #1347) * t67 / 0.4E1 + t513 / 0.4E1 + t875 / 0.4E1) / 0.2E1
        t2078 = t4 * (t756 * t813 / 0.2E1 + t1691 / 0.2E1)
        t2091 = t2078 * (t815 / 0.4E1 + t817 / 0.4E1 + t158 / 0.4E1 + t1
     #61 / 0.4E1) + t2078 * dt * ((t1698 - t931) * t123 / 0.4E1 + (t931 
     #- t2040) * t123 / 0.4E1 + t906 / 0.4E1 + t909 / 0.4E1) / 0.2E1
        t2098 = t367 ** 2
        t2099 = t380 ** 2
        t2100 = t378 ** 2
        t2103 = t734 ** 2
        t2104 = t747 ** 2
        t2105 = t745 ** 2
        t2107 = t756 * (t2103 + t2104 + t2105)
        t2112 = t1165 ** 2
        t2113 = t1178 ** 2
        t2114 = t1176 ** 2
        t2046 = t757 * (t734 * t743 + t747 * t735 + t745 * t739)
        t2134 = t2046 * t819
        t2147 = k + 2
        t2148 = u(t5,j,t2147,n)
        t2155 = u(i,j,t2147,n)
        t2157 = (t2155 - t207) * t176
        t2159 = t2157 / 0.2E1 + t209 / 0.2E1
        t2161 = t717 * t2159
        t2165 = u(t518,j,t2147,n)
        t2182 = t2046 * t765
        t2195 = t1540 ** 2
        t2196 = t1532 ** 2
        t2197 = t1536 ** 2
        t2200 = t743 ** 2
        t2201 = t735 ** 2
        t2202 = t739 ** 2
        t2204 = t756 * (t2200 + t2201 + t2202)
        t2209 = t1887 ** 2
        t2210 = t1879 ** 2
        t2211 = t1883 ** 2
        t2220 = u(i,t120,t2147,n)
        t2228 = t771 * t2159
        t2232 = u(i,t125,t2147,n)
        t2242 = rx(i,j,t2147,0,0)
        t2243 = rx(i,j,t2147,1,1)
        t2245 = rx(i,j,t2147,2,2)
        t2247 = rx(i,j,t2147,1,2)
        t2249 = rx(i,j,t2147,2,1)
        t2251 = rx(i,j,t2147,1,0)
        t2253 = rx(i,j,t2147,0,2)
        t2255 = rx(i,j,t2147,0,1)
        t2258 = rx(i,j,t2147,2,0)
        t2264 = 0.1E1 / (t2242 * t2243 * t2245 - t2242 * t2247 * t2249 +
     # t2251 * t2249 * t2253 - t2251 * t2255 * t2245 + t2258 * t2255 * t
     #2247 - t2258 * t2243 * t2253)
        t2265 = t4 * t2264
        t2296 = t2258 ** 2
        t2297 = t2249 ** 2
        t2298 = t2245 ** 2
        t2307 = (t4 * (t389 * (t2098 + t2099 + t2100) / 0.2E1 + t2107 / 
     #0.2E1) * t398 - t4 * (t2107 / 0.2E1 + t1187 * (t2112 + t2113 + t21
     #14) / 0.2E1) * t763) * t67 + (t390 * (t367 * t376 + t380 * t368 + 
     #t378 * t372) * t456 - t2134) * t67 / 0.2E1 + (t2134 - t1188 * (t11
     #65 * t1174 + t1178 * t1166 + t1170 * t1176) * t1250) * t67 / 0.2E1
     # + (t428 * ((t2148 - t190) * t176 / 0.2E1 + t192 / 0.2E1) - t2161)
     # * t67 / 0.2E1 + (t2161 - t1206 * ((t2165 - t578) * t176 / 0.2E1 +
     # t580 / 0.2E1)) * t67 / 0.2E1 + (t1554 * (t1531 * t1540 + t1544 * 
     #t1532 + t1542 * t1536) * t1564 - t2182) * t123 / 0.2E1 + (t2182 - 
     #t1901 * (t1878 * t1887 + t1891 * t1879 + t1889 * t1883) * t1911) *
     # t123 / 0.2E1 + (t4 * (t1553 * (t2195 + t2196 + t2197) / 0.2E1 + t
     #2204 / 0.2E1) * t815 - t4 * (t2204 / 0.2E1 + t1900 * (t2209 + t221
     #0 + t2211) / 0.2E1) * t817) * t123 + (t1578 * ((t2220 - t698) * t1
     #76 / 0.2E1 + t700 / 0.2E1) - t2228) * t123 / 0.2E1 + (t2228 - t191
     #0 * ((t2232 - t721) * t176 / 0.2E1 + t723 / 0.2E1)) * t123 / 0.2E1
     # + (t2265 * (t2242 * t2258 + t2255 * t2249 + t2253 * t2245) * ((t2
     #148 - t2155) * t67 / 0.2E1 + (t2155 - t2165) * t67 / 0.2E1) - t767
     #) * t176 / 0.2E1 + t772 + (t2265 * (t2251 * t2258 + t2243 * t2249 
     #+ t2247 * t2245) * ((t2220 - t2155) * t123 / 0.2E1 + (t2155 - t223
     #2) * t123 / 0.2E1) - t821) * t176 / 0.2E1 + t826 + (t4 * (t2264 * 
     #(t2296 + t2297 + t2298) / 0.2E1 + t846 / 0.2E1) * t2157 - t855) * 
     #t176
        t2323 = dz * (t933 / 0.2E1 + t936 / 0.2E1) / 0.2E1
        t2327 = sqrt(0.2E1 * t842 + 0.2E1 * t843 + 0.2E1 * t844 + 0.2E1 
     #* t847 + 0.2E1 * t848 + 0.2E1 * t849)
        t2331 = t855 + t854 * dt * t933 / 0.2E1 + cc * t853 * (t931 + dt
     # * (t2307 * t755 + src(i,j,t173,nComp,n)) / 0.2E1 - dz * ((ut(i,j,
     #t2147,n) - t931) * t176 / 0.2E1 + t933 / 0.2E1) / 0.2E1 - t2 - t87
     #2 - t2323) / t2327
        t2336 = t4 * (t918 / 0.2E1 + t795 * t800 / 0.2E1)
        t2349 = t2336 * (t68 / 0.4E1 + t552 / 0.4E1 + t439 / 0.4E1 + t80
     #2 / 0.4E1) + t2336 * dt * (t513 / 0.4E1 + t875 / 0.4E1 + (t928 - t
     #934) * t67 / 0.4E1 + (t934 - t1350) * t67 / 0.4E1) / 0.2E1
        t2354 = t4 * (t1691 / 0.2E1 + t795 * t830 / 0.2E1)
        t2367 = t2354 * (t158 / 0.4E1 + t161 / 0.4E1 + t832 / 0.4E1 + t8
     #34 / 0.4E1) + t2354 * dt * (t906 / 0.4E1 + t909 / 0.4E1 + (t1701 -
     # t934) * t123 / 0.4E1 + (t934 - t2043) * t123 / 0.4E1) / 0.2E1
        t2374 = t408 ** 2
        t2375 = t421 ** 2
        t2376 = t419 ** 2
        t2379 = t773 ** 2
        t2380 = t786 ** 2
        t2381 = t784 ** 2
        t2383 = t795 * (t2379 + t2380 + t2381)
        t2388 = t1204 ** 2
        t2389 = t1217 ** 2
        t2390 = t1215 ** 2
        t2309 = t796 * (t773 * t782 + t786 * t774 + t784 * t778)
        t2410 = t2309 * t836
        t2423 = k - 2
        t2424 = u(t5,j,t2423,n)
        t2431 = u(i,j,t2423,n)
        t2433 = (t210 - t2431) * t176
        t2435 = t212 / 0.2E1 + t2433 / 0.2E1
        t2437 = t754 * t2435
        t2441 = u(t518,j,t2423,n)
        t2458 = t2309 * t804
        t2471 = t1581 ** 2
        t2472 = t1573 ** 2
        t2473 = t1577 ** 2
        t2476 = t782 ** 2
        t2477 = t774 ** 2
        t2478 = t778 ** 2
        t2480 = t795 * (t2476 + t2477 + t2478)
        t2485 = t1928 ** 2
        t2486 = t1920 ** 2
        t2487 = t1924 ** 2
        t2496 = u(i,t120,t2423,n)
        t2504 = t790 * t2435
        t2508 = u(i,t125,t2423,n)
        t2518 = rx(i,j,t2423,0,0)
        t2519 = rx(i,j,t2423,1,1)
        t2521 = rx(i,j,t2423,2,2)
        t2523 = rx(i,j,t2423,1,2)
        t2525 = rx(i,j,t2423,2,1)
        t2527 = rx(i,j,t2423,1,0)
        t2529 = rx(i,j,t2423,0,2)
        t2531 = rx(i,j,t2423,0,1)
        t2534 = rx(i,j,t2423,2,0)
        t2540 = 0.1E1 / (t2519 * t2518 * t2521 - t2518 * t2523 * t2525 +
     # t2527 * t2525 * t2529 - t2527 * t2531 * t2521 + t2534 * t2531 * t
     #2523 - t2534 * t2519 * t2529)
        t2541 = t4 * t2540
        t2572 = t2534 ** 2
        t2573 = t2525 ** 2
        t2574 = t2521 ** 2
        t2583 = (t4 * (t430 * (t2374 + t2375 + t2376) / 0.2E1 + t2383 / 
     #0.2E1) * t439 - t4 * (t2383 / 0.2E1 + t1226 * (t2388 + t2389 + t23
     #90) / 0.2E1) * t802) * t67 + (t431 * (t408 * t417 + t421 * t409 + 
     #t419 * t413) * t473 - t2410) * t67 / 0.2E1 + (t2410 - t1227 * (t12
     #13 * t1204 + t1217 * t1205 + t1215 * t1209) * t1267) * t67 / 0.2E1
     # + (t441 * (t195 / 0.2E1 + (t193 - t2424) * t176 / 0.2E1) - t2437)
     # * t67 / 0.2E1 + (t2437 - t1222 * (t583 / 0.2E1 + (t581 - t2441) *
     # t176 / 0.2E1)) * t67 / 0.2E1 + (t1595 * (t1572 * t1581 + t1585 * 
     #t1573 + t1583 * t1577) * t1605 - t2458) * t123 / 0.2E1 + (t2458 - 
     #t1942 * (t1919 * t1928 + t1932 * t1920 + t1930 * t1924) * t1952) *
     # t123 / 0.2E1 + (t4 * (t1594 * (t2471 + t2472 + t2473) / 0.2E1 + t
     #2480 / 0.2E1) * t832 - t4 * (t2480 / 0.2E1 + t1941 * (t2485 + t248
     #6 + t2487) / 0.2E1) * t834) * t123 + (t1592 * (t703 / 0.2E1 + (t70
     #1 - t2496) * t176 / 0.2E1) - t2504) * t123 / 0.2E1 + (t2504 - t192
     #5 * (t726 / 0.2E1 + (t724 - t2508) * t176 / 0.2E1)) * t123 / 0.2E1
     # + t809 + (t806 - t2541 * (t2518 * t2534 + t2531 * t2525 + t2529 *
     # t2521) * ((t2424 - t2431) * t67 / 0.2E1 + (t2431 - t2441) * t67 /
     # 0.2E1)) * t176 / 0.2E1 + t841 + (t838 - t2541 * (t2527 * t2534 + 
     #t2519 * t2525 + t2523 * t2521) * ((t2496 - t2431) * t123 / 0.2E1 +
     # (t2431 - t2508) * t123 / 0.2E1)) * t176 / 0.2E1 + (t864 - t4 * (t
     #860 / 0.2E1 + t2540 * (t2572 + t2573 + t2574) / 0.2E1) * t2433) * 
     #t176
        t2599 = sqrt(0.2E1 * t847 + 0.2E1 * t848 + 0.2E1 * t849 + 0.2E1 
     #* t856 + 0.2E1 * t857 + 0.2E1 * t858)
        t2603 = t864 + t863 * dt * t936 / 0.2E1 + cc * t862 * (t2 + t872
     # - t2323 - t934 - dt * (t2583 * t794 + src(i,j,t178,nComp,n)) / 0.
     #2E1 - dz * (t936 / 0.2E1 + (t934 - ut(i,j,t2423,n)) * t176 / 0.2E1
     #) / 0.2E1) / t2599
        t2610 = src(i,j,k,nComp,n + 1)

        unew(i,j,k) = t1 + dt * t2 + (t887 * t888 / 0.2E1 + t915 * t8
     #88 / 0.2E1 + t942 * t888 / 0.2E1 - t1318 * t888 / 0.2E1 - t1338 * 
     #t888 / 0.2E1 - t1358 * t888 / 0.2E1) * t55 * t67 + (t1380 * t888 /
     # 0.2E1 + t1688 * t888 / 0.2E1 + t1709 * t888 / 0.2E1 - t1727 * t88
     #8 / 0.2E1 - t2031 * t888 / 0.2E1 - t2051 * t888 / 0.2E1) * t55 * t
     #123 + (t2073 * t888 / 0.2E1 + t2091 * t888 / 0.2E1 + t2331 * t888 
     #/ 0.2E1 - t2349 * t888 / 0.2E1 - t2367 * t888 / 0.2E1 - t2603 * t8
     #88 / 0.2E1) * t55 * t176 + t2610 * t888 / 0.2E1

        utnew(i,j,k) = t2 + (t887 * 
     #dt + t915 * dt + t942 * dt - t1318 * dt - t1338 * dt - t1358 * dt)
     # * t55 * t67 + (t1380 * dt + t1688 * dt + t1709 * dt - t1727 * dt 
     #- t2031 * dt - t2051 * dt) * t55 * t123 + (t2073 * dt + t2091 * dt
     # + t2331 * dt - t2349 * dt - t2367 * dt - t2603 * dt) * t55 * t176
     # + t2610 * dt

c        blah = array(int(t1 + dt * t2 + (t887 * t888 / 0.2E1 + t915 * t8
c     #88 / 0.2E1 + t942 * t888 / 0.2E1 - t1318 * t888 / 0.2E1 - t1338 * 
c     #t888 / 0.2E1 - t1358 * t888 / 0.2E1) * t55 * t67 + (t1380 * t888 /
c     # 0.2E1 + t1688 * t888 / 0.2E1 + t1709 * t888 / 0.2E1 - t1727 * t88
c     #8 / 0.2E1 - t2031 * t888 / 0.2E1 - t2051 * t888 / 0.2E1) * t55 * t
c     #123 + (t2073 * t888 / 0.2E1 + t2091 * t888 / 0.2E1 + t2331 * t888 
c     #/ 0.2E1 - t2349 * t888 / 0.2E1 - t2367 * t888 / 0.2E1 - t2603 * t8
c     #88 / 0.2E1) * t55 * t176 + t2610 * t888 / 0.2E1),int(t2 + (t887 * 
c     #dt + t915 * dt + t942 * dt - t1318 * dt - t1338 * dt - t1358 * dt)
c     # * t55 * t67 + (t1380 * dt + t1688 * dt + t1709 * dt - t1727 * dt 
c     #- t2031 * dt - t2051 * dt) * t55 * t123 + (t2073 * dt + t2091 * dt
c     # + t2331 * dt - t2349 * dt - t2367 * dt - t2603 * dt) * t55 * t176
c     # + t2610 * dt))

        return
      end
