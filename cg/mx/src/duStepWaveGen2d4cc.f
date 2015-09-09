      subroutine duStepWaveGen2d4cc( 
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
        real t1002
        real t1004
        real t1007
        real t1008
        real t1010
        real t1013
        real t1017
        real t1018
        real t102
        real t1020
        real t1023
        real t1024
        real t1026
        real t1029
        real t1033
        real t1035
        real t1036
        real t1038
        real t104
        real t1041
        real t1042
        real t1044
        real t1047
        real t1051
        real t1053
        real t1059
        real t1061
        real t1063
        real t1064
        real t1066
        real t1067
        real t1069
        real t1073
        real t1075
        real t1077
        real t1079
        real t1086
        real t1088
        real t109
        real t1091
        real t1095
        real t1098
        real t11
        real t110
        real t1101
        real t1105
        real t1107
        real t111
        real t1110
        real t1112
        real t1115
        real t1119
        real t112
        real t1121
        real t1127
        real t1129
        real t113
        real t1133
        real t1135
        real t1137
        real t1139
        real t1141
        real t1143
        real t1145
        real t1149
        real t115
        real t1151
        real t1152
        real t1153
        real t1155
        real t1160
        real t1161
        real t1165
        real t1167
        real t1168
        real t1169
        real t1170
        real t1171
        real t1172
        real t1173
        real t1174
        real t1175
        real t1178
        real t1180
        real t1181
        real t1182
        real t1183
        real t1185
        real t1186
        real t1187
        real t119
        real t1193
        real t1194
        real t1196
        real t1199
        integer t12
        real t1202
        real t1204
        real t1205
        real t1206
        real t1207
        real t1209
        real t121
        real t1213
        real t1214
        real t1215
        real t1217
        real t1219
        real t122
        real t1220
        real t1223
        real t1225
        real t1227
        real t123
        real t1230
        real t1234
        real t1236
        real t1237
        real t1238
        real t1239
        real t124
        real t1240
        real t1241
        real t1243
        real t1244
        real t1246
        real t1247
        real t1255
        real t1259
        real t126
        real t1261
        real t1262
        real t1263
        real t1264
        real t1266
        real t1267
        real t1269
        real t127
        real t1270
        real t1278
        real t1280
        real t1281
        real t1282
        real t1283
        real t1285
        real t1286
        real t1287
        real t1289
        real t129
        real t1292
        real t1293
        real t1294
        real t1295
        real t1297
        real t13
        real t130
        real t1300
        real t1301
        real t1303
        real t1304
        real t1305
        real t1307
        real t1310
        real t1312
        real t1313
        real t1314
        real t1318
        real t1322
        real t1323
        real t1324
        real t1325
        real t1329
        real t1331
        real t1332
        real t1333
        real t1334
        real t1336
        real t1337
        real t1338
        real t1340
        real t1342
        real t1344
        real t1347
        real t1349
        real t1351
        real t1353
        real t1354
        real t1356
        real t1357
        real t1358
        real t1359
        real t136
        real t1360
        real t1361
        real t1362
        real t1365
        real t1366
        real t1368
        real t1369
        real t1370
        real t1372
        real t1375
        real t1376
        real t1377
        real t1378
        real t138
        real t1380
        real t1383
        real t1384
        real t1386
        real t1387
        real t1389
        real t1393
        real t1397
        real t1398
        real t1399
        real t14
        real t1400
        real t1404
        real t1406
        real t1407
        real t1408
        real t1409
        real t1411
        real t1412
        real t1414
        real t1415
        real t1421
        real t1425
        real t1427
        real t1428
        real t1429
        real t1430
        real t1432
        real t1435
        real t1436
        real t1438
        real t1440
        real t1442
        real t1443
        real t1444
        real t1446
        real t1447
        real t1448
        real t1450
        real t1453
        real t1454
        real t1455
        real t1456
        real t1458
        real t146
        real t1461
        real t1462
        real t1464
        real t1465
        real t1467
        real t1471
        real t1475
        real t1477
        real t1478
        real t1482
        real t1483
        real t1484
        real t1485
        real t1486
        real t1487
        real t1489
        real t1490
        real t1492
        real t1493
        real t1499
        real t15
        real t150
        real t1503
        real t1504
        real t1505
        real t1506
        real t1507
        real t1508
        real t151
        real t1510
        real t1513
        real t1514
        real t1516
        real t1518
        real t1520
        real t1524
        real t1525
        real t1526
        real t1528
        real t153
        real t1531
        real t1532
        real t1534
        real t1538
        real t154
        real t1540
        real t1541
        real t1542
        real t1543
        real t1544
        real t1545
        real t1546
        real t1547
        real t1548
        real t1550
        real t1551
        real t1553
        real t1554
        real t1556
        real t156
        real t1560
        real t1562
        real t1563
        real t1564
        real t1565
        real t1566
        real t1567
        real t1568
        real t157
        real t1571
        real t1572
        real t1575
        real t1576
        real t1577
        real t1579
        real t1580
        real t1582
        real t1583
        real t1584
        real t1585
        real t1587
        real t1590
        real t1591
        real t1593
        real t1598
        real t16
        real t1600
        real t1604
        real t1606
        real t1607
        real t1608
        real t1609
        real t1611
        real t1612
        real t1614
        real t1615
        real t1621
        real t1625
        real t1627
        real t1628
        real t1629
        real t163
        real t1630
        real t1632
        real t1635
        real t1636
        real t1638
        real t1640
        real t1642
        real t1643
        real t1644
        real t1646
        real t1647
        real t1649
        real t165
        real t1650
        real t1651
        real t1652
        real t1654
        real t1657
        real t1658
        real t1660
        real t1665
        real t1667
        real t1671
        real t1673
        real t1674
        real t1675
        real t1676
        real t1678
        real t1679
        real t1681
        real t1682
        real t1688
        real t1692
        real t1694
        real t1695
        real t1696
        real t1697
        real t1699
        real t17
        real t1702
        real t1703
        real t1705
        real t1707
        real t1709
        real t1713
        real t1716
        real t1718
        real t1720
        real t1724
        real t1728
        real t173
        real t1731
        real t1733
        real t1735
        real t1739
        real t174
        real t1742
        real t1743
        real t1744
        real t1747
        real t1748
        real t175
        real t1750
        real t176
        real t1761
        real t177
        real t178
        real t18
        real t180
        real t1816
        real t183
        real t1835
        real t1847
        real t185
        real t1851
        real t1855
        real t1857
        real t186
        real t1863
        real t1875
        real t188
        real t1882
        real t1884
        real t1889
        real t19
        real t1902
        real t1903
        real t1905
        real t191
        real t1923
        real t1925
        real t1929
        real t193
        real t1931
        real t1966
        real t197
        real t198
        real t1983
        real t1989
        real t199
        real t1993
        real t1997
        real t2
        real t20
        real t200
        real t2003
        real t2010
        real t2019
        real t202
        real t2021
        real t2026
        real t203
        real t2032
        real t2037
        real t2045
        real t2046
        real t2048
        real t2049
        real t205
        real t2051
        real t2055
        real t2056
        real t2058
        real t2059
        real t206
        real t2062
        real t2063
        real t2064
        real t207
        real t2074
        real t2077
        real t208
        real t2087
        real t2088
        real t209
        real t2090
        real t2091
        real t2094
        integer t21
        real t210
        real t2108
        real t2109
        real t2119
        real t2122
        real t2123
        real t2125
        real t2126
        real t2129
        real t213
        real t2130
        real t2131
        real t2144
        real t215
        real t2154
        real t2155
        real t2157
        real t2158
        real t216
        real t2161
        real t2175
        real t2176
        real t218
        real t2186
        real t22
        real t2205
        real t221
        real t222
        real t2223
        real t2225
        real t2226
        real t223
        real t2230
        real t2233
        real t2235
        real t2237
        real t2239
        real t2246
        real t2247
        real t2249
        real t225
        real t2254
        real t2257
        real t226
        real t2260
        real t2262
        real t2264
        real t2266
        real t2268
        real t2269
        real t2271
        real t2279
        real t228
        real t2281
        real t2284
        real t2286
        real t2288
        real t2290
        real t2297
        real t2298
        real t23
        real t2300
        real t2302
        real t2304
        real t2306
        real t2308
        real t2315
        real t232
        real t2321
        real t2325
        real t2326
        real t2327
        real t2333
        real t2334
        real t2335
        real t2339
        real t234
        real t2341
        real t2342
        real t2347
        real t235
        real t2350
        real t2352
        real t2354
        real t236
        real t2360
        real t2365
        real t2367
        real t2368
        real t237
        real t2371
        real t2372
        real t2378
        real t2389
        real t239
        real t2390
        real t2391
        real t2394
        real t2395
        real t2396
        real t2398
        real t240
        real t2401
        real t2402
        real t2406
        real t2408
        real t2412
        real t2415
        real t2416
        real t2418
        real t242
        real t2420
        real t2425
        real t243
        real t2430
        real t2431
        real t2433
        real t2437
        real t2439
        real t2441
        real t2449
        real t2451
        real t2456
        real t2458
        real t2459
        real t2462
        real t2466
        real t2468
        real t2476
        real t2478
        real t2483
        real t2485
        real t2486
        real t2489
        real t249
        real t2492
        real t2494
        real t2498
        real t25
        real t2500
        real t2501
        real t2502
        real t2503
        real t2504
        real t2505
        real t2506
        real t2507
        real t2509
        real t2512
        real t2513
        real t2515
        real t2516
        real t2517
        real t2518
        real t2519
        real t2520
        real t2521
        real t2523
        real t2529
        real t253
        real t2533
        real t2534
        real t2537
        real t2538
        real t2541
        real t2543
        real t2545
        real t2552
        real t2554
        real t2555
        real t2558
        real t2559
        real t2565
        real t257
        real t2576
        real t2577
        real t2578
        real t2581
        real t2582
        real t2583
        real t2585
        real t2588
        real t2589
        real t259
        real t2593
        real t2595
        real t2599
        real t26
        real t260
        real t2602
        real t2603
        real t2605
        real t2607
        real t261
        real t2613
        real t2617
        real t262
        real t2624
        real t2628
        real t2629
        integer t2630
        real t2631
        real t2632
        real t2634
        real t2635
        real t2638
        real t2639
        real t264
        real t2640
        real t2641
        real t2642
        real t2645
        real t2646
        real t2648
        real t265
        real t2651
        real t2656
        real t2658
        real t2659
        real t2661
        real t2667
        real t267
        real t2670
        real t2674
        real t2678
        real t268
        real t2681
        real t2683
        real t2685
        real t2687
        real t2690
        real t2691
        real t2692
        real t2694
        real t2695
        real t2696
        real t2698
        real t2701
        real t2702
        real t2703
        real t2704
        real t2706
        real t2709
        real t271
        real t2710
        real t2713
        real t2716
        real t2719
        real t2721
        real t2722
        real t2724
        real t2727
        real t2728
        real t2731
        real t2739
        real t274
        real t2742
        real t2750
        real t2755
        real t2760
        real t2773
        real t2775
        real t2779
        real t278
        real t2784
        real t2786
        real t2789
        real t2791
        real t2795
        real t2796
        real t2798
        real t2799
        real t280
        real t2802
        real t2803
        real t2804
        real t281
        real t2817
        real t282
        real t2827
        real t2828
        real t283
        real t2830
        real t2831
        real t2834
        real t2848
        real t2849
        real t285
        real t2859
        real t286
        real t2862
        real t2863
        real t2865
        real t2866
        real t2869
        real t287
        real t2870
        real t2871
        real t2884
        real t289
        real t2894
        real t2895
        real t2897
        real t2898
        real t29
        real t2901
        real t291
        real t2915
        real t2916
        real t292
        real t2926
        real t293
        real t294
        real t2945
        real t295
        real t2963
        real t2965
        real t297
        real t2976
        real t30
        real t300
        real t301
        real t303
        real t3031
        real t304
        real t305
        real t3050
        real t3062
        real t3066
        real t3070
        real t3072
        real t3078
        real t308
        real t309
        real t3090
        real t31
        integer t310
        real t3107
        real t3108
        real t311
        real t312
        real t3128
        real t3130
        real t3134
        real t3136
        real t314
        real t315
        real t317
        real t3171
        real t318
        real t3188
        real t319
        real t3194
        real t3198
        real t32
        real t320
        real t3202
        real t3208
        real t321
        real t322
        real t3227
        real t3232
        real t3240
        real t3241
        real t3245
        real t3248
        real t325
        real t3250
        real t3252
        real t3255
        real t3259
        real t326
        real t3260
        real t3262
        real t3267
        real t327
        real t3270
        real t3271
        real t3273
        real t3279
        real t328
        real t3282
        real t329
        real t3290
        real t3297
        real t33
        real t3303
        real t3306
        real t3307
        real t3308
        real t3309
        real t331
        real t3324
        real t3333
        real t335
        real t3352
        real t3357
        real t3359
        real t336
        real t3365
        real t3371
        real t3375
        real t338
        real t3380
        real t3386
        real t339
        real t3390
        real t3394
        real t34
        real t3400
        real t3404
        real t3407
        real t341
        real t3410
        real t3412
        real t3414
        real t3425
        real t3445
        real t3449
        real t345
        real t3457
        real t3463
        real t3466
        real t3468
        real t347
        real t3470
        real t3476
        real t348
        real t3482
        real t3484
        real t3488
        real t349
        real t3491
        real t3493
        real t3494
        real t3497
        real t3498
        real t350
        real t3504
        real t3515
        real t3516
        real t3517
        real t352
        real t3520
        real t3521
        real t3522
        real t3524
        real t3527
        real t3528
        real t353
        real t3532
        real t3534
        real t3535
        real t3538
        real t354
        real t3541
        real t3542
        real t3544
        real t3546
        real t355
        real t3551
        real t3556
        real t3558
        real t356
        real t3566
        real t3567
        real t3569
        real t3575
        real t3579
        real t3582
        real t3585
        real t3587
        real t3589
        real t3597
        real t3599
        real t36
        real t360
        real t3603
        real t3606
        real t3608
        real t3609
        real t3612
        real t3613
        real t3619
        real t362
        real t3630
        real t3631
        real t3632
        real t3635
        real t3636
        real t3637
        real t3639
        real t3642
        real t3643
        real t3647
        real t3649
        real t3653
        real t3656
        real t3657
        real t3659
        real t366
        real t3661
        real t3667
        real t367
        real t3671
        real t3676
        real t3677
        real t3678
        real t3679
        real t3680
        real t3681
        real t3682
        real t3684
        real t3685
        real t3686
        real t3687
        real t3689
        real t3692
        real t3694
        real t3695
        real t3696
        real t3697
        real t3699
        real t370
        real t3700
        real t3702
        real t3705
        real t3707
        real t3709
        real t3711
        real t3713
        real t3714
        real t3715
        real t3716
        real t3717
        real t3719
        real t372
        real t3720
        real t3721
        real t3723
        real t3726
        real t3728
        real t3729
        real t373
        real t3731
        real t3734
        integer t3738
        real t3739
        real t374
        real t3741
        real t3746
        real t3748
        real t375
        real t3752
        real t3756
        real t3758
        real t3766
        real t3767
        real t3769
        real t377
        real t3770
        real t3773
        real t378
        real t3787
        real t3789
        real t3790
        real t3791
        real t3792
        real t3795
        real t3798
        real t38
        real t380
        real t3805
        real t3808
        real t3809
        real t381
        real t3812
        real t3814
        real t3818
        real t3819
        real t3821
        real t3825
        real t3828
        real t3829
        real t3831
        real t3835
        real t3838
        real t3840
        real t3842
        real t3848
        real t385
        real t3852
        real t3853
        real t3856
        real t3858
        real t3862
        real t3865
        real t3867
        real t3869
        real t387
        real t3874
        real t3878
        real t3880
        real t3882
        real t3884
        real t3888
        real t3892
        real t3894
        real t39
        real t3900
        real t391
        real t3912
        real t393
        real t394
        real t3942
        real t395
        real t396
        real t3961
        real t397
        real t398
        real t399
        real t4
        real t40
        real t400
        real t4006
        real t401
        real t402
        real t4024
        real t4025
        real t4028
        real t4030
        real t4031
        real t405
        real t406
        real t4062
        real t407
        real t408
        real t4081
        real t409
        real t410
        real t413
        real t4133
        real t4134
        real t414
        real t4141
        real t4142
        real t4151
        real t416
        real t4161
        real t4162
        real t4164
        real t4165
        real t4168
        real t417
        real t418
        real t4182
        real t4183
        real t4193
        real t420
        real t4200
        real t4202
        real t4206
        real t4210
        real t4211
        real t4220
        real t423
        real t4230
        real t4231
        real t4233
        real t4234
        real t4237
        real t4251
        real t4252
        real t426
        real t4262
        real t427
        real t428
        real t4287
        real t429
        real t4290
        real t4292
        real t4294
        real t4296
        real t4297
        real t4299
        real t43
        real t4301
        real t4303
        real t4305
        real t4307
        real t431
        real t4310
        real t4312
        real t4315
        real t4317
        real t4324
        real t4328
        real t4329
        real t433
        real t4331
        real t4337
        real t4340
        real t4342
        real t4344
        real t4346
        real t4348
        real t435
        real t4350
        real t4356
        real t4358
        real t436
        real t4361
        real t4363
        real t4365
        real t4367
        real t4373
        real t4375
        real t438
        real t4382
        real t4383
        real t4385
        real t4387
        real t4389
        real t439
        real t4391
        real t4393
        real t44
        real t4400
        real t4402
        real t4403
        real t4409
        real t441
        real t4413
        real t4414
        real t4415
        real t4416
        real t442
        real t4431
        real t444
        real t4440
        real t4459
        real t4464
        real t4466
        real t4475
        real t4481
        real t4485
        real t4488
        real t449
        real t4491
        real t4493
        real t4495
        real t45
        real t4506
        real t451
        real t452
        real t4526
        real t4530
        real t4535
        real t4536
        real t4537
        real t4539
        real t454
        real t4540
        real t4541
        real t4543
        real t4546
        real t4548
        real t4549
        real t4551
        real t4554
        integer t4558
        real t4559
        real t4561
        real t4566
        real t4568
        real t4572
        real t4576
        real t4578
        real t458
        real t4586
        real t4587
        real t4589
        real t4590
        real t4593
        real t460
        real t4607
        real t4609
        real t4610
        real t4611
        real t4612
        real t4615
        real t4618
        real t462
        real t4625
        real t4628
        real t4629
        real t4632
        real t4634
        real t4638
        real t4639
        real t464
        real t4641
        real t4645
        real t4648
        real t4649
        real t465
        real t4651
        real t4655
        real t4658
        real t4660
        real t4662
        real t4668
        real t467
        real t4672
        real t4673
        real t4676
        real t4678
        real t4680
        real t4683
        real t4687
        real t4689
        real t4692
        real t4694
        real t47
        real t4701
        real t4702
        real t471
        real t4711
        real t4721
        real t4722
        real t4724
        real t4725
        real t4728
        real t473
        real t474
        real t4742
        real t4743
        real t4753
        real t476
        real t4760
        real t4762
        real t4766
        real t4770
        real t4771
        real t478
        real t4780
        real t4790
        real t4791
        real t4793
        real t4794
        real t4797
        real t48
        real t4811
        real t4812
        real t482
        real t4822
        real t4847
        real t4849
        real t4852
        real t4854
        real t4858
        real t486
        real t4862
        real t4864
        real t4870
        real t4882
        real t489
        real t491
        real t4912
        real t493
        real t4931
        real t497
        real t4976
        real t4994
        real t4995
        real t4997
        real t5
        integer t50
        real t500
        real t501
        real t502
        real t5028
        real t5047
        real t505
        real t508
        real t509
        real t5099
        real t51
        real t5100
        real t5102
        real t5109
        real t511
        real t5111
        real t5114
        real t5116
        real t512
        real t5125
        real t5129
        real t5130
        real t5132
        real t5135
        real t5139
        real t514
        real t5141
        real t5143
        real t5146
        real t515
        real t5154
        real t5161
        real t5167
        real t5168
        real t517
        real t5178
        real t5179
        real t5190
        real t5191
        real t52
        real t5200
        real t521
        real t5210
        real t5211
        real t5222
        real t5223
        real t5227
        real t523
        real t524
        real t526
        real t530
        real t534
        real t536
        real t537
        real t539
        real t54
        real t543
        real t545
        real t546
        real t547
        real t548
        real t55
        real t550
        real t551
        real t552
        real t554
        real t555
        real t557
        real t558
        real t560
        real t561
        real t563
        real t564
        real t566
        real t57
        real t570
        real t572
        real t573
        real t575
        real t579
        real t58
        real t583
        real t585
        real t586
        real t588
        real t59
        real t592
        real t594
        real t595
        real t596
        real t597
        real t599
        real t6
        real t60
        real t600
        real t601
        real t603
        real t605
        real t608
        real t609
        real t61
        real t610
        real t611
        real t613
        real t614
        real t617
        real t618
        real t619
        real t62
        real t620
        real t621
        real t623
        real t625
        real t627
        real t629
        real t631
        real t637
        real t638
        real t639
        integer t640
        real t641
        real t642
        real t644
        real t645
        real t647
        real t648
        real t649
        real t65
        real t650
        real t651
        real t652
        real t654
        real t656
        real t66
        real t662
        real t663
        real t667
        real t669
        real t670
        real t671
        real t672
        real t673
        real t675
        real t677
        real t678
        real t679
        real t68
        real t680
        real t683
        real t684
        real t687
        real t688
        real t69
        real t690
        real t691
        real t692
        real t698
        integer t699
        real t7
        real t700
        real t701
        real t702
        real t705
        integer t706
        real t707
        real t709
        real t71
        real t712
        real t716
        real t717
        real t719
        real t722
        real t723
        real t725
        real t728
        real t732
        real t734
        real t735
        real t737
        real t740
        real t741
        real t743
        real t746
        real t750
        real t752
        real t758
        real t759
        integer t76
        real t760
        real t762
        real t766
        real t767
        real t769
        real t77
        real t770
        real t772
        real t776
        real t778
        real t780
        real t782
        real t784
        real t789
        real t79
        real t791
        real t794
        real t798
        real t8
        real t801
        real t804
        real t808
        integer t81
        real t810
        real t813
        real t815
        real t818
        real t82
        real t822
        real t824
        real t829
        real t830
        real t832
        real t833
        real t835
        real t836
        real t84
        real t840
        real t842
        real t844
        real t848
        real t850
        real t852
        real t854
        real t856
        real t857
        real t858
        real t860
        real t861
        real t863
        real t864
        real t868
        real t87
        real t870
        real t872
        real t876
        real t878
        real t880
        real t882
        real t887
        real t888
        real t889
        real t890
        real t891
        real t892
        real t894
        real t896
        real t898
        real t9
        real t900
        real t902
        real t908
        real t909
        real t910
        real t911
        real t912
        real t913
        real t914
        real t916
        real t918
        real t92
        real t924
        real t925
        real t929
        real t93
        real t931
        real t932
        real t933
        real t934
        real t936
        real t937
        real t938
        real t939
        real t944
        real t945
        real t947
        real t948
        real t949
        real t95
        real t952
        real t953
        real t955
        real t956
        real t957
        real t96
        real t963
        real t964
        real t966
        real t967
        real t968
        real t969
        real t970
        real t971
        real t975
        real t977
        real t978
        real t979
        real t98
        real t980
        real t981
        real t983
        real t985
        real t986
        real t987
        real t988
        real t991
        real t992
        real t994
        real t995
        real t996
        t1 = u(i,j,n)
        t2 = ut(i,j,n)
        t4 = beta ** 2
        t5 = sqrt(0.3E1)
        t6 = t5 / 0.6E1
        t7 = 0.1E1 / 0.2E1 + t6
        t8 = t7 ** 2
        t9 = t4 * t8
        t10 = dt ** 2
        t11 = t10 * dx
        t12 = i + 2
        t13 = rx(t12,j,0,0)
        t14 = t13 ** 2
        t15 = rx(t12,j,0,1)
        t16 = t15 ** 2
        t17 = t14 + t16
        t18 = sqrt(t17)
        t19 = cc * t18
        t20 = cc ** 2
        t21 = i + 3
        t22 = rx(t21,j,0,0)
        t23 = rx(t21,j,1,1)
        t25 = rx(t21,j,0,1)
        t26 = rx(t21,j,1,0)
        t29 = 0.1E1 / (t22 * t23 - t25 * t26)
        t30 = t22 ** 2
        t31 = t25 ** 2
        t32 = t30 + t31
        t33 = t29 * t32
        t34 = rx(t12,j,1,1)
        t36 = rx(t12,j,1,0)
        t38 = t13 * t34 - t15 * t36
        t39 = 0.1E1 / t38
        t40 = t39 * t17
        t43 = t20 * (t33 / 0.2E1 + t40 / 0.2E1)
        t44 = ut(t21,j,n)
        t45 = ut(t12,j,n)
        t47 = 0.1E1 / dx
        t48 = (t44 - t45) * t47
        t50 = i + 1
        t51 = rx(t50,j,0,0)
        t52 = rx(t50,j,1,1)
        t54 = rx(t50,j,0,1)
        t55 = rx(t50,j,1,0)
        t57 = t51 * t52 - t54 * t55
        t58 = 0.1E1 / t57
        t59 = t51 ** 2
        t60 = t54 ** 2
        t61 = t59 + t60
        t62 = t58 * t61
        t65 = t20 * (t40 / 0.2E1 + t62 / 0.2E1)
        t66 = ut(t50,j,n)
        t68 = (t45 - t66) * t47
        t69 = t65 * t68
        t71 = (t43 * t48 - t69) * t47
        t76 = j + 1
        t77 = ut(t21,t76,n)
        t79 = 0.1E1 / dy
        t81 = j - 1
        t82 = ut(t21,t81,n)
        t92 = t13 * t36 + t15 * t34
        t93 = ut(t12,t76,n)
        t95 = (t93 - t45) * t79
        t96 = ut(t12,t81,n)
        t98 = (t45 - t96) * t79
        t84 = t20 * t39 * t92
        t102 = t84 * (t95 / 0.2E1 + t98 / 0.2E1)
        t87 = t20 * t29 * (t22 * t26 + t23 * t25)
        t104 = (t87 * ((t77 - t44) * t79 / 0.2E1 + (t44 - t82) * t79 / 0
     #.2E1) - t102) * t47
        t109 = t51 * t55 + t52 * t54
        t110 = ut(t50,t76,n)
        t112 = (t110 - t66) * t79
        t113 = ut(t50,t81,n)
        t115 = (t66 - t113) * t79
        t111 = t20 * t58 * t109
        t119 = t111 * (t112 / 0.2E1 + t115 / 0.2E1)
        t121 = (t102 - t119) * t47
        t122 = t121 / 0.2E1
        t123 = rx(t12,t76,0,0)
        t124 = rx(t12,t76,1,1)
        t126 = rx(t12,t76,0,1)
        t127 = rx(t12,t76,1,0)
        t129 = t123 * t124 - t126 * t127
        t130 = 0.1E1 / t129
        t136 = (t77 - t93) * t47
        t138 = (t93 - t110) * t47
        t146 = t84 * (t48 / 0.2E1 + t68 / 0.2E1)
        t150 = rx(t12,t81,0,0)
        t151 = rx(t12,t81,1,1)
        t153 = rx(t12,t81,0,1)
        t154 = rx(t12,t81,1,0)
        t156 = t150 * t151 - t153 * t154
        t157 = 0.1E1 / t156
        t163 = (t82 - t96) * t47
        t165 = (t96 - t113) * t47
        t173 = t127 ** 2
        t174 = t124 ** 2
        t176 = t130 * (t173 + t174)
        t177 = t36 ** 2
        t178 = t34 ** 2
        t180 = t39 * (t177 + t178)
        t183 = t20 * (t176 / 0.2E1 + t180 / 0.2E1)
        t185 = t154 ** 2
        t186 = t151 ** 2
        t188 = t157 * (t185 + t186)
        t191 = t20 * (t180 / 0.2E1 + t188 / 0.2E1)
        t197 = sqrt(t61)
        t198 = cc * t197
        t199 = rx(i,j,0,0)
        t200 = rx(i,j,1,1)
        t202 = rx(i,j,0,1)
        t203 = rx(i,j,1,0)
        t205 = t199 * t200 - t202 * t203
        t206 = 0.1E1 / t205
        t207 = t199 ** 2
        t208 = t202 ** 2
        t209 = t207 + t208
        t210 = t206 * t209
        t213 = t20 * (t62 / 0.2E1 + t210 / 0.2E1)
        t215 = (t66 - t2) * t47
        t216 = t213 * t215
        t218 = (t69 - t216) * t47
        t222 = t199 * t203 + t200 * t202
        t223 = ut(i,t76,n)
        t225 = (t223 - t2) * t79
        t226 = ut(i,t81,n)
        t228 = (t2 - t226) * t79
        t175 = t20 * t206 * t222
        t232 = t175 * (t225 / 0.2E1 + t228 / 0.2E1)
        t234 = (t119 - t232) * t47
        t235 = t234 / 0.2E1
        t236 = rx(t50,t76,0,0)
        t237 = rx(t50,t76,1,1)
        t239 = rx(t50,t76,0,1)
        t240 = rx(t50,t76,1,0)
        t242 = t236 * t237 - t239 * t240
        t243 = 0.1E1 / t242
        t249 = (t110 - t223) * t47
        t193 = t20 * t243 * (t236 * t240 + t237 * t239)
        t253 = t193 * (t138 / 0.2E1 + t249 / 0.2E1)
        t257 = t111 * (t68 / 0.2E1 + t215 / 0.2E1)
        t259 = (t253 - t257) * t79
        t260 = t259 / 0.2E1
        t261 = rx(t50,t81,0,0)
        t262 = rx(t50,t81,1,1)
        t264 = rx(t50,t81,0,1)
        t265 = rx(t50,t81,1,0)
        t267 = t261 * t262 - t264 * t265
        t268 = 0.1E1 / t267
        t274 = (t113 - t226) * t47
        t221 = t20 * t268 * (t261 * t265 + t262 * t264)
        t278 = t221 * (t165 / 0.2E1 + t274 / 0.2E1)
        t280 = (t257 - t278) * t79
        t281 = t280 / 0.2E1
        t282 = t240 ** 2
        t283 = t237 ** 2
        t285 = t243 * (t282 + t283)
        t286 = t55 ** 2
        t287 = t52 ** 2
        t289 = t58 * (t286 + t287)
        t292 = t20 * (t285 / 0.2E1 + t289 / 0.2E1)
        t293 = t292 * t112
        t294 = t265 ** 2
        t295 = t262 ** 2
        t297 = t268 * (t294 + t295)
        t300 = t20 * (t289 / 0.2E1 + t297 / 0.2E1)
        t301 = t300 * t115
        t303 = (t293 - t301) * t79
        t304 = t218 + t122 + t235 + t260 + t281 + t303
        t305 = t198 * t304
        t308 = sqrt(t209)
        t309 = cc * t308
        t310 = i - 1
        t311 = rx(t310,j,0,0)
        t312 = rx(t310,j,1,1)
        t314 = rx(t310,j,0,1)
        t315 = rx(t310,j,1,0)
        t317 = t311 * t312 - t314 * t315
        t318 = 0.1E1 / t317
        t319 = t311 ** 2
        t320 = t314 ** 2
        t321 = t319 + t320
        t322 = t318 * t321
        t325 = t20 * (t210 / 0.2E1 + t322 / 0.2E1)
        t326 = ut(t310,j,n)
        t328 = (t2 - t326) * t47
        t329 = t325 * t328
        t331 = (t216 - t329) * t47
        t335 = t311 * t315 + t312 * t314
        t336 = ut(t310,t76,n)
        t338 = (t336 - t326) * t79
        t339 = ut(t310,t81,n)
        t341 = (t326 - t339) * t79
        t271 = t20 * t318 * t335
        t345 = t271 * (t338 / 0.2E1 + t341 / 0.2E1)
        t347 = (t232 - t345) * t47
        t348 = t347 / 0.2E1
        t349 = rx(i,t76,0,0)
        t350 = rx(i,t76,1,1)
        t352 = rx(i,t76,0,1)
        t353 = rx(i,t76,1,0)
        t355 = t349 * t350 - t352 * t353
        t356 = 0.1E1 / t355
        t360 = t349 * t353 + t350 * t352
        t362 = (t223 - t336) * t47
        t291 = t20 * t356 * t360
        t366 = t291 * (t249 / 0.2E1 + t362 / 0.2E1)
        t370 = t175 * (t215 / 0.2E1 + t328 / 0.2E1)
        t372 = (t366 - t370) * t79
        t373 = t372 / 0.2E1
        t374 = rx(i,t81,0,0)
        t375 = rx(i,t81,1,1)
        t377 = rx(i,t81,0,1)
        t378 = rx(i,t81,1,0)
        t380 = t374 * t375 - t377 * t378
        t381 = 0.1E1 / t380
        t385 = t374 * t378 + t375 * t377
        t387 = (t226 - t339) * t47
        t327 = t20 * t381 * t385
        t391 = t327 * (t274 / 0.2E1 + t387 / 0.2E1)
        t393 = (t370 - t391) * t79
        t394 = t393 / 0.2E1
        t395 = t353 ** 2
        t396 = t350 ** 2
        t397 = t395 + t396
        t398 = t356 * t397
        t399 = t203 ** 2
        t400 = t200 ** 2
        t401 = t399 + t400
        t402 = t206 * t401
        t405 = t20 * (t398 / 0.2E1 + t402 / 0.2E1)
        t406 = t405 * t225
        t407 = t378 ** 2
        t408 = t375 ** 2
        t409 = t407 + t408
        t410 = t381 * t409
        t413 = t20 * (t402 / 0.2E1 + t410 / 0.2E1)
        t414 = t413 * t228
        t416 = (t406 - t414) * t79
        t417 = t331 + t235 + t348 + t373 + t394 + t416
        t418 = t309 * t417
        t420 = (t305 - t418) * t47
        t354 = t20 * t130 * (t123 * t127 + t124 * t126)
        t367 = t20 * t157 * (t150 * t154 + t151 * t153)
        t423 = t11 * ((t19 * (t71 + t104 / 0.2E1 + t122 + (t354 * (t136 
     #/ 0.2E1 + t138 / 0.2E1) - t146) * t79 / 0.2E1 + (t146 - t367 * (t1
     #63 / 0.2E1 + t165 / 0.2E1)) * t79 / 0.2E1 + (t183 * t95 - t191 * t
     #98) * t79) - t305) * t47 / 0.2E1 + t420 / 0.2E1)
        t426 = beta * t7
        t427 = dt * dx
        t428 = u(t21,j,n)
        t429 = u(t12,j,n)
        t431 = (t428 - t429) * t47
        t433 = u(t50,j,n)
        t435 = (t429 - t433) * t47
        t436 = t65 * t435
        t438 = (t43 * t431 - t436) * t47
        t439 = u(t21,t76,n)
        t441 = (t439 - t428) * t79
        t442 = u(t21,t81,n)
        t444 = (t428 - t442) * t79
        t449 = u(t12,t76,n)
        t451 = (t449 - t429) * t79
        t452 = u(t12,t81,n)
        t454 = (t429 - t452) * t79
        t458 = t84 * (t451 / 0.2E1 + t454 / 0.2E1)
        t460 = (t87 * (t441 / 0.2E1 + t444 / 0.2E1) - t458) * t47
        t462 = u(t50,t76,n)
        t464 = (t462 - t433) * t79
        t465 = u(t50,t81,n)
        t467 = (t433 - t465) * t79
        t471 = t111 * (t464 / 0.2E1 + t467 / 0.2E1)
        t473 = (t458 - t471) * t47
        t474 = t473 / 0.2E1
        t476 = (t439 - t449) * t47
        t478 = (t449 - t462) * t47
        t482 = t354 * (t476 / 0.2E1 + t478 / 0.2E1)
        t486 = t84 * (t431 / 0.2E1 + t435 / 0.2E1)
        t489 = (t482 - t486) * t79 / 0.2E1
        t491 = (t442 - t452) * t47
        t493 = (t452 - t465) * t47
        t497 = t367 * (t491 / 0.2E1 + t493 / 0.2E1)
        t500 = (t486 - t497) * t79 / 0.2E1
        t501 = t183 * t451
        t502 = t191 * t454
        t505 = t438 + t460 / 0.2E1 + t474 + t489 + t500 + (t501 - t502) 
     #* t79
        t508 = (t433 - t1) * t47
        t509 = t213 * t508
        t511 = (t436 - t509) * t47
        t512 = u(i,t76,n)
        t514 = (t512 - t1) * t79
        t515 = u(i,t81,n)
        t517 = (t1 - t515) * t79
        t521 = t175 * (t514 / 0.2E1 + t517 / 0.2E1)
        t523 = (t471 - t521) * t47
        t524 = t523 / 0.2E1
        t526 = (t462 - t512) * t47
        t530 = t193 * (t478 / 0.2E1 + t526 / 0.2E1)
        t534 = t111 * (t435 / 0.2E1 + t508 / 0.2E1)
        t536 = (t530 - t534) * t79
        t537 = t536 / 0.2E1
        t539 = (t465 - t515) * t47
        t543 = t221 * (t493 / 0.2E1 + t539 / 0.2E1)
        t545 = (t534 - t543) * t79
        t546 = t545 / 0.2E1
        t547 = t292 * t464
        t548 = t300 * t467
        t550 = (t547 - t548) * t79
        t551 = t511 + t474 + t524 + t537 + t546 + t550
        t552 = t198 * t551
        t554 = (t19 * t505 - t552) * t47
        t555 = u(t310,j,n)
        t557 = (t1 - t555) * t47
        t558 = t325 * t557
        t560 = (t509 - t558) * t47
        t561 = u(t310,t76,n)
        t563 = (t561 - t555) * t79
        t564 = u(t310,t81,n)
        t566 = (t555 - t564) * t79
        t570 = t271 * (t563 / 0.2E1 + t566 / 0.2E1)
        t572 = (t521 - t570) * t47
        t573 = t572 / 0.2E1
        t575 = (t512 - t561) * t47
        t579 = t291 * (t526 / 0.2E1 + t575 / 0.2E1)
        t583 = t175 * (t508 / 0.2E1 + t557 / 0.2E1)
        t585 = (t579 - t583) * t79
        t586 = t585 / 0.2E1
        t588 = (t515 - t564) * t47
        t592 = t327 * (t539 / 0.2E1 + t588 / 0.2E1)
        t594 = (t583 - t592) * t79
        t595 = t594 / 0.2E1
        t596 = t405 * t514
        t597 = t413 * t517
        t599 = (t596 - t597) * t79
        t600 = t560 + t524 + t573 + t586 + t595 + t599
        t601 = t309 * t600
        t603 = (t552 - t601) * t47
        t605 = t427 * (t554 - t603)
        t608 = 0.1E1 / 0.2E1 - t6
        t609 = t608 * dt
        t610 = t218 - t331
        t611 = dx * t610
        t613 = t609 * t611 / 0.24E2
        t614 = t7 * dt
        t617 = beta * t608
        t618 = t617 * dt
        t619 = t62 / 0.2E1
        t620 = t210 / 0.2E1
        t621 = dx ** 2
        t623 = (t40 - t62) * t47
        t625 = (t62 - t210) * t47
        t627 = (t623 - t625) * t47
        t629 = (t210 - t322) * t47
        t631 = (t625 - t629) * t47
        t637 = t20 * (t619 + t620 - t621 * (t627 / 0.2E1 + t631 / 0.2E1)
     # / 0.8E1)
        t638 = t637 * t508
        t639 = t322 / 0.2E1
        t640 = i - 2
        t641 = rx(t640,j,0,0)
        t642 = rx(t640,j,1,1)
        t644 = rx(t640,j,0,1)
        t645 = rx(t640,j,1,0)
        t647 = t641 * t642 - t644 * t645
        t648 = 0.1E1 / t647
        t649 = t641 ** 2
        t650 = t644 ** 2
        t651 = t649 + t650
        t652 = t648 * t651
        t654 = (t322 - t652) * t47
        t656 = (t629 - t654) * t47
        t662 = t20 * (t620 + t639 - t621 * (t631 / 0.2E1 + t656 / 0.2E1)
     # / 0.8E1)
        t663 = t662 * t557
        t667 = (t435 - t508) * t47
        t669 = (t508 - t557) * t47
        t670 = t667 - t669
        t671 = t670 * t47
        t672 = t213 * t671
        t673 = u(t640,j,n)
        t675 = (t555 - t673) * t47
        t677 = (t557 - t675) * t47
        t678 = t669 - t677
        t679 = t678 * t47
        t680 = t325 * t679
        t683 = t511 - t560
        t684 = t683 * t47
        t687 = t20 * (t322 / 0.2E1 + t652 / 0.2E1)
        t688 = t687 * t675
        t690 = (t558 - t688) * t47
        t691 = t560 - t690
        t692 = t691 * t47
        t698 = dy ** 2
        t699 = j + 2
        t700 = u(t50,t699,n)
        t702 = (t700 - t462) * t79
        t705 = (t702 / 0.2E1 - t467 / 0.2E1) * t79
        t706 = j - 2
        t707 = u(t50,t706,n)
        t709 = (t465 - t707) * t79
        t712 = (t464 / 0.2E1 - t709 / 0.2E1) * t79
        t716 = t111 * (t705 - t712) * t79
        t717 = u(i,t699,n)
        t719 = (t717 - t512) * t79
        t722 = (t719 / 0.2E1 - t517 / 0.2E1) * t79
        t723 = u(i,t706,n)
        t725 = (t515 - t723) * t79
        t728 = (t514 / 0.2E1 - t725 / 0.2E1) * t79
        t732 = t175 * (t722 - t728) * t79
        t734 = (t716 - t732) * t47
        t735 = u(t310,t699,n)
        t737 = (t735 - t561) * t79
        t740 = (t737 / 0.2E1 - t566 / 0.2E1) * t79
        t741 = u(t310,t706,n)
        t743 = (t564 - t741) * t79
        t746 = (t563 / 0.2E1 - t743 / 0.2E1) * t79
        t750 = t271 * (t740 - t746) * t79
        t752 = (t732 - t750) * t47
        t758 = (t473 - t523) * t47
        t760 = (t523 - t572) * t47
        t762 = (t758 - t760) * t47
        t766 = t641 * t645 + t642 * t644
        t767 = u(t640,t76,n)
        t769 = (t767 - t673) * t79
        t770 = u(t640,t81,n)
        t772 = (t673 - t770) * t79
        t701 = t20 * t648 * t766
        t776 = t701 * (t769 / 0.2E1 + t772 / 0.2E1)
        t778 = (t570 - t776) * t47
        t780 = (t572 - t778) * t47
        t782 = (t760 - t780) * t47
        t789 = (t478 / 0.2E1 - t575 / 0.2E1) * t47
        t791 = (t561 - t767) * t47
        t794 = (t526 / 0.2E1 - t791 / 0.2E1) * t47
        t798 = t291 * (t789 - t794) * t47
        t801 = (t435 / 0.2E1 - t557 / 0.2E1) * t47
        t804 = (t508 / 0.2E1 - t675 / 0.2E1) * t47
        t808 = t175 * (t801 - t804) * t47
        t810 = (t798 - t808) * t79
        t813 = (t493 / 0.2E1 - t588 / 0.2E1) * t47
        t815 = (t564 - t770) * t47
        t818 = (t539 / 0.2E1 - t815 / 0.2E1) * t47
        t822 = t327 * (t813 - t818) * t47
        t824 = (t808 - t822) * t79
        t829 = rx(i,t699,0,0)
        t830 = rx(i,t699,1,1)
        t832 = rx(i,t699,0,1)
        t833 = rx(i,t699,1,0)
        t835 = t829 * t830 - t832 * t833
        t836 = 0.1E1 / t835
        t840 = t829 * t833 + t830 * t832
        t842 = (t700 - t717) * t47
        t844 = (t717 - t735) * t47
        t759 = t20 * t836 * t840
        t848 = t759 * (t842 / 0.2E1 + t844 / 0.2E1)
        t850 = (t848 - t579) * t79
        t852 = (t850 - t585) * t79
        t854 = (t585 - t594) * t79
        t856 = (t852 - t854) * t79
        t857 = rx(i,t706,0,0)
        t858 = rx(i,t706,1,1)
        t860 = rx(i,t706,0,1)
        t861 = rx(i,t706,1,0)
        t863 = t857 * t858 - t860 * t861
        t864 = 0.1E1 / t863
        t868 = t857 * t861 + t858 * t860
        t870 = (t707 - t723) * t47
        t872 = (t723 - t741) * t47
        t784 = t20 * t864 * t868
        t876 = t784 * (t870 / 0.2E1 + t872 / 0.2E1)
        t878 = (t592 - t876) * t79
        t880 = (t594 - t878) * t79
        t882 = (t854 - t880) * t79
        t887 = t398 / 0.2E1
        t888 = t402 / 0.2E1
        t889 = t833 ** 2
        t890 = t830 ** 2
        t891 = t889 + t890
        t892 = t836 * t891
        t894 = (t892 - t398) * t79
        t896 = (t398 - t402) * t79
        t898 = (t894 - t896) * t79
        t900 = (t402 - t410) * t79
        t902 = (t896 - t900) * t79
        t908 = t20 * (t887 + t888 - t698 * (t898 / 0.2E1 + t902 / 0.2E1)
     # / 0.8E1)
        t909 = t908 * t514
        t910 = t410 / 0.2E1
        t911 = t861 ** 2
        t912 = t858 ** 2
        t913 = t911 + t912
        t914 = t864 * t913
        t916 = (t410 - t914) * t79
        t918 = (t900 - t916) * t79
        t924 = t20 * (t888 + t910 - t698 * (t902 / 0.2E1 + t918 / 0.2E1)
     # / 0.8E1)
        t925 = t924 * t517
        t929 = (t719 - t514) * t79
        t931 = (t514 - t517) * t79
        t932 = t929 - t931
        t933 = t932 * t79
        t934 = t405 * t933
        t936 = (t517 - t725) * t79
        t937 = t931 - t936
        t938 = t937 * t79
        t939 = t413 * t938
        t944 = t20 * (t892 / 0.2E1 + t398 / 0.2E1)
        t945 = t944 * t719
        t947 = (t945 - t596) * t79
        t948 = t947 - t599
        t949 = t948 * t79
        t952 = t20 * (t410 / 0.2E1 + t914 / 0.2E1)
        t953 = t952 * t725
        t955 = (t597 - t953) * t79
        t956 = t599 - t955
        t957 = t956 * t79
        t963 = (t638 - t663) * t47 - t621 * ((t672 - t680) * t47 + (t684
     # - t692) * t47) / 0.24E2 + t524 + t573 - t698 * (t734 / 0.2E1 + t7
     #52 / 0.2E1) / 0.6E1 - t621 * (t762 / 0.2E1 + t782 / 0.2E1) / 0.6E1
     # + t586 + t595 - t621 * (t810 / 0.2E1 + t824 / 0.2E1) / 0.6E1 - t6
     #98 * (t856 / 0.2E1 + t882 / 0.2E1) / 0.6E1 + (t909 - t925) * t79 -
     # t698 * ((t934 - t939) * t79 + (t949 - t957) * t79) / 0.24E2
        t964 = t309 * t963
        t966 = t618 * t964 / 0.2E1
        t967 = t608 ** 2
        t968 = t4 * t967
        t969 = t968 * t10
        t970 = t637 * t215
        t971 = t662 * t328
        t975 = (t68 - t215) * t47
        t977 = (t215 - t328) * t47
        t978 = t975 - t977
        t979 = t978 * t47
        t980 = t213 * t979
        t981 = ut(t640,j,n)
        t983 = (t326 - t981) * t47
        t985 = (t328 - t983) * t47
        t986 = t977 - t985
        t987 = t986 * t47
        t988 = t325 * t987
        t991 = t610 * t47
        t992 = t687 * t983
        t994 = (t329 - t992) * t47
        t995 = t331 - t994
        t996 = t995 * t47
        t1002 = ut(t50,t699,n)
        t1004 = (t1002 - t110) * t79
        t1007 = (t1004 / 0.2E1 - t115 / 0.2E1) * t79
        t1008 = ut(t50,t706,n)
        t1010 = (t113 - t1008) * t79
        t1013 = (t112 / 0.2E1 - t1010 / 0.2E1) * t79
        t1017 = t111 * (t1007 - t1013) * t79
        t1018 = ut(i,t699,n)
        t1020 = (t1018 - t223) * t79
        t1023 = (t1020 / 0.2E1 - t228 / 0.2E1) * t79
        t1024 = ut(i,t706,n)
        t1026 = (t226 - t1024) * t79
        t1029 = (t225 / 0.2E1 - t1026 / 0.2E1) * t79
        t1033 = t175 * (t1023 - t1029) * t79
        t1035 = (t1017 - t1033) * t47
        t1036 = ut(t310,t699,n)
        t1038 = (t1036 - t336) * t79
        t1041 = (t1038 / 0.2E1 - t341 / 0.2E1) * t79
        t1042 = ut(t310,t706,n)
        t1044 = (t339 - t1042) * t79
        t1047 = (t338 / 0.2E1 - t1044 / 0.2E1) * t79
        t1051 = t271 * (t1041 - t1047) * t79
        t1053 = (t1033 - t1051) * t47
        t1059 = (t121 - t234) * t47
        t1061 = (t234 - t347) * t47
        t1063 = (t1059 - t1061) * t47
        t1064 = ut(t640,t76,n)
        t1066 = (t1064 - t981) * t79
        t1067 = ut(t640,t81,n)
        t1069 = (t981 - t1067) * t79
        t1073 = t701 * (t1066 / 0.2E1 + t1069 / 0.2E1)
        t1075 = (t345 - t1073) * t47
        t1077 = (t347 - t1075) * t47
        t1079 = (t1061 - t1077) * t47
        t1086 = (t138 / 0.2E1 - t362 / 0.2E1) * t47
        t1088 = (t336 - t1064) * t47
        t1091 = (t249 / 0.2E1 - t1088 / 0.2E1) * t47
        t1095 = t291 * (t1086 - t1091) * t47
        t1098 = (t68 / 0.2E1 - t328 / 0.2E1) * t47
        t1101 = (t215 / 0.2E1 - t983 / 0.2E1) * t47
        t1105 = t175 * (t1098 - t1101) * t47
        t1107 = (t1095 - t1105) * t79
        t1110 = (t165 / 0.2E1 - t387 / 0.2E1) * t47
        t1112 = (t339 - t1067) * t47
        t1115 = (t274 / 0.2E1 - t1112 / 0.2E1) * t47
        t1119 = t327 * (t1110 - t1115) * t47
        t1121 = (t1105 - t1119) * t79
        t1127 = (t1002 - t1018) * t47
        t1129 = (t1018 - t1036) * t47
        t1133 = t759 * (t1127 / 0.2E1 + t1129 / 0.2E1)
        t1135 = (t1133 - t366) * t79
        t1137 = (t1135 - t372) * t79
        t1139 = (t372 - t393) * t79
        t1141 = (t1137 - t1139) * t79
        t1143 = (t1008 - t1024) * t47
        t1145 = (t1024 - t1042) * t47
        t1149 = t784 * (t1143 / 0.2E1 + t1145 / 0.2E1)
        t1151 = (t391 - t1149) * t79
        t1153 = (t393 - t1151) * t79
        t1155 = (t1139 - t1153) * t79
        t1160 = t908 * t225
        t1161 = t924 * t228
        t1165 = (t1020 - t225) * t79
        t1167 = (t225 - t228) * t79
        t1168 = t1165 - t1167
        t1169 = t1168 * t79
        t1170 = t405 * t1169
        t1172 = (t228 - t1026) * t79
        t1173 = t1167 - t1172
        t1174 = t1173 * t79
        t1175 = t413 * t1174
        t1178 = t944 * t1020
        t1180 = (t1178 - t406) * t79
        t1181 = t1180 - t416
        t1182 = t1181 * t79
        t1183 = t952 * t1026
        t1185 = (t414 - t1183) * t79
        t1186 = t416 - t1185
        t1187 = t1186 * t79
        t1193 = (t970 - t971) * t47 - t621 * ((t980 - t988) * t47 + (t99
     #1 - t996) * t47) / 0.24E2 + t235 + t348 - t698 * (t1035 / 0.2E1 + 
     #t1053 / 0.2E1) / 0.6E1 - t621 * (t1063 / 0.2E1 + t1079 / 0.2E1) / 
     #0.6E1 + t373 + t394 - t621 * (t1107 / 0.2E1 + t1121 / 0.2E1) / 0.6
     #E1 - t698 * (t1141 / 0.2E1 + t1155 / 0.2E1) / 0.6E1 + (t1160 - t11
     #61) * t79 - t698 * ((t1170 - t1175) * t79 + (t1182 - t1187) * t79)
     # / 0.24E2
        t1194 = t309 * t1193
        t1196 = t969 * t1194 / 0.4E1
        t1199 = t427 * (t554 / 0.2E1 + t603 / 0.2E1)
        t1202 = t8 * t7
        t1204 = t10 * dt
        t1205 = t304 * t57
        t1206 = t417 * t205
        t1207 = t1205 - t1206
        t1209 = t1204 * t1207 * t47
        t1213 = t551 * t57
        t1214 = t600 * t205
        t1215 = t1213 - t1214
        t1217 = t10 * t1215 * t47
        t1219 = t213 * t967 * t1217 / 0.2E1
        t1220 = t967 * t608
        t1223 = t213 * t1220 * t1209 / 0.6E1
        t1225 = t968 * t423 / 0.8E1
        t1227 = t617 * t605 / 0.24E2
        t1230 = t215 - dx * t978 / 0.24E2
        t1234 = t637 * t609 * t1230
        t1236 = t617 * t1199 / 0.4E1
        t1237 = sqrt(t321)
        t1238 = cc * t1237
        t1239 = t1075 / 0.2E1
        t1240 = rx(t310,t76,0,0)
        t1241 = rx(t310,t76,1,1)
        t1243 = rx(t310,t76,0,1)
        t1244 = rx(t310,t76,1,0)
        t1246 = t1240 * t1241 - t1243 * t1244
        t1247 = 0.1E1 / t1246
        t1152 = t20 * t1247 * (t1240 * t1244 + t1241 * t1243)
        t1255 = t1152 * (t362 / 0.2E1 + t1088 / 0.2E1)
        t1259 = t271 * (t328 / 0.2E1 + t983 / 0.2E1)
        t1261 = (t1255 - t1259) * t79
        t1262 = t1261 / 0.2E1
        t1263 = rx(t310,t81,0,0)
        t1264 = rx(t310,t81,1,1)
        t1266 = rx(t310,t81,0,1)
        t1267 = rx(t310,t81,1,0)
        t1269 = t1263 * t1264 - t1266 * t1267
        t1270 = 0.1E1 / t1269
        t1171 = t20 * t1270 * (t1263 * t1267 + t1264 * t1266)
        t1278 = t1171 * (t387 / 0.2E1 + t1112 / 0.2E1)
        t1280 = (t1259 - t1278) * t79
        t1281 = t1280 / 0.2E1
        t1282 = t1244 ** 2
        t1283 = t1241 ** 2
        t1285 = t1247 * (t1282 + t1283)
        t1286 = t315 ** 2
        t1287 = t312 ** 2
        t1289 = t318 * (t1286 + t1287)
        t1292 = t20 * (t1285 / 0.2E1 + t1289 / 0.2E1)
        t1293 = t1292 * t338
        t1294 = t1267 ** 2
        t1295 = t1264 ** 2
        t1297 = t1270 * (t1294 + t1295)
        t1300 = t20 * (t1289 / 0.2E1 + t1297 / 0.2E1)
        t1301 = t1300 * t341
        t1303 = (t1293 - t1301) * t79
        t1304 = t994 + t348 + t1239 + t1262 + t1281 + t1303
        t1305 = t1238 * t1304
        t1307 = (t418 - t1305) * t47
        t1310 = t11 * (t420 / 0.2E1 + t1307 / 0.2E1)
        t1312 = t9 * t1310 / 0.8E1
        t1313 = -t9 * t423 / 0.8E1 + t426 * t605 / 0.24E2 + t613 - t614 
     #* t611 / 0.24E2 + t966 + t1196 - t426 * t1199 / 0.4E1 + t213 * t12
     #02 * t1209 / 0.6E1 - t1219 - t1223 + t1225 - t1227 + t637 * t614 *
     # t1230 - t1234 + t1236 - t1312
        t1314 = t778 / 0.2E1
        t1318 = t1152 * (t575 / 0.2E1 + t791 / 0.2E1)
        t1322 = t271 * (t557 / 0.2E1 + t675 / 0.2E1)
        t1324 = (t1318 - t1322) * t79
        t1325 = t1324 / 0.2E1
        t1329 = t1171 * (t588 / 0.2E1 + t815 / 0.2E1)
        t1331 = (t1322 - t1329) * t79
        t1332 = t1331 / 0.2E1
        t1333 = t1292 * t563
        t1334 = t1300 * t566
        t1336 = (t1333 - t1334) * t79
        t1337 = t690 + t573 + t1314 + t1325 + t1332 + t1336
        t1338 = t1238 * t1337
        t1340 = (t601 - t1338) * t47
        t1342 = t427 * (t603 - t1340)
        t1344 = t426 * t1342 / 0.24E2
        t1347 = t427 * (t603 / 0.2E1 + t1340 / 0.2E1)
        t1349 = t426 * t1347 / 0.4E1
        t1351 = t617 * t1347 / 0.4E1
        t1353 = t968 * t1310 / 0.8E1
        t1354 = t4 * beta
        t1356 = t1354 * t1220 * t1204
        t1357 = t1215 * t47
        t1358 = t213 * t1357
        t1359 = t1337 * t317
        t1360 = t1214 - t1359
        t1361 = t1360 * t47
        t1362 = t325 * t1361
        t1365 = t123 ** 2
        t1366 = t126 ** 2
        t1368 = t130 * (t1365 + t1366)
        t1369 = t236 ** 2
        t1370 = t239 ** 2
        t1372 = t243 * (t1369 + t1370)
        t1375 = t20 * (t1368 / 0.2E1 + t1372 / 0.2E1)
        t1376 = t1375 * t478
        t1377 = t349 ** 2
        t1378 = t352 ** 2
        t1380 = t356 * (t1377 + t1378)
        t1383 = t20 * (t1372 / 0.2E1 + t1380 / 0.2E1)
        t1384 = t1383 * t526
        t1386 = (t1376 - t1384) * t47
        t1387 = u(t12,t699,n)
        t1389 = (t1387 - t449) * t79
        t1393 = t354 * (t1389 / 0.2E1 + t451 / 0.2E1)
        t1397 = t193 * (t702 / 0.2E1 + t464 / 0.2E1)
        t1399 = (t1393 - t1397) * t47
        t1400 = t1399 / 0.2E1
        t1404 = t291 * (t719 / 0.2E1 + t514 / 0.2E1)
        t1406 = (t1397 - t1404) * t47
        t1407 = t1406 / 0.2E1
        t1408 = rx(t50,t699,0,0)
        t1409 = rx(t50,t699,1,1)
        t1411 = rx(t50,t699,0,1)
        t1412 = rx(t50,t699,1,0)
        t1414 = t1408 * t1409 - t1411 * t1412
        t1415 = 0.1E1 / t1414
        t1421 = (t1387 - t700) * t47
        t1323 = t20 * t1415 * (t1408 * t1412 + t1409 * t1411)
        t1425 = t1323 * (t1421 / 0.2E1 + t842 / 0.2E1)
        t1427 = (t1425 - t530) * t79
        t1428 = t1427 / 0.2E1
        t1429 = t1412 ** 2
        t1430 = t1409 ** 2
        t1432 = t1415 * (t1429 + t1430)
        t1435 = t20 * (t1432 / 0.2E1 + t285 / 0.2E1)
        t1436 = t1435 * t702
        t1438 = (t1436 - t547) * t79
        t1440 = (t1386 + t1400 + t1407 + t1428 + t537 + t1438) * t242
        t1442 = (t1440 - t1213) * t79
        t1443 = t150 ** 2
        t1444 = t153 ** 2
        t1446 = t157 * (t1443 + t1444)
        t1447 = t261 ** 2
        t1448 = t264 ** 2
        t1450 = t268 * (t1447 + t1448)
        t1453 = t20 * (t1446 / 0.2E1 + t1450 / 0.2E1)
        t1454 = t1453 * t493
        t1455 = t374 ** 2
        t1456 = t377 ** 2
        t1458 = t381 * (t1455 + t1456)
        t1461 = t20 * (t1450 / 0.2E1 + t1458 / 0.2E1)
        t1462 = t1461 * t539
        t1464 = (t1454 - t1462) * t47
        t1465 = u(t12,t706,n)
        t1467 = (t452 - t1465) * t79
        t1471 = t367 * (t454 / 0.2E1 + t1467 / 0.2E1)
        t1475 = t221 * (t467 / 0.2E1 + t709 / 0.2E1)
        t1477 = (t1471 - t1475) * t47
        t1478 = t1477 / 0.2E1
        t1482 = t327 * (t517 / 0.2E1 + t725 / 0.2E1)
        t1484 = (t1475 - t1482) * t47
        t1485 = t1484 / 0.2E1
        t1486 = rx(t50,t706,0,0)
        t1487 = rx(t50,t706,1,1)
        t1489 = rx(t50,t706,0,1)
        t1490 = rx(t50,t706,1,0)
        t1492 = t1486 * t1487 - t1489 * t1490
        t1493 = 0.1E1 / t1492
        t1499 = (t1465 - t707) * t47
        t1398 = t20 * t1493 * (t1486 * t1490 + t1487 * t1489)
        t1503 = t1398 * (t1499 / 0.2E1 + t870 / 0.2E1)
        t1505 = (t543 - t1503) * t79
        t1506 = t1505 / 0.2E1
        t1507 = t1490 ** 2
        t1508 = t1487 ** 2
        t1510 = t1493 * (t1507 + t1508)
        t1513 = t20 * (t297 / 0.2E1 + t1510 / 0.2E1)
        t1514 = t1513 * t709
        t1516 = (t548 - t1514) * t79
        t1518 = (t1464 + t1478 + t1485 + t546 + t1506 + t1516) * t267
        t1520 = (t1213 - t1518) * t79
        t1524 = t111 * (t1442 / 0.2E1 + t1520 / 0.2E1)
        t1525 = t1240 ** 2
        t1526 = t1243 ** 2
        t1528 = t1247 * (t1525 + t1526)
        t1531 = t20 * (t1380 / 0.2E1 + t1528 / 0.2E1)
        t1532 = t1531 * t575
        t1534 = (t1384 - t1532) * t47
        t1538 = t1152 * (t737 / 0.2E1 + t563 / 0.2E1)
        t1540 = (t1404 - t1538) * t47
        t1541 = t1540 / 0.2E1
        t1542 = t850 / 0.2E1
        t1543 = t1534 + t1407 + t1541 + t1542 + t586 + t947
        t1544 = t1543 * t355
        t1545 = t1544 - t1214
        t1546 = t1545 * t79
        t1547 = t1263 ** 2
        t1548 = t1266 ** 2
        t1550 = t1270 * (t1547 + t1548)
        t1553 = t20 * (t1458 / 0.2E1 + t1550 / 0.2E1)
        t1554 = t1553 * t588
        t1556 = (t1462 - t1554) * t47
        t1560 = t1171 * (t566 / 0.2E1 + t743 / 0.2E1)
        t1562 = (t1482 - t1560) * t47
        t1563 = t1562 / 0.2E1
        t1564 = t878 / 0.2E1
        t1565 = t1556 + t1485 + t1563 + t595 + t1564 + t955
        t1566 = t1565 * t380
        t1567 = t1214 - t1566
        t1568 = t1567 * t79
        t1572 = t175 * (t1546 / 0.2E1 + t1568 / 0.2E1)
        t1575 = (t1524 - t1572) * t47 / 0.2E1
        t1576 = rx(t640,t76,0,0)
        t1577 = rx(t640,t76,1,1)
        t1579 = rx(t640,t76,0,1)
        t1580 = rx(t640,t76,1,0)
        t1582 = t1576 * t1577 - t1579 * t1580
        t1583 = 0.1E1 / t1582
        t1584 = t1576 ** 2
        t1585 = t1579 ** 2
        t1587 = t1583 * (t1584 + t1585)
        t1590 = t20 * (t1528 / 0.2E1 + t1587 / 0.2E1)
        t1591 = t1590 * t791
        t1593 = (t1532 - t1591) * t47
        t1598 = u(t640,t699,n)
        t1600 = (t1598 - t767) * t79
        t1483 = t20 * t1583 * (t1576 * t1580 + t1577 * t1579)
        t1604 = t1483 * (t1600 / 0.2E1 + t769 / 0.2E1)
        t1606 = (t1538 - t1604) * t47
        t1607 = t1606 / 0.2E1
        t1608 = rx(t310,t699,0,0)
        t1609 = rx(t310,t699,1,1)
        t1611 = rx(t310,t699,0,1)
        t1612 = rx(t310,t699,1,0)
        t1614 = t1608 * t1609 - t1611 * t1612
        t1615 = 0.1E1 / t1614
        t1621 = (t735 - t1598) * t47
        t1504 = t20 * t1615 * (t1608 * t1612 + t1609 * t1611)
        t1625 = t1504 * (t844 / 0.2E1 + t1621 / 0.2E1)
        t1627 = (t1625 - t1318) * t79
        t1628 = t1627 / 0.2E1
        t1629 = t1612 ** 2
        t1630 = t1609 ** 2
        t1632 = t1615 * (t1629 + t1630)
        t1635 = t20 * (t1632 / 0.2E1 + t1285 / 0.2E1)
        t1636 = t1635 * t737
        t1638 = (t1636 - t1333) * t79
        t1640 = (t1593 + t1541 + t1607 + t1628 + t1325 + t1638) * t1246
        t1642 = (t1640 - t1359) * t79
        t1643 = rx(t640,t81,0,0)
        t1644 = rx(t640,t81,1,1)
        t1646 = rx(t640,t81,0,1)
        t1647 = rx(t640,t81,1,0)
        t1649 = t1643 * t1644 - t1646 * t1647
        t1650 = 0.1E1 / t1649
        t1651 = t1643 ** 2
        t1652 = t1646 ** 2
        t1654 = t1650 * (t1651 + t1652)
        t1657 = t20 * (t1550 / 0.2E1 + t1654 / 0.2E1)
        t1658 = t1657 * t815
        t1660 = (t1554 - t1658) * t47
        t1665 = u(t640,t706,n)
        t1667 = (t770 - t1665) * t79
        t1551 = t20 * t1650 * (t1643 * t1647 + t1644 * t1646)
        t1671 = t1551 * (t772 / 0.2E1 + t1667 / 0.2E1)
        t1673 = (t1560 - t1671) * t47
        t1674 = t1673 / 0.2E1
        t1675 = rx(t310,t706,0,0)
        t1676 = rx(t310,t706,1,1)
        t1678 = rx(t310,t706,0,1)
        t1679 = rx(t310,t706,1,0)
        t1681 = t1675 * t1676 - t1678 * t1679
        t1682 = 0.1E1 / t1681
        t1688 = (t741 - t1665) * t47
        t1571 = t20 * t1682 * (t1675 * t1679 + t1676 * t1678)
        t1692 = t1571 * (t872 / 0.2E1 + t1688 / 0.2E1)
        t1694 = (t1329 - t1692) * t79
        t1695 = t1694 / 0.2E1
        t1696 = t1679 ** 2
        t1697 = t1676 ** 2
        t1699 = t1682 * (t1696 + t1697)
        t1702 = t20 * (t1297 / 0.2E1 + t1699 / 0.2E1)
        t1703 = t1702 * t743
        t1705 = (t1334 - t1703) * t79
        t1707 = (t1660 + t1563 + t1674 + t1332 + t1695 + t1705) * t1269
        t1709 = (t1359 - t1707) * t79
        t1713 = t271 * (t1642 / 0.2E1 + t1709 / 0.2E1)
        t1716 = (t1572 - t1713) * t47 / 0.2E1
        t1718 = (t1440 - t1544) * t47
        t1720 = (t1544 - t1640) * t47
        t1724 = t291 * (t1718 / 0.2E1 + t1720 / 0.2E1)
        t1728 = t175 * (t1357 / 0.2E1 + t1361 / 0.2E1)
        t1731 = (t1724 - t1728) * t79 / 0.2E1
        t1733 = (t1518 - t1566) * t47
        t1735 = (t1566 - t1707) * t47
        t1739 = t327 * (t1733 / 0.2E1 + t1735 / 0.2E1)
        t1742 = (t1728 - t1739) * t79 / 0.2E1
        t1743 = t405 * t1546
        t1744 = t413 * t1568
        t1747 = (t1358 - t1362) * t47 + t1575 + t1716 + t1731 + t1742 + 
     #(t1743 - t1744) * t79
        t1748 = t309 * t1747
        t1750 = t1356 * t1748 / 0.12E2
        t1761 = t20 * (t40 / 0.2E1 + t619 - t621 * (((t33 - t40) * t47 -
     # t623) * t47 / 0.2E1 + t627 / 0.2E1) / 0.8E1)
        t1816 = t111 * ((t431 / 0.2E1 - t508 / 0.2E1) * t47 - t801) * t4
     #7
        t1835 = (t536 - t545) * t79
        t1847 = t289 / 0.2E1
        t1851 = (t285 - t289) * t79
        t1855 = (t289 - t297) * t79
        t1857 = (t1851 - t1855) * t79
        t1863 = t20 * (t285 / 0.2E1 + t1847 - t698 * (((t1432 - t285) * 
     #t79 - t1851) * t79 / 0.2E1 + t1857 / 0.2E1) / 0.8E1)
        t1875 = t20 * (t1847 + t297 / 0.2E1 - t698 * (t1857 / 0.2E1 + (t
     #1855 - (t297 - t1510) * t79) * t79 / 0.2E1) / 0.8E1)
        t1882 = (t464 - t467) * t79
        t1884 = ((t702 - t464) * t79 - t1882) * t79
        t1889 = (t1882 - (t467 - t709) * t79) * t79
        t1902 = (t1761 * t435 - t638) * t47 - t621 * ((t65 * ((t431 - t4
     #35) * t47 - t667) * t47 - t672) * t47 + ((t438 - t511) * t47 - t68
     #4) * t47) / 0.24E2 + t474 + t524 - t698 * ((t84 * ((t1389 / 0.2E1 
     #- t454 / 0.2E1) * t79 - (t451 / 0.2E1 - t1467 / 0.2E1) * t79) * t7
     #9 - t716) * t47 / 0.2E1 + t734 / 0.2E1) / 0.6E1 - t621 * (((t460 -
     # t473) * t47 - t758) * t47 / 0.2E1 + t762 / 0.2E1) / 0.6E1 + t537 
     #+ t546 - t621 * ((t193 * ((t476 / 0.2E1 - t526 / 0.2E1) * t47 - t7
     #89) * t47 - t1816) * t79 / 0.2E1 + (t1816 - t221 * ((t491 / 0.2E1 
     #- t539 / 0.2E1) * t47 - t813) * t47) * t79 / 0.2E1) / 0.6E1 - t698
     # * (((t1427 - t536) * t79 - t1835) * t79 / 0.2E1 + (t1835 - (t545 
     #- t1505) * t79) * t79 / 0.2E1) / 0.6E1 + (t1863 * t464 - t1875 * t
     #467) * t79 - t698 * ((t1884 * t292 - t1889 * t300) * t79 + ((t1438
     # - t550) * t79 - (t550 - t1516) * t79) * t79) / 0.24E2
        t1903 = t198 * t1902
        t1905 = t618 * t1903 / 0.2E1
        t1923 = ut(t12,t699,n)
        t1925 = (t1923 - t93) * t79
        t1929 = ut(t12,t706,n)
        t1931 = (t96 - t1929) * t79
        t1966 = t111 * ((t48 / 0.2E1 - t215 / 0.2E1) * t47 - t1098) * t4
     #7
        t1983 = (t1923 - t1002) * t47
        t1989 = (t1323 * (t1983 / 0.2E1 + t1127 / 0.2E1) - t253) * t79
        t1993 = (t259 - t280) * t79
        t1997 = (t1929 - t1008) * t47
        t2003 = (t278 - t1398 * (t1997 / 0.2E1 + t1143 / 0.2E1)) * t79
        t2019 = (t112 - t115) * t79
        t2021 = ((t1004 - t112) * t79 - t2019) * t79
        t2026 = (t2019 - (t115 - t1010) * t79) * t79
        t2032 = (t1004 * t1435 - t293) * t79
        t2037 = (-t1010 * t1513 + t301) * t79
        t2045 = (t1761 * t68 - t970) * t47 - t621 * ((t65 * ((t48 - t68)
     # * t47 - t975) * t47 - t980) * t47 + ((t71 - t218) * t47 - t991) *
     # t47) / 0.24E2 + t122 + t235 - t698 * ((t84 * ((t1925 / 0.2E1 - t9
     #8 / 0.2E1) * t79 - (t95 / 0.2E1 - t1931 / 0.2E1) * t79) * t79 - t1
     #017) * t47 / 0.2E1 + t1035 / 0.2E1) / 0.6E1 - t621 * (((t104 - t12
     #1) * t47 - t1059) * t47 / 0.2E1 + t1063 / 0.2E1) / 0.6E1 + t260 + 
     #t281 - t621 * ((t193 * ((t136 / 0.2E1 - t249 / 0.2E1) * t47 - t108
     #6) * t47 - t1966) * t79 / 0.2E1 + (t1966 - t221 * ((t163 / 0.2E1 -
     # t274 / 0.2E1) * t47 - t1110) * t47) * t79 / 0.2E1) / 0.6E1 - t698
     # * (((t1989 - t259) * t79 - t1993) * t79 / 0.2E1 + (t1993 - (t280 
     #- t2003) * t79) * t79 / 0.2E1) / 0.6E1 + (t112 * t1863 - t115 * t1
     #875) * t79 - t698 * ((t2021 * t292 - t2026 * t300) * t79 + ((t2032
     # - t303) * t79 - (t303 - t2037) * t79) * t79) / 0.24E2
        t2046 = t198 * t2045
        t2048 = t969 * t2046 / 0.4E1
        t2049 = t505 * t38
        t2051 = (t2049 - t1213) * t47
        t2055 = rx(t21,t76,0,0)
        t2056 = rx(t21,t76,1,1)
        t2058 = rx(t21,t76,0,1)
        t2059 = rx(t21,t76,1,0)
        t2062 = 0.1E1 / (t2055 * t2056 - t2058 * t2059)
        t2063 = t2055 ** 2
        t2064 = t2058 ** 2
        t2077 = u(t21,t699,n)
        t2087 = rx(t12,t699,0,0)
        t2088 = rx(t12,t699,1,1)
        t2090 = rx(t12,t699,0,1)
        t2091 = rx(t12,t699,1,0)
        t2094 = 0.1E1 / (t2087 * t2088 - t2090 * t2091)
        t2108 = t2091 ** 2
        t2109 = t2088 ** 2
        t2010 = t20 * t2094 * (t2087 * t2091 + t2088 * t2090)
        t2119 = ((t20 * (t2062 * (t2063 + t2064) / 0.2E1 + t1368 / 0.2E1
     #) * t476 - t1376) * t47 + (t20 * t2062 * (t2055 * t2059 + t2056 * 
     #t2058) * ((t2077 - t439) * t79 / 0.2E1 + t441 / 0.2E1) - t1393) * 
     #t47 / 0.2E1 + t1400 + (t2010 * ((t2077 - t1387) * t47 / 0.2E1 + t1
     #421 / 0.2E1) - t482) * t79 / 0.2E1 + t489 + (t20 * (t2094 * (t2108
     # + t2109) / 0.2E1 + t176 / 0.2E1) * t1389 - t501) * t79) * t129
        t2122 = rx(t21,t81,0,0)
        t2123 = rx(t21,t81,1,1)
        t2125 = rx(t21,t81,0,1)
        t2126 = rx(t21,t81,1,0)
        t2129 = 0.1E1 / (t2122 * t2123 - t2125 * t2126)
        t2130 = t2122 ** 2
        t2131 = t2125 ** 2
        t2144 = u(t21,t706,n)
        t2154 = rx(t12,t706,0,0)
        t2155 = rx(t12,t706,1,1)
        t2157 = rx(t12,t706,0,1)
        t2158 = rx(t12,t706,1,0)
        t2161 = 0.1E1 / (t2154 * t2155 - t2157 * t2158)
        t2175 = t2158 ** 2
        t2176 = t2155 ** 2
        t2074 = t20 * t2161 * (t2154 * t2158 + t2155 * t2157)
        t2186 = ((t20 * (t2129 * (t2130 + t2131) / 0.2E1 + t1446 / 0.2E1
     #) * t491 - t1454) * t47 + (t20 * t2129 * (t2122 * t2126 + t2123 * 
     #t2125) * (t444 / 0.2E1 + (t442 - t2144) * t79 / 0.2E1) - t1471) * 
     #t47 / 0.2E1 + t1478 + t500 + (t497 - t2074 * ((t2144 - t1465) * t4
     #7 / 0.2E1 + t1499 / 0.2E1)) * t79 / 0.2E1 + (t502 - t20 * (t188 / 
     #0.2E1 + t2161 * (t2175 + t2176) / 0.2E1) * t1467) * t79) * t156
        t2205 = t111 * (t2051 / 0.2E1 + t1357 / 0.2E1)
        t2223 = t198 * ((t2051 * t65 - t1358) * t47 + (t84 * ((t2119 - t
     #2049) * t79 / 0.2E1 + (t2049 - t2186) * t79 / 0.2E1) - t1524) * t4
     #7 / 0.2E1 + t1575 + (t193 * ((t2119 - t1440) * t47 / 0.2E1 + t1718
     # / 0.2E1) - t2205) * t79 / 0.2E1 + (t2205 - t221 * ((t2186 - t1518
     #) * t47 / 0.2E1 + t1733 / 0.2E1)) * t79 / 0.2E1 + (t1442 * t292 - 
     #t1520 * t300) * t79)
        t2225 = t1356 * t2223 / 0.12E2
        t2226 = t9 * t10
        t2230 = t1354 * t1202 * t1204
        t2233 = t426 * dt
        t2235 = t2233 * t964 / 0.2E1
        t2237 = t2226 * t1194 / 0.4E1
        t2239 = t2230 * t1748 / 0.12E2
        t2246 = t617 * t1342 / 0.24E2
        t2247 = -t1344 - t1349 + t1351 + t1353 + t1750 - t1905 - t2048 -
     # t2225 + t2226 * t2046 / 0.4E1 + t2230 * t2223 / 0.12E2 - t2235 - 
     #t2237 - t2239 + t2233 * t1903 / 0.2E1 + t213 * t8 * t1217 / 0.2E1 
     #+ t2246
        t2249 = (t1313 + t2247) * t5
        t2254 = dx * t683 / 0.24E2
        t2257 = cc * t58 * t197 * t66
        t2260 = cc * t39 * t18 * t45
        t2262 = (-t2257 + t2260) * t47
        t2264 = cc * t206
        t2266 = t2264 * t308 * t2
        t2268 = (-t2266 + t2257) * t47
        t2269 = t2268 / 0.2E1
        t2271 = sqrt(t32)
        t2279 = (t2262 - t2268) * t47
        t2281 = (((cc * t2271 * t29 * t44 - t2260) * t47 - t2262) * t47 
     #- t2279) * t47
        t2284 = cc * t318 * t1237 * t326
        t2286 = (t2266 - t2284) * t47
        t2288 = (t2268 - t2286) * t47
        t2290 = (t2279 - t2288) * t47
        t2297 = dx * (t2262 / 0.2E1 + t2269 - t621 * (t2281 / 0.2E1 + t2
     #290 / 0.2E1) / 0.6E1) / 0.4E1
        t2298 = t2286 / 0.2E1
        t2300 = sqrt(t651)
        t2302 = cc * t648 * t2300 * t981
        t2304 = (-t2302 + t2284) * t47
        t2306 = (t2286 - t2304) * t47
        t2308 = (t2288 - t2306) * t47
        t2315 = dx * (t2269 + t2298 - t621 * (t2290 / 0.2E1 + t2308 / 0.
     #2E1) / 0.6E1) / 0.4E1
        t2321 = t621 * (t2288 - dx * (t2290 - t2308) / 0.12E2) / 0.24E2
        t2325 = t637 * (t508 - dx * t670 / 0.24E2)
        t2326 = t2266 / 0.2E1
        t2327 = t2257 / 0.2E1
        t2333 = t621 * (t2279 - dx * (t2281 - t2290) / 0.12E2) / 0.24E2
        t2334 = -t2249 * t608 - t1196 - t2254 - t2297 - t2315 - t2321 + 
     #t2325 - t2326 + t2327 + t2333 - t613 - t966
        t2335 = t1219 + t1223 - t1225 + t1227 + t1234 - t1236 - t1351 - 
     #t1353 - t1750 + t1905 + t2048 + t2225 - t2246
        t2339 = t58 * t109
        t2341 = t206 * t222
        t2342 = t2341 / 0.2E1
        t2347 = (t2339 - t2341) * t47
        t2350 = t318 * t335
        t2352 = (t2341 - t2350) * t47
        t2354 = (t2347 - t2352) * t47
        t2360 = t20 * (t2339 / 0.2E1 + t2342 - t621 * (((t39 * t92 - t23
     #39) * t47 - t2347) * t47 / 0.2E1 + t2354 / 0.2E1) / 0.8E1)
        t2365 = t698 * (t2021 / 0.2E1 + t2026 / 0.2E1)
        t2367 = t225 / 0.4E1
        t2368 = t228 / 0.4E1
        t2371 = t698 * (t1169 / 0.2E1 + t1174 / 0.2E1)
        t2372 = t2371 / 0.12E2
        t2378 = (t95 - t98) * t79
        t2389 = t112 / 0.2E1
        t2390 = t115 / 0.2E1
        t2391 = t2365 / 0.6E1
        t2394 = t225 / 0.2E1
        t2395 = t228 / 0.2E1
        t2396 = t2371 / 0.6E1
        t2398 = (t2389 + t2390 - t2391 - t2394 - t2395 + t2396) * t47
        t2401 = t338 / 0.2E1
        t2402 = t341 / 0.2E1
        t2406 = (t338 - t341) * t79
        t2408 = ((t1038 - t338) * t79 - t2406) * t79
        t2412 = (t2406 - (t341 - t1044) * t79) * t79
        t2415 = t698 * (t2408 / 0.2E1 + t2412 / 0.2E1)
        t2416 = t2415 / 0.6E1
        t2418 = (t2394 + t2395 - t2396 - t2401 - t2402 + t2416) * t47
        t2420 = (t2398 - t2418) * t47
        t2425 = t112 / 0.4E1 + t115 / 0.4E1 - t2365 / 0.12E2 + t2367 + t
     #2368 - t2372 - t621 * (((t95 / 0.2E1 + t98 / 0.2E1 - t698 * (((t19
     #25 - t95) * t79 - t2378) * t79 / 0.2E1 + (t2378 - (t98 - t1931) * 
     #t79) * t79 / 0.2E1) / 0.6E1 - t2389 - t2390 + t2391) * t47 - t2398
     #) * t47 / 0.2E1 + t2420 / 0.2E1) / 0.8E1
        t2430 = t20 * (t2339 / 0.2E1 + t2341 / 0.2E1)
        t2431 = t8 * t10
        t2433 = t1442 / 0.4E1 + t1520 / 0.4E1 + t1546 / 0.4E1 + t1568 / 
     #0.4E1
        t2437 = t1202 * t1204
        t2439 = t1383 * t249
        t2441 = (t1375 * t138 - t2439) * t47
        t2449 = t193 * (t1004 / 0.2E1 + t112 / 0.2E1)
        t2451 = (t354 * (t1925 / 0.2E1 + t95 / 0.2E1) - t2449) * t47
        t2456 = t291 * (t1020 / 0.2E1 + t225 / 0.2E1)
        t2458 = (t2449 - t2456) * t47
        t2459 = t2458 / 0.2E1
        t2462 = (t2441 + t2451 / 0.2E1 + t2459 + t1989 / 0.2E1 + t260 + 
     #t2032) * t242
        t2466 = t1461 * t274
        t2468 = (t1453 * t165 - t2466) * t47
        t2476 = t221 * (t115 / 0.2E1 + t1010 / 0.2E1)
        t2478 = (t367 * (t98 / 0.2E1 + t1931 / 0.2E1) - t2476) * t47
        t2483 = t327 * (t228 / 0.2E1 + t1026 / 0.2E1)
        t2485 = (t2476 - t2483) * t47
        t2486 = t2485 / 0.2E1
        t2489 = (t2468 + t2478 / 0.2E1 + t2486 + t281 + t2003 / 0.2E1 + 
     #t2037) * t267
        t2492 = t1531 * t362
        t2494 = (t2439 - t2492) * t47
        t2498 = t1152 * (t1038 / 0.2E1 + t338 / 0.2E1)
        t2500 = (t2456 - t2498) * t47
        t2501 = t2500 / 0.2E1
        t2502 = t1135 / 0.2E1
        t2503 = t2494 + t2459 + t2501 + t2502 + t373 + t1180
        t2504 = t2503 * t355
        t2505 = t2504 - t1206
        t2506 = t2505 * t79
        t2507 = t1553 * t387
        t2509 = (t2466 - t2507) * t47
        t2513 = t1171 * (t341 / 0.2E1 + t1044 / 0.2E1)
        t2515 = (t2483 - t2513) * t47
        t2516 = t2515 / 0.2E1
        t2517 = t1151 / 0.2E1
        t2518 = t2509 + t2486 + t2516 + t394 + t2517 + t1185
        t2519 = t2518 * t380
        t2520 = t1206 - t2519
        t2521 = t2520 * t79
        t2523 = (t2462 - t1205) * t79 / 0.4E1 + (t1205 - t2489) * t79 / 
     #0.4E1 + t2506 / 0.4E1 + t2521 / 0.4E1
        t2529 = dx * (t121 / 0.2E1 - t347 / 0.2E1)
        t2533 = t2360 * t609 * t2425
        t2534 = t967 * t10
        t2537 = t2430 * t2534 * t2433 / 0.2E1
        t2538 = t1220 * t1204
        t2541 = t2430 * t2538 * t2523 / 0.6E1
        t2543 = t609 * t2529 / 0.24E2
        t2545 = (t2360 * t614 * t2425 + t2430 * t2431 * t2433 / 0.2E1 + 
     #t2430 * t2437 * t2523 / 0.6E1 - t614 * t2529 / 0.24E2 - t2533 - t2
     #537 - t2541 + t2543) * t5
        t2552 = t698 * (t1884 / 0.2E1 + t1889 / 0.2E1)
        t2554 = t514 / 0.4E1
        t2555 = t517 / 0.4E1
        t2558 = t698 * (t933 / 0.2E1 + t938 / 0.2E1)
        t2559 = t2558 / 0.12E2
        t2565 = (t451 - t454) * t79
        t2576 = t464 / 0.2E1
        t2577 = t467 / 0.2E1
        t2578 = t2552 / 0.6E1
        t2581 = t514 / 0.2E1
        t2582 = t517 / 0.2E1
        t2583 = t2558 / 0.6E1
        t2585 = (t2576 + t2577 - t2578 - t2581 - t2582 + t2583) * t47
        t2588 = t563 / 0.2E1
        t2589 = t566 / 0.2E1
        t2593 = (t563 - t566) * t79
        t2595 = ((t737 - t563) * t79 - t2593) * t79
        t2599 = (t2593 - (t566 - t743) * t79) * t79
        t2602 = t698 * (t2595 / 0.2E1 + t2599 / 0.2E1)
        t2603 = t2602 / 0.6E1
        t2605 = (t2581 + t2582 - t2583 - t2588 - t2589 + t2603) * t47
        t2607 = (t2585 - t2605) * t47
        t2613 = t2360 * (t464 / 0.4E1 + t467 / 0.4E1 - t2552 / 0.12E2 + 
     #t2554 + t2555 - t2559 - t621 * (((t451 / 0.2E1 + t454 / 0.2E1 - t6
     #98 * (((t1389 - t451) * t79 - t2565) * t79 / 0.2E1 + (t2565 - (t45
     #4 - t1467) * t79) * t79 / 0.2E1) / 0.6E1 - t2576 - t2577 + t2578) 
     #* t47 - t2585) * t47 / 0.2E1 + t2607 / 0.2E1) / 0.8E1)
        t2617 = dx * (t473 / 0.2E1 - t572 / 0.2E1) / 0.24E2
        t2624 = t328 - dx * t986 / 0.24E2
        t2628 = t662 * t609 * t2624
        t2629 = cc * t2300
        t2630 = i - 3
        t2631 = rx(t2630,j,0,0)
        t2632 = rx(t2630,j,1,1)
        t2634 = rx(t2630,j,0,1)
        t2635 = rx(t2630,j,1,0)
        t2638 = 0.1E1 / (t2631 * t2632 - t2634 * t2635)
        t2639 = t2631 ** 2
        t2640 = t2634 ** 2
        t2641 = t2639 + t2640
        t2642 = t2638 * t2641
        t2645 = t20 * (t652 / 0.2E1 + t2642 / 0.2E1)
        t2646 = u(t2630,j,n)
        t2648 = (t673 - t2646) * t47
        t2651 = (-t2645 * t2648 + t688) * t47
        t2656 = u(t2630,t76,n)
        t2658 = (t2656 - t2646) * t79
        t2659 = u(t2630,t81,n)
        t2661 = (t2646 - t2659) * t79
        t2512 = t20 * t2638 * (t2631 * t2635 + t2632 * t2634)
        t2667 = (t776 - t2512 * (t2658 / 0.2E1 + t2661 / 0.2E1)) * t47
        t2670 = (t767 - t2656) * t47
        t2674 = t1483 * (t791 / 0.2E1 + t2670 / 0.2E1)
        t2678 = t701 * (t675 / 0.2E1 + t2648 / 0.2E1)
        t2681 = (t2674 - t2678) * t79 / 0.2E1
        t2683 = (t770 - t2659) * t47
        t2687 = t1551 * (t815 / 0.2E1 + t2683 / 0.2E1)
        t2690 = (t2678 - t2687) * t79 / 0.2E1
        t2691 = t1580 ** 2
        t2692 = t1577 ** 2
        t2694 = t1583 * (t2691 + t2692)
        t2695 = t645 ** 2
        t2696 = t642 ** 2
        t2698 = t648 * (t2695 + t2696)
        t2701 = t20 * (t2694 / 0.2E1 + t2698 / 0.2E1)
        t2702 = t2701 * t769
        t2703 = t1647 ** 2
        t2704 = t1644 ** 2
        t2706 = t1650 * (t2703 + t2704)
        t2709 = t20 * (t2698 / 0.2E1 + t2706 / 0.2E1)
        t2710 = t2709 * t772
        t2713 = t2651 + t1314 + t2667 / 0.2E1 + t2681 + t2690 + (t2702 -
     # t2710) * t79
        t2716 = (-t2629 * t2713 + t1338) * t47
        t2719 = t427 * (t1340 / 0.2E1 + t2716 / 0.2E1)
        t2721 = t617 * t2719 / 0.4E1
        t2722 = ut(t2630,j,n)
        t2724 = (t981 - t2722) * t47
        t2727 = (-t2645 * t2724 + t992) * t47
        t2728 = ut(t2630,t76,n)
        t2731 = ut(t2630,t81,n)
        t2739 = (t1073 - t2512 * ((t2728 - t2722) * t79 / 0.2E1 + (t2722
     # - t2731) * t79 / 0.2E1)) * t47
        t2742 = (t1064 - t2728) * t47
        t2750 = t701 * (t983 / 0.2E1 + t2724 / 0.2E1)
        t2755 = (t1067 - t2731) * t47
        t2773 = t11 * (t1307 / 0.2E1 + (t1305 - t2629 * (t2727 + t1239 +
     # t2739 / 0.2E1 + (t1483 * (t1088 / 0.2E1 + t2742 / 0.2E1) - t2750)
     # * t79 / 0.2E1 + (t2750 - t1551 * (t1112 / 0.2E1 + t2755 / 0.2E1))
     # * t79 / 0.2E1 + (t1066 * t2701 - t1069 * t2709) * t79)) * t47 / 0
     #.2E1)
        t2775 = t968 * t2773 / 0.8E1
        t2779 = t427 * (t1340 - t2716)
        t2784 = dx * t995
        t2786 = t609 * t2784 / 0.24E2
        t2789 = t2713 * t647
        t2791 = (t1359 - t2789) * t47
        t2795 = rx(t2630,t76,0,0)
        t2796 = rx(t2630,t76,1,1)
        t2798 = rx(t2630,t76,0,1)
        t2799 = rx(t2630,t76,1,0)
        t2802 = 0.1E1 / (t2795 * t2796 - t2798 * t2799)
        t2803 = t2795 ** 2
        t2804 = t2798 ** 2
        t2817 = u(t2630,t699,n)
        t2827 = rx(t640,t699,0,0)
        t2828 = rx(t640,t699,1,1)
        t2830 = rx(t640,t699,0,1)
        t2831 = rx(t640,t699,1,0)
        t2834 = 0.1E1 / (t2827 * t2828 - t2830 * t2831)
        t2848 = t2831 ** 2
        t2849 = t2828 ** 2
        t2685 = t20 * t2834 * (t2827 * t2831 + t2828 * t2830)
        t2859 = ((t1591 - t20 * (t1587 / 0.2E1 + t2802 * (t2803 + t2804)
     # / 0.2E1) * t2670) * t47 + t1607 + (t1604 - t20 * t2802 * (t2795 *
     # t2799 + t2796 * t2798) * ((t2817 - t2656) * t79 / 0.2E1 + t2658 /
     # 0.2E1)) * t47 / 0.2E1 + (t2685 * (t1621 / 0.2E1 + (t1598 - t2817)
     # * t47 / 0.2E1) - t2674) * t79 / 0.2E1 + t2681 + (t20 * (t2834 * (
     #t2848 + t2849) / 0.2E1 + t2694 / 0.2E1) * t1600 - t2702) * t79) * 
     #t1582
        t2862 = rx(t2630,t81,0,0)
        t2863 = rx(t2630,t81,1,1)
        t2865 = rx(t2630,t81,0,1)
        t2866 = rx(t2630,t81,1,0)
        t2869 = 0.1E1 / (t2862 * t2863 - t2865 * t2866)
        t2870 = t2862 ** 2
        t2871 = t2865 ** 2
        t2884 = u(t2630,t706,n)
        t2894 = rx(t640,t706,0,0)
        t2895 = rx(t640,t706,1,1)
        t2897 = rx(t640,t706,0,1)
        t2898 = rx(t640,t706,1,0)
        t2901 = 0.1E1 / (t2894 * t2895 - t2897 * t2898)
        t2915 = t2898 ** 2
        t2916 = t2895 ** 2
        t2760 = t20 * t2901 * (t2894 * t2898 + t2895 * t2897)
        t2926 = ((t1658 - t20 * (t1654 / 0.2E1 + t2869 * (t2870 + t2871)
     # / 0.2E1) * t2683) * t47 + t1674 + (t1671 - t20 * t2869 * (t2862 *
     # t2866 + t2863 * t2865) * (t2661 / 0.2E1 + (t2659 - t2884) * t79 /
     # 0.2E1)) * t47 / 0.2E1 + t2690 + (t2687 - t2760 * (t1688 / 0.2E1 +
     # (t1665 - t2884) * t47 / 0.2E1)) * t79 / 0.2E1 + (t2710 - t20 * (t
     #2706 / 0.2E1 + t2901 * (t2915 + t2916) / 0.2E1) * t1667) * t79) * 
     #t1649
        t2945 = t271 * (t1361 / 0.2E1 + t2791 / 0.2E1)
        t2963 = t1238 * ((-t2791 * t687 + t1362) * t47 + t1716 + (t1713 
     #- t701 * ((t2859 - t2789) * t79 / 0.2E1 + (t2789 - t2926) * t79 / 
     #0.2E1)) * t47 / 0.2E1 + (t1152 * (t1720 / 0.2E1 + (t1640 - t2859) 
     #* t47 / 0.2E1) - t2945) * t79 / 0.2E1 + (t2945 - t1171 * (t1735 / 
     #0.2E1 + (t1707 - t2926) * t47 / 0.2E1)) * t79 / 0.2E1 + (t1292 * t
     #1642 - t1300 * t1709) * t79)
        t2965 = t1356 * t2963 / 0.12E2
        t2976 = t20 * (t639 + t652 / 0.2E1 - t621 * (t656 / 0.2E1 + (t65
     #4 - (t652 - t2642) * t47) * t47 / 0.2E1) / 0.8E1)
        t3031 = t271 * (t804 - (t557 / 0.2E1 - t2648 / 0.2E1) * t47) * t
     #47
        t3050 = (t1324 - t1331) * t79
        t3062 = t1289 / 0.2E1
        t3066 = (t1285 - t1289) * t79
        t3070 = (t1289 - t1297) * t79
        t3072 = (t3066 - t3070) * t79
        t3078 = t20 * (t1285 / 0.2E1 + t3062 - t698 * (((t1632 - t1285) 
     #* t79 - t3066) * t79 / 0.2E1 + t3072 / 0.2E1) / 0.8E1)
        t3090 = t20 * (t3062 + t1297 / 0.2E1 - t698 * (t3072 / 0.2E1 + (
     #t3070 - (t1297 - t1699) * t79) * t79 / 0.2E1) / 0.8E1)
        t3107 = (-t2976 * t675 + t663) * t47 - t621 * ((t680 - t687 * (t
     #677 - (t675 - t2648) * t47) * t47) * t47 + (t692 - (t690 - t2651) 
     #* t47) * t47) / 0.24E2 + t573 + t1314 - t698 * (t752 / 0.2E1 + (t7
     #50 - t701 * ((t1600 / 0.2E1 - t772 / 0.2E1) * t79 - (t769 / 0.2E1 
     #- t1667 / 0.2E1) * t79) * t79) * t47 / 0.2E1) / 0.6E1 - t621 * (t7
     #82 / 0.2E1 + (t780 - (t778 - t2667) * t47) * t47 / 0.2E1) / 0.6E1 
     #+ t1325 + t1332 - t621 * ((t1152 * (t794 - (t575 / 0.2E1 - t2670 /
     # 0.2E1) * t47) * t47 - t3031) * t79 / 0.2E1 + (t3031 - t1171 * (t8
     #18 - (t588 / 0.2E1 - t2683 / 0.2E1) * t47) * t47) * t79 / 0.2E1) /
     # 0.6E1 - t698 * (((t1627 - t1324) * t79 - t3050) * t79 / 0.2E1 + (
     #t3050 - (t1331 - t1694) * t79) * t79 / 0.2E1) / 0.6E1 + (t3078 * t
     #563 - t3090 * t566) * t79 - t698 * ((t1292 * t2595 - t1300 * t2599
     #) * t79 + ((t1638 - t1336) * t79 - (t1336 - t1705) * t79) * t79) /
     # 0.24E2
        t3108 = t1238 * t3107
        t3128 = ut(t640,t699,n)
        t3130 = (t3128 - t1064) * t79
        t3134 = ut(t640,t706,n)
        t3136 = (t1067 - t3134) * t79
        t3171 = t271 * (t1101 - (t328 / 0.2E1 - t2724 / 0.2E1) * t47) * 
     #t47
        t3188 = (t1036 - t3128) * t47
        t3194 = (t1504 * (t1129 / 0.2E1 + t3188 / 0.2E1) - t1255) * t79
        t3198 = (t1261 - t1280) * t79
        t3202 = (t1042 - t3134) * t47
        t3208 = (t1278 - t1571 * (t1145 / 0.2E1 + t3202 / 0.2E1)) * t79
        t3227 = (t1038 * t1635 - t1293) * t79
        t3232 = (-t1044 * t1702 + t1301) * t79
        t3240 = (-t2976 * t983 + t971) * t47 - t621 * ((t988 - t687 * (t
     #985 - (t983 - t2724) * t47) * t47) * t47 + (t996 - (t994 - t2727) 
     #* t47) * t47) / 0.24E2 + t348 + t1239 - t698 * (t1053 / 0.2E1 + (t
     #1051 - t701 * ((t3130 / 0.2E1 - t1069 / 0.2E1) * t79 - (t1066 / 0.
     #2E1 - t3136 / 0.2E1) * t79) * t79) * t47 / 0.2E1) / 0.6E1 - t621 *
     # (t1079 / 0.2E1 + (t1077 - (t1075 - t2739) * t47) * t47 / 0.2E1) /
     # 0.6E1 + t1262 + t1281 - t621 * ((t1152 * (t1091 - (t362 / 0.2E1 -
     # t2742 / 0.2E1) * t47) * t47 - t3171) * t79 / 0.2E1 + (t3171 - t11
     #71 * (t1115 - (t387 / 0.2E1 - t2755 / 0.2E1) * t47) * t47) * t79 /
     # 0.2E1) / 0.6E1 - t698 * (((t3194 - t1261) * t79 - t3198) * t79 / 
     #0.2E1 + (t3198 - (t1280 - t3208) * t79) * t79 / 0.2E1) / 0.6E1 + (
     #t3078 * t338 - t3090 * t341) * t79 - t698 * ((t1292 * t2408 - t130
     #0 * t2412) * t79 + ((t3227 - t1303) * t79 - (t1303 - t3232) * t79)
     # * t79) / 0.24E2
        t3241 = t1238 * t3240
        t3245 = t969 * t3241 / 0.4E1
        t3248 = t662 * t614 * t2624 - t2628 + t2721 + t2775 - t9 * t2773
     # / 0.8E1 - t426 * t2779 / 0.24E2 - t426 * t2719 / 0.4E1 + t2786 - 
     #t614 * t2784 / 0.24E2 + t2965 - t2233 * t3108 / 0.2E1 - t2226 * t3
     #241 / 0.4E1 + t3245 - t966 - t1196 - t2230 * t2963 / 0.12E2
        t3250 = t618 * t3108 / 0.2E1
        t3252 = t617 * t2779 / 0.24E2
        t3255 = t10 * t1360 * t47
        t3259 = t1304 * t317
        t3260 = t1206 - t3259
        t3262 = t1204 * t3260 * t47
        t3267 = t325 * t967 * t3255 / 0.2E1
        t3270 = t325 * t1220 * t3262 / 0.6E1
        t3271 = t3250 + t3252 + t325 * t8 * t3255 / 0.2E1 + t325 * t1202
     # * t3262 / 0.6E1 - t3267 - t3270 - t1312 + t1344 - t1349 + t1351 +
     # t1353 - t1750 + t2235 + t2237 + t2239 - t2246
        t3273 = (t3248 + t3271) * t5
        t3279 = t662 * (t557 - dx * t678 / 0.24E2)
        t3282 = sqrt(t2641)
        t3290 = (t2306 - (t2304 - (-cc * t2638 * t2722 * t3282 + t2302) 
     #* t47) * t47) * t47
        t3297 = dx * (t2298 + t2304 / 0.2E1 - t621 * (t2308 / 0.2E1 + t3
     #290 / 0.2E1) / 0.6E1) / 0.4E1
        t3303 = t621 * (t2306 - dx * (t2308 - t3290) / 0.12E2) / 0.24E2
        t3306 = dx * t691 / 0.24E2
        t3307 = t2284 / 0.2E1
        t3308 = -t3273 * t608 - t2315 + t2321 + t2326 + t2628 - t2721 - 
     #t2775 + t3279 - t3297 - t3303 - t3306 - t3307
        t3309 = -t2786 - t2965 - t3245 + t966 + t1196 - t3250 - t3252 + 
     #t3267 + t3270 - t1351 - t1353 + t1750 + t2246
        t3324 = t20 * (t2342 + t2350 / 0.2E1 - t621 * (t2354 / 0.2E1 + (
     #t2352 - (-t648 * t766 + t2350) * t47) * t47 / 0.2E1) / 0.8E1)
        t3333 = (t1066 - t1069) * t79
        t3352 = t2367 + t2368 - t2372 + t338 / 0.4E1 + t341 / 0.4E1 - t2
     #415 / 0.12E2 - t621 * (t2420 / 0.2E1 + (t2418 - (t2401 + t2402 - t
     #2416 - t1066 / 0.2E1 - t1069 / 0.2E1 + t698 * (((t3130 - t1066) * 
     #t79 - t3333) * t79 / 0.2E1 + (t3333 - (t1069 - t3136) * t79) * t79
     # / 0.2E1) / 0.6E1) * t47) * t47 / 0.2E1) / 0.8E1
        t3357 = t20 * (t2341 / 0.2E1 + t2350 / 0.2E1)
        t3359 = t1546 / 0.4E1 + t1568 / 0.4E1 + t1642 / 0.4E1 + t1709 / 
     #0.4E1
        t3365 = (-t1088 * t1590 + t2492) * t47
        t3371 = (t2498 - t1483 * (t3130 / 0.2E1 + t1066 / 0.2E1)) * t47
        t3375 = (t3365 + t2501 + t3371 / 0.2E1 + t3194 / 0.2E1 + t1262 +
     # t3227) * t1246
        t3380 = (-t1112 * t1657 + t2507) * t47
        t3386 = (t2513 - t1551 * (t1069 / 0.2E1 + t3136 / 0.2E1)) * t47
        t3390 = (t3380 + t2516 + t3386 / 0.2E1 + t1281 + t3208 / 0.2E1 +
     # t3232) * t1269
        t3394 = t2506 / 0.4E1 + t2521 / 0.4E1 + (t3375 - t3259) * t79 / 
     #0.4E1 + (t3259 - t3390) * t79 / 0.4E1
        t3400 = dx * (t234 / 0.2E1 - t1075 / 0.2E1)
        t3404 = t3324 * t609 * t3352
        t3407 = t3357 * t2534 * t3359 / 0.2E1
        t3410 = t3357 * t2538 * t3394 / 0.6E1
        t3412 = t609 * t3400 / 0.24E2
        t3414 = (t3324 * t614 * t3352 + t3357 * t2431 * t3359 / 0.2E1 + 
     #t3357 * t2437 * t3394 / 0.6E1 - t614 * t3400 / 0.24E2 - t3404 - t3
     #407 - t3410 + t3412) * t5
        t3425 = (t769 - t772) * t79
        t3445 = t3324 * (t2554 + t2555 - t2559 + t563 / 0.4E1 + t566 / 0
     #.4E1 - t2602 / 0.12E2 - t621 * (t2607 / 0.2E1 + (t2605 - (t2588 + 
     #t2589 - t2603 - t769 / 0.2E1 - t772 / 0.2E1 + t698 * (((t1600 - t7
     #69) * t79 - t3425) * t79 / 0.2E1 + (t3425 - (t772 - t1667) * t79) 
     #* t79 / 0.2E1) / 0.6E1) * t47) * t47 / 0.2E1) / 0.8E1)
        t3449 = dx * (t523 / 0.2E1 - t778 / 0.2E1) / 0.24E2
        t3457 = t356 * t360
        t3463 = (t3457 - t2341) * t79
        t3466 = t381 * t385
        t3468 = (t2341 - t3466) * t79
        t3470 = (t3463 - t3468) * t79
        t3476 = t20 * (t3457 / 0.2E1 + t2342 - t698 * (((t836 * t840 - t
     #3457) * t79 - t3463) * t79 / 0.2E1 + t3470 / 0.2E1) / 0.8E1)
        t3482 = (t249 - t362) * t47
        t3484 = ((t138 - t249) * t47 - t3482) * t47
        t3488 = (t3482 - (t362 - t1088) * t47) * t47
        t3491 = t621 * (t3484 / 0.2E1 + t3488 / 0.2E1)
        t3493 = t215 / 0.4E1
        t3494 = t328 / 0.4E1
        t3497 = t621 * (t979 / 0.2E1 + t987 / 0.2E1)
        t3498 = t3497 / 0.12E2
        t3504 = (t1127 - t1129) * t47
        t3515 = t249 / 0.2E1
        t3516 = t362 / 0.2E1
        t3517 = t3491 / 0.6E1
        t3520 = t215 / 0.2E1
        t3521 = t328 / 0.2E1
        t3522 = t3497 / 0.6E1
        t3524 = (t3515 + t3516 - t3517 - t3520 - t3521 + t3522) * t79
        t3527 = t274 / 0.2E1
        t3528 = t387 / 0.2E1
        t3532 = (t274 - t387) * t47
        t3534 = ((t165 - t274) * t47 - t3532) * t47
        t3538 = (t3532 - (t387 - t1112) * t47) * t47
        t3541 = t621 * (t3534 / 0.2E1 + t3538 / 0.2E1)
        t3542 = t3541 / 0.6E1
        t3544 = (t3520 + t3521 - t3522 - t3527 - t3528 + t3542) * t79
        t3546 = (t3524 - t3544) * t79
        t3551 = t249 / 0.4E1 + t362 / 0.4E1 - t3491 / 0.12E2 + t3493 + t
     #3494 - t3498 - t698 * (((t1127 / 0.2E1 + t1129 / 0.2E1 - t621 * ((
     #(t1983 - t1127) * t47 - t3504) * t47 / 0.2E1 + (t3504 - (t1129 - t
     #3188) * t47) * t47 / 0.2E1) / 0.6E1 - t3515 - t3516 + t3517) * t79
     # - t3524) * t79 / 0.2E1 + t3546 / 0.2E1) / 0.8E1
        t3556 = t20 * (t3457 / 0.2E1 + t2341 / 0.2E1)
        t3558 = t1718 / 0.4E1 + t1720 / 0.4E1 + t1357 / 0.4E1 + t1361 / 
     #0.4E1
        t3566 = t1207 * t47
        t3567 = t3260 * t47
        t3569 = (t2462 - t2504) * t47 / 0.4E1 + (t2504 - t3375) * t47 / 
     #0.4E1 + t3566 / 0.4E1 + t3567 / 0.4E1
        t3575 = dy * (t1135 / 0.2E1 - t393 / 0.2E1)
        t3579 = t3476 * t609 * t3551
        t3582 = t3556 * t2534 * t3558 / 0.2E1
        t3585 = t3556 * t2538 * t3569 / 0.6E1
        t3587 = t609 * t3575 / 0.24E2
        t3589 = (t3476 * t614 * t3551 + t3556 * t2431 * t3558 / 0.2E1 + 
     #t3556 * t2437 * t3569 / 0.6E1 - t614 * t3575 / 0.24E2 - t3579 - t3
     #582 - t3585 + t3587) * t5
        t3597 = (t526 - t575) * t47
        t3599 = ((t478 - t526) * t47 - t3597) * t47
        t3603 = (t3597 - (t575 - t791) * t47) * t47
        t3606 = t621 * (t3599 / 0.2E1 + t3603 / 0.2E1)
        t3608 = t508 / 0.4E1
        t3609 = t557 / 0.4E1
        t3612 = t621 * (t671 / 0.2E1 + t679 / 0.2E1)
        t3613 = t3612 / 0.12E2
        t3619 = (t842 - t844) * t47
        t3630 = t526 / 0.2E1
        t3631 = t575 / 0.2E1
        t3632 = t3606 / 0.6E1
        t3635 = t508 / 0.2E1
        t3636 = t557 / 0.2E1
        t3637 = t3612 / 0.6E1
        t3639 = (t3630 + t3631 - t3632 - t3635 - t3636 + t3637) * t79
        t3642 = t539 / 0.2E1
        t3643 = t588 / 0.2E1
        t3647 = (t539 - t588) * t47
        t3649 = ((t493 - t539) * t47 - t3647) * t47
        t3653 = (t3647 - (t588 - t815) * t47) * t47
        t3656 = t621 * (t3649 / 0.2E1 + t3653 / 0.2E1)
        t3657 = t3656 / 0.6E1
        t3659 = (t3635 + t3636 - t3637 - t3642 - t3643 + t3657) * t79
        t3661 = (t3639 - t3659) * t79
        t3667 = t3476 * (t526 / 0.4E1 + t575 / 0.4E1 - t3606 / 0.12E2 + 
     #t3608 + t3609 - t3613 - t698 * (((t842 / 0.2E1 + t844 / 0.2E1 - t6
     #21 * (((t1421 - t842) * t47 - t3619) * t47 / 0.2E1 + (t3619 - (t84
     #4 - t1621) * t47) * t47 / 0.2E1) / 0.6E1 - t3630 - t3631 + t3632) 
     #* t79 - t3639) * t79 / 0.2E1 + t3661 / 0.2E1) / 0.8E1)
        t3671 = dy * (t850 / 0.2E1 - t594 / 0.2E1) / 0.24E2
        t3676 = dt * dy
        t3677 = sqrt(t397)
        t3678 = cc * t3677
        t3679 = t3678 * t1543
        t3680 = sqrt(t401)
        t3681 = cc * t3680
        t3682 = t3681 * t600
        t3684 = (t3679 - t3682) * t79
        t3685 = sqrt(t409)
        t3686 = cc * t3685
        t3687 = t3686 * t1565
        t3689 = (t3682 - t3687) * t79
        t3692 = t3676 * (t3684 / 0.2E1 + t3689 / 0.2E1)
        t3694 = t426 * t3692 / 0.4E1
        t3695 = t10 * dy
        t3696 = t3678 * t2503
        t3697 = t3681 * t417
        t3699 = (t3696 - t3697) * t79
        t3700 = t3686 * t2518
        t3702 = (t3697 - t3700) * t79
        t3705 = t3695 * (t3699 / 0.2E1 + t3702 / 0.2E1)
        t3707 = t9 * t3705 / 0.8E1
        t3709 = t968 * t3705 / 0.8E1
        t3711 = t3676 * (t3684 - t3689)
        t3713 = t617 * t3711 / 0.24E2
        t3714 = sqrt(t891)
        t3715 = cc * t3714
        t3716 = t1408 ** 2
        t3717 = t1411 ** 2
        t3719 = t1415 * (t3716 + t3717)
        t3720 = t829 ** 2
        t3721 = t832 ** 2
        t3723 = t836 * (t3720 + t3721)
        t3726 = t20 * (t3719 / 0.2E1 + t3723 / 0.2E1)
        t3728 = t1608 ** 2
        t3729 = t1611 ** 2
        t3731 = t1615 * (t3728 + t3729)
        t3734 = t20 * (t3723 / 0.2E1 + t3731 / 0.2E1)
        t3738 = j + 3
        t3739 = ut(t50,t3738,n)
        t3741 = (t3739 - t1002) * t79
        t3746 = ut(i,t3738,n)
        t3748 = (t3746 - t1018) * t79
        t3752 = t759 * (t3748 / 0.2E1 + t1020 / 0.2E1)
        t3756 = ut(t310,t3738,n)
        t3758 = (t3756 - t1036) * t79
        t3766 = rx(i,t3738,0,0)
        t3767 = rx(i,t3738,1,1)
        t3769 = rx(i,t3738,0,1)
        t3770 = rx(i,t3738,1,0)
        t3773 = 0.1E1 / (t3766 * t3767 - t3769 * t3770)
        t3535 = t20 * t3773 * (t3766 * t3770 + t3767 * t3769)
        t3787 = (t3535 * ((t3739 - t3746) * t47 / 0.2E1 + (t3746 - t3756
     #) * t47 / 0.2E1) - t1133) * t79
        t3789 = t3770 ** 2
        t3790 = t3767 ** 2
        t3791 = t3789 + t3790
        t3792 = t3773 * t3791
        t3795 = t20 * (t3792 / 0.2E1 + t892 / 0.2E1)
        t3798 = (t3748 * t3795 - t1178) * t79
        t3805 = t3695 * ((t3715 * ((t1127 * t3726 - t1129 * t3734) * t47
     # + (t1323 * (t3741 / 0.2E1 + t1004 / 0.2E1) - t3752) * t47 / 0.2E1
     # + (t3752 - t1504 * (t3758 / 0.2E1 + t1038 / 0.2E1)) * t47 / 0.2E1
     # + t3787 / 0.2E1 + t2502 + t3798) - t3696) * t79 / 0.2E1 + t3699 /
     # 0.2E1)
        t3808 = t3726 * t842
        t3809 = t3734 * t844
        t3812 = u(t50,t3738,n)
        t3814 = (t3812 - t700) * t79
        t3818 = t1323 * (t3814 / 0.2E1 + t702 / 0.2E1)
        t3819 = u(i,t3738,n)
        t3821 = (t3819 - t717) * t79
        t3825 = t759 * (t3821 / 0.2E1 + t719 / 0.2E1)
        t3828 = (t3818 - t3825) * t47 / 0.2E1
        t3829 = u(t310,t3738,n)
        t3831 = (t3829 - t735) * t79
        t3835 = t1504 * (t3831 / 0.2E1 + t737 / 0.2E1)
        t3838 = (t3825 - t3835) * t47 / 0.2E1
        t3840 = (t3812 - t3819) * t47
        t3842 = (t3819 - t3829) * t47
        t3848 = (t3535 * (t3840 / 0.2E1 + t3842 / 0.2E1) - t848) * t79
        t3852 = (t3795 * t3821 - t945) * t79
        t3853 = (t3808 - t3809) * t47 + t3828 + t3838 + t3848 / 0.2E1 + 
     #t1542 + t3852
        t3856 = (t3715 * t3853 - t3679) * t79
        t3858 = t3676 * (t3856 - t3684)
        t3862 = t426 * t3711 / 0.24E2
        t3865 = t3676 * (t3856 / 0.2E1 + t3684 / 0.2E1)
        t3867 = t617 * t3865 / 0.4E1
        t3869 = t968 * t3805 / 0.8E1
        t3874 = t225 - dy * t1168 / 0.24E2
        t3878 = t908 * t609 * t3874
        t3880 = t617 * t3692 / 0.4E1
        t3882 = t617 * t3858 / 0.24E2
        t3884 = t1380 / 0.2E1
        t3888 = (t1372 - t1380) * t47
        t3892 = (t1380 - t1528) * t47
        t3894 = (t3888 - t3892) * t47
        t3900 = t20 * (t1372 / 0.2E1 + t3884 - t621 * (((t1368 - t1372) 
     #* t47 - t3888) * t47 / 0.2E1 + t3894 / 0.2E1) / 0.8E1)
        t3912 = t20 * (t3884 + t1528 / 0.2E1 - t621 * (t3894 / 0.2E1 + (
     #t3892 - (t1528 - t1587) * t47) * t47 / 0.2E1) / 0.8E1)
        t3942 = t291 * ((t3748 / 0.2E1 - t225 / 0.2E1) * t79 - t1023) * 
     #t79
        t3961 = (t2458 - t2500) * t47
        t4006 = t20 * (t892 / 0.2E1 + t887 - t698 * (((t3792 - t892) * t
     #79 - t894) * t79 / 0.2E1 + t898 / 0.2E1) / 0.8E1)
        t4024 = (t249 * t3900 - t362 * t3912) * t47 - t621 * ((t1383 * t
     #3484 - t1531 * t3488) * t47 + ((t2441 - t2494) * t47 - (t2494 - t3
     #365) * t47) * t47) / 0.24E2 + t2459 + t2501 - t698 * ((t193 * ((t3
     #741 / 0.2E1 - t112 / 0.2E1) * t79 - t1007) * t79 - t3942) * t47 / 
     #0.2E1 + (t3942 - t1152 * ((t3758 / 0.2E1 - t338 / 0.2E1) * t79 - t
     #1041) * t79) * t47 / 0.2E1) / 0.6E1 - t621 * (((t2451 - t2458) * t
     #47 - t3961) * t47 / 0.2E1 + (t3961 - (t2500 - t3371) * t47) * t47 
     #/ 0.2E1) / 0.6E1 + t2502 + t373 - t621 * ((t759 * ((t1983 / 0.2E1 
     #- t1129 / 0.2E1) * t47 - (t1127 / 0.2E1 - t3188 / 0.2E1) * t47) * 
     #t47 - t1095) * t79 / 0.2E1 + t1107 / 0.2E1) / 0.6E1 - t698 * (((t3
     #787 - t1135) * t79 - t1137) * t79 / 0.2E1 + t1141 / 0.2E1) / 0.6E1
     # + (t1020 * t4006 - t1160) * t79 - t698 * ((t944 * ((t3748 - t1020
     #) * t79 - t1165) * t79 - t1170) * t79 + ((t3798 - t1180) * t79 - t
     #1182) * t79) / 0.24E2
        t4025 = t3678 * t4024
        t4028 = t3681 * t963
        t4030 = t2233 * t4028 / 0.2E1
        t4031 = -t3694 - t3707 + t3709 + t3713 - t9 * t3805 / 0.8E1 + t4
     #26 * t3858 / 0.24E2 - t3862 + t3867 + t3869 - t426 * t3865 / 0.4E1
     # + t908 * t614 * t3874 - t3878 + t3880 - t3882 + t2226 * t4025 / 0
     #.4E1 - t4030
        t4062 = t291 * ((t3821 / 0.2E1 - t514 / 0.2E1) * t79 - t722) * t
     #79
        t4081 = (t1406 - t1540) * t47
        t4133 = (t3900 * t526 - t3912 * t575) * t47 - t621 * ((t1383 * t
     #3599 - t1531 * t3603) * t47 + ((t1386 - t1534) * t47 - (t1534 - t1
     #593) * t47) * t47) / 0.24E2 + t1407 + t1541 - t698 * ((t193 * ((t3
     #814 / 0.2E1 - t464 / 0.2E1) * t79 - t705) * t79 - t4062) * t47 / 0
     #.2E1 + (t4062 - t1152 * ((t3831 / 0.2E1 - t563 / 0.2E1) * t79 - t7
     #40) * t79) * t47 / 0.2E1) / 0.6E1 - t621 * (((t1399 - t1406) * t47
     # - t4081) * t47 / 0.2E1 + (t4081 - (t1540 - t1606) * t47) * t47 / 
     #0.2E1) / 0.6E1 + t1542 + t586 - t621 * ((t759 * ((t1421 / 0.2E1 - 
     #t844 / 0.2E1) * t47 - (t842 / 0.2E1 - t1621 / 0.2E1) * t47) * t47 
     #- t798) * t79 / 0.2E1 + t810 / 0.2E1) / 0.6E1 - t698 * (((t3848 - 
     #t850) * t79 - t852) * t79 / 0.2E1 + t856 / 0.2E1) / 0.6E1 + (t4006
     # * t719 - t909) * t79 - t698 * ((t944 * ((t3821 - t719) * t79 - t9
     #29) * t79 - t934) * t79 + ((t3852 - t947) * t79 - t949) * t79) / 0
     #.24E2
        t4134 = t3678 * t4133
        t4141 = t2087 ** 2
        t4142 = t2090 ** 2
        t4151 = u(t12,t3738,n)
        t4161 = rx(t50,t3738,0,0)
        t4162 = rx(t50,t3738,1,1)
        t4164 = rx(t50,t3738,0,1)
        t4165 = rx(t50,t3738,1,0)
        t4168 = 0.1E1 / (t4161 * t4162 - t4164 * t4165)
        t4182 = t4165 ** 2
        t4183 = t4162 ** 2
        t4193 = ((t20 * (t2094 * (t4141 + t4142) / 0.2E1 + t3719 / 0.2E1
     #) * t1421 - t3808) * t47 + (t2010 * ((t4151 - t1387) * t79 / 0.2E1
     # + t1389 / 0.2E1) - t3818) * t47 / 0.2E1 + t3828 + (t20 * t4168 * 
     #(t4161 * t4165 + t4162 * t4164) * ((t4151 - t3812) * t47 / 0.2E1 +
     # t3840 / 0.2E1) - t1425) * t79 / 0.2E1 + t1428 + (t20 * (t4168 * (
     #t4182 + t4183) / 0.2E1 + t1432 / 0.2E1) * t3814 - t1436) * t79) * 
     #t1414
        t4200 = t3853 * t835
        t4202 = (t4200 - t1544) * t79
        t4206 = t291 * (t4202 / 0.2E1 + t1546 / 0.2E1)
        t4210 = t2827 ** 2
        t4211 = t2830 ** 2
        t4220 = u(t640,t3738,n)
        t4230 = rx(t310,t3738,0,0)
        t4231 = rx(t310,t3738,1,1)
        t4233 = rx(t310,t3738,0,1)
        t4234 = rx(t310,t3738,1,0)
        t4237 = 0.1E1 / (t4230 * t4231 - t4233 * t4234)
        t4251 = t4234 ** 2
        t4252 = t4231 ** 2
        t4262 = ((t3809 - t20 * (t3731 / 0.2E1 + t2834 * (t4210 + t4211)
     # / 0.2E1) * t1621) * t47 + t3838 + (t3835 - t2685 * ((t4220 - t159
     #8) * t79 / 0.2E1 + t1600 / 0.2E1)) * t47 / 0.2E1 + (t20 * t4237 * 
     #(t4230 * t4234 + t4231 * t4233) * (t3842 / 0.2E1 + (t3829 - t4220)
     # * t47 / 0.2E1) - t1625) * t79 / 0.2E1 + t1628 + (t20 * (t4237 * (
     #t4251 + t4252) / 0.2E1 + t1632 / 0.2E1) * t3831 - t1636) * t79) * 
     #t1614
        t4287 = t3678 * ((t1383 * t1718 - t1531 * t1720) * t47 + (t193 *
     # ((t4193 - t1440) * t79 / 0.2E1 + t1442 / 0.2E1) - t4206) * t47 / 
     #0.2E1 + (t4206 - t1152 * ((t4262 - t1640) * t79 / 0.2E1 + t1642 / 
     #0.2E1)) * t47 / 0.2E1 + (t759 * ((t4193 - t4200) * t47 / 0.2E1 + (
     #t4200 - t4262) * t47 / 0.2E1) - t1724) * t79 / 0.2E1 + t1731 + (t4
     #202 * t944 - t1743) * t79)
        t4290 = t3681 * t1747
        t4292 = t1356 * t4290 / 0.12E2
        t4294 = t1356 * t4287 / 0.12E2
        t4296 = t618 * t4028 / 0.2E1
        t4297 = t3681 * t1193
        t4299 = t969 * t4297 / 0.4E1
        t4301 = t2230 * t4290 / 0.12E2
        t4303 = t618 * t4134 / 0.2E1
        t4305 = t969 * t4025 / 0.4E1
        t4307 = t2226 * t4297 / 0.4E1
        t4310 = t10 * t1545 * t79
        t4312 = t405 * t967 * t4310 / 0.2E1
        t4315 = t1204 * t2505 * t79
        t4317 = t405 * t1220 * t4315 / 0.6E1
        t4324 = dy * t1181
        t4328 = t609 * t4324 / 0.24E2
        t4329 = t2233 * t4134 / 0.2E1 + t2230 * t4287 / 0.12E2 + t4292 -
     # t4294 + t4296 + t4299 - t4301 - t4303 - t4305 - t4307 - t4312 - t
     #4317 + t405 * t8 * t4310 / 0.2E1 + t405 * t1202 * t4315 / 0.6E1 - 
     #t614 * t4324 / 0.24E2 + t4328
        t4331 = (t4031 + t4329) * t5
        t4337 = cc * t356 * t3677 * t223
        t4340 = cc * t836 * t3714 * t1018
        t4342 = (-t4337 + t4340) * t79
        t4344 = t2264 * t3680 * t2
        t4346 = (t4337 - t4344) * t79
        t4348 = (t4342 - t4346) * t79
        t4350 = sqrt(t3791)
        t4358 = (((cc * t3746 * t3773 * t4350 - t4340) * t79 - t4342) * 
     #t79 - t4348) * t79
        t4361 = cc * t381 * t3685 * t226
        t4363 = (t4344 - t4361) * t79
        t4365 = (t4346 - t4363) * t79
        t4367 = (t4348 - t4365) * t79
        t4373 = t698 * (t4348 - dy * (t4358 - t4367) / 0.12E2) / 0.24E2
        t4375 = t4346 / 0.2E1
        t4382 = dy * (t4342 / 0.2E1 + t4375 - t698 * (t4358 / 0.2E1 + t4
     #367 / 0.2E1) / 0.6E1) / 0.4E1
        t4383 = t4363 / 0.2E1
        t4385 = sqrt(t913)
        t4387 = cc * t864 * t4385 * t1024
        t4389 = (-t4387 + t4361) * t79
        t4391 = (t4363 - t4389) * t79
        t4393 = (t4365 - t4391) * t79
        t4400 = dy * (t4375 + t4383 - t698 * (t4367 / 0.2E1 + t4393 / 0.
     #2E1) / 0.6E1) / 0.4E1
        t4402 = dy * t948 / 0.24E2
        t4403 = -t4331 * t608 - t3709 - t3713 - t3867 - t3869 + t3878 - 
     #t3880 + t3882 + t4373 - t4382 - t4400 - t4402
        t4409 = t698 * (t4365 - dy * (t4367 - t4393) / 0.12E2) / 0.24E2
        t4413 = t908 * (t514 - dy * t932 / 0.24E2)
        t4414 = t4344 / 0.2E1
        t4415 = t4337 / 0.2E1
        t4416 = -t4409 - t4292 + t4294 - t4296 - t4299 + t4303 + t4305 +
     # t4413 - t4414 + t4415 + t4312 + t4317 - t4328
        t4431 = t20 * (t2342 + t3466 / 0.2E1 - t698 * (t3470 / 0.2E1 + (
     #t3468 - (-t864 * t868 + t3466) * t79) * t79 / 0.2E1) / 0.8E1)
        t4440 = (t1143 - t1145) * t47
        t4459 = t3493 + t3494 - t3498 + t274 / 0.4E1 + t387 / 0.4E1 - t3
     #541 / 0.12E2 - t698 * (t3546 / 0.2E1 + (t3544 - (t3527 + t3528 - t
     #3542 - t1143 / 0.2E1 - t1145 / 0.2E1 + t621 * (((t1997 - t1143) * 
     #t47 - t4440) * t47 / 0.2E1 + (t4440 - (t1145 - t3202) * t47) * t47
     # / 0.2E1) / 0.6E1) * t79) * t79 / 0.2E1) / 0.8E1
        t4464 = t20 * (t2341 / 0.2E1 + t3466 / 0.2E1)
        t4466 = t1357 / 0.4E1 + t1361 / 0.4E1 + t1733 / 0.4E1 + t1735 / 
     #0.4E1
        t4475 = t3566 / 0.4E1 + t3567 / 0.4E1 + (t2489 - t2519) * t47 / 
     #0.4E1 + (t2519 - t3390) * t47 / 0.4E1
        t4481 = dy * (t372 / 0.2E1 - t1151 / 0.2E1)
        t4485 = t4431 * t609 * t4459
        t4488 = t4464 * t2534 * t4466 / 0.2E1
        t4491 = t4464 * t2538 * t4475 / 0.6E1
        t4493 = t609 * t4481 / 0.24E2
        t4495 = (t4431 * t614 * t4459 + t4464 * t2431 * t4466 / 0.2E1 + 
     #t4464 * t2437 * t4475 / 0.6E1 - t614 * t4481 / 0.24E2 - t4485 - t4
     #488 - t4491 + t4493) * t5
        t4506 = (t870 - t872) * t47
        t4526 = t4431 * (t3608 + t3609 - t3613 + t539 / 0.4E1 + t588 / 0
     #.4E1 - t3656 / 0.12E2 - t698 * (t3661 / 0.2E1 + (t3659 - (t3642 + 
     #t3643 - t3657 - t870 / 0.2E1 - t872 / 0.2E1 + t621 * (((t1499 - t8
     #70) * t47 - t4506) * t47 / 0.2E1 + (t4506 - (t872 - t1688) * t47) 
     #* t47 / 0.2E1) / 0.6E1) * t79) * t79 / 0.2E1) / 0.8E1)
        t4530 = dy * (t585 / 0.2E1 - t878 / 0.2E1) / 0.24E2
        t4535 = cc * t4385
        t4536 = t1486 ** 2
        t4537 = t1489 ** 2
        t4539 = t1493 * (t4536 + t4537)
        t4540 = t857 ** 2
        t4541 = t860 ** 2
        t4543 = t864 * (t4540 + t4541)
        t4546 = t20 * (t4539 / 0.2E1 + t4543 / 0.2E1)
        t4548 = t1675 ** 2
        t4549 = t1678 ** 2
        t4551 = t1682 * (t4548 + t4549)
        t4554 = t20 * (t4543 / 0.2E1 + t4551 / 0.2E1)
        t4558 = j - 3
        t4559 = ut(t50,t4558,n)
        t4561 = (t1008 - t4559) * t79
        t4566 = ut(i,t4558,n)
        t4568 = (t1024 - t4566) * t79
        t4572 = t784 * (t1026 / 0.2E1 + t4568 / 0.2E1)
        t4576 = ut(t310,t4558,n)
        t4578 = (t1042 - t4576) * t79
        t4586 = rx(i,t4558,0,0)
        t4587 = rx(i,t4558,1,1)
        t4589 = rx(i,t4558,0,1)
        t4590 = rx(i,t4558,1,0)
        t4593 = 0.1E1 / (t4586 * t4587 - t4589 * t4590)
        t4356 = t20 * t4593 * (t4586 * t4590 + t4587 * t4589)
        t4607 = (t1149 - t4356 * ((t4559 - t4566) * t47 / 0.2E1 + (t4566
     # - t4576) * t47 / 0.2E1)) * t79
        t4609 = t4590 ** 2
        t4610 = t4587 ** 2
        t4611 = t4609 + t4610
        t4612 = t4593 * t4611
        t4615 = t20 * (t914 / 0.2E1 + t4612 / 0.2E1)
        t4618 = (-t4568 * t4615 + t1183) * t79
        t4625 = t3695 * (t3702 / 0.2E1 + (t3700 - t4535 * ((t1143 * t454
     #6 - t1145 * t4554) * t47 + (t1398 * (t1010 / 0.2E1 + t4561 / 0.2E1
     #) - t4572) * t47 / 0.2E1 + (t4572 - t1571 * (t1044 / 0.2E1 + t4578
     # / 0.2E1)) * t47 / 0.2E1 + t2517 + t4607 / 0.2E1 + t4618)) * t79 /
     # 0.2E1)
        t4628 = t4546 * t870
        t4629 = t4554 * t872
        t4632 = u(t50,t4558,n)
        t4634 = (t707 - t4632) * t79
        t4638 = t1398 * (t709 / 0.2E1 + t4634 / 0.2E1)
        t4639 = u(i,t4558,n)
        t4641 = (t723 - t4639) * t79
        t4645 = t784 * (t725 / 0.2E1 + t4641 / 0.2E1)
        t4648 = (t4638 - t4645) * t47 / 0.2E1
        t4649 = u(t310,t4558,n)
        t4651 = (t741 - t4649) * t79
        t4655 = t1571 * (t743 / 0.2E1 + t4651 / 0.2E1)
        t4658 = (t4645 - t4655) * t47 / 0.2E1
        t4660 = (t4632 - t4639) * t47
        t4662 = (t4639 - t4649) * t47
        t4668 = (t876 - t4356 * (t4660 / 0.2E1 + t4662 / 0.2E1)) * t79
        t4672 = (-t4615 * t4641 + t953) * t79
        t4673 = (t4628 - t4629) * t47 + t4648 + t4658 + t1564 + t4668 / 
     #0.2E1 + t4672
        t4676 = (-t4535 * t4673 + t3687) * t79
        t4678 = t3676 * (t3689 - t4676)
        t4680 = t617 * t4678 / 0.24E2
        t4683 = t3676 * (t3689 / 0.2E1 + t4676 / 0.2E1)
        t4687 = t617 * t4683 / 0.4E1
        t4689 = t968 * t4625 / 0.8E1
        t4692 = dy * t1186
        t4694 = t609 * t4692 / 0.24E2
        t4701 = t2154 ** 2
        t4702 = t2157 ** 2
        t4711 = u(t12,t4558,n)
        t4721 = rx(t50,t4558,0,0)
        t4722 = rx(t50,t4558,1,1)
        t4724 = rx(t50,t4558,0,1)
        t4725 = rx(t50,t4558,1,0)
        t4728 = 0.1E1 / (t4721 * t4722 - t4724 * t4725)
        t4742 = t4725 ** 2
        t4743 = t4722 ** 2
        t4753 = ((t20 * (t2161 * (t4701 + t4702) / 0.2E1 + t4539 / 0.2E1
     #) * t1499 - t4628) * t47 + (t2074 * (t1467 / 0.2E1 + (t1465 - t471
     #1) * t79 / 0.2E1) - t4638) * t47 / 0.2E1 + t4648 + t1506 + (t1503 
     #- t20 * t4728 * (t4721 * t4725 + t4722 * t4724) * ((t4711 - t4632)
     # * t47 / 0.2E1 + t4660 / 0.2E1)) * t79 / 0.2E1 + (t1514 - t20 * (t
     #1510 / 0.2E1 + t4728 * (t4742 + t4743) / 0.2E1) * t4634) * t79) * 
     #t1492
        t4760 = t4673 * t863
        t4762 = (t1566 - t4760) * t79
        t4766 = t327 * (t1568 / 0.2E1 + t4762 / 0.2E1)
        t4770 = t2894 ** 2
        t4771 = t2897 ** 2
        t4780 = u(t640,t4558,n)
        t4790 = rx(t310,t4558,0,0)
        t4791 = rx(t310,t4558,1,1)
        t4793 = rx(t310,t4558,0,1)
        t4794 = rx(t310,t4558,1,0)
        t4797 = 0.1E1 / (t4790 * t4791 - t4793 * t4794)
        t4811 = t4794 ** 2
        t4812 = t4791 ** 2
        t4822 = ((t4629 - t20 * (t4551 / 0.2E1 + t2901 * (t4770 + t4771)
     # / 0.2E1) * t1688) * t47 + t4658 + (t4655 - t2760 * (t1667 / 0.2E1
     # + (t1665 - t4780) * t79 / 0.2E1)) * t47 / 0.2E1 + t1695 + (t1692 
     #- t20 * t4797 * (t4790 * t4794 + t4791 * t4793) * (t4662 / 0.2E1 +
     # (t4649 - t4780) * t47 / 0.2E1)) * t79 / 0.2E1 + (t1703 - t20 * (t
     #1699 / 0.2E1 + t4797 * (t4811 + t4812) / 0.2E1) * t4651) * t79) * 
     #t1681
        t4847 = t3686 * ((t1461 * t1733 - t1553 * t1735) * t47 + (t221 *
     # (t1520 / 0.2E1 + (t1518 - t4753) * t79 / 0.2E1) - t4766) * t47 / 
     #0.2E1 + (t4766 - t1171 * (t1709 / 0.2E1 + (t1707 - t4822) * t79 / 
     #0.2E1)) * t47 / 0.2E1 + t1742 + (t1739 - t784 * ((t4753 - t4760) *
     # t47 / 0.2E1 + (t4760 - t4822) * t47 / 0.2E1)) * t79 / 0.2E1 + (-t
     #4762 * t952 + t1744) * t79)
        t4849 = t1356 * t4847 / 0.12E2
        t4852 = -t3694 - t3707 + t3709 - t3713 - t9 * t4625 / 0.8E1 + t4
     #680 - t426 * t4683 / 0.4E1 + t4687 + t4689 - t426 * t4678 / 0.24E2
     # + t4694 + t3862 - t614 * t4692 / 0.24E2 + t3880 + t4849 - t2230 *
     # t4847 / 0.12E2
        t4854 = t1458 / 0.2E1
        t4858 = (t1450 - t1458) * t47
        t4862 = (t1458 - t1550) * t47
        t4864 = (t4858 - t4862) * t47
        t4870 = t20 * (t1450 / 0.2E1 + t4854 - t621 * (((t1446 - t1450) 
     #* t47 - t4858) * t47 / 0.2E1 + t4864 / 0.2E1) / 0.8E1)
        t4882 = t20 * (t4854 + t1550 / 0.2E1 - t621 * (t4864 / 0.2E1 + (
     #t4862 - (t1550 - t1654) * t47) * t47 / 0.2E1) / 0.8E1)
        t4912 = t327 * (t728 - (t517 / 0.2E1 - t4641 / 0.2E1) * t79) * t
     #79
        t4931 = (t1484 - t1562) * t47
        t4976 = t20 * (t910 + t914 / 0.2E1 - t698 * (t918 / 0.2E1 + (t91
     #6 - (t914 - t4612) * t79) * t79 / 0.2E1) / 0.8E1)
        t4994 = (t4870 * t539 - t4882 * t588) * t47 - t621 * ((t1461 * t
     #3649 - t1553 * t3653) * t47 + ((t1464 - t1556) * t47 - (t1556 - t1
     #660) * t47) * t47) / 0.24E2 + t1485 + t1563 - t698 * ((t221 * (t71
     #2 - (t467 / 0.2E1 - t4634 / 0.2E1) * t79) * t79 - t4912) * t47 / 0
     #.2E1 + (t4912 - t1171 * (t746 - (t566 / 0.2E1 - t4651 / 0.2E1) * t
     #79) * t79) * t47 / 0.2E1) / 0.6E1 - t621 * (((t1477 - t1484) * t47
     # - t4931) * t47 / 0.2E1 + (t4931 - (t1562 - t1673) * t47) * t47 / 
     #0.2E1) / 0.6E1 + t595 + t1564 - t621 * (t824 / 0.2E1 + (t822 - t78
     #4 * ((t1499 / 0.2E1 - t872 / 0.2E1) * t47 - (t870 / 0.2E1 - t1688 
     #/ 0.2E1) * t47) * t47) * t79 / 0.2E1) / 0.6E1 - t698 * (t882 / 0.2
     #E1 + (t880 - (t878 - t4668) * t79) * t79 / 0.2E1) / 0.6E1 + (-t497
     #6 * t725 + t925) * t79 - t698 * ((t939 - t952 * (t936 - (t725 - t4
     #641) * t79) * t79) * t79 + (t957 - (t955 - t4672) * t79) * t79) / 
     #0.24E2
        t4995 = t3686 * t4994
        t4997 = t618 * t4995 / 0.2E1
        t5028 = t327 * (t1029 - (t228 / 0.2E1 - t4568 / 0.2E1) * t79) * 
     #t79
        t5047 = (t2485 - t2515) * t47
        t5099 = (t274 * t4870 - t387 * t4882) * t47 - t621 * ((t1461 * t
     #3534 - t1553 * t3538) * t47 + ((t2468 - t2509) * t47 - (t2509 - t3
     #380) * t47) * t47) / 0.24E2 + t2486 + t2516 - t698 * ((t221 * (t10
     #13 - (t115 / 0.2E1 - t4561 / 0.2E1) * t79) * t79 - t5028) * t47 / 
     #0.2E1 + (t5028 - t1171 * (t1047 - (t341 / 0.2E1 - t4578 / 0.2E1) *
     # t79) * t79) * t47 / 0.2E1) / 0.6E1 - t621 * (((t2478 - t2485) * t
     #47 - t5047) * t47 / 0.2E1 + (t5047 - (t2515 - t3386) * t47) * t47 
     #/ 0.2E1) / 0.6E1 + t394 + t2517 - t621 * (t1121 / 0.2E1 + (t1119 -
     # t784 * ((t1997 / 0.2E1 - t1145 / 0.2E1) * t47 - (t1143 / 0.2E1 - 
     #t3202 / 0.2E1) * t47) * t47) * t79 / 0.2E1) / 0.6E1 - t698 * (t115
     #5 / 0.2E1 + (t1153 - (t1151 - t4607) * t79) * t79 / 0.2E1) / 0.6E1
     # + (-t1026 * t4976 + t1161) * t79 - t698 * ((t1175 - t952 * (t1172
     # - (t1026 - t4568) * t79) * t79) * t79 + (t1187 - (t1185 - t4618) 
     #* t79) * t79) / 0.24E2
        t5100 = t3686 * t5099
        t5102 = t969 * t5100 / 0.4E1
        t5109 = t10 * t1567 * t79
        t5111 = t413 * t967 * t5109 / 0.2E1
        t5114 = t1204 * t2520 * t79
        t5116 = t413 * t1220 * t5114 / 0.6E1
        t5125 = t228 - dy * t1173 / 0.24E2
        t5129 = t924 * t609 * t5125
        t5130 = t4997 + t5102 - t2226 * t5100 / 0.4E1 - t2233 * t4995 / 
     #0.2E1 - t5111 - t5116 + t4030 - t4292 - t4296 - t4299 + t4301 + t4
     #307 + t413 * t8 * t5109 / 0.2E1 + t413 * t1202 * t5114 / 0.6E1 + t
     #924 * t614 * t5125 - t5129
        t5132 = (t4852 + t5130) * t5
        t5135 = t4361 / 0.2E1
        t5139 = t924 * (t517 - dy * t937 / 0.24E2)
        t5141 = -t5132 * t608 - t3709 + t3713 - t3880 - t4400 + t4409 - 
     #t4680 - t4687 - t4689 - t4694 - t5135 + t5139
        t5143 = dy * t956 / 0.24E2
        t5146 = sqrt(t4611)
        t5154 = (t4391 - (t4389 - (-cc * t4566 * t4593 * t5146 + t4387) 
     #* t79) * t79) * t79
        t5161 = dy * (t4383 + t4389 / 0.2E1 - t698 * (t4393 / 0.2E1 + t5
     #154 / 0.2E1) / 0.6E1) / 0.4E1
        t5167 = t698 * (t4391 - dy * (t4393 - t5154) / 0.12E2) / 0.24E2
        t5168 = -t5143 - t5161 - t5167 - t4849 - t4997 - t5102 + t5111 +
     # t5116 + t4292 + t4296 + t4299 + t4414 + t5129
        t5178 = t2325 + t1234 + t1219 - t2254 + t1223 - t613 + t2327 + t
     #1905 - t2297 + t2048 - t1236 + t2333
        t5179 = t2225 - t1225 + t1227 - t2326 - t966 - t2315 - t1196 - t
     #1351 - t2321 - t1750 - t1353 - t2246
        t5190 = t3279 + t2628 + t3267 - t3306 + t3270 - t2786 + t2326 + 
     #t966 - t2315 + t1196 - t1351 + t2321
        t5191 = t1750 - t1353 + t2246 - t3307 - t3250 - t3297 - t3245 - 
     #t2721 - t3303 - t2965 - t2775 - t3252
        t5200 = t2249 * dt / 0.2E1 + (t5178 + t5179) * dt - t2249 * t609
     # + t2545 * dt / 0.2E1 + (t2613 + t2533 + t2537 - t2617 + t2541 - t
     #2543) * dt - t2545 * t609 - t3273 * dt / 0.2E1 - (t5190 + t5191) *
     # dt + t3273 * t609 - t3414 * dt / 0.2E1 - (t3445 + t3404 + t3407 -
     # t3449 + t3410 - t3412) * dt + t3414 * t609
        t5210 = t4413 + t3878 + t4312 - t4402 + t4317 - t4328 + t4415 + 
     #t4303 - t4382 + t4305 - t3867 + t4373
        t5211 = t4294 - t3869 + t3882 - t4414 - t4296 - t4400 - t4299 - 
     #t3880 - t4409 - t4292 - t3709 - t3713
        t5222 = t5139 + t5129 + t5111 - t5143 + t5116 - t4694 + t4414 + 
     #t4296 - t4400 + t4299 - t3880 + t4409
        t5223 = t4292 - t3709 + t3713 - t5135 - t4997 - t5161 - t5102 - 
     #t4687 - t5167 - t4849 - t4689 - t4680
        t5227 = t3589 * dt / 0.2E1 + (t3667 + t3579 + t3582 - t3671 + t3
     #585 - t3587) * dt - t3589 * t609 + t4331 * dt / 0.2E1 + (t5210 + t
     #5211) * dt - t4331 * t609 - t4495 * dt / 0.2E1 - (t4526 + t4485 + 
     #t4488 - t4530 + t4491 - t4493) * dt + t4495 * t609 - t5132 * dt / 
     #0.2E1 - (t5222 + t5223) * dt + t5132 * t609

        unew(i,j) = t1 + dt * t2 + (t2249 * t10 / 0.6E1 + (t2334 + 
     #t2335) * t10 / 0.2E1 + t2545 * t10 / 0.6E1 + (-t2545 * t608 + t253
     #3 + t2537 + t2541 - t2543 + t2613 - t2617) * t10 / 0.2E1 - t3273 *
     # t10 / 0.6E1 - (t3308 + t3309) * t10 / 0.2E1 - t3414 * t10 / 0.6E1
     # - (-t3414 * t608 + t3404 + t3407 + t3410 - t3412 + t3445 - t3449)
     # * t10 / 0.2E1) * t205 * t47 + (t3589 * t10 / 0.6E1 + (-t3589 * t6
     #08 + t3579 + t3582 + t3585 - t3587 + t3667 - t3671) * t10 / 0.2E1 
     #+ t4331 * t10 / 0.6E1 + (t4403 + t4416) * t10 / 0.2E1 - t4495 * t1
     #0 / 0.6E1 - (-t4495 * t608 + t4485 + t4488 + t4491 - t4493 + t4526
     # - t4530) * t10 / 0.2E1 - t5132 * t10 / 0.6E1 - (t5141 + t5168) * 
     #t10 / 0.2E1) * t205 * t79

        utnew(i,j) = t205 * t47 * t5200 + t205 * t5227 *
     # t79 + t2

        return
      end
