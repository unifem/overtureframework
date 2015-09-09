      subroutine duStepWaveGen2d4ccOLD( 
     *   nd1a,nd1b,nd2a,nd2b,
     *   n1a,n1b,n2a,n2b,
     *   u,ut,unew,utnew,rx,
     *   dx,dy,dt,cc,beta,
     *   i,j,n )

      implicit none
c
c.. declarations of incoming variables      
      integer nd1a,nd1b,nd2a,nd2b
      integer n1a,n1b,n2a,n2b
      integer i,j,n

      real u    (nd1a:nd1b,nd2a:nd2b,*)
      real ut   (nd1a:nd1b,nd2a:nd2b,*)
      real unew (nd1a:nd1b,nd2a:nd2b)
      real utnew(nd1a:nd1b,nd2a:nd2b)
      real rx   (nd1a:nd1b,nd2a:nd2b,0:1,0:1)
      real dx,dy,dt,cc,beta
c
c.. generated code to follow
c
        real t1
        real t10
        real t100
        real t1001
        real t1007
        real t101
        real t1012
        real t102
        real t1020
        real t1022
        real t1025
        real t1027
        real t1028
        real t1030
        real t1031
        real t1033
        real t1034
        real t104
        real t1042
        real t1046
        real t1049
        real t105
        real t1050
        real t1051
        real t1053
        real t1054
        real t1056
        real t1057
        real t1065
        real t1068
        real t1069
        real t107
        real t1070
        real t1072
        real t1073
        real t1074
        real t1076
        real t1079
        real t108
        real t1080
        real t1081
        real t1082
        real t1084
        real t1087
        real t1088
        real t1092
        real t1094
        real t1095
        real t1097
        real t1098
        real t110
        real t1100
        real t1106
        real t1107
        real t1108
        real t1110
        real t1113
        real t1114
        real t1116
        real t1117
        real t112
        real t1120
        real t1121
        real t1122
        real t1125
        real t1126
        real t1128
        real t1133
        real t1134
        real t1136
        real t1139
        real t114
        real t1140
        real t1147
        integer t115
        real t1157
        real t116
        real t1164
        real t1166
        real t1167
        real t1168
        real t1169
        real t1171
        real t1172
        real t1175
        real t118
        real t1187
        real t1189
        real t119
        real t1190
        real t12
        integer t120
        real t1200
        real t1203
        real t1204
        real t1206
        real t1207
        real t121
        real t1210
        real t1211
        real t1212
        real t1215
        real t1216
        real t1218
        real t1223
        real t1224
        real t1226
        real t1229
        real t123
        real t1230
        real t1237
        real t1247
        real t1254
        real t1255
        real t1256
        real t1257
        real t1258
        real t1259
        real t1261
        real t1262
        real t1265
        real t127
        real t1273
        real t1279
        real t1280
        real t129
        real t1290
        real t1294
        real t1297
        real t1298
        real t13
        real t1300
        real t1303
        real t1304
        real t1306
        real t1309
        real t131
        real t1310
        real t1312
        real t1313
        real t1314
        real t1316
        real t1318
        real t1319
        real t132
        real t1320
        real t1322
        real t1325
        real t1326
        real t1328
        real t1332
        real t1334
        real t1335
        real t1336
        real t1338
        real t134
        real t1340
        real t1344
        real t1348
        real t1349
        real t135
        real t1351
        real t1352
        real t1354
        real t1355
        real t1356
        real t1357
        real t1359
        real t1362
        real t1363
        real t1365
        real t137
        real t1370
        real t1372
        real t1376
        real t1378
        real t1379
        real t1380
        real t1381
        real t1383
        real t1384
        real t1386
        real t1387
        real t1391
        real t1393
        real t1397
        real t1399
        real t14
        real t1400
        real t1401
        real t1402
        real t1404
        real t1407
        real t1408
        real t141
        real t1410
        real t1412
        real t1413
        real t1414
        real t1415
        real t1416
        real t1418
        real t1419
        real t1421
        real t1422
        real t1423
        real t1424
        real t1426
        real t1429
        real t143
        real t1430
        real t1432
        real t1437
        real t1439
        real t144
        real t1443
        real t1445
        real t1446
        real t1447
        real t1448
        real t1450
        real t1451
        real t1453
        real t1454
        real t1458
        real t146
        real t1460
        real t1464
        real t1466
        real t1467
        real t1468
        real t1469
        real t1471
        real t1474
        real t1475
        real t1477
        real t1479
        real t148
        real t1480
        real t1481
        real t1483
        real t1485
        real t1488
        real t149
        real t1492
        real t1498
        real t15
        real t1505
        real t151
        real t1514
        real t1519
        real t152
        real t1522
        real t1531
        real t154
        real t1550
        real t1553
        real t1557
        real t1560
        integer t1561
        real t1562
        real t1563
        real t1565
        real t1566
        real t1568
        real t1569
        real t1570
        real t1571
        real t1573
        real t158
        real t1580
        real t1581
        real t1582
        real t1585
        real t1587
        real t1589
        real t1590
        real t1591
        real t1592
        real t1597
        real t1598
        real t160
        real t1600
        real t1601
        real t1602
        real t161
        real t1610
        real t1613
        real t1617
        real t1619
        real t162
        real t1627
        real t1628
        real t163
        real t1630
        real t1631
        real t1633
        real t1637
        real t1639
        real t1641
        real t1643
        real t1649
        real t165
        real t1652
        real t1656
        real t1659
        real t166
        real t1663
        real t1665
        real t1667
        real t1670
        real t1674
        real t1676
        real t168
        real t1682
        real t1684
        real t1686
        real t1688
        real t169
        real t1690
        real t1695
        real t1696
        real t17
        real t1700
        real t1705
        real t1706
        real t1707
        real t1708
        real t171
        real t1710
        real t1717
        real t1718
        real t1719
        real t1723
        real t1725
        real t1726
        real t1727
        real t1728
        real t1730
        real t1731
        real t1732
        real t1733
        real t1736
        real t1737
        real t1738
        real t1739
        real t1745
        real t1747
        real t1748
        real t1749
        real t175
        real t1750
        real t1752
        real t1754
        real t1755
        real t1756
        real t1759
        real t1760
        real t1763
        real t1764
        real t1767
        real t177
        real t1770
        real t1772
        real t1773
        real t1774
        real t1780
        real t1782
        real t1785
        real t1786
        real t1788
        real t1791
        real t1795
        real t1797
        real t18
        real t1802
        real t1804
        real t1805
        real t1807
        real t181
        real t1811
        real t1813
        real t1815
        real t1817
        real t1823
        real t1826
        real t1830
        real t1833
        real t1837
        real t1839
        real t1841
        real t1844
        real t1845
        real t1848
        real t185
        real t1850
        real t1856
        real t1857
        real t1860
        real t1862
        real t1864
        real t1866
        real t1868
        real t187
        real t1870
        real t1874
        real t1876
        real t1878
        real t188
        real t1880
        real t1885
        real t1886
        real t1888
        real t189
        real t1890
        real t1892
        real t1893
        real t1894
        real t1895
        real t1897
        real t1898
        real t1899
        real t19
        real t190
        real t1900
        real t1903
        real t1905
        real t1906
        real t1907
        real t1908
        real t1910
        real t1911
        real t1912
        real t1914
        real t1918
        real t192
        real t1920
        real t1922
        real t1923
        real t1927
        real t193
        real t1931
        real t1933
        real t1934
        real t1938
        real t194
        real t1940
        real t1941
        real t1942
        real t1943
        real t1945
        real t1946
        real t1947
        real t1949
        real t195
        real t1952
        real t1953
        real t1954
        real t1955
        real t1957
        real t196
        real t1960
        real t1961
        real t1963
        real t1965
        real t1966
        real t1967
        real t1969
        real t1970
        real t1972
        real t1978
        real t1979
        real t1982
        real t1983
        real t1985
        real t1986
        real t1988
        real t1989
        real t1990
        real t1991
        real t1993
        real t1996
        real t1997
        real t1999
        real t2
        real t20
        real t2004
        real t2006
        real t2010
        real t2012
        real t2013
        real t2014
        real t2015
        real t2017
        real t2018
        real t202
        real t2020
        real t2021
        real t2027
        real t2031
        real t2033
        real t2034
        real t2035
        real t2036
        real t2038
        real t204
        real t2041
        real t2042
        real t2044
        real t2046
        real t2048
        real t2049
        real t2050
        real t2052
        real t2053
        real t2055
        real t2056
        real t2057
        real t2058
        real t2060
        real t2063
        real t2064
        real t2066
        real t2071
        real t2073
        real t2077
        real t2079
        real t208
        real t2080
        real t2081
        real t2082
        real t2084
        real t2085
        real t2087
        real t2088
        real t2094
        real t2098
        real t210
        real t2100
        real t2101
        real t2102
        real t2103
        real t2105
        real t2108
        real t2109
        real t211
        real t2111
        real t2113
        real t2115
        real t2119
        real t212
        real t2122
        real t2124
        real t2128
        real t213
        real t2130
        real t2133
        real t2135
        real t2139
        real t2142
        real t2143
        real t2144
        real t2149
        real t215
        real t2151
        real t2152
        real t2156
        real t216
        real t2160
        real t2162
        real t2163
        real t2167
        real t2169
        real t217
        real t2170
        real t2171
        real t2172
        real t2174
        real t2176
        real t2177
        real t2178
        real t2181
        real t2183
        real t2185
        real t2187
        real t2188
        real t219
        real t2191
        real t2192
        real t22
        real t2202
        real t2203
        real t2207
        real t2208
        real t2210
        real t2211
        real t2214
        real t2215
        real t2218
        real t222
        real t2220
        real t2221
        real t2223
        real t2228
        real t223
        real t2235
        real t2237
        real t2239
        real t224
        real t2241
        real t2243
        real t2245
        real t2246
        real t2249
        real t225
        real t2251
        real t2257
        real t2259
        real t2264
        real t2266
        real t2267
        real t227
        real t2271
        real t2279
        real t2284
        real t2286
        real t2287
        real t2290
        real t2291
        real t2297
        real t23
        real t230
        real t2308
        real t2309
        real t231
        real t2310
        real t2313
        real t2314
        real t2315
        real t2316
        real t2317
        real t2321
        real t2323
        real t2327
        real t233
        real t2330
        real t2331
        real t2338
        real t2343
        real t2344
        real t2346
        real t235
        real t2350
        real t2352
        real t2354
        real t2362
        real t2364
        real t2369
        real t2371
        real t2372
        real t2375
        real t2379
        real t238
        real t2381
        real t2389
        real t239
        real t2391
        real t2396
        real t2398
        real t2399
        real t2402
        real t2405
        real t2407
        real t241
        real t2411
        real t2413
        real t2414
        real t2415
        real t2417
        real t2418
        real t2419
        real t242
        real t2420
        real t2422
        real t2426
        real t2428
        real t2429
        real t243
        real t2430
        real t2432
        real t2433
        real t2434
        real t2436
        real t244
        real t2440
        real t2442
        real t2446
        real t2447
        real t2450
        real t2451
        real t2454
        real t2456
        real t2458
        real t2465
        real t2467
        real t2468
        real t2471
        real t2472
        real t2478
        real t248
        real t2489
        real t249
        real t2490
        real t2491
        real t2494
        real t2495
        real t2496
        real t2497
        real t2498
        real t25
        real t2502
        real t2504
        real t2508
        real t251
        real t2511
        real t2512
        real t252
        real t2520
        real t2524
        real t2531
        real t2536
        real t254
        real t2541
        real t2544
        real t2547
        integer t2549
        real t2550
        real t2551
        real t2553
        real t2554
        real t2557
        real t2558
        real t2559
        real t2561
        real t2569
        real t2573
        real t2575
        real t258
        real t2585
        real t2588
        real t26
        real t260
        real t261
        real t2616
        real t2618
        real t2619
        real t262
        real t2621
        real t2627
        real t263
        real t2637
        real t264
        real t265
        real t2651
        real t2655
        real t266
        real t2672
        real t268
        real t2684
        real t269
        real t2694
        real t27
        real t2706
        real t2723
        real t2725
        real t2728
        real t273
        real t2730
        real t2734
        real t2741
        real t275
        real t2750
        real t2758
        real t2760
        real t2764
        real t2766
        real t2780
        real t2783
        real t279
        real t2791
        real t28
        real t2801
        real t2815
        real t2819
        real t283
        real t2830
        real t2834
        real t284
        real t2840
        real t2844
        real t2848
        real t285
        real t2854
        real t286
        real t287
        real t2873
        real t2878
        real t288
        real t2884
        real t2886
        real t2888
        real t2895
        real t2899
        real t290
        real t2902
        real t2906
        real t2909
        real t291
        real t2910
        real t2911
        real t2913
        real t2914
        real t2915
        real t2917
        real t2920
        real t2921
        real t2922
        real t2923
        real t2925
        real t2928
        real t2929
        real t293
        real t2933
        real t2935
        real t2937
        real t2938
        real t294
        real t2946
        real t2950
        real t2951
        real t2953
        real t2954
        real t2957
        real t2958
        real t2959
        real t2972
        real t298
        real t2982
        real t2983
        real t2985
        real t2986
        real t2989
        real t30
        real t300
        real t3003
        real t3004
        real t3014
        real t3017
        real t3018
        real t3020
        real t3021
        real t3024
        real t3025
        real t3026
        real t3039
        real t304
        real t3049
        real t3050
        real t3052
        real t3053
        real t3056
        real t306
        real t307
        real t3070
        real t3071
        real t308
        real t3081
        real t309
        real t3098
        real t31
        real t311
        real t3117
        real t312
        real t3128
        real t313
        real t3149
        real t315
        real t3153
        real t3156
        real t3168
        real t3169
        real t3174
        real t3177
        real t318
        real t3180
        real t3182
        real t319
        real t3194
        real t3197
        real t3199
        integer t32
        real t320
        real t3205
        real t3207
        real t321
        real t3223
        real t323
        real t3234
        real t3251
        real t3256
        real t3258
        real t326
        real t3264
        real t327
        real t3270
        real t3274
        real t3279
        real t3285
        real t3289
        real t329
        real t3293
        real t3299
        real t33
        real t3303
        real t3306
        real t3309
        real t331
        real t3311
        real t3313
        real t332
        real t3326
        real t334
        real t3344
        real t3348
        real t3356
        real t3361
        real t3369
        real t337
        real t3375
        real t3377
        real t3381
        real t3384
        real t3386
        real t3387
        real t3388
        real t339
        real t3394
        real t34
        real t340
        real t3405
        real t3406
        real t3407
        real t341
        real t3410
        real t3411
        real t3415
        real t3417
        real t3421
        real t3424
        real t3425
        real t3426
        real t343
        real t3432
        real t3437
        real t3439
        real t344
        real t3448
        real t3454
        real t3458
        real t346
        real t3461
        real t3464
        real t3466
        real t3468
        real t347
        real t3476
        real t3478
        real t3482
        real t3485
        real t3487
        real t3488
        real t349
        real t3491
        real t3492
        real t3498
        real t3509
        real t3510
        real t3511
        real t3514
        real t3515
        real t3516
        real t3517
        real t3518
        real t3522
        real t3524
        real t3528
        real t353
        real t3531
        real t3532
        real t354
        real t3540
        real t3544
        real t3551
        real t3556
        real t356
        real t3561
        real t3564
        real t3567
        real t3569
        real t357
        real t3579
        real t359
        real t3591
        real t36
        integer t3608
        real t3609
        real t3611
        real t3619
        real t3621
        real t3628
        real t363
        real t3631
        real t3633
        real t365
        real t3650
        real t366
        real t367
        real t3677
        real t3678
        real t3680
        real t3681
        real t3684
        real t369
        real t3690
        real t3692
        real t3698
        real t37
        real t370
        real t3708
        real t3709
        real t3711
        real t3719
        real t372
        real t3732
        real t3735
        real t3743
        real t3745
        real t3748
        real t3750
        real t3754
        real t376
        real t3761
        real t3779
        real t378
        real t3781
        real t379
        real t3795
        real t3798
        real t3800
        real t381
        real t3817
        real t383
        real t3853
        real t387
        real t3870
        real t3878
        real t3880
        real t3883
        real t3884
        real t3885
        real t3887
        real t3888
        real t3889
        real t3891
        real t3894
        real t3895
        real t3896
        real t3897
        real t3899
        real t39
        real t3902
        real t3903
        real t3909
        real t391
        real t3913
        real t3916
        real t3920
        real t3923
        real t3926
        real t3928
        real t393
        real t3930
        real t3931
        real t3939
        real t394
        real t3944
        real t3945
        real t3954
        real t396
        real t3964
        real t3965
        real t3967
        real t3968
        real t3971
        real t398
        real t3985
        real t3986
        real t3996
        real t4
        real t40
        real t4004
        real t4008
        real t4009
        real t4018
        real t402
        real t4028
        real t4029
        real t4031
        real t4032
        real t4035
        real t404
        real t4049
        real t405
        real t4050
        real t406
        real t4060
        real t407
        real t4086
        real t4089
        real t409
        real t41
        real t4101
        real t411
        real t4119
        real t412
        real t4123
        real t4128
        real t4129
        real t4131
        real t4137
        real t414
        real t4140
        real t4142
        real t4144
        real t4146
        real t4147
        real t415
        real t4150
        real t4151
        real t4161
        real t4162
        real t4167
        real t417
        real t4170
        real t4173
        real t4175
        real t418
        real t4181
        real t4188
        real t4190
        real t4192
        real t4193
        real t4196
        real t4198
        real t42
        real t420
        real t4204
        real t4206
        real t4222
        real t4233
        real t424
        real t4250
        real t4255
        real t4257
        real t426
        real t4266
        real t427
        real t4272
        real t4276
        real t4279
        real t4282
        real t4284
        real t4286
        real t429
        real t4299
        real t4317
        real t4321
        real t4328
        real t433
        real t4333
        real t4338
        real t4341
        real t4344
        real t4346
        real t4356
        real t4368
        real t437
        integer t4385
        real t4386
        real t4388
        real t439
        real t4396
        real t4398
        real t44
        real t440
        real t4405
        real t4408
        real t4410
        real t442
        real t4427
        real t4454
        real t4455
        real t4457
        real t4458
        real t446
        real t4461
        real t4467
        real t4469
        real t4475
        real t448
        real t4485
        real t4486
        real t4488
        real t449
        real t4496
        real t450
        real t4509
        real t451
        real t4512
        real t4520
        real t4522
        real t4525
        real t4527
        real t453
        real t4531
        real t4538
        real t455
        real t4556
        real t4558
        real t456
        real t4572
        real t4575
        real t4577
        real t458
        real t4594
        real t46
        real t461
        real t462
        real t4630
        real t4647
        real t465
        real t4655
        real t4657
        real t466
        real t4660
        real t4661
        real t4663
        real t4664
        real t4665
        real t4667
        real t4670
        real t4671
        real t4672
        real t4673
        real t4675
        real t4678
        real t4679
        integer t468
        real t4685
        real t4689
        real t469
        real t4692
        real t4696
        real t4699
        real t470
        real t4702
        real t4704
        real t4706
        real t4707
        real t4715
        real t472
        real t4720
        real t4721
        real t473
        real t4730
        real t4740
        real t4741
        real t4743
        real t4744
        real t4747
        real t476
        real t4761
        real t4762
        real t477
        real t4772
        real t478
        real t4780
        real t4784
        real t4785
        real t4794
        integer t48
        real t480
        real t4804
        real t4805
        real t4807
        real t4808
        real t4811
        real t4825
        real t4826
        real t4836
        real t484
        real t4862
        real t4876
        real t4894
        real t4898
        real t49
        real t490
        real t4901
        real t4913
        real t4914
        real t4919
        real t492
        real t4922
        real t4925
        real t4927
        real t4939
        real t4942
        real t4944
        real t495
        real t4950
        real t4952
        real t496
        real t498
        real t4981
        integer t5
        real t50
        real t5004
        real t502
        real t503
        real t507
        real t508
        real t509
        real t510
        real t515
        real t518
        real t52
        real t521
        real t522
        real t528
        integer t529
        real t53
        real t530
        real t532
        integer t536
        real t537
        real t539
        real t545
        real t547
        real t549
        real t55
        real t552
        real t553
        real t555
        real t558
        real t56
        real t562
        real t563
        real t565
        real t567
        real t57
        real t570
        real t571
        real t573
        real t576
        real t58
        real t580
        real t582
        real t591
        real t593
        real t594
        real t596
        real t6
        real t60
        real t602
        real t606
        real t610
        real t612
        real t618
        real t62
        real t624
        real t634
        real t638
        real t642
        real t648
        real t659
        real t660
        real t662
        real t663
        real t665
        real t666
        real t67
        real t672
        real t674
        real t678
        real t68
        real t680
        real t684
        real t687
        real t688
        real t69
        real t690
        real t691
        real t693
        real t694
        real t7
        real t70
        real t700
        real t702
        real t706
        real t708
        real t71
        real t718
        real t719
        real t72
        real t720
        real t722
        real t73
        real t732
        real t737
        real t738
        real t740
        real t748
        real t75
        real t755
        real t757
        real t76
        real t762
        real t768
        real t769
        real t771
        real t776
        real t777
        real t779
        real t78
        real t787
        real t789
        real t792
        real t793
        real t795
        real t799
        real t80
        real t800
        real t807
        real t808
        real t809
        real t81
        real t811
        real t815
        real t820
        real t823
        real t829
        real t83
        real t831
        real t835
        real t837
        real t845
        real t847
        real t85
        real t850
        real t851
        real t853
        real t856
        real t86
        real t860
        real t863
        real t865
        real t868
        real t869
        real t871
        real t874
        real t878
        real t880
        real t885
        real t888
        real t89
        real t896
        real t9
        real t900
        real t904
        real t906
        real t912
        real t918
        real t928
        real t932
        real t936
        real t94
        real t942
        real t95
        real t954
        real t956
        real t962
        real t966
        real t97
        real t970
        real t972
        real t978
        real t981
        real t992
        real t994
        real t996
        t1 = u(i,j,n)
        t2 = ut(i,j,n)
        t4 = cc ** 2
        t5 = i + 1
        t6 = rx(t5,j,0,0)
        t7 = rx(t5,j,1,1)
        t9 = rx(t5,j,1,0)
        t10 = rx(t5,j,0,1)
        t12 = t6 * t7 - t9 * t10
        t13 = 0.1E1 / t12
        t14 = t6 ** 2
        t15 = t10 ** 2
        t17 = t13 * (t14 + t15)
        t18 = t17 / 0.2E1
        t19 = rx(i,j,0,0)
        t20 = rx(i,j,1,1)
        t22 = rx(i,j,1,0)
        t23 = rx(i,j,0,1)
        t25 = t19 * t20 - t22 * t23
        t26 = 0.1E1 / t25
        t27 = t19 ** 2
        t28 = t23 ** 2
        t30 = t26 * (t27 + t28)
        t31 = t30 / 0.2E1
        t32 = i + 2
        t33 = rx(t32,j,0,0)
        t34 = rx(t32,j,1,1)
        t36 = rx(t32,j,1,0)
        t37 = rx(t32,j,0,1)
        t39 = t33 * t34 - t36 * t37
        t40 = 0.1E1 / t39
        t41 = t33 ** 2
        t42 = t37 ** 2
        t44 = t40 * (t41 + t42)
        t46 = 0.1E1 / dx
        t48 = i - 1
        t49 = rx(t48,j,0,0)
        t50 = rx(t48,j,1,1)
        t52 = rx(t48,j,1,0)
        t53 = rx(t48,j,0,1)
        t55 = t49 * t50 - t52 * t53
        t56 = 0.1E1 / t55
        t57 = t49 ** 2
        t58 = t53 ** 2
        t60 = t56 * (t57 + t58)
        t62 = (t30 - t60) * t46
        t67 = t18 + t31 - dx * ((t44 - t17) * t46 / 0.2E1 - t62 / 0.2E1)
     # / 0.8E1
        t68 = t4 * t67
        t69 = sqrt(0.3E1)
        t70 = t69 / 0.6E1
        t71 = 0.1E1 / 0.2E1 + t70
        t72 = t71 * dt
        t73 = ut(t5,j,n)
        t75 = (t73 - t2) * t46
        t76 = ut(t32,j,n)
        t78 = (t76 - t73) * t46
        t80 = (t78 - t75) * t46
        t81 = ut(t48,j,n)
        t83 = (t2 - t81) * t46
        t85 = (t75 - t83) * t46
        t86 = t80 - t85
        t89 = t75 - dx * t86 / 0.24E2
        t94 = t4 * (t17 / 0.2E1 + t30 / 0.2E1)
        t95 = t71 ** 2
        t97 = dt ** 2
        t100 = t4 * (t44 / 0.2E1 + t17 / 0.2E1)
        t101 = u(t32,j,n)
        t102 = u(t5,j,n)
        t104 = (t101 - t102) * t46
        t105 = t100 * t104
        t107 = (t102 - t1) * t46
        t108 = t94 * t107
        t110 = (t105 - t108) * t46
        t114 = t33 * t36 + t37 * t34
        t115 = j + 1
        t116 = u(t32,t115,n)
        t118 = 0.1E1 / dy
        t119 = (t116 - t101) * t118
        t120 = j - 1
        t121 = u(t32,t120,n)
        t123 = (t101 - t121) * t118
        t112 = t4 * t40 * t114
        t127 = t112 * (t119 / 0.2E1 + t123 / 0.2E1)
        t131 = t6 * t9 + t10 * t7
        t132 = u(t5,t115,n)
        t134 = (t132 - t102) * t118
        t135 = u(t5,t120,n)
        t137 = (t102 - t135) * t118
        t129 = t4 * t13 * t131
        t141 = t129 * (t134 / 0.2E1 + t137 / 0.2E1)
        t143 = (t127 - t141) * t46
        t144 = t143 / 0.2E1
        t148 = t19 * t22 + t23 * t20
        t149 = u(i,t115,n)
        t151 = (t149 - t1) * t118
        t152 = u(i,t120,n)
        t154 = (t1 - t152) * t118
        t146 = t4 * t26 * t148
        t158 = t146 * (t151 / 0.2E1 + t154 / 0.2E1)
        t160 = (t141 - t158) * t46
        t161 = t160 / 0.2E1
        t162 = rx(t5,t115,0,0)
        t163 = rx(t5,t115,1,1)
        t165 = rx(t5,t115,1,0)
        t166 = rx(t5,t115,0,1)
        t168 = t162 * t163 - t165 * t166
        t169 = 0.1E1 / t168
        t175 = (t116 - t132) * t46
        t177 = (t132 - t149) * t46
        t171 = t4 * t169 * (t162 * t165 + t166 * t163)
        t181 = t171 * (t175 / 0.2E1 + t177 / 0.2E1)
        t185 = t129 * (t104 / 0.2E1 + t107 / 0.2E1)
        t187 = (t181 - t185) * t118
        t188 = t187 / 0.2E1
        t189 = rx(t5,t120,0,0)
        t190 = rx(t5,t120,1,1)
        t192 = rx(t5,t120,1,0)
        t193 = rx(t5,t120,0,1)
        t195 = t189 * t190 - t192 * t193
        t196 = 0.1E1 / t195
        t202 = (t121 - t135) * t46
        t204 = (t135 - t152) * t46
        t194 = t4 * t196 * (t189 * t192 + t193 * t190)
        t208 = t194 * (t202 / 0.2E1 + t204 / 0.2E1)
        t210 = (t185 - t208) * t118
        t211 = t210 / 0.2E1
        t212 = t165 ** 2
        t213 = t163 ** 2
        t215 = t169 * (t212 + t213)
        t216 = t9 ** 2
        t217 = t7 ** 2
        t219 = t13 * (t216 + t217)
        t222 = t4 * (t215 / 0.2E1 + t219 / 0.2E1)
        t223 = t222 * t134
        t224 = t192 ** 2
        t225 = t190 ** 2
        t227 = t196 * (t224 + t225)
        t230 = t4 * (t219 / 0.2E1 + t227 / 0.2E1)
        t231 = t230 * t137
        t233 = (t223 - t231) * t118
        t235 = (t110 + t144 + t161 + t188 + t211 + t233) * t12
        t238 = t4 * (t30 / 0.2E1 + t60 / 0.2E1)
        t239 = u(t48,j,n)
        t241 = (t1 - t239) * t46
        t242 = t238 * t241
        t244 = (t108 - t242) * t46
        t248 = t49 * t52 + t53 * t50
        t249 = u(t48,t115,n)
        t251 = (t249 - t239) * t118
        t252 = u(t48,t120,n)
        t254 = (t239 - t252) * t118
        t243 = t4 * t56 * t248
        t258 = t243 * (t251 / 0.2E1 + t254 / 0.2E1)
        t260 = (t158 - t258) * t46
        t261 = t260 / 0.2E1
        t262 = rx(i,t115,0,0)
        t263 = rx(i,t115,1,1)
        t265 = rx(i,t115,1,0)
        t266 = rx(i,t115,0,1)
        t268 = t262 * t263 - t265 * t266
        t269 = 0.1E1 / t268
        t273 = t262 * t265 + t266 * t263
        t275 = (t149 - t249) * t46
        t264 = t4 * t269 * t273
        t279 = t264 * (t177 / 0.2E1 + t275 / 0.2E1)
        t283 = t146 * (t107 / 0.2E1 + t241 / 0.2E1)
        t285 = (t279 - t283) * t118
        t286 = t285 / 0.2E1
        t287 = rx(i,t120,0,0)
        t288 = rx(i,t120,1,1)
        t290 = rx(i,t120,1,0)
        t291 = rx(i,t120,0,1)
        t293 = t287 * t288 - t290 * t291
        t294 = 0.1E1 / t293
        t298 = t287 * t290 + t291 * t288
        t300 = (t152 - t252) * t46
        t284 = t4 * t294 * t298
        t304 = t284 * (t204 / 0.2E1 + t300 / 0.2E1)
        t306 = (t283 - t304) * t118
        t307 = t306 / 0.2E1
        t308 = t265 ** 2
        t309 = t263 ** 2
        t311 = t269 * (t308 + t309)
        t312 = t22 ** 2
        t313 = t20 ** 2
        t315 = t26 * (t312 + t313)
        t318 = t4 * (t311 / 0.2E1 + t315 / 0.2E1)
        t319 = t318 * t151
        t320 = t290 ** 2
        t321 = t288 ** 2
        t323 = t294 * (t320 + t321)
        t326 = t4 * (t315 / 0.2E1 + t323 / 0.2E1)
        t327 = t326 * t154
        t329 = (t319 - t327) * t118
        t331 = (t244 + t161 + t261 + t286 + t307 + t329) * t25
        t332 = t235 - t331
        t334 = t97 * t332 * t46
        t337 = t95 * t71
        t339 = t97 * dt
        t340 = t100 * t78
        t341 = t94 * t75
        t343 = (t340 - t341) * t46
        t344 = ut(t32,t115,n)
        t346 = (t344 - t76) * t118
        t347 = ut(t32,t120,n)
        t349 = (t76 - t347) * t118
        t353 = t112 * (t346 / 0.2E1 + t349 / 0.2E1)
        t354 = ut(t5,t115,n)
        t356 = (t354 - t73) * t118
        t357 = ut(t5,t120,n)
        t359 = (t73 - t357) * t118
        t363 = t129 * (t356 / 0.2E1 + t359 / 0.2E1)
        t365 = (t353 - t363) * t46
        t366 = t365 / 0.2E1
        t367 = ut(i,t115,n)
        t369 = (t367 - t2) * t118
        t370 = ut(i,t120,n)
        t372 = (t2 - t370) * t118
        t376 = t146 * (t369 / 0.2E1 + t372 / 0.2E1)
        t378 = (t363 - t376) * t46
        t379 = t378 / 0.2E1
        t381 = (t344 - t354) * t46
        t383 = (t354 - t367) * t46
        t387 = t171 * (t381 / 0.2E1 + t383 / 0.2E1)
        t391 = t129 * (t78 / 0.2E1 + t75 / 0.2E1)
        t393 = (t387 - t391) * t118
        t394 = t393 / 0.2E1
        t396 = (t347 - t357) * t46
        t398 = (t357 - t370) * t46
        t402 = t194 * (t396 / 0.2E1 + t398 / 0.2E1)
        t404 = (t391 - t402) * t118
        t405 = t404 / 0.2E1
        t406 = t222 * t356
        t407 = t230 * t359
        t409 = (t406 - t407) * t118
        t411 = (t343 + t366 + t379 + t394 + t405 + t409) * t12
        t412 = t238 * t83
        t414 = (t341 - t412) * t46
        t415 = ut(t48,t115,n)
        t417 = (t415 - t81) * t118
        t418 = ut(t48,t120,n)
        t420 = (t81 - t418) * t118
        t424 = t243 * (t417 / 0.2E1 + t420 / 0.2E1)
        t426 = (t376 - t424) * t46
        t427 = t426 / 0.2E1
        t429 = (t367 - t415) * t46
        t433 = t264 * (t383 / 0.2E1 + t429 / 0.2E1)
        t437 = t146 * (t75 / 0.2E1 + t83 / 0.2E1)
        t439 = (t433 - t437) * t118
        t440 = t439 / 0.2E1
        t442 = (t370 - t418) * t46
        t446 = t284 * (t398 / 0.2E1 + t442 / 0.2E1)
        t448 = (t437 - t446) * t118
        t449 = t448 / 0.2E1
        t450 = t318 * t369
        t451 = t326 * t372
        t453 = (t450 - t451) * t118
        t455 = (t414 + t379 + t427 + t440 + t449 + t453) * t25
        t456 = t411 - t455
        t458 = t339 * t456 * t46
        t461 = t343 - t414
        t462 = dx * t461
        t465 = cc * t67
        t466 = beta * t71
        t468 = i + 3
        t469 = rx(t468,j,0,0)
        t470 = rx(t468,j,1,1)
        t472 = rx(t468,j,1,0)
        t473 = rx(t468,j,0,1)
        t476 = 0.1E1 / (t469 * t470 - t472 * t473)
        t477 = t469 ** 2
        t478 = t473 ** 2
        t480 = t476 * (t477 + t478)
        t484 = (t17 - t30) * t46
        t490 = t4 * (t44 / 0.2E1 + t18 - dx * ((t480 - t44) * t46 / 0.2E
     #1 - t484 / 0.2E1) / 0.8E1)
        t492 = t68 * t107
        t495 = dx ** 2
        t496 = u(t468,j,n)
        t498 = (t496 - t101) * t46
        t502 = (t104 - t107) * t46
        t507 = (t107 - t241) * t46
        t508 = t502 - t507
        t509 = t508 * t46
        t510 = t94 * t509
        t515 = t4 * (t480 / 0.2E1 + t44 / 0.2E1)
        t518 = (t515 * t498 - t105) * t46
        t521 = t110 - t244
        t522 = t521 * t46
        t528 = dy ** 2
        t529 = j + 2
        t530 = u(t32,t529,n)
        t532 = (t530 - t116) * t118
        t536 = j - 2
        t537 = u(t32,t536,n)
        t539 = (t121 - t537) * t118
        t547 = u(t5,t529,n)
        t549 = (t547 - t132) * t118
        t552 = (t549 / 0.2E1 - t137 / 0.2E1) * t118
        t553 = u(t5,t536,n)
        t555 = (t135 - t553) * t118
        t558 = (t134 / 0.2E1 - t555 / 0.2E1) * t118
        t562 = t129 * (t552 - t558) * t118
        t565 = u(i,t529,n)
        t567 = (t565 - t149) * t118
        t570 = (t567 / 0.2E1 - t154 / 0.2E1) * t118
        t571 = u(i,t536,n)
        t573 = (t152 - t571) * t118
        t576 = (t151 / 0.2E1 - t573 / 0.2E1) * t118
        t580 = t146 * (t570 - t576) * t118
        t582 = (t562 - t580) * t46
        t591 = u(t468,t115,n)
        t593 = (t591 - t496) * t118
        t594 = u(t468,t120,n)
        t596 = (t496 - t594) * t118
        t503 = t4 * t476 * (t469 * t472 + t473 * t470)
        t602 = (t503 * (t593 / 0.2E1 + t596 / 0.2E1) - t127) * t46
        t606 = (t143 - t160) * t46
        t610 = (t160 - t260) * t46
        t612 = (t606 - t610) * t46
        t618 = (t591 - t116) * t46
        t624 = (t175 / 0.2E1 - t275 / 0.2E1) * t46
        t634 = (t104 / 0.2E1 - t241 / 0.2E1) * t46
        t638 = t129 * ((t498 / 0.2E1 - t107 / 0.2E1) * t46 - t634) * t46
        t642 = (t594 - t121) * t46
        t648 = (t202 / 0.2E1 - t300 / 0.2E1) * t46
        t659 = rx(t5,t529,0,0)
        t660 = rx(t5,t529,1,1)
        t662 = rx(t5,t529,1,0)
        t663 = rx(t5,t529,0,1)
        t665 = t659 * t660 - t662 * t663
        t666 = 0.1E1 / t665
        t672 = (t530 - t547) * t46
        t674 = (t547 - t565) * t46
        t545 = t4 * t666 * (t659 * t662 + t663 * t660)
        t678 = t545 * (t672 / 0.2E1 + t674 / 0.2E1)
        t680 = (t678 - t181) * t118
        t684 = (t187 - t210) * t118
        t687 = rx(t5,t536,0,0)
        t688 = rx(t5,t536,1,1)
        t690 = rx(t5,t536,1,0)
        t691 = rx(t5,t536,0,1)
        t693 = t687 * t688 - t690 * t691
        t694 = 0.1E1 / t693
        t700 = (t537 - t553) * t46
        t702 = (t553 - t571) * t46
        t563 = t4 * t694 * (t687 * t690 + t691 * t688)
        t706 = t563 * (t700 / 0.2E1 + t702 / 0.2E1)
        t708 = (t208 - t706) * t118
        t718 = t219 / 0.2E1
        t719 = t662 ** 2
        t720 = t660 ** 2
        t722 = t666 * (t719 + t720)
        t732 = t4 * (t215 / 0.2E1 + t718 - dy * ((t722 - t215) * t118 / 
     #0.2E1 - (t219 - t227) * t118 / 0.2E1) / 0.8E1)
        t737 = t690 ** 2
        t738 = t688 ** 2
        t740 = t694 * (t737 + t738)
        t748 = t4 * (t718 + t227 / 0.2E1 - dy * ((t215 - t219) * t118 / 
     #0.2E1 - (t227 - t740) * t118 / 0.2E1) / 0.8E1)
        t755 = (t134 - t137) * t118
        t757 = ((t549 - t134) * t118 - t755) * t118
        t762 = (t755 - (t137 - t555) * t118) * t118
        t768 = t4 * (t722 / 0.2E1 + t215 / 0.2E1)
        t769 = t768 * t549
        t771 = (t769 - t223) * t118
        t776 = t4 * (t227 / 0.2E1 + t740 / 0.2E1)
        t777 = t776 * t555
        t779 = (t231 - t777) * t118
        t787 = (t490 * t104 - t492) * t46 - t495 * ((t100 * ((t498 - t10
     #4) * t46 - t502) * t46 - t510) * t46 + ((t518 - t110) * t46 - t522
     #) * t46) / 0.24E2 + t144 + t161 - t528 * ((t112 * ((t532 / 0.2E1 -
     # t123 / 0.2E1) * t118 - (t119 / 0.2E1 - t539 / 0.2E1) * t118) * t1
     #18 - t562) * t46 / 0.2E1 + t582 / 0.2E1) / 0.6E1 - t495 * (((t602 
     #- t143) * t46 - t606) * t46 / 0.2E1 + t612 / 0.2E1) / 0.6E1 + t188
     # + t211 - t495 * ((t171 * ((t618 / 0.2E1 - t177 / 0.2E1) * t46 - t
     #624) * t46 - t638) * t118 / 0.2E1 + (t638 - t194 * ((t642 / 0.2E1 
     #- t204 / 0.2E1) * t46 - t648) * t46) * t118 / 0.2E1) / 0.6E1 - t52
     #8 * (((t680 - t187) * t118 - t684) * t118 / 0.2E1 + (t684 - (t210 
     #- t708) * t118) * t118 / 0.2E1) / 0.6E1 + (t732 * t134 - t748 * t1
     #37) * t118 - t528 * ((t222 * t757 - t230 * t762) * t118 + ((t771 -
     # t233) * t118 - (t233 - t779) * t118) * t118) / 0.24E2
        t789 = dt * t787 * t12
        t792 = t75 / 0.2E1
        t793 = ut(t468,j,n)
        t795 = (t793 - t76) * t46
        t799 = ((t795 - t78) * t46 - t80) * t46
        t800 = t86 * t46
        t807 = dx * (t78 / 0.2E1 + t792 - t495 * (t799 / 0.2E1 + t800 / 
     #0.2E1) / 0.6E1) / 0.2E1
        t808 = beta ** 2
        t809 = t808 * t95
        t811 = t68 * t75
        t815 = t94 * t800
        t820 = (t515 * t795 - t340) * t46
        t823 = t461 * t46
        t829 = ut(t32,t529,n)
        t831 = (t829 - t344) * t118
        t835 = ut(t32,t536,n)
        t837 = (t347 - t835) * t118
        t845 = ut(t5,t529,n)
        t847 = (t845 - t354) * t118
        t850 = (t847 / 0.2E1 - t359 / 0.2E1) * t118
        t851 = ut(t5,t536,n)
        t853 = (t357 - t851) * t118
        t856 = (t356 / 0.2E1 - t853 / 0.2E1) * t118
        t860 = t129 * (t850 - t856) * t118
        t863 = ut(i,t529,n)
        t865 = (t863 - t367) * t118
        t868 = (t865 / 0.2E1 - t372 / 0.2E1) * t118
        t869 = ut(i,t536,n)
        t871 = (t370 - t869) * t118
        t874 = (t369 / 0.2E1 - t871 / 0.2E1) * t118
        t878 = t146 * (t868 - t874) * t118
        t880 = (t860 - t878) * t46
        t885 = ut(t468,t115,n)
        t888 = ut(t468,t120,n)
        t896 = (t503 * ((t885 - t793) * t118 / 0.2E1 + (t793 - t888) * t
     #118 / 0.2E1) - t353) * t46
        t900 = (t365 - t378) * t46
        t904 = (t378 - t426) * t46
        t906 = (t900 - t904) * t46
        t912 = (t885 - t344) * t46
        t918 = (t381 / 0.2E1 - t429 / 0.2E1) * t46
        t928 = (t78 / 0.2E1 - t83 / 0.2E1) * t46
        t932 = t129 * ((t795 / 0.2E1 - t75 / 0.2E1) * t46 - t928) * t46
        t936 = (t888 - t347) * t46
        t942 = (t396 / 0.2E1 - t442 / 0.2E1) * t46
        t954 = (t829 - t845) * t46
        t956 = (t845 - t863) * t46
        t962 = (t545 * (t954 / 0.2E1 + t956 / 0.2E1) - t387) * t118
        t966 = (t393 - t404) * t118
        t970 = (t835 - t851) * t46
        t972 = (t851 - t869) * t46
        t978 = (t402 - t563 * (t970 / 0.2E1 + t972 / 0.2E1)) * t118
        t994 = (t356 - t359) * t118
        t996 = ((t847 - t356) * t118 - t994) * t118
        t1001 = (t994 - (t359 - t853) * t118) * t118
        t1007 = (t768 * t847 - t406) * t118
        t1012 = (t407 - t776 * t853) * t118
        t1020 = (t490 * t78 - t811) * t46 - t495 * ((t100 * t799 - t815)
     # * t46 + ((t820 - t343) * t46 - t823) * t46) / 0.24E2 + t366 + t37
     #9 - t528 * ((t112 * ((t831 / 0.2E1 - t349 / 0.2E1) * t118 - (t346 
     #/ 0.2E1 - t837 / 0.2E1) * t118) * t118 - t860) * t46 / 0.2E1 + t88
     #0 / 0.2E1) / 0.6E1 - t495 * (((t896 - t365) * t46 - t900) * t46 / 
     #0.2E1 + t906 / 0.2E1) / 0.6E1 + t394 + t405 - t495 * ((t171 * ((t9
     #12 / 0.2E1 - t383 / 0.2E1) * t46 - t918) * t46 - t932) * t118 / 0.
     #2E1 + (t932 - t194 * ((t936 / 0.2E1 - t398 / 0.2E1) * t46 - t942) 
     #* t46) * t118 / 0.2E1) / 0.6E1 - t528 * (((t962 - t393) * t118 - t
     #966) * t118 / 0.2E1 + (t966 - (t404 - t978) * t118) * t118 / 0.2E1
     #) / 0.6E1 + (t732 * t356 - t748 * t359) * t118 - t528 * ((t222 * t
     #996 - t230 * t1001) * t118 + ((t1007 - t409) * t118 - (t409 - t101
     #2) * t118) * t118) / 0.24E2
        t1022 = t97 * t1020 * t12
        t1025 = dt * dx
        t1027 = rx(t32,t115,0,0)
        t1028 = rx(t32,t115,1,1)
        t1030 = rx(t32,t115,1,0)
        t1031 = rx(t32,t115,0,1)
        t1033 = t1027 * t1028 - t1030 * t1031
        t1034 = 0.1E1 / t1033
        t981 = t4 * t1034 * (t1027 * t1030 + t1031 * t1028)
        t1042 = t981 * (t618 / 0.2E1 + t175 / 0.2E1)
        t1046 = t112 * (t498 / 0.2E1 + t104 / 0.2E1)
        t1049 = (t1042 - t1046) * t118 / 0.2E1
        t1050 = rx(t32,t120,0,0)
        t1051 = rx(t32,t120,1,1)
        t1053 = rx(t32,t120,1,0)
        t1054 = rx(t32,t120,0,1)
        t1056 = t1050 * t1051 - t1053 * t1054
        t1057 = 0.1E1 / t1056
        t992 = t4 * t1057 * (t1050 * t1053 + t1054 * t1051)
        t1065 = t992 * (t642 / 0.2E1 + t202 / 0.2E1)
        t1068 = (t1046 - t1065) * t118 / 0.2E1
        t1069 = t1030 ** 2
        t1070 = t1028 ** 2
        t1072 = t1034 * (t1069 + t1070)
        t1073 = t36 ** 2
        t1074 = t34 ** 2
        t1076 = t40 * (t1073 + t1074)
        t1079 = t4 * (t1072 / 0.2E1 + t1076 / 0.2E1)
        t1080 = t1079 * t119
        t1081 = t1053 ** 2
        t1082 = t1051 ** 2
        t1084 = t1057 * (t1081 + t1082)
        t1087 = t4 * (t1076 / 0.2E1 + t1084 / 0.2E1)
        t1088 = t1087 * t123
        t1092 = (t518 + t602 / 0.2E1 + t144 + t1049 + t1068 + (t1080 - t
     #1088) * t118) * t39
        t1094 = (t1092 - t235) * t46
        t1095 = t332 * t46
        t1097 = t1094 / 0.2E1 + t1095 / 0.2E1
        t1098 = t1025 * t1097
        t1106 = t495 * (t80 - dx * (t799 - t800) / 0.12E2) / 0.12E2
        t1107 = t808 * beta
        t1108 = t1107 * t337
        t1110 = t94 * t1095
        t1113 = rx(t468,t115,0,0)
        t1114 = rx(t468,t115,1,1)
        t1116 = rx(t468,t115,1,0)
        t1117 = rx(t468,t115,0,1)
        t1120 = 0.1E1 / (t1113 * t1114 - t1116 * t1117)
        t1121 = t1113 ** 2
        t1122 = t1117 ** 2
        t1125 = t1027 ** 2
        t1126 = t1031 ** 2
        t1128 = t1034 * (t1125 + t1126)
        t1133 = t162 ** 2
        t1134 = t166 ** 2
        t1136 = t169 * (t1133 + t1134)
        t1139 = t4 * (t1128 / 0.2E1 + t1136 / 0.2E1)
        t1140 = t1139 * t175
        t1147 = u(t468,t529,n)
        t1157 = t981 * (t532 / 0.2E1 + t119 / 0.2E1)
        t1164 = t171 * (t549 / 0.2E1 + t134 / 0.2E1)
        t1166 = (t1157 - t1164) * t46
        t1167 = t1166 / 0.2E1
        t1168 = rx(t32,t529,0,0)
        t1169 = rx(t32,t529,1,1)
        t1171 = rx(t32,t529,1,0)
        t1172 = rx(t32,t529,0,1)
        t1175 = 0.1E1 / (t1168 * t1169 - t1171 * t1172)
        t1189 = t1171 ** 2
        t1190 = t1169 ** 2
        t1100 = t4 * t1175 * (t1168 * t1171 + t1172 * t1169)
        t1200 = ((t4 * (t1120 * (t1121 + t1122) / 0.2E1 + t1128 / 0.2E1)
     # * t618 - t1140) * t46 + (t4 * t1120 * (t1113 * t1116 + t1117 * t1
     #114) * ((t1147 - t591) * t118 / 0.2E1 + t593 / 0.2E1) - t1157) * t
     #46 / 0.2E1 + t1167 + (t1100 * ((t1147 - t530) * t46 / 0.2E1 + t672
     # / 0.2E1) - t1042) * t118 / 0.2E1 + t1049 + (t4 * (t1175 * (t1189 
     #+ t1190) / 0.2E1 + t1072 / 0.2E1) * t532 - t1080) * t118) * t1033
        t1203 = rx(t468,t120,0,0)
        t1204 = rx(t468,t120,1,1)
        t1206 = rx(t468,t120,1,0)
        t1207 = rx(t468,t120,0,1)
        t1210 = 0.1E1 / (t1203 * t1204 - t1206 * t1207)
        t1211 = t1203 ** 2
        t1212 = t1207 ** 2
        t1215 = t1050 ** 2
        t1216 = t1054 ** 2
        t1218 = t1057 * (t1215 + t1216)
        t1223 = t189 ** 2
        t1224 = t193 ** 2
        t1226 = t196 * (t1223 + t1224)
        t1229 = t4 * (t1218 / 0.2E1 + t1226 / 0.2E1)
        t1230 = t1229 * t202
        t1237 = u(t468,t536,n)
        t1247 = t992 * (t123 / 0.2E1 + t539 / 0.2E1)
        t1254 = t194 * (t137 / 0.2E1 + t555 / 0.2E1)
        t1256 = (t1247 - t1254) * t46
        t1257 = t1256 / 0.2E1
        t1258 = rx(t32,t536,0,0)
        t1259 = rx(t32,t536,1,1)
        t1261 = rx(t32,t536,1,0)
        t1262 = rx(t32,t536,0,1)
        t1265 = 0.1E1 / (t1258 * t1259 - t1261 * t1262)
        t1279 = t1261 ** 2
        t1280 = t1259 ** 2
        t1187 = t4 * t1265 * (t1258 * t1261 + t1262 * t1259)
        t1290 = ((t4 * (t1210 * (t1211 + t1212) / 0.2E1 + t1218 / 0.2E1)
     # * t642 - t1230) * t46 + (t4 * t1210 * (t1203 * t1206 + t1207 * t1
     #204) * (t596 / 0.2E1 + (t594 - t1237) * t118 / 0.2E1) - t1247) * t
     #46 / 0.2E1 + t1257 + t1068 + (t1065 - t1187 * ((t1237 - t537) * t4
     #6 / 0.2E1 + t700 / 0.2E1)) * t118 / 0.2E1 + (t1088 - t4 * (t1084 /
     # 0.2E1 + t1265 * (t1279 + t1280) / 0.2E1) * t539) * t118) * t1056
        t1297 = t262 ** 2
        t1298 = t266 ** 2
        t1300 = t269 * (t1297 + t1298)
        t1303 = t4 * (t1136 / 0.2E1 + t1300 / 0.2E1)
        t1304 = t1303 * t177
        t1306 = (t1140 - t1304) * t46
        t1310 = t264 * (t567 / 0.2E1 + t151 / 0.2E1)
        t1312 = (t1164 - t1310) * t46
        t1313 = t1312 / 0.2E1
        t1314 = t680 / 0.2E1
        t1316 = (t1306 + t1167 + t1313 + t1314 + t188 + t771) * t168
        t1318 = (t1316 - t235) * t118
        t1319 = t287 ** 2
        t1320 = t291 ** 2
        t1322 = t294 * (t1319 + t1320)
        t1325 = t4 * (t1226 / 0.2E1 + t1322 / 0.2E1)
        t1326 = t1325 * t204
        t1328 = (t1230 - t1326) * t46
        t1332 = t284 * (t154 / 0.2E1 + t573 / 0.2E1)
        t1334 = (t1254 - t1332) * t46
        t1335 = t1334 / 0.2E1
        t1336 = t708 / 0.2E1
        t1338 = (t1328 + t1257 + t1335 + t211 + t1336 + t779) * t195
        t1340 = (t235 - t1338) * t118
        t1344 = t129 * (t1318 / 0.2E1 + t1340 / 0.2E1)
        t1348 = rx(t48,t115,0,0)
        t1349 = rx(t48,t115,1,1)
        t1351 = rx(t48,t115,1,0)
        t1352 = rx(t48,t115,0,1)
        t1354 = t1348 * t1349 - t1351 * t1352
        t1355 = 0.1E1 / t1354
        t1356 = t1348 ** 2
        t1357 = t1352 ** 2
        t1359 = t1355 * (t1356 + t1357)
        t1362 = t4 * (t1300 / 0.2E1 + t1359 / 0.2E1)
        t1363 = t1362 * t275
        t1365 = (t1304 - t1363) * t46
        t1370 = u(t48,t529,n)
        t1372 = (t1370 - t249) * t118
        t1255 = t4 * t1355 * (t1348 * t1351 + t1352 * t1349)
        t1376 = t1255 * (t1372 / 0.2E1 + t251 / 0.2E1)
        t1378 = (t1310 - t1376) * t46
        t1379 = t1378 / 0.2E1
        t1380 = rx(i,t529,0,0)
        t1381 = rx(i,t529,1,1)
        t1383 = rx(i,t529,1,0)
        t1384 = rx(i,t529,0,1)
        t1386 = t1380 * t1381 - t1383 * t1384
        t1387 = 0.1E1 / t1386
        t1391 = t1380 * t1383 + t1384 * t1381
        t1393 = (t565 - t1370) * t46
        t1273 = t4 * t1387 * t1391
        t1397 = t1273 * (t674 / 0.2E1 + t1393 / 0.2E1)
        t1399 = (t1397 - t279) * t118
        t1400 = t1399 / 0.2E1
        t1401 = t1383 ** 2
        t1402 = t1381 ** 2
        t1404 = t1387 * (t1401 + t1402)
        t1407 = t4 * (t1404 / 0.2E1 + t311 / 0.2E1)
        t1408 = t1407 * t567
        t1410 = (t1408 - t319) * t118
        t1412 = (t1365 + t1313 + t1379 + t1400 + t286 + t1410) * t268
        t1413 = t1412 - t331
        t1414 = t1413 * t118
        t1415 = rx(t48,t120,0,0)
        t1416 = rx(t48,t120,1,1)
        t1418 = rx(t48,t120,1,0)
        t1419 = rx(t48,t120,0,1)
        t1421 = t1415 * t1416 - t1418 * t1419
        t1422 = 0.1E1 / t1421
        t1423 = t1415 ** 2
        t1424 = t1419 ** 2
        t1426 = t1422 * (t1423 + t1424)
        t1429 = t4 * (t1322 / 0.2E1 + t1426 / 0.2E1)
        t1430 = t1429 * t300
        t1432 = (t1326 - t1430) * t46
        t1437 = u(t48,t536,n)
        t1439 = (t252 - t1437) * t118
        t1294 = t4 * t1422 * (t1415 * t1418 + t1419 * t1416)
        t1443 = t1294 * (t254 / 0.2E1 + t1439 / 0.2E1)
        t1445 = (t1332 - t1443) * t46
        t1446 = t1445 / 0.2E1
        t1447 = rx(i,t536,0,0)
        t1448 = rx(i,t536,1,1)
        t1450 = rx(i,t536,1,0)
        t1451 = rx(i,t536,0,1)
        t1453 = t1447 * t1448 - t1450 * t1451
        t1454 = 0.1E1 / t1453
        t1458 = t1447 * t1450 + t1451 * t1448
        t1460 = (t571 - t1437) * t46
        t1309 = t4 * t1454 * t1458
        t1464 = t1309 * (t702 / 0.2E1 + t1460 / 0.2E1)
        t1466 = (t304 - t1464) * t118
        t1467 = t1466 / 0.2E1
        t1468 = t1450 ** 2
        t1469 = t1448 ** 2
        t1471 = t1454 * (t1468 + t1469)
        t1474 = t4 * (t323 / 0.2E1 + t1471 / 0.2E1)
        t1475 = t1474 * t573
        t1477 = (t327 - t1475) * t118
        t1479 = (t1432 + t1335 + t1446 + t307 + t1467 + t1477) * t293
        t1480 = t331 - t1479
        t1481 = t1480 * t118
        t1483 = t1414 / 0.2E1 + t1481 / 0.2E1
        t1485 = t146 * t1483
        t1488 = (t1344 - t1485) * t46 / 0.2E1
        t1492 = (t1316 - t1412) * t46
        t1498 = t129 * t1097
        t1505 = (t1338 - t1479) * t46
        t1519 = t339 * ((t100 * t1094 - t1110) * t46 + (t112 * ((t1200 -
     # t1092) * t118 / 0.2E1 + (t1092 - t1290) * t118 / 0.2E1) - t1344) 
     #* t46 / 0.2E1 + t1488 + (t171 * ((t1200 - t1316) * t46 / 0.2E1 + t
     #1492 / 0.2E1) - t1498) * t118 / 0.2E1 + (t1498 - t194 * ((t1290 - 
     #t1338) * t46 / 0.2E1 + t1505 / 0.2E1)) * t118 / 0.2E1 + (t222 * t1
     #318 - t230 * t1340) * t118) * t12
        t1522 = t97 * dx
        t1531 = t112 * (t795 / 0.2E1 + t78 / 0.2E1)
        t1550 = t456 * t46
        t1553 = t1522 * (((t820 + t896 / 0.2E1 + t366 + (t981 * (t912 / 
     #0.2E1 + t381 / 0.2E1) - t1531) * t118 / 0.2E1 + (t1531 - t992 * (t
     #936 / 0.2E1 + t396 / 0.2E1)) * t118 / 0.2E1 + (t1079 * t346 - t108
     #7 * t349) * t118) * t39 - t411) * t46 / 0.2E1 + t1550 / 0.2E1)
        t1557 = t1025 * (t1094 - t1095)
        t1560 = t60 / 0.2E1
        t1561 = i - 2
        t1562 = rx(t1561,j,0,0)
        t1563 = rx(t1561,j,1,1)
        t1565 = rx(t1561,j,1,0)
        t1566 = rx(t1561,j,0,1)
        t1568 = t1562 * t1563 - t1565 * t1566
        t1569 = 0.1E1 / t1568
        t1570 = t1562 ** 2
        t1571 = t1566 ** 2
        t1573 = t1569 * (t1570 + t1571)
        t1580 = t31 + t1560 - dx * (t484 / 0.2E1 - (t60 - t1573) * t46 /
     # 0.2E1) / 0.8E1
        t1581 = t4 * t1580
        t1582 = t1581 * t241
        t1585 = u(t1561,j,n)
        t1587 = (t239 - t1585) * t46
        t1589 = (t241 - t1587) * t46
        t1590 = t507 - t1589
        t1591 = t1590 * t46
        t1592 = t238 * t1591
        t1597 = t4 * (t60 / 0.2E1 + t1573 / 0.2E1)
        t1598 = t1597 * t1587
        t1600 = (t242 - t1598) * t46
        t1601 = t244 - t1600
        t1602 = t1601 * t46
        t1610 = (t1372 / 0.2E1 - t254 / 0.2E1) * t118
        t1613 = (t251 / 0.2E1 - t1439 / 0.2E1) * t118
        t1617 = t243 * (t1610 - t1613) * t118
        t1619 = (t580 - t1617) * t46
        t1627 = t1562 * t1565 + t1566 * t1563
        t1628 = u(t1561,t115,n)
        t1630 = (t1628 - t1585) * t118
        t1631 = u(t1561,t120,n)
        t1633 = (t1585 - t1631) * t118
        t1514 = t4 * t1569 * t1627
        t1637 = t1514 * (t1630 / 0.2E1 + t1633 / 0.2E1)
        t1639 = (t258 - t1637) * t46
        t1641 = (t260 - t1639) * t46
        t1643 = (t610 - t1641) * t46
        t1649 = (t249 - t1628) * t46
        t1652 = (t177 / 0.2E1 - t1649 / 0.2E1) * t46
        t1656 = t264 * (t624 - t1652) * t46
        t1659 = (t107 / 0.2E1 - t1587 / 0.2E1) * t46
        t1663 = t146 * (t634 - t1659) * t46
        t1665 = (t1656 - t1663) * t118
        t1667 = (t252 - t1631) * t46
        t1670 = (t204 / 0.2E1 - t1667 / 0.2E1) * t46
        t1674 = t284 * (t648 - t1670) * t46
        t1676 = (t1663 - t1674) * t118
        t1682 = (t1399 - t285) * t118
        t1684 = (t285 - t306) * t118
        t1686 = (t1682 - t1684) * t118
        t1688 = (t306 - t1466) * t118
        t1690 = (t1684 - t1688) * t118
        t1695 = t311 / 0.2E1
        t1696 = t315 / 0.2E1
        t1700 = (t315 - t323) * t118
        t1705 = t1695 + t1696 - dy * ((t1404 - t311) * t118 / 0.2E1 - t1
     #700 / 0.2E1) / 0.8E1
        t1706 = t4 * t1705
        t1707 = t1706 * t151
        t1708 = t323 / 0.2E1
        t1710 = (t311 - t315) * t118
        t1717 = t1696 + t1708 - dy * (t1710 / 0.2E1 - (t323 - t1471) * t
     #118 / 0.2E1) / 0.8E1
        t1718 = t4 * t1717
        t1719 = t1718 * t154
        t1723 = (t567 - t151) * t118
        t1725 = (t151 - t154) * t118
        t1726 = t1723 - t1725
        t1727 = t1726 * t118
        t1728 = t318 * t1727
        t1730 = (t154 - t573) * t118
        t1731 = t1725 - t1730
        t1732 = t1731 * t118
        t1733 = t326 * t1732
        t1736 = t1410 - t329
        t1737 = t1736 * t118
        t1738 = t329 - t1477
        t1739 = t1738 * t118
        t1745 = (t492 - t1582) * t46 - t495 * ((t510 - t1592) * t46 + (t
     #522 - t1602) * t46) / 0.24E2 + t161 + t261 - t528 * (t582 / 0.2E1 
     #+ t1619 / 0.2E1) / 0.6E1 - t495 * (t612 / 0.2E1 + t1643 / 0.2E1) /
     # 0.6E1 + t286 + t307 - t495 * (t1665 / 0.2E1 + t1676 / 0.2E1) / 0.
     #6E1 - t528 * (t1686 / 0.2E1 + t1690 / 0.2E1) / 0.6E1 + (t1707 - t1
     #719) * t118 - t528 * ((t1728 - t1733) * t118 + (t1737 - t1739) * t
     #118) / 0.24E2
        t1747 = dt * t1745 * t25
        t1748 = t466 * t1747
        t1749 = t83 / 0.2E1
        t1750 = ut(t1561,j,n)
        t1752 = (t81 - t1750) * t46
        t1754 = (t83 - t1752) * t46
        t1755 = t85 - t1754
        t1756 = t1755 * t46
        t1759 = t495 * (t800 / 0.2E1 + t1756 / 0.2E1)
        t1760 = t1759 / 0.6E1
        t1763 = dx * (t792 + t1749 - t1760) / 0.2E1
        t1764 = t1581 * t83
        t1767 = t238 * t1756
        t1770 = t1597 * t1752
        t1772 = (t412 - t1770) * t46
        t1773 = t414 - t1772
        t1774 = t1773 * t46
        t1780 = ut(t48,t529,n)
        t1782 = (t1780 - t415) * t118
        t1785 = (t1782 / 0.2E1 - t420 / 0.2E1) * t118
        t1786 = ut(t48,t536,n)
        t1788 = (t418 - t1786) * t118
        t1791 = (t417 / 0.2E1 - t1788 / 0.2E1) * t118
        t1795 = t243 * (t1785 - t1791) * t118
        t1797 = (t878 - t1795) * t46
        t1802 = ut(t1561,t115,n)
        t1804 = (t1802 - t1750) * t118
        t1805 = ut(t1561,t120,n)
        t1807 = (t1750 - t1805) * t118
        t1811 = t1514 * (t1804 / 0.2E1 + t1807 / 0.2E1)
        t1813 = (t424 - t1811) * t46
        t1815 = (t426 - t1813) * t46
        t1817 = (t904 - t1815) * t46
        t1823 = (t415 - t1802) * t46
        t1826 = (t383 / 0.2E1 - t1823 / 0.2E1) * t46
        t1830 = t264 * (t918 - t1826) * t46
        t1833 = (t75 / 0.2E1 - t1752 / 0.2E1) * t46
        t1837 = t146 * (t928 - t1833) * t46
        t1839 = (t1830 - t1837) * t118
        t1841 = (t418 - t1805) * t46
        t1844 = (t398 / 0.2E1 - t1841 / 0.2E1) * t46
        t1848 = t284 * (t942 - t1844) * t46
        t1850 = (t1837 - t1848) * t118
        t1856 = (t863 - t1780) * t46
        t1860 = t1273 * (t956 / 0.2E1 + t1856 / 0.2E1)
        t1862 = (t1860 - t433) * t118
        t1864 = (t1862 - t439) * t118
        t1866 = (t439 - t448) * t118
        t1868 = (t1864 - t1866) * t118
        t1870 = (t869 - t1786) * t46
        t1874 = t1309 * (t972 / 0.2E1 + t1870 / 0.2E1)
        t1876 = (t446 - t1874) * t118
        t1878 = (t448 - t1876) * t118
        t1880 = (t1866 - t1878) * t118
        t1885 = t1706 * t369
        t1886 = t1718 * t372
        t1890 = (t865 - t369) * t118
        t1892 = (t369 - t372) * t118
        t1893 = t1890 - t1892
        t1894 = t1893 * t118
        t1895 = t318 * t1894
        t1897 = (t372 - t871) * t118
        t1898 = t1892 - t1897
        t1899 = t1898 * t118
        t1900 = t326 * t1899
        t1903 = t1407 * t865
        t1905 = (t1903 - t450) * t118
        t1906 = t1905 - t453
        t1907 = t1906 * t118
        t1908 = t1474 * t871
        t1910 = (t451 - t1908) * t118
        t1911 = t453 - t1910
        t1912 = t1911 * t118
        t1918 = (t811 - t1764) * t46 - t495 * ((t815 - t1767) * t46 + (t
     #823 - t1774) * t46) / 0.24E2 + t379 + t427 - t528 * (t880 / 0.2E1 
     #+ t1797 / 0.2E1) / 0.6E1 - t495 * (t906 / 0.2E1 + t1817 / 0.2E1) /
     # 0.6E1 + t440 + t449 - t495 * (t1839 / 0.2E1 + t1850 / 0.2E1) / 0.
     #6E1 - t528 * (t1868 / 0.2E1 + t1880 / 0.2E1) / 0.6E1 + (t1885 - t1
     #886) * t118 - t528 * ((t1895 - t1900) * t118 + (t1907 - t1912) * t
     #118) / 0.24E2
        t1920 = t97 * t1918 * t25
        t1922 = t809 * t1920 / 0.2E1
        t1923 = t1639 / 0.2E1
        t1927 = t1255 * (t275 / 0.2E1 + t1649 / 0.2E1)
        t1931 = t243 * (t241 / 0.2E1 + t1587 / 0.2E1)
        t1933 = (t1927 - t1931) * t118
        t1934 = t1933 / 0.2E1
        t1938 = t1294 * (t300 / 0.2E1 + t1667 / 0.2E1)
        t1940 = (t1931 - t1938) * t118
        t1941 = t1940 / 0.2E1
        t1942 = t1351 ** 2
        t1943 = t1349 ** 2
        t1945 = t1355 * (t1942 + t1943)
        t1946 = t52 ** 2
        t1947 = t50 ** 2
        t1949 = t56 * (t1946 + t1947)
        t1952 = t4 * (t1945 / 0.2E1 + t1949 / 0.2E1)
        t1953 = t1952 * t251
        t1954 = t1418 ** 2
        t1955 = t1416 ** 2
        t1957 = t1422 * (t1954 + t1955)
        t1960 = t4 * (t1949 / 0.2E1 + t1957 / 0.2E1)
        t1961 = t1960 * t254
        t1963 = (t1953 - t1961) * t118
        t1965 = (t1600 + t261 + t1923 + t1934 + t1941 + t1963) * t55
        t1966 = t331 - t1965
        t1967 = t1966 * t46
        t1969 = t1095 / 0.2E1 + t1967 / 0.2E1
        t1970 = t1025 * t1969
        t1972 = t466 * t1970 / 0.2E1
        t1978 = t495 * (t85 - dx * (t800 - t1756) / 0.12E2) / 0.12E2
        t1979 = t238 * t1967
        t1982 = rx(t1561,t115,0,0)
        t1983 = rx(t1561,t115,1,1)
        t1985 = rx(t1561,t115,1,0)
        t1986 = rx(t1561,t115,0,1)
        t1988 = t1982 * t1983 - t1985 * t1986
        t1989 = 0.1E1 / t1988
        t1990 = t1982 ** 2
        t1991 = t1986 ** 2
        t1993 = t1989 * (t1990 + t1991)
        t1996 = t4 * (t1359 / 0.2E1 + t1993 / 0.2E1)
        t1997 = t1996 * t1649
        t1999 = (t1363 - t1997) * t46
        t2004 = u(t1561,t529,n)
        t2006 = (t2004 - t1628) * t118
        t1845 = t4 * t1989 * (t1982 * t1985 + t1986 * t1983)
        t2010 = t1845 * (t2006 / 0.2E1 + t1630 / 0.2E1)
        t2012 = (t1376 - t2010) * t46
        t2013 = t2012 / 0.2E1
        t2014 = rx(t48,t529,0,0)
        t2015 = rx(t48,t529,1,1)
        t2017 = rx(t48,t529,1,0)
        t2018 = rx(t48,t529,0,1)
        t2020 = t2014 * t2015 - t2017 * t2018
        t2021 = 0.1E1 / t2020
        t2027 = (t1370 - t2004) * t46
        t1857 = t4 * t2021 * (t2014 * t2017 + t2018 * t2015)
        t2031 = t1857 * (t1393 / 0.2E1 + t2027 / 0.2E1)
        t2033 = (t2031 - t1927) * t118
        t2034 = t2033 / 0.2E1
        t2035 = t2017 ** 2
        t2036 = t2015 ** 2
        t2038 = t2021 * (t2035 + t2036)
        t2041 = t4 * (t2038 / 0.2E1 + t1945 / 0.2E1)
        t2042 = t2041 * t1372
        t2044 = (t2042 - t1953) * t118
        t2046 = (t1999 + t1379 + t2013 + t2034 + t1934 + t2044) * t1354
        t2048 = (t2046 - t1965) * t118
        t2049 = rx(t1561,t120,0,0)
        t2050 = rx(t1561,t120,1,1)
        t2052 = rx(t1561,t120,1,0)
        t2053 = rx(t1561,t120,0,1)
        t2055 = t2049 * t2050 - t2052 * t2053
        t2056 = 0.1E1 / t2055
        t2057 = t2049 ** 2
        t2058 = t2053 ** 2
        t2060 = t2056 * (t2057 + t2058)
        t2063 = t4 * (t1426 / 0.2E1 + t2060 / 0.2E1)
        t2064 = t2063 * t1667
        t2066 = (t1430 - t2064) * t46
        t2071 = u(t1561,t536,n)
        t2073 = (t1631 - t2071) * t118
        t1888 = t4 * t2056 * (t2049 * t2052 + t2053 * t2050)
        t2077 = t1888 * (t1633 / 0.2E1 + t2073 / 0.2E1)
        t2079 = (t1443 - t2077) * t46
        t2080 = t2079 / 0.2E1
        t2081 = rx(t48,t536,0,0)
        t2082 = rx(t48,t536,1,1)
        t2084 = rx(t48,t536,1,0)
        t2085 = rx(t48,t536,0,1)
        t2087 = t2081 * t2082 - t2084 * t2085
        t2088 = 0.1E1 / t2087
        t2094 = (t1437 - t2071) * t46
        t1914 = t4 * t2088 * (t2081 * t2084 + t2085 * t2082)
        t2098 = t1914 * (t1460 / 0.2E1 + t2094 / 0.2E1)
        t2100 = (t1938 - t2098) * t118
        t2101 = t2100 / 0.2E1
        t2102 = t2084 ** 2
        t2103 = t2082 ** 2
        t2105 = t2088 * (t2102 + t2103)
        t2108 = t4 * (t1957 / 0.2E1 + t2105 / 0.2E1)
        t2109 = t2108 * t1439
        t2111 = (t1961 - t2109) * t118
        t2113 = (t2066 + t1446 + t2080 + t1941 + t2101 + t2111) * t1421
        t2115 = (t1965 - t2113) * t118
        t2119 = t243 * (t2048 / 0.2E1 + t2115 / 0.2E1)
        t2122 = (t1485 - t2119) * t46 / 0.2E1
        t2124 = (t1412 - t2046) * t46
        t2128 = t264 * (t1492 / 0.2E1 + t2124 / 0.2E1)
        t2130 = t146 * t1969
        t2133 = (t2128 - t2130) * t118 / 0.2E1
        t2135 = (t1479 - t2113) * t46
        t2139 = t284 * (t1505 / 0.2E1 + t2135 / 0.2E1)
        t2142 = (t2130 - t2139) * t118 / 0.2E1
        t2143 = t318 * t1414
        t2144 = t326 * t1481
        t2149 = t339 * ((t1110 - t1979) * t46 + t1488 + t2122 + t2133 + 
     #t2142 + (t2143 - t2144) * t118) * t25
        t2151 = t1108 * t2149 / 0.6E1
        t2152 = t1813 / 0.2E1
        t2156 = t1255 * (t429 / 0.2E1 + t1823 / 0.2E1)
        t2160 = t243 * (t83 / 0.2E1 + t1752 / 0.2E1)
        t2162 = (t2156 - t2160) * t118
        t2163 = t2162 / 0.2E1
        t2167 = t1294 * (t442 / 0.2E1 + t1841 / 0.2E1)
        t2169 = (t2160 - t2167) * t118
        t2170 = t2169 / 0.2E1
        t2171 = t1952 * t417
        t2172 = t1960 * t420
        t2174 = (t2171 - t2172) * t118
        t2176 = (t1772 + t427 + t2152 + t2163 + t2170 + t2174) * t55
        t2177 = t455 - t2176
        t2178 = t2177 * t46
        t2181 = t1522 * (t1550 / 0.2E1 + t2178 / 0.2E1)
        t2183 = t809 * t2181 / 0.4E1
        t2185 = t1025 * (t1095 - t1967)
        t2187 = t466 * t2185 / 0.12E2
        t2188 = t73 + t466 * t789 - t807 + t809 * t1022 / 0.2E1 - t466 *
     # t1098 / 0.2E1 + t1106 + t1108 * t1519 / 0.6E1 - t809 * t1553 / 0.
     #4E1 + t466 * t1557 / 0.12E2 - t2 - t1748 - t1763 - t1922 - t1972 -
     # t1978 - t2151 - t2183 - t2187
        t2191 = 0.8E1 * t27
        t2192 = 0.8E1 * t28
        t2202 = sqrt(0.8E1 * t14 + 0.8E1 * t15 + t2191 + t2192 - 0.2E1 *
     # dx * ((t41 + t42 - t14 - t15) * t46 / 0.2E1 - (t27 + t28 - t57 - 
     #t58) * t46 / 0.2E1))
        t2203 = 0.1E1 / t2202
        t2207 = 0.1E1 / 0.2E1 - t70
        t2208 = t2207 * dt
        t2210 = t68 * t2208 * t89
        t2211 = t2207 ** 2
        t2214 = t94 * t2211 * t334 / 0.2E1
        t2215 = t2211 * t2207
        t2218 = t94 * t2215 * t458 / 0.6E1
        t2220 = t2208 * t462 / 0.24E2
        t2221 = beta * t2207
        t2223 = t808 * t2211
        t2228 = t1107 * t2215
        t2235 = t2221 * t1747
        t2237 = t2223 * t1920 / 0.2E1
        t2239 = t2221 * t1970 / 0.2E1
        t2241 = t2228 * t2149 / 0.6E1
        t2243 = t2223 * t2181 / 0.4E1
        t2245 = t2221 * t2185 / 0.12E2
        t2246 = t73 + t2221 * t789 - t807 + t2223 * t1022 / 0.2E1 - t222
     #1 * t1098 / 0.2E1 + t1106 + t2228 * t1519 / 0.6E1 - t2223 * t1553 
     #/ 0.4E1 + t2221 * t1557 / 0.12E2 - t2 - t2235 - t1763 - t2237 - t2
     #239 - t1978 - t2241 - t2243 - t2245
        t2249 = 0.2E1 * t465 * t2246 * t2203
        t2251 = (t68 * t72 * t89 + t94 * t95 * t334 / 0.2E1 + t94 * t337
     # * t458 / 0.6E1 - t72 * t462 / 0.24E2 + 0.2E1 * t465 * t2188 * t22
     #03 - t2210 - t2214 - t2218 + t2220 - t2249) * t69
        t2257 = t68 * (t107 - dx * t508 / 0.24E2)
        t2259 = dx * t521 / 0.24E2
        t2264 = t13 * t131
        t2266 = t26 * t148
        t2267 = t2266 / 0.2E1
        t2271 = t56 * t248
        t2279 = t4 * (t2264 / 0.2E1 + t2267 - dx * ((t40 * t114 - t2264)
     # * t46 / 0.2E1 - (t2266 - t2271) * t46 / 0.2E1) / 0.8E1)
        t2284 = t528 * (t996 / 0.2E1 + t1001 / 0.2E1)
        t2286 = t369 / 0.4E1
        t2287 = t372 / 0.4E1
        t2290 = t528 * (t1894 / 0.2E1 + t1899 / 0.2E1)
        t2291 = t2290 / 0.12E2
        t2297 = (t346 - t349) * t118
        t2308 = t356 / 0.2E1
        t2309 = t359 / 0.2E1
        t2310 = t2284 / 0.6E1
        t2313 = t369 / 0.2E1
        t2314 = t372 / 0.2E1
        t2315 = t2290 / 0.6E1
        t2316 = t417 / 0.2E1
        t2317 = t420 / 0.2E1
        t2321 = (t417 - t420) * t118
        t2323 = ((t1782 - t417) * t118 - t2321) * t118
        t2327 = (t2321 - (t420 - t1788) * t118) * t118
        t2330 = t528 * (t2323 / 0.2E1 + t2327 / 0.2E1)
        t2331 = t2330 / 0.6E1
        t2338 = t356 / 0.4E1 + t359 / 0.4E1 - t2284 / 0.12E2 + t2286 + t
     #2287 - t2291 - dx * ((t346 / 0.2E1 + t349 / 0.2E1 - t528 * (((t831
     # - t346) * t118 - t2297) * t118 / 0.2E1 + (t2297 - (t349 - t837) *
     # t118) * t118 / 0.2E1) / 0.6E1 - t2308 - t2309 + t2310) * t46 / 0.
     #2E1 - (t2313 + t2314 - t2315 - t2316 - t2317 + t2331) * t46 / 0.2E
     #1) / 0.8E1
        t2343 = t4 * (t2264 / 0.2E1 + t2266 / 0.2E1)
        t2344 = t95 * t97
        t2346 = t1318 / 0.4E1 + t1340 / 0.4E1 + t1414 / 0.4E1 + t1481 / 
     #0.4E1
        t2350 = t337 * t339
        t2352 = t1303 * t383
        t2354 = (t1139 * t381 - t2352) * t46
        t2362 = t171 * (t847 / 0.2E1 + t356 / 0.2E1)
        t2364 = (t981 * (t831 / 0.2E1 + t346 / 0.2E1) - t2362) * t46
        t2369 = t264 * (t865 / 0.2E1 + t369 / 0.2E1)
        t2371 = (t2362 - t2369) * t46
        t2372 = t2371 / 0.2E1
        t2375 = (t2354 + t2364 / 0.2E1 + t2372 + t962 / 0.2E1 + t394 + t
     #1007) * t168
        t2379 = t1325 * t398
        t2381 = (t1229 * t396 - t2379) * t46
        t2389 = t194 * (t359 / 0.2E1 + t853 / 0.2E1)
        t2391 = (t992 * (t349 / 0.2E1 + t837 / 0.2E1) - t2389) * t46
        t2396 = t284 * (t372 / 0.2E1 + t871 / 0.2E1)
        t2398 = (t2389 - t2396) * t46
        t2399 = t2398 / 0.2E1
        t2402 = (t2381 + t2391 / 0.2E1 + t2399 + t405 + t978 / 0.2E1 + t
     #1012) * t195
        t2405 = t1362 * t429
        t2407 = (t2352 - t2405) * t46
        t2411 = t1255 * (t1782 / 0.2E1 + t417 / 0.2E1)
        t2413 = (t2369 - t2411) * t46
        t2414 = t2413 / 0.2E1
        t2415 = t1862 / 0.2E1
        t2417 = (t2407 + t2372 + t2414 + t2415 + t440 + t1905) * t268
        t2418 = t2417 - t455
        t2419 = t2418 * t118
        t2420 = t1429 * t442
        t2422 = (t2379 - t2420) * t46
        t2426 = t1294 * (t420 / 0.2E1 + t1788 / 0.2E1)
        t2428 = (t2396 - t2426) * t46
        t2429 = t2428 / 0.2E1
        t2430 = t1876 / 0.2E1
        t2432 = (t2422 + t2399 + t2429 + t449 + t2430 + t1910) * t293
        t2433 = t455 - t2432
        t2434 = t2433 * t118
        t2436 = (t2375 - t411) * t118 / 0.4E1 + (t411 - t2402) * t118 / 
     #0.4E1 + t2419 / 0.4E1 + t2434 / 0.4E1
        t2442 = dx * (t365 / 0.2E1 - t426 / 0.2E1)
        t2446 = t2279 * t2208 * t2338
        t2447 = t2211 * t97
        t2450 = t2343 * t2447 * t2346 / 0.2E1
        t2451 = t2215 * t339
        t2454 = t2343 * t2451 * t2436 / 0.6E1
        t2456 = t2208 * t2442 / 0.24E2
        t2458 = (t2279 * t72 * t2338 + t2343 * t2344 * t2346 / 0.2E1 + t
     #2343 * t2350 * t2436 / 0.6E1 - t72 * t2442 / 0.24E2 - t2446 - t245
     #0 - t2454 + t2456) * t69
        t2465 = t528 * (t757 / 0.2E1 + t762 / 0.2E1)
        t2467 = t151 / 0.4E1
        t2468 = t154 / 0.4E1
        t2471 = t528 * (t1727 / 0.2E1 + t1732 / 0.2E1)
        t2472 = t2471 / 0.12E2
        t2478 = (t119 - t123) * t118
        t2489 = t134 / 0.2E1
        t2490 = t137 / 0.2E1
        t2491 = t2465 / 0.6E1
        t2494 = t151 / 0.2E1
        t2495 = t154 / 0.2E1
        t2496 = t2471 / 0.6E1
        t2497 = t251 / 0.2E1
        t2498 = t254 / 0.2E1
        t2502 = (t251 - t254) * t118
        t2504 = ((t1372 - t251) * t118 - t2502) * t118
        t2508 = (t2502 - (t254 - t1439) * t118) * t118
        t2511 = t528 * (t2504 / 0.2E1 + t2508 / 0.2E1)
        t2512 = t2511 / 0.6E1
        t2520 = t2279 * (t134 / 0.4E1 + t137 / 0.4E1 - t2465 / 0.12E2 + 
     #t2467 + t2468 - t2472 - dx * ((t119 / 0.2E1 + t123 / 0.2E1 - t528 
     #* (((t532 - t119) * t118 - t2478) * t118 / 0.2E1 + (t2478 - (t123 
     #- t539) * t118) * t118 / 0.2E1) / 0.6E1 - t2489 - t2490 + t2491) *
     # t46 / 0.2E1 - (t2494 + t2495 - t2496 - t2497 - t2498 + t2512) * t
     #46 / 0.2E1) / 0.8E1)
        t2524 = dx * (t143 / 0.2E1 - t260 / 0.2E1) / 0.24E2
        t2531 = t83 - dx * t1755 / 0.24E2
        t2536 = t97 * t1966 * t46
        t2541 = t339 * t2177 * t46
        t2544 = dx * t1773
        t2547 = cc * t1580
        t2549 = i - 3
        t2550 = rx(t2549,j,0,0)
        t2551 = rx(t2549,j,1,1)
        t2553 = rx(t2549,j,1,0)
        t2554 = rx(t2549,j,0,1)
        t2557 = 0.1E1 / (t2550 * t2551 - t2553 * t2554)
        t2558 = t2550 ** 2
        t2559 = t2554 ** 2
        t2561 = t2557 * (t2558 + t2559)
        t2569 = t4 * (t1560 + t1573 / 0.2E1 - dx * (t62 / 0.2E1 - (t1573
     # - t2561) * t46 / 0.2E1) / 0.8E1)
        t2573 = u(t2549,j,n)
        t2575 = (t1585 - t2573) * t46
        t2585 = t4 * (t1573 / 0.2E1 + t2561 / 0.2E1)
        t2588 = (t1598 - t2585 * t2575) * t46
        t2616 = u(t2549,t115,n)
        t2618 = (t2616 - t2573) * t118
        t2619 = u(t2549,t120,n)
        t2621 = (t2573 - t2619) * t118
        t2440 = t4 * t2557 * (t2550 * t2553 + t2554 * t2551)
        t2627 = (t1637 - t2440 * (t2618 / 0.2E1 + t2621 / 0.2E1)) * t46
        t2637 = (t1628 - t2616) * t46
        t2651 = t243 * (t1659 - (t241 / 0.2E1 - t2575 / 0.2E1) * t46) * 
     #t46
        t2655 = (t1631 - t2619) * t46
        t2672 = (t1933 - t1940) * t118
        t2684 = t1949 / 0.2E1
        t2694 = t4 * (t1945 / 0.2E1 + t2684 - dy * ((t2038 - t1945) * t1
     #18 / 0.2E1 - (t1949 - t1957) * t118 / 0.2E1) / 0.8E1)
        t2706 = t4 * (t2684 + t1957 / 0.2E1 - dy * ((t1945 - t1949) * t1
     #18 / 0.2E1 - (t1957 - t2105) * t118 / 0.2E1) / 0.8E1)
        t2723 = (t1582 - t2569 * t1587) * t46 - t495 * ((t1592 - t1597 *
     # (t1589 - (t1587 - t2575) * t46) * t46) * t46 + (t1602 - (t1600 - 
     #t2588) * t46) * t46) / 0.24E2 + t261 + t1923 - t528 * (t1619 / 0.2
     #E1 + (t1617 - t1514 * ((t2006 / 0.2E1 - t1633 / 0.2E1) * t118 - (t
     #1630 / 0.2E1 - t2073 / 0.2E1) * t118) * t118) * t46 / 0.2E1) / 0.6
     #E1 - t495 * (t1643 / 0.2E1 + (t1641 - (t1639 - t2627) * t46) * t46
     # / 0.2E1) / 0.6E1 + t1934 + t1941 - t495 * ((t1255 * (t1652 - (t27
     #5 / 0.2E1 - t2637 / 0.2E1) * t46) * t46 - t2651) * t118 / 0.2E1 + 
     #(t2651 - t1294 * (t1670 - (t300 / 0.2E1 - t2655 / 0.2E1) * t46) * 
     #t46) * t118 / 0.2E1) / 0.6E1 - t528 * (((t2033 - t1933) * t118 - t
     #2672) * t118 / 0.2E1 + (t2672 - (t1940 - t2100) * t118) * t118 / 0
     #.2E1) / 0.6E1 + (t2694 * t251 - t2706 * t254) * t118 - t528 * ((t1
     #952 * t2504 - t1960 * t2508) * t118 + ((t2044 - t1963) * t118 - (t
     #1963 - t2111) * t118) * t118) / 0.24E2
        t2725 = dt * t2723 * t55
        t2728 = ut(t2549,j,n)
        t2730 = (t1750 - t2728) * t46
        t2734 = (t1754 - (t1752 - t2730) * t46) * t46
        t2741 = dx * (t1749 + t1752 / 0.2E1 - t495 * (t1756 / 0.2E1 + t2
     #734 / 0.2E1) / 0.6E1) / 0.2E1
        t2750 = (t1770 - t2585 * t2730) * t46
        t2758 = ut(t1561,t529,n)
        t2760 = (t2758 - t1802) * t118
        t2764 = ut(t1561,t536,n)
        t2766 = (t1805 - t2764) * t118
        t2780 = ut(t2549,t115,n)
        t2783 = ut(t2549,t120,n)
        t2791 = (t1811 - t2440 * ((t2780 - t2728) * t118 / 0.2E1 + (t272
     #8 - t2783) * t118 / 0.2E1)) * t46
        t2801 = (t1802 - t2780) * t46
        t2815 = t243 * (t1833 - (t83 / 0.2E1 - t2730 / 0.2E1) * t46) * t
     #46
        t2819 = (t1805 - t2783) * t46
        t2834 = (t1780 - t2758) * t46
        t2840 = (t1857 * (t1856 / 0.2E1 + t2834 / 0.2E1) - t2156) * t118
        t2844 = (t2162 - t2169) * t118
        t2848 = (t1786 - t2764) * t46
        t2854 = (t2167 - t1914 * (t1870 / 0.2E1 + t2848 / 0.2E1)) * t118
        t2873 = (t2041 * t1782 - t2171) * t118
        t2878 = (t2172 - t2108 * t1788) * t118
        t2886 = (t1764 - t2569 * t1752) * t46 - t495 * ((t1767 - t1597 *
     # t2734) * t46 + (t1774 - (t1772 - t2750) * t46) * t46) / 0.24E2 + 
     #t427 + t2152 - t528 * (t1797 / 0.2E1 + (t1795 - t1514 * ((t2760 / 
     #0.2E1 - t1807 / 0.2E1) * t118 - (t1804 / 0.2E1 - t2766 / 0.2E1) * 
     #t118) * t118) * t46 / 0.2E1) / 0.6E1 - t495 * (t1817 / 0.2E1 + (t1
     #815 - (t1813 - t2791) * t46) * t46 / 0.2E1) / 0.6E1 + t2163 + t217
     #0 - t495 * ((t1255 * (t1826 - (t429 / 0.2E1 - t2801 / 0.2E1) * t46
     #) * t46 - t2815) * t118 / 0.2E1 + (t2815 - t1294 * (t1844 - (t442 
     #/ 0.2E1 - t2819 / 0.2E1) * t46) * t46) * t118 / 0.2E1) / 0.6E1 - t
     #528 * (((t2840 - t2162) * t118 - t2844) * t118 / 0.2E1 + (t2844 - 
     #(t2169 - t2854) * t118) * t118 / 0.2E1) / 0.6E1 + (t2694 * t417 - 
     #t2706 * t420) * t118 - t528 * ((t1952 * t2323 - t1960 * t2327) * t
     #118 + ((t2873 - t2174) * t118 - (t2174 - t2878) * t118) * t118) / 
     #0.24E2
        t2888 = t97 * t2886 * t55
        t2895 = t1845 * (t1649 / 0.2E1 + t2637 / 0.2E1)
        t2899 = t1514 * (t1587 / 0.2E1 + t2575 / 0.2E1)
        t2902 = (t2895 - t2899) * t118 / 0.2E1
        t2906 = t1888 * (t1667 / 0.2E1 + t2655 / 0.2E1)
        t2909 = (t2899 - t2906) * t118 / 0.2E1
        t2910 = t1985 ** 2
        t2911 = t1983 ** 2
        t2913 = t1989 * (t2910 + t2911)
        t2914 = t1565 ** 2
        t2915 = t1563 ** 2
        t2917 = t1569 * (t2914 + t2915)
        t2920 = t4 * (t2913 / 0.2E1 + t2917 / 0.2E1)
        t2921 = t2920 * t1630
        t2922 = t2052 ** 2
        t2923 = t2050 ** 2
        t2925 = t2056 * (t2922 + t2923)
        t2928 = t4 * (t2917 / 0.2E1 + t2925 / 0.2E1)
        t2929 = t2928 * t1633
        t2933 = (t2588 + t1923 + t2627 / 0.2E1 + t2902 + t2909 + (t2921 
     #- t2929) * t118) * t1568
        t2935 = (t1965 - t2933) * t46
        t2937 = t1967 / 0.2E1 + t2935 / 0.2E1
        t2938 = t1025 * t2937
        t2946 = t495 * (t1754 - dx * (t1756 - t2734) / 0.12E2) / 0.12E2
        t2950 = rx(t2549,t115,0,0)
        t2951 = rx(t2549,t115,1,1)
        t2953 = rx(t2549,t115,1,0)
        t2954 = rx(t2549,t115,0,1)
        t2957 = 0.1E1 / (t2950 * t2951 - t2953 * t2954)
        t2958 = t2950 ** 2
        t2959 = t2954 ** 2
        t2972 = u(t2549,t529,n)
        t2982 = rx(t1561,t529,0,0)
        t2983 = rx(t1561,t529,1,1)
        t2985 = rx(t1561,t529,1,0)
        t2986 = rx(t1561,t529,0,1)
        t2989 = 0.1E1 / (t2982 * t2983 - t2985 * t2986)
        t3003 = t2985 ** 2
        t3004 = t2983 ** 2
        t2830 = t4 * t2989 * (t2982 * t2985 + t2986 * t2983)
        t3014 = ((t1997 - t4 * (t1993 / 0.2E1 + t2957 * (t2958 + t2959) 
     #/ 0.2E1) * t2637) * t46 + t2013 + (t2010 - t4 * t2957 * (t2950 * t
     #2953 + t2954 * t2951) * ((t2972 - t2616) * t118 / 0.2E1 + t2618 / 
     #0.2E1)) * t46 / 0.2E1 + (t2830 * (t2027 / 0.2E1 + (t2004 - t2972) 
     #* t46 / 0.2E1) - t2895) * t118 / 0.2E1 + t2902 + (t4 * (t2989 * (t
     #3003 + t3004) / 0.2E1 + t2913 / 0.2E1) * t2006 - t2921) * t118) * 
     #t1988
        t3017 = rx(t2549,t120,0,0)
        t3018 = rx(t2549,t120,1,1)
        t3020 = rx(t2549,t120,1,0)
        t3021 = rx(t2549,t120,0,1)
        t3024 = 0.1E1 / (t3017 * t3018 - t3020 * t3021)
        t3025 = t3017 ** 2
        t3026 = t3021 ** 2
        t3039 = u(t2549,t536,n)
        t3049 = rx(t1561,t536,0,0)
        t3050 = rx(t1561,t536,1,1)
        t3052 = rx(t1561,t536,1,0)
        t3053 = rx(t1561,t536,0,1)
        t3056 = 0.1E1 / (t3049 * t3050 - t3052 * t3053)
        t3070 = t3052 ** 2
        t3071 = t3050 ** 2
        t2884 = t4 * t3056 * (t3049 * t3052 + t3053 * t3050)
        t3081 = ((t2064 - t4 * (t2060 / 0.2E1 + t3024 * (t3025 + t3026) 
     #/ 0.2E1) * t2655) * t46 + t2080 + (t2077 - t4 * t3024 * (t3017 * t
     #3020 + t3021 * t3018) * (t2621 / 0.2E1 + (t2619 - t3039) * t118 / 
     #0.2E1)) * t46 / 0.2E1 + t2909 + (t2906 - t2884 * (t2094 / 0.2E1 + 
     #(t2071 - t3039) * t46 / 0.2E1)) * t118 / 0.2E1 + (t2929 - t4 * (t2
     #925 / 0.2E1 + t3056 * (t3070 + t3071) / 0.2E1) * t2073) * t118) * 
     #t2055
        t3098 = t243 * t2937
        t3117 = t339 * ((t1979 - t1597 * t2935) * t46 + t2122 + (t2119 -
     # t1514 * ((t3014 - t2933) * t118 / 0.2E1 + (t2933 - t3081) * t118 
     #/ 0.2E1)) * t46 / 0.2E1 + (t1255 * (t2124 / 0.2E1 + (t2046 - t3014
     #) * t46 / 0.2E1) - t3098) * t118 / 0.2E1 + (t3098 - t1294 * (t2135
     # / 0.2E1 + (t2113 - t3081) * t46 / 0.2E1)) * t118 / 0.2E1 + (t1952
     # * t2048 - t1960 * t2115) * t118) * t55
        t3128 = t1514 * (t1752 / 0.2E1 + t2730 / 0.2E1)
        t3149 = t1522 * (t2178 / 0.2E1 + (t2176 - (t2750 + t2152 + t2791
     # / 0.2E1 + (t1845 * (t1823 / 0.2E1 + t2801 / 0.2E1) - t3128) * t11
     #8 / 0.2E1 + (t3128 - t1888 * (t1841 / 0.2E1 + t2819 / 0.2E1)) * t1
     #18 / 0.2E1 + (t2920 * t1804 - t2928 * t1807) * t118) * t1568) * t4
     #6 / 0.2E1)
        t3153 = t1025 * (t1967 - t2935)
        t3156 = t2 + t1748 - t1763 + t1922 - t1972 + t1978 + t2151 - t21
     #83 + t2187 - t81 - t466 * t2725 - t2741 - t809 * t2888 / 0.2E1 - t
     #466 * t2938 / 0.2E1 - t2946 - t1108 * t3117 / 0.6E1 - t809 * t3149
     # / 0.4E1 - t466 * t3153 / 0.12E2
        t3168 = sqrt(t2191 + t2192 + 0.8E1 * t57 + 0.8E1 * t58 - 0.2E1 *
     # dx * ((t14 + t15 - t27 - t28) * t46 / 0.2E1 - (t57 + t58 - t1570 
     #- t1571) * t46 / 0.2E1))
        t3169 = 0.1E1 / t3168
        t3174 = t1581 * t2208 * t2531
        t3177 = t238 * t2211 * t2536 / 0.2E1
        t3180 = t238 * t2215 * t2541 / 0.6E1
        t3182 = t2208 * t2544 / 0.24E2
        t3194 = t2 + t2235 - t1763 + t2237 - t2239 + t1978 + t2241 - t22
     #43 + t2245 - t81 - t2221 * t2725 - t2741 - t2223 * t2888 / 0.2E1 -
     # t2221 * t2938 / 0.2E1 - t2946 - t2228 * t3117 / 0.6E1 - t2223 * t
     #3149 / 0.4E1 - t2221 * t3153 / 0.12E2
        t3197 = 0.2E1 * t2547 * t3194 * t3169
        t3199 = (t1581 * t72 * t2531 + t238 * t95 * t2536 / 0.2E1 + t238
     # * t337 * t2541 / 0.6E1 - t72 * t2544 / 0.24E2 + 0.2E1 * t2547 * t
     #3156 * t3169 - t3174 - t3177 - t3180 + t3182 - t3197) * t69
        t3205 = t1581 * (t241 - dx * t1590 / 0.24E2)
        t3207 = dx * t1601 / 0.24E2
        t3223 = t4 * (t2267 + t2271 / 0.2E1 - dx * ((t2264 - t2266) * t4
     #6 / 0.2E1 - (t2271 - t1569 * t1627) * t46 / 0.2E1) / 0.8E1)
        t3234 = (t1804 - t1807) * t118
        t3251 = t2286 + t2287 - t2291 + t417 / 0.4E1 + t420 / 0.4E1 - t2
     #330 / 0.12E2 - dx * ((t2308 + t2309 - t2310 - t2313 - t2314 + t231
     #5) * t46 / 0.2E1 - (t2316 + t2317 - t2331 - t1804 / 0.2E1 - t1807 
     #/ 0.2E1 + t528 * (((t2760 - t1804) * t118 - t3234) * t118 / 0.2E1 
     #+ (t3234 - (t1807 - t2766) * t118) * t118 / 0.2E1) / 0.6E1) * t46 
     #/ 0.2E1) / 0.8E1
        t3256 = t4 * (t2266 / 0.2E1 + t2271 / 0.2E1)
        t3258 = t1414 / 0.4E1 + t1481 / 0.4E1 + t2048 / 0.4E1 + t2115 / 
     #0.4E1
        t3264 = (t2405 - t1996 * t1823) * t46
        t3270 = (t2411 - t1845 * (t2760 / 0.2E1 + t1804 / 0.2E1)) * t46
        t3274 = (t3264 + t2414 + t3270 / 0.2E1 + t2840 / 0.2E1 + t2163 +
     # t2873) * t1354
        t3279 = (t2420 - t2063 * t1841) * t46
        t3285 = (t2426 - t1888 * (t1807 / 0.2E1 + t2766 / 0.2E1)) * t46
        t3289 = (t3279 + t2429 + t3285 / 0.2E1 + t2170 + t2854 / 0.2E1 +
     # t2878) * t1421
        t3293 = t2419 / 0.4E1 + t2434 / 0.4E1 + (t3274 - t2176) * t118 /
     # 0.4E1 + (t2176 - t3289) * t118 / 0.4E1
        t3299 = dx * (t378 / 0.2E1 - t1813 / 0.2E1)
        t3303 = t3223 * t2208 * t3251
        t3306 = t3256 * t2447 * t3258 / 0.2E1
        t3309 = t3256 * t2451 * t3293 / 0.6E1
        t3311 = t2208 * t3299 / 0.24E2
        t3313 = (t3223 * t72 * t3251 + t3256 * t2344 * t3258 / 0.2E1 + t
     #3256 * t2350 * t3293 / 0.6E1 - t72 * t3299 / 0.24E2 - t3303 - t330
     #6 - t3309 + t3311) * t69
        t3326 = (t1630 - t1633) * t118
        t3344 = t3223 * (t2467 + t2468 - t2472 + t251 / 0.4E1 + t254 / 0
     #.4E1 - t2511 / 0.12E2 - dx * ((t2489 + t2490 - t2491 - t2494 - t24
     #95 + t2496) * t46 / 0.2E1 - (t2497 + t2498 - t2512 - t1630 / 0.2E1
     # - t1633 / 0.2E1 + t528 * (((t2006 - t1630) * t118 - t3326) * t118
     # / 0.2E1 + (t3326 - (t1633 - t2073) * t118) * t118 / 0.2E1) / 0.6E
     #1) * t46 / 0.2E1) / 0.8E1)
        t3348 = dx * (t160 / 0.2E1 - t1639 / 0.2E1) / 0.24E2
        t3356 = t269 * t273
        t3361 = t294 * t298
        t3369 = t4 * (t3356 / 0.2E1 + t2267 - dy * ((t1387 * t1391 - t33
     #56) * t118 / 0.2E1 - (t2266 - t3361) * t118 / 0.2E1) / 0.8E1)
        t3375 = (t383 - t429) * t46
        t3377 = ((t381 - t383) * t46 - t3375) * t46
        t3381 = (t3375 - (t429 - t1823) * t46) * t46
        t3384 = t495 * (t3377 / 0.2E1 + t3381 / 0.2E1)
        t3386 = t75 / 0.4E1
        t3387 = t83 / 0.4E1
        t3388 = t1759 / 0.12E2
        t3394 = (t956 - t1856) * t46
        t3405 = t383 / 0.2E1
        t3406 = t429 / 0.2E1
        t3407 = t3384 / 0.6E1
        t3410 = t398 / 0.2E1
        t3411 = t442 / 0.2E1
        t3415 = (t398 - t442) * t46
        t3417 = ((t396 - t398) * t46 - t3415) * t46
        t3421 = (t3415 - (t442 - t1841) * t46) * t46
        t3424 = t495 * (t3417 / 0.2E1 + t3421 / 0.2E1)
        t3425 = t3424 / 0.6E1
        t3432 = t383 / 0.4E1 + t429 / 0.4E1 - t3384 / 0.12E2 + t3386 + t
     #3387 - t3388 - dy * ((t956 / 0.2E1 + t1856 / 0.2E1 - t495 * (((t95
     #4 - t956) * t46 - t3394) * t46 / 0.2E1 + (t3394 - (t1856 - t2834) 
     #* t46) * t46 / 0.2E1) / 0.6E1 - t3405 - t3406 + t3407) * t118 / 0.
     #2E1 - (t792 + t1749 - t1760 - t3410 - t3411 + t3425) * t118 / 0.2E
     #1) / 0.8E1
        t3437 = t4 * (t3356 / 0.2E1 + t2266 / 0.2E1)
        t3439 = t1492 / 0.4E1 + t2124 / 0.4E1 + t1095 / 0.4E1 + t1967 / 
     #0.4E1
        t3448 = (t2375 - t2417) * t46 / 0.4E1 + (t2417 - t3274) * t46 / 
     #0.4E1 + t1550 / 0.4E1 + t2178 / 0.4E1
        t3454 = dy * (t1862 / 0.2E1 - t448 / 0.2E1)
        t3458 = t3369 * t2208 * t3432
        t3461 = t3437 * t2447 * t3439 / 0.2E1
        t3464 = t3437 * t2451 * t3448 / 0.6E1
        t3466 = t2208 * t3454 / 0.24E2
        t3468 = (t3369 * t72 * t3432 + t3437 * t2344 * t3439 / 0.2E1 + t
     #3437 * t2350 * t3448 / 0.6E1 - t72 * t3454 / 0.24E2 - t3458 - t346
     #1 - t3464 + t3466) * t69
        t3476 = (t177 - t275) * t46
        t3478 = ((t175 - t177) * t46 - t3476) * t46
        t3482 = (t3476 - (t275 - t1649) * t46) * t46
        t3485 = t495 * (t3478 / 0.2E1 + t3482 / 0.2E1)
        t3487 = t107 / 0.4E1
        t3488 = t241 / 0.4E1
        t3491 = t495 * (t509 / 0.2E1 + t1591 / 0.2E1)
        t3492 = t3491 / 0.12E2
        t3498 = (t674 - t1393) * t46
        t3509 = t177 / 0.2E1
        t3510 = t275 / 0.2E1
        t3511 = t3485 / 0.6E1
        t3514 = t107 / 0.2E1
        t3515 = t241 / 0.2E1
        t3516 = t3491 / 0.6E1
        t3517 = t204 / 0.2E1
        t3518 = t300 / 0.2E1
        t3522 = (t204 - t300) * t46
        t3524 = ((t202 - t204) * t46 - t3522) * t46
        t3528 = (t3522 - (t300 - t1667) * t46) * t46
        t3531 = t495 * (t3524 / 0.2E1 + t3528 / 0.2E1)
        t3532 = t3531 / 0.6E1
        t3540 = t3369 * (t177 / 0.4E1 + t275 / 0.4E1 - t3485 / 0.12E2 + 
     #t3487 + t3488 - t3492 - dy * ((t674 / 0.2E1 + t1393 / 0.2E1 - t495
     # * (((t672 - t674) * t46 - t3498) * t46 / 0.2E1 + (t3498 - (t1393 
     #- t2027) * t46) * t46 / 0.2E1) / 0.6E1 - t3509 - t3510 + t3511) * 
     #t118 / 0.2E1 - (t3514 + t3515 - t3516 - t3517 - t3518 + t3532) * t
     #118 / 0.2E1) / 0.8E1)
        t3544 = dy * (t1399 / 0.2E1 - t306 / 0.2E1) / 0.24E2
        t3551 = t369 - dy * t1893 / 0.24E2
        t3556 = t97 * t1413 * t118
        t3561 = t339 * t2418 * t118
        t3564 = dy * t1906
        t3567 = cc * t1705
        t3569 = t1300 / 0.2E1
        t3579 = t4 * (t1136 / 0.2E1 + t3569 - dx * ((t1128 - t1136) * t4
     #6 / 0.2E1 - (t1300 - t1359) * t46 / 0.2E1) / 0.8E1)
        t3591 = t4 * (t3569 + t1359 / 0.2E1 - dx * ((t1136 - t1300) * t4
     #6 / 0.2E1 - (t1359 - t1993) * t46 / 0.2E1) / 0.8E1)
        t3608 = j + 3
        t3609 = u(t5,t3608,n)
        t3611 = (t3609 - t547) * t118
        t3619 = u(i,t3608,n)
        t3621 = (t3619 - t565) * t118
        t3628 = t264 * ((t3621 / 0.2E1 - t151 / 0.2E1) * t118 - t570) * 
     #t118
        t3631 = u(t48,t3608,n)
        t3633 = (t3631 - t1370) * t118
        t3650 = (t1312 - t1378) * t46
        t3677 = rx(i,t3608,0,0)
        t3678 = rx(i,t3608,1,1)
        t3680 = rx(i,t3608,1,0)
        t3681 = rx(i,t3608,0,1)
        t3684 = 0.1E1 / (t3677 * t3678 - t3680 * t3681)
        t3690 = (t3609 - t3619) * t46
        t3692 = (t3619 - t3631) * t46
        t3426 = t4 * t3684 * (t3677 * t3680 + t3681 * t3678)
        t3698 = (t3426 * (t3690 / 0.2E1 + t3692 / 0.2E1) - t1397) * t118
        t3708 = t3680 ** 2
        t3709 = t3678 ** 2
        t3711 = t3684 * (t3708 + t3709)
        t3719 = t4 * (t1404 / 0.2E1 + t1695 - dy * ((t3711 - t1404) * t1
     #18 / 0.2E1 - t1710 / 0.2E1) / 0.8E1)
        t3732 = t4 * (t3711 / 0.2E1 + t1404 / 0.2E1)
        t3735 = (t3732 * t3621 - t1408) * t118
        t3743 = (t3579 * t177 - t3591 * t275) * t46 - t495 * ((t1303 * t
     #3478 - t1362 * t3482) * t46 + ((t1306 - t1365) * t46 - (t1365 - t1
     #999) * t46) * t46) / 0.24E2 + t1313 + t1379 - t528 * ((t171 * ((t3
     #611 / 0.2E1 - t134 / 0.2E1) * t118 - t552) * t118 - t3628) * t46 /
     # 0.2E1 + (t3628 - t1255 * ((t3633 / 0.2E1 - t251 / 0.2E1) * t118 -
     # t1610) * t118) * t46 / 0.2E1) / 0.6E1 - t495 * (((t1166 - t1312) 
     #* t46 - t3650) * t46 / 0.2E1 + (t3650 - (t1378 - t2012) * t46) * t
     #46 / 0.2E1) / 0.6E1 + t1400 + t286 - t495 * ((t1273 * ((t672 / 0.2
     #E1 - t1393 / 0.2E1) * t46 - (t674 / 0.2E1 - t2027 / 0.2E1) * t46) 
     #* t46 - t1656) * t118 / 0.2E1 + t1665 / 0.2E1) / 0.6E1 - t528 * ((
     #(t3698 - t1399) * t118 - t1682) * t118 / 0.2E1 + t1686 / 0.2E1) / 
     #0.6E1 + (t3719 * t567 - t1707) * t118 - t528 * ((t1407 * ((t3621 -
     # t567) * t118 - t1723) * t118 - t1728) * t118 + ((t3735 - t1410) *
     # t118 - t1737) * t118) / 0.24E2
        t3745 = dt * t3743 * t268
        t3748 = ut(i,t3608,n)
        t3750 = (t3748 - t863) * t118
        t3754 = ((t3750 - t865) * t118 - t1890) * t118
        t3761 = dy * (t865 / 0.2E1 + t2313 - t528 * (t3754 / 0.2E1 + t18
     #94 / 0.2E1) / 0.6E1) / 0.2E1
        t3779 = ut(t5,t3608,n)
        t3781 = (t3779 - t845) * t118
        t3795 = t264 * ((t3750 / 0.2E1 - t369 / 0.2E1) * t118 - t868) * 
     #t118
        t3798 = ut(t48,t3608,n)
        t3800 = (t3798 - t1780) * t118
        t3817 = (t2371 - t2413) * t46
        t3853 = (t3426 * ((t3779 - t3748) * t46 / 0.2E1 + (t3748 - t3798
     #) * t46 / 0.2E1) - t1860) * t118
        t3870 = (t3732 * t3750 - t1903) * t118
        t3878 = (t3579 * t383 - t3591 * t429) * t46 - t495 * ((t1303 * t
     #3377 - t1362 * t3381) * t46 + ((t2354 - t2407) * t46 - (t2407 - t3
     #264) * t46) * t46) / 0.24E2 + t2372 + t2414 - t528 * ((t171 * ((t3
     #781 / 0.2E1 - t356 / 0.2E1) * t118 - t850) * t118 - t3795) * t46 /
     # 0.2E1 + (t3795 - t1255 * ((t3800 / 0.2E1 - t417 / 0.2E1) * t118 -
     # t1785) * t118) * t46 / 0.2E1) / 0.6E1 - t495 * (((t2364 - t2371) 
     #* t46 - t3817) * t46 / 0.2E1 + (t3817 - (t2413 - t3270) * t46) * t
     #46 / 0.2E1) / 0.6E1 + t2415 + t440 - t495 * ((t1273 * ((t954 / 0.2
     #E1 - t1856 / 0.2E1) * t46 - (t956 / 0.2E1 - t2834 / 0.2E1) * t46) 
     #* t46 - t1830) * t118 / 0.2E1 + t1839 / 0.2E1) / 0.6E1 - t528 * ((
     #(t3853 - t1862) * t118 - t1864) * t118 / 0.2E1 + t1868 / 0.2E1) / 
     #0.6E1 + (t3719 * t865 - t1885) * t118 - t528 * ((t1407 * t3754 - t
     #1895) * t118 + ((t3870 - t1905) * t118 - t1907) * t118) / 0.24E2
        t3880 = t97 * t3878 * t268
        t3883 = dt * dy
        t3884 = t659 ** 2
        t3885 = t663 ** 2
        t3887 = t666 * (t3884 + t3885)
        t3888 = t1380 ** 2
        t3889 = t1384 ** 2
        t3891 = t1387 * (t3888 + t3889)
        t3894 = t4 * (t3887 / 0.2E1 + t3891 / 0.2E1)
        t3895 = t3894 * t674
        t3896 = t2014 ** 2
        t3897 = t2018 ** 2
        t3899 = t2021 * (t3896 + t3897)
        t3902 = t4 * (t3891 / 0.2E1 + t3899 / 0.2E1)
        t3903 = t3902 * t1393
        t3909 = t545 * (t3611 / 0.2E1 + t549 / 0.2E1)
        t3913 = t1273 * (t3621 / 0.2E1 + t567 / 0.2E1)
        t3916 = (t3909 - t3913) * t46 / 0.2E1
        t3920 = t1857 * (t3633 / 0.2E1 + t1372 / 0.2E1)
        t3923 = (t3913 - t3920) * t46 / 0.2E1
        t3926 = ((t3895 - t3903) * t46 + t3916 + t3923 + t3698 / 0.2E1 +
     # t1400 + t3735) * t1386
        t3928 = (t3926 - t1412) * t118
        t3930 = t3928 / 0.2E1 + t1414 / 0.2E1
        t3931 = t3883 * t3930
        t3939 = t528 * (t1890 - dy * (t3754 - t1894) / 0.12E2) / 0.12E2
        t3944 = t1168 ** 2
        t3945 = t1172 ** 2
        t3954 = u(t32,t3608,n)
        t3964 = rx(t5,t3608,0,0)
        t3965 = rx(t5,t3608,1,1)
        t3967 = rx(t5,t3608,1,0)
        t3968 = rx(t5,t3608,0,1)
        t3971 = 0.1E1 / (t3964 * t3965 - t3967 * t3968)
        t3985 = t3967 ** 2
        t3986 = t3965 ** 2
        t3996 = ((t4 * (t1175 * (t3944 + t3945) / 0.2E1 + t3887 / 0.2E1)
     # * t672 - t3895) * t46 + (t1100 * ((t3954 - t530) * t118 / 0.2E1 +
     # t532 / 0.2E1) - t3909) * t46 / 0.2E1 + t3916 + (t4 * t3971 * (t39
     #64 * t3967 + t3968 * t3965) * ((t3954 - t3609) * t46 / 0.2E1 + t36
     #90 / 0.2E1) - t678) * t118 / 0.2E1 + t1314 + (t4 * (t3971 * (t3985
     # + t3986) / 0.2E1 + t722 / 0.2E1) * t3611 - t769) * t118) * t665
        t4004 = t264 * t3930
        t4008 = t2982 ** 2
        t4009 = t2986 ** 2
        t4018 = u(t1561,t3608,n)
        t4028 = rx(t48,t3608,0,0)
        t4029 = rx(t48,t3608,1,1)
        t4031 = rx(t48,t3608,1,0)
        t4032 = rx(t48,t3608,0,1)
        t4035 = 0.1E1 / (t4028 * t4029 - t4031 * t4032)
        t4049 = t4031 ** 2
        t4050 = t4029 ** 2
        t4060 = ((t3903 - t4 * (t3899 / 0.2E1 + t2989 * (t4008 + t4009) 
     #/ 0.2E1) * t2027) * t46 + t3923 + (t3920 - t2830 * ((t4018 - t2004
     #) * t118 / 0.2E1 + t2006 / 0.2E1)) * t46 / 0.2E1 + (t4 * t4035 * (
     #t4028 * t4031 + t4032 * t4029) * (t3692 / 0.2E1 + (t3631 - t4018) 
     #* t46 / 0.2E1) - t2031) * t118 / 0.2E1 + t2034 + (t4 * (t4035 * (t
     #4049 + t4050) / 0.2E1 + t2038 / 0.2E1) * t3633 - t2042) * t118) * 
     #t2020
        t4086 = t339 * ((t1303 * t1492 - t1362 * t2124) * t46 + (t171 * 
     #((t3996 - t1316) * t118 / 0.2E1 + t1318 / 0.2E1) - t4004) * t46 / 
     #0.2E1 + (t4004 - t1255 * ((t4060 - t2046) * t118 / 0.2E1 + t2048 /
     # 0.2E1)) * t46 / 0.2E1 + (t1273 * ((t3996 - t3926) * t46 / 0.2E1 +
     # (t3926 - t4060) * t46 / 0.2E1) - t2128) * t118 / 0.2E1 + t2133 + 
     #(t1407 * t3928 - t2143) * t118) * t268
        t4089 = t97 * dy
        t4101 = t1273 * (t3750 / 0.2E1 + t865 / 0.2E1)
        t4119 = t4089 * ((((t3894 * t956 - t3902 * t1856) * t46 + (t545 
     #* (t3781 / 0.2E1 + t847 / 0.2E1) - t4101) * t46 / 0.2E1 + (t4101 -
     # t1857 * (t3800 / 0.2E1 + t1782 / 0.2E1)) * t46 / 0.2E1 + t3853 / 
     #0.2E1 + t2415 + t3870) * t1386 - t2417) * t118 / 0.2E1 + t2419 / 0
     #.2E1)
        t4123 = t3883 * (t3928 - t1414)
        t4128 = dy * (t2313 + t2314 - t2315) / 0.2E1
        t4129 = t3883 * t1483
        t4131 = t466 * t4129 / 0.2E1
        t4137 = t528 * (t1892 - dy * (t1894 - t1899) / 0.12E2) / 0.12E2
        t4140 = t4089 * (t2419 / 0.2E1 + t2434 / 0.2E1)
        t4142 = t809 * t4140 / 0.4E1
        t4144 = t3883 * (t1414 - t1481)
        t4146 = t466 * t4144 / 0.12E2
        t4147 = t367 + t466 * t3745 - t3761 + t809 * t3880 / 0.2E1 - t46
     #6 * t3931 / 0.2E1 + t3939 + t1108 * t4086 / 0.6E1 - t809 * t4119 /
     # 0.4E1 + t466 * t4123 / 0.12E2 - t2 - t1748 - t4128 - t1922 - t413
     #1 - t4137 - t2151 - t4142 - t4146
        t4150 = 0.8E1 * t312
        t4151 = 0.8E1 * t313
        t4161 = sqrt(0.8E1 * t308 + 0.8E1 * t309 + t4150 + t4151 - 0.2E1
     # * dy * ((t1401 + t1402 - t308 - t309) * t118 / 0.2E1 - (t312 + t3
     #13 - t320 - t321) * t118 / 0.2E1))
        t4162 = 0.1E1 / t4161
        t4167 = t1706 * t2208 * t3551
        t4170 = t318 * t2211 * t3556 / 0.2E1
        t4173 = t318 * t2215 * t3561 / 0.6E1
        t4175 = t2208 * t3564 / 0.24E2
        t4188 = t2221 * t4129 / 0.2E1
        t4190 = t2223 * t4140 / 0.4E1
        t4192 = t2221 * t4144 / 0.12E2
        t4193 = t367 + t2221 * t3745 - t3761 + t2223 * t3880 / 0.2E1 - t
     #2221 * t3931 / 0.2E1 + t3939 + t2228 * t4086 / 0.6E1 - t2223 * t41
     #19 / 0.4E1 + t2221 * t4123 / 0.12E2 - t2 - t2235 - t4128 - t2237 -
     # t4188 - t4137 - t2241 - t4190 - t4192
        t4196 = 0.2E1 * t3567 * t4193 * t4162
        t4198 = (t1706 * t72 * t3551 + t318 * t95 * t3556 / 0.2E1 + t318
     # * t337 * t3561 / 0.6E1 - t72 * t3564 / 0.24E2 + 0.2E1 * t3567 * t
     #4147 * t4162 - t4167 - t4170 - t4173 + t4175 - t4196) * t69
        t4204 = t1706 * (t151 - dy * t1726 / 0.24E2)
        t4206 = dy * t1736 / 0.24E2
        t4222 = t4 * (t2267 + t3361 / 0.2E1 - dy * ((t3356 - t2266) * t1
     #18 / 0.2E1 - (t3361 - t1458 * t1454) * t118 / 0.2E1) / 0.8E1)
        t4233 = (t972 - t1870) * t46
        t4250 = t3386 + t3387 - t3388 + t398 / 0.4E1 + t442 / 0.4E1 - t3
     #424 / 0.12E2 - dy * ((t3405 + t3406 - t3407 - t792 - t1749 + t1760
     #) * t118 / 0.2E1 - (t3410 + t3411 - t3425 - t972 / 0.2E1 - t1870 /
     # 0.2E1 + t495 * (((t970 - t972) * t46 - t4233) * t46 / 0.2E1 + (t4
     #233 - (t1870 - t2848) * t46) * t46 / 0.2E1) / 0.6E1) * t118 / 0.2E
     #1) / 0.8E1
        t4255 = t4 * (t2266 / 0.2E1 + t3361 / 0.2E1)
        t4257 = t1095 / 0.4E1 + t1967 / 0.4E1 + t1505 / 0.4E1 + t2135 / 
     #0.4E1
        t4266 = t1550 / 0.4E1 + t2178 / 0.4E1 + (t2402 - t2432) * t46 / 
     #0.4E1 + (t2432 - t3289) * t46 / 0.4E1
        t4272 = dy * (t439 / 0.2E1 - t1876 / 0.2E1)
        t4276 = t4222 * t2208 * t4250
        t4279 = t4255 * t2447 * t4257 / 0.2E1
        t4282 = t4255 * t2451 * t4266 / 0.6E1
        t4284 = t2208 * t4272 / 0.24E2
        t4286 = (t4222 * t72 * t4250 + t4255 * t2344 * t4257 / 0.2E1 + t
     #4255 * t2350 * t4266 / 0.6E1 - t72 * t4272 / 0.24E2 - t4276 - t427
     #9 - t4282 + t4284) * t69
        t4299 = (t702 - t1460) * t46
        t4317 = t4222 * (t3487 + t3488 - t3492 + t204 / 0.4E1 + t300 / 0
     #.4E1 - t3531 / 0.12E2 - dy * ((t3509 + t3510 - t3511 - t3514 - t35
     #15 + t3516) * t118 / 0.2E1 - (t3517 + t3518 - t3532 - t702 / 0.2E1
     # - t1460 / 0.2E1 + t495 * (((t700 - t702) * t46 - t4299) * t46 / 0
     #.2E1 + (t4299 - (t1460 - t2094) * t46) * t46 / 0.2E1) / 0.6E1) * t
     #118 / 0.2E1) / 0.8E1)
        t4321 = dy * (t285 / 0.2E1 - t1466 / 0.2E1) / 0.24E2
        t4328 = t372 - dy * t1898 / 0.24E2
        t4333 = t97 * t1480 * t118
        t4338 = t339 * t2433 * t118
        t4341 = dy * t1911
        t4344 = cc * t1717
        t4346 = t1322 / 0.2E1
        t4356 = t4 * (t1226 / 0.2E1 + t4346 - dx * ((t1218 - t1226) * t4
     #6 / 0.2E1 - (t1322 - t1426) * t46 / 0.2E1) / 0.8E1)
        t4368 = t4 * (t4346 + t1426 / 0.2E1 - dx * ((t1226 - t1322) * t4
     #6 / 0.2E1 - (t1426 - t2060) * t46 / 0.2E1) / 0.8E1)
        t4385 = j - 3
        t4386 = u(t5,t4385,n)
        t4388 = (t553 - t4386) * t118
        t4396 = u(i,t4385,n)
        t4398 = (t571 - t4396) * t118
        t4405 = t284 * (t576 - (t154 / 0.2E1 - t4398 / 0.2E1) * t118) * 
     #t118
        t4408 = u(t48,t4385,n)
        t4410 = (t1437 - t4408) * t118
        t4427 = (t1334 - t1445) * t46
        t4454 = rx(i,t4385,0,0)
        t4455 = rx(i,t4385,1,1)
        t4457 = rx(i,t4385,1,0)
        t4458 = rx(i,t4385,0,1)
        t4461 = 0.1E1 / (t4454 * t4455 - t4457 * t4458)
        t4467 = (t4386 - t4396) * t46
        t4469 = (t4396 - t4408) * t46
        t4181 = t4 * t4461 * (t4454 * t4457 + t4458 * t4455)
        t4475 = (t1464 - t4181 * (t4467 / 0.2E1 + t4469 / 0.2E1)) * t118
        t4485 = t4457 ** 2
        t4486 = t4455 ** 2
        t4488 = t4461 * (t4485 + t4486)
        t4496 = t4 * (t1708 + t1471 / 0.2E1 - dy * (t1700 / 0.2E1 - (t14
     #71 - t4488) * t118 / 0.2E1) / 0.8E1)
        t4509 = t4 * (t1471 / 0.2E1 + t4488 / 0.2E1)
        t4512 = (t1475 - t4509 * t4398) * t118
        t4520 = (t4356 * t204 - t4368 * t300) * t46 - t495 * ((t1325 * t
     #3524 - t1429 * t3528) * t46 + ((t1328 - t1432) * t46 - (t1432 - t2
     #066) * t46) * t46) / 0.24E2 + t1335 + t1446 - t528 * ((t194 * (t55
     #8 - (t137 / 0.2E1 - t4388 / 0.2E1) * t118) * t118 - t4405) * t46 /
     # 0.2E1 + (t4405 - t1294 * (t1613 - (t254 / 0.2E1 - t4410 / 0.2E1) 
     #* t118) * t118) * t46 / 0.2E1) / 0.6E1 - t495 * (((t1256 - t1334) 
     #* t46 - t4427) * t46 / 0.2E1 + (t4427 - (t1445 - t2079) * t46) * t
     #46 / 0.2E1) / 0.6E1 + t307 + t1467 - t495 * (t1676 / 0.2E1 + (t167
     #4 - t1309 * ((t700 / 0.2E1 - t1460 / 0.2E1) * t46 - (t702 / 0.2E1 
     #- t2094 / 0.2E1) * t46) * t46) * t118 / 0.2E1) / 0.6E1 - t528 * (t
     #1690 / 0.2E1 + (t1688 - (t1466 - t4475) * t118) * t118 / 0.2E1) / 
     #0.6E1 + (t1719 - t4496 * t573) * t118 - t528 * ((t1733 - t1474 * (
     #t1730 - (t573 - t4398) * t118) * t118) * t118 + (t1739 - (t1477 - 
     #t4512) * t118) * t118) / 0.24E2
        t4522 = dt * t4520 * t293
        t4525 = ut(i,t4385,n)
        t4527 = (t869 - t4525) * t118
        t4531 = (t1897 - (t871 - t4527) * t118) * t118
        t4538 = dy * (t2314 + t871 / 0.2E1 - t528 * (t1899 / 0.2E1 + t45
     #31 / 0.2E1) / 0.6E1) / 0.2E1
        t4556 = ut(t5,t4385,n)
        t4558 = (t851 - t4556) * t118
        t4572 = t284 * (t874 - (t372 / 0.2E1 - t4527 / 0.2E1) * t118) * 
     #t118
        t4575 = ut(t48,t4385,n)
        t4577 = (t1786 - t4575) * t118
        t4594 = (t2398 - t2428) * t46
        t4630 = (t1874 - t4181 * ((t4556 - t4525) * t46 / 0.2E1 + (t4525
     # - t4575) * t46 / 0.2E1)) * t118
        t4647 = (t1908 - t4509 * t4527) * t118
        t4655 = (t4356 * t398 - t4368 * t442) * t46 - t495 * ((t1325 * t
     #3417 - t1429 * t3421) * t46 + ((t2381 - t2422) * t46 - (t2422 - t3
     #279) * t46) * t46) / 0.24E2 + t2399 + t2429 - t528 * ((t194 * (t85
     #6 - (t359 / 0.2E1 - t4558 / 0.2E1) * t118) * t118 - t4572) * t46 /
     # 0.2E1 + (t4572 - t1294 * (t1791 - (t420 / 0.2E1 - t4577 / 0.2E1) 
     #* t118) * t118) * t46 / 0.2E1) / 0.6E1 - t495 * (((t2391 - t2398) 
     #* t46 - t4594) * t46 / 0.2E1 + (t4594 - (t2428 - t3285) * t46) * t
     #46 / 0.2E1) / 0.6E1 + t449 + t2430 - t495 * (t1850 / 0.2E1 + (t184
     #8 - t1309 * ((t970 / 0.2E1 - t1870 / 0.2E1) * t46 - (t972 / 0.2E1 
     #- t2848 / 0.2E1) * t46) * t46) * t118 / 0.2E1) / 0.6E1 - t528 * (t
     #1880 / 0.2E1 + (t1878 - (t1876 - t4630) * t118) * t118 / 0.2E1) / 
     #0.6E1 + (t1886 - t4496 * t871) * t118 - t528 * ((t1900 - t1474 * t
     #4531) * t118 + (t1912 - (t1910 - t4647) * t118) * t118) / 0.24E2
        t4657 = t97 * t4655 * t293
        t4660 = t687 ** 2
        t4661 = t691 ** 2
        t4663 = t694 * (t4660 + t4661)
        t4664 = t1447 ** 2
        t4665 = t1451 ** 2
        t4667 = t1454 * (t4664 + t4665)
        t4670 = t4 * (t4663 / 0.2E1 + t4667 / 0.2E1)
        t4671 = t4670 * t702
        t4672 = t2081 ** 2
        t4673 = t2085 ** 2
        t4675 = t2088 * (t4672 + t4673)
        t4678 = t4 * (t4667 / 0.2E1 + t4675 / 0.2E1)
        t4679 = t4678 * t1460
        t4685 = t563 * (t555 / 0.2E1 + t4388 / 0.2E1)
        t4689 = t1309 * (t573 / 0.2E1 + t4398 / 0.2E1)
        t4692 = (t4685 - t4689) * t46 / 0.2E1
        t4696 = t1914 * (t1439 / 0.2E1 + t4410 / 0.2E1)
        t4699 = (t4689 - t4696) * t46 / 0.2E1
        t4702 = ((t4671 - t4679) * t46 + t4692 + t4699 + t1467 + t4475 /
     # 0.2E1 + t4512) * t1453
        t4704 = (t1479 - t4702) * t118
        t4706 = t1481 / 0.2E1 + t4704 / 0.2E1
        t4707 = t3883 * t4706
        t4715 = t528 * (t1897 - dy * (t1899 - t4531) / 0.12E2) / 0.12E2
        t4720 = t1258 ** 2
        t4721 = t1262 ** 2
        t4730 = u(t32,t4385,n)
        t4740 = rx(t5,t4385,0,0)
        t4741 = rx(t5,t4385,1,1)
        t4743 = rx(t5,t4385,1,0)
        t4744 = rx(t5,t4385,0,1)
        t4747 = 0.1E1 / (t4740 * t4741 - t4743 * t4744)
        t4761 = t4743 ** 2
        t4762 = t4741 ** 2
        t4772 = ((t4 * (t1265 * (t4720 + t4721) / 0.2E1 + t4663 / 0.2E1)
     # * t700 - t4671) * t46 + (t1187 * (t539 / 0.2E1 + (t537 - t4730) *
     # t118 / 0.2E1) - t4685) * t46 / 0.2E1 + t4692 + t1336 + (t706 - t4
     # * t4747 * (t4740 * t4743 + t4744 * t4741) * ((t4730 - t4386) * t4
     #6 / 0.2E1 + t4467 / 0.2E1)) * t118 / 0.2E1 + (t777 - t4 * (t740 / 
     #0.2E1 + t4747 * (t4761 + t4762) / 0.2E1) * t4388) * t118) * t693
        t4780 = t284 * t4706
        t4784 = t3049 ** 2
        t4785 = t3053 ** 2
        t4794 = u(t1561,t4385,n)
        t4804 = rx(t48,t4385,0,0)
        t4805 = rx(t48,t4385,1,1)
        t4807 = rx(t48,t4385,1,0)
        t4808 = rx(t48,t4385,0,1)
        t4811 = 0.1E1 / (t4804 * t4805 - t4807 * t4808)
        t4825 = t4807 ** 2
        t4826 = t4805 ** 2
        t4836 = ((t4679 - t4 * (t4675 / 0.2E1 + t3056 * (t4784 + t4785) 
     #/ 0.2E1) * t2094) * t46 + t4699 + (t4696 - t2884 * (t2073 / 0.2E1 
     #+ (t2071 - t4794) * t118 / 0.2E1)) * t46 / 0.2E1 + t2101 + (t2098 
     #- t4 * t4811 * (t4804 * t4807 + t4808 * t4805) * (t4469 / 0.2E1 + 
     #(t4408 - t4794) * t46 / 0.2E1)) * t118 / 0.2E1 + (t2109 - t4 * (t2
     #105 / 0.2E1 + t4811 * (t4825 + t4826) / 0.2E1) * t4410) * t118) * 
     #t2087
        t4862 = t339 * ((t1325 * t1505 - t1429 * t2135) * t46 + (t194 * 
     #(t1340 / 0.2E1 + (t1338 - t4772) * t118 / 0.2E1) - t4780) * t46 / 
     #0.2E1 + (t4780 - t1294 * (t2115 / 0.2E1 + (t2113 - t4836) * t118 /
     # 0.2E1)) * t46 / 0.2E1 + t2142 + (t2139 - t1309 * ((t4772 - t4702)
     # * t46 / 0.2E1 + (t4702 - t4836) * t46 / 0.2E1)) * t118 / 0.2E1 + 
     #(t2144 - t1474 * t4704) * t118) * t293
        t4876 = t1309 * (t871 / 0.2E1 + t4527 / 0.2E1)
        t4894 = t4089 * (t2434 / 0.2E1 + (t2432 - ((t4670 * t972 - t4678
     # * t1870) * t46 + (t563 * (t853 / 0.2E1 + t4558 / 0.2E1) - t4876) 
     #* t46 / 0.2E1 + (t4876 - t1914 * (t1788 / 0.2E1 + t4577 / 0.2E1)) 
     #* t46 / 0.2E1 + t2430 + t4630 / 0.2E1 + t4647) * t1453) * t118 / 0
     #.2E1)
        t4898 = t3883 * (t1481 - t4704)
        t4901 = t2 + t1748 - t4128 + t1922 - t4131 + t4137 + t2151 - t41
     #42 + t4146 - t370 - t466 * t4522 - t4538 - t809 * t4657 / 0.2E1 - 
     #t466 * t4707 / 0.2E1 - t4715 - t1108 * t4862 / 0.6E1 - t809 * t489
     #4 / 0.4E1 - t466 * t4898 / 0.12E2
        t4913 = sqrt(t4150 + t4151 + 0.8E1 * t320 + 0.8E1 * t321 - 0.2E1
     # * dy * ((t308 + t309 - t312 - t313) * t118 / 0.2E1 - (t320 + t321
     # - t1468 - t1469) * t118 / 0.2E1))
        t4914 = 0.1E1 / t4913
        t4919 = t1718 * t2208 * t4328
        t4922 = t326 * t2211 * t4333 / 0.2E1
        t4925 = t326 * t2215 * t4338 / 0.6E1
        t4927 = t2208 * t4341 / 0.24E2
        t4939 = t2 + t2235 - t4128 + t2237 - t4188 + t4137 + t2241 - t41
     #90 + t4192 - t370 - t2221 * t4522 - t4538 - t2223 * t4657 / 0.2E1 
     #- t2221 * t4707 / 0.2E1 - t4715 - t2228 * t4862 / 0.6E1 - t2223 * 
     #t4894 / 0.4E1 - t2221 * t4898 / 0.12E2
        t4942 = 0.2E1 * t4344 * t4939 * t4914
        t4944 = (t1718 * t72 * t4328 + t326 * t95 * t4333 / 0.2E1 + t326
     # * t337 * t4338 / 0.6E1 - t72 * t4341 / 0.24E2 + 0.2E1 * t4344 * t
     #4901 * t4914 - t4919 - t4922 - t4925 + t4927 - t4942) * t69
        t4950 = t1718 * (t154 - dy * t1731 / 0.24E2)
        t4952 = dy * t1738 / 0.24E2
        t4981 = t2251 * dt / 0.2E1 + (t2257 + t2210 + t2214 - t2259 + t2
     #218 - t2220 + t2249) * dt - t2251 * t2208 + t2458 * dt / 0.2E1 + (
     #t2520 + t2446 + t2450 - t2524 + t2454 - t2456) * dt - t2458 * t220
     #8 - t3199 * dt / 0.2E1 - (t3205 + t3174 + t3177 - t3207 + t3180 - 
     #t3182 + t3197) * dt + t3199 * t2208 - t3313 * dt / 0.2E1 - (t3344 
     #+ t3303 + t3306 - t3348 + t3309 - t3311) * dt + t3313 * t2208
        t5004 = t3468 * dt / 0.2E1 + (t3540 + t3458 + t3461 - t3544 + t3
     #464 - t3466) * dt - t3468 * t2208 + t4198 * dt / 0.2E1 + (t4204 + 
     #t4167 + t4170 - t4206 + t4173 - t4175 + t4196) * dt - t4198 * t220
     #8 - t4286 * dt / 0.2E1 - (t4317 + t4276 + t4279 - t4321 + t4282 - 
     #t4284) * dt + t4286 * t2208 - t4944 * dt / 0.2E1 - (t4950 + t4919 
     #+ t4922 - t4952 + t4925 - t4927 + t4942) * dt + t4944 * t2208


        unew(i,j) = t1 + dt * t2 + (t2251 * t97 / 0.6E1 + (t2257 + 
     #t2210 + t2214 - t2259 + t2218 - t2220 + t2249 - t2251 * t2207) * t
     #97 / 0.2E1 + t2458 * t97 / 0.6E1 + (t2520 + t2446 + t2450 - t2524 
     #+ t2454 - t2456 - t2458 * t2207) * t97 / 0.2E1 - t3199 * t97 / 0.6
     #E1 - (t3205 + t3174 + t3177 - t3207 + t3180 - t3182 + t3197 - t319
     #9 * t2207) * t97 / 0.2E1 - t3313 * t97 / 0.6E1 - (t3344 + t3303 + 
     #t3306 - t3348 + t3309 - t3311 - t3313 * t2207) * t97 / 0.2E1) * t2
     #5 * t46 + (t3468 * t97 / 0.6E1 + (t3540 + t3458 + t3461 - t3544 + 
     #t3464 - t3466 - t3468 * t2207) * t97 / 0.2E1 + t4198 * t97 / 0.6E1
     # + (t4204 + t4167 + t4170 - t4206 + t4173 - t4175 + t4196 - t4198 
     #* t2207) * t97 / 0.2E1 - t4286 * t97 / 0.6E1 - (t4317 + t4276 + t4
     #279 - t4321 + t4282 - t4284 - t4286 * t2207) * t97 / 0.2E1 - t4944
     # * t97 / 0.6E1 - (t4950 + t4919 + t4922 - t4952 + t4925 - t4927 + 
     #t4942 - t4944 * t2207) * t97 / 0.2E1) * t25 * t118

        utnew(i,j) = t2 + t4981
     # * t25 * t46 + t5004 * t25 * t118

c        blah = array(int(t1 + dt * t2 + (t2251 * t97 / 0.6E1 + (t2257 + 
c     #t2210 + t2214 - t2259 + t2218 - t2220 + t2249 - t2251 * t2207) * t
c     #97 / 0.2E1 + t2458 * t97 / 0.6E1 + (t2520 + t2446 + t2450 - t2524 
c     #+ t2454 - t2456 - t2458 * t2207) * t97 / 0.2E1 - t3199 * t97 / 0.6
c     #E1 - (t3205 + t3174 + t3177 - t3207 + t3180 - t3182 + t3197 - t319
c     #9 * t2207) * t97 / 0.2E1 - t3313 * t97 / 0.6E1 - (t3344 + t3303 + 
c     #t3306 - t3348 + t3309 - t3311 - t3313 * t2207) * t97 / 0.2E1) * t2
c     #5 * t46 + (t3468 * t97 / 0.6E1 + (t3540 + t3458 + t3461 - t3544 + 
c     #t3464 - t3466 - t3468 * t2207) * t97 / 0.2E1 + t4198 * t97 / 0.6E1
c     # + (t4204 + t4167 + t4170 - t4206 + t4173 - t4175 + t4196 - t4198 
c     #* t2207) * t97 / 0.2E1 - t4286 * t97 / 0.6E1 - (t4317 + t4276 + t4
c     #279 - t4321 + t4282 - t4284 - t4286 * t2207) * t97 / 0.2E1 - t4944
c     # * t97 / 0.6E1 - (t4950 + t4919 + t4922 - t4952 + t4925 - t4927 + 
c     #t4942 - t4944 * t2207) * t97 / 0.2E1) * t25 * t118),int(t2 + t4981
c     # * t25 * t46 + t5004 * t25 * t118))

        return
      end
