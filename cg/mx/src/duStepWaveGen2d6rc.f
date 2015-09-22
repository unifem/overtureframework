      subroutine duStepWaveGen2d6rc( 
     *   nd1a,nd1b,nd2a,nd2b,
     *   n1a,n1b,n2a,n2b,
     *   u,ut,unew,utnew,
     *   dx,dy,dt,cc,
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
      real dx,dy,dt,cc
c
c.. generated code to follow
c
        real t1
        real t10
        real t100
        real t1004
        real t1007
        real t1009
        real t101
        real t1013
        real t1015
        real t1016
        real t1018
        real t1025
        real t1029
        real t103
        real t1032
        real t1033
        real t1037
        real t1038
        real t1039
        real t104
        real t1043
        real t1045
        real t1048
        real t1050
        real t1054
        real t1055
        real t1059
        real t106
        real t1072
        real t108
        real t1082
        integer t109
        real t11
        real t110
        real t111
        real t1111
        real t1112
        real t1115
        real t1118
        real t1123
        real t1124
        real t1127
        real t1129
        real t113
        real t1131
        real t1132
        real t1134
        real t1136
        real t1137
        real t1142
        real t1143
        real t1145
        real t1147
        real t1148
        real t115
        real t1150
        real t1152
        real t1154
        real t1159
        real t116
        real t1169
        real t1170
        real t1172
        real t1173
        real t1175
        real t118
        real t1182
        real t1183
        real t1188
        real t1189
        real t1192
        real t1196
        real t1197
        real t1199
        real t12
        real t120
        real t1200
        real t1202
        real t1204
        real t1208
        integer t121
        real t1212
        real t1216
        real t122
        real t1220
        real t1223
        real t1224
        real t1225
        real t1227
        real t123
        real t1230
        real t1232
        real t1236
        real t1237
        real t1249
        real t125
        real t1255
        real t1262
        real t1268
        real t127
        real t1272
        real t1277
        real t1278
        real t128
        real t129
        real t1291
        real t1297
        real t13
        real t1301
        real t1305
        real t1309
        real t131
        real t1313
        real t1316
        real t1317
        real t1318
        real t132
        real t1322
        real t1326
        real t133
        real t1330
        real t1334
        real t1337
        real t1341
        real t1345
        real t1349
        real t135
        real t1353
        real t1357
        real t1360
        real t1361
        real t1362
        real t1366
        real t137
        real t1370
        real t1374
        real t1378
        real t138
        real t1381
        real t1387
        real t1391
        real t1392
        real t1394
        real t1397
        real t1399
        real t14
        real t140
        real t1403
        real t1407
        real t1408
        real t142
        real t1421
        real t1427
        real t143
        real t1433
        real t1434
        real t1438
        real t1439
        real t1441
        real t1444
        real t1445
        real t1447
        real t1448
        real t145
        real t1450
        real t1452
        real t1453
        real t1455
        real t1456
        real t1458
        real t1460
        real t1464
        real t1466
        real t1467
        real t1469
        real t147
        real t1471
        real t1476
        real t1477
        real t1479
        real t148
        real t1481
        real t1482
        real t1484
        real t1486
        real t1487
        real t1489
        real t149
        real t1490
        real t1492
        real t1494
        real t1495
        real t1497
        real t1499
        integer t15
        real t1500
        real t1502
        real t1508
        real t1509
        real t151
        real t1511
        real t1514
        real t1516
        real t1520
        real t1521
        real t153
        real t1533
        real t1539
        real t154
        real t1552
        real t1556
        real t156
        real t1561
        real t157
        real t1574
        real t1575
        real t1578
        real t1579
        real t1580
        real t1581
        real t1582
        real t1584
        real t1585
        real t1587
        real t1589
        real t159
        real t1590
        real t1592
        real t1594
        real t1598
        real t16
        real t1600
        real t1602
        real t1607
        real t1608
        real t1609
        real t161
        real t1611
        real t1613
        real t1614
        real t1616
        real t162
        real t1622
        real t1624
        real t1628
        real t1629
        real t163
        real t1631
        real t1632
        real t1634
        real t1635
        real t1637
        real t1638
        real t1640
        real t1641
        real t1643
        real t1647
        real t1648
        real t1649
        real t165
        real t1650
        real t1659
        real t1661
        real t1665
        real t1666
        real t1668
        real t1669
        real t167
        real t1671
        real t1672
        real t1674
        real t1675
        real t1677
        real t1678
        real t168
        real t1680
        real t1684
        real t1685
        real t1686
        real t1687
        real t1691
        real t1692
        real t1694
        real t1696
        real t1697
        real t1699
        integer t17
        real t170
        real t1703
        real t1706
        real t1708
        real t1712
        real t1713
        real t172
        real t1723
        real t1727
        real t1729
        real t173
        real t1730
        real t1733
        real t1736
        real t1737
        real t1740
        real t1744
        real t1745
        real t175
        real t1750
        real t1752
        real t1754
        real t1756
        real t1757
        real t1758
        integer t176
        real t1760
        real t1766
        real t177
        real t1771
        real t1772
        real t1774
        real t178
        real t1782
        real t18
        real t180
        real t1807
        real t1810
        real t1812
        real t182
        real t183
        real t184
        real t1843
        real t1844
        real t1854
        real t1855
        real t1856
        real t186
        real t1860
        real t1861
        real t1868
        real t187
        real t1874
        real t1875
        real t188
        real t1886
        real t1892
        real t1896
        real t19
        real t190
        real t1900
        real t1904
        real t1915
        real t1916
        real t192
        real t1924
        real t1928
        real t193
        real t1932
        real t1936
        real t1942
        real t1943
        real t1946
        real t1947
        real t1948
        real t1949
        real t195
        real t1955
        real t1961
        real t1962
        real t1966
        real t197
        real t1970
        real t1972
        real t1973
        real t1975
        real t1979
        real t198
        real t1980
        real t1992
        real t1993
        real t1994
        real t1996
        real t1997
        real t1999
        real t2
        real t200
        real t2000
        real t2001
        real t2003
        real t2004
        real t2005
        real t2006
        real t2008
        real t2009
        real t2011
        real t2012
        real t2013
        real t2015
        real t2019
        real t202
        real t2020
        real t2022
        real t2023
        real t2025
        real t2026
        real t2028
        real t2029
        real t203
        real t2031
        real t2032
        real t2034
        real t2039
        real t204
        real t2041
        real t2043
        real t2044
        real t2045
        real t2046
        real t2047
        real t2049
        real t2051
        real t2053
        real t2054
        real t2055
        real t2056
        real t2057
        real t206
        real t2061
        real t2062
        real t2064
        real t2069
        real t2070
        real t2075
        real t2077
        real t2078
        real t208
        real t2080
        real t2082
        real t2083
        real t2085
        real t2087
        real t2089
        real t209
        real t2090
        real t2093
        real t2096
        real t2099
        real t21
        real t2102
        real t2105
        real t2109
        real t211
        real t2110
        real t2112
        real t2113
        real t2116
        real t212
        real t2120
        real t2124
        real t2128
        real t2129
        real t2136
        real t2138
        real t2139
        real t214
        real t2145
        real t2149
        real t2150
        real t2154
        real t2156
        real t2157
        real t2159
        real t216
        real t2161
        real t2162
        real t2167
        real t217
        real t2173
        real t2178
        real t2179
        real t218
        real t2181
        real t2183
        real t2186
        real t2188
        real t2189
        real t2194
        real t2197
        real t2198
        real t22
        real t220
        real t2203
        real t2205
        real t2207
        real t2210
        real t2211
        real t2212
        real t2213
        real t2216
        real t2219
        real t222
        real t2222
        real t2226
        real t223
        real t2235
        real t2239
        real t2242
        real t2244
        real t2246
        real t2248
        real t225
        real t2250
        real t2252
        real t2254
        real t2261
        real t2265
        real t2267
        real t2269
        real t227
        real t2274
        real t2276
        real t2278
        real t228
        real t2281
        real t2283
        real t2287
        real t2293
        real t2299
        integer t23
        real t230
        real t2305
        real t2311
        real t2321
        real t2329
        real t233
        integer t2332
        real t2334
        real t2338
        real t2342
        real t235
        real t2355
        real t2359
        real t236
        real t2374
        real t2375
        real t2379
        real t238
        real t2381
        real t2384
        real t2386
        real t2390
        real t2391
        real t2395
        real t24
        real t240
        real t2408
        real t241
        real t2412
        real t2418
        real t2422
        real t243
        real t2435
        real t244
        real t2441
        real t2447
        real t2448
        real t2451
        real t2452
        real t2453
        real t2457
        real t2458
        real t246
        real t2462
        real t2467
        real t2473
        real t2477
        real t2478
        real t2479
        real t248
        real t2483
        real t2487
        integer t249
        real t2491
        real t2492
        real t2493
        real t2497
        real t25
        real t250
        real t2508
        real t2509
        real t251
        real t2519
        real t2523
        real t253
        real t2541
        real t2547
        real t2548
        real t255
        real t2552
        real t2554
        real t2557
        real t2559
        real t256
        real t2563
        real t2564
        real t2568
        real t258
        real t2581
        real t2591
        real t260
        real t261
        real t2620
        real t2621
        real t2624
        real t2628
        real t263
        real t2633
        real t2634
        real t2638
        real t2639
        real t264
        real t2643
        real t2644
        real t2645
        real t2649
        real t2653
        real t2657
        real t2658
        real t2659
        real t266
        real t2663
        real t2668
        real t267
        real t2672
        real t2674
        real t2675
        real t2678
        real t2684
        real t2685
        real t269
        real t2690
        real t2692
        real t2695
        real t2697
        real t27
        real t271
        real t2717
        real t272
        real t274
        real t2740
        real t2744
        real t2748
        real t275
        real t2751
        real t2754
        real t2761
        real t2765
        real t2769
        real t277
        real t2772
        real t2775
        real t2781
        real t2785
        real t2786
        real t2788
        real t279
        real t2791
        real t2793
        real t2797
        integer t280
        real t2801
        real t2802
        real t281
        real t2815
        real t282
        real t2821
        real t2827
        real t2828
        real t2838
        real t2839
        real t284
        real t2840
        real t2847
        real t2848
        real t2854
        real t2855
        real t2858
        real t286
        real t2863
        real t2868
        real t2869
        real t287
        real t2872
        real t2873
        real t2876
        real t2884
        real t2885
        real t2889
        real t289
        real t2892
        real t2894
        real t2897
        real t29
        real t2903
        real t2907
        real t291
        real t2911
        real t2915
        real t292
        real t2921
        real t2922
        real t2933
        real t2934
        real t294
        real t2941
        real t2944
        real t2946
        real t297
        real t298
        real t2984
        real t2985
        real t2993
        integer t30
        real t300
        real t3000
        real t301
        real t3013
        real t3019
        real t302
        real t303
        real t3031
        real t3033
        real t3035
        real t3036
        real t3037
        real t3038
        real t3041
        real t3043
        real t305
        real t3053
        real t3063
        real t307
        real t3087
        real t309
        real t3090
        real t3092
        real t3094
        real t3096
        real t3098
        real t31
        real t311
        real t3117
        real t3129
        real t313
        real t314
        real t3141
        real t3148
        real t315
        real t3150
        real t3153
        real t3157
        real t3163
        real t3169
        real t317
        real t3175
        real t318
        real t319
        real t32
        real t320
        real t3208
        real t3216
        integer t3217
        real t3219
        real t322
        real t3223
        real t3224
        real t3228
        real t3234
        real t3235
        real t324
        real t3245
        real t3246
        real t3248
        real t3249
        real t3251
        real t3253
        real t3254
        real t3256
        real t3258
        real t3259
        real t326
        real t3260
        real t3262
        real t3270
        real t3271
        real t3273
        real t3274
        real t3276
        real t3278
        real t3279
        real t328
        real t3281
        real t3283
        real t3284
        real t3285
        real t3287
        real t3289
        real t3290
        real t3292
        real t330
        real t3303
        real t3306
        real t3308
        real t332
        real t3331
        real t334
        real t3346
        real t3350
        real t336
        real t3365
        real t3366
        real t3369
        real t3370
        real t3372
        real t3375
        real t3376
        real t3377
        real t3379
        real t338
        real t3381
        real t3383
        real t3385
        real t3386
        real t3388
        real t3390
        real t3392
        real t3397
        real t3398
        real t34
        real t340
        real t3408
        real t3409
        real t3411
        real t3412
        real t3414
        real t3416
        real t3418
        real t3419
        real t342
        real t3420
        real t3422
        real t3429
        real t3431
        real t3434
        real t3436
        real t344
        real t3456
        real t3459
        real t346
        real t3463
        real t3465
        real t3467
        real t3473
        real t3477
        real t3479
        real t348
        real t3483
        real t3485
        real t3487
        real t3489
        real t349
        real t3491
        real t3493
        real t3495
        real t3496
        real t3499
        real t35
        real t350
        real t3500
        real t3506
        real t3516
        real t3519
        real t352
        real t3521
        real t353
        real t354
        real t355
        real t356
        real t3560
        real t3561
        real t3567
        real t3569
        real t3571
        real t3572
        real t3573
        real t3576
        real t3577
        real t358
        real t3580
        real t3581
        real t3583
        real t3584
        real t3586
        real t3587
        real t3589
        real t359
        real t3590
        real t3592
        real t3593
        real t3595
        real t3596
        real t3597
        real t3599
        integer t36
        real t3601
        real t3602
        real t3603
        real t3606
        real t3609
        real t361
        real t3613
        real t3616
        real t3618
        real t3622
        real t3623
        real t3626
        real t363
        real t3633
        real t3634
        real t364
        real t3641
        real t3642
        real t3644
        real t3646
        real t3648
        real t365
        real t3650
        real t3652
        real t3654
        real t3656
        real t3658
        real t3659
        real t366
        real t3662
        real t3663
        real t3665
        real t3666
        real t367
        real t3671
        real t3673
        real t3675
        real t3677
        real t3678
        real t3679
        real t3681
        real t3688
        real t369
        real t3690
        real t3692
        real t3694
        real t3695
        real t3697
        real t3699
        real t37
        real t370
        real t3701
        real t3703
        real t3705
        real t3706
        real t3707
        real t3709
        real t3716
        real t3717
        real t3719
        real t372
        real t3720
        real t3722
        real t3723
        real t3725
        real t373
        real t3731
        real t3739
        real t3741
        real t3744
        real t3746
        real t375
        real t3750
        real t3752
        real t3756
        real t3757
        real t376
        real t3761
        real t3766
        real t3767
        real t377
        real t3773
        real t378
        real t3783
        real t3794
        real t3797
        real t3799
        real t38
        real t380
        real t381
        real t383
        real t3831
        real t3835
        real t3839
        real t384
        real t385
        real t3852
        real t3856
        real t387
        real t3871
        real t3872
        real t3875
        real t3883
        real t3884
        real t3888
        real t389
        real t3892
        real t3896
        real t3909
        real t391
        real t3920
        real t3921
        real t3924
        real t3929
        real t393
        real t3930
        real t3932
        real t3934
        real t3937
        real t3938
        real t3939
        real t3941
        real t3942
        real t3944
        real t3945
        real t3947
        real t3948
        real t395
        real t3950
        real t3951
        real t3953
        real t3954
        real t3955
        real t3957
        real t3959
        real t3960
        real t3961
        real t3964
        real t3967
        real t3969
        real t397
        real t3972
        real t3974
        real t3976
        real t3977
        real t3978
        real t3979
        real t398
        real t3981
        real t3983
        real t3985
        real t3986
        real t3987
        real t3989
        real t399
        real t3994
        real t3995
        real t3997
        real t3999
        real t4
        real t40
        real t4000
        real t4003
        real t4007
        real t4010
        real t4012
        real t402
        real t403
        real t404
        real t4053
        real t4054
        real t4057
        real t4058
        real t406
        real t4061
        real t4064
        real t4066
        real t4069
        real t407
        real t4071
        real t4074
        real t4077
        real t408
        real t4084
        real t4086
        real t4089
        real t4090
        real t4094
        real t4097
        real t4099
        real t410
        real t4107
        real t4113
        real t412
        real t4128
        real t413
        real t4130
        real t4136
        real t414
        real t4140
        real t4141
        real t4143
        real t415
        real t4156
        real t4158
        real t416
        real t4161
        real t417
        real t4178
        real t4186
        real t419
        real t4190
        real t4192
        real t4196
        real t4198
        real t42
        real t420
        real t4202
        real t4204
        real t4209
        real t4211
        real t4214
        real t4218
        real t422
        real t4224
        real t423
        real t4230
        real t4236
        real t425
        integer t4254
        real t4255
        real t426
        real t4266
        real t427
        real t4272
        real t4279
        real t428
        real t4280
        real t4284
        real t4288
        real t4292
        real t43
        real t430
        real t4305
        real t4306
        real t431
        real t4310
        real t4321
        real t4322
        real t4326
        real t433
        real t4334
        real t434
        real t4342
        real t4344
        real t4348
        real t4349
        real t435
        real t4353
        real t4357
        real t4358
        real t4359
        real t4369
        real t437
        real t4370
        real t4371
        real t4378
        real t4379
        real t4382
        real t4385
        real t4387
        real t4388
        real t439
        real t4391
        real t4394
        real t4395
        real t4406
        real t4407
        real t441
        real t4414
        real t4415
        real t4418
        real t443
        real t4430
        real t4431
        real t4437
        real t4438
        real t4446
        real t4447
        real t445
        real t4462
        real t4463
        real t4469
        real t447
        real t4479
        real t448
        real t449
        real t4490
        real t4493
        real t4495
        real t45
        real t452
        real t4530
        real t454
        real t4543
        real t4547
        real t455
        real t456
        real t4562
        real t4563
        real t4567
        real t457
        real t4570
        real t4572
        real t459
        real t4592
        real t4596
        real t460
        real t4600
        real t4604
        real t4607
        real t4611
        real t4617
        real t4618
        real t462
        real t4621
        real t463
        real t4634
        real t4637
        real t4639
        real t464
        real t466
        real t468
        real t4682
        real t4683
        real t4687
        real t4690
        real t4692
        real t47
        real t470
        real t471
        real t472
        real t4733
        real t4734
        real t4741
        real t4742
        real t475
        real t4756
        real t4759
        real t476
        real t4761
        real t477
        real t479
        real t48
        real t480
        real t4800
        real t4801
        real t4804
        real t4806
        real t4809
        real t481
        real t4811
        real t4814
        real t4816
        real t4820
        real t483
        real t4831
        real t4842
        real t485
        real t4859
        real t486
        real t4869
        real t487
        real t4871
        real t488
        real t4882
        real t489
        real t49
        real t4901
        real t491
        real t4910
        real t492
        real t4921
        real t4923
        real t4926
        real t4930
        real t4936
        real t494
        real t4942
        real t4948
        real t495
        real t497
        real t498
        real t499
        real t5
        real t500
        real t502
        real t503
        real t505
        real t506
        real t507
        real t509
        real t51
        real t511
        real t513
        real t515
        real t517
        real t519
        real t52
        real t520
        real t521
        real t524
        real t526
        real t529
        real t53
        real t530
        real t531
        real t532
        real t534
        real t535
        real t537
        real t538
        real t539
        real t541
        real t543
        real t545
        real t546
        real t547
        real t55
        real t550
        real t551
        real t552
        real t554
        real t555
        real t556
        real t558
        real t560
        real t561
        real t562
        real t563
        real t564
        real t566
        real t567
        real t569
        real t57
        real t570
        real t572
        real t573
        real t574
        real t575
        real t577
        real t578
        real t58
        real t580
        real t581
        real t582
        real t584
        real t586
        real t588
        real t590
        real t592
        real t594
        real t595
        real t596
        real t599
        real t6
        real t60
        real t601
        real t604
        real t605
        real t606
        real t608
        real t609
        integer t61
        real t610
        real t612
        real t614
        real t616
        real t618
        real t62
        real t620
        real t622
        real t624
        real t625
        real t626
        real t628
        real t63
        real t630
        real t631
        real t632
        real t634
        real t635
        real t636
        real t638
        real t639
        real t640
        real t642
        real t644
        real t646
        real t648
        real t649
        real t65
        real t650
        real t652
        real t657
        real t658
        real t660
        real t661
        real t662
        real t664
        real t665
        real t667
        real t668
        real t67
        real t670
        real t671
        real t673
        real t674
        real t676
        real t677
        real t678
        real t68
        real t680
        real t682
        real t683
        real t684
        real t687
        real t688
        real t689
        real t69
        real t691
        real t692
        real t694
        real t695
        real t697
        real t698
        real t7
        real t700
        real t701
        real t703
        real t704
        real t705
        real t707
        real t709
        real t71
        real t710
        real t711
        real t714
        real t715
        real t717
        real t718
        real t72
        real t720
        real t721
        real t722
        real t724
        real t725
        real t726
        real t729
        real t73
        real t730
        real t731
        real t733
        real t734
        real t736
        real t737
        real t739
        real t740
        real t742
        real t743
        real t745
        real t746
        real t747
        real t749
        real t75
        real t751
        real t752
        real t753
        real t756
        real t757
        real t759
        real t760
        real t762
        real t763
        real t765
        real t766
        real t767
        real t769
        real t77
        real t770
        real t771
        real t774
        real t775
        real t776
        real t778
        real t779
        real t78
        real t781
        real t782
        real t784
        real t785
        real t787
        real t788
        real t790
        real t791
        real t792
        real t794
        real t796
        real t797
        real t798
        real t8
        real t80
        real t801
        real t802
        real t804
        real t807
        real t808
        real t810
        real t811
        real t813
        real t814
        real t816
        real t817
        real t819
        real t82
        real t820
        real t822
        real t826
        real t827
        real t828
        real t829
        real t83
        real t833
        real t834
        real t836
        real t837
        real t839
        real t840
        real t842
        real t843
        real t845
        real t846
        real t848
        real t849
        real t85
        real t850
        real t852
        real t854
        real t855
        real t856
        real t859
        real t86
        real t860
        real t862
        real t863
        real t865
        real t866
        real t867
        real t869
        real t870
        real t871
        real t874
        real t875
        real t877
        real t878
        real t879
        real t88
        real t881
        real t882
        real t884
        real t885
        real t887
        real t888
        real t890
        real t891
        real t893
        real t894
        real t895
        real t897
        real t899
        real t9
        real t90
        real t900
        real t901
        real t904
        real t905
        real t907
        real t908
        integer t91
        real t910
        real t911
        real t912
        real t914
        real t915
        real t916
        real t919
        real t92
        real t920
        real t922
        real t925
        real t926
        real t928
        real t929
        real t93
        real t931
        real t932
        real t934
        real t935
        real t937
        real t938
        real t940
        real t944
        real t945
        real t946
        real t947
        real t95
        real t951
        real t952
        real t954
        real t955
        real t956
        real t958
        real t959
        real t961
        real t963
        real t964
        real t966
        real t968
        real t97
        real t970
        real t971
        real t973
        real t975
        real t977
        real t978
        real t98
        real t980
        real t983
        integer t985
        real t986
        real t987
        real t991
        real t995
        real t999
        t1 = u(i,j,n)
        t2 = ut(i,j,n)
        t4 = sqrt(0.15E2)
        t5 = t4 / 0.10E2
        t6 = 0.1E1 / 0.2E1 - t5
        t7 = t6 ** 2
        t8 = t7 ** 2
        t9 = t8 * t6
        t10 = dt ** 2
        t11 = t10 ** 2
        t12 = t11 * dt
        t13 = t9 * t12
        t14 = cc ** 2
        t15 = i + 3
        t16 = u(t15,j,n)
        t17 = i + 2
        t18 = u(t17,j,n)
        t19 = t16 - t18
        t21 = 0.1E1 / dx
        t22 = t14 * t19 * t21
        t23 = i + 1
        t24 = u(t23,j,n)
        t25 = t18 - t24
        t27 = t14 * t25 * t21
        t29 = (t22 - t27) * t21
        t30 = j + 1
        t31 = u(t17,t30,n)
        t32 = t31 - t18
        t34 = 0.1E1 / dy
        t35 = t14 * t32 * t34
        t36 = j - 1
        t37 = u(t17,t36,n)
        t38 = t18 - t37
        t40 = t14 * t38 * t34
        t42 = (t35 - t40) * t34
        t43 = t24 - t1
        t45 = t14 * t43 * t21
        t47 = (t27 - t45) * t21
        t48 = u(t23,t30,n)
        t49 = t48 - t24
        t51 = t14 * t49 * t34
        t52 = u(t23,t36,n)
        t53 = t24 - t52
        t55 = t14 * t53 * t34
        t57 = (t51 - t55) * t34
        t58 = t29 + t42 - t47 - t57
        t60 = t14 * t58 * t21
        t61 = i - 1
        t62 = u(t61,j,n)
        t63 = t1 - t62
        t65 = t14 * t63 * t21
        t67 = (t45 - t65) * t21
        t68 = u(i,t30,n)
        t69 = t68 - t1
        t71 = t14 * t69 * t34
        t72 = u(i,t36,n)
        t73 = t1 - t72
        t75 = t14 * t73 * t34
        t77 = (t71 - t75) * t34
        t78 = t47 + t57 - t67 - t77
        t80 = t14 * t78 * t21
        t82 = (t60 - t80) * t21
        t83 = t31 - t48
        t85 = t14 * t83 * t21
        t86 = t48 - t68
        t88 = t14 * t86 * t21
        t90 = (t85 - t88) * t21
        t91 = j + 2
        t92 = u(t23,t91,n)
        t93 = t92 - t48
        t95 = t14 * t93 * t34
        t97 = (t95 - t51) * t34
        t98 = t90 + t97 - t47 - t57
        t100 = t14 * t98 * t34
        t101 = t37 - t52
        t103 = t14 * t101 * t21
        t104 = t52 - t72
        t106 = t14 * t104 * t21
        t108 = (t103 - t106) * t21
        t109 = j - 2
        t110 = u(t23,t109,n)
        t111 = t52 - t110
        t113 = t14 * t111 * t34
        t115 = (t55 - t113) * t34
        t116 = t47 + t57 - t108 - t115
        t118 = t14 * t116 * t34
        t120 = (t100 - t118) * t34
        t121 = i - 2
        t122 = u(t121,j,n)
        t123 = t62 - t122
        t125 = t14 * t123 * t21
        t127 = (t65 - t125) * t21
        t128 = u(t61,t30,n)
        t129 = t128 - t62
        t131 = t14 * t129 * t34
        t132 = u(t61,t36,n)
        t133 = t62 - t132
        t135 = t14 * t133 * t34
        t137 = (t131 - t135) * t34
        t138 = t67 + t77 - t127 - t137
        t140 = t14 * t138 * t21
        t142 = (t80 - t140) * t21
        t143 = t68 - t128
        t145 = t14 * t143 * t21
        t147 = (t88 - t145) * t21
        t148 = u(i,t91,n)
        t149 = t148 - t68
        t151 = t14 * t149 * t34
        t153 = (t151 - t71) * t34
        t154 = t147 + t153 - t67 - t77
        t156 = t14 * t154 * t34
        t157 = t72 - t132
        t159 = t14 * t157 * t21
        t161 = (t106 - t159) * t21
        t162 = u(i,t109,n)
        t163 = t72 - t162
        t165 = t14 * t163 * t34
        t167 = (t75 - t165) * t34
        t168 = t67 + t77 - t161 - t167
        t170 = t14 * t168 * t34
        t172 = (t156 - t170) * t34
        t173 = t82 + t120 - t142 - t172
        t175 = t14 * t173 * t21
        t176 = i - 3
        t177 = u(t176,j,n)
        t178 = t122 - t177
        t180 = t14 * t178 * t21
        t182 = (t125 - t180) * t21
        t183 = u(t121,t30,n)
        t184 = t183 - t122
        t186 = t14 * t184 * t34
        t187 = u(t121,t36,n)
        t188 = t122 - t187
        t190 = t14 * t188 * t34
        t192 = (t186 - t190) * t34
        t193 = t127 + t137 - t182 - t192
        t195 = t14 * t193 * t21
        t197 = (t140 - t195) * t21
        t198 = t128 - t183
        t200 = t14 * t198 * t21
        t202 = (t145 - t200) * t21
        t203 = u(t61,t91,n)
        t204 = t203 - t128
        t206 = t14 * t204 * t34
        t208 = (t206 - t131) * t34
        t209 = t202 + t208 - t127 - t137
        t211 = t14 * t209 * t34
        t212 = t132 - t187
        t214 = t14 * t212 * t21
        t216 = (t159 - t214) * t21
        t217 = u(t61,t109,n)
        t218 = t132 - t217
        t220 = t14 * t218 * t34
        t222 = (t135 - t220) * t34
        t223 = t127 + t137 - t216 - t222
        t225 = t14 * t223 * t34
        t227 = (t211 - t225) * t34
        t228 = t142 + t172 - t197 - t227
        t230 = t14 * t228 * t21
        t233 = t90 + t97 - t147 - t153
        t235 = t14 * t233 * t21
        t236 = t147 + t153 - t202 - t208
        t238 = t14 * t236 * t21
        t240 = (t235 - t238) * t21
        t241 = t92 - t148
        t243 = t14 * t241 * t21
        t244 = t148 - t203
        t246 = t14 * t244 * t21
        t248 = (t243 - t246) * t21
        t249 = j + 3
        t250 = u(i,t249,n)
        t251 = t250 - t148
        t253 = t14 * t251 * t34
        t255 = (t253 - t151) * t34
        t256 = t248 + t255 - t147 - t153
        t258 = t14 * t256 * t34
        t260 = (t258 - t156) * t34
        t261 = t240 + t260 - t142 - t172
        t263 = t14 * t261 * t34
        t264 = t108 + t115 - t161 - t167
        t266 = t14 * t264 * t21
        t267 = t161 + t167 - t216 - t222
        t269 = t14 * t267 * t21
        t271 = (t266 - t269) * t21
        t272 = t110 - t162
        t274 = t14 * t272 * t21
        t275 = t162 - t217
        t277 = t14 * t275 * t21
        t279 = (t274 - t277) * t21
        t280 = j - 3
        t281 = u(i,t280,n)
        t282 = t162 - t281
        t284 = t14 * t282 * t34
        t286 = (t165 - t284) * t34
        t287 = t161 + t167 - t279 - t286
        t289 = t14 * t287 * t34
        t291 = (t170 - t289) * t34
        t292 = t142 + t172 - t271 - t291
        t294 = t14 * t292 * t34
        t297 = (t175 - t230) * t21 + (t263 - t294) * t34
        t298 = cc * t297
        t300 = t13 * t298 / 0.240E3
        t301 = t7 * t6
        t302 = t10 * dt
        t303 = t301 * t302
        t305 = cc * (t82 + t120)
        t307 = cc * (t142 + t172)
        t309 = (t305 - t307) * t21
        t311 = cc * (t197 + t227)
        t313 = (t307 - t311) * t21
        t314 = t309 - t313
        t315 = dx * t314
        t317 = t303 * t315 / 0.144E3
        t318 = dt * t6
        t319 = dx ** 2
        t320 = t319 * dx
        t322 = cc * (t29 + t42)
        t324 = cc * (t47 + t57)
        t326 = (t322 - t324) * t21
        t328 = cc * (t67 + t77)
        t330 = (t324 - t328) * t21
        t332 = (t326 - t330) * t21
        t334 = cc * (t127 + t137)
        t336 = (t328 - t334) * t21
        t338 = (t330 - t336) * t21
        t340 = (t332 - t338) * t21
        t342 = cc * (t182 + t192)
        t344 = (t334 - t342) * t21
        t346 = (t336 - t344) * t21
        t348 = (t338 - t346) * t21
        t349 = t340 - t348
        t350 = t320 * t349
        t352 = t318 * t350 / 0.1440E4
        t353 = t7 * t10
        t354 = ut(t17,j,n)
        t355 = ut(t23,j,n)
        t356 = t354 - t355
        t358 = t14 * t356 * t21
        t359 = t355 - t2
        t361 = t14 * t359 * t21
        t363 = (t358 - t361) * t21
        t364 = ut(t15,j,n)
        t365 = t364 - t354
        t366 = t365 * t21
        t367 = t356 * t21
        t369 = (t366 - t367) * t21
        t370 = t359 * t21
        t372 = (t367 - t370) * t21
        t373 = t369 - t372
        t375 = t14 * t373 * t21
        t376 = ut(t61,j,n)
        t377 = t2 - t376
        t378 = t377 * t21
        t380 = (t370 - t378) * t21
        t381 = t372 - t380
        t383 = t14 * t381 * t21
        t384 = t375 - t383
        t385 = t384 * t21
        t387 = t14 * t365 * t21
        t389 = (t387 - t358) * t21
        t391 = (t389 - t363) * t21
        t393 = t14 * t377 * t21
        t395 = (t361 - t393) * t21
        t397 = (t363 - t395) * t21
        t398 = t391 - t397
        t399 = t398 * t21
        t402 = t319 * (t385 + t399) / 0.24E2
        t403 = ut(t23,t30,n)
        t404 = t403 - t355
        t406 = t14 * t404 * t34
        t407 = ut(t23,t36,n)
        t408 = t355 - t407
        t410 = t14 * t408 * t34
        t412 = (t406 - t410) * t34
        t413 = dy ** 2
        t414 = ut(t23,t91,n)
        t415 = t414 - t403
        t416 = t415 * t34
        t417 = t404 * t34
        t419 = (t416 - t417) * t34
        t420 = t408 * t34
        t422 = (t417 - t420) * t34
        t423 = t419 - t422
        t425 = t14 * t423 * t34
        t426 = ut(t23,t109,n)
        t427 = t407 - t426
        t428 = t427 * t34
        t430 = (t420 - t428) * t34
        t431 = t422 - t430
        t433 = t14 * t431 * t34
        t434 = t425 - t433
        t435 = t434 * t34
        t437 = t14 * t415 * t34
        t439 = (t437 - t406) * t34
        t441 = (t439 - t412) * t34
        t443 = t14 * t427 * t34
        t445 = (t410 - t443) * t34
        t447 = (t412 - t445) * t34
        t448 = t441 - t447
        t449 = t448 * t34
        t452 = t413 * (t435 + t449) / 0.24E2
        t454 = cc * (t363 - t402 + t412 - t452)
        t455 = ut(t121,j,n)
        t456 = t376 - t455
        t457 = t456 * t21
        t459 = (t378 - t457) * t21
        t460 = t380 - t459
        t462 = t14 * t460 * t21
        t463 = t383 - t462
        t464 = t463 * t21
        t466 = t14 * t456 * t21
        t468 = (t393 - t466) * t21
        t470 = (t395 - t468) * t21
        t471 = t397 - t470
        t472 = t471 * t21
        t475 = t319 * (t464 + t472) / 0.24E2
        t476 = ut(i,t30,n)
        t477 = t476 - t2
        t479 = t14 * t477 * t34
        t480 = ut(i,t36,n)
        t481 = t2 - t480
        t483 = t14 * t481 * t34
        t485 = (t479 - t483) * t34
        t486 = ut(i,t91,n)
        t487 = t486 - t476
        t488 = t487 * t34
        t489 = t477 * t34
        t491 = (t488 - t489) * t34
        t492 = t481 * t34
        t494 = (t489 - t492) * t34
        t495 = t491 - t494
        t497 = t14 * t495 * t34
        t498 = ut(i,t109,n)
        t499 = t480 - t498
        t500 = t499 * t34
        t502 = (t492 - t500) * t34
        t503 = t494 - t502
        t505 = t14 * t503 * t34
        t506 = t497 - t505
        t507 = t506 * t34
        t509 = t14 * t487 * t34
        t511 = (t509 - t479) * t34
        t513 = (t511 - t485) * t34
        t515 = t14 * t499 * t34
        t517 = (t483 - t515) * t34
        t519 = (t485 - t517) * t34
        t520 = t513 - t519
        t521 = t520 * t34
        t524 = t413 * (t507 + t521) / 0.24E2
        t526 = cc * (t395 - t475 + t485 - t524)
        t529 = (t454 - t526) * t21 / 0.2E1
        t530 = ut(t176,j,n)
        t531 = t455 - t530
        t532 = t531 * t21
        t534 = (t457 - t532) * t21
        t535 = t459 - t534
        t537 = t14 * t535 * t21
        t538 = t462 - t537
        t539 = t538 * t21
        t541 = t14 * t531 * t21
        t543 = (t466 - t541) * t21
        t545 = (t468 - t543) * t21
        t546 = t470 - t545
        t547 = t546 * t21
        t550 = t319 * (t539 + t547) / 0.24E2
        t551 = ut(t61,t30,n)
        t552 = t551 - t376
        t554 = t14 * t552 * t34
        t555 = ut(t61,t36,n)
        t556 = t376 - t555
        t558 = t14 * t556 * t34
        t560 = (t554 - t558) * t34
        t561 = ut(t61,t91,n)
        t562 = t561 - t551
        t563 = t562 * t34
        t564 = t552 * t34
        t566 = (t563 - t564) * t34
        t567 = t556 * t34
        t569 = (t564 - t567) * t34
        t570 = t566 - t569
        t572 = t14 * t570 * t34
        t573 = ut(t61,t109,n)
        t574 = t555 - t573
        t575 = t574 * t34
        t577 = (t567 - t575) * t34
        t578 = t569 - t577
        t580 = t14 * t578 * t34
        t581 = t572 - t580
        t582 = t581 * t34
        t584 = t14 * t562 * t34
        t586 = (t584 - t554) * t34
        t588 = (t586 - t560) * t34
        t590 = t14 * t574 * t34
        t592 = (t558 - t590) * t34
        t594 = (t560 - t592) * t34
        t595 = t588 - t594
        t596 = t595 * t34
        t599 = t413 * (t582 + t596) / 0.24E2
        t601 = cc * (t468 - t550 + t560 - t599)
        t604 = (t526 - t601) * t21 / 0.2E1
        t605 = ut(t17,t30,n)
        t606 = t605 - t354
        t608 = t14 * t606 * t34
        t609 = ut(t17,t36,n)
        t610 = t354 - t609
        t612 = t14 * t610 * t34
        t614 = (t608 - t612) * t34
        t616 = cc * (t389 + t614)
        t618 = cc * (t363 + t412)
        t620 = (t616 - t618) * t21
        t622 = cc * (t395 + t485)
        t624 = (t618 - t622) * t21
        t625 = t620 - t624
        t626 = t625 * t21
        t628 = cc * (t468 + t560)
        t630 = (t622 - t628) * t21
        t631 = t624 - t630
        t632 = t631 * t21
        t634 = (t626 - t632) * t21
        t635 = ut(t121,t30,n)
        t636 = t635 - t455
        t638 = t14 * t636 * t34
        t639 = ut(t121,t36,n)
        t640 = t455 - t639
        t642 = t14 * t640 * t34
        t644 = (t638 - t642) * t34
        t646 = cc * (t543 + t644)
        t648 = (t628 - t646) * t21
        t649 = t630 - t648
        t650 = t649 * t21
        t652 = (t632 - t650) * t21
        t657 = t529 + t604 - t319 * (t634 / 0.2E1 + t652 / 0.2E1) / 0.6E
     #1
        t658 = dx * t657
        t660 = t353 * t658 / 0.8E1
        t661 = t19 * t21
        t662 = t25 * t21
        t664 = (t661 - t662) * t21
        t665 = t43 * t21
        t667 = (t662 - t665) * t21
        t668 = t664 - t667
        t670 = t14 * t668 * t21
        t671 = t63 * t21
        t673 = (t665 - t671) * t21
        t674 = t667 - t673
        t676 = t14 * t674 * t21
        t677 = t670 - t676
        t678 = t677 * t21
        t680 = (t29 - t47) * t21
        t682 = (t47 - t67) * t21
        t683 = t680 - t682
        t684 = t683 * t21
        t687 = t319 * (t678 + t684) / 0.24E2
        t688 = t93 * t34
        t689 = t49 * t34
        t691 = (t688 - t689) * t34
        t692 = t53 * t34
        t694 = (t689 - t692) * t34
        t695 = t691 - t694
        t697 = t14 * t695 * t34
        t698 = t111 * t34
        t700 = (t692 - t698) * t34
        t701 = t694 - t700
        t703 = t14 * t701 * t34
        t704 = t697 - t703
        t705 = t704 * t34
        t707 = (t97 - t57) * t34
        t709 = (t57 - t115) * t34
        t710 = t707 - t709
        t711 = t710 * t34
        t714 = t413 * (t705 + t711) / 0.24E2
        t715 = t123 * t21
        t717 = (t671 - t715) * t21
        t718 = t673 - t717
        t720 = t14 * t718 * t21
        t721 = t676 - t720
        t722 = t721 * t21
        t724 = (t67 - t127) * t21
        t725 = t682 - t724
        t726 = t725 * t21
        t729 = t319 * (t722 + t726) / 0.24E2
        t730 = t149 * t34
        t731 = t69 * t34
        t733 = (t730 - t731) * t34
        t734 = t73 * t34
        t736 = (t731 - t734) * t34
        t737 = t733 - t736
        t739 = t14 * t737 * t34
        t740 = t163 * t34
        t742 = (t734 - t740) * t34
        t743 = t736 - t742
        t745 = t14 * t743 * t34
        t746 = t739 - t745
        t747 = t746 * t34
        t749 = (t153 - t77) * t34
        t751 = (t77 - t167) * t34
        t752 = t749 - t751
        t753 = t752 * t34
        t756 = t413 * (t747 + t753) / 0.24E2
        t757 = t47 - t687 + t57 - t714 - t67 + t729 - t77 + t756
        t759 = t14 * t757 * t21
        t760 = t178 * t21
        t762 = (t715 - t760) * t21
        t763 = t717 - t762
        t765 = t14 * t763 * t21
        t766 = t720 - t765
        t767 = t766 * t21
        t769 = (t127 - t182) * t21
        t770 = t724 - t769
        t771 = t770 * t21
        t774 = t319 * (t767 + t771) / 0.24E2
        t775 = t204 * t34
        t776 = t129 * t34
        t778 = (t775 - t776) * t34
        t779 = t133 * t34
        t781 = (t776 - t779) * t34
        t782 = t778 - t781
        t784 = t14 * t782 * t34
        t785 = t218 * t34
        t787 = (t779 - t785) * t34
        t788 = t781 - t787
        t790 = t14 * t788 * t34
        t791 = t784 - t790
        t792 = t791 * t34
        t794 = (t208 - t137) * t34
        t796 = (t137 - t222) * t34
        t797 = t794 - t796
        t798 = t797 * t34
        t801 = t413 * (t792 + t798) / 0.24E2
        t802 = t67 - t729 + t77 - t756 - t127 + t774 - t137 + t801
        t804 = t14 * t802 * t21
        t807 = t58 * t21
        t808 = t78 * t21
        t810 = (t807 - t808) * t21
        t811 = t138 * t21
        t813 = (t808 - t811) * t21
        t814 = t810 - t813
        t816 = t14 * t814 * t21
        t817 = t193 * t21
        t819 = (t811 - t817) * t21
        t820 = t813 - t819
        t822 = t14 * t820 * t21
        t826 = t82 - t142
        t827 = t826 * t21
        t828 = t142 - t197
        t829 = t828 * t21
        t833 = t83 * t21
        t834 = t86 * t21
        t836 = (t833 - t834) * t21
        t837 = t143 * t21
        t839 = (t834 - t837) * t21
        t840 = t836 - t839
        t842 = t14 * t840 * t21
        t843 = t198 * t21
        t845 = (t837 - t843) * t21
        t846 = t839 - t845
        t848 = t14 * t846 * t21
        t849 = t842 - t848
        t850 = t849 * t21
        t852 = (t90 - t147) * t21
        t854 = (t147 - t202) * t21
        t855 = t852 - t854
        t856 = t855 * t21
        t859 = t319 * (t850 + t856) / 0.24E2
        t860 = t251 * t34
        t862 = (t860 - t730) * t34
        t863 = t862 - t733
        t865 = t14 * t863 * t34
        t866 = t865 - t739
        t867 = t866 * t34
        t869 = (t255 - t153) * t34
        t870 = t869 - t749
        t871 = t870 * t34
        t874 = t413 * (t867 + t871) / 0.24E2
        t875 = t147 - t859 + t153 - t874 - t67 + t729 - t77 + t756
        t877 = t14 * t875 * t34
        t878 = t101 * t21
        t879 = t104 * t21
        t881 = (t878 - t879) * t21
        t882 = t157 * t21
        t884 = (t879 - t882) * t21
        t885 = t881 - t884
        t887 = t14 * t885 * t21
        t888 = t212 * t21
        t890 = (t882 - t888) * t21
        t891 = t884 - t890
        t893 = t14 * t891 * t21
        t894 = t887 - t893
        t895 = t894 * t21
        t897 = (t108 - t161) * t21
        t899 = (t161 - t216) * t21
        t900 = t897 - t899
        t901 = t900 * t21
        t904 = t319 * (t895 + t901) / 0.24E2
        t905 = t282 * t34
        t907 = (t740 - t905) * t34
        t908 = t742 - t907
        t910 = t14 * t908 * t34
        t911 = t745 - t910
        t912 = t911 * t34
        t914 = (t167 - t286) * t34
        t915 = t751 - t914
        t916 = t915 * t34
        t919 = t413 * (t912 + t916) / 0.24E2
        t920 = t67 - t729 + t77 - t756 - t161 + t904 - t167 + t919
        t922 = t14 * t920 * t34
        t925 = t256 * t34
        t926 = t154 * t34
        t928 = (t925 - t926) * t34
        t929 = t168 * t34
        t931 = (t926 - t929) * t34
        t932 = t928 - t931
        t934 = t14 * t932 * t34
        t935 = t287 * t34
        t937 = (t929 - t935) * t34
        t938 = t931 - t937
        t940 = t14 * t938 * t34
        t944 = t260 - t172
        t945 = t944 * t34
        t946 = t172 - t291
        t947 = t946 * t34
        t951 = (t759 - t804) * t21 - dx * (t816 - t822) / 0.24E2 - dx * 
     #(t827 - t829) / 0.24E2 + (t877 - t922) * t34 - dy * (t934 - t940) 
     #/ 0.24E2 - dy * (t945 - t947) / 0.24E2
        t952 = cc * t951
        t954 = t303 * t952 / 0.12E2
        t955 = cc * t355
        t956 = cc * t354
        t958 = (-t955 + t956) * t21
        t959 = cc * t2
        t961 = (-t959 + t955) * t21
        t963 = (t958 - t961) * t21
        t964 = cc * t376
        t966 = (t959 - t964) * t21
        t968 = (t961 - t966) * t21
        t970 = (t963 - t968) * t21
        t971 = cc * t455
        t973 = (t964 - t971) * t21
        t975 = (t966 - t973) * t21
        t977 = (t968 - t975) * t21
        t978 = t970 - t977
        t980 = t320 * t978 / 0.1440E4
        t985 = i + 4
        t986 = ut(t985,j,n)
        t987 = t986 - t364
        t991 = (t21 * t987 - t366) * t21 - t369
        t983 = t14 * t21
        t995 = (t983 * t991 - t375) * t21
        t999 = (t385 - t464) * t21
        t1004 = t373 * t21
        t1007 = t381 * t21
        t1009 = (t1004 - t1007) * t21
        t1013 = t460 * t21
        t1015 = (t1007 - t1013) * t21
        t1016 = t1009 - t1015
        t1018 = t14 * t1016 * t21
        t1025 = (t983 * t987 - t387) * t21
        t1029 = ((t1025 - t389) * t21 - t391) * t21
        t1032 = t399 - t472
        t1033 = t1032 * t21
        t1037 = t413 * dy
        t1038 = ut(t23,t249,n)
        t1039 = t1038 - t414
        t1043 = (t1039 * t34 - t416) * t34 - t419
        t1045 = t423 * t34
        t1048 = t431 * t34
        t1050 = (t1045 - t1048) * t34
        t1054 = ut(t23,t280,n)
        t1055 = t426 - t1054
        t1059 = t430 - (-t1055 * t34 + t428) * t34
        t1072 = (t1039 * t14 * t34 - t437) * t34
        t1082 = (-t1055 * t14 * t34 + t443) * t34
        t1111 = -dx * t384 / 0.24E2 - dx * t398 / 0.24E2 + t320 * ((t995
     # - t385) * t21 - t999) / 0.576E3 + 0.3E1 / 0.640E3 * t320 * (t14 *
     # ((t21 * t991 - t1004) * t21 - t1009) * t21 - t1018) + 0.3E1 / 0.6
     #40E3 * t320 * ((t1029 - t399) * t21 - t1033) + t363 + 0.3E1 / 0.64
     #0E3 * t1037 * (t14 * ((t1043 * t34 - t1045) * t34 - t1050) * t34 -
     # t14 * (t1050 - (-t1059 * t34 + t1048) * t34) * t34) + 0.3E1 / 0.6
     #40E3 * t1037 * ((((t1072 - t439) * t34 - t441) * t34 - t449) * t34
     # - (t449 - (t447 - (t445 - t1082) * t34) * t34) * t34) - dy * t434
     # / 0.24E2 - dy * t448 / 0.24E2 + t1037 * (((t1043 * t14 * t34 - t4
     #25) * t34 - t435) * t34 - (t435 - (-t1059 * t14 * t34 + t433) * t3
     #4) * t34) / 0.576E3 + t412
        t1112 = cc * t1111
        t1115 = t320 * t1032
        t1118 = t14 * t6
        t1123 = t370 - dx * t381 / 0.24E2 + 0.3E1 / 0.640E3 * t320 * t10
     #16
        t1124 = dt * t1123
        t1127 = cc * (t47 - t687 + t57 - t714)
        t1129 = cc * (t67 - t729 + t77 - t756)
        t1131 = (t1127 - t1129) * t21
        t1132 = t1131 / 0.2E1
        t1134 = cc * (t127 - t774 + t137 - t801)
        t1136 = (t1129 - t1134) * t21
        t1137 = t1136 / 0.2E1
        t1142 = t1132 + t1137 - t319 * (t340 / 0.2E1 + t348 / 0.2E1) / 0
     #.6E1
        t1143 = dx * t1142
        t1145 = t318 * t1143 / 0.4E1
        t1147 = t961 / 0.2E1
        t1148 = cc * t364
        t1150 = (-t956 + t1148) * t21
        t1152 = (t1150 - t958) * t21
        t1154 = (t1152 - t963) * t21
        t1159 = t319 ** 2
        t1169 = t1154 - t970
        t1170 = t1169 * t21
        t1172 = (((((cc * t986 - t1148) * t21 - t1150) * t21 - t1152) * 
     #t21 - t1154) * t21 - t1170) * t21
        t1173 = t978 * t21
        t1175 = (t1170 - t1173) * t21
        t1182 = dx * (t958 / 0.2E1 + t1147 - t319 * (t1154 / 0.2E1 + t97
     #0 / 0.2E1) / 0.6E1 + t1159 * (t1172 / 0.2E1 + t1175 / 0.2E1) / 0.3
     #0E2) / 0.4E1
        t1183 = -t300 - t317 + t352 - t660 - t954 + t980 + t353 * t1112 
     #/ 0.4E1 + 0.7E1 / 0.5760E4 * t318 * t1115 + t1118 * t1124 - t1145 
     #- t1182
        t1188 = (t363 - t402 - t395 + t475) * t21 - dx * t1032 / 0.24E2
        t1189 = t319 * t1188
        t1192 = dx * t826
        t1196 = t309 / 0.2E1 + t313 / 0.2E1
        t1197 = dx * t1196
        t1199 = t303 * t1197 / 0.24E2
        t1200 = dx * t631
        t1202 = t353 * t1200 / 0.48E2
        t1204 = u(t985,j,n) - t16
        t1208 = (t1204 * t21 - t661) * t21 - t664
        t1212 = (t1208 * t14 * t21 - t670) * t21
        t1216 = (t1204 * t14 * t21 - t22) * t21
        t1220 = ((t1216 - t29) * t21 - t680) * t21
        t1223 = t319 * (t1212 + t1220) / 0.24E2
        t1224 = u(t17,t91,n)
        t1225 = t1224 - t31
        t1227 = t32 * t34
        t1230 = t38 * t34
        t1232 = (t1227 - t1230) * t34
        t1236 = u(t17,t109,n)
        t1237 = t37 - t1236
        t1249 = (t1225 * t14 * t34 - t35) * t34
        t1255 = (-t1237 * t14 * t34 + t40) * t34
        t1262 = t413 * ((t14 * ((t1225 * t34 - t1227) * t34 - t1232) * t
     #34 - t14 * (t1232 - (-t1237 * t34 + t1230) * t34) * t34) * t34 + (
     #(t1249 - t42) * t34 - (t42 - t1255) * t34) * t34) / 0.24E2
        t1268 = u(t15,t30,n)
        t1272 = u(t15,t36,n)
        t1277 = (t14 * (t1268 - t16) * t34 - t14 * (t16 - t1272) * t34) 
     #* t34
        t1278 = t1216 + t1277 - t29 - t42
        t1291 = (t1278 * t14 * t21 - t60) * t21
        t1297 = t1268 - t31
        t1301 = (t1297 * t21 - t833) * t21 - t836
        t1305 = (t1301 * t14 * t21 - t842) * t21
        t1309 = (t1297 * t14 * t21 - t85) * t21
        t1313 = ((t1309 - t90) * t21 - t852) * t21
        t1316 = t319 * (t1305 + t1313) / 0.24E2
        t1317 = u(t23,t249,n)
        t1318 = t1317 - t92
        t1322 = (t1318 * t34 - t688) * t34 - t691
        t1326 = (t1322 * t14 * t34 - t697) * t34
        t1330 = (t1318 * t14 * t34 - t95) * t34
        t1334 = ((t1330 - t97) * t34 - t707) * t34
        t1337 = t413 * (t1326 + t1334) / 0.24E2
        t1341 = t1272 - t37
        t1345 = (t1341 * t21 - t878) * t21 - t881
        t1349 = (t1345 * t14 * t21 - t887) * t21
        t1353 = (t1341 * t14 * t21 - t103) * t21
        t1357 = ((t1353 - t108) * t21 - t897) * t21
        t1360 = t319 * (t1349 + t1357) / 0.24E2
        t1361 = u(t23,t280,n)
        t1362 = t110 - t1361
        t1366 = t700 - (-t1362 * t34 + t698) * t34
        t1370 = (-t1366 * t14 * t34 + t703) * t34
        t1374 = (-t1362 * t14 * t34 + t113) * t34
        t1378 = (t709 - (t115 - t1374) * t34) * t34
        t1381 = t413 * (t1370 + t1378) / 0.24E2
        t1387 = t1224 - t92
        t1391 = (t1387 * t14 * t21 - t243) * t21
        t1392 = t1391 + t1330 - t90 - t97
        t1394 = t98 * t34
        t1397 = t116 * t34
        t1399 = (t1394 - t1397) * t34
        t1403 = t1236 - t110
        t1407 = (t14 * t1403 * t21 - t274) * t21
        t1408 = t108 + t115 - t1407 - t1374
        t1421 = (t1392 * t14 * t34 - t100) * t34
        t1427 = (-t14 * t1408 * t34 + t118) * t34
        t1433 = (t14 * (t29 - t1223 + t42 - t1262 - t47 + t687 - t57 + t
     #714) * t21 - t759) * t21 - dx * (t14 * ((t1278 * t21 - t807) * t21
     # - t810) * t21 - t816) / 0.24E2 - dx * ((t1291 - t82) * t21 - t827
     #) / 0.24E2 + (t14 * (t90 - t1316 + t97 - t1337 - t47 + t687 - t57 
     #+ t714) * t34 - t14 * (t47 - t687 + t57 - t714 - t108 + t1360 - t1
     #15 + t1381) * t34) * t34 - dy * (t14 * ((t1392 * t34 - t1394) * t3
     #4 - t1399) * t34 - t14 * (t1399 - (-t1408 * t34 + t1397) * t34) * 
     #t34) / 0.24E2 - dy * ((t1421 - t120) * t34 - (t120 - t1427) * t34)
     # / 0.24E2
        t1434 = cc * t1433
        t1438 = t320 * t1169 / 0.1440E4
        t1439 = t14 * t8
        t1441 = t11 * t173 * t21
        t1444 = t14 * t9
        t1445 = t389 + t614 - t363 - t412
        t1447 = t14 * t1445 * t21
        t1448 = t363 + t412 - t395 - t485
        t1450 = t14 * t1448 * t21
        t1452 = (t1447 - t1450) * t21
        t1453 = t605 - t403
        t1455 = t14 * t1453 * t21
        t1456 = t403 - t476
        t1458 = t14 * t1456 * t21
        t1460 = (t1455 - t1458) * t21
        t1464 = t609 - t407
        t1466 = t14 * t1464 * t21
        t1467 = t407 - t480
        t1469 = t14 * t1467 * t21
        t1471 = (t1466 - t1469) * t21
        t1476 = (t14 * (t1460 + t439 - t363 - t412) * t34 - t14 * (t363 
     #+ t412 - t1471 - t445) * t34) * t34
        t1477 = t395 + t485 - t468 - t560
        t1479 = t14 * t1477 * t21
        t1481 = (t1450 - t1479) * t21
        t1482 = t476 - t551
        t1484 = t14 * t1482 * t21
        t1486 = (t1458 - t1484) * t21
        t1487 = t1486 + t511 - t395 - t485
        t1489 = t14 * t1487 * t34
        t1490 = t480 - t555
        t1492 = t14 * t1490 * t21
        t1494 = (t1469 - t1492) * t21
        t1495 = t395 + t485 - t1494 - t517
        t1497 = t14 * t1495 * t34
        t1499 = (t1489 - t1497) * t34
        t1500 = t1452 + t1476 - t1481 - t1499
        t1502 = t12 * t1500 * t21
        t1508 = ut(t17,t91,n)
        t1509 = t1508 - t605
        t1511 = t606 * t34
        t1514 = t610 * t34
        t1516 = (t1511 - t1514) * t34
        t1520 = ut(t17,t109,n)
        t1521 = t609 - t1520
        t1533 = (t14 * t1509 * t34 - t608) * t34
        t1539 = (-t14 * t1521 * t34 + t612) * t34
        t1552 = ut(t15,t30,n)
        t1556 = ut(t15,t36,n)
        t1561 = (t14 * (t1552 - t364) * t34 - t14 * (t364 - t1556) * t34
     #) * t34
        t1574 = (cc * (t389 - t319 * (t995 + t1029) / 0.24E2 + t614 - t4
     #13 * ((t14 * ((t1509 * t34 - t1511) * t34 - t1516) * t34 - t14 * (
     #t1516 - (-t1521 * t34 + t1514) * t34) * t34) * t34 + ((t1533 - t61
     #4) * t34 - (t614 - t1539) * t34) * t34) / 0.24E2) - t454) * t21 / 
     #0.2E1 + t529 - t319 * ((((cc * (t1025 + t1561) - t616) * t21 - t62
     #0) * t21 - t626) * t21 / 0.2E1 + t634 / 0.2E1) / 0.6E1
        t1575 = dx * t1574
        t1578 = t8 * t11
        t1579 = t1452 + t1476
        t1580 = cc * t1579
        t1581 = t1481 + t1499
        t1582 = cc * t1581
        t1584 = (t1580 - t1582) * t21
        t1585 = t468 + t560 - t543 - t644
        t1587 = t14 * t1585 * t21
        t1589 = (t1479 - t1587) * t21
        t1590 = t551 - t635
        t1592 = t14 * t1590 * t21
        t1594 = (t1484 - t1592) * t21
        t1598 = t555 - t639
        t1600 = t14 * t1598 * t21
        t1602 = (t1492 - t1600) * t21
        t1607 = (t14 * (t1594 + t586 - t468 - t560) * t34 - t14 * (t468 
     #+ t560 - t1602 - t592) * t34) * t34
        t1608 = t1589 + t1607
        t1609 = cc * t1608
        t1611 = (t1582 - t1609) * t21
        t1613 = t1584 / 0.2E1 + t1611 / 0.2E1
        t1614 = dx * t1613
        t1616 = t1578 * t1614 / 0.96E2
        t1622 = (t867 - t747) * t34
        t1624 = (t747 - t912) * t34
        t1628 = t863 * t34
        t1629 = t737 * t34
        t1631 = (t1628 - t1629) * t34
        t1632 = t743 * t34
        t1634 = (t1629 - t1632) * t34
        t1635 = t1631 - t1634
        t1637 = t14 * t1635 * t34
        t1638 = t908 * t34
        t1640 = (t1632 - t1638) * t34
        t1641 = t1634 - t1640
        t1643 = t14 * t1641 * t34
        t1647 = t871 - t753
        t1648 = t1647 * t34
        t1649 = t753 - t916
        t1650 = t1649 * t34
        t1659 = (t678 - t722) * t21
        t1661 = (t722 - t767) * t21
        t1665 = t668 * t21
        t1666 = t674 * t21
        t1668 = (t1665 - t1666) * t21
        t1669 = t718 * t21
        t1671 = (t1666 - t1669) * t21
        t1672 = t1668 - t1671
        t1674 = t14 * t1672 * t21
        t1675 = t763 * t21
        t1677 = (t1669 - t1675) * t21
        t1678 = t1671 - t1677
        t1680 = t14 * t1678 * t21
        t1684 = t684 - t726
        t1685 = t1684 * t21
        t1686 = t726 - t771
        t1687 = t1686 * t21
        t1691 = -dy * t746 / 0.24E2 - dy * t752 / 0.24E2 + t1037 * (t162
     #2 - t1624) / 0.576E3 + 0.3E1 / 0.640E3 * t1037 * (t1637 - t1643) +
     # t77 + 0.3E1 / 0.640E3 * t1037 * (t1648 - t1650) - dx * t721 / 0.2
     #4E2 - dx * t725 / 0.24E2 + t320 * (t1659 - t1661) / 0.576E3 + 0.3E
     #1 / 0.640E3 * t320 * (t1674 - t1680) + 0.3E1 / 0.640E3 * t320 * (t
     #1685 - t1687) + t67
        t1692 = cc * t1691
        t1694 = t318 * t1692 / 0.2E1
        t1696 = 0.7E1 / 0.5760E4 * t320 * t1684
        t1697 = -t318 * t1189 / 0.24E2 - t353 * t1192 / 0.48E2 - t1199 -
     # t1202 + t303 * t1434 / 0.12E2 - t1438 + t1439 * t1441 / 0.24E2 + 
     #t1444 * t1502 / 0.120E3 - t353 * t1575 / 0.8E1 - t1616 - t1694 + t
     #1696
        t1699 = t14 * t301
        t1703 = t1448 * t21
        t1706 = t1477 * t21
        t1708 = (t1703 - t1706) * t21
        t1712 = (t363 - t402 + t412 - t452 - t395 + t475 - t485 + t524) 
     #* t21 - dx * ((t1445 * t21 - t1703) * t21 - t1708) / 0.24E2
        t1713 = t302 * t1712
        t1723 = (t14 * (t1309 + t1249 - t29 - t42) * t34 - t14 * (t29 + 
     #t42 - t1353 - t1255) * t34) * t34
        t1727 = (cc * (t1291 + t1723) - t305) * t21
        t1729 = t1727 / 0.2E1 + t309 / 0.2E1
        t1730 = dx * t1729
        t1733 = dx * t625
        t1736 = t1452 - t1481
        t1737 = dx * t1736
        t1740 = t14 * t7
        t1744 = t757 * t21 - dx * t814 / 0.24E2
        t1745 = t10 * t1744
        t1750 = cc * t530
        t1752 = (-t1750 + t971) * t21
        t1754 = (t973 - t1752) * t21
        t1756 = (t975 - t1754) * t21
        t1757 = t977 - t1756
        t1758 = t1757 * t21
        t1760 = (t1173 - t1758) * t21
        t1766 = t319 * (t968 - dx * t978 / 0.12E2 + t320 * (t1175 - t176
     #0) / 0.90E2) / 0.24E2
        t1771 = (t1131 - t1136) * t21 - dx * t349 / 0.12E2
        t1772 = t319 * t1771
        t1774 = t318 * t1772 / 0.24E2
        t1782 = t319 * (t963 - dx * t1169 / 0.12E2 + t320 * (t1172 - t11
     #75) / 0.90E2) / 0.24E2
        t1807 = t695 * t34
        t1810 = t701 * t34
        t1812 = (t1807 - t1810) * t34
        t1843 = 0.3E1 / 0.640E3 * t320 * ((t1220 - t684) * t21 - t1685) 
     #+ t47 - dx * t677 / 0.24E2 - dx * t683 / 0.24E2 + t320 * ((t1212 -
     # t678) * t21 - t1659) / 0.576E3 + 0.3E1 / 0.640E3 * t320 * (t14 * 
     #((t1208 * t21 - t1665) * t21 - t1668) * t21 - t1674) + 0.3E1 / 0.6
     #40E3 * t1037 * (t14 * ((t1322 * t34 - t1807) * t34 - t1812) * t34 
     #- t14 * (t1812 - (-t1366 * t34 + t1810) * t34) * t34) + 0.3E1 / 0.
     #640E3 * t1037 * ((t1334 - t711) * t34 - (t711 - t1378) * t34) - dy
     # * t704 / 0.24E2 - dy * t710 / 0.24E2 + t1037 * ((t1326 - t705) * 
     #t34 - (t705 - t1370) * t34) / 0.576E3 + t57
        t1844 = cc * t1843
        t1854 = (((cc * (t1216 + t1277) - t322) * t21 - t326) * t21 - t3
     #32) * t21
        t1855 = t1854 - t340
        t1856 = t320 * t1855
        t1860 = t1578 * t1582 / 0.48E2
        t1861 = t1699 * t1713 / 0.6E1 - t303 * t1730 / 0.24E2 + t353 * t
     #1733 / 0.48E2 - t303 * t1737 / 0.288E3 + t1740 * t1745 / 0.2E1 - t
     #1766 - t1774 + t1782 + t318 * t1844 / 0.2E1 - t318 * t1856 / 0.144
     #0E4 - t1860
        t1868 = t319 * ((t47 - t687 - t67 + t729) * t21 - dx * t1684 / 0
     #.24E2) / 0.24E2
        t1874 = t14 * (t665 - dx * t674 / 0.24E2 + 0.3E1 / 0.640E3 * t32
     #0 * t1672)
        t1875 = t966 / 0.2E1
        t1886 = dx * (t1147 + t1875 - t319 * (t970 / 0.2E1 + t977 / 0.2E
     #1) / 0.6E1 + t1159 * (t1175 / 0.2E1 + t1760 / 0.2E1) / 0.30E2) / 0
     #.4E1
        t1892 = t1552 - t605
        t1896 = (t14 * t1892 * t21 - t1455) * t21
        t1900 = t1556 - t609
        t1904 = (t14 * t1900 * t21 - t1466) * t21
        t1915 = (cc * ((t14 * (t1025 + t1561 - t389 - t614) * t21 - t144
     #7) * t21 + (t14 * (t1896 + t1533 - t389 - t614) * t34 - t14 * (t38
     #9 + t614 - t1904 - t1539) * t34) * t34) - t1580) * t21 / 0.2E1 + t
     #1584 / 0.2E1
        t1916 = dx * t1915
        t1924 = t1309 + t1249 - t90 - t97
        t1928 = (t14 * t1924 * t21 - t235) * t21
        t1932 = t1353 + t1255 - t108 - t115
        t1936 = (t14 * t1932 * t21 - t266) * t21
        t1942 = (t14 * (t1291 + t1723 - t82 - t120) * t21 - t175) * t21 
     #+ (t14 * (t1928 + t1421 - t82 - t120) * t34 - t14 * (t82 + t120 - 
     #t1936 - t1427) * t34) * t34
        t1943 = cc * t1942
        t1946 = t955 / 0.2E1
        t1947 = t959 / 0.2E1
        t1948 = t1727 - t309
        t1949 = dx * t1948
        t1955 = (cc * (t29 - t1223 + t42 - t1262) - t1127) * t21
        t1961 = t1955 / 0.2E1 + t1132 - t319 * (t1854 / 0.2E1 + t340 / 0
     #.2E1) / 0.6E1
        t1962 = dx * t1961
        t1966 = (t464 - t539) * t21
        t1970 = t535 * t21
        t1972 = (t1013 - t1970) * t21
        t1973 = t1015 - t1972
        t1975 = t14 * t1973 * t21
        t1979 = t472 - t547
        t1980 = t1979 * t21
        t1992 = ut(i,t249,n)
        t1993 = t1992 - t486
        t1994 = t1993 * t34
        t1996 = (t1994 - t488) * t34
        t1997 = t1996 - t491
        t1999 = t14 * t1997 * t34
        t2000 = t1999 - t497
        t2001 = t2000 * t34
        t2003 = (t2001 - t507) * t34
        t2004 = ut(i,t280,n)
        t2005 = t498 - t2004
        t2006 = t2005 * t34
        t2008 = (t500 - t2006) * t34
        t2009 = t502 - t2008
        t2011 = t14 * t2009 * t34
        t2012 = t505 - t2011
        t2013 = t2012 * t34
        t2015 = (t507 - t2013) * t34
        t2019 = t1997 * t34
        t2020 = t495 * t34
        t2022 = (t2019 - t2020) * t34
        t2023 = t503 * t34
        t2025 = (t2020 - t2023) * t34
        t2026 = t2022 - t2025
        t2028 = t14 * t2026 * t34
        t2029 = t2009 * t34
        t2031 = (t2023 - t2029) * t34
        t2032 = t2025 - t2031
        t2034 = t14 * t2032 * t34
        t2039 = t14 * t1993 * t34
        t2041 = (t2039 - t509) * t34
        t2043 = (t2041 - t511) * t34
        t2044 = t2043 - t513
        t2045 = t2044 * t34
        t2046 = t2045 - t521
        t2047 = t2046 * t34
        t2049 = t14 * t2005 * t34
        t2051 = (t515 - t2049) * t34
        t2053 = (t517 - t2051) * t34
        t2054 = t519 - t2053
        t2055 = t2054 * t34
        t2056 = t521 - t2055
        t2057 = t2056 * t34
        t2061 = t320 * (t999 - t1966) / 0.576E3 + 0.3E1 / 0.640E3 * t320
     # * (t1018 - t1975) + 0.3E1 / 0.640E3 * t320 * (t1033 - t1980) + t3
     #95 - dx * t463 / 0.24E2 - dx * t471 / 0.24E2 + t485 - dy * t506 / 
     #0.24E2 - dy * t520 / 0.24E2 + t1037 * (t2003 - t2015) / 0.576E3 + 
     #0.3E1 / 0.640E3 * t1037 * (t2028 - t2034) + 0.3E1 / 0.640E3 * t103
     #7 * (t2047 - t2057)
        t2062 = cc * t2061
        t2064 = t353 * t2062 / 0.4E1
        t2069 = (t1955 - t1131) * t21 - dx * t1855 / 0.12E2
        t2070 = t319 * t2069
        t2075 = -t1868 + t1874 - t1886 - t1578 * t1916 / 0.96E2 + t13 * 
     #t1943 / 0.240E3 + t1946 - t1947 + t303 * t1949 / 0.144E3 - t318 * 
     #t1962 / 0.4E1 - t2064 + t318 * t2070 / 0.24E2 + t1578 * t1580 / 0.
     #48E2
        t2077 = t1183 + t1697 + t1861 + t2075
        t2078 = dt / 0.2E1
        t2080 = 0.1E1 / (t318 - t2078)
        t2082 = 0.1E1 / 0.2E1 + t5
        t2083 = dt * t2082
        t2085 = 0.1E1 / (t318 - t2083)
        t2087 = t302 * dx
        t2089 = t2087 * t1196 / 0.192E3
        t2090 = t10 * cc
        t2093 = t12 * cc
        t2096 = dt * t319
        t2099 = dt * t320
        t2102 = t11 * dx
        t2105 = t11 * cc
        t2109 = t2102 * t1613 / 0.1536E4
        t2110 = t10 * dx
        t2112 = t2110 * t657 / 0.32E2
        t2113 = dt * cc
        t2116 = -t2089 + t2090 * t1111 / 0.16E2 + t2093 * t1942 / 0.7680
     #E4 - t2096 * t1188 / 0.48E2 + 0.7E1 / 0.11520E5 * t2099 * t1032 - 
     #t2102 * t1915 / 0.1536E4 + t2105 * t1579 / 0.768E3 - t2109 + t980 
     #- t2112 + t2113 * t1843 / 0.4E1
        t2120 = t2087 * t314 / 0.1152E4
        t2124 = t2113 * t1691 / 0.4E1
        t2128 = t2110 * t631 / 0.192E3
        t2129 = t302 * cc
        t2136 = dt * dx
        t2138 = t2136 * t1142 / 0.8E1
        t2139 = -t2110 * t1574 / 0.32E2 - t2120 - t2110 * t826 / 0.192E3
     # - t2124 + t2096 * t2069 / 0.48E2 - t1182 - t2128 + t2129 * t1433 
     #/ 0.96E2 - t2099 * t1855 / 0.2880E4 - t1438 + t2110 * t625 / 0.192
     #E3 - t2138
        t2145 = t14 * dt
        t2149 = t2093 * t297 / 0.7680E4
        t2150 = t14 * t10
        t2154 = t2129 * t951 / 0.96E2
        t2156 = t2090 * t2061 / 0.16E2
        t2157 = t1696 + t2087 * t1948 / 0.1152E4 - t2087 * t1736 / 0.230
     #4E4 - t1766 + t2145 * t1123 / 0.2E1 - t2149 + t2150 * t1744 / 0.8E
     #1 + t1782 - t2154 - t2156 - t1868
        t2159 = t2096 * t1771 / 0.48E2
        t2161 = t2105 * t1581 / 0.768E3
        t2162 = t14 * t302
        t2167 = t14 * t12
        t2173 = t14 * t11
        t2178 = t2099 * t349 / 0.2880E4
        t2179 = t1874 - t2159 - t2161 + t2162 * t1712 / 0.48E2 - t2087 *
     # t1729 / 0.192E3 + t2167 * t1500 * t21 / 0.3840E4 - t1886 + t1946 
     #- t1947 - t2136 * t1961 / 0.8E1 + t2173 * t173 * t21 / 0.384E3 + t
     #2178
        t2181 = t2116 + t2139 + t2157 + t2179
        t2183 = -t2080
        t2186 = 0.1E1 / (t2078 - t2083)
        t2188 = t2082 ** 2
        t2189 = t2188 * t10
        t2194 = t14 * t2188
        t2197 = t2188 * t2082
        t2198 = t14 * t2197
        t2203 = t2197 * t302
        t2205 = t2203 * t952 / 0.12E2
        t2207 = t2189 * t658 / 0.8E1
        t2210 = t980 - t2189 * t1575 / 0.8E1 + t2189 * t1112 / 0.4E1 + t
     #2194 * t1745 / 0.2E1 + t2198 * t1713 / 0.6E1 - t2189 * t1192 / 0.4
     #8E2 - t1182 - t2205 - t2207 - t2083 * t1962 / 0.4E1 - t1438
        t2211 = t2188 ** 2
        t2212 = t2211 * t2082
        t2213 = t2212 * t12
        t2216 = t14 * t2211
        t2219 = t14 * t2212
        t2222 = t2211 * t11
        t2226 = t2083 * t350 / 0.1440E4
        t2235 = t14 * t2082
        t2239 = t2213 * t1943 / 0.240E3 + t2216 * t1441 / 0.24E2 + t2219
     # * t1502 / 0.120E3 - t2222 * t1916 / 0.96E2 + t2226 + 0.7E1 / 0.57
     #60E4 * t2083 * t1115 - t2203 * t1737 / 0.288E3 - t2203 * t1730 / 0
     #.24E2 + t2189 * t1733 / 0.48E2 + t1696 + t2235 * t1124 - t2083 * t
     #1189 / 0.24E2
        t2242 = t2222 * t1614 / 0.96E2
        t2244 = t2203 * t315 / 0.144E3
        t2246 = t2083 * t1772 / 0.24E2
        t2248 = t2222 * t1582 / 0.48E2
        t2250 = t2213 * t298 / 0.240E3
        t2252 = t2189 * t2062 / 0.4E1
        t2254 = t2083 * t1692 / 0.2E1
        t2261 = -t2242 - t2244 - t2246 - t2248 - t2250 - t2252 - t2254 +
     # t2203 * t1949 / 0.144E3 - t2083 * t1856 / 0.1440E4 - t1766 + t220
     #3 * t1434 / 0.12E2
        t2265 = t2083 * t1143 / 0.4E1
        t2267 = t2203 * t1197 / 0.24E2
        t2269 = t2189 * t1200 / 0.48E2
        t2274 = t1782 - t1868 + t1874 - t1886 + t2083 * t1844 / 0.2E1 + 
     #t1946 - t1947 - t2265 - t2267 - t2269 + t2083 * t2070 / 0.24E2 + t
     #2222 * t1580 / 0.48E2
        t2276 = t2210 + t2239 + t2261 + t2274
        t2278 = -t2085
        t2281 = -t2186
        t2283 = t2077 * t2080 * t2085 + t2181 * t2183 * t2186 + t2276 * 
     #t2278 * t2281
        t2287 = dt * t2077
        t2293 = dt * t2181
        t2299 = dt * t2276
        t2305 = (-t2287 / 0.2E1 - t2287 * t2082) * t2080 * t2085 + (-t20
     #82 * t2293 - t2293 * t6) * t2183 * t2186 + (-t2299 * t6 - t2299 / 
     #0.2E1) * t2278 * t2281
        t2311 = t2082 * t2080 * t2085
        t2321 = t6 * t2278 * t2281
        t2329 = t320 * t1979
        t2332 = i - 4
        t2334 = t177 - u(t2332,j,n)
        t2338 = (-t2334 * t983 + t180) * t21
        t2342 = (t769 - (t182 - t2338) * t21) * t21
        t2355 = t762 - (-t21 * t2334 + t760) * t21
        t2359 = (-t2355 * t983 + t765) * t21
        t2374 = u(t61,t249,n)
        t2375 = t2374 - t203
        t2379 = (t2375 * t34 - t775) * t34 - t778
        t2381 = t782 * t34
        t2384 = t788 * t34
        t2386 = (t2381 - t2384) * t34
        t2390 = u(t61,t280,n)
        t2391 = t217 - t2390
        t2395 = t787 - (-t2391 * t34 + t785) * t34
        t2408 = (t14 * t2375 * t34 - t206) * t34
        t2412 = ((t2408 - t208) * t34 - t794) * t34
        t2418 = (-t14 * t2391 * t34 + t220) * t34
        t2422 = (t796 - (t222 - t2418) * t34) * t34
        t2435 = (t14 * t2379 * t34 - t784) * t34
        t2441 = (-t14 * t2395 * t34 + t790) * t34
        t2447 = 0.3E1 / 0.640E3 * t320 * (t1687 - (t771 - t2342) * t21) 
     #+ t127 - dx * t766 / 0.24E2 - dx * t770 / 0.24E2 + t320 * (t1661 -
     # (t767 - t2359) * t21) / 0.576E3 + 0.3E1 / 0.640E3 * t320 * (t1680
     # - t14 * (t1677 - (-t21 * t2355 + t1675) * t21) * t21) + 0.3E1 / 0
     #.640E3 * t1037 * (t14 * ((t2379 * t34 - t2381) * t34 - t2386) * t3
     #4 - t14 * (t2386 - (-t2395 * t34 + t2384) * t34) * t34) + 0.3E1 / 
     #0.640E3 * t1037 * ((t2412 - t798) * t34 - (t798 - t2422) * t34) - 
     #dy * t791 / 0.24E2 - dy * t797 / 0.24E2 + t1037 * ((t2435 - t792) 
     #* t34 - (t792 - t2441) * t34) / 0.576E3 + t137
        t2448 = cc * t2447
        t2451 = t300 + t317 - t352 - t660 + t954 - t980 - t1578 * t1609 
     #/ 0.48E2 - t1145 + 0.7E1 / 0.5760E4 * t318 * t2329 - t318 * t2448 
     #/ 0.2E1 - t1199
        t2452 = ut(t2332,j,n)
        t2453 = t530 - t2452
        t2457 = (-t2453 * t983 + t541) * t21
        t2458 = ut(t176,t30,n)
        t2462 = ut(t176,t36,n)
        t2467 = (t14 * (t2458 - t530) * t34 - t14 * (t530 - t2462) * t34
     #) * t34
        t2473 = t635 - t2458
        t2477 = (-t2473 * t983 + t1592) * t21
        t2478 = ut(t121,t91,n)
        t2479 = t2478 - t635
        t2483 = (t14 * t2479 * t34 - t638) * t34
        t2487 = t639 - t2462
        t2491 = (-t2487 * t983 + t1600) * t21
        t2492 = ut(t121,t109,n)
        t2493 = t639 - t2492
        t2497 = (-t14 * t2493 * t34 + t642) * t34
        t2508 = t1611 / 0.2E1 + (t1609 - cc * ((t1587 - t14 * (t543 + t6
     #44 - t2457 - t2467) * t21) * t21 + (t14 * (t2477 + t2483 - t543 - 
     #t644) * t34 - t14 * (t543 + t644 - t2491 - t2497) * t34) * t34)) *
     # t21 / 0.2E1
        t2509 = dx * t2508
        t2519 = t534 - (-t21 * t2453 + t532) * t21
        t2523 = (-t2519 * t983 + t537) * t21
        t2541 = (t545 - (t543 - t2457) * t21) * t21
        t2547 = ut(t61,t249,n)
        t2548 = t2547 - t561
        t2552 = (t2548 * t34 - t563) * t34 - t566
        t2554 = t570 * t34
        t2557 = t578 * t34
        t2559 = (t2554 - t2557) * t34
        t2563 = ut(t61,t280,n)
        t2564 = t573 - t2563
        t2568 = t577 - (-t2564 * t34 + t575) * t34
        t2581 = (t14 * t2548 * t34 - t584) * t34
        t2591 = (-t14 * t2564 * t34 + t590) * t34
        t2620 = -dx * t538 / 0.24E2 - dx * t546 / 0.24E2 + t320 * (t1966
     # - (t539 - t2523) * t21) / 0.576E3 + 0.3E1 / 0.640E3 * t320 * (t19
     #75 - t14 * (t1972 - (-t21 * t2519 + t1970) * t21) * t21) + 0.3E1 /
     # 0.640E3 * t320 * (t1980 - (t547 - t2541) * t21) + t468 + 0.3E1 / 
     #0.640E3 * t1037 * (t14 * ((t2552 * t34 - t2554) * t34 - t2559) * t
     #34 - t14 * (t2559 - (-t2568 * t34 + t2557) * t34) * t34) + 0.3E1 /
     # 0.640E3 * t1037 * ((((t2581 - t586) * t34 - t588) * t34 - t596) *
     # t34 - (t596 - (t594 - (t592 - t2591) * t34) * t34) * t34) - dy * 
     #t581 / 0.24E2 - dy * t595 / 0.24E2 + t1037 * (((t14 * t2552 * t34 
     #- t572) * t34 - t582) * t34 - (t582 - (-t14 * t2568 * t34 + t580) 
     #* t34) * t34) / 0.576E3 + t560
        t2621 = cc * t2620
        t2624 = u(t176,t30,n)
        t2628 = u(t176,t36,n)
        t2633 = (t14 * (t2624 - t177) * t34 - t14 * (t177 - t2628) * t34
     #) * t34
        t2634 = t182 + t192 - t2338 - t2633
        t2638 = (-t2634 * t983 + t195) * t21
        t2639 = t183 - t2624
        t2643 = (-t2639 * t983 + t200) * t21
        t2644 = u(t121,t91,n)
        t2645 = t2644 - t183
        t2649 = (t14 * t2645 * t34 - t186) * t34
        t2653 = t187 - t2628
        t2657 = (-t2653 * t983 + t214) * t21
        t2658 = u(t121,t109,n)
        t2659 = t187 - t2658
        t2663 = (-t14 * t2659 * t34 + t190) * t34
        t2668 = (t14 * (t2643 + t2649 - t182 - t192) * t34 - t14 * (t182
     # + t192 - t2657 - t2663) * t34) * t34
        t2672 = (t311 - cc * (t2638 + t2668)) * t21
        t2674 = t313 / 0.2E1 + t2672 / 0.2E1
        t2675 = dx * t2674
        t2678 = dx * t649
        t2684 = t802 * t21 - dx * t820 / 0.24E2
        t2685 = t10 * t2684
        t2690 = t319 * (t2359 + t2342) / 0.24E2
        t2692 = t184 * t34
        t2695 = t188 * t34
        t2697 = (t2692 - t2695) * t34
        t2717 = t413 * ((t14 * ((t2645 * t34 - t2692) * t34 - t2697) * t
     #34 - t14 * (t2697 - (-t2659 * t34 + t2695) * t34) * t34) * t34 + (
     #(t2649 - t192) * t34 - (t192 - t2663) * t34) * t34) / 0.24E2
        t2740 = t845 - (-t21 * t2639 + t843) * t21
        t2744 = (-t2740 * t983 + t848) * t21
        t2748 = (t854 - (t202 - t2643) * t21) * t21
        t2751 = t319 * (t2744 + t2748) / 0.24E2
        t2754 = t413 * (t2435 + t2412) / 0.24E2
        t2761 = t890 - (-t21 * t2653 + t888) * t21
        t2765 = (-t2761 * t983 + t893) * t21
        t2769 = (t899 - (t216 - t2657) * t21) * t21
        t2772 = t319 * (t2765 + t2769) / 0.24E2
        t2775 = t413 * (t2441 + t2422) / 0.24E2
        t2781 = t203 - t2644
        t2785 = (-t2781 * t983 + t246) * t21
        t2786 = t2785 + t2408 - t202 - t208
        t2788 = t209 * t34
        t2791 = t223 * t34
        t2793 = (t2788 - t2791) * t34
        t2797 = t217 - t2658
        t2801 = (-t2797 * t983 + t277) * t21
        t2802 = t216 + t222 - t2801 - t2418
        t2815 = (t14 * t2786 * t34 - t211) * t34
        t2821 = (-t14 * t2802 * t34 + t225) * t34
        t2827 = (t804 - t14 * (t127 - t774 + t137 - t801 - t182 + t2690 
     #- t192 + t2717) * t21) * t21 - dx * (t822 - t14 * (t819 - (-t21 * 
     #t2634 + t817) * t21) * t21) / 0.24E2 - dx * (t829 - (t197 - t2638)
     # * t21) / 0.24E2 + (t14 * (t202 - t2751 + t208 - t2754 - t127 + t7
     #74 - t137 + t801) * t34 - t14 * (t127 - t774 + t137 - t801 - t216 
     #+ t2772 - t222 + t2775) * t34) * t34 - dy * (t14 * ((t2786 * t34 -
     # t2788) * t34 - t2793) * t34 - t14 * (t2793 - (-t2802 * t34 + t279
     #1) * t34) * t34) / 0.24E2 - dy * ((t2815 - t227) * t34 - (t227 - t
     #2821) * t34) / 0.24E2
        t2828 = cc * t2827
        t2838 = (t346 - (t344 - (t342 - cc * (t2338 + t2633)) * t21) * t
     #21) * t21
        t2839 = t348 - t2838
        t2840 = t320 * t2839
        t2847 = t378 - dx * t460 / 0.24E2 + 0.3E1 / 0.640E3 * t320 * t19
     #73
        t2848 = dt * t2847
        t2854 = (t395 - t475 - t468 + t550) * t21 - dx * t1979 / 0.24E2
        t2855 = t319 * t2854
        t2858 = t1202 - t1578 * t2509 / 0.96E2 - t1616 + t1694 - t353 * 
     #t2621 / 0.4E1 - t303 * t2675 / 0.24E2 - t353 * t2678 / 0.48E2 + t1
     #740 * t2685 / 0.2E1 - t303 * t2828 / 0.12E2 + t318 * t2840 / 0.144
     #0E4 + t1118 * t2848 - t318 * t2855 / 0.24E2
        t2863 = (t1134 - cc * (t182 - t2690 + t192 - t2717)) * t21
        t2868 = (t1136 - t2863) * t21 - dx * t2839 / 0.12E2
        t2869 = t319 * t2868
        t2872 = t1481 - t1589
        t2873 = dx * t2872
        t2876 = dx * t828
        t2884 = t1137 + t2863 / 0.2E1 - t319 * (t348 / 0.2E1 + t2838 / 0
     #.2E1) / 0.6E1
        t2885 = dx * t2884
        t2889 = t11 * t228 * t21
        t2892 = t1481 + t1499 - t1589 - t1607
        t2894 = t12 * t2892 * t21
        t2897 = t1766 + t1774 - t318 * t2869 / 0.24E2 - t303 * t2873 / 0
     #.288E3 - t353 * t2876 / 0.48E2 + t1860 - t1886 - t318 * t2885 / 0.
     #4E1 + t1947 + t1439 * t2889 / 0.24E2 + t1444 * t2894 / 0.120E3
        t2903 = t202 + t208 - t2643 - t2649
        t2907 = (-t2903 * t983 + t238) * t21
        t2911 = t216 + t222 - t2657 - t2663
        t2915 = (-t2911 * t983 + t269) * t21
        t2921 = (t230 - t14 * (t197 + t227 - t2638 - t2668) * t21) * t21
     # + (t14 * (t2907 + t2815 - t197 - t227) * t34 - t14 * (t197 + t227
     # - t2915 - t2821) * t34) * t34
        t2922 = cc * t2921
        t2933 = (t395 - t475 + t485 - t524 - t468 + t550 - t560 + t599) 
     #* t21 - dx * (t1708 - (-t1585 * t21 + t1706) * t21) / 0.24E2
        t2934 = t302 * t2933
        t2941 = t636 * t34
        t2944 = t640 * t34
        t2946 = (t2941 - t2944) * t34
        t2984 = t604 + (t601 - cc * (t543 - t319 * (t2523 + t2541) / 0.2
     #4E2 + t644 - t413 * ((t14 * ((t2479 * t34 - t2941) * t34 - t2946) 
     #* t34 - t14 * (t2946 - (-t2493 * t34 + t2944) * t34) * t34) * t34 
     #+ ((t2483 - t644) * t34 - (t644 - t2497) * t34) * t34) / 0.24E2)) 
     #* t21 / 0.2E1 - t319 * (t652 / 0.2E1 + (t650 - (t648 - (t646 - cc 
     #* (t2457 + t2467)) * t21) * t21) * t21 / 0.2E1) / 0.6E1
        t2985 = dx * t2984
        t2993 = t14 * (t671 - dx * t718 / 0.24E2 + 0.3E1 / 0.640E3 * t32
     #0 * t1678)
        t3000 = t319 * ((t67 - t729 - t127 + t774) * t21 - dx * t1686 / 
     #0.24E2) / 0.24E2
        t3013 = (t1758 - (t1756 - (t1754 - (t1752 - (-cc * t2452 + t1750
     #) * t21) * t21) * t21) * t21) * t21
        t3019 = t319 * (t975 - dx * t1757 / 0.12E2 + t320 * (t1760 - t30
     #13) / 0.90E2) / 0.24E2
        t3031 = dx * (t1875 + t973 / 0.2E1 - t319 * (t977 / 0.2E1 + t175
     #6 / 0.2E1) / 0.6E1 + t1159 * (t1760 / 0.2E1 + t3013 / 0.2E1) / 0.3
     #0E2) / 0.4E1
        t3033 = 0.7E1 / 0.5760E4 * t320 * t1686
        t3035 = t320 * t1757 / 0.1440E4
        t3036 = t964 / 0.2E1
        t3037 = t313 - t2672
        t3038 = dx * t3037
        t3041 = -t13 * t2922 / 0.240E3 + t1699 * t2934 / 0.6E1 - t353 * 
     #t2985 / 0.8E1 + t2993 - t3000 - t3019 - t3031 + t3033 + t3035 - t3
     #036 + t2064 - t303 * t3038 / 0.144E3
        t3043 = t2451 + t2858 + t2897 + t3041
        t3053 = -t2089 - t2109 - t980 - t2112 + t2120 + t2124 + t2128 - 
     #t2138 + t2167 * t2892 * t21 / 0.3840E4 - t2102 * t2508 / 0.1536E4 
     #- t2087 * t3037 / 0.1152E4
        t3063 = t2099 * t2839 / 0.2880E4 - t2129 * t2827 / 0.96E2 + t176
     #6 + t2149 + t2154 + t2156 + t2159 + t2161 + t2173 * t228 * t21 / 0
     #.384E3 - t1886 + t1947 - t2090 * t2620 / 0.16E2
        t3087 = -t2113 * t2447 / 0.4E1 - t2110 * t2984 / 0.32E2 - t2096 
     #* t2868 / 0.48E2 - t2105 * t1608 / 0.768E3 - t2087 * t2674 / 0.192
     #E3 - t2093 * t2921 / 0.7680E4 + t2150 * t2684 / 0.8E1 - t2110 * t6
     #49 / 0.192E3 - t2087 * t2872 / 0.2304E4 + t2162 * t2933 / 0.48E2 +
     # t2145 * t2847 / 0.2E1
        t3096 = 0.7E1 / 0.11520E5 * t2099 * t1979 - t2096 * t2854 / 0.48
     #E2 - t2110 * t828 / 0.192E3 - t2178 + t2993 - t3000 - t3019 - t303
     #1 + t3033 + t3035 - t3036 - t2136 * t2884 / 0.8E1
        t3098 = t3053 + t3063 + t3087 + t3096
        t3117 = -t2083 * t2869 / 0.24E2 - t980 - t2189 * t2985 / 0.8E1 -
     # t2203 * t2828 / 0.12E2 - t2203 * t3038 / 0.144E3 + t2083 * t2840 
     #/ 0.1440E4 + t2219 * t2894 / 0.120E3 - t2189 * t2621 / 0.4E1 + t22
     #05 - t2207 - t2213 * t2922 / 0.240E3
        t3129 = 0.7E1 / 0.5760E4 * t2083 * t2329 - t2083 * t2855 / 0.24E
     #2 + t2235 * t2848 - t2226 - t2083 * t2448 / 0.2E1 - t2242 + t2244 
     #- t2222 * t1609 / 0.48E2 - t2203 * t2873 / 0.288E3 + t2246 + t2248
     # + t2250
        t3141 = t2252 + t2254 + t1766 + t2194 * t2685 / 0.2E1 + t2198 * 
     #t2934 / 0.6E1 - t1886 - t2083 * t2885 / 0.4E1 + t1947 - t2189 * t2
     #876 / 0.48E2 + t2216 * t2889 / 0.24E2 - t2265
        t3148 = -t2267 + t2269 - t2222 * t2509 / 0.96E2 - t2203 * t2675 
     #/ 0.24E2 - t2189 * t2678 / 0.48E2 + t2993 - t3000 - t3019 - t3031 
     #+ t3033 + t3035 - t3036
        t3150 = t3117 + t3129 + t3141 + t3148
        t3090 = t2080 * t2085
        t3092 = t2183 * t2186
        t3094 = t2278 * t2281
        t3153 = t3043 * t3090 + t3092 * t3098 + t3094 * t3150
        t3157 = dt * t3043
        t3163 = dt * t3098
        t3169 = dt * t3150
        t3175 = (-t3157 / 0.2E1 - t3157 * t2082) * t2080 * t2085 + (-t20
     #82 * t3163 - t3163 * t6) * t2183 * t2186 + (-t3169 * t6 - t3169 / 
     #0.2E1) * t2278 * t2281
        t3208 = (t14 * (t1391 + t1330 - t248 - t255) * t21 - t14 * (t248
     # + t255 - t2785 - t2408) * t21) * t21
        t3216 = (t14 * (t1317 - t250) * t21 - t14 * (t250 - t2374) * t21
     #) * t21
        t3217 = j + 4
        t3219 = u(i,t3217,n) - t250
        t3223 = (t14 * t3219 * t34 - t253) * t34
        t3224 = t3216 + t3223 - t248 - t255
        t3228 = (t14 * t3224 * t34 - t258) * t34
        t3234 = (t14 * (t1928 + t1421 - t240 - t260) * t21 - t14 * (t240
     # + t260 - t2907 - t2815) * t21) * t21 + (t14 * (t3208 + t3228 - t2
     #40 - t260) * t34 - t263) * t34
        t3235 = cc * t3234
        t3245 = (t14 * (t1460 + t439 - t1486 - t511) * t21 - t14 * (t148
     #6 + t511 - t1594 - t586) * t21) * t21
        t3246 = t414 - t486
        t3248 = t14 * t3246 * t21
        t3249 = t486 - t561
        t3251 = t14 * t3249 * t21
        t3253 = (t3248 - t3251) * t21
        t3254 = t3253 + t2041 - t1486 - t511
        t3256 = t14 * t3254 * t34
        t3258 = (t3256 - t1489) * t34
        t3259 = t3245 + t3258
        t3260 = cc * t3259
        t3262 = (t3260 - t1582) * t34
        t3270 = (t14 * (t1471 + t445 - t1494 - t517) * t21 - t14 * (t149
     #4 + t517 - t1602 - t592) * t21) * t21
        t3271 = t426 - t498
        t3273 = t14 * t3271 * t21
        t3274 = t498 - t573
        t3276 = t14 * t3274 * t21
        t3278 = (t3273 - t3276) * t21
        t3279 = t1494 + t517 - t3278 - t2051
        t3281 = t14 * t3279 * t34
        t3283 = (t1497 - t3281) * t34
        t3284 = t3270 + t3283
        t3285 = cc * t3284
        t3287 = (t1582 - t3285) * t34
        t3289 = t3262 / 0.2E1 + t3287 / 0.2E1
        t3290 = dy * t3289
        t3292 = t1578 * t3290 / 0.96E2
        t3303 = t840 * t21
        t3306 = t846 * t21
        t3308 = (t3303 - t3306) * t21
        t3331 = ((t3223 - t255) * t34 - t869) * t34
        t3346 = (t3219 * t34 - t860) * t34 - t862
        t3350 = (t14 * t3346 * t34 - t865) * t34
        t3365 = -dx * t855 / 0.24E2 + t320 * ((t1305 - t850) * t21 - (t8
     #50 - t2744) * t21) / 0.576E3 + 0.3E1 / 0.640E3 * t320 * (t14 * ((t
     #1301 * t21 - t3303) * t21 - t3308) * t21 - t14 * (t3308 - (-t21 * 
     #t2740 + t3306) * t21) * t21) + 0.3E1 / 0.640E3 * t320 * ((t1313 - 
     #t856) * t21 - (t856 - t2748) * t21) + t147 + 0.3E1 / 0.640E3 * t10
     #37 * ((t3331 - t871) * t34 - t1648) - dx * t849 / 0.24E2 - dy * t8
     #66 / 0.24E2 - dy * t870 / 0.24E2 + t1037 * ((t3350 - t867) * t34 -
     # t1622) / 0.576E3 + 0.3E1 / 0.640E3 * t1037 * (t14 * ((t3346 * t34
     # - t1628) * t34 - t1631) * t34 - t1637) + t153
        t3366 = cc * t3365
        t3369 = cc * t476
        t3370 = cc * t486
        t3372 = (-t3369 + t3370) * t34
        t3375 = (-t959 + t3369) * t34
        t3376 = t3375 / 0.2E1
        t3377 = cc * t1992
        t3379 = (-t3370 + t3377) * t34
        t3381 = (-t3372 + t3379) * t34
        t3383 = (t3372 - t3375) * t34
        t3385 = (t3381 - t3383) * t34
        t3386 = cc * t480
        t3388 = (t959 - t3386) * t34
        t3390 = (t3375 - t3388) * t34
        t3392 = (t3383 - t3390) * t34
        t3397 = t413 ** 2
        t3398 = ut(i,t3217,n)
        t3408 = t3385 - t3392
        t3409 = t3408 * t34
        t3411 = (((((cc * t3398 - t3377) * t34 - t3379) * t34 - t3381) *
     # t34 - t3385) * t34 - t3409) * t34
        t3412 = cc * t498
        t3414 = (-t3412 + t3386) * t34
        t3416 = (-t3414 + t3388) * t34
        t3418 = (t3390 - t3416) * t34
        t3419 = t3392 - t3418
        t3420 = t3419 * t34
        t3422 = (t3409 - t3420) * t34
        t3429 = dy * (t3372 / 0.2E1 + t3376 - t413 * (t3385 / 0.2E1 + t3
     #392 / 0.2E1) / 0.6E1 + t3397 * (t3411 / 0.2E1 + t3422 / 0.2E1) / 0
     #.30E2) / 0.4E1
        t3431 = t241 * t21
        t3434 = t244 * t21
        t3436 = (t3431 - t3434) * t21
        t3456 = t319 * ((t14 * ((t1387 * t21 - t3431) * t21 - t3436) * t
     #21 - t14 * (t3436 - (-t21 * t2781 + t3434) * t21) * t21) * t21 + (
     #(t1391 - t248) * t21 - (t248 - t2785) * t21) * t21) / 0.24E2
        t3459 = t413 * (t3350 + t3331) / 0.24E2
        t3463 = cc * (t147 - t859 + t153 - t874)
        t3465 = (cc * (t248 - t3456 + t255 - t3459) - t3463) * t34
        t3467 = (t3463 - t1129) * t34
        t3473 = cc * (t248 + t255)
        t3477 = cc * (t147 + t153)
        t3479 = (t3473 - t3477) * t34
        t3483 = (t3477 - t328) * t34
        t3485 = (t3479 - t3483) * t34
        t3487 = (((cc * (t3216 + t3223) - t3473) * t34 - t3479) * t34 - 
     #t3485) * t34
        t3489 = cc * (t161 + t167)
        t3491 = (t328 - t3489) * t34
        t3493 = (t3483 - t3491) * t34
        t3495 = (t3485 - t3493) * t34
        t3496 = t3487 - t3495
        t3499 = (t3465 - t3467) * t34 - dy * t3496 / 0.12E2
        t3500 = t413 * t3499
        t3506 = 0.7E1 / 0.5760E4 * t1037 * t1647
        t3516 = t233 * t21
        t3519 = t236 * t21
        t3521 = (t3516 - t3519) * t21
        t3560 = (t14 * (t90 - t1316 + t97 - t1337 - t147 + t859 - t153 +
     # t874) * t21 - t14 * (t147 - t859 + t153 - t874 - t202 + t2751 - t
     #208 + t2754) * t21) * t21 - dx * (t14 * ((t1924 * t21 - t3516) * t
     #21 - t3521) * t21 - t14 * (t3521 - (-t21 * t2903 + t3519) * t21) *
     # t21) / 0.24E2 - dx * ((t1928 - t240) * t21 - (t240 - t2907) * t21
     #) / 0.24E2 + (t14 * (t248 - t3456 + t255 - t3459 - t147 + t859 - t
     #153 + t874) * t34 - t877) * t34 - dy * (t14 * ((t3224 * t34 - t925
     #) * t34 - t928) * t34 - t934) / 0.24E2 - dy * ((t3228 - t260) * t3
     #4 - t945) / 0.24E2
        t3561 = cc * t3560
        t3567 = cc * (t240 + t260)
        t3569 = (cc * (t3208 + t3228) - t3567) * t34
        t3571 = (t3567 - t307) * t34
        t3572 = t3569 - t3571
        t3573 = dy * t3572
        t3576 = t13 * t3235 / 0.240E3 - t300 - t3292 - t954 + t318 * t33
     #66 / 0.2E1 - t3429 + t318 * t3500 / 0.24E2 + t1578 * t3260 / 0.48E
     #2 + t3506 + t303 * t3561 / 0.12E2 + t303 * t3573 / 0.144E3
        t3577 = t1037 * t3496
        t3580 = t1453 * t21
        t3581 = t1456 * t21
        t3583 = (t3580 - t3581) * t21
        t3584 = t1482 * t21
        t3586 = (t3581 - t3584) * t21
        t3587 = t3583 - t3586
        t3589 = t14 * t3587 * t21
        t3590 = t1590 * t21
        t3592 = (t3584 - t3590) * t21
        t3593 = t3586 - t3592
        t3595 = t14 * t3593 * t21
        t3596 = t3589 - t3595
        t3597 = t3596 * t21
        t3599 = (t1460 - t1486) * t21
        t3601 = (t1486 - t1594) * t21
        t3602 = t3599 - t3601
        t3603 = t3602 * t21
        t3606 = t319 * (t3597 + t3603) / 0.24E2
        t3609 = t413 * (t2001 + t2045) / 0.24E2
        t3613 = t1487 * t34
        t3616 = t1495 * t34
        t3618 = (t3613 - t3616) * t34
        t3622 = (t1486 - t3606 + t511 - t3609 - t395 + t475 - t485 + t52
     #4) * t34 - dy * ((t3254 * t34 - t3613) * t34 - t3618) / 0.24E2
        t3623 = t302 * t3622
        t3626 = t1037 * t2046
        t3633 = (t511 - t3609 - t485 + t524) * t34 - dy * t2046 / 0.24E2
        t3634 = t413 * t3633
        t3641 = t489 - t495 * dy / 0.24E2 + 0.3E1 / 0.640E3 * t1037 * t2
     #026
        t3642 = dt * t3641
        t3644 = t3369 / 0.2E1
        t3646 = cc * (t161 - t904 + t167 - t919)
        t3648 = (t1129 - t3646) * t34
        t3652 = cc * (t279 + t286)
        t3654 = (t3489 - t3652) * t34
        t3656 = (t3491 - t3654) * t34
        t3658 = (t3493 - t3656) * t34
        t3659 = t3495 - t3658
        t3662 = (t3467 - t3648) * t34 - dy * t3659 / 0.12E2
        t3663 = t413 * t3662
        t3665 = t318 * t3663 / 0.24E2
        t3666 = t3388 / 0.2E1
        t3671 = cc * t2004
        t3673 = (-t3671 + t3412) * t34
        t3675 = (t3414 - t3673) * t34
        t3677 = (t3416 - t3675) * t34
        t3678 = t3418 - t3677
        t3679 = t3678 * t34
        t3681 = (t3420 - t3679) * t34
        t3688 = dy * (t3376 + t3666 - t413 * (t3392 / 0.2E1 + t3418 / 0.
     #2E1) / 0.6E1 + t3397 * (t3422 / 0.2E1 + t3681 / 0.2E1) / 0.30E2) /
     # 0.4E1
        t3690 = cc * (t271 + t291)
        t3692 = (t307 - t3690) * t34
        t3694 = t3571 / 0.2E1 + t3692 / 0.2E1
        t3695 = dy * t3694
        t3697 = t303 * t3695 / 0.24E2
        t3699 = cc * (t1486 + t511)
        t3701 = (t3699 - t622) * t34
        t3703 = cc * (t1494 + t517)
        t3705 = (t622 - t3703) * t34
        t3706 = t3701 - t3705
        t3707 = dy * t3706
        t3709 = t353 * t3707 / 0.48E2
        t3716 = t413 * ((t153 - t874 - t77 + t756) * t34 - dy * t1647 / 
     #0.24E2) / 0.24E2
        t3717 = -t318 * t3577 / 0.1440E4 + t1699 * t3623 / 0.6E1 + 0.7E1
     # / 0.5760E4 * t318 * t3626 - t318 * t3634 / 0.24E2 + t1118 * t3642
     # + t3644 - t3665 - t3688 - t3697 - t3709 - t1694 - t3716
        t3719 = t3571 - t3692
        t3720 = dy * t3719
        t3722 = t303 * t3720 / 0.144E3
        t3723 = t1037 * t3659
        t3725 = t318 * t3723 / 0.1440E4
        t3731 = t14 * (t731 - dy * t737 / 0.24E2 + 0.3E1 / 0.640E3 * t10
     #37 * t1635)
        t3739 = t413 * (t3383 - dy * t3408 / 0.12E2 + t1037 * (t3411 - t
     #3422) / 0.90E2) / 0.24E2
        t3741 = t11 * t261 * t34
        t3744 = t3245 + t3258 - t1481 - t1499
        t3746 = t12 * t3744 * t34
        t3750 = t1037 * t3408 / 0.1440E4
        t3752 = t1037 * t3419 / 0.1440E4
        t3756 = t875 * t34 - dy * t932 / 0.24E2
        t3757 = t10 * t3756
        t3761 = t3467 / 0.2E1
        t3766 = t3465 / 0.2E1 + t3761 - t413 * (t3487 / 0.2E1 + t3495 / 
     #0.2E1) / 0.6E1
        t3767 = dy * t3766
        t3773 = (t1892 * t21 - t3580) * t21 - t3583
        t3783 = t3592 - (-t21 * t2473 + t3590) * t21
        t3794 = t3587 * t21
        t3797 = t3593 * t21
        t3799 = (t3794 - t3797) * t21
        t3831 = t3398 - t1992
        t3650 = t14 * t34
        t3835 = (t3650 * t3831 - t2039) * t34
        t3839 = ((t3835 - t2041) * t34 - t2043) * t34
        t3852 = (t34 * t3831 - t1994) * t34 - t1996
        t3856 = (t3650 * t3852 - t1999) * t34
        t3871 = t320 * (((t3773 * t983 - t3589) * t21 - t3597) * t21 - (
     #t3597 - (-t3783 * t983 + t3595) * t21) * t21) / 0.576E3 + 0.3E1 / 
     #0.640E3 * t320 * (t14 * ((t21 * t3773 - t3794) * t21 - t3799) * t2
     #1 - t14 * (t3799 - (-t21 * t3783 + t3797) * t21) * t21) + 0.3E1 / 
     #0.640E3 * t320 * ((((t1896 - t1460) * t21 - t3599) * t21 - t3603) 
     #* t21 - (t3603 - (t3601 - (t1594 - t2477) * t21) * t21) * t21) + t
     #1486 - dx * t3596 / 0.24E2 - dx * t3602 / 0.24E2 + 0.3E1 / 0.640E3
     # * t1037 * ((t3839 - t2045) * t34 - t2047) - dy * t2000 / 0.24E2 -
     # dy * t2044 / 0.24E2 + t1037 * ((t3856 - t2001) * t34 - t2003) / 0
     #.576E3 + 0.3E1 / 0.640E3 * t1037 * (t14 * ((t34 * t3852 - t2019) *
     # t34 - t2022) * t34 - t2028) + t511
        t3872 = cc * t3871
        t3875 = -t3722 + t3725 + t3731 + t3739 + t1439 * t3741 / 0.24E2 
     #+ t1444 * t3746 / 0.120E3 - t3750 + t3752 + t1740 * t3757 / 0.2E1 
     #- t318 * t3767 / 0.4E1 + t353 * t3872 / 0.4E1
        t3883 = t413 * (t3390 - dy * t3419 / 0.12E2 + t1037 * (t3422 - t
     #3681) / 0.90E2) / 0.24E2
        t3884 = t1508 - t414
        t3888 = (t3884 * t983 - t3248) * t21
        t3892 = t561 - t2478
        t3896 = (-t3892 * t983 + t3251) * t21
        t3909 = (t14 * (t1038 - t1992) * t21 - t14 * (t1992 - t2547) * t
     #21) * t21
        t3920 = (cc * ((t14 * (t3888 + t1072 - t3253 - t2041) * t21 - t1
     #4 * (t3253 + t2041 - t3896 - t2581) * t21) * t21 + (t14 * (t3909 +
     # t3835 - t3253 - t2041) * t34 - t3256) * t34) - t3260) * t34 / 0.2
     #E1 + t3262 / 0.2E1
        t3921 = dy * t3920
        t3924 = t3648 / 0.2E1
        t3929 = t3761 + t3924 - t413 * (t3495 / 0.2E1 + t3658 / 0.2E1) /
     # 0.6E1
        t3930 = dy * t3929
        t3932 = t318 * t3930 / 0.4E1
        t3934 = cc * (t1486 - t3606 + t511 - t3609)
        t3937 = (t3934 - t526) * t34 / 0.2E1
        t3938 = t1464 * t21
        t3939 = t1467 * t21
        t3941 = (t3938 - t3939) * t21
        t3942 = t1490 * t21
        t3944 = (t3939 - t3942) * t21
        t3945 = t3941 - t3944
        t3947 = t14 * t3945 * t21
        t3948 = t1598 * t21
        t3950 = (t3942 - t3948) * t21
        t3951 = t3944 - t3950
        t3953 = t14 * t3951 * t21
        t3954 = t3947 - t3953
        t3955 = t3954 * t21
        t3957 = (t1471 - t1494) * t21
        t3959 = (t1494 - t1602) * t21
        t3960 = t3957 - t3959
        t3961 = t3960 * t21
        t3964 = t319 * (t3955 + t3961) / 0.24E2
        t3967 = t413 * (t2013 + t2055) / 0.24E2
        t3969 = cc * (t1494 - t3964 + t517 - t3967)
        t3972 = (t526 - t3969) * t34 / 0.2E1
        t3974 = cc * (t3253 + t2041)
        t3976 = (t3974 - t3699) * t34
        t3977 = t3976 - t3701
        t3978 = t3977 * t34
        t3979 = t3706 * t34
        t3981 = (t3978 - t3979) * t34
        t3983 = cc * (t3278 + t2051)
        t3985 = (t3703 - t3983) * t34
        t3986 = t3705 - t3985
        t3987 = t3986 * t34
        t3989 = (t3979 - t3987) * t34
        t3994 = t3937 + t3972 - t413 * (t3981 / 0.2E1 + t3989 / 0.2E1) /
     # 0.6E1
        t3995 = dy * t3994
        t3997 = t353 * t3995 / 0.8E1
        t3999 = t3569 / 0.2E1 + t3571 / 0.2E1
        t4000 = dy * t3999
        t4003 = dy * t3977
        t4007 = t3246 * t21
        t4010 = t3249 * t21
        t4012 = (t4007 - t4010) * t21
        t4053 = (cc * (t3253 - t319 * ((t14 * ((t21 * t3884 - t4007) * t
     #21 - t4012) * t21 - t14 * (t4012 - (-t21 * t3892 + t4010) * t21) *
     # t21) * t21 + ((t3888 - t3253) * t21 - (t3253 - t3896) * t21) * t2
     #1) / 0.24E2 + t2041 - t413 * (t3856 + t3839) / 0.24E2) - t3934) * 
     #t34 / 0.2E1 + t3937 - t413 * ((((cc * (t3909 + t3835) - t3974) * t
     #34 - t3976) * t34 - t3978) * t34 / 0.2E1 + t3981 / 0.2E1) / 0.6E1
        t4054 = dy * t4053
        t4057 = t3258 - t1499
        t4058 = dy * t4057
        t4061 = dy * t944
        t4064 = -t3883 - t1578 * t3921 / 0.96E2 - t3932 - t1860 - t3997 
     #- t1947 - t303 * t4000 / 0.24E2 + t353 * t4003 / 0.48E2 - t353 * t
     #4054 / 0.8E1 - t303 * t4058 / 0.288E3 - t353 * t4061 / 0.48E2 - t2
     #064
        t4066 = t3576 + t3717 + t3875 + t4064
        t4069 = dt * t413
        t4071 = t4069 * t3662 / 0.48E2
        t4074 = dt * t1037
        t4077 = t10 * dy
        t4084 = t302 * dy
        t4086 = t4084 * t3694 / 0.192E3
        t4089 = -t4071 + t2129 * t3560 / 0.96E2 - t4074 * t3496 / 0.2880
     #E4 - t3429 + t4077 * t3977 / 0.192E3 + t2093 * t3234 / 0.7680E4 + 
     #t3506 - t2124 + 0.7E1 / 0.11520E5 * t4074 * t2046 - t4086 - t4084 
     #* t3999 / 0.192E3
        t4090 = dt * dy
        t4094 = t4077 * t3706 / 0.192E3
        t4097 = t11 * dy
        t4099 = t4097 * t3289 / 0.1536E4
        t4107 = t4090 * t3929 / 0.8E1
        t4113 = -t4090 * t3766 / 0.8E1 - t4094 + t4084 * t3572 / 0.1152E
     #4 - t4099 + t2145 * t3641 / 0.2E1 - t4097 * t3920 / 0.1536E4 + t36
     #44 + t2105 * t3259 / 0.768E3 - t4107 + t2167 * t3744 * t34 / 0.384
     #0E4 + t2090 * t3871 / 0.16E2 - t3688
        t4128 = t2113 * t3365 / 0.4E1 - t3716 - t4077 * t4053 / 0.32E2 +
     # t2150 * t3756 / 0.8E1 + t3731 + t2162 * t3622 / 0.48E2 + t3739 + 
     #t4069 * t3499 / 0.48E2 + t2173 * t261 * t34 / 0.384E3 - t3750 + t3
     #752
        t4130 = t4074 * t3659 / 0.2880E4
        t4136 = t4077 * t3994 / 0.32E2
        t4140 = t4084 * t3719 / 0.1152E4
        t4141 = t4130 - t3883 - t2149 - t2154 - t2156 - t4077 * t944 / 0
     #.192E3 - t2161 - t4069 * t3633 / 0.48E2 - t4136 - t4084 * t4057 / 
     #0.2304E4 - t1947 - t4140
        t4143 = t4089 + t4113 + t4128 + t4141
        t4156 = t2083 * t3663 / 0.24E2
        t4158 = t2083 * t3723 / 0.1440E4
        t4161 = t2198 * t3623 / 0.6E1 + t2235 * t3642 + t2203 * t3573 / 
     #0.144E3 + t2083 * t3500 / 0.24E2 + t2222 * t3260 / 0.48E2 - t3429 
     #+ t3506 - t4156 - t2205 + t4158 - t2203 * t4058 / 0.288E3
        t4178 = 0.7E1 / 0.5760E4 * t2083 * t3626 - t2189 * t4061 / 0.48E
     #2 + t3644 - t2083 * t3767 / 0.4E1 + t2194 * t3757 / 0.2E1 - t3688 
     #- t2203 * t4000 / 0.24E2 + t2189 * t4003 / 0.48E2 - t3716 + t2189 
     #* t3872 / 0.4E1 + t3731 + t2083 * t3366 / 0.2E1
        t4186 = t3739 + t2213 * t3235 / 0.240E3 + t2216 * t3741 / 0.24E2
     # + t2219 * t3746 / 0.120E3 - t3750 + t3752 - t2248 - t2250 - t2252
     # - t2254 - t3883
        t4190 = t2203 * t3695 / 0.24E2
        t4192 = t2189 * t3707 / 0.48E2
        t4196 = t2083 * t3930 / 0.4E1
        t4198 = t2189 * t3995 / 0.8E1
        t4202 = t2203 * t3720 / 0.144E3
        t4204 = t2222 * t3290 / 0.96E2
        t4209 = -t2083 * t3634 / 0.24E2 - t4190 - t4192 - t2083 * t3577 
     #/ 0.1440E4 - t4196 - t4198 - t2222 * t3921 / 0.96E2 - t4202 - t194
     #7 - t4204 - t2189 * t4054 / 0.8E1 + t2203 * t3561 / 0.12E2
        t4211 = t4161 + t4178 + t4186 + t4209
        t4214 = t3090 * t4066 + t3092 * t4143 + t3094 * t4211
        t4218 = dt * t4066
        t4224 = dt * t4143
        t4230 = dt * t4211
        t4236 = (-t4218 / 0.2E1 - t4218 * t2082) * t2080 * t2085 + (-t20
     #82 * t4224 - t4224 * t6) * t2183 * t2186 + (-t4230 * t6 - t4230 / 
     #0.2E1) * t2278 * t2281
        t4254 = j - 4
        t4255 = ut(i,t4254,n)
        t4266 = (t3679 - (t3677 - (t3675 - (t3673 - (-cc * t4255 + t3671
     #) * t34) * t34) * t34) * t34) * t34
        t4272 = t413 * (t3416 - dy * t3678 / 0.12E2 + t1037 * (t3681 - t
     #4266) / 0.90E2) / 0.24E2
        t4279 = t413 * ((t77 - t756 - t167 + t919) * t34 - dy * t1649 / 
     #0.24E2) / 0.24E2
        t4280 = t1520 - t426
        t4284 = (t4280 * t983 - t3273) * t21
        t4288 = t573 - t2492
        t4292 = (-t4288 * t983 + t3276) * t21
        t4305 = (t14 * (t1054 - t2004) * t21 - t14 * (t2004 - t2563) * t
     #21) * t21
        t4306 = t2004 - t4255
        t4310 = (-t3650 * t4306 + t2049) * t34
        t4321 = t3287 / 0.2E1 + (t3285 - cc * ((t14 * (t4284 + t1082 - t
     #3278 - t2051) * t21 - t14 * (t3278 + t2051 - t4292 - t2591) * t21)
     # * t21 + (t3281 - t14 * (t3278 + t2051 - t4305 - t4310) * t34) * t
     #34)) * t34 / 0.2E1
        t4322 = dy * t4321
        t4326 = t1037 * t3678 / 0.1440E4
        t4334 = (t14 * (t1407 + t1374 - t279 - t286) * t21 - t14 * (t279
     # + t286 - t2801 - t2418) * t21) * t21
        t4342 = (t14 * (t1361 - t281) * t21 - t14 * (t281 - t2390) * t21
     #) * t21
        t4344 = t281 - u(i,t4254,n)
        t4348 = (-t3650 * t4344 + t284) * t34
        t4349 = t279 + t286 - t4342 - t4348
        t4353 = (-t3650 * t4349 + t289) * t34
        t4357 = (t3690 - cc * (t4334 + t4353)) * t34
        t4358 = t3692 - t4357
        t4359 = dy * t4358
        t4369 = (t3656 - (t3654 - (t3652 - cc * (t4342 + t4348)) * t34) 
     #* t34) * t34
        t4370 = t3658 - t4369
        t4371 = t1037 * t4370
        t4378 = (t485 - t524 - t517 + t3967) * t34 - dy * t2056 / 0.24E2
        t4379 = t413 * t4378
        t4382 = t1037 * t2056
        t4385 = -t4272 + t300 - t4279 - t3292 + t954 - t1578 * t4322 / 0
     #.96E2 + t4326 - t303 * t4359 / 0.144E3 + t318 * t4371 / 0.1440E4 -
     # t318 * t4379 / 0.24E2 + 0.7E1 / 0.5760E4 * t318 * t4382
        t4387 = t3692 / 0.2E1 + t4357 / 0.2E1
        t4388 = dy * t4387
        t4391 = dy * t3986
        t4394 = t1499 - t3283
        t4395 = dy * t4394
        t4406 = (t395 - t475 + t485 - t524 - t1494 + t3964 - t517 + t396
     #7) * t34 - dy * (t3618 - (-t3279 * t34 + t3616) * t34) / 0.24E2
        t4407 = t302 * t4406
        t4414 = t492 - dy * t503 / 0.24E2 + 0.3E1 / 0.640E3 * t1037 * t2
     #032
        t4415 = dt * t4414
        t4418 = 0.7E1 / 0.5760E4 * t1037 * t1649
        t4430 = dy * (t3666 + t3414 / 0.2E1 - t413 * (t3418 / 0.2E1 + t3
     #677 / 0.2E1) / 0.6E1 + t3397 * (t3681 / 0.2E1 + t4266 / 0.2E1) / 0
     #.30E2) / 0.4E1
        t4431 = dy * t946
        t4437 = t920 * t34 - dy * t938 / 0.24E2
        t4438 = t10 * t4437
        t4446 = t14 * (t734 - dy * t743 / 0.24E2 + 0.3E1 / 0.640E3 * t10
     #37 * t1641)
        t4447 = -t303 * t4388 / 0.24E2 - t353 * t4391 / 0.48E2 - t303 * 
     #t4395 / 0.288E3 + t1699 * t4407 / 0.6E1 + t1118 * t4415 + t4418 - 
     #t4430 - t353 * t4431 / 0.48E2 + t1740 * t4438 / 0.2E1 + t4446 + t3
     #665 - t3688
        t4462 = (t14 * (t1936 + t1427 - t271 - t291) * t21 - t14 * (t271
     # + t291 - t2915 - t2821) * t21) * t21 + (t294 - t14 * (t271 + t291
     # - t4334 - t4353) * t34) * t34
        t4463 = cc * t4462
        t4469 = (t1900 * t21 - t3938) * t21 - t3941
        t4479 = t3950 - (-t21 * t2487 + t3948) * t21
        t4490 = t3945 * t21
        t4493 = t3951 * t21
        t4495 = (t4490 - t4493) * t21
        t4530 = (t2053 - (t2051 - t4310) * t34) * t34
        t4543 = t2008 - (-t34 * t4306 + t2006) * t34
        t4547 = (-t3650 * t4543 + t2011) * t34
        t4562 = t320 * (((t4469 * t983 - t3947) * t21 - t3955) * t21 - (
     #t3955 - (-t4479 * t983 + t3953) * t21) * t21) / 0.576E3 + 0.3E1 / 
     #0.640E3 * t320 * (t14 * ((t21 * t4469 - t4490) * t21 - t4495) * t2
     #1 - t14 * (t4495 - (-t21 * t4479 + t4493) * t21) * t21) + 0.3E1 / 
     #0.640E3 * t320 * ((((t1904 - t1471) * t21 - t3957) * t21 - t3961) 
     #* t21 - (t3961 - (t3959 - (t1602 - t2491) * t21) * t21) * t21) + t
     #1494 - dx * t3954 / 0.24E2 - dx * t3960 / 0.24E2 + 0.3E1 / 0.640E3
     # * t1037 * (t2057 - (t2055 - t4530) * t34) - dy * t2012 / 0.24E2 -
     # dy * t2054 / 0.24E2 + t1037 * (t2015 - (t2013 - t4547) * t34) / 0
     #.576E3 + 0.3E1 / 0.640E3 * t1037 * (t2034 - t14 * (t2031 - (-t34 *
     # t4543 + t2029) * t34) * t34) + t517
        t4563 = cc * t4562
        t4567 = t272 * t21
        t4570 = t275 * t21
        t4572 = (t4567 - t4570) * t21
        t4592 = t319 * ((t14 * ((t1403 * t21 - t4567) * t21 - t4572) * t
     #21 - t14 * (t4572 - (-t21 * t2797 + t4570) * t21) * t21) * t21 + (
     #(t1407 - t279) * t21 - (t279 - t2801) * t21) * t21) / 0.24E2
        t4596 = t907 - (-t34 * t4344 + t905) * t34
        t4600 = (-t3650 * t4596 + t910) * t34
        t4604 = (t914 - (t286 - t4348) * t34) * t34
        t4607 = t413 * (t4600 + t4604) / 0.24E2
        t4611 = (t3646 - cc * (t279 - t4592 + t286 - t4607)) * t34
        t4617 = t3924 + t4611 / 0.2E1 - t413 * (t3658 / 0.2E1 + t4369 / 
     #0.2E1) / 0.6E1
        t4618 = dy * t4617
        t4621 = -t3697 + t3709 + t1694 - t13 * t4463 / 0.240E3 + t3722 -
     # t3725 - t3752 - t353 * t4563 / 0.4E1 - t318 * t4618 / 0.4E1 + t38
     #83 - t3932
        t4634 = t885 * t21
        t4637 = t891 * t21
        t4639 = (t4634 - t4637) * t21
        t4682 = -dx * t894 / 0.24E2 - dx * t900 / 0.24E2 + t320 * ((t134
     #9 - t895) * t21 - (t895 - t2765) * t21) / 0.576E3 + 0.3E1 / 0.640E
     #3 * t320 * (t14 * ((t1345 * t21 - t4634) * t21 - t4639) * t21 - t1
     #4 * (t4639 - (-t21 * t2761 + t4637) * t21) * t21) + 0.3E1 / 0.640E
     #3 * t320 * ((t1357 - t901) * t21 - (t901 - t2769) * t21) + t161 + 
     #0.3E1 / 0.640E3 * t1037 * (t1650 - (t916 - t4604) * t34) - dy * t9
     #11 / 0.24E2 - dy * t915 / 0.24E2 + t1037 * (t1624 - (t912 - t4600)
     # * t34) / 0.576E3 + 0.3E1 / 0.640E3 * t1037 * (t1643 - t14 * (t164
     #0 - (-t34 * t4596 + t1638) * t34) * t34) + t167
        t4683 = cc * t4682
        t4687 = t3271 * t21
        t4690 = t3274 * t21
        t4692 = (t4687 - t4690) * t21
        t4733 = t3972 + (t3969 - cc * (t3278 - t319 * ((t14 * ((t21 * t4
     #280 - t4687) * t21 - t4692) * t21 - t14 * (t4692 - (-t21 * t4288 +
     # t4690) * t21) * t21) * t21 + ((t4284 - t3278) * t21 - (t3278 - t4
     #292) * t21) * t21) / 0.24E2 + t2051 - t413 * (t4547 + t4530) / 0.2
     #4E2)) * t34 / 0.2E1 - t413 * (t3989 / 0.2E1 + (t3987 - (t3985 - (t
     #3983 - cc * (t4305 + t4310)) * t34) * t34) * t34 / 0.2E1) / 0.6E1
        t4734 = dy * t4733
        t4741 = (t3648 - t4611) * t34 - dy * t4370 / 0.12E2
        t4742 = t413 * t4741
        t4756 = t264 * t21
        t4759 = t267 * t21
        t4761 = (t4756 - t4759) * t21
        t4800 = (t14 * (t108 - t1360 + t115 - t1381 - t161 + t904 - t167
     # + t919) * t21 - t14 * (t161 - t904 + t167 - t919 - t216 + t2772 -
     # t222 + t2775) * t21) * t21 - dx * (t14 * ((t1932 * t21 - t4756) *
     # t21 - t4761) * t21 - t14 * (t4761 - (-t21 * t2911 + t4759) * t21)
     # * t21) / 0.24E2 - dx * ((t1936 - t271) * t21 - (t271 - t2915) * t
     #21) / 0.24E2 + (t922 - t14 * (t161 - t904 + t167 - t919 - t279 + t
     #4592 - t286 + t4607) * t34) * t34 - dy * (t940 - t14 * (t937 - (-t
     #34 * t4349 + t935) * t34) * t34) / 0.24E2 - dy * (t947 - (t291 - t
     #4353) * t34) / 0.24E2
        t4801 = cc * t4800
        t4804 = t3386 / 0.2E1
        t4806 = t11 * t292 * t34
        t4809 = t1481 + t1499 - t3270 - t3283
        t4811 = t12 * t4809 * t34
        t4814 = -t318 * t4683 / 0.2E1 + t1860 - t3997 + t1947 - t353 * t
     #4734 / 0.8E1 - t318 * t4742 / 0.24E2 - t1578 * t3285 / 0.48E2 - t3
     #03 * t4801 / 0.12E2 + t2064 - t4804 + t1439 * t4806 / 0.24E2 + t14
     #44 * t4811 / 0.120E3
        t4816 = t4385 + t4447 + t4621 + t4814
        t4831 = -t4272 - t4279 + t4071 + t4074 * t4370 / 0.2880E4 + t432
     #6 - t4084 * t4358 / 0.1152E4 - t4090 * t4617 / 0.8E1 - t4077 * t47
     #33 / 0.32E2 + t2124 - t4069 * t4741 / 0.48E2 - t2105 * t3284 / 0.7
     #68E3
        t4842 = -t4086 + t2162 * t4406 / 0.48E2 + t4418 - t4430 + t4094 
     #- t4077 * t946 / 0.192E3 - t4099 - t2093 * t4462 / 0.7680E4 + t444
     #6 - t4077 * t3986 / 0.192E3 - t4107 - t4084 * t4394 / 0.2304E4
        t4859 = t2167 * t4809 * t34 / 0.3840E4 - t3688 - t4069 * t4378 /
     # 0.48E2 - t4097 * t4321 / 0.1536E4 - t4084 * t4387 / 0.192E3 + 0.7
     #E1 / 0.11520E5 * t4074 * t2056 - t3752 - t2090 * t4562 / 0.16E2 + 
     #t2145 * t4414 / 0.2E1 - t4130 + t3883
        t4869 = t2149 + t2154 - t2129 * t4800 / 0.96E2 + t2156 + t2161 -
     # t2113 * t4682 / 0.4E1 + t2173 * t292 * t34 / 0.384E3 - t4136 + t1
     #947 + t2150 * t4437 / 0.8E1 + t4140 - t4804
        t4871 = t4831 + t4842 + t4859 + t4869
        t4882 = -t4272 + t2194 * t4438 / 0.2E1 + t2198 * t4407 / 0.6E1 -
     # t4279 - t2189 * t4563 / 0.4E1 + t4326 - t2189 * t4734 / 0.8E1 + t
     #4156 + t4418 + t2205 - t4430
        t4901 = t2083 * t4371 / 0.1440E4 - t2083 * t4742 / 0.24E2 - t222
     #2 * t3285 / 0.48E2 - t4158 - t2083 * t4683 / 0.2E1 - t2203 * t4388
     # / 0.24E2 - t2189 * t4391 / 0.48E2 + t4446 - t2203 * t4395 / 0.288
     #E3 - t2189 * t4431 / 0.48E2 - t2203 * t4801 / 0.12E2 - t3688
        t4910 = -t2083 * t4379 / 0.24E2 - t2203 * t4359 / 0.144E3 + 0.7E
     #1 / 0.5760E4 * t2083 * t4382 - t3752 + t2248 + t2250 + t2252 + t22
     #54 + t2235 * t4415 + t3883 - t4190
        t4921 = t4192 - t4196 - t2083 * t4618 / 0.4E1 - t4198 - t2213 * 
     #t4463 / 0.240E3 + t4202 + t1947 - t4204 - t2222 * t4322 / 0.96E2 -
     # t4804 + t2216 * t4806 / 0.24E2 + t2219 * t4811 / 0.120E3
        t4923 = t4882 + t4901 + t4910 + t4921
        t4926 = t3090 * t4816 + t3092 * t4871 + t3094 * t4923
        t4930 = dt * t4816
        t4936 = dt * t4871
        t4942 = dt * t4923
        t4948 = (-t4930 / 0.2E1 - t4930 * t2082) * t2080 * t2085 + (-t20
     #82 * t4936 - t4936 * t6) * t2183 * t2186 + (-t4942 * t6 - t4942 / 
     #0.2E1) * t2278 * t2281
        t4820 = t6 * t2082 * t2183 * t2186

        unew(i,j) = t1 + dt * t2 + (t2283 * t11 / 0.12E2 + t2305 * 
     #t302 / 0.6E1 + (t2077 * t10 * t2311 / 0.2E1 + t2181 * t10 * t4820 
     #+ t2276 * t10 * t2321 / 0.2E1) * t10 / 0.2E1 - t3153 * t11 / 0.12E
     #2 - t3175 * t302 / 0.6E1 - (t3043 * t10 * t2311 / 0.2E1 + t3098 * 
     #t10 * t4820 + t3150 * t10 * t2321 / 0.2E1) * t10 / 0.2E1) * t21 + 
     #(t4214 * t11 / 0.12E2 + t4236 * t302 / 0.6E1 + (t4066 * t10 * t231
     #1 / 0.2E1 + t4143 * t10 * t4820 + t4211 * t10 * t2321 / 0.2E1) * t
     #10 / 0.2E1 - t4926 * t11 / 0.12E2 - t4948 * t302 / 0.6E1 - (t4816 
     #* t10 * t2311 / 0.2E1 + t4871 * t10 * t4820 + t4923 * t10 * t2321 
     #/ 0.2E1) * t10 / 0.2E1) * t34

        utnew(i,j) = t2 + (t2283 * t302 / 0.3E1 + t2
     #305 * t10 / 0.2E1 + t2077 * t302 * t2311 / 0.2E1 + t2181 * t302 * 
     #t4820 + t2276 * t302 * t2321 / 0.2E1 - t3153 * t302 / 0.3E1 - t317
     #5 * t10 / 0.2E1 - t3043 * t302 * t2311 / 0.2E1 - t3098 * t302 * t4
     #820 - t3150 * t302 * t2321 / 0.2E1) * t21 + (t4214 * t302 / 0.3E1 
     #+ t4236 * t10 / 0.2E1 + t4066 * t302 * t2311 / 0.2E1 + t4143 * t30
     #2 * t4820 + t4211 * t302 * t2321 / 0.2E1 - t4926 * t302 / 0.3E1 - 
     #t4948 * t10 / 0.2E1 - t4816 * t302 * t2311 / 0.2E1 - t4871 * t302 
     #* t4820 - t4923 * t302 * t2321 / 0.2E1) * t34

        return
      end
