      subroutine duStepWaveGen3d6rcOLD( 
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
        integer t10
        real t100
        real t1000
        real t1002
        real t1003
        real t1004
        real t1006
        real t1007
        real t1008
        real t1010
        real t1012
        real t1013
        real t1015
        real t1016
        real t1018
        real t102
        real t1020
        real t1021
        real t1022
        real t1024
        real t1025
        real t1026
        real t1028
        real t1030
        real t1031
        real t1033
        real t1035
        real t1036
        real t1038
        real t104
        real t1040
        real t1041
        real t1043
        real t1044
        real t1046
        real t1048
        real t1049
        real t1051
        real t1052
        real t1054
        real t1056
        real t1057
        real t1059
        real t106
        real t1060
        real t1062
        real t1064
        real t1065
        real t1067
        real t1069
        real t107
        real t1070
        real t1074
        real t1075
        real t1078
        real t108
        real t1085
        integer t1086
        real t1087
        real t1088
        real t1092
        integer t1099
        real t11
        real t110
        real t1100
        real t1101
        real t1105
        real t112
        real t1138
        integer t1139
        real t114
        real t1140
        real t1141
        real t1145
        integer t1152
        real t1153
        real t1154
        real t1158
        real t116
        real t1169
        real t1172
        real t1174
        real t118
        real t12
        real t120
        real t121
        integer t1212
        real t1214
        real t1218
        real t122
        real t1222
        real t1226
        real t1237
        real t1244
        real t1248
        real t125
        real t1251
        real t1258
        integer t126
        real t1261
        real t1263
        real t127
        real t128
        real t1280
        real t1284
        real t1289
        real t1291
        real t1295
        real t13
        real t130
        real t1301
        real t1308
        real t1309
        real t131
        real t1312
        real t1316
        real t1317
        real t1319
        integer t132
        real t1322
        real t1324
        real t1328
        real t1329
        real t133
        real t134
        real t1355
        real t1356
        real t1358
        real t136
        real t1361
        real t1363
        real t1367
        real t1368
        real t138
        real t139
        real t1394
        real t1395
        real t1397
        real t1398
        integer t140
        real t1402
        real t1408
        real t141
        real t1412
        real t142
        real t1423
        real t1424
        real t1429
        real t143
        real t1436
        real t1437
        real t1438
        real t144
        real t1440
        real t1443
        real t1445
        real t1449
        real t1450
        real t146
        real t147
        real t1483
        real t149
        real t1491
        real t1492
        real t1494
        real t1497
        real t1499
        real t15
        real t150
        real t1503
        real t1504
        real t152
        integer t153
        real t1530
        real t1533
        real t154
        real t155
        real t156
        real t1560
        real t1565
        real t1570
        real t1573
        real t1579
        real t158
        real t1582
        real t1587
        real t1588
        real t159
        real t1590
        real t1592
        real t1594
        real t1595
        real t1596
        real t1597
        real t1598
        real t1599
        integer t16
        real t1600
        real t1602
        real t1604
        real t1606
        real t1607
        real t1608
        real t1609
        real t161
        real t1610
        real t1614
        real t1615
        real t1616
        real t1618
        real t1619
        real t162
        real t1621
        real t1622
        real t1623
        real t1625
        real t1626
        real t1627
        real t1628
        real t163
        real t1630
        real t1631
        real t1633
        real t1634
        real t1635
        real t1637
        real t1641
        real t1642
        real t1644
        real t1645
        real t1647
        real t1648
        real t165
        real t1650
        real t1651
        real t1653
        real t1654
        real t1656
        real t1661
        real t1663
        real t1665
        real t1666
        real t1667
        real t1668
        real t1669
        real t167
        real t1671
        real t1673
        real t1675
        real t1676
        real t1677
        real t1678
        real t1679
        integer t1687
        real t1688
        real t1689
        real t169
        real t1690
        real t1692
        real t1693
        real t1695
        real t1696
        real t1697
        real t1699
        real t17
        real t1703
        real t1705
        real t1706
        real t1708
        real t171
        real t1713
        real t1715
        real t1717
        real t1718
        real t1719
        real t1720
        real t1721
        real t1725
        real t1727
        real t1728
        real t1729
        real t173
        real t1730
        real t1732
        real t1733
        real t1735
        real t1736
        real t1738
        real t1739
        real t1741
        real t1742
        real t1743
        real t1745
        real t1746
        real t1748
        real t175
        real t1753
        real t1754
        real t1755
        real t1757
        real t1759
        real t176
        real t1760
        real t1761
        real t1763
        real t177
        real t1771
        real t1773
        real t1774
        real t1779
        real t1780
        real t1781
        real t1783
        real t1784
        real t1785
        real t1786
        real t1787
        real t1788
        real t1789
        real t1796
        real t1797
        real t1799
        real t18
        real t180
        real t1802
        real t1803
        real t1804
        real t1805
        real t1806
        real t1808
        real t1809
        integer t181
        real t1811
        real t1812
        real t1814
        real t1815
        real t1816
        real t1817
        real t1819
        real t182
        real t1820
        real t1822
        real t1823
        real t1824
        real t1826
        real t1828
        real t183
        real t1830
        real t1832
        real t1834
        real t1836
        real t1837
        real t1838
        real t1841
        real t1842
        real t1843
        real t1844
        real t1845
        real t1847
        real t1848
        real t185
        real t1850
        real t1851
        real t1853
        real t1854
        real t1855
        real t1856
        real t1858
        real t1859
        real t186
        real t1861
        real t1862
        real t1863
        real t1865
        real t1867
        real t1869
        integer t187
        real t1871
        real t1873
        real t1875
        real t1876
        real t1877
        real t188
        real t1880
        real t1881
        real t1882
        real t1883
        real t1884
        real t1885
        real t1887
        real t1888
        real t1889
        real t189
        real t1891
        real t1893
        real t1894
        real t1895
        real t1897
        real t1898
        real t1899
        real t19
        real t1901
        real t1903
        real t1904
        real t1905
        real t1907
        real t1908
        real t1909
        real t191
        real t1914
        real t1916
        real t1921
        real t1923
        real t1927
        real t1929
        real t193
        real t1931
        real t1933
        real t1936
        real t1937
        real t1938
        real t194
        real t1940
        real t1943
        real t1945
        real t1949
        integer t195
        real t1950
        real t196
        real t1962
        real t1968
        real t197
        real t1975
        real t1976
        real t1977
        real t1979
        real t198
        real t1982
        real t1984
        real t1988
        real t1989
        real t199
        real t2
        real t2001
        real t2007
        real t201
        real t2014
        real t2015
        real t2016
        real t2017
        real t2018
        real t2019
        real t202
        real t2021
        real t2022
        real t2023
        real t2025
        real t2027
        real t2028
        real t2029
        real t2031
        real t2032
        real t2033
        real t2035
        real t2037
        real t2038
        real t2039
        real t204
        real t2040
        real t2041
        real t2042
        real t2043
        real t2048
        real t205
        real t2050
        real t2053
        real t2056
        real t2058
        real t2060
        real t2062
        real t2064
        real t2065
        real t2068
        real t207
        real t2071
        real t2072
        real t2073
        real t2074
        real t2075
        real t2076
        real t2078
        integer t208
        real t2080
        real t2081
        real t2083
        real t2085
        real t2086
        real t2088
        real t2089
        real t209
        real t2090
        real t2093
        real t2094
        real t2095
        real t2098
        real t21
        real t210
        real t2101
        real t2102
        real t2104
        real t2107
        real t2108
        real t211
        real t2112
        real t2114
        real t2117
        real t2118
        real t2121
        real t2127
        real t213
        real t2130
        real t2133
        real t2136
        real t2139
        real t214
        real t2142
        real t2143
        real t2145
        real t2146
        real t2148
        real t2149
        real t2151
        real t2152
        real t2154
        real t2155
        real t2157
        real t2158
        real t216
        real t2160
        real t2161
        real t2166
        real t2168
        real t217
        real t2171
        real t2173
        real t2175
        real t2176
        real t2179
        real t218
        real t2180
        real t2185
        real t2186
        real t2189
        real t2193
        real t2196
        real t22
        real t220
        real t2214
        real t2215
        real t2217
        real t2219
        real t222
        real t2221
        real t2223
        real t2225
        real t2227
        real t2228
        real t2233
        real t2235
        real t2238
        real t224
        real t2240
        real t2244
        real t2250
        real t2256
        real t226
        real t2262
        real t2268
        real t2273
        real t228
        real t2289
        real t2294
        real t2299
        real t230
        real t2308
        real t231
        real t2311
        real t2316
        real t2319
        real t232
        real t2325
        real t2326
        real t2328
        real t2330
        real t2331
        real t2335
        real t2340
        real t2344
        real t2346
        real t2348
        real t2349
        real t235
        real t2353
        real t2358
        real t236
        real t2364
        real t2366
        real t2368
        real t237
        real t2376
        real t2380
        real t2382
        real t2384
        real t239
        real t2392
        real t2398
        real t240
        real t2402
        real t2406
        real t241
        real t2410
        real t2411
        real t2415
        real t2416
        real t2420
        real t2425
        real t2429
        real t243
        real t2433
        real t2434
        real t2438
        real t2443
        real t2449
        real t245
        real t2453
        real t246
        real t2461
        real t2465
        real t2469
        real t247
        real t2477
        real t248
        real t2483
        real t2487
        real t249
        real t2492
        integer t2493
        real t2495
        real t2499
        real t25
        real t2503
        real t251
        real t2512
        real t252
        real t2526
        real t2527
        real t2538
        real t2539
        real t254
        real t255
        real t2556
        real t2558
        real t2561
        real t2563
        real t257
        real t2570
        real t258
        real t2580
        real t2581
        real t259
        real t2592
        real t2593
        real t26
        real t260
        real t2612
        real t262
        real t2622
        real t263
        real t2633
        real t2636
        real t2638
        real t265
        real t266
        real t267
        real t2673
        real t2681
        real t269
        real t2690
        real t2694
        integer t27
        real t2706
        real t2707
        real t271
        real t2713
        real t2714
        real t2716
        real t2719
        real t2721
        real t2725
        real t2726
        real t273
        real t275
        real t2752
        real t2753
        real t2755
        real t2758
        real t2760
        real t2764
        real t2765
        real t277
        real t279
        real t2791
        real t2792
        real t2794
        real t2798
        real t28
        real t280
        real t2804
        real t2808
        real t281
        real t2819
        real t2824
        real t2831
        real t2832
        real t2833
        real t2835
        real t2838
        real t284
        real t2840
        real t2844
        real t2845
        real t285
        real t286
        real t2878
        real t288
        real t2886
        real t2887
        real t2889
        real t289
        real t2892
        real t2894
        real t2898
        real t2899
        real t29
        real t290
        real t292
        real t2925
        real t294
        real t295
        real t2958
        real t296
        real t2963
        real t2966
        real t297
        real t2972
        real t2975
        real t298
        real t2980
        real t2983
        real t2985
        real t2988
        real t2991
        real t2995
        real t2998
        real t30
        real t300
        real t3002
        real t3005
        real t3008
        real t301
        real t3011
        real t3015
        real t3018
        real t3021
        real t3024
        real t3027
        real t303
        real t3030
        real t3035
        real t304
        real t3055
        real t306
        real t3069
        real t307
        real t3074
        real t3077
        real t308
        real t3081
        real t3087
        real t309
        real t3093
        real t3099
        real t311
        real t312
        real t3122
        real t3125
        real t3126
        real t3127
        real t3129
        real t3130
        real t3131
        real t3132
        real t3133
        real t3134
        real t3135
        real t3136
        real t3137
        real t3138
        real t314
        real t3141
        real t3144
        real t3145
        real t3147
        real t3148
        real t315
        real t3150
        real t3151
        real t3153
        real t3154
        real t3156
        real t3157
        real t3159
        real t316
        real t3160
        real t3161
        real t3163
        real t3165
        real t3166
        real t3167
        real t3170
        real t3171
        real t3172
        real t3173
        real t3174
        real t3176
        real t3177
        real t3179
        real t318
        real t3180
        real t3182
        real t3183
        real t3184
        real t3185
        real t3187
        real t3188
        real t3190
        real t3191
        real t3192
        real t3194
        real t3196
        real t3198
        real t32
        real t320
        real t3200
        real t3202
        real t3204
        real t3205
        real t3206
        real t3209
        real t3212
        real t3213
        real t3214
        real t3215
        real t3217
        real t3218
        real t322
        real t3220
        real t3222
        real t3223
        real t3224
        real t3226
        real t3227
        real t3228
        real t3230
        real t3232
        real t3233
        real t3234
        real t3235
        real t3237
        real t3238
        real t324
        real t3240
        real t3241
        real t3244
        real t3253
        real t3255
        real t3259
        real t326
        real t3261
        real t3263
        real t3265
        real t3268
        real t3270
        real t3273
        real t3275
        real t328
        real t329
        real t3295
        real t3296
        real t3297
        real t3299
        real t33
        real t330
        real t3302
        real t3304
        real t3308
        real t3309
        real t3321
        real t3327
        real t333
        real t3334
        real t3335
        real t3336
        real t3337
        real t3339
        real t3340
        real t3342
        real t3344
        real t3345
        real t3346
        real t3348
        real t3349
        real t335
        real t3350
        real t3352
        real t3354
        real t3355
        real t3356
        real t3357
        real t3358
        real t3359
        real t336
        real t3360
        real t3361
        real t3362
        real t3363
        real t3366
        real t3369
        real t337
        real t3372
        real t3375
        real t3389
        real t339
        real t3397
        real t3398
        real t34
        real t3400
        real t3402
        real t341
        real t3413
        real t3414
        real t3416
        real t3418
        real t3424
        real t3428
        real t3429
        real t343
        real t3433
        real t344
        real t3445
        real t345
        real t3453
        real t3454
        real t3458
        real t3469
        real t3470
        real t3474
        real t348
        real t3480
        real t3484
        real t3485
        real t3488
        real t349
        integer t3491
        real t3493
        real t3497
        real t35
        real t350
        real t3501
        real t351
        real t3510
        real t3514
        real t352
        real t3533
        real t3534
        real t3538
        real t354
        real t3545
        real t3546
        real t355
        real t3550
        real t356
        real t3561
        real t3564
        real t3566
        real t358
        real t360
        real t3606
        real t361
        real t3610
        real t3617
        real t362
        real t3621
        real t3632
        real t3635
        real t3637
        real t364
        real t365
        real t366
        real t3677
        real t368
        real t3681
        real t3686
        real t3688
        real t3692
        real t3698
        real t37
        real t370
        real t3705
        real t3706
        real t3709
        real t371
        real t3710
        real t3711
        real t3713
        real t3716
        real t3718
        real t372
        real t3722
        real t3723
        real t373
        real t374
        real t3752
        real t3754
        real t3757
        real t3759
        real t376
        real t3763
        real t377
        real t378
        real t3789
        real t3790
        real t3792
        integer t38
        real t380
        real t3801
        real t3805
        real t381
        real t3816
        real t3817
        real t382
        real t3822
        real t3829
        real t3830
        real t3831
        real t3833
        real t3836
        real t3838
        real t384
        real t3842
        real t3843
        real t386
        real t3869
        real t387
        real t3871
        real t3874
        real t3876
        real t388
        real t3880
        real t39
        real t390
        real t391
        real t3913
        real t392
        real t3921
        real t3924
        real t394
        real t3951
        real t3956
        real t396
        real t3961
        real t3964
        real t397
        real t3970
        real t3973
        real t3974
        real t3979
        real t398
        real t3980
        real t3981
        real t3983
        real t3984
        real t3985
        real t3986
        real t3987
        real t3988
        real t3989
        real t3996
        real t3997
        real t3998
        real t4
        real t40
        real t400
        real t4000
        real t4001
        real t4003
        real t4004
        real t4006
        real t4007
        real t4009
        real t401
        real t4010
        real t4012
        real t4013
        real t4014
        real t4016
        real t4018
        real t4019
        real t4020
        real t4023
        real t4024
        real t4025
        real t4026
        real t4027
        real t4029
        real t4030
        real t4032
        real t4033
        real t4035
        real t4036
        real t4037
        real t4038
        real t404
        real t4040
        real t4041
        real t4043
        real t4044
        real t4045
        real t4047
        real t4049
        real t4051
        real t4053
        real t4055
        real t4057
        real t4058
        real t4059
        real t4062
        real t4065
        real t4066
        real t4067
        real t4068
        real t4069
        real t4071
        real t4072
        real t4074
        real t4076
        real t4077
        real t4078
        real t4080
        real t4081
        real t4082
        real t4084
        real t4086
        real t4087
        real t4088
        real t409
        real t4090
        real t4091
        real t4092
        real t4097
        real t4099
        real t41
        real t4104
        real t4106
        real t4110
        real t4112
        real t4114
        real t4116
        real t4119
        real t4120
        real t4121
        real t4123
        real t4126
        real t4128
        real t4132
        real t4133
        real t414
        real t4145
        real t415
        real t4151
        real t4158
        real t416
        real t4160
        real t4163
        real t4165
        real t417
        real t418
        real t4185
        real t4186
        real t4187
        real t4188
        real t4189
        real t4191
        real t4192
        real t4194
        real t4196
        real t4197
        real t4198
        real t420
        real t4200
        real t4201
        real t4202
        real t4204
        real t4206
        real t4207
        real t4208
        real t4209
        real t421
        real t4210
        real t4211
        real t4212
        real t4217
        real t4219
        real t422
        real t4222
        real t4225
        real t4227
        real t4229
        real t4231
        real t4233
        real t4234
        real t4239
        real t424
        real t4242
        real t4244
        real t4247
        real t4250
        real t4254
        real t4257
        real t426
        real t4261
        real t4264
        real t4267
        real t427
        real t4273
        real t4276
        real t4279
        real t428
        real t4282
        real t4285
        real t4288
        real t4289
        real t4291
        real t4292
        real t4294
        real t4295
        real t4297
        real t4298
        real t43
        real t430
        real t4300
        real t4301
        real t4303
        real t4304
        real t4309
        real t433
        real t4342
        real t4344
        real t4346
        real t4348
        real t435
        real t4350
        real t4352
        real t4353
        real t4358
        real t4361
        real t4365
        real t4371
        real t4377
        real t4383
        real t439
        real t44
        real t440
        real t4404
        real t4409
        real t4414
        real t4423
        real t4426
        real t4431
        real t4434
        real t4448
        real t4456
        real t4457
        real t4459
        real t4461
        real t4472
        real t4473
        real t4475
        real t4477
        real t4483
        real t4487
        real t4491
        real t45
        real t4503
        real t4511
        real t4512
        real t4516
        real t452
        real t4527
        real t4528
        real t4532
        real t4538
        real t4542
        real t4547
        integer t4552
        real t4554
        real t4558
        real t4562
        real t458
        real t4581
        real t4582
        real t4586
        real t4593
        real t4594
        real t4598
        real t4635
        real t4639
        real t4646
        real t465
        real t4650
        real t4661
        real t4664
        real t4666
        real t467
        real t469
        real t47
        real t4703
        real t4706
        real t4708
        real t4724
        real t4728
        real t473
        real t4734
        real t4743
        real t4747
        real t475
        real t4759
        real t4760
        real t4766
        real t4767
        real t4769
        real t477
        real t4772
        real t4774
        real t4778
        real t4779
        real t479
        real t48
        real t4805
        real t4807
        real t481
        real t4810
        real t4812
        real t4816
        real t483
        real t4842
        real t4843
        real t485
        real t4853
        real t4857
        real t4868
        real t487
        real t4873
        real t4880
        real t4888
        real t489
        real t4896
        real t4898
        real t4901
        real t4903
        real t4907
        real t491
        real t4933
        real t4934
        real t4936
        real t4939
        real t494
        real t4941
        real t4945
        real t4946
        real t495
        real t496
        real t4972
        real t498
        real t499
        integer t5
        real t500
        real t5005
        real t5010
        real t5013
        real t5019
        real t502
        real t5022
        real t5027
        real t5030
        real t5032
        real t5035
        real t5038
        real t504
        real t5042
        real t5045
        real t5049
        real t505
        real t5052
        real t5055
        real t5058
        real t506
        real t5062
        real t5065
        real t5068
        real t5071
        real t5074
        real t5077
        real t508
        real t5082
        real t5102
        real t511
        real t5116
        real t5121
        real t5124
        real t5128
        real t513
        real t5134
        real t5140
        real t5146
        real t5169
        real t517
        real t5172
        real t5173
        real t5174
        real t5176
        real t5177
        real t5178
        real t5179
        real t518
        real t5180
        real t5181
        real t5182
        real t5183
        real t5184
        real t5185
        real t5188
        real t5191
        real t5192
        real t5194
        real t5195
        real t5197
        real t5198
        real t52
        real t5200
        real t5201
        real t5203
        real t5204
        real t5206
        real t5207
        real t5208
        real t5210
        real t5212
        real t5213
        real t5214
        real t5217
        real t5220
        real t5221
        real t5222
        real t5224
        real t5225
        real t5227
        real t5228
        real t5230
        real t5231
        real t5233
        real t5234
        real t5236
        real t5237
        real t5238
        real t5240
        real t5242
        real t5243
        real t5244
        real t5247
        real t5248
        real t5249
        real t5250
        real t5252
        real t5253
        real t5255
        real t5257
        real t5258
        real t5260
        real t5261
        real t5263
        real t5265
        real t5266
        real t5267
        real t5268
        real t5270
        real t5271
        real t5273
        real t5274
        real t5277
        real t5286
        real t5288
        real t5291
        real t5293
        real t53
        real t530
        real t5313
        real t5315
        real t5318
        real t5320
        real t5340
        real t5342
        real t5346
        real t5348
        real t5350
        real t5352
        real t5355
        real t5356
        real t5357
        real t5358
        real t536
        real t5360
        real t5361
        real t5363
        real t5365
        real t5366
        real t5368
        real t5369
        real t5371
        real t5373
        real t5374
        real t5375
        real t5376
        real t5377
        real t5378
        real t5379
        real t5380
        real t5381
        real t5382
        real t5385
        real t5388
        real t5391
        real t5394
        real t54
        real t5416
        real t5417
        real t5421
        real t5422
        real t5426
        real t543
        real t5446
        real t5447
        real t545
        real t5451
        real t5452
        real t5455
        real t5458
        real t5462
        real t5469
        real t5473
        real t5484
        real t5487
        real t5489
        real t549
        real t55
        real t551
        integer t5529
        real t553
        real t5531
        real t5535
        real t5539
        real t555
        real t5557
        real t5561
        real t5571
        real t558
        real t5582
        real t559
        real t5599
        real t56
        real t560
        real t5609
        real t562
        real t5620
        real t5623
        real t5625
        real t563
        real t564
        real t5642
        real t5646
        real t5651
        real t5653
        real t5657
        real t566
        real t5663
        real t5670
        real t5671
        real t5674
        real t5678
        real t568
        real t5680
        real t5683
        real t5685
        real t5689
        real t569
        real t57
        real t570
        real t571
        real t5715
        real t5717
        real t572
        real t5720
        real t5722
        real t5726
        real t574
        real t575
        real t5752
        real t5753
        real t5755
        real t577
        real t5777
        real t5778
        real t578
        real t5783
        real t5790
        real t5791
        real t5793
        real t5796
        real t5798
        real t58
        real t580
        real t5802
        real t581
        real t582
        real t5828
        real t583
        real t5830
        real t5833
        real t5835
        real t5839
        real t585
        real t586
        real t5872
        real t588
        real t5880
        real t5883
        real t59
        real t5910
        real t5915
        real t592
        real t5920
        real t5923
        real t5929
        real t5932
        real t5933
        real t5938
        real t5939
        real t594
        real t5940
        real t5942
        real t5943
        real t5944
        real t5945
        real t5946
        real t5947
        real t5948
        real t5955
        real t5956
        real t5957
        real t5959
        real t596
        real t5960
        real t5962
        real t5963
        real t5965
        real t5966
        real t5968
        real t5969
        real t5971
        real t5972
        real t5973
        real t5975
        real t5977
        real t5978
        real t5979
        real t598
        real t5982
        real t5983
        real t5984
        real t5986
        real t5987
        real t5989
        real t5990
        real t5992
        real t5993
        real t5995
        real t5996
        real t5998
        real t5999
        real t6
        real t600
        real t6000
        real t6002
        real t6004
        real t6005
        real t6006
        real t6009
        real t6012
        real t6013
        real t6014
        real t6015
        real t6016
        real t6018
        real t6019
        real t602
        real t6021
        real t6023
        real t6024
        real t6026
        real t6027
        real t6029
        real t6031
        real t6032
        real t6033
        real t6035
        real t6036
        real t6037
        real t604
        real t6042
        real t6044
        real t6049
        real t6051
        real t6055
        real t6057
        real t6059
        real t6061
        real t6064
        real t6066
        real t6069
        real t607
        real t6071
        real t608
        real t609
        real t6091
        real t6093
        real t6096
        real t6098
        real t61
        real t611
        real t6118
        real t6119
        real t612
        real t6120
        real t6121
        real t6122
        real t6124
        real t6125
        real t6127
        real t6129
        real t613
        real t6130
        real t6132
        real t6133
        real t6135
        real t6137
        real t6138
        real t6139
        real t6140
        real t6141
        real t6142
        real t6143
        real t6148
        real t615
        real t6150
        real t6153
        real t6156
        real t6158
        real t6160
        real t6162
        real t6164
        real t6165
        real t617
        real t6170
        real t6173
        real t6175
        real t6178
        real t618
        real t6181
        real t6185
        real t6188
        real t619
        real t6192
        real t6195
        real t6198
        real t62
        real t620
        real t6204
        real t6207
        real t621
        real t6210
        real t6213
        real t6216
        real t6219
        real t6220
        real t6222
        real t6223
        real t6225
        real t6226
        real t6228
        real t6229
        real t623
        real t6231
        real t6232
        real t6234
        real t6235
        real t624
        real t6240
        real t626
        real t627
        real t6273
        real t6275
        real t6277
        real t6279
        real t6281
        real t6283
        real t6284
        real t6289
        real t629
        real t6292
        real t6296
        real t63
        real t630
        real t6302
        real t6308
        real t631
        real t6314
        real t632
        real t6335
        real t634
        real t6340
        real t6345
        real t635
        real t6354
        real t6357
        real t6362
        real t6365
        real t637
        real t6387
        real t6388
        real t6392
        real t6396
        real t64
        real t641
        real t6416
        real t6417
        real t6421
        real t6426
        real t6429
        real t643
        real t6433
        real t6440
        real t6444
        real t645
        real t6455
        real t6458
        real t6460
        real t647
        real t649
        integer t6500
        real t6502
        real t6506
        real t651
        real t6510
        real t6520
        real t6524
        real t653
        real t6531
        real t6535
        real t6546
        real t6549
        real t6551
        real t656
        real t657
        real t658
        real t659
        real t6590
        real t6594
        real t66
        real t660
        real t6611
        real t662
        real t6620
        real t6624
        real t663
        real t6636
        real t6637
        real t664
        real t6640
        real t6642
        real t6645
        real t6647
        real t6651
        real t666
        real t6677
        real t6679
        real t668
        real t6682
        real t6684
        real t6688
        real t669
        real t67
        real t670
        real t6717
        real t6718
        real t672
        real t673
        real t674
        real t6741
        real t6746
        real t6753
        real t676
        real t6761
        real t6769
        real t6771
        real t6774
        real t6776
        real t678
        real t6780
        real t679
        real t680
        real t6806
        real t6808
        real t681
        real t6811
        real t6813
        real t6817
        real t682
        real t683
        real t684
        real t6843
        real t685
        real t686
        real t6876
        real t688
        real t6881
        real t6884
        real t689
        real t6890
        real t6893
        real t6898
        real t690
        real t6901
        real t6903
        real t6906
        real t6907
        real t6909
        real t6913
        real t6916
        real t692
        real t6920
        real t6923
        real t6926
        real t6929
        real t6933
        real t6936
        real t6939
        real t694
        real t6942
        real t6945
        real t6948
        real t695
        real t6953
        real t696
        real t6973
        real t698
        real t6987
        real t699
        real t6992
        real t6995
        real t6999
        real t7
        real t70
        real t700
        real t7005
        real t7011
        real t7017
        real t702
        real t704
        real t705
        real t706
        real t707
        real t708
        real t709
        real t71
        real t712
        real t715
        real t718
        real t72
        real t721
        real t724
        real t725
        real t729
        real t731
        real t732
        real t734
        real t735
        real t737
        real t739
        real t74
        real t740
        real t744
        real t749
        real t75
        real t753
        real t755
        real t756
        real t758
        real t76
        real t760
        real t761
        real t765
        real t77
        real t770
        real t776
        real t778
        real t779
        real t78
        real t781
        real t783
        real t79
        real t791
        real t795
        real t797
        real t798
        real t8
        real t80
        real t800
        real t802
        real t81
        real t810
        real t817
        real t819
        real t82
        real t820
        real t822
        real t824
        real t825
        real t826
        real t828
        real t829
        real t830
        real t832
        real t834
        real t835
        real t837
        real t838
        real t84
        real t840
        real t842
        real t843
        real t844
        real t846
        real t847
        real t848
        real t85
        real t850
        real t852
        real t853
        real t855
        real t857
        real t858
        real t86
        real t860
        real t862
        real t863
        real t865
        real t866
        real t868
        real t87
        real t870
        real t871
        real t873
        real t874
        real t876
        real t878
        real t879
        real t88
        real t881
        real t882
        real t884
        real t886
        real t887
        real t889
        real t89
        real t891
        real t892
        real t896
        real t897
        real t9
        real t901
        real t902
        real t903
        real t907
        real t909
        real t910
        real t913
        real t915
        real t917
        real t918
        real t92
        real t922
        real t927
        real t931
        real t934
        real t936
        real t938
        real t939
        real t943
        real t948
        real t95
        real t954
        real t957
        real t959
        real t96
        real t961
        real t969
        real t973
        real t976
        real t978
        real t98
        real t980
        real t988
        real t995
        real t997
        real t998
        t1 = u(i,j,k,n)
        t2 = ut(i,j,k,n)
        t4 = cc ** 2
        t5 = i + 1
        t6 = u(t5,j,k,n)
        t7 = t6 - t1
        t8 = 0.1E1 / dx
        t9 = t7 * t8
        t10 = i + 2
        t11 = u(t10,j,k,n)
        t12 = t11 - t6
        t13 = t12 * t8
        t15 = (t13 - t9) * t8
        t16 = i - 1
        t17 = u(t16,j,k,n)
        t18 = t1 - t17
        t19 = t18 * t8
        t21 = (t9 - t19) * t8
        t22 = t15 - t21
        t25 = dx ** 2
        t26 = t25 * dx
        t27 = i + 3
        t28 = u(t27,j,k,n)
        t29 = t28 - t11
        t30 = t29 * t8
        t32 = (t30 - t13) * t8
        t33 = t32 - t15
        t34 = t33 * t8
        t35 = t22 * t8
        t37 = (t34 - t35) * t8
        t38 = i - 2
        t39 = u(t38,j,k,n)
        t40 = t17 - t39
        t41 = t40 * t8
        t43 = (t19 - t41) * t8
        t44 = t21 - t43
        t45 = t44 * t8
        t47 = (t35 - t45) * t8
        t48 = t37 - t47
        t52 = t4 * (t9 - dx * t22 / 0.24E2 + 0.3E1 / 0.640E3 * t26 * t48
     #)
        t53 = t4 * dt
        t54 = ut(t5,j,k,n)
        t55 = t54 - t2
        t56 = t55 * t8
        t57 = ut(t10,j,k,n)
        t58 = t57 - t54
        t59 = t58 * t8
        t61 = (t59 - t56) * t8
        t62 = ut(t16,j,k,n)
        t63 = t2 - t62
        t64 = t63 * t8
        t66 = (t56 - t64) * t8
        t67 = t61 - t66
        t70 = ut(t27,j,k,n)
        t71 = t70 - t57
        t72 = t71 * t8
        t74 = (t72 - t59) * t8
        t75 = t74 - t61
        t76 = t75 * t8
        t77 = t67 * t8
        t78 = t76 - t77
        t79 = t78 * t8
        t80 = ut(t38,j,k,n)
        t81 = t62 - t80
        t82 = t81 * t8
        t84 = (t64 - t82) * t8
        t85 = t66 - t84
        t86 = t85 * t8
        t87 = t77 - t86
        t88 = t87 * t8
        t89 = t79 - t88
        t92 = t56 - dx * t67 / 0.24E2 + 0.3E1 / 0.640E3 * t26 * t89
        t95 = dt ** 2
        t96 = t4 * t95
        t98 = t4 * t12 * t8
        t100 = t4 * t7 * t8
        t102 = (t98 - t100) * t8
        t104 = t4 * t33 * t8
        t106 = t4 * t22 * t8
        t107 = t104 - t106
        t108 = t107 * t8
        t110 = t4 * t29 * t8
        t112 = (t110 - t98) * t8
        t114 = (t112 - t102) * t8
        t116 = t4 * t18 * t8
        t118 = (t100 - t116) * t8
        t120 = (t102 - t118) * t8
        t121 = t114 - t120
        t122 = t121 * t8
        t125 = t25 * (t108 + t122) / 0.24E2
        t126 = j + 1
        t127 = u(t5,t126,k,n)
        t128 = t127 - t6
        t130 = 0.1E1 / dy
        t131 = t4 * t128 * t130
        t132 = j - 1
        t133 = u(t5,t132,k,n)
        t134 = t6 - t133
        t136 = t4 * t134 * t130
        t138 = (t131 - t136) * t130
        t139 = dy ** 2
        t140 = j + 2
        t141 = u(t5,t140,k,n)
        t142 = t141 - t127
        t143 = t142 * t130
        t144 = t128 * t130
        t146 = (t143 - t144) * t130
        t147 = t134 * t130
        t149 = (t144 - t147) * t130
        t150 = t146 - t149
        t152 = t4 * t150 * t130
        t153 = j - 2
        t154 = u(t5,t153,k,n)
        t155 = t133 - t154
        t156 = t155 * t130
        t158 = (t147 - t156) * t130
        t159 = t149 - t158
        t161 = t4 * t159 * t130
        t162 = t152 - t161
        t163 = t162 * t130
        t165 = t4 * t142 * t130
        t167 = (t165 - t131) * t130
        t169 = (t167 - t138) * t130
        t171 = t4 * t155 * t130
        t173 = (t136 - t171) * t130
        t175 = (t138 - t173) * t130
        t176 = t169 - t175
        t177 = t176 * t130
        t180 = t139 * (t163 + t177) / 0.24E2
        t181 = k + 1
        t182 = u(t5,j,t181,n)
        t183 = t182 - t6
        t185 = 0.1E1 / dz
        t186 = t4 * t183 * t185
        t187 = k - 1
        t188 = u(t5,j,t187,n)
        t189 = t6 - t188
        t191 = t4 * t189 * t185
        t193 = (t186 - t191) * t185
        t194 = dz ** 2
        t195 = k + 2
        t196 = u(t5,j,t195,n)
        t197 = t196 - t182
        t198 = t197 * t185
        t199 = t183 * t185
        t201 = (t198 - t199) * t185
        t202 = t189 * t185
        t204 = (t199 - t202) * t185
        t205 = t201 - t204
        t207 = t4 * t205 * t185
        t208 = k - 2
        t209 = u(t5,j,t208,n)
        t210 = t188 - t209
        t211 = t210 * t185
        t213 = (t202 - t211) * t185
        t214 = t204 - t213
        t216 = t4 * t214 * t185
        t217 = t207 - t216
        t218 = t217 * t185
        t220 = t4 * t197 * t185
        t222 = (t220 - t186) * t185
        t224 = (t222 - t193) * t185
        t226 = t4 * t210 * t185
        t228 = (t191 - t226) * t185
        t230 = (t193 - t228) * t185
        t231 = t224 - t230
        t232 = t231 * t185
        t235 = t194 * (t218 + t232) / 0.24E2
        t236 = u(i,t126,k,n)
        t237 = t236 - t1
        t239 = t4 * t237 * t130
        t240 = u(i,t132,k,n)
        t241 = t1 - t240
        t243 = t4 * t241 * t130
        t245 = (t239 - t243) * t130
        t246 = u(i,t140,k,n)
        t247 = t246 - t236
        t248 = t247 * t130
        t249 = t237 * t130
        t251 = (t248 - t249) * t130
        t252 = t241 * t130
        t254 = (t249 - t252) * t130
        t255 = t251 - t254
        t257 = t4 * t255 * t130
        t258 = u(i,t153,k,n)
        t259 = t240 - t258
        t260 = t259 * t130
        t262 = (t252 - t260) * t130
        t263 = t254 - t262
        t265 = t4 * t263 * t130
        t266 = t257 - t265
        t267 = t266 * t130
        t269 = t4 * t247 * t130
        t271 = (t269 - t239) * t130
        t273 = (t271 - t245) * t130
        t275 = t4 * t259 * t130
        t277 = (t243 - t275) * t130
        t279 = (t245 - t277) * t130
        t280 = t273 - t279
        t281 = t280 * t130
        t284 = t139 * (t267 + t281) / 0.24E2
        t285 = u(i,j,t181,n)
        t286 = t285 - t1
        t288 = t4 * t286 * t185
        t289 = u(i,j,t187,n)
        t290 = t1 - t289
        t292 = t4 * t290 * t185
        t294 = (t288 - t292) * t185
        t295 = u(i,j,t195,n)
        t296 = t295 - t285
        t297 = t296 * t185
        t298 = t286 * t185
        t300 = (t297 - t298) * t185
        t301 = t290 * t185
        t303 = (t298 - t301) * t185
        t304 = t300 - t303
        t306 = t4 * t304 * t185
        t307 = u(i,j,t208,n)
        t308 = t289 - t307
        t309 = t308 * t185
        t311 = (t301 - t309) * t185
        t312 = t303 - t311
        t314 = t4 * t312 * t185
        t315 = t306 - t314
        t316 = t315 * t185
        t318 = t4 * t296 * t185
        t320 = (t318 - t288) * t185
        t322 = (t320 - t294) * t185
        t324 = t4 * t308 * t185
        t326 = (t292 - t324) * t185
        t328 = (t294 - t326) * t185
        t329 = t322 - t328
        t330 = t329 * t185
        t333 = t194 * (t316 + t330) / 0.24E2
        t335 = t4 * t44 * t8
        t336 = t106 - t335
        t337 = t336 * t8
        t339 = t4 * t40 * t8
        t341 = (t116 - t339) * t8
        t343 = (t118 - t341) * t8
        t344 = t120 - t343
        t345 = t344 * t8
        t348 = t25 * (t337 + t345) / 0.24E2
        t349 = t102 - t125 + t138 - t180 + t193 - t235 - t118 - t245 + t
     #284 - t294 + t333 + t348
        t350 = t349 * t8
        t351 = u(t10,t126,k,n)
        t352 = t351 - t11
        t354 = t4 * t352 * t130
        t355 = u(t10,t132,k,n)
        t356 = t11 - t355
        t358 = t4 * t356 * t130
        t360 = (t354 - t358) * t130
        t361 = u(t10,j,t181,n)
        t362 = t361 - t11
        t364 = t4 * t362 * t185
        t365 = u(t10,j,t187,n)
        t366 = t11 - t365
        t368 = t4 * t366 * t185
        t370 = (t364 - t368) * t185
        t371 = t112 + t360 + t370 - t102 - t138 - t193
        t372 = t371 * t8
        t373 = t102 + t138 + t193 - t118 - t245 - t294
        t374 = t373 * t8
        t376 = (t372 - t374) * t8
        t377 = u(t16,t126,k,n)
        t378 = t377 - t17
        t380 = t4 * t378 * t130
        t381 = u(t16,t132,k,n)
        t382 = t17 - t381
        t384 = t4 * t382 * t130
        t386 = (t380 - t384) * t130
        t387 = u(t16,j,t181,n)
        t388 = t387 - t17
        t390 = t4 * t388 * t185
        t391 = u(t16,j,t187,n)
        t392 = t17 - t391
        t394 = t4 * t392 * t185
        t396 = (t390 - t394) * t185
        t397 = t118 + t245 + t294 - t341 - t386 - t396
        t398 = t397 * t8
        t400 = (t374 - t398) * t8
        t401 = t376 - t400
        t404 = t350 - dx * t401 / 0.24E2
        t409 = t122 - t345
        t414 = t25 * ((t102 - t125 - t118 + t348) * t8 - dx * t409 / 0.2
     #4E2) / 0.24E2
        t415 = t95 * dt
        t416 = t4 * t415
        t417 = ut(t5,j,t181,n)
        t418 = t417 - t54
        t420 = t4 * t418 * t185
        t421 = ut(t5,j,t187,n)
        t422 = t54 - t421
        t424 = t4 * t422 * t185
        t426 = (t420 - t424) * t185
        t427 = ut(t5,j,t195,n)
        t428 = t427 - t417
        t430 = t418 * t185
        t433 = t422 * t185
        t435 = (t430 - t433) * t185
        t439 = ut(t5,j,t208,n)
        t440 = t421 - t439
        t452 = (t4 * t428 * t185 - t420) * t185
        t458 = (t424 - t4 * t440 * t185) * t185
        t465 = t194 * ((t4 * ((t428 * t185 - t430) * t185 - t435) * t185
     # - t4 * (t435 - (t433 - t440 * t185) * t185) * t185) * t185 + ((t4
     #52 - t426) * t185 - (t426 - t458) * t185) * t185) / 0.24E2
        t467 = t4 * t75 * t8
        t469 = t4 * t67 * t8
        t473 = t4 * t71 * t8
        t475 = t4 * t58 * t8
        t477 = (t473 - t475) * t8
        t479 = t4 * t55 * t8
        t481 = (t475 - t479) * t8
        t483 = (t477 - t481) * t8
        t485 = t4 * t63 * t8
        t487 = (t479 - t485) * t8
        t489 = (t481 - t487) * t8
        t491 = (t483 - t489) * t8
        t494 = t25 * ((t467 - t469) * t8 + t491) / 0.24E2
        t495 = ut(t5,t126,k,n)
        t496 = t495 - t54
        t498 = t4 * t496 * t130
        t499 = ut(t5,t132,k,n)
        t500 = t54 - t499
        t502 = t4 * t500 * t130
        t504 = (t498 - t502) * t130
        t505 = ut(t5,t140,k,n)
        t506 = t505 - t495
        t508 = t496 * t130
        t511 = t500 * t130
        t513 = (t508 - t511) * t130
        t517 = ut(t5,t153,k,n)
        t518 = t499 - t517
        t530 = (t4 * t506 * t130 - t498) * t130
        t536 = (t502 - t4 * t518 * t130) * t130
        t543 = t139 * ((t4 * ((t506 * t130 - t508) * t130 - t513) * t130
     # - t4 * (t513 - (t511 - t518 * t130) * t130) * t130) * t130 + ((t5
     #30 - t504) * t130 - (t504 - t536) * t130) * t130) / 0.24E2
        t545 = t4 * t85 * t8
        t549 = t4 * t81 * t8
        t551 = (t485 - t549) * t8
        t553 = (t487 - t551) * t8
        t555 = (t489 - t553) * t8
        t558 = t25 * ((t469 - t545) * t8 + t555) / 0.24E2
        t559 = ut(i,t126,k,n)
        t560 = t559 - t2
        t562 = t4 * t560 * t130
        t563 = ut(i,t132,k,n)
        t564 = t2 - t563
        t566 = t4 * t564 * t130
        t568 = (t562 - t566) * t130
        t569 = ut(i,t140,k,n)
        t570 = t569 - t559
        t571 = t570 * t130
        t572 = t560 * t130
        t574 = (t571 - t572) * t130
        t575 = t564 * t130
        t577 = (t572 - t575) * t130
        t578 = t574 - t577
        t580 = t4 * t578 * t130
        t581 = ut(i,t153,k,n)
        t582 = t563 - t581
        t583 = t582 * t130
        t585 = (t575 - t583) * t130
        t586 = t577 - t585
        t588 = t4 * t586 * t130
        t592 = t4 * t570 * t130
        t594 = (t592 - t562) * t130
        t596 = (t594 - t568) * t130
        t598 = t4 * t582 * t130
        t600 = (t566 - t598) * t130
        t602 = (t568 - t600) * t130
        t604 = (t596 - t602) * t130
        t607 = t139 * ((t580 - t588) * t130 + t604) / 0.24E2
        t608 = ut(i,j,t181,n)
        t609 = t608 - t2
        t611 = t4 * t609 * t185
        t612 = ut(i,j,t187,n)
        t613 = t2 - t612
        t615 = t4 * t613 * t185
        t617 = (t611 - t615) * t185
        t618 = ut(i,j,t195,n)
        t619 = t618 - t608
        t620 = t619 * t185
        t621 = t609 * t185
        t623 = (t620 - t621) * t185
        t624 = t613 * t185
        t626 = (t621 - t624) * t185
        t627 = t623 - t626
        t629 = t4 * t627 * t185
        t630 = ut(i,j,t208,n)
        t631 = t612 - t630
        t632 = t631 * t185
        t634 = (t624 - t632) * t185
        t635 = t626 - t634
        t637 = t4 * t635 * t185
        t641 = t4 * t619 * t185
        t643 = (t641 - t611) * t185
        t645 = (t643 - t617) * t185
        t647 = t4 * t631 * t185
        t649 = (t615 - t647) * t185
        t651 = (t617 - t649) * t185
        t653 = (t645 - t651) * t185
        t656 = t194 * ((t629 - t637) * t185 + t653) / 0.24E2
        t657 = t426 - t465 - t494 + t481 + t504 - t543 - t487 + t558 - t
     #568 + t607 - t617 + t656
        t658 = t657 * t8
        t659 = ut(t10,t126,k,n)
        t660 = t659 - t57
        t662 = t4 * t660 * t130
        t663 = ut(t10,t132,k,n)
        t664 = t57 - t663
        t666 = t4 * t664 * t130
        t668 = (t662 - t666) * t130
        t669 = ut(t10,j,t181,n)
        t670 = t669 - t57
        t672 = t4 * t670 * t185
        t673 = ut(t10,j,t187,n)
        t674 = t57 - t673
        t676 = t4 * t674 * t185
        t678 = (t672 - t676) * t185
        t679 = t477 + t668 + t678 - t481 - t504 - t426
        t680 = t679 * t8
        t681 = t481 + t504 + t426 - t487 - t568 - t617
        t682 = t681 * t8
        t683 = t680 - t682
        t684 = t683 * t8
        t685 = ut(t16,t126,k,n)
        t686 = t685 - t62
        t688 = t4 * t686 * t130
        t689 = ut(t16,t132,k,n)
        t690 = t62 - t689
        t692 = t4 * t690 * t130
        t694 = (t688 - t692) * t130
        t695 = ut(t16,j,t181,n)
        t696 = t695 - t62
        t698 = t4 * t696 * t185
        t699 = ut(t16,j,t187,n)
        t700 = t62 - t699
        t702 = t4 * t700 * t185
        t704 = (t698 - t702) * t185
        t705 = t487 + t568 + t617 - t551 - t694 - t704
        t706 = t705 * t8
        t707 = t682 - t706
        t708 = t707 * t8
        t709 = t684 - t708
        t712 = t658 - dx * t709 / 0.24E2
        t715 = dt * t25
        t718 = t491 - t555
        t721 = (t481 - t494 - t487 + t558) * t8 - dx * t718 / 0.24E2
        t724 = t95 ** 2
        t725 = t4 * t724
        t729 = t4 * t373 * t8
        t731 = (t4 * t371 * t8 - t729) * t8
        t732 = t351 - t127
        t734 = t4 * t732 * t8
        t735 = t127 - t236
        t737 = t4 * t735 * t8
        t739 = (t734 - t737) * t8
        t740 = u(t5,t126,t181,n)
        t744 = u(t5,t126,t187,n)
        t749 = (t4 * (t740 - t127) * t185 - t4 * (t127 - t744) * t185) *
     # t185
        t753 = t355 - t133
        t755 = t4 * t753 * t8
        t756 = t133 - t240
        t758 = t4 * t756 * t8
        t760 = (t755 - t758) * t8
        t761 = u(t5,t132,t181,n)
        t765 = u(t5,t132,t187,n)
        t770 = (t4 * (t761 - t133) * t185 - t4 * (t133 - t765) * t185) *
     # t185
        t776 = t361 - t182
        t778 = t4 * t776 * t8
        t779 = t182 - t285
        t781 = t4 * t779 * t8
        t783 = (t778 - t781) * t8
        t791 = (t4 * (t740 - t182) * t130 - t4 * (t182 - t761) * t130) *
     # t130
        t795 = t365 - t188
        t797 = t4 * t795 * t8
        t798 = t188 - t289
        t800 = t4 * t798 * t8
        t802 = (t797 - t800) * t8
        t810 = (t4 * (t744 - t188) * t130 - t4 * (t188 - t765) * t130) *
     # t130
        t817 = t4 * t397 * t8
        t819 = (t729 - t817) * t8
        t820 = t236 - t377
        t822 = t4 * t820 * t8
        t824 = (t737 - t822) * t8
        t825 = u(i,t126,t181,n)
        t826 = t825 - t236
        t828 = t4 * t826 * t185
        t829 = u(i,t126,t187,n)
        t830 = t236 - t829
        t832 = t4 * t830 * t185
        t834 = (t828 - t832) * t185
        t835 = t824 + t271 + t834 - t118 - t245 - t294
        t837 = t4 * t835 * t130
        t838 = t240 - t381
        t840 = t4 * t838 * t8
        t842 = (t758 - t840) * t8
        t843 = u(i,t132,t181,n)
        t844 = t843 - t240
        t846 = t4 * t844 * t185
        t847 = u(i,t132,t187,n)
        t848 = t240 - t847
        t850 = t4 * t848 * t185
        t852 = (t846 - t850) * t185
        t853 = t118 + t245 + t294 - t842 - t277 - t852
        t855 = t4 * t853 * t130
        t857 = (t837 - t855) * t130
        t858 = t285 - t387
        t860 = t4 * t858 * t8
        t862 = (t781 - t860) * t8
        t863 = t825 - t285
        t865 = t4 * t863 * t130
        t866 = t285 - t843
        t868 = t4 * t866 * t130
        t870 = (t865 - t868) * t130
        t871 = t862 + t870 + t320 - t118 - t245 - t294
        t873 = t4 * t871 * t185
        t874 = t289 - t391
        t876 = t4 * t874 * t8
        t878 = (t800 - t876) * t8
        t879 = t829 - t289
        t881 = t4 * t879 * t130
        t882 = t289 - t847
        t884 = t4 * t882 * t130
        t886 = (t881 - t884) * t130
        t887 = t118 + t245 + t294 - t878 - t886 - t326
        t889 = t4 * t887 * t185
        t891 = (t873 - t889) * t185
        t892 = t731 + (t4 * (t739 + t167 + t749 - t102 - t138 - t193) * 
     #t130 - t4 * (t102 + t138 + t193 - t760 - t173 - t770) * t130) * t1
     #30 + (t4 * (t783 + t791 + t222 - t102 - t138 - t193) * t185 - t4 *
     # (t102 + t138 + t193 - t802 - t810 - t228) * t185) * t185 - t819 -
     # t857 - t891
        t896 = t95 * dx
        t897 = t731 - t819
        t901 = 0.7E1 / 0.5760E4 * t26 * t409
        t902 = t724 * dt
        t903 = t4 * t902
        t907 = t4 * t681 * t8
        t909 = (t4 * t679 * t8 - t907) * t8
        t910 = t659 - t495
        t913 = t495 - t559
        t915 = t4 * t913 * t8
        t917 = (t4 * t910 * t8 - t915) * t8
        t918 = ut(t5,t126,t181,n)
        t922 = ut(t5,t126,t187,n)
        t927 = (t4 * (t918 - t495) * t185 - t4 * (t495 - t922) * t185) *
     # t185
        t931 = t663 - t499
        t934 = t499 - t563
        t936 = t4 * t934 * t8
        t938 = (t4 * t931 * t8 - t936) * t8
        t939 = ut(t5,t132,t181,n)
        t943 = ut(t5,t132,t187,n)
        t948 = (t4 * (t939 - t499) * t185 - t4 * (t499 - t943) * t185) *
     # t185
        t954 = t669 - t417
        t957 = t417 - t608
        t959 = t4 * t957 * t8
        t961 = (t4 * t954 * t8 - t959) * t8
        t969 = (t4 * (t918 - t417) * t130 - t4 * (t417 - t939) * t130) *
     # t130
        t973 = t673 - t421
        t976 = t421 - t612
        t978 = t4 * t976 * t8
        t980 = (t4 * t973 * t8 - t978) * t8
        t988 = (t4 * (t922 - t421) * t130 - t4 * (t421 - t943) * t130) *
     # t130
        t995 = t4 * t705 * t8
        t997 = (t907 - t995) * t8
        t998 = t559 - t685
        t1000 = t4 * t998 * t8
        t1002 = (t915 - t1000) * t8
        t1003 = ut(i,t126,t181,n)
        t1004 = t1003 - t559
        t1006 = t4 * t1004 * t185
        t1007 = ut(i,t126,t187,n)
        t1008 = t559 - t1007
        t1010 = t4 * t1008 * t185
        t1012 = (t1006 - t1010) * t185
        t1013 = t1002 + t594 + t1012 - t487 - t568 - t617
        t1015 = t4 * t1013 * t130
        t1016 = t563 - t689
        t1018 = t4 * t1016 * t8
        t1020 = (t936 - t1018) * t8
        t1021 = ut(i,t132,t181,n)
        t1022 = t1021 - t563
        t1024 = t4 * t1022 * t185
        t1025 = ut(i,t132,t187,n)
        t1026 = t563 - t1025
        t1028 = t4 * t1026 * t185
        t1030 = (t1024 - t1028) * t185
        t1031 = t487 + t568 + t617 - t1020 - t600 - t1030
        t1033 = t4 * t1031 * t130
        t1035 = (t1015 - t1033) * t130
        t1036 = t608 - t695
        t1038 = t4 * t1036 * t8
        t1040 = (t959 - t1038) * t8
        t1041 = t1003 - t608
        t1043 = t4 * t1041 * t130
        t1044 = t608 - t1021
        t1046 = t4 * t1044 * t130
        t1048 = (t1043 - t1046) * t130
        t1049 = t1040 + t1048 + t643 - t487 - t568 - t617
        t1051 = t4 * t1049 * t185
        t1052 = t612 - t699
        t1054 = t4 * t1052 * t8
        t1056 = (t978 - t1054) * t8
        t1057 = t1007 - t612
        t1059 = t4 * t1057 * t130
        t1060 = t612 - t1025
        t1062 = t4 * t1060 * t130
        t1064 = (t1059 - t1062) * t130
        t1065 = t487 + t568 + t617 - t1056 - t1064 - t649
        t1067 = t4 * t1065 * t185
        t1069 = (t1051 - t1067) * t185
        t1070 = t909 + (t4 * (t917 + t530 + t927 - t481 - t504 - t426) *
     # t130 - t4 * (t481 + t504 + t426 - t938 - t536 - t948) * t130) * t
     #130 + (t4 * (t961 + t969 + t452 - t481 - t504 - t426) * t185 - t4 
     #* (t481 + t504 + t426 - t980 - t988 - t458) * t185) * t185 - t997 
     #- t1035 - t1069
        t1074 = t415 * dx
        t1075 = t909 - t997
        t1078 = dt * t26
        t1085 = t139 * dy
        t1086 = j + 3
        t1087 = u(t5,t1086,k,n)
        t1088 = t1087 - t141
        t1092 = (t1088 * t130 - t143) * t130 - t146
        t1099 = j - 3
        t1100 = u(t5,t1099,k,n)
        t1101 = t154 - t1100
        t1105 = t158 - (t156 - t1101 * t130) * t130
        t1138 = t194 * dz
        t1139 = k + 3
        t1140 = u(t5,j,t1139,n)
        t1141 = t1140 - t196
        t1145 = (t1141 * t185 - t198) * t185 - t201
        t1152 = k - 3
        t1153 = u(t5,j,t1152,n)
        t1154 = t209 - t1153
        t1158 = t213 - (t211 - t1154 * t185) * t185
        t1169 = t205 * t185
        t1172 = t214 * t185
        t1174 = (t1169 - t1172) * t185
        t1212 = i + 4
        t1214 = u(t1212,j,k,n) - t28
        t1218 = (t1214 * t8 - t30) * t8 - t32
        t1222 = (t4 * t1218 * t8 - t104) * t8
        t1226 = (t108 - t337) * t8
        t1237 = t4 * t48 * t8
        t1244 = (t4 * t1214 * t8 - t110) * t8
        t1248 = ((t1244 - t112) * t8 - t114) * t8
        t1251 = t409 * t8
        t1258 = t150 * t130
        t1261 = t159 * t130
        t1263 = (t1258 - t1261) * t130
        t1280 = t102 + t138 + t193 - dy * t162 / 0.24E2 - dy * t176 / 0.
     #24E2 + t1085 * (((t4 * t1092 * t130 - t152) * t130 - t163) * t130 
     #- (t163 - (t161 - t4 * t1105 * t130) * t130) * t130) / 0.576E3 + 0
     #.3E1 / 0.640E3 * t1085 * (((((t4 * t1088 * t130 - t165) * t130 - t
     #167) * t130 - t169) * t130 - t177) * t130 - (t177 - (t175 - (t173 
     #- (t171 - t4 * t1101 * t130) * t130) * t130) * t130) * t130) + t11
     #38 * (((t4 * t1145 * t185 - t207) * t185 - t218) * t185 - (t218 - 
     #(t216 - t4 * t1158 * t185) * t185) * t185) / 0.576E3 + 0.3E1 / 0.6
     #40E3 * t1138 * (t4 * ((t1145 * t185 - t1169) * t185 - t1174) * t18
     #5 - t4 * (t1174 - (t1172 - t1158 * t185) * t185) * t185) + 0.3E1 /
     # 0.640E3 * t1138 * (((((t4 * t1141 * t185 - t220) * t185 - t222) *
     # t185 - t224) * t185 - t232) * t185 - (t232 - (t230 - (t228 - (t22
     #6 - t4 * t1154 * t185) * t185) * t185) * t185) * t185) - dx * t107
     # / 0.24E2 + t26 * ((t1222 - t108) * t8 - t1226) / 0.576E3 + 0.3E1 
     #/ 0.640E3 * t26 * (t4 * ((t1218 * t8 - t34) * t8 - t37) * t8 - t12
     #37) + 0.3E1 / 0.640E3 * t26 * ((t1248 - t122) * t8 - t1251) - dz *
     # t231 / 0.24E2 + 0.3E1 / 0.640E3 * t1085 * (t4 * ((t1092 * t130 - 
     #t1258) * t130 - t1263) * t130 - t4 * (t1263 - (t1261 - t1105 * t13
     #0) * t130) * t130) - dx * t121 / 0.24E2 - dz * t217 / 0.24E2
        t1284 = t56 / 0.2E1
        t1289 = t25 ** 2
        t1291 = ut(t1212,j,k,n) - t70
        t1295 = (t1291 * t8 - t72) * t8 - t74
        t1301 = t89 * t8
        t1308 = dx * (t59 / 0.2E1 + t1284 - t25 * (t76 / 0.2E1 + t77 / 0
     #.2E1) / 0.6E1 + t1289 * (((t1295 * t8 - t76) * t8 - t79) * t8 / 0.
     #2E1 + t1301 / 0.2E1) / 0.30E2) / 0.2E1
        t1309 = t426 - t465 - t494 + t481 + t504 - t543
        t1312 = dt * dx
        t1316 = u(t10,t140,k,n)
        t1317 = t1316 - t351
        t1319 = t352 * t130
        t1322 = t356 * t130
        t1324 = (t1319 - t1322) * t130
        t1328 = u(t10,t153,k,n)
        t1329 = t355 - t1328
        t1355 = u(t10,j,t195,n)
        t1356 = t1355 - t361
        t1358 = t362 * t185
        t1361 = t366 * t185
        t1363 = (t1358 - t1361) * t185
        t1367 = u(t10,j,t208,n)
        t1368 = t365 - t1367
        t1394 = -t25 * (t1222 + t1248) / 0.24E2 - t139 * ((t4 * ((t1317 
     #* t130 - t1319) * t130 - t1324) * t130 - t4 * (t1324 - (t1322 - t1
     #329 * t130) * t130) * t130) * t130 + (((t4 * t1317 * t130 - t354) 
     #* t130 - t360) * t130 - (t360 - (t358 - t4 * t1329 * t130) * t130)
     # * t130) * t130) / 0.24E2 + t370 + t112 + t360 - t194 * ((t4 * ((t
     #1356 * t185 - t1358) * t185 - t1363) * t185 - t4 * (t1363 - (t1361
     # - t1368 * t185) * t185) * t185) * t185 + (((t4 * t1356 * t185 - t
     #364) * t185 - t370) * t185 - (t370 - (t368 - t4 * t1368 * t185) * 
     #t185) * t185) * t185) / 0.24E2 - t102 + t125 - t138 + t180 - t193 
     #+ t235
        t1395 = t1394 * t8
        t1397 = t350 / 0.2E1
        t1398 = u(t27,t126,k,n)
        t1402 = u(t27,t132,k,n)
        t1408 = u(t27,j,t181,n)
        t1412 = u(t27,j,t187,n)
        t1423 = (((t1244 + (t4 * (t1398 - t28) * t130 - t4 * (t28 - t140
     #2) * t130) * t130 + (t4 * (t1408 - t28) * t185 - t4 * (t28 - t1412
     #) * t185) * t185 - t112 - t360 - t370) * t8 - t372) * t8 - t376) *
     # t8
        t1424 = t401 * t8
        t1429 = t1395 / 0.2E1 + t1397 - t25 * (t1423 / 0.2E1 + t1424 / 0
     #.2E1) / 0.6E1
        t1436 = t25 * (t61 - dx * t78 / 0.12E2) / 0.12E2
        t1437 = ut(t10,j,t195,n)
        t1438 = t1437 - t669
        t1440 = t670 * t185
        t1443 = t674 * t185
        t1445 = (t1440 - t1443) * t185
        t1449 = ut(t10,j,t208,n)
        t1450 = t673 - t1449
        t1483 = (t4 * t1291 * t8 - t473) * t8
        t1491 = ut(t10,t140,k,n)
        t1492 = t1491 - t659
        t1494 = t660 * t130
        t1497 = t664 * t130
        t1499 = (t1494 - t1497) * t130
        t1503 = ut(t10,t153,k,n)
        t1504 = t663 - t1503
        t1530 = t477 + t668 + t678 - t194 * ((t4 * ((t1438 * t185 - t144
     #0) * t185 - t1445) * t185 - t4 * (t1445 - (t1443 - t1450 * t185) *
     # t185) * t185) * t185 + (((t4 * t1438 * t185 - t672) * t185 - t678
     #) * t185 - (t678 - (t676 - t4 * t1450 * t185) * t185) * t185) * t1
     #85) / 0.24E2 - t25 * ((t4 * t1295 * t8 - t467) * t8 + ((t1483 - t4
     #77) * t8 - t483) * t8) / 0.24E2 - t139 * ((t4 * ((t1492 * t130 - t
     #1494) * t130 - t1499) * t130 - t4 * (t1499 - (t1497 - t1504 * t130
     #) * t130) * t130) * t130 + (((t4 * t1492 * t130 - t662) * t130 - t
     #668) * t130 - (t668 - (t666 - t4 * t1504 * t130) * t130) * t130) *
     # t130) / 0.24E2 - t426 + t465 + t494 - t481 - t504 + t543
        t1533 = t658 / 0.2E1
        t1560 = t709 * t8
        t1565 = t1530 * t8 / 0.2E1 + t1533 - t25 * ((((t1483 + (t4 * (ut
     #(t27,t126,k,n) - t70) * t130 - t4 * (t70 - ut(t27,t132,k,n)) * t13
     #0) * t130 + (t4 * (ut(t27,j,t181,n) - t70) * t185 - t4 * (t70 - ut
     #(t27,j,t187,n)) * t185) * t185 - t477 - t668 - t678) * t8 - t680) 
     #* t8 - t684) * t8 / 0.2E1 + t1560 / 0.2E1) / 0.6E1
        t1570 = t1423 - t1424
        t1573 = (t1395 - t350) * t8 - dx * t1570 / 0.12E2
        t1579 = t26 * t78 / 0.720E3
        t1582 = t54 + dt * t1280 / 0.2E1 - t1308 + t95 * t1309 / 0.8E1 -
     # t1312 * t1429 / 0.4E1 + t1436 - t896 * t1565 / 0.16E2 + t715 * t1
     #573 / 0.24E2 + t896 * t683 / 0.96E2 - t1579 - t1078 * t1570 / 0.14
     #40E4
        t1587 = u(i,t1086,k,n)
        t1588 = t1587 - t246
        t1590 = t4 * t1588 * t130
        t1592 = (t1590 - t269) * t130
        t1594 = (t1592 - t271) * t130
        t1595 = t1594 - t273
        t1596 = t1595 * t130
        t1597 = t1596 - t281
        t1598 = t1597 * t130
        t1599 = u(i,t1099,k,n)
        t1600 = t258 - t1599
        t1602 = t4 * t1600 * t130
        t1604 = (t275 - t1602) * t130
        t1606 = (t277 - t1604) * t130
        t1607 = t279 - t1606
        t1608 = t1607 * t130
        t1609 = t281 - t1608
        t1610 = t1609 * t130
        t1614 = u(i,j,t1139,n)
        t1615 = t1614 - t295
        t1616 = t1615 * t185
        t1618 = (t1616 - t297) * t185
        t1619 = t1618 - t300
        t1621 = t4 * t1619 * t185
        t1622 = t1621 - t306
        t1623 = t1622 * t185
        t1625 = (t1623 - t316) * t185
        t1626 = u(i,j,t1152,n)
        t1627 = t307 - t1626
        t1628 = t1627 * t185
        t1630 = (t309 - t1628) * t185
        t1631 = t311 - t1630
        t1633 = t4 * t1631 * t185
        t1634 = t314 - t1633
        t1635 = t1634 * t185
        t1637 = (t316 - t1635) * t185
        t1641 = t1619 * t185
        t1642 = t304 * t185
        t1644 = (t1641 - t1642) * t185
        t1645 = t312 * t185
        t1647 = (t1642 - t1645) * t185
        t1648 = t1644 - t1647
        t1650 = t4 * t1648 * t185
        t1651 = t1631 * t185
        t1653 = (t1645 - t1651) * t185
        t1654 = t1647 - t1653
        t1656 = t4 * t1654 * t185
        t1661 = t4 * t1615 * t185
        t1663 = (t1661 - t318) * t185
        t1665 = (t1663 - t320) * t185
        t1666 = t1665 - t322
        t1667 = t1666 * t185
        t1668 = t1667 - t330
        t1669 = t1668 * t185
        t1671 = t4 * t1627 * t185
        t1673 = (t324 - t1671) * t185
        t1675 = (t326 - t1673) * t185
        t1676 = t328 - t1675
        t1677 = t1676 * t185
        t1678 = t330 - t1677
        t1679 = t1678 * t185
        t1687 = i - 3
        t1688 = u(t1687,j,k,n)
        t1689 = t39 - t1688
        t1690 = t1689 * t8
        t1692 = (t41 - t1690) * t8
        t1693 = t43 - t1692
        t1695 = t4 * t1693 * t8
        t1696 = t335 - t1695
        t1697 = t1696 * t8
        t1699 = (t337 - t1697) * t8
        t1703 = t1693 * t8
        t1705 = (t45 - t1703) * t8
        t1706 = t47 - t1705
        t1708 = t4 * t1706 * t8
        t1713 = t4 * t1689 * t8
        t1715 = (t339 - t1713) * t8
        t1717 = (t341 - t1715) * t8
        t1718 = t343 - t1717
        t1719 = t1718 * t8
        t1720 = t345 - t1719
        t1721 = t1720 * t8
        t1725 = t1588 * t130
        t1727 = (t1725 - t248) * t130
        t1728 = t1727 - t251
        t1729 = t1728 * t130
        t1730 = t255 * t130
        t1732 = (t1729 - t1730) * t130
        t1733 = t263 * t130
        t1735 = (t1730 - t1733) * t130
        t1736 = t1732 - t1735
        t1738 = t4 * t1736 * t130
        t1739 = t1600 * t130
        t1741 = (t260 - t1739) * t130
        t1742 = t262 - t1741
        t1743 = t1742 * t130
        t1745 = (t1733 - t1743) * t130
        t1746 = t1735 - t1745
        t1748 = t4 * t1746 * t130
        t1753 = t4 * t1728 * t130
        t1754 = t1753 - t257
        t1755 = t1754 * t130
        t1757 = (t1755 - t267) * t130
        t1759 = t4 * t1742 * t130
        t1760 = t265 - t1759
        t1761 = t1760 * t130
        t1763 = (t267 - t1761) * t130
        t1771 = t118 + t245 + t294 - dy * t266 / 0.24E2 - dy * t280 / 0.
     #24E2 + 0.3E1 / 0.640E3 * t1085 * (t1598 - t1610) + t1138 * (t1625 
     #- t1637) / 0.576E3 + 0.3E1 / 0.640E3 * t1138 * (t1650 - t1656) + 0
     #.3E1 / 0.640E3 * t1138 * (t1669 - t1679) - dx * t336 / 0.24E2 - dx
     # * t344 / 0.24E2 + t26 * (t1226 - t1699) / 0.576E3 + 0.3E1 / 0.640
     #E3 * t26 * (t1237 - t1708) + 0.3E1 / 0.640E3 * t26 * (t1251 - t172
     #1) + 0.3E1 / 0.640E3 * t1085 * (t1738 - t1748) + t1085 * (t1757 - 
     #t1763) / 0.576E3 - dz * t315 / 0.24E2 - dz * t329 / 0.24E2
        t1773 = dt * t1771 / 0.2E1
        t1774 = t64 / 0.2E1
        t1779 = ut(t1687,j,k,n)
        t1780 = t80 - t1779
        t1781 = t1780 * t8
        t1783 = (t82 - t1781) * t8
        t1784 = t84 - t1783
        t1785 = t1784 * t8
        t1786 = t86 - t1785
        t1787 = t1786 * t8
        t1788 = t88 - t1787
        t1789 = t1788 * t8
        t1796 = dx * (t1284 + t1774 - t25 * (t77 / 0.2E1 + t86 / 0.2E1) 
     #/ 0.6E1 + t1289 * (t1301 / 0.2E1 + t1789 / 0.2E1) / 0.30E2) / 0.2E
     #1
        t1797 = t487 - t558 + t568 - t607 + t617 - t656
        t1799 = t95 * t1797 / 0.8E1
        t1802 = t25 * (t1697 + t1719) / 0.24E2
        t1803 = u(t16,j,t195,n)
        t1804 = t1803 - t387
        t1805 = t1804 * t185
        t1806 = t388 * t185
        t1808 = (t1805 - t1806) * t185
        t1809 = t392 * t185
        t1811 = (t1806 - t1809) * t185
        t1812 = t1808 - t1811
        t1814 = t4 * t1812 * t185
        t1815 = u(t16,j,t208,n)
        t1816 = t391 - t1815
        t1817 = t1816 * t185
        t1819 = (t1809 - t1817) * t185
        t1820 = t1811 - t1819
        t1822 = t4 * t1820 * t185
        t1823 = t1814 - t1822
        t1824 = t1823 * t185
        t1826 = t4 * t1804 * t185
        t1828 = (t1826 - t390) * t185
        t1830 = (t1828 - t396) * t185
        t1832 = t4 * t1816 * t185
        t1834 = (t394 - t1832) * t185
        t1836 = (t396 - t1834) * t185
        t1837 = t1830 - t1836
        t1838 = t1837 * t185
        t1841 = t194 * (t1824 + t1838) / 0.24E2
        t1842 = u(t16,t140,k,n)
        t1843 = t1842 - t377
        t1844 = t1843 * t130
        t1845 = t378 * t130
        t1847 = (t1844 - t1845) * t130
        t1848 = t382 * t130
        t1850 = (t1845 - t1848) * t130
        t1851 = t1847 - t1850
        t1853 = t4 * t1851 * t130
        t1854 = u(t16,t153,k,n)
        t1855 = t381 - t1854
        t1856 = t1855 * t130
        t1858 = (t1848 - t1856) * t130
        t1859 = t1850 - t1858
        t1861 = t4 * t1859 * t130
        t1862 = t1853 - t1861
        t1863 = t1862 * t130
        t1865 = t4 * t1843 * t130
        t1867 = (t1865 - t380) * t130
        t1869 = (t1867 - t386) * t130
        t1871 = t4 * t1855 * t130
        t1873 = (t384 - t1871) * t130
        t1875 = (t386 - t1873) * t130
        t1876 = t1869 - t1875
        t1877 = t1876 * t130
        t1880 = t139 * (t1863 + t1877) / 0.24E2
        t1881 = t118 + t245 - t284 + t294 - t333 - t348 - t341 - t386 - 
     #t396 + t1802 + t1841 + t1880
        t1882 = t1881 * t8
        t1883 = t1882 / 0.2E1
        t1884 = u(t38,t126,k,n)
        t1885 = t1884 - t39
        t1887 = t4 * t1885 * t130
        t1888 = u(t38,t132,k,n)
        t1889 = t39 - t1888
        t1891 = t4 * t1889 * t130
        t1893 = (t1887 - t1891) * t130
        t1894 = u(t38,j,t181,n)
        t1895 = t1894 - t39
        t1897 = t4 * t1895 * t185
        t1898 = u(t38,j,t187,n)
        t1899 = t39 - t1898
        t1901 = t4 * t1899 * t185
        t1903 = (t1897 - t1901) * t185
        t1904 = t341 + t386 + t396 - t1715 - t1893 - t1903
        t1905 = t1904 * t8
        t1907 = (t398 - t1905) * t8
        t1908 = t400 - t1907
        t1909 = t1908 * t8
        t1914 = t1397 + t1883 - t25 * (t1424 / 0.2E1 + t1909 / 0.2E1) / 
     #0.6E1
        t1916 = t1312 * t1914 / 0.4E1
        t1921 = t25 * (t66 - dx * t87 / 0.12E2) / 0.12E2
        t1923 = t4 * t1784 * t8
        t1927 = t4 * t1780 * t8
        t1929 = (t549 - t1927) * t8
        t1931 = (t551 - t1929) * t8
        t1933 = (t553 - t1931) * t8
        t1936 = t25 * ((t545 - t1923) * t8 + t1933) / 0.24E2
        t1937 = ut(t16,j,t195,n)
        t1938 = t1937 - t695
        t1940 = t696 * t185
        t1943 = t700 * t185
        t1945 = (t1940 - t1943) * t185
        t1949 = ut(t16,j,t208,n)
        t1950 = t699 - t1949
        t1962 = (t4 * t1938 * t185 - t698) * t185
        t1968 = (t702 - t4 * t1950 * t185) * t185
        t1975 = t194 * ((t4 * ((t1938 * t185 - t1940) * t185 - t1945) * 
     #t185 - t4 * (t1945 - (t1943 - t1950 * t185) * t185) * t185) * t185
     # + ((t1962 - t704) * t185 - (t704 - t1968) * t185) * t185) / 0.24E
     #2
        t1976 = ut(t16,t140,k,n)
        t1977 = t1976 - t685
        t1979 = t686 * t130
        t1982 = t690 * t130
        t1984 = (t1979 - t1982) * t130
        t1988 = ut(t16,t153,k,n)
        t1989 = t689 - t1988
        t2001 = (t4 * t1977 * t130 - t688) * t130
        t2007 = (t692 - t4 * t1989 * t130) * t130
        t2014 = t139 * ((t4 * ((t1977 * t130 - t1979) * t130 - t1984) * 
     #t130 - t4 * (t1984 - (t1982 - t1989 * t130) * t130) * t130) * t130
     # + ((t2001 - t694) * t130 - (t694 - t2007) * t130) * t130) / 0.24E
     #2
        t2015 = t487 - t558 + t568 - t607 + t617 - t656 + t1936 + t1975 
     #+ t2014 - t551 - t694 - t704
        t2016 = t2015 * t8
        t2017 = t2016 / 0.2E1
        t2018 = ut(t38,t126,k,n)
        t2019 = t2018 - t80
        t2021 = t4 * t2019 * t130
        t2022 = ut(t38,t132,k,n)
        t2023 = t80 - t2022
        t2025 = t4 * t2023 * t130
        t2027 = (t2021 - t2025) * t130
        t2028 = ut(t38,j,t181,n)
        t2029 = t2028 - t80
        t2031 = t4 * t2029 * t185
        t2032 = ut(t38,j,t187,n)
        t2033 = t80 - t2032
        t2035 = t4 * t2033 * t185
        t2037 = (t2031 - t2035) * t185
        t2038 = t551 + t694 + t704 - t1929 - t2027 - t2037
        t2039 = t2038 * t8
        t2040 = t706 - t2039
        t2041 = t2040 * t8
        t2042 = t708 - t2041
        t2043 = t2042 * t8
        t2048 = t1533 + t2017 - t25 * (t1560 / 0.2E1 + t2043 / 0.2E1) / 
     #0.6E1
        t2050 = t896 * t2048 / 0.16E2
        t2053 = t1424 - t1909
        t2056 = (t350 - t1882) * t8 - dx * t2053 / 0.12E2
        t2058 = t715 * t2056 / 0.24E2
        t2060 = t896 * t707 / 0.96E2
        t2062 = t26 * t87 / 0.720E3
        t2064 = t1078 * t2053 / 0.1440E4
        t2065 = -t2 - t1773 - t1796 - t1799 - t1916 - t1921 - t2050 - t2
     #058 - t2060 + t2062 + t2064
        t2068 = sqrt(0.256E3)
        t2071 = t52 + t53 * t92 / 0.2E1 + t96 * t404 / 0.8E1 - t414 + t4
     #16 * t712 / 0.48E2 - t715 * t721 / 0.48E2 + t725 * t892 * t8 / 0.3
     #84E3 - t896 * t897 / 0.192E3 + t901 + t903 * t1070 * t8 / 0.3840E4
     # - t1074 * t1075 / 0.2304E4 + 0.7E1 / 0.11520E5 * t1078 * t718 + c
     #c * (t1582 + t2065) * t2068 / 0.32E2
        t2072 = dt / 0.2E1
        t2073 = sqrt(0.15E2)
        t2074 = t2073 / 0.10E2
        t2075 = 0.1E1 / 0.2E1 - t2074
        t2076 = dt * t2075
        t2078 = 0.1E1 / (t2072 - t2076)
        t2080 = 0.1E1 / 0.2E1 + t2074
        t2081 = dt * t2080
        t2083 = 0.1E1 / (t2072 - t2081)
        t2085 = t4 * t2075
        t2086 = dt * t92
        t2088 = t2075 ** 2
        t2089 = t4 * t2088
        t2090 = t95 * t404
        t2093 = t2088 * t2075
        t2094 = t4 * t2093
        t2095 = t415 * t712
        t2098 = t25 * t721
        t2101 = t2088 ** 2
        t2102 = t4 * t2101
        t2104 = t724 * t892 * t8
        t2107 = t2088 * t95
        t2108 = dx * t897
        t2112 = t4 * t2101 * t2075
        t2114 = t902 * t1070 * t8
        t2117 = t2093 * t415
        t2118 = dx * t1075
        t2121 = t26 * t718
        t2127 = dx * t1429
        t2130 = dx * t1565
        t2133 = t25 * t1573
        t2136 = dx * t683
        t2139 = t26 * t1570
        t2142 = t54 + t2076 * t1280 - t1308 + t2107 * t1309 / 0.2E1 - t2
     #076 * t2127 / 0.2E1 + t1436 - t2107 * t2130 / 0.4E1 + t2076 * t213
     #3 / 0.12E2 + t2107 * t2136 / 0.24E2 - t1579 - t2076 * t2139 / 0.72
     #0E3
        t2143 = t2076 * t1771
        t2145 = t2107 * t1797 / 0.2E1
        t2146 = dx * t1914
        t2148 = t2076 * t2146 / 0.2E1
        t2149 = dx * t2048
        t2151 = t2107 * t2149 / 0.4E1
        t2152 = t25 * t2056
        t2154 = t2076 * t2152 / 0.12E2
        t2155 = dx * t707
        t2157 = t2107 * t2155 / 0.24E2
        t2158 = t26 * t2053
        t2160 = t2076 * t2158 / 0.720E3
        t2161 = -t2 - t2143 - t1796 - t2145 - t2148 - t1921 - t2151 - t2
     #154 - t2157 + t2062 + t2160
        t2166 = t52 + t2085 * t2086 + t2089 * t2090 / 0.2E1 - t414 + t20
     #94 * t2095 / 0.6E1 - t2076 * t2098 / 0.24E2 + t2102 * t2104 / 0.24
     #E2 - t2107 * t2108 / 0.48E2 + t901 + t2112 * t2114 / 0.120E3 - t21
     #17 * t2118 / 0.288E3 + 0.7E1 / 0.5760E4 * t2076 * t2121 + cc * (t2
     #142 + t2161) * t2068 / 0.32E2
        t2168 = -t2078
        t2171 = 0.1E1 / (t2076 - t2081)
        t2173 = t4 * t2080
        t2175 = t2080 ** 2
        t2176 = t4 * t2175
        t2179 = t2175 * t2080
        t2180 = t4 * t2179
        t2185 = t2175 ** 2
        t2186 = t4 * t2185
        t2189 = t2175 * t95
        t2193 = t4 * t2185 * t2080
        t2196 = t2179 * t415
        t2214 = t54 + t2081 * t1280 - t1308 + t2189 * t1309 / 0.2E1 - t2
     #081 * t2127 / 0.2E1 + t1436 - t2189 * t2130 / 0.4E1 + t2081 * t213
     #3 / 0.12E2 + t2189 * t2136 / 0.24E2 - t1579 - t2081 * t2139 / 0.72
     #0E3
        t2215 = t2081 * t1771
        t2217 = t2189 * t1797 / 0.2E1
        t2219 = t2081 * t2146 / 0.2E1
        t2221 = t2189 * t2149 / 0.4E1
        t2223 = t2081 * t2152 / 0.12E2
        t2225 = t2189 * t2155 / 0.24E2
        t2227 = t2081 * t2158 / 0.720E3
        t2228 = -t2 - t2215 - t1796 - t2217 - t2219 - t1921 - t2221 - t2
     #223 - t2225 + t2062 + t2227
        t2233 = t52 + t2173 * t2086 + t2176 * t2090 / 0.2E1 - t414 + t21
     #80 * t2095 / 0.6E1 - t2081 * t2098 / 0.24E2 + t2186 * t2104 / 0.24
     #E2 - t2189 * t2108 / 0.48E2 + t901 + t2193 * t2114 / 0.120E3 - t21
     #96 * t2118 / 0.288E3 + 0.7E1 / 0.5760E4 * t2081 * t2121 + cc * (t2
     #214 + t2228) * t2068 / 0.32E2
        t2235 = -t2171
        t2238 = -t2083
        t2240 = t2071 * t2078 * t2083 + t2166 * t2168 * t2171 + t2233 * 
     #t2235 * t2238
        t2244 = t2166 * dt
        t2250 = t2071 * dt
        t2256 = t2233 * dt
        t2262 = (-t2244 / 0.2E1 - t2244 * t2080) * t2168 * t2171 + (-t22
     #50 * t2075 - t2250 * t2080) * t2078 * t2083 + (-t2256 * t2075 - t2
     #256 / 0.2E1) * t2235 * t2238
        t2268 = t2080 * t2168 * t2171
        t2273 = t2075 * t2235 * t2238
        t2289 = t4 * (t19 - dx * t44 / 0.24E2 + 0.3E1 / 0.640E3 * t26 * 
     #t1706)
        t2294 = t64 - dx * t85 / 0.24E2 + 0.3E1 / 0.640E3 * t26 * t1788
        t2299 = t1882 - dx * t1908 / 0.24E2
        t2308 = t25 * ((t118 - t348 - t341 + t1802) * t8 - dx * t1720 / 
     #0.24E2) / 0.24E2
        t2311 = t2016 - dx * t2042 / 0.24E2
        t2316 = t555 - t1933
        t2319 = (t487 - t558 - t551 + t1936) * t8 - dx * t2316 / 0.24E2
        t2325 = (t817 - t4 * t1904 * t8) * t8
        t2326 = t377 - t1884
        t2328 = t4 * t2326 * t8
        t2330 = (t822 - t2328) * t8
        t2331 = u(t16,t126,t181,n)
        t2335 = u(t16,t126,t187,n)
        t2340 = (t4 * (t2331 - t377) * t185 - t4 * (t377 - t2335) * t185
     #) * t185
        t2344 = t381 - t1888
        t2346 = t4 * t2344 * t8
        t2348 = (t840 - t2346) * t8
        t2349 = u(t16,t132,t181,n)
        t2353 = u(t16,t132,t187,n)
        t2358 = (t4 * (t2349 - t381) * t185 - t4 * (t381 - t2353) * t185
     #) * t185
        t2364 = t387 - t1894
        t2366 = t4 * t2364 * t8
        t2368 = (t860 - t2366) * t8
        t2376 = (t4 * (t2331 - t387) * t130 - t4 * (t387 - t2349) * t130
     #) * t130
        t2380 = t391 - t1898
        t2382 = t4 * t2380 * t8
        t2384 = (t876 - t2382) * t8
        t2392 = (t4 * (t2335 - t391) * t130 - t4 * (t391 - t2353) * t130
     #) * t130
        t2398 = t819 + t857 + t891 - t2325 - (t4 * (t2330 + t1867 + t234
     #0 - t341 - t386 - t396) * t130 - t4 * (t341 + t386 + t396 - t2348 
     #- t1873 - t2358) * t130) * t130 - (t4 * (t2368 + t2376 + t1828 - t
     #341 - t386 - t396) * t185 - t4 * (t341 + t386 + t396 - t2384 - t23
     #92 - t1834) * t185) * t185
        t2402 = t819 - t2325
        t2406 = 0.7E1 / 0.5760E4 * t26 * t1720
        t2410 = (t995 - t4 * t2038 * t8) * t8
        t2411 = t685 - t2018
        t2415 = (t1000 - t4 * t2411 * t8) * t8
        t2416 = ut(t16,t126,t181,n)
        t2420 = ut(t16,t126,t187,n)
        t2425 = (t4 * (t2416 - t685) * t185 - t4 * (t685 - t2420) * t185
     #) * t185
        t2429 = t689 - t2022
        t2433 = (t1018 - t4 * t2429 * t8) * t8
        t2434 = ut(t16,t132,t181,n)
        t2438 = ut(t16,t132,t187,n)
        t2443 = (t4 * (t2434 - t689) * t185 - t4 * (t689 - t2438) * t185
     #) * t185
        t2449 = t695 - t2028
        t2453 = (t1038 - t4 * t2449 * t8) * t8
        t2461 = (t4 * (t2416 - t695) * t130 - t4 * (t695 - t2434) * t130
     #) * t130
        t2465 = t699 - t2032
        t2469 = (t1054 - t4 * t2465 * t8) * t8
        t2477 = (t4 * (t2420 - t699) * t130 - t4 * (t699 - t2438) * t130
     #) * t130
        t2483 = t997 + t1035 + t1069 - t2410 - (t4 * (t2415 + t2001 + t2
     #425 - t551 - t694 - t704) * t130 - t4 * (t551 + t694 + t704 - t243
     #3 - t2007 - t2443) * t130) * t130 - (t4 * (t2453 + t2461 + t1962 -
     # t551 - t694 - t704) * t185 - t4 * (t551 + t694 + t704 - t2469 - t
     #2477 - t1968) * t185) * t185
        t2487 = t997 - t2410
        t2492 = t2 + t1773 - t1796 + t1799 - t1916 + t1921 - t2050 + t20
     #58 + t2060 - t2062 - t2064
        t2493 = i - 4
        t2495 = t1688 - u(t2493,j,k,n)
        t2499 = (t1713 - t4 * t2495 * t8) * t8
        t2503 = (t1717 - (t1715 - t2499) * t8) * t8
        t2512 = t1692 - (t1690 - t2495 * t8) * t8
        t2526 = u(t16,j,t1139,n)
        t2527 = t2526 - t1803
        t2538 = u(t16,j,t1152,n)
        t2539 = t1815 - t2538
        t2556 = (t2527 * t185 - t1805) * t185 - t1808
        t2558 = t1812 * t185
        t2561 = t1820 * t185
        t2563 = (t2558 - t2561) * t185
        t2570 = t1819 - (t1817 - t2539 * t185) * t185
        t2580 = u(t16,t1086,k,n)
        t2581 = t2580 - t1842
        t2592 = u(t16,t1099,k,n)
        t2593 = t1854 - t2592
        t2612 = (t2581 * t130 - t1844) * t130 - t1847
        t2622 = t1858 - (t1856 - t2593 * t130) * t130
        t2633 = t1851 * t130
        t2636 = t1859 * t130
        t2638 = (t2633 - t2636) * t130
        t2673 = (t1695 - t4 * t2512 * t8) * t8
        t2681 = 0.3E1 / 0.640E3 * t26 * (t1721 - (t1719 - t2503) * t8) +
     # 0.3E1 / 0.640E3 * t26 * (t1708 - t4 * (t1705 - (t1703 - t2512 * t
     #8) * t8) * t8) - dx * t1718 / 0.24E2 - dx * t1696 / 0.24E2 + 0.3E1
     # / 0.640E3 * t1138 * (((((t4 * t2527 * t185 - t1826) * t185 - t182
     #8) * t185 - t1830) * t185 - t1838) * t185 - (t1838 - (t1836 - (t18
     #34 - (t1832 - t4 * t2539 * t185) * t185) * t185) * t185) * t185) +
     # 0.3E1 / 0.640E3 * t1138 * (t4 * ((t2556 * t185 - t2558) * t185 - 
     #t2563) * t185 - t4 * (t2563 - (t2561 - t2570 * t185) * t185) * t18
     #5) + 0.3E1 / 0.640E3 * t1085 * (((((t4 * t2581 * t130 - t1865) * t
     #130 - t1867) * t130 - t1869) * t130 - t1877) * t130 - (t1877 - (t1
     #875 - (t1873 - (t1871 - t4 * t2593 * t130) * t130) * t130) * t130)
     # * t130) - dy * t1876 / 0.24E2 + t1085 * (((t4 * t2612 * t130 - t1
     #853) * t130 - t1863) * t130 - (t1863 - (t1861 - t4 * t2622 * t130)
     # * t130) * t130) / 0.576E3 + 0.3E1 / 0.640E3 * t1085 * (t4 * ((t26
     #12 * t130 - t2633) * t130 - t2638) * t130 - t4 * (t2638 - (t2636 -
     # t2622 * t130) * t130) * t130) - dy * t1862 / 0.24E2 + t341 + t386
     # + t396 - dz * t1837 / 0.24E2 + t1138 * (((t4 * t2556 * t185 - t18
     #14) * t185 - t1824) * t185 - (t1824 - (t1822 - t4 * t2570 * t185) 
     #* t185) * t185) / 0.576E3 + t26 * (t1699 - (t1697 - t2673) * t8) /
     # 0.576E3 - dz * t1823 / 0.24E2
        t2690 = t1779 - ut(t2493,j,k,n)
        t2694 = t1783 - (t1781 - t2690 * t8) * t8
        t2706 = dx * (t1774 + t82 / 0.2E1 - t25 * (t86 / 0.2E1 + t1785 /
     # 0.2E1) / 0.6E1 + t1289 * (t1789 / 0.2E1 + (t1787 - (t1785 - t2694
     # * t8) * t8) * t8 / 0.2E1) / 0.30E2) / 0.2E1
        t2707 = -t1936 - t1975 - t2014 + t551 + t694 + t704
        t2713 = u(t38,j,t195,n)
        t2714 = t2713 - t1894
        t2716 = t1895 * t185
        t2719 = t1899 * t185
        t2721 = (t2716 - t2719) * t185
        t2725 = u(t38,j,t208,n)
        t2726 = t1898 - t2725
        t2752 = u(t38,t140,k,n)
        t2753 = t2752 - t1884
        t2755 = t1885 * t130
        t2758 = t1889 * t130
        t2760 = (t2755 - t2758) * t130
        t2764 = u(t38,t153,k,n)
        t2765 = t1888 - t2764
        t2791 = t341 + t386 + t396 - t1802 - t1841 - t1880 + t25 * (t267
     #3 + t2503) / 0.24E2 + t194 * ((t4 * ((t2714 * t185 - t2716) * t185
     # - t2721) * t185 - t4 * (t2721 - (t2719 - t2726 * t185) * t185) * 
     #t185) * t185 + (((t4 * t2714 * t185 - t1897) * t185 - t1903) * t18
     #5 - (t1903 - (t1901 - t4 * t2726 * t185) * t185) * t185) * t185) /
     # 0.24E2 + t139 * ((t4 * ((t2753 * t130 - t2755) * t130 - t2760) * 
     #t130 - t4 * (t2760 - (t2758 - t2765 * t130) * t130) * t130) * t130
     # + (((t4 * t2753 * t130 - t1887) * t130 - t1893) * t130 - (t1893 -
     # (t1891 - t4 * t2765 * t130) * t130) * t130) * t130) / 0.24E2 - t1
     #903 - t1893 - t1715
        t2792 = t2791 * t8
        t2794 = u(t1687,t126,k,n)
        t2798 = u(t1687,t132,k,n)
        t2804 = u(t1687,j,t181,n)
        t2808 = u(t1687,j,t187,n)
        t2819 = (t1907 - (t1905 - (t1715 + t1893 + t1903 - t2499 - (t4 *
     # (t2794 - t1688) * t130 - t4 * (t1688 - t2798) * t130) * t130 - (t
     #4 * (t2804 - t1688) * t185 - t4 * (t1688 - t2808) * t185) * t185) 
     #* t8) * t8) * t8
        t2824 = t1883 + t2792 / 0.2E1 - t25 * (t1909 / 0.2E1 + t2819 / 0
     #.2E1) / 0.6E1
        t2831 = t25 * (t84 - dx * t1786 / 0.12E2) / 0.12E2
        t2832 = ut(t38,t140,k,n)
        t2833 = t2832 - t2018
        t2835 = t2019 * t130
        t2838 = t2023 * t130
        t2840 = (t2835 - t2838) * t130
        t2844 = ut(t38,t153,k,n)
        t2845 = t2022 - t2844
        t2878 = (t1927 - t4 * t2690 * t8) * t8
        t2886 = ut(t38,j,t195,n)
        t2887 = t2886 - t2028
        t2889 = t2029 * t185
        t2892 = t2033 * t185
        t2894 = (t2889 - t2892) * t185
        t2898 = ut(t38,j,t208,n)
        t2899 = t2032 - t2898
        t2925 = -t1936 - t1975 - t2014 + t551 + t694 + t704 - t1929 - t2
     #027 + t139 * ((t4 * ((t2833 * t130 - t2835) * t130 - t2840) * t130
     # - t4 * (t2840 - (t2838 - t2845 * t130) * t130) * t130) * t130 + (
     #((t4 * t2833 * t130 - t2021) * t130 - t2027) * t130 - (t2027 - (t2
     #025 - t4 * t2845 * t130) * t130) * t130) * t130) / 0.24E2 + t25 * 
     #((t1923 - t4 * t2694 * t8) * t8 + (t1931 - (t1929 - t2878) * t8) *
     # t8) / 0.24E2 + t194 * ((t4 * ((t2887 * t185 - t2889) * t185 - t28
     #94) * t185 - t4 * (t2894 - (t2892 - t2899 * t185) * t185) * t185) 
     #* t185 + (((t4 * t2887 * t185 - t2031) * t185 - t2037) * t185 - (t
     #2037 - (t2035 - t4 * t2899 * t185) * t185) * t185) * t185) / 0.24E
     #2 - t2037
        t2958 = t2017 + t2925 * t8 / 0.2E1 - t25 * (t2043 / 0.2E1 + (t20
     #41 - (t2039 - (t1929 + t2027 + t2037 - t2878 - (t4 * (ut(t1687,t12
     #6,k,n) - t1779) * t130 - t4 * (t1779 - ut(t1687,t132,k,n)) * t130)
     # * t130 - (t4 * (ut(t1687,j,t181,n) - t1779) * t185 - t4 * (t1779 
     #- ut(t1687,j,t187,n)) * t185) * t185) * t8) * t8) * t8 / 0.2E1) / 
     #0.6E1
        t2963 = t1909 - t2819
        t2966 = (t1882 - t2792) * t8 - dx * t2963 / 0.12E2
        t2972 = t26 * t1786 / 0.720E3
        t2975 = -t62 - dt * t2681 / 0.2E1 - t2706 - t95 * t2707 / 0.8E1 
     #- t1312 * t2824 / 0.4E1 - t2831 - t896 * t2958 / 0.16E2 - t715 * t
     #2966 / 0.24E2 - t896 * t2040 / 0.96E2 + t2972 + t1078 * t2963 / 0.
     #1440E4
        t2980 = t2289 + t53 * t2294 / 0.2E1 + t96 * t2299 / 0.8E1 - t230
     #8 + t416 * t2311 / 0.48E2 - t715 * t2319 / 0.48E2 + t725 * t2398 *
     # t8 / 0.384E3 - t896 * t2402 / 0.192E3 + t2406 + t903 * t2483 * t8
     # / 0.3840E4 - t1074 * t2487 / 0.2304E4 + 0.7E1 / 0.11520E5 * t1078
     # * t2316 + cc * (t2492 + t2975) * t2068 / 0.32E2
        t2983 = dt * t2294
        t2985 = t95 * t2299
        t2988 = t415 * t2311
        t2991 = t25 * t2319
        t2995 = t724 * t2398 * t8
        t2998 = dx * t2402
        t3002 = t902 * t2483 * t8
        t3005 = dx * t2487
        t3008 = t26 * t2316
        t3011 = t2 + t2143 - t1796 + t2145 - t2148 + t1921 - t2151 + t21
     #54 + t2157 - t2062 - t2160
        t3015 = dx * t2824
        t3018 = dx * t2958
        t3021 = t25 * t2966
        t3024 = dx * t2040
        t3027 = t26 * t2963
        t3030 = -t62 - t2076 * t2681 - t2706 - t2107 * t2707 / 0.2E1 - t
     #2076 * t3015 / 0.2E1 - t2831 - t2107 * t3018 / 0.4E1 - t2076 * t30
     #21 / 0.12E2 - t2107 * t3024 / 0.24E2 + t2972 + t2076 * t3027 / 0.7
     #20E3
        t3035 = t2289 + t2085 * t2983 + t2089 * t2985 / 0.2E1 - t2308 + 
     #t2094 * t2988 / 0.6E1 - t2076 * t2991 / 0.24E2 + t2102 * t2995 / 0
     #.24E2 - t2107 * t2998 / 0.48E2 + t2406 + t2112 * t3002 / 0.120E3 -
     # t2117 * t3005 / 0.288E3 + 0.7E1 / 0.5760E4 * t2076 * t3008 + cc *
     # (t3011 + t3030) * t2068 / 0.32E2
        t3055 = t2 + t2215 - t1796 + t2217 - t2219 + t1921 - t2221 + t22
     #23 + t2225 - t2062 - t2227
        t3069 = -t62 - t2081 * t2681 - t2706 - t2189 * t2707 / 0.2E1 - t
     #2081 * t3015 / 0.2E1 - t2831 - t2189 * t3018 / 0.4E1 - t2081 * t30
     #21 / 0.12E2 - t2189 * t3024 / 0.24E2 + t2972 + t2081 * t3027 / 0.7
     #20E3
        t3074 = t2289 + t2173 * t2983 + t2176 * t2985 / 0.2E1 - t2308 + 
     #t2180 * t2988 / 0.6E1 - t2081 * t2991 / 0.24E2 + t2186 * t2995 / 0
     #.24E2 - t2189 * t2998 / 0.48E2 + t2406 + t2193 * t3002 / 0.120E3 -
     # t2196 * t3005 / 0.288E3 + 0.7E1 / 0.5760E4 * t2081 * t3008 + cc *
     # (t3055 + t3069) * t2068 / 0.32E2
        t3077 = t2980 * t2078 * t2083 + t3035 * t2168 * t2171 + t3074 * 
     #t2235 * t2238
        t3081 = t3035 * dt
        t3087 = t2980 * dt
        t3093 = t3074 * dt
        t3099 = (-t3081 / 0.2E1 - t3081 * t2080) * t2168 * t2171 + (-t30
     #87 * t2075 - t3087 * t2080) * t2078 * t2083 + (-t3093 * t2075 - t3
     #093 / 0.2E1) * t2235 * t2238
        t3122 = t4 * (t249 - dy * t255 / 0.24E2 + 0.3E1 / 0.640E3 * t108
     #5 * t1736)
        t3125 = ut(i,t1086,k,n)
        t3126 = t3125 - t569
        t3127 = t3126 * t130
        t3129 = (t3127 - t571) * t130
        t3130 = t3129 - t574
        t3131 = t3130 * t130
        t3132 = t578 * t130
        t3133 = t3131 - t3132
        t3134 = t3133 * t130
        t3135 = t586 * t130
        t3136 = t3132 - t3135
        t3137 = t3136 * t130
        t3138 = t3134 - t3137
        t3141 = t572 - dy * t578 / 0.24E2 + 0.3E1 / 0.640E3 * t1085 * t3
     #138
        t3144 = t732 * t8
        t3145 = t735 * t8
        t3147 = (t3144 - t3145) * t8
        t3148 = t820 * t8
        t3150 = (t3145 - t3148) * t8
        t3151 = t3147 - t3150
        t3153 = t4 * t3151 * t8
        t3154 = t2326 * t8
        t3156 = (t3148 - t3154) * t8
        t3157 = t3150 - t3156
        t3159 = t4 * t3157 * t8
        t3160 = t3153 - t3159
        t3161 = t3160 * t8
        t3163 = (t739 - t824) * t8
        t3165 = (t824 - t2330) * t8
        t3166 = t3163 - t3165
        t3167 = t3166 * t8
        t3170 = t25 * (t3161 + t3167) / 0.24E2
        t3171 = u(i,t126,t195,n)
        t3172 = t3171 - t825
        t3173 = t3172 * t185
        t3174 = t826 * t185
        t3176 = (t3173 - t3174) * t185
        t3177 = t830 * t185
        t3179 = (t3174 - t3177) * t185
        t3180 = t3176 - t3179
        t3182 = t4 * t3180 * t185
        t3183 = u(i,t126,t208,n)
        t3184 = t829 - t3183
        t3185 = t3184 * t185
        t3187 = (t3177 - t3185) * t185
        t3188 = t3179 - t3187
        t3190 = t4 * t3188 * t185
        t3191 = t3182 - t3190
        t3192 = t3191 * t185
        t3194 = t4 * t3172 * t185
        t3196 = (t3194 - t828) * t185
        t3198 = (t3196 - t834) * t185
        t3200 = t4 * t3184 * t185
        t3202 = (t832 - t3200) * t185
        t3204 = (t834 - t3202) * t185
        t3205 = t3198 - t3204
        t3206 = t3205 * t185
        t3209 = t194 * (t3192 + t3206) / 0.24E2
        t3212 = t139 * (t1755 + t1596) / 0.24E2
        t3213 = t834 + t824 + t271 - t3170 - t3209 - t3212 - t118 - t245
     # + t284 - t294 + t333 + t348
        t3214 = t3213 * t130
        t3215 = t141 - t246
        t3217 = t4 * t3215 * t8
        t3218 = t246 - t1842
        t3220 = t4 * t3218 * t8
        t3222 = (t3217 - t3220) * t8
        t3223 = u(i,t140,t181,n)
        t3224 = t3223 - t246
        t3226 = t4 * t3224 * t185
        t3227 = u(i,t140,t187,n)
        t3228 = t246 - t3227
        t3230 = t4 * t3228 * t185
        t3232 = (t3226 - t3230) * t185
        t3233 = t3222 + t1592 + t3232 - t824 - t271 - t834
        t3234 = t3233 * t130
        t3235 = t835 * t130
        t3237 = (t3234 - t3235) * t130
        t3238 = t853 * t130
        t3240 = (t3235 - t3238) * t130
        t3241 = t3237 - t3240
        t3244 = t3214 - dy * t3241 / 0.24E2
        t3253 = t139 * ((t271 - t3212 - t245 + t284) * t130 - dy * t1597
     # / 0.24E2) / 0.24E2
        t3255 = t4 * t3130 * t130
        t3259 = t4 * t3126 * t130
        t3261 = (t3259 - t592) * t130
        t3263 = (t3261 - t594) * t130
        t3265 = (t3263 - t596) * t130
        t3268 = t139 * ((t3255 - t580) * t130 + t3265) / 0.24E2
        t3270 = t913 * t8
        t3273 = t998 * t8
        t3275 = (t3270 - t3273) * t8
        t3295 = t25 * ((t4 * ((t910 * t8 - t3270) * t8 - t3275) * t8 - t
     #4 * (t3275 - (t3273 - t2411 * t8) * t8) * t8) * t8 + ((t917 - t100
     #2) * t8 - (t1002 - t2415) * t8) * t8) / 0.24E2
        t3296 = ut(i,t126,t195,n)
        t3297 = t3296 - t1003
        t3299 = t1004 * t185
        t3302 = t1008 * t185
        t3304 = (t3299 - t3302) * t185
        t3308 = ut(i,t126,t208,n)
        t3309 = t1007 - t3308
        t3321 = (t4 * t3297 * t185 - t1006) * t185
        t3327 = (t1010 - t4 * t3309 * t185) * t185
        t3334 = t194 * ((t4 * ((t3297 * t185 - t3299) * t185 - t3304) * 
     #t185 - t4 * (t3304 - (t3302 - t3309 * t185) * t185) * t185) * t185
     # + ((t3321 - t1012) * t185 - (t1012 - t3327) * t185) * t185) / 0.2
     #4E2
        t3335 = t1012 + t1002 + t594 - t3268 - t3295 - t3334 - t487 + t5
     #58 - t568 + t607 - t617 + t656
        t3336 = t3335 * t130
        t3337 = t505 - t569
        t3339 = t4 * t3337 * t8
        t3340 = t569 - t1976
        t3342 = t4 * t3340 * t8
        t3344 = (t3339 - t3342) * t8
        t3345 = ut(i,t140,t181,n)
        t3346 = t3345 - t569
        t3348 = t4 * t3346 * t185
        t3349 = ut(i,t140,t187,n)
        t3350 = t569 - t3349
        t3352 = t4 * t3350 * t185
        t3354 = (t3348 - t3352) * t185
        t3355 = t3344 + t3261 + t3354 - t1002 - t594 - t1012
        t3356 = t3355 * t130
        t3357 = t1013 * t130
        t3358 = t3356 - t3357
        t3359 = t3358 * t130
        t3360 = t1031 * t130
        t3361 = t3357 - t3360
        t3362 = t3361 * t130
        t3363 = t3359 - t3362
        t3366 = t3336 - dy * t3363 / 0.24E2
        t3369 = dt * t139
        t3372 = t3265 - t604
        t3375 = (t594 - t3268 - t568 + t607) * t130 - dy * t3372 / 0.24E
     #2
        t3389 = (t4 * t3233 * t130 - t837) * t130
        t3397 = (t4 * (t740 - t825) * t8 - t4 * (t825 - t2331) * t8) * t
     #8
        t3398 = t3223 - t825
        t3400 = t4 * t3398 * t130
        t3402 = (t3400 - t865) * t130
        t3413 = (t4 * (t744 - t829) * t8 - t4 * (t829 - t2335) * t8) * t
     #8
        t3414 = t3227 - t829
        t3416 = t4 * t3414 * t130
        t3418 = (t3416 - t881) * t130
        t3424 = (t4 * (t739 + t167 + t749 - t824 - t271 - t834) * t8 - t
     #4 * (t824 + t271 + t834 - t2330 - t1867 - t2340) * t8) * t8 + t338
     #9 + (t4 * (t3397 + t3402 + t3196 - t824 - t271 - t834) * t185 - t4
     # * (t824 + t271 + t834 - t3413 - t3418 - t3202) * t185) * t185 - t
     #819 - t857 - t891
        t3428 = t95 * dy
        t3429 = t3389 - t857
        t3433 = 0.7E1 / 0.5760E4 * t1085 * t1597
        t3445 = (t4 * t3355 * t130 - t1015) * t130
        t3453 = (t4 * (t918 - t1003) * t8 - t4 * (t1003 - t2416) * t8) *
     # t8
        t3454 = t3345 - t1003
        t3458 = (t4 * t3454 * t130 - t1043) * t130
        t3469 = (t4 * (t922 - t1007) * t8 - t4 * (t1007 - t2420) * t8) *
     # t8
        t3470 = t3349 - t1007
        t3474 = (t4 * t3470 * t130 - t1059) * t130
        t3480 = (t4 * (t917 + t530 + t927 - t1002 - t594 - t1012) * t8 -
     # t4 * (t1002 + t594 + t1012 - t2415 - t2001 - t2425) * t8) * t8 + 
     #t3445 + (t4 * (t3453 + t3458 + t3321 - t1002 - t594 - t1012) * t18
     #5 - t4 * (t1002 + t594 + t1012 - t3469 - t3474 - t3327) * t185) * 
     #t185 - t997 - t1035 - t1069
        t3484 = t415 * dy
        t3485 = t3445 - t1035
        t3488 = dt * t1085
        t3491 = j + 4
        t3493 = u(i,t3491,k,n) - t1587
        t3497 = (t4 * t3493 * t130 - t1590) * t130
        t3501 = ((t3497 - t1592) * t130 - t1594) * t130
        t3510 = (t3493 * t130 - t1725) * t130 - t1727
        t3514 = (t4 * t3510 * t130 - t1753) * t130
        t3533 = u(i,t126,t1139,n)
        t3534 = t3533 - t3171
        t3538 = (t3534 * t185 - t3173) * t185 - t3176
        t3545 = u(i,t126,t1152,n)
        t3546 = t3183 - t3545
        t3550 = t3187 - (t3185 - t3546 * t185) * t185
        t3561 = t3180 * t185
        t3564 = t3188 * t185
        t3566 = (t3561 - t3564) * t185
        t3606 = t1398 - t351
        t3610 = (t3606 * t8 - t3144) * t8 - t3147
        t3617 = t1884 - t2794
        t3621 = t3156 - (t3154 - t3617 * t8) * t8
        t3632 = t3151 * t8
        t3635 = t3157 * t8
        t3637 = (t3632 - t3635) * t8
        t3677 = 0.3E1 / 0.640E3 * t1085 * ((t3501 - t1596) * t130 - t159
     #8) + t1085 * ((t3514 - t1755) * t130 - t1757) / 0.576E3 + 0.3E1 / 
     #0.640E3 * t1085 * (t4 * ((t3510 * t130 - t1729) * t130 - t1732) * 
     #t130 - t1738) + t271 - dz * t3191 / 0.24E2 - dz * t3205 / 0.24E2 +
     # t1138 * (((t4 * t3538 * t185 - t3182) * t185 - t3192) * t185 - (t
     #3192 - (t3190 - t4 * t3550 * t185) * t185) * t185) / 0.576E3 + 0.3
     #E1 / 0.640E3 * t1138 * (t4 * ((t3538 * t185 - t3561) * t185 - t356
     #6) * t185 - t4 * (t3566 - (t3564 - t3550 * t185) * t185) * t185) +
     # 0.3E1 / 0.640E3 * t1138 * (((((t4 * t3534 * t185 - t3194) * t185 
     #- t3196) * t185 - t3198) * t185 - t3206) * t185 - (t3206 - (t3204 
     #- (t3202 - (t3200 - t4 * t3546 * t185) * t185) * t185) * t185) * t
     #185) - dx * t3160 / 0.24E2 - dx * t3166 / 0.24E2 + t26 * (((t4 * t
     #3610 * t8 - t3153) * t8 - t3161) * t8 - (t3161 - (t3159 - t4 * t36
     #21 * t8) * t8) * t8) / 0.576E3 + 0.3E1 / 0.640E3 * t26 * (t4 * ((t
     #3610 * t8 - t3632) * t8 - t3637) * t8 - t4 * (t3637 - (t3635 - t36
     #21 * t8) * t8) * t8) + 0.3E1 / 0.640E3 * t26 * (((((t4 * t3606 * t
     #8 - t734) * t8 - t739) * t8 - t3163) * t8 - t3167) * t8 - (t3167 -
     # (t3165 - (t2330 - (t2328 - t4 * t3617 * t8) * t8) * t8) * t8) * t
     #8) - dy * t1754 / 0.24E2 - dy * t1595 / 0.24E2 + t824 + t834
        t3681 = t572 / 0.2E1
        t3686 = t139 ** 2
        t3688 = ut(i,t3491,k,n) - t3125
        t3692 = (t3688 * t130 - t3127) * t130 - t3129
        t3698 = t3138 * t130
        t3705 = dy * (t571 / 0.2E1 + t3681 - t139 * (t3131 / 0.2E1 + t31
     #32 / 0.2E1) / 0.6E1 + t3686 * (((t3692 * t130 - t3131) * t130 - t3
     #134) * t130 / 0.2E1 + t3698 / 0.2E1) / 0.30E2) / 0.2E1
        t3706 = t1012 + t1002 + t594 - t3268 - t3295 - t3334
        t3709 = dt * dy
        t3710 = u(i,t140,t195,n)
        t3711 = t3710 - t3223
        t3713 = t3224 * t185
        t3716 = t3228 * t185
        t3718 = (t3713 - t3716) * t185
        t3722 = u(i,t140,t208,n)
        t3723 = t3227 - t3722
        t3752 = t1316 - t141
        t3754 = t3215 * t8
        t3757 = t3218 * t8
        t3759 = (t3754 - t3757) * t8
        t3763 = t1842 - t2752
        t3789 = -t194 * ((t4 * ((t3711 * t185 - t3713) * t185 - t3718) *
     # t185 - t4 * (t3718 - (t3716 - t3723 * t185) * t185) * t185) * t18
     #5 + (((t4 * t3711 * t185 - t3226) * t185 - t3232) * t185 - (t3232 
     #- (t3230 - t4 * t3723 * t185) * t185) * t185) * t185) / 0.24E2 - t
     #139 * (t3514 + t3501) / 0.24E2 - t25 * ((t4 * ((t3752 * t8 - t3754
     #) * t8 - t3759) * t8 - t4 * (t3759 - (t3757 - t3763 * t8) * t8) * 
     #t8) * t8 + (((t4 * t3752 * t8 - t3217) * t8 - t3222) * t8 - (t3222
     # - (t3220 - t4 * t3763 * t8) * t8) * t8) * t8) / 0.24E2 + t3222 + 
     #t3232 + t1592 - t834 - t824 - t271 + t3170 + t3209 + t3212
        t3790 = t3789 * t130
        t3792 = t3214 / 0.2E1
        t3801 = u(i,t1086,t181,n)
        t3805 = u(i,t1086,t187,n)
        t3816 = ((((t4 * (t1087 - t1587) * t8 - t4 * (t1587 - t2580) * t
     #8) * t8 + t3497 + (t4 * (t3801 - t1587) * t185 - t4 * (t1587 - t38
     #05) * t185) * t185 - t3222 - t1592 - t3232) * t130 - t3234) * t130
     # - t3237) * t130
        t3817 = t3241 * t130
        t3822 = t3790 / 0.2E1 + t3792 - t139 * (t3816 / 0.2E1 + t3817 / 
     #0.2E1) / 0.6E1
        t3829 = t139 * (t574 - dy * t3133 / 0.12E2) / 0.12E2
        t3830 = ut(i,t140,t195,n)
        t3831 = t3830 - t3345
        t3833 = t3346 * t185
        t3836 = t3350 * t185
        t3838 = (t3833 - t3836) * t185
        t3842 = ut(i,t140,t208,n)
        t3843 = t3349 - t3842
        t3869 = t1491 - t505
        t3871 = t3337 * t8
        t3874 = t3340 * t8
        t3876 = (t3871 - t3874) * t8
        t3880 = t1976 - t2832
        t3913 = (t4 * t3688 * t130 - t3259) * t130
        t3921 = -t194 * ((t4 * ((t3831 * t185 - t3833) * t185 - t3838) *
     # t185 - t4 * (t3838 - (t3836 - t3843 * t185) * t185) * t185) * t18
     #5 + (((t4 * t3831 * t185 - t3348) * t185 - t3354) * t185 - (t3354 
     #- (t3352 - t4 * t3843 * t185) * t185) * t185) * t185) / 0.24E2 - t
     #25 * ((t4 * ((t3869 * t8 - t3871) * t8 - t3876) * t8 - t4 * (t3876
     # - (t3874 - t3880 * t8) * t8) * t8) * t8 + (((t4 * t3869 * t8 - t3
     #339) * t8 - t3344) * t8 - (t3344 - (t3342 - t4 * t3880 * t8) * t8)
     # * t8) * t8) / 0.24E2 + t3354 + t3261 + t3344 - t139 * ((t4 * t369
     #2 * t130 - t3255) * t130 + ((t3913 - t3261) * t130 - t3263) * t130
     #) / 0.24E2 - t1012 - t1002 - t594 + t3268 + t3295 + t3334
        t3924 = t3336 / 0.2E1
        t3951 = t3363 * t130
        t3956 = t3921 * t130 / 0.2E1 + t3924 - t139 * (((((t4 * (ut(t5,t
     #1086,k,n) - t3125) * t8 - t4 * (t3125 - ut(t16,t1086,k,n)) * t8) *
     # t8 + t3913 + (t4 * (ut(i,t1086,t181,n) - t3125) * t185 - t4 * (t3
     #125 - ut(i,t1086,t187,n)) * t185) * t185 - t3344 - t3261 - t3354) 
     #* t130 - t3356) * t130 - t3359) * t130 / 0.2E1 + t3951 / 0.2E1) / 
     #0.6E1
        t3961 = t3816 - t3817
        t3964 = (t3790 - t3214) * t130 - dy * t3961 / 0.12E2
        t3970 = t1085 * t3133 / 0.720E3
        t3973 = t559 + dt * t3677 / 0.2E1 - t3705 + t95 * t3706 / 0.8E1 
     #- t3709 * t3822 / 0.4E1 + t3829 - t3428 * t3956 / 0.16E2 + t3369 *
     # t3964 / 0.24E2 + t3428 * t3358 / 0.96E2 - t3970 - t3488 * t3961 /
     # 0.1440E4
        t3974 = t575 / 0.2E1
        t3979 = ut(i,t1099,k,n)
        t3980 = t581 - t3979
        t3981 = t3980 * t130
        t3983 = (t583 - t3981) * t130
        t3984 = t585 - t3983
        t3985 = t3984 * t130
        t3986 = t3135 - t3985
        t3987 = t3986 * t130
        t3988 = t3137 - t3987
        t3989 = t3988 * t130
        t3996 = dy * (t3681 + t3974 - t139 * (t3132 / 0.2E1 + t3135 / 0.
     #2E1) / 0.6E1 + t3686 * (t3698 / 0.2E1 + t3989 / 0.2E1) / 0.30E2) /
     # 0.2E1
        t3997 = t753 * t8
        t3998 = t756 * t8
        t4000 = (t3997 - t3998) * t8
        t4001 = t838 * t8
        t4003 = (t3998 - t4001) * t8
        t4004 = t4000 - t4003
        t4006 = t4 * t4004 * t8
        t4007 = t2344 * t8
        t4009 = (t4001 - t4007) * t8
        t4010 = t4003 - t4009
        t4012 = t4 * t4010 * t8
        t4013 = t4006 - t4012
        t4014 = t4013 * t8
        t4016 = (t760 - t842) * t8
        t4018 = (t842 - t2348) * t8
        t4019 = t4016 - t4018
        t4020 = t4019 * t8
        t4023 = t25 * (t4014 + t4020) / 0.24E2
        t4024 = u(i,t132,t195,n)
        t4025 = t4024 - t843
        t4026 = t4025 * t185
        t4027 = t844 * t185
        t4029 = (t4026 - t4027) * t185
        t4030 = t848 * t185
        t4032 = (t4027 - t4030) * t185
        t4033 = t4029 - t4032
        t4035 = t4 * t4033 * t185
        t4036 = u(i,t132,t208,n)
        t4037 = t847 - t4036
        t4038 = t4037 * t185
        t4040 = (t4030 - t4038) * t185
        t4041 = t4032 - t4040
        t4043 = t4 * t4041 * t185
        t4044 = t4035 - t4043
        t4045 = t4044 * t185
        t4047 = t4 * t4025 * t185
        t4049 = (t4047 - t846) * t185
        t4051 = (t4049 - t852) * t185
        t4053 = t4 * t4037 * t185
        t4055 = (t850 - t4053) * t185
        t4057 = (t852 - t4055) * t185
        t4058 = t4051 - t4057
        t4059 = t4058 * t185
        t4062 = t194 * (t4045 + t4059) / 0.24E2
        t4065 = t139 * (t1761 + t1608) / 0.24E2
        t4066 = t118 + t245 - t284 + t294 - t333 - t348 - t277 + t4023 +
     # t4062 + t4065 - t842 - t852
        t4067 = t4066 * t130
        t4068 = t4067 / 0.2E1
        t4069 = t154 - t258
        t4071 = t4 * t4069 * t8
        t4072 = t258 - t1854
        t4074 = t4 * t4072 * t8
        t4076 = (t4071 - t4074) * t8
        t4077 = u(i,t153,t181,n)
        t4078 = t4077 - t258
        t4080 = t4 * t4078 * t185
        t4081 = u(i,t153,t187,n)
        t4082 = t258 - t4081
        t4084 = t4 * t4082 * t185
        t4086 = (t4080 - t4084) * t185
        t4087 = t842 + t277 + t852 - t4076 - t1604 - t4086
        t4088 = t4087 * t130
        t4090 = (t3238 - t4088) * t130
        t4091 = t3240 - t4090
        t4092 = t4091 * t130
        t4097 = t3792 + t4068 - t139 * (t3817 / 0.2E1 + t4092 / 0.2E1) /
     # 0.6E1
        t4099 = t3709 * t4097 / 0.4E1
        t4104 = t139 * (t577 - dy * t3136 / 0.12E2) / 0.12E2
        t4106 = t4 * t3984 * t130
        t4110 = t4 * t3980 * t130
        t4112 = (t598 - t4110) * t130
        t4114 = (t600 - t4112) * t130
        t4116 = (t602 - t4114) * t130
        t4119 = t139 * ((t588 - t4106) * t130 + t4116) / 0.24E2
        t4120 = ut(i,t132,t195,n)
        t4121 = t4120 - t1021
        t4123 = t1022 * t185
        t4126 = t1026 * t185
        t4128 = (t4123 - t4126) * t185
        t4132 = ut(i,t132,t208,n)
        t4133 = t1025 - t4132
        t4145 = (t4 * t4121 * t185 - t1024) * t185
        t4151 = (t1028 - t4 * t4133 * t185) * t185
        t4158 = t194 * ((t4 * ((t4121 * t185 - t4123) * t185 - t4128) * 
     #t185 - t4 * (t4128 - (t4126 - t4133 * t185) * t185) * t185) * t185
     # + ((t4145 - t1030) * t185 - (t1030 - t4151) * t185) * t185) / 0.2
     #4E2
        t4160 = t934 * t8
        t4163 = t1016 * t8
        t4165 = (t4160 - t4163) * t8
        t4185 = t25 * ((t4 * ((t931 * t8 - t4160) * t8 - t4165) * t8 - t
     #4 * (t4165 - (t4163 - t2429 * t8) * t8) * t8) * t8 + ((t938 - t102
     #0) * t8 - (t1020 - t2433) * t8) * t8) / 0.24E2
        t4186 = t487 - t558 + t568 - t607 + t617 - t656 - t1020 - t600 +
     # t4119 + t4158 - t1030 + t4185
        t4187 = t4186 * t130
        t4188 = t4187 / 0.2E1
        t4189 = t517 - t581
        t4191 = t4 * t4189 * t8
        t4192 = t581 - t1988
        t4194 = t4 * t4192 * t8
        t4196 = (t4191 - t4194) * t8
        t4197 = ut(i,t153,t181,n)
        t4198 = t4197 - t581
        t4200 = t4 * t4198 * t185
        t4201 = ut(i,t153,t187,n)
        t4202 = t581 - t4201
        t4204 = t4 * t4202 * t185
        t4206 = (t4200 - t4204) * t185
        t4207 = t1020 + t600 + t1030 - t4196 - t4112 - t4206
        t4208 = t4207 * t130
        t4209 = t3360 - t4208
        t4210 = t4209 * t130
        t4211 = t3362 - t4210
        t4212 = t4211 * t130
        t4217 = t3924 + t4188 - t139 * (t3951 / 0.2E1 + t4212 / 0.2E1) /
     # 0.6E1
        t4219 = t3428 * t4217 / 0.16E2
        t4222 = t3817 - t4092
        t4225 = (t3214 - t4067) * t130 - dy * t4222 / 0.12E2
        t4227 = t3369 * t4225 / 0.24E2
        t4229 = t3428 * t3361 / 0.96E2
        t4231 = t1085 * t3136 / 0.720E3
        t4233 = t3488 * t4222 / 0.1440E4
        t4234 = -t2 - t1773 - t3996 - t1799 - t4099 - t4104 - t4219 - t4
     #227 - t4229 + t4231 + t4233
        t4239 = t3122 + t53 * t3141 / 0.2E1 + t96 * t3244 / 0.8E1 - t325
     #3 + t416 * t3366 / 0.48E2 - t3369 * t3375 / 0.48E2 + t725 * t3424 
     #* t130 / 0.384E3 - t3428 * t3429 / 0.192E3 + t3433 + t903 * t3480 
     #* t130 / 0.3840E4 - t3484 * t3485 / 0.2304E4 + 0.7E1 / 0.11520E5 *
     # t3488 * t3372 + cc * (t3973 + t4234) * t2068 / 0.32E2
        t4242 = dt * t3141
        t4244 = t95 * t3244
        t4247 = t415 * t3366
        t4250 = t139 * t3375
        t4254 = t724 * t3424 * t130
        t4257 = dy * t3429
        t4261 = t902 * t3480 * t130
        t4264 = dy * t3485
        t4267 = t1085 * t3372
        t4273 = dy * t3822
        t4276 = dy * t3956
        t4279 = t139 * t3964
        t4282 = dy * t3358
        t4285 = t1085 * t3961
        t4288 = t559 + t2076 * t3677 - t3705 + t2107 * t3706 / 0.2E1 - t
     #2076 * t4273 / 0.2E1 + t3829 - t2107 * t4276 / 0.4E1 + t2076 * t42
     #79 / 0.12E2 + t2107 * t4282 / 0.24E2 - t3970 - t2076 * t4285 / 0.7
     #20E3
        t4289 = dy * t4097
        t4291 = t2076 * t4289 / 0.2E1
        t4292 = dy * t4217
        t4294 = t2107 * t4292 / 0.4E1
        t4295 = t139 * t4225
        t4297 = t2076 * t4295 / 0.12E2
        t4298 = dy * t3361
        t4300 = t2107 * t4298 / 0.24E2
        t4301 = t1085 * t4222
        t4303 = t2076 * t4301 / 0.720E3
        t4304 = -t2 - t2143 - t3996 - t2145 - t4291 - t4104 - t4294 - t4
     #297 - t4300 + t4231 + t4303
        t4309 = t3122 + t2085 * t4242 + t2089 * t4244 / 0.2E1 - t3253 + 
     #t2094 * t4247 / 0.6E1 - t2076 * t4250 / 0.24E2 + t2102 * t4254 / 0
     #.24E2 - t2107 * t4257 / 0.48E2 + t3433 + t2112 * t4261 / 0.120E3 -
     # t2117 * t4264 / 0.288E3 + 0.7E1 / 0.5760E4 * t2076 * t4267 + cc *
     # (t4288 + t4304) * t2068 / 0.32E2
        t4342 = t559 + t2081 * t3677 - t3705 + t2189 * t3706 / 0.2E1 - t
     #2081 * t4273 / 0.2E1 + t3829 - t2189 * t4276 / 0.4E1 + t2081 * t42
     #79 / 0.12E2 + t2189 * t4282 / 0.24E2 - t3970 - t2081 * t4285 / 0.7
     #20E3
        t4344 = t2081 * t4289 / 0.2E1
        t4346 = t2189 * t4292 / 0.4E1
        t4348 = t2081 * t4295 / 0.12E2
        t4350 = t2189 * t4298 / 0.24E2
        t4352 = t2081 * t4301 / 0.720E3
        t4353 = -t2 - t2215 - t3996 - t2217 - t4344 - t4104 - t4346 - t4
     #348 - t4350 + t4231 + t4352
        t4358 = t3122 + t2173 * t4242 + t2176 * t4244 / 0.2E1 - t3253 + 
     #t2180 * t4247 / 0.6E1 - t2081 * t4250 / 0.24E2 + t2186 * t4254 / 0
     #.24E2 - t2189 * t4257 / 0.48E2 + t3433 + t2193 * t4261 / 0.120E3 -
     # t2196 * t4264 / 0.288E3 + 0.7E1 / 0.5760E4 * t2081 * t4267 + cc *
     # (t4342 + t4353) * t2068 / 0.32E2
        t4361 = t4239 * t2078 * t2083 + t4309 * t2168 * t2171 + t4358 * 
     #t2235 * t2238
        t4365 = t4309 * dt
        t4371 = t4239 * dt
        t4377 = t4358 * dt
        t4383 = (-t4365 / 0.2E1 - t4365 * t2080) * t2168 * t2171 + (-t43
     #71 * t2075 - t4371 * t2080) * t2078 * t2083 + (-t4377 * t2075 - t4
     #377 / 0.2E1) * t2235 * t2238
        t4404 = t4 * (t252 - dy * t263 / 0.24E2 + 0.3E1 / 0.640E3 * t108
     #5 * t1746)
        t4409 = t575 - dy * t586 / 0.24E2 + 0.3E1 / 0.640E3 * t1085 * t3
     #988
        t4414 = t4067 - dy * t4091 / 0.24E2
        t4423 = t139 * ((t245 - t284 - t277 + t4065) * t130 - dy * t1609
     # / 0.24E2) / 0.24E2
        t4426 = t4187 - dy * t4211 / 0.24E2
        t4431 = t604 - t4116
        t4434 = (t568 - t607 - t600 + t4119) * t130 - dy * t4431 / 0.24E
     #2
        t4448 = (t855 - t4 * t4087 * t130) * t130
        t4456 = (t4 * (t761 - t843) * t8 - t4 * (t843 - t2349) * t8) * t
     #8
        t4457 = t843 - t4077
        t4459 = t4 * t4457 * t130
        t4461 = (t868 - t4459) * t130
        t4472 = (t4 * (t765 - t847) * t8 - t4 * (t847 - t2353) * t8) * t
     #8
        t4473 = t847 - t4081
        t4475 = t4 * t4473 * t130
        t4477 = (t884 - t4475) * t130
        t4483 = t819 + t857 + t891 - (t4 * (t760 + t173 + t770 - t842 - 
     #t277 - t852) * t8 - t4 * (t842 + t277 + t852 - t2348 - t1873 - t23
     #58) * t8) * t8 - t4448 - (t4 * (t4456 + t4461 + t4049 - t842 - t27
     #7 - t852) * t185 - t4 * (t842 + t277 + t852 - t4472 - t4477 - t405
     #5) * t185) * t185
        t4487 = t857 - t4448
        t4491 = 0.7E1 / 0.5760E4 * t1085 * t1609
        t4503 = (t1033 - t4 * t4207 * t130) * t130
        t4511 = (t4 * (t939 - t1021) * t8 - t4 * (t1021 - t2434) * t8) *
     # t8
        t4512 = t1021 - t4197
        t4516 = (t1046 - t4 * t4512 * t130) * t130
        t4527 = (t4 * (t943 - t1025) * t8 - t4 * (t1025 - t2438) * t8) *
     # t8
        t4528 = t1025 - t4201
        t4532 = (t1062 - t4 * t4528 * t130) * t130
        t4538 = t997 + t1035 + t1069 - (t4 * (t938 + t536 + t948 - t1020
     # - t600 - t1030) * t8 - t4 * (t1020 + t600 + t1030 - t2433 - t2007
     # - t2443) * t8) * t8 - t4503 - (t4 * (t4511 + t4516 + t4145 - t102
     #0 - t600 - t1030) * t185 - t4 * (t1020 + t600 + t1030 - t4527 - t4
     #532 - t4151) * t185) * t185
        t4542 = t1035 - t4503
        t4547 = t2 + t1773 - t3996 + t1799 - t4099 + t4104 - t4219 + t42
     #27 + t4229 - t4231 - t4233
        t4552 = j - 4
        t4554 = t1599 - u(i,t4552,k,n)
        t4558 = t1741 - (t1739 - t4554 * t130) * t130
        t4562 = (t1759 - t4 * t4558 * t130) * t130
        t4581 = u(i,t132,t1139,n)
        t4582 = t4581 - t4024
        t4586 = (t4582 * t185 - t4026) * t185 - t4029
        t4593 = u(i,t132,t1152,n)
        t4594 = t4036 - t4593
        t4598 = t4040 - (t4038 - t4594 * t185) * t185
        t4635 = t1402 - t355
        t4639 = (t4635 * t8 - t3997) * t8 - t4000
        t4646 = t1888 - t2798
        t4650 = t4009 - (t4007 - t4646 * t8) * t8
        t4661 = t4004 * t8
        t4664 = t4010 * t8
        t4666 = (t4661 - t4664) * t8
        t4703 = t4033 * t185
        t4706 = t4041 * t185
        t4708 = (t4703 - t4706) * t185
        t4724 = (t1602 - t4 * t4554 * t130) * t130
        t4728 = (t1606 - (t1604 - t4724) * t130) * t130
        t4734 = -dy * t1760 / 0.24E2 - dy * t1607 / 0.24E2 + t1085 * (t1
     #763 - (t1761 - t4562) * t130) / 0.576E3 + 0.3E1 / 0.640E3 * t1085 
     #* (t1748 - t4 * (t1745 - (t1743 - t4558 * t130) * t130) * t130) - 
     #dz * t4044 / 0.24E2 - dz * t4058 / 0.24E2 + t1138 * (((t4 * t4586 
     #* t185 - t4035) * t185 - t4045) * t185 - (t4045 - (t4043 - t4 * t4
     #598 * t185) * t185) * t185) / 0.576E3 + 0.3E1 / 0.640E3 * t1138 * 
     #(((((t4 * t4582 * t185 - t4047) * t185 - t4049) * t185 - t4051) * 
     #t185 - t4059) * t185 - (t4059 - (t4057 - (t4055 - (t4053 - t4 * t4
     #594 * t185) * t185) * t185) * t185) * t185) - dx * t4013 / 0.24E2 
     #- dx * t4019 / 0.24E2 + t26 * (((t4 * t4639 * t8 - t4006) * t8 - t
     #4014) * t8 - (t4014 - (t4012 - t4 * t4650 * t8) * t8) * t8) / 0.57
     #6E3 + 0.3E1 / 0.640E3 * t26 * (t4 * ((t4639 * t8 - t4661) * t8 - t
     #4666) * t8 - t4 * (t4666 - (t4664 - t4650 * t8) * t8) * t8) + 0.3E
     #1 / 0.640E3 * t26 * (((((t4 * t4635 * t8 - t755) * t8 - t760) * t8
     # - t4016) * t8 - t4020) * t8 - (t4020 - (t4018 - (t2348 - (t2346 -
     # t4 * t4646 * t8) * t8) * t8) * t8) * t8) + 0.3E1 / 0.640E3 * t113
     #8 * (t4 * ((t4586 * t185 - t4703) * t185 - t4708) * t185 - t4 * (t
     #4708 - (t4706 - t4598 * t185) * t185) * t185) + 0.3E1 / 0.640E3 * 
     #t1085 * (t1610 - (t1608 - t4728) * t130) + t277 + t842 + t852
        t4743 = t3979 - ut(i,t4552,k,n)
        t4747 = t3983 - (t3981 - t4743 * t130) * t130
        t4759 = dy * (t3974 + t583 / 0.2E1 - t139 * (t3135 / 0.2E1 + t39
     #85 / 0.2E1) / 0.6E1 + t3686 * (t3989 / 0.2E1 + (t3987 - (t3985 - t
     #4747 * t130) * t130) * t130 / 0.2E1) / 0.30E2) / 0.2E1
        t4760 = t1020 + t600 - t4119 - t4158 + t1030 - t4185
        t4766 = u(i,t153,t195,n)
        t4767 = t4766 - t4077
        t4769 = t4078 * t185
        t4772 = t4082 * t185
        t4774 = (t4769 - t4772) * t185
        t4778 = u(i,t153,t208,n)
        t4779 = t4081 - t4778
        t4805 = t1328 - t154
        t4807 = t4069 * t8
        t4810 = t4072 * t8
        t4812 = (t4807 - t4810) * t8
        t4816 = t1854 - t2764
        t4842 = t277 - t4023 - t4062 - t4065 + t842 + t852 + t139 * (t45
     #62 + t4728) / 0.24E2 + t194 * ((t4 * ((t4767 * t185 - t4769) * t18
     #5 - t4774) * t185 - t4 * (t4774 - (t4772 - t4779 * t185) * t185) *
     # t185) * t185 + (((t4 * t4767 * t185 - t4080) * t185 - t4086) * t1
     #85 - (t4086 - (t4084 - t4 * t4779 * t185) * t185) * t185) * t185) 
     #/ 0.24E2 + t25 * ((t4 * ((t4805 * t8 - t4807) * t8 - t4812) * t8 -
     # t4 * (t4812 - (t4810 - t4816 * t8) * t8) * t8) * t8 + (((t4 * t48
     #05 * t8 - t4071) * t8 - t4076) * t8 - (t4076 - (t4074 - t4 * t4816
     # * t8) * t8) * t8) * t8) / 0.24E2 - t4076 - t4086 - t1604
        t4843 = t4842 * t130
        t4853 = u(i,t1099,t181,n)
        t4857 = u(i,t1099,t187,n)
        t4868 = (t4090 - (t4088 - (t4076 + t1604 + t4086 - (t4 * (t1100 
     #- t1599) * t8 - t4 * (t1599 - t2592) * t8) * t8 - t4724 - (t4 * (t
     #4853 - t1599) * t185 - t4 * (t1599 - t4857) * t185) * t185) * t130
     #) * t130) * t130
        t4873 = t4068 + t4843 / 0.2E1 - t139 * (t4092 / 0.2E1 + t4868 / 
     #0.2E1) / 0.6E1
        t4880 = t139 * (t585 - dy * t3986 / 0.12E2) / 0.12E2
        t4888 = (t4110 - t4 * t4743 * t130) * t130
        t4896 = t1503 - t517
        t4898 = t4189 * t8
        t4901 = t4192 * t8
        t4903 = (t4898 - t4901) * t8
        t4907 = t1988 - t2844
        t4933 = ut(i,t153,t195,n)
        t4934 = t4933 - t4197
        t4936 = t4198 * t185
        t4939 = t4202 * t185
        t4941 = (t4936 - t4939) * t185
        t4945 = ut(i,t153,t208,n)
        t4946 = t4201 - t4945
        t4972 = t1020 + t600 - t4119 - t4158 + t1030 - t4185 + t139 * ((
     #t4106 - t4 * t4747 * t130) * t130 + (t4114 - (t4112 - t4888) * t13
     #0) * t130) / 0.24E2 + t25 * ((t4 * ((t4896 * t8 - t4898) * t8 - t4
     #903) * t8 - t4 * (t4903 - (t4901 - t4907 * t8) * t8) * t8) * t8 + 
     #(((t4 * t4896 * t8 - t4191) * t8 - t4196) * t8 - (t4196 - (t4194 -
     # t4 * t4907 * t8) * t8) * t8) * t8) / 0.24E2 + t194 * ((t4 * ((t49
     #34 * t185 - t4936) * t185 - t4941) * t185 - t4 * (t4941 - (t4939 -
     # t4946 * t185) * t185) * t185) * t185 + (((t4 * t4934 * t185 - t42
     #00) * t185 - t4206) * t185 - (t4206 - (t4204 - t4 * t4946 * t185) 
     #* t185) * t185) * t185) / 0.24E2 - t4112 - t4196 - t4206
        t5005 = t4188 + t4972 * t130 / 0.2E1 - t139 * (t4212 / 0.2E1 + (
     #t4210 - (t4208 - (t4196 + t4112 + t4206 - (t4 * (ut(t5,t1099,k,n) 
     #- t3979) * t8 - t4 * (t3979 - ut(t16,t1099,k,n)) * t8) * t8 - t488
     #8 - (t4 * (ut(i,t1099,t181,n) - t3979) * t185 - t4 * (t3979 - ut(i
     #,t1099,t187,n)) * t185) * t185) * t130) * t130) * t130 / 0.2E1) / 
     #0.6E1
        t5010 = t4092 - t4868
        t5013 = (t4067 - t4843) * t130 - dy * t5010 / 0.12E2
        t5019 = t1085 * t3986 / 0.720E3
        t5022 = -t563 - dt * t4734 / 0.2E1 - t4759 - t95 * t4760 / 0.8E1
     # - t3709 * t4873 / 0.4E1 - t4880 - t3428 * t5005 / 0.16E2 - t3369 
     #* t5013 / 0.24E2 - t3428 * t4209 / 0.96E2 + t5019 + t3488 * t5010 
     #/ 0.1440E4
        t5027 = t4404 + t53 * t4409 / 0.2E1 + t96 * t4414 / 0.8E1 - t442
     #3 + t416 * t4426 / 0.48E2 - t3369 * t4434 / 0.48E2 + t725 * t4483 
     #* t130 / 0.384E3 - t3428 * t4487 / 0.192E3 + t4491 + t903 * t4538 
     #* t130 / 0.3840E4 - t3484 * t4542 / 0.2304E4 + 0.7E1 / 0.11520E5 *
     # t3488 * t4431 + cc * (t4547 + t5022) * t2068 / 0.32E2
        t5030 = dt * t4409
        t5032 = t95 * t4414
        t5035 = t415 * t4426
        t5038 = t139 * t4434
        t5042 = t724 * t4483 * t130
        t5045 = dy * t4487
        t5049 = t902 * t4538 * t130
        t5052 = dy * t4542
        t5055 = t1085 * t4431
        t5058 = t2 + t2143 - t3996 + t2145 - t4291 + t4104 - t4294 + t42
     #97 + t4300 - t4231 - t4303
        t5062 = dy * t4873
        t5065 = dy * t5005
        t5068 = t139 * t5013
        t5071 = dy * t4209
        t5074 = t1085 * t5010
        t5077 = -t563 - t2076 * t4734 - t4759 - t2107 * t4760 / 0.2E1 - 
     #t2076 * t5062 / 0.2E1 - t4880 - t2107 * t5065 / 0.4E1 - t2076 * t5
     #068 / 0.12E2 - t2107 * t5071 / 0.24E2 + t5019 + t2076 * t5074 / 0.
     #720E3
        t5082 = t4404 + t2085 * t5030 + t2089 * t5032 / 0.2E1 - t4423 + 
     #t2094 * t5035 / 0.6E1 - t2076 * t5038 / 0.24E2 + t2102 * t5042 / 0
     #.24E2 - t2107 * t5045 / 0.48E2 + t4491 + t2112 * t5049 / 0.120E3 -
     # t2117 * t5052 / 0.288E3 + 0.7E1 / 0.5760E4 * t2076 * t5055 + cc *
     # (t5058 + t5077) * t2068 / 0.32E2
        t5102 = t2 + t2215 - t3996 + t2217 - t4344 + t4104 - t4346 + t43
     #48 + t4350 - t4231 - t4352
        t5116 = -t563 - t2081 * t4734 - t4759 - t2189 * t4760 / 0.2E1 - 
     #t2081 * t5062 / 0.2E1 - t4880 - t2189 * t5065 / 0.4E1 - t2081 * t5
     #068 / 0.12E2 - t2189 * t5071 / 0.24E2 + t5019 + t2081 * t5074 / 0.
     #720E3
        t5121 = t4404 + t2173 * t5030 + t2176 * t5032 / 0.2E1 - t4423 + 
     #t2180 * t5035 / 0.6E1 - t2081 * t5038 / 0.24E2 + t2186 * t5042 / 0
     #.24E2 - t2189 * t5045 / 0.48E2 + t4491 + t2193 * t5049 / 0.120E3 -
     # t2196 * t5052 / 0.288E3 + 0.7E1 / 0.5760E4 * t2081 * t5055 + cc *
     # (t5102 + t5116) * t2068 / 0.32E2
        t5124 = t5027 * t2078 * t2083 + t5082 * t2168 * t2171 + t5121 * 
     #t2235 * t2238
        t5128 = t5082 * dt
        t5134 = t5027 * dt
        t5140 = t5121 * dt
        t5146 = (-t5128 / 0.2E1 - t5128 * t2080) * t2168 * t2171 + (-t51
     #34 * t2075 - t5134 * t2080) * t2078 * t2083 + (-t5140 * t2075 - t5
     #140 / 0.2E1) * t2235 * t2238
        t5169 = t4 * (t298 - dz * t304 / 0.24E2 + 0.3E1 / 0.640E3 * t113
     #8 * t1648)
        t5172 = ut(i,j,t1139,n)
        t5173 = t5172 - t618
        t5174 = t5173 * t185
        t5176 = (t5174 - t620) * t185
        t5177 = t5176 - t623
        t5178 = t5177 * t185
        t5179 = t627 * t185
        t5180 = t5178 - t5179
        t5181 = t5180 * t185
        t5182 = t635 * t185
        t5183 = t5179 - t5182
        t5184 = t5183 * t185
        t5185 = t5181 - t5184
        t5188 = t621 - dz * t627 / 0.24E2 + 0.3E1 / 0.640E3 * t1138 * t5
     #185
        t5191 = t3398 * t130
        t5192 = t863 * t130
        t5194 = (t5191 - t5192) * t130
        t5195 = t866 * t130
        t5197 = (t5192 - t5195) * t130
        t5198 = t5194 - t5197
        t5200 = t4 * t5198 * t130
        t5201 = t4457 * t130
        t5203 = (t5195 - t5201) * t130
        t5204 = t5197 - t5203
        t5206 = t4 * t5204 * t130
        t5207 = t5200 - t5206
        t5208 = t5207 * t130
        t5210 = (t3402 - t870) * t130
        t5212 = (t870 - t4461) * t130
        t5213 = t5210 - t5212
        t5214 = t5213 * t130
        t5217 = t139 * (t5208 + t5214) / 0.24E2
        t5220 = t194 * (t1623 + t1667) / 0.24E2
        t5221 = t776 * t8
        t5222 = t779 * t8
        t5224 = (t5221 - t5222) * t8
        t5225 = t858 * t8
        t5227 = (t5222 - t5225) * t8
        t5228 = t5224 - t5227
        t5230 = t4 * t5228 * t8
        t5231 = t2364 * t8
        t5233 = (t5225 - t5231) * t8
        t5234 = t5227 - t5233
        t5236 = t4 * t5234 * t8
        t5237 = t5230 - t5236
        t5238 = t5237 * t8
        t5240 = (t783 - t862) * t8
        t5242 = (t862 - t2368) * t8
        t5243 = t5240 - t5242
        t5244 = t5243 * t8
        t5247 = t25 * (t5238 + t5244) / 0.24E2
        t5248 = -t5217 - t5220 - t5247 + t320 + t862 + t870 - t118 - t24
     #5 + t284 - t294 + t333 + t348
        t5249 = t5248 * t185
        t5250 = t196 - t295
        t5252 = t4 * t5250 * t8
        t5253 = t295 - t1803
        t5255 = t4 * t5253 * t8
        t5257 = (t5252 - t5255) * t8
        t5258 = t3171 - t295
        t5260 = t4 * t5258 * t130
        t5261 = t295 - t4024
        t5263 = t4 * t5261 * t130
        t5265 = (t5260 - t5263) * t130
        t5266 = t5257 + t5265 + t1663 - t862 - t870 - t320
        t5267 = t5266 * t185
        t5268 = t871 * t185
        t5270 = (t5267 - t5268) * t185
        t5271 = t887 * t185
        t5273 = (t5268 - t5271) * t185
        t5274 = t5270 - t5273
        t5277 = t5249 - dz * t5274 / 0.24E2
        t5286 = t194 * ((t320 - t5220 - t294 + t333) * t185 - dz * t1668
     # / 0.24E2) / 0.24E2
        t5288 = t957 * t8
        t5291 = t1036 * t8
        t5293 = (t5288 - t5291) * t8
        t5313 = t25 * ((t4 * ((t954 * t8 - t5288) * t8 - t5293) * t8 - t
     #4 * (t5293 - (t5291 - t2449 * t8) * t8) * t8) * t8 + ((t961 - t104
     #0) * t8 - (t1040 - t2453) * t8) * t8) / 0.24E2
        t5315 = t1041 * t130
        t5318 = t1044 * t130
        t5320 = (t5315 - t5318) * t130
        t5340 = t139 * ((t4 * ((t3454 * t130 - t5315) * t130 - t5320) * 
     #t130 - t4 * (t5320 - (t5318 - t4512 * t130) * t130) * t130) * t130
     # + ((t3458 - t1048) * t130 - (t1048 - t4516) * t130) * t130) / 0.2
     #4E2
        t5342 = t4 * t5177 * t185
        t5346 = t4 * t5173 * t185
        t5348 = (t5346 - t641) * t185
        t5350 = (t5348 - t643) * t185
        t5352 = (t5350 - t645) * t185
        t5355 = t194 * ((t5342 - t629) * t185 + t5352) / 0.24E2
        t5356 = t1040 + t1048 + t643 - t5313 - t5340 - t5355 - t487 + t5
     #58 - t568 + t607 - t617 + t656
        t5357 = t5356 * t185
        t5358 = t427 - t618
        t5360 = t4 * t5358 * t8
        t5361 = t618 - t1937
        t5363 = t4 * t5361 * t8
        t5365 = (t5360 - t5363) * t8
        t5366 = t3296 - t618
        t5368 = t4 * t5366 * t130
        t5369 = t618 - t4120
        t5371 = t4 * t5369 * t130
        t5373 = (t5368 - t5371) * t130
        t5374 = t5365 + t5373 + t5348 - t1040 - t1048 - t643
        t5375 = t5374 * t185
        t5376 = t1049 * t185
        t5377 = t5375 - t5376
        t5378 = t5377 * t185
        t5379 = t1065 * t185
        t5380 = t5376 - t5379
        t5381 = t5380 * t185
        t5382 = t5378 - t5381
        t5385 = t5357 - dz * t5382 / 0.24E2
        t5388 = dt * t194
        t5391 = t5352 - t653
        t5394 = (t643 - t5355 - t617 + t656) * t185 - dz * t5391 / 0.24E
     #2
        t5416 = (t4 * t5266 * t185 - t873) * t185
        t5417 = (t4 * (t783 + t791 + t222 - t862 - t870 - t320) * t8 - t
     #4 * (t862 + t870 + t320 - t2368 - t2376 - t1828) * t8) * t8 + (t4 
     #* (t3397 + t3402 + t3196 - t862 - t870 - t320) * t130 - t4 * (t862
     # + t870 + t320 - t4456 - t4461 - t4049) * t130) * t130 + t5416 - t
     #819 - t857 - t891
        t5421 = t95 * dz
        t5422 = t5416 - t891
        t5426 = 0.7E1 / 0.5760E4 * t1138 * t1668
        t5446 = (t4 * t5374 * t185 - t1051) * t185
        t5447 = (t4 * (t961 + t969 + t452 - t1040 - t1048 - t643) * t8 -
     # t4 * (t1040 + t1048 + t643 - t2453 - t2461 - t1962) * t8) * t8 + 
     #(t4 * (t3453 + t3458 + t3321 - t1040 - t1048 - t643) * t130 - t4 *
     # (t1040 + t1048 + t643 - t4511 - t4516 - t4145) * t130) * t130 + t
     #5446 - t997 - t1035 - t1069
        t5451 = t415 * dz
        t5452 = t5446 - t1069
        t5455 = dt * t1138
        t5458 = t3801 - t3223
        t5462 = (t5458 * t130 - t5191) * t130 - t5194
        t5469 = t4077 - t4853
        t5473 = t5203 - (t5201 - t5469 * t130) * t130
        t5484 = t5198 * t130
        t5487 = t5204 * t130
        t5489 = (t5484 - t5487) * t130
        t5529 = k + 4
        t5531 = u(i,j,t5529,n) - t1614
        t5535 = (t5531 * t185 - t1616) * t185 - t1618
        t5539 = (t4 * t5535 * t185 - t1621) * t185
        t5557 = (t4 * t5531 * t185 - t1661) * t185
        t5561 = ((t5557 - t1663) * t185 - t1665) * t185
        t5571 = t1408 - t361
        t5582 = t1894 - t2804
        t5599 = (t5571 * t8 - t5221) * t8 - t5224
        t5609 = t5233 - (t5231 - t5582 * t8) * t8
        t5620 = t5228 * t8
        t5623 = t5234 * t8
        t5625 = (t5620 - t5623) * t8
        t5642 = t320 + t1085 * (((t4 * t5462 * t130 - t5200) * t130 - t5
     #208) * t130 - (t5208 - (t5206 - t4 * t5473 * t130) * t130) * t130)
     # / 0.576E3 + 0.3E1 / 0.640E3 * t1085 * (t4 * ((t5462 * t130 - t548
     #4) * t130 - t5489) * t130 - t4 * (t5489 - (t5487 - t5473 * t130) *
     # t130) * t130) + 0.3E1 / 0.640E3 * t1085 * (((((t4 * t5458 * t130 
     #- t3400) * t130 - t3402) * t130 - t5210) * t130 - t5214) * t130 - 
     #(t5214 - (t5212 - (t4461 - (t4459 - t4 * t5469 * t130) * t130) * t
     #130) * t130) * t130) - dz * t1622 / 0.24E2 - dz * t1666 / 0.24E2 +
     # t1138 * ((t5539 - t1623) * t185 - t1625) / 0.576E3 + 0.3E1 / 0.64
     #0E3 * t1138 * (t4 * ((t5535 * t185 - t1641) * t185 - t1644) * t185
     # - t1650) + 0.3E1 / 0.640E3 * t1138 * ((t5561 - t1667) * t185 - t1
     #669) - dx * t5237 / 0.24E2 - dx * t5243 / 0.24E2 + 0.3E1 / 0.640E3
     # * t26 * (((((t4 * t5571 * t8 - t778) * t8 - t783) * t8 - t5240) *
     # t8 - t5244) * t8 - (t5244 - (t5242 - (t2368 - (t2366 - t4 * t5582
     # * t8) * t8) * t8) * t8) * t8) + t26 * (((t4 * t5599 * t8 - t5230)
     # * t8 - t5238) * t8 - (t5238 - (t5236 - t4 * t5609 * t8) * t8) * t
     #8) / 0.576E3 + 0.3E1 / 0.640E3 * t26 * (t4 * ((t5599 * t8 - t5620)
     # * t8 - t5625) * t8 - t4 * (t5625 - (t5623 - t5609 * t8) * t8) * t
     #8) - dy * t5213 / 0.24E2 - dy * t5207 / 0.24E2 + t862 + t870
        t5646 = t621 / 0.2E1
        t5651 = t194 ** 2
        t5653 = ut(i,j,t5529,n) - t5172
        t5657 = (t5653 * t185 - t5174) * t185 - t5176
        t5663 = t5185 * t185
        t5670 = dz * (t620 / 0.2E1 + t5646 - t194 * (t5178 / 0.2E1 + t51
     #79 / 0.2E1) / 0.6E1 + t5651 * (((t5657 * t185 - t5178) * t185 - t5
     #181) * t185 / 0.2E1 + t5663 / 0.2E1) / 0.30E2) / 0.2E1
        t5671 = t1040 + t1048 + t643 - t5313 - t5340 - t5355
        t5674 = dt * dz
        t5678 = t1355 - t196
        t5680 = t5250 * t8
        t5683 = t5253 * t8
        t5685 = (t5680 - t5683) * t8
        t5689 = t1803 - t2713
        t5715 = t3710 - t3171
        t5717 = t5258 * t130
        t5720 = t5261 * t130
        t5722 = (t5717 - t5720) * t130
        t5726 = t4024 - t4766
        t5752 = t5257 + t5265 - t194 * (t5539 + t5561) / 0.24E2 - t25 * 
     #((t4 * ((t5678 * t8 - t5680) * t8 - t5685) * t8 - t4 * (t5685 - (t
     #5683 - t5689 * t8) * t8) * t8) * t8 + (((t4 * t5678 * t8 - t5252) 
     #* t8 - t5257) * t8 - (t5257 - (t5255 - t4 * t5689 * t8) * t8) * t8
     #) * t8) / 0.24E2 - t139 * ((t4 * ((t5715 * t130 - t5717) * t130 - 
     #t5722) * t130 - t4 * (t5722 - (t5720 - t5726 * t130) * t130) * t13
     #0) * t130 + (((t4 * t5715 * t130 - t5260) * t130 - t5265) * t130 -
     # (t5265 - (t5263 - t4 * t5726 * t130) * t130) * t130) * t130) / 0.
     #24E2 + t1663 + t5217 + t5220 + t5247 - t320 - t862 - t870
        t5753 = t5752 * t185
        t5755 = t5249 / 0.2E1
        t5777 = ((((t4 * (t1140 - t1614) * t8 - t4 * (t1614 - t2526) * t
     #8) * t8 + (t4 * (t3533 - t1614) * t130 - t4 * (t1614 - t4581) * t1
     #30) * t130 + t5557 - t5257 - t5265 - t1663) * t185 - t5267) * t185
     # - t5270) * t185
        t5778 = t5274 * t185
        t5783 = t5753 / 0.2E1 + t5755 - t194 * (t5777 / 0.2E1 + t5778 / 
     #0.2E1) / 0.6E1
        t5790 = t194 * (t623 - dz * t5180 / 0.12E2) / 0.12E2
        t5791 = t3830 - t3296
        t5793 = t5366 * t130
        t5796 = t5369 * t130
        t5798 = (t5793 - t5796) * t130
        t5802 = t4120 - t4933
        t5828 = t1437 - t427
        t5830 = t5358 * t8
        t5833 = t5361 * t8
        t5835 = (t5830 - t5833) * t8
        t5839 = t1937 - t2886
        t5872 = (t4 * t5653 * t185 - t5346) * t185
        t5880 = -t139 * ((t4 * ((t5791 * t130 - t5793) * t130 - t5798) *
     # t130 - t4 * (t5798 - (t5796 - t5802 * t130) * t130) * t130) * t13
     #0 + (((t4 * t5791 * t130 - t5368) * t130 - t5373) * t130 - (t5373 
     #- (t5371 - t4 * t5802 * t130) * t130) * t130) * t130) / 0.24E2 - t
     #25 * ((t4 * ((t5828 * t8 - t5830) * t8 - t5835) * t8 - t4 * (t5835
     # - (t5833 - t5839 * t8) * t8) * t8) * t8 + (((t4 * t5828 * t8 - t5
     #360) * t8 - t5365) * t8 - (t5365 - (t5363 - t4 * t5839 * t8) * t8)
     # * t8) * t8) / 0.24E2 - t194 * ((t4 * t5657 * t185 - t5342) * t185
     # + ((t5872 - t5348) * t185 - t5350) * t185) / 0.24E2 + t5348 + t53
     #65 + t5373 - t1040 - t1048 - t643 + t5313 + t5340 + t5355
        t5883 = t5357 / 0.2E1
        t5910 = t5382 * t185
        t5915 = t5880 * t185 / 0.2E1 + t5883 - t194 * (((((t4 * (ut(t5,j
     #,t1139,n) - t5172) * t8 - t4 * (t5172 - ut(t16,j,t1139,n)) * t8) *
     # t8 + (t4 * (ut(i,t126,t1139,n) - t5172) * t130 - t4 * (t5172 - ut
     #(i,t132,t1139,n)) * t130) * t130 + t5872 - t5365 - t5373 - t5348) 
     #* t185 - t5375) * t185 - t5378) * t185 / 0.2E1 + t5910 / 0.2E1) / 
     #0.6E1
        t5920 = t5777 - t5778
        t5923 = (t5753 - t5249) * t185 - dz * t5920 / 0.12E2
        t5929 = t1138 * t5180 / 0.720E3
        t5932 = t608 + dt * t5642 / 0.2E1 - t5670 + t95 * t5671 / 0.8E1 
     #- t5674 * t5783 / 0.4E1 + t5790 - t5421 * t5915 / 0.16E2 + t5388 *
     # t5923 / 0.24E2 + t5421 * t5377 / 0.96E2 - t5929 - t5455 * t5920 /
     # 0.1440E4
        t5933 = t624 / 0.2E1
        t5938 = ut(i,j,t1152,n)
        t5939 = t630 - t5938
        t5940 = t5939 * t185
        t5942 = (t632 - t5940) * t185
        t5943 = t634 - t5942
        t5944 = t5943 * t185
        t5945 = t5182 - t5944
        t5946 = t5945 * t185
        t5947 = t5184 - t5946
        t5948 = t5947 * t185
        t5955 = dz * (t5646 + t5933 - t194 * (t5179 / 0.2E1 + t5182 / 0.
     #2E1) / 0.6E1 + t5651 * (t5663 / 0.2E1 + t5948 / 0.2E1) / 0.30E2) /
     # 0.2E1
        t5956 = t3414 * t130
        t5957 = t879 * t130
        t5959 = (t5956 - t5957) * t130
        t5960 = t882 * t130
        t5962 = (t5957 - t5960) * t130
        t5963 = t5959 - t5962
        t5965 = t4 * t5963 * t130
        t5966 = t4473 * t130
        t5968 = (t5960 - t5966) * t130
        t5969 = t5962 - t5968
        t5971 = t4 * t5969 * t130
        t5972 = t5965 - t5971
        t5973 = t5972 * t130
        t5975 = (t3418 - t886) * t130
        t5977 = (t886 - t4477) * t130
        t5978 = t5975 - t5977
        t5979 = t5978 * t130
        t5982 = t139 * (t5973 + t5979) / 0.24E2
        t5983 = t795 * t8
        t5984 = t798 * t8
        t5986 = (t5983 - t5984) * t8
        t5987 = t874 * t8
        t5989 = (t5984 - t5987) * t8
        t5990 = t5986 - t5989
        t5992 = t4 * t5990 * t8
        t5993 = t2380 * t8
        t5995 = (t5987 - t5993) * t8
        t5996 = t5989 - t5995
        t5998 = t4 * t5996 * t8
        t5999 = t5992 - t5998
        t6000 = t5999 * t8
        t6002 = (t802 - t878) * t8
        t6004 = (t878 - t2384) * t8
        t6005 = t6002 - t6004
        t6006 = t6005 * t8
        t6009 = t25 * (t6000 + t6006) / 0.24E2
        t6012 = t194 * (t1635 + t1677) / 0.24E2
        t6013 = t118 + t245 - t284 + t294 - t333 - t348 + t5982 + t6009 
     #+ t6012 - t326 - t878 - t886
        t6014 = t6013 * t185
        t6015 = t6014 / 0.2E1
        t6016 = t209 - t307
        t6018 = t4 * t6016 * t8
        t6019 = t307 - t1815
        t6021 = t4 * t6019 * t8
        t6023 = (t6018 - t6021) * t8
        t6024 = t3183 - t307
        t6026 = t4 * t6024 * t130
        t6027 = t307 - t4036
        t6029 = t4 * t6027 * t130
        t6031 = (t6026 - t6029) * t130
        t6032 = t878 + t886 + t326 - t6023 - t6031 - t1673
        t6033 = t6032 * t185
        t6035 = (t5271 - t6033) * t185
        t6036 = t5273 - t6035
        t6037 = t6036 * t185
        t6042 = t5755 + t6015 - t194 * (t5778 / 0.2E1 + t6037 / 0.2E1) /
     # 0.6E1
        t6044 = t5674 * t6042 / 0.4E1
        t6049 = t194 * (t626 - dz * t5183 / 0.12E2) / 0.12E2
        t6051 = t4 * t5943 * t185
        t6055 = t4 * t5939 * t185
        t6057 = (t647 - t6055) * t185
        t6059 = (t649 - t6057) * t185
        t6061 = (t651 - t6059) * t185
        t6064 = t194 * ((t637 - t6051) * t185 + t6061) / 0.24E2
        t6066 = t1057 * t130
        t6069 = t1060 * t130
        t6071 = (t6066 - t6069) * t130
        t6091 = t139 * ((t4 * ((t3470 * t130 - t6066) * t130 - t6071) * 
     #t130 - t4 * (t6071 - (t6069 - t4528 * t130) * t130) * t130) * t130
     # + ((t3474 - t1064) * t130 - (t1064 - t4532) * t130) * t130) / 0.2
     #4E2
        t6093 = t976 * t8
        t6096 = t1052 * t8
        t6098 = (t6093 - t6096) * t8
        t6118 = t25 * ((t4 * ((t973 * t8 - t6093) * t8 - t6098) * t8 - t
     #4 * (t6098 - (t6096 - t2465 * t8) * t8) * t8) * t8 + ((t980 - t105
     #6) * t8 - (t1056 - t2469) * t8) * t8) / 0.24E2
        t6119 = t487 - t558 + t568 - t607 + t617 - t656 - t649 - t1056 -
     # t1064 + t6064 + t6091 + t6118
        t6120 = t6119 * t185
        t6121 = t6120 / 0.2E1
        t6122 = t439 - t630
        t6124 = t4 * t6122 * t8
        t6125 = t630 - t1949
        t6127 = t4 * t6125 * t8
        t6129 = (t6124 - t6127) * t8
        t6130 = t3308 - t630
        t6132 = t4 * t6130 * t130
        t6133 = t630 - t4132
        t6135 = t4 * t6133 * t130
        t6137 = (t6132 - t6135) * t130
        t6138 = t1056 + t1064 + t649 - t6129 - t6137 - t6057
        t6139 = t6138 * t185
        t6140 = t5379 - t6139
        t6141 = t6140 * t185
        t6142 = t5381 - t6141
        t6143 = t6142 * t185
        t6148 = t5883 + t6121 - t194 * (t5910 / 0.2E1 + t6143 / 0.2E1) /
     # 0.6E1
        t6150 = t5421 * t6148 / 0.16E2
        t6153 = t5778 - t6037
        t6156 = (t5249 - t6014) * t185 - dz * t6153 / 0.12E2
        t6158 = t5388 * t6156 / 0.24E2
        t6160 = t5421 * t5380 / 0.96E2
        t6162 = t1138 * t5183 / 0.720E3
        t6164 = t5455 * t6153 / 0.1440E4
        t6165 = -t2 - t1773 - t5955 - t1799 - t6044 - t6049 - t6150 - t6
     #158 - t6160 + t6162 + t6164
        t6170 = t5169 + t53 * t5188 / 0.2E1 + t96 * t5277 / 0.8E1 - t528
     #6 + t416 * t5385 / 0.48E2 - t5388 * t5394 / 0.48E2 + t725 * t5417 
     #* t185 / 0.384E3 - t5421 * t5422 / 0.192E3 + t5426 + t903 * t5447 
     #* t185 / 0.3840E4 - t5451 * t5452 / 0.2304E4 + 0.7E1 / 0.11520E5 *
     # t5455 * t5391 + cc * (t5932 + t6165) * t2068 / 0.32E2
        t6173 = dt * t5188
        t6175 = t95 * t5277
        t6178 = t415 * t5385
        t6181 = t194 * t5394
        t6185 = t724 * t5417 * t185
        t6188 = dz * t5422
        t6192 = t902 * t5447 * t185
        t6195 = dz * t5452
        t6198 = t1138 * t5391
        t6204 = dz * t5783
        t6207 = dz * t5915
        t6210 = t194 * t5923
        t6213 = dz * t5377
        t6216 = t1138 * t5920
        t6219 = t608 + t2076 * t5642 - t5670 + t2107 * t5671 / 0.2E1 - t
     #2076 * t6204 / 0.2E1 + t5790 - t2107 * t6207 / 0.4E1 + t2076 * t62
     #10 / 0.12E2 + t2107 * t6213 / 0.24E2 - t5929 - t2076 * t6216 / 0.7
     #20E3
        t6220 = dz * t6042
        t6222 = t2076 * t6220 / 0.2E1
        t6223 = dz * t6148
        t6225 = t2107 * t6223 / 0.4E1
        t6226 = t194 * t6156
        t6228 = t2076 * t6226 / 0.12E2
        t6229 = dz * t5380
        t6231 = t2107 * t6229 / 0.24E2
        t6232 = t1138 * t6153
        t6234 = t2076 * t6232 / 0.720E3
        t6235 = -t2 - t2143 - t5955 - t2145 - t6222 - t6049 - t6225 - t6
     #228 - t6231 + t6162 + t6234
        t6240 = t5169 + t2085 * t6173 + t2089 * t6175 / 0.2E1 - t5286 + 
     #t2094 * t6178 / 0.6E1 - t2076 * t6181 / 0.24E2 + t2102 * t6185 / 0
     #.24E2 - t2107 * t6188 / 0.48E2 + t5426 + t2112 * t6192 / 0.120E3 -
     # t2117 * t6195 / 0.288E3 + 0.7E1 / 0.5760E4 * t2076 * t6198 + cc *
     # (t6219 + t6235) * t2068 / 0.32E2
        t6273 = t608 + t2081 * t5642 - t5670 + t2189 * t5671 / 0.2E1 - t
     #2081 * t6204 / 0.2E1 + t5790 - t2189 * t6207 / 0.4E1 + t2081 * t62
     #10 / 0.12E2 + t2189 * t6213 / 0.24E2 - t5929 - t2081 * t6216 / 0.7
     #20E3
        t6275 = t2081 * t6220 / 0.2E1
        t6277 = t2189 * t6223 / 0.4E1
        t6279 = t2081 * t6226 / 0.12E2
        t6281 = t2189 * t6229 / 0.24E2
        t6283 = t2081 * t6232 / 0.720E3
        t6284 = -t2 - t2215 - t5955 - t2217 - t6275 - t6049 - t6277 - t6
     #279 - t6281 + t6162 + t6283
        t6289 = t5169 + t2173 * t6173 + t2176 * t6175 / 0.2E1 - t5286 + 
     #t2180 * t6178 / 0.6E1 - t2081 * t6181 / 0.24E2 + t2186 * t6185 / 0
     #.24E2 - t2189 * t6188 / 0.48E2 + t5426 + t2193 * t6192 / 0.120E3 -
     # t2196 * t6195 / 0.288E3 + 0.7E1 / 0.5760E4 * t2081 * t6198 + cc *
     # (t6273 + t6284) * t2068 / 0.32E2
        t6292 = t6170 * t2078 * t2083 + t6240 * t2168 * t2171 + t6289 * 
     #t2235 * t2238
        t6296 = t6240 * dt
        t6302 = t6170 * dt
        t6308 = t6289 * dt
        t6314 = (-t6296 / 0.2E1 - t6296 * t2080) * t2168 * t2171 + (-t63
     #02 * t2075 - t6302 * t2080) * t2078 * t2083 + (-t6308 * t2075 - t6
     #308 / 0.2E1) * t2235 * t2238
        t6335 = t4 * (t301 - dz * t312 / 0.24E2 + 0.3E1 / 0.640E3 * t113
     #8 * t1654)
        t6340 = t624 - dz * t635 / 0.24E2 + 0.3E1 / 0.640E3 * t1138 * t5
     #947
        t6345 = t6014 - dz * t6036 / 0.24E2
        t6354 = t194 * ((t294 - t333 - t326 + t6012) * t185 - dz * t1678
     # / 0.24E2) / 0.24E2
        t6357 = t6120 - dz * t6142 / 0.24E2
        t6362 = t653 - t6061
        t6365 = (t617 - t656 - t649 + t6064) * t185 - dz * t6362 / 0.24E
     #2
        t6387 = (t889 - t4 * t6032 * t185) * t185
        t6388 = t819 + t857 + t891 - (t4 * (t802 + t810 + t228 - t878 - 
     #t886 - t326) * t8 - t4 * (t878 + t886 + t326 - t2384 - t2392 - t18
     #34) * t8) * t8 - (t4 * (t3413 + t3418 + t3202 - t878 - t886 - t326
     #) * t130 - t4 * (t878 + t886 + t326 - t4472 - t4477 - t4055) * t13
     #0) * t130 - t6387
        t6392 = t891 - t6387
        t6396 = 0.7E1 / 0.5760E4 * t1138 * t1678
        t6416 = (t1067 - t4 * t6138 * t185) * t185
        t6417 = t997 + t1035 + t1069 - (t4 * (t980 + t988 + t458 - t1056
     # - t1064 - t649) * t8 - t4 * (t1056 + t1064 + t649 - t2469 - t2477
     # - t1968) * t8) * t8 - (t4 * (t3469 + t3474 + t3327 - t1056 - t106
     #4 - t649) * t130 - t4 * (t1056 + t1064 + t649 - t4527 - t4532 - t4
     #151) * t130) * t130 - t6416
        t6421 = t1069 - t6416
        t6426 = t2 + t1773 - t5955 + t1799 - t6044 + t6049 - t6150 + t61
     #58 + t6160 - t6162 - t6164
        t6429 = t3805 - t3227
        t6433 = (t6429 * t130 - t5956) * t130 - t5959
        t6440 = t4081 - t4857
        t6444 = t5968 - (t5966 - t6440 * t130) * t130
        t6455 = t5963 * t130
        t6458 = t5969 * t130
        t6460 = (t6455 - t6458) * t130
        t6500 = k - 4
        t6502 = t1626 - u(i,j,t6500,n)
        t6506 = t1630 - (t1628 - t6502 * t185) * t185
        t6510 = (t1633 - t4 * t6506 * t185) * t185
        t6520 = t1412 - t365
        t6524 = (t6520 * t8 - t5983) * t8 - t5986
        t6531 = t1898 - t2808
        t6535 = t5995 - (t5993 - t6531 * t8) * t8
        t6546 = t5990 * t8
        t6549 = t5996 * t8
        t6551 = (t6546 - t6549) * t8
        t6590 = (t1671 - t4 * t6502 * t185) * t185
        t6594 = (t1675 - (t1673 - t6590) * t185) * t185
        t6611 = t326 - dy * t5978 / 0.24E2 + t1085 * (((t4 * t6433 * t13
     #0 - t5965) * t130 - t5973) * t130 - (t5973 - (t5971 - t4 * t6444 *
     # t130) * t130) * t130) / 0.576E3 + 0.3E1 / 0.640E3 * t1085 * (t4 *
     # ((t6433 * t130 - t6455) * t130 - t6460) * t130 - t4 * (t6460 - (t
     #6458 - t6444 * t130) * t130) * t130) + 0.3E1 / 0.640E3 * t1085 * (
     #((((t4 * t6429 * t130 - t3416) * t130 - t3418) * t130 - t5975) * t
     #130 - t5979) * t130 - (t5979 - (t5977 - (t4477 - (t4475 - t4 * t64
     #40 * t130) * t130) * t130) * t130) * t130) - dz * t1634 / 0.24E2 -
     # dz * t1676 / 0.24E2 + t1138 * (t1637 - (t1635 - t6510) * t185) / 
     #0.576E3 - dx * t5999 / 0.24E2 - dx * t6005 / 0.24E2 + t26 * (((t4 
     #* t6524 * t8 - t5992) * t8 - t6000) * t8 - (t6000 - (t5998 - t4 * 
     #t6535 * t8) * t8) * t8) / 0.576E3 + 0.3E1 / 0.640E3 * t26 * (t4 * 
     #((t6524 * t8 - t6546) * t8 - t6551) * t8 - t4 * (t6551 - (t6549 - 
     #t6535 * t8) * t8) * t8) + 0.3E1 / 0.640E3 * t26 * (((((t4 * t6520 
     #* t8 - t797) * t8 - t802) * t8 - t6002) * t8 - t6006) * t8 - (t600
     #6 - (t6004 - (t2384 - (t2382 - t4 * t6531 * t8) * t8) * t8) * t8) 
     #* t8) + 0.3E1 / 0.640E3 * t1138 * (t1679 - (t1677 - t6594) * t185)
     # + 0.3E1 / 0.640E3 * t1138 * (t1656 - t4 * (t1653 - (t1651 - t6506
     # * t185) * t185) * t185) - dy * t5972 / 0.24E2 + t878 + t886
        t6620 = t5938 - ut(i,j,t6500,n)
        t6624 = t5942 - (t5940 - t6620 * t185) * t185
        t6636 = dz * (t5933 + t632 / 0.2E1 - t194 * (t5182 / 0.2E1 + t59
     #44 / 0.2E1) / 0.6E1 + t5651 * (t5948 / 0.2E1 + (t5946 - (t5944 - t
     #6624 * t185) * t185) * t185 / 0.2E1) / 0.30E2) / 0.2E1
        t6637 = t649 + t1056 + t1064 - t6064 - t6091 - t6118
        t6640 = t1367 - t209
        t6642 = t6016 * t8
        t6645 = t6019 * t8
        t6647 = (t6642 - t6645) * t8
        t6651 = t1815 - t2725
        t6677 = t3722 - t3183
        t6679 = t6024 * t130
        t6682 = t6027 * t130
        t6684 = (t6679 - t6682) * t130
        t6688 = t4036 - t4778
        t6717 = -t5982 - t6009 - t6012 + t326 + t878 + t886 - t6023 - t6
     #031 + t25 * ((t4 * ((t6640 * t8 - t6642) * t8 - t6647) * t8 - t4 *
     # (t6647 - (t6645 - t6651 * t8) * t8) * t8) * t8 + (((t4 * t6640 * 
     #t8 - t6018) * t8 - t6023) * t8 - (t6023 - (t6021 - t4 * t6651 * t8
     #) * t8) * t8) * t8) / 0.24E2 + t139 * ((t4 * ((t6677 * t130 - t667
     #9) * t130 - t6684) * t130 - t4 * (t6684 - (t6682 - t6688 * t130) *
     # t130) * t130) * t130 + (((t4 * t6677 * t130 - t6026) * t130 - t60
     #31) * t130 - (t6031 - (t6029 - t4 * t6688 * t130) * t130) * t130) 
     #* t130) / 0.24E2 + t194 * (t6510 + t6594) / 0.24E2 - t1673
        t6718 = t6717 * t185
        t6741 = (t6035 - (t6033 - (t6023 + t6031 + t1673 - (t4 * (t1153 
     #- t1626) * t8 - t4 * (t1626 - t2538) * t8) * t8 - (t4 * (t3545 - t
     #1626) * t130 - t4 * (t1626 - t4593) * t130) * t130 - t6590) * t185
     #) * t185) * t185
        t6746 = t6015 + t6718 / 0.2E1 - t194 * (t6037 / 0.2E1 + t6741 / 
     #0.2E1) / 0.6E1
        t6753 = t194 * (t634 - dz * t5945 / 0.12E2) / 0.12E2
        t6761 = (t6055 - t4 * t6620 * t185) * t185
        t6769 = t1449 - t439
        t6771 = t6122 * t8
        t6774 = t6125 * t8
        t6776 = (t6771 - t6774) * t8
        t6780 = t1949 - t2898
        t6806 = t3842 - t3308
        t6808 = t6130 * t130
        t6811 = t6133 * t130
        t6813 = (t6808 - t6811) * t130
        t6817 = t4132 - t4945
        t6843 = t649 + t1056 + t1064 - t6064 - t6091 - t6118 + t194 * ((
     #t6051 - t4 * t6624 * t185) * t185 + (t6059 - (t6057 - t6761) * t18
     #5) * t185) / 0.24E2 + t25 * ((t4 * ((t6769 * t8 - t6771) * t8 - t6
     #776) * t8 - t4 * (t6776 - (t6774 - t6780 * t8) * t8) * t8) * t8 + 
     #(((t4 * t6769 * t8 - t6124) * t8 - t6129) * t8 - (t6129 - (t6127 -
     # t4 * t6780 * t8) * t8) * t8) * t8) / 0.24E2 + t139 * ((t4 * ((t68
     #06 * t130 - t6808) * t130 - t6813) * t130 - t4 * (t6813 - (t6811 -
     # t6817 * t130) * t130) * t130) * t130 + (((t4 * t6806 * t130 - t61
     #32) * t130 - t6137) * t130 - (t6137 - (t6135 - t4 * t6817 * t130) 
     #* t130) * t130) * t130) / 0.24E2 - t6057 - t6129 - t6137
        t6876 = t6121 + t6843 * t185 / 0.2E1 - t194 * (t6143 / 0.2E1 + (
     #t6141 - (t6139 - (t6129 + t6137 + t6057 - (t4 * (ut(t5,j,t1152,n) 
     #- t5938) * t8 - t4 * (t5938 - ut(t16,j,t1152,n)) * t8) * t8 - (t4 
     #* (ut(i,t126,t1152,n) - t5938) * t130 - t4 * (t5938 - ut(i,t132,t1
     #152,n)) * t130) * t130 - t6761) * t185) * t185) * t185 / 0.2E1) / 
     #0.6E1
        t6881 = t6037 - t6741
        t6884 = (t6014 - t6718) * t185 - dz * t6881 / 0.12E2
        t6890 = t1138 * t5945 / 0.720E3
        t6893 = -t612 - dt * t6611 / 0.2E1 - t6636 - t95 * t6637 / 0.8E1
     # - t5674 * t6746 / 0.4E1 - t6753 - t5421 * t6876 / 0.16E2 - t5388 
     #* t6884 / 0.24E2 - t5421 * t6140 / 0.96E2 + t6890 + t5455 * t6881 
     #/ 0.1440E4
        t6898 = t6335 + t53 * t6340 / 0.2E1 + t96 * t6345 / 0.8E1 - t635
     #4 + t416 * t6357 / 0.48E2 - t5388 * t6365 / 0.48E2 + t725 * t6388 
     #* t185 / 0.384E3 - t5421 * t6392 / 0.192E3 + t6396 + t903 * t6417 
     #* t185 / 0.3840E4 - t5451 * t6421 / 0.2304E4 + 0.7E1 / 0.11520E5 *
     # t5455 * t6362 + cc * (t6426 + t6893) * t2068 / 0.32E2
        t6901 = dt * t6340
        t6903 = t95 * t6345
        t6906 = t415 * t6357
        t6909 = t194 * t6365
        t6913 = t724 * t6388 * t185
        t6916 = dz * t6392
        t6920 = t902 * t6417 * t185
        t6923 = dz * t6421
        t6926 = t1138 * t6362
        t6929 = t2 + t2143 - t5955 + t2145 - t6222 + t6049 - t6225 + t62
     #28 + t6231 - t6162 - t6234
        t6933 = dz * t6746
        t6936 = dz * t6876
        t6939 = t194 * t6884
        t6942 = dz * t6140
        t6945 = t1138 * t6881
        t6948 = -t612 - t2076 * t6611 - t6636 - t2107 * t6637 / 0.2E1 - 
     #t2076 * t6933 / 0.2E1 - t6753 - t2107 * t6936 / 0.4E1 - t2076 * t6
     #939 / 0.12E2 - t2107 * t6942 / 0.24E2 + t6890 + t2076 * t6945 / 0.
     #720E3
        t6953 = t6335 + t2085 * t6901 + t2089 * t6903 / 0.2E1 - t6354 + 
     #t2094 * t6906 / 0.6E1 - t2076 * t6909 / 0.24E2 + t2102 * t6913 / 0
     #.24E2 - t2107 * t6916 / 0.48E2 + t6396 + t2112 * t6920 / 0.120E3 -
     # t2117 * t6923 / 0.288E3 + 0.7E1 / 0.5760E4 * t2076 * t6926 + cc *
     # (t6929 + t6948) * t2068 / 0.32E2
        t6973 = t2 + t2215 - t5955 + t2217 - t6275 + t6049 - t6277 + t62
     #79 + t6281 - t6162 - t6283
        t6987 = -t612 - t2081 * t6611 - t6636 - t2189 * t6637 / 0.2E1 - 
     #t2081 * t6933 / 0.2E1 - t6753 - t2189 * t6936 / 0.4E1 - t2081 * t6
     #939 / 0.12E2 - t2189 * t6942 / 0.24E2 + t6890 + t2081 * t6945 / 0.
     #720E3
        t6992 = t6335 + t2173 * t6901 + t2176 * t6903 / 0.2E1 - t6354 + 
     #t2180 * t6906 / 0.6E1 - t2081 * t6909 / 0.24E2 + t2186 * t6913 / 0
     #.24E2 - t2189 * t6916 / 0.48E2 + t6396 + t2193 * t6920 / 0.120E3 -
     # t2196 * t6923 / 0.288E3 + 0.7E1 / 0.5760E4 * t2081 * t6926 + cc *
     # (t6973 + t6987) * t2068 / 0.32E2
        t6995 = t6898 * t2078 * t2083 + t6953 * t2168 * t2171 + t6992 * 
     #t2235 * t2238
        t6999 = t6953 * dt
        t7005 = t6898 * dt
        t7011 = t6992 * dt
        t7017 = (-t6999 / 0.2E1 - t6999 * t2080) * t2168 * t2171 + (-t70
     #05 * t2075 - t7005 * t2080) * t2078 * t2083 + (-t7011 * t2075 - t7
     #011 / 0.2E1) * t2235 * t2238
        t6907 = t2075 * t2080 * t2078 * t2083

        unew(i,j,k) = t1 + dt * t2 + (t2240 * t724 / 0.12E2 + t2262 *
     # t415 / 0.6E1 + (t2166 * t95 * t2268 / 0.2E1 + t2233 * t95 * t2273
     # / 0.2E1 + t2071 * t95 * t6907) * t95 / 0.2E1 - t3077 * t724 / 0.1
     #2E2 - t3099 * t415 / 0.6E1 - (t3035 * t95 * t2268 / 0.2E1 + t3074 
     #* t95 * t2273 / 0.2E1 + t2980 * t95 * t6907) * t95 / 0.2E1) * t8 +
     # (t4361 * t724 / 0.12E2 + t4383 * t415 / 0.6E1 + (t4309 * t95 * t2
     #268 / 0.2E1 + t4358 * t95 * t2273 / 0.2E1 + t4239 * t95 * t6907) *
     # t95 / 0.2E1 - t5124 * t724 / 0.12E2 - t5146 * t415 / 0.6E1 - (t50
     #82 * t95 * t2268 / 0.2E1 + t5121 * t95 * t2273 / 0.2E1 + t5027 * t
     #95 * t6907) * t95 / 0.2E1) * t130 + (t6292 * t724 / 0.12E2 + t6314
     # * t415 / 0.6E1 + (t6240 * t95 * t2268 / 0.2E1 + t6289 * t95 * t22
     #73 / 0.2E1 + t6170 * t95 * t6907) * t95 / 0.2E1 - t6995 * t724 / 0
     #.12E2 - t7017 * t415 / 0.6E1 - (t6953 * t95 * t2268 / 0.2E1 + t699
     #2 * t95 * t2273 / 0.2E1 + t6898 * t95 * t6907) * t95 / 0.2E1) * t1
     #85

        utnew(i,j,k) = 
     #t2 + (t2240 * t415 / 0.3E1 + t2262 * t95 / 0.2E1 + t2071 *
     # t415 * t6907 - t3077 * t415 / 0.3E1 - t3099 * t95 / 0.2E1 - t2980
     # * t415 * t6907 + t2166 * t415 * t2268 / 0.2E1 + t2233 * t415 * t2
     #273 / 0.2E1 - t3035 * t415 * t2268 / 0.2E1 - t3074 * t415 * t2273 
     #/ 0.2E1) * t8 + (t4361 * t415 / 0.3E1 + t4383 * t95 / 0.2E1 + t423
     #9 * t415 * t6907 - t5124 * t415 / 0.3E1 - t5146 * t95 / 0.2E1 - t5
     #027 * t415 * t6907 + t4309 * t415 * t2268 / 0.2E1 + t4358 * t415 *
     # t2273 / 0.2E1 - t5082 * t415 * t2268 / 0.2E1 - t5121 * t415 * t22
     #73 / 0.2E1) * t130 + (-t6995 * t415 / 0.3E1 - t7017 * t95 / 0.2E1 
     #- t6953 * t415 * t2268 / 0.2E1 - t6992 * t415 * t2273 / 0.2E1 - t6
     #898 * t415 * t6907 + t6292 * t415 / 0.3E1 + t6314 * t95 / 0.2E1 + 
     #t6240 * t415 * t2268 / 0.2E1 + t6289 * t415 * t2273 / 0.2E1 + t617
     #0 * t415 * t6907) * t185

c        blah = array(int(t1 + dt * t2 + (t2240 * t724 / 0.12E2 + t2262 *
c     # t415 / 0.6E1 + (t2166 * t95 * t2268 / 0.2E1 + t2233 * t95 * t2273
c     # / 0.2E1 + t2071 * t95 * t6907) * t95 / 0.2E1 - t3077 * t724 / 0.1
c     #2E2 - t3099 * t415 / 0.6E1 - (t3035 * t95 * t2268 / 0.2E1 + t3074 
c     #* t95 * t2273 / 0.2E1 + t2980 * t95 * t6907) * t95 / 0.2E1) * t8 +
c     # (t4361 * t724 / 0.12E2 + t4383 * t415 / 0.6E1 + (t4309 * t95 * t2
c     #268 / 0.2E1 + t4358 * t95 * t2273 / 0.2E1 + t4239 * t95 * t6907) *
c     # t95 / 0.2E1 - t5124 * t724 / 0.12E2 - t5146 * t415 / 0.6E1 - (t50
c     #82 * t95 * t2268 / 0.2E1 + t5121 * t95 * t2273 / 0.2E1 + t5027 * t
c     #95 * t6907) * t95 / 0.2E1) * t130 + (t6292 * t724 / 0.12E2 + t6314
c     # * t415 / 0.6E1 + (t6240 * t95 * t2268 / 0.2E1 + t6289 * t95 * t22
c     #73 / 0.2E1 + t6170 * t95 * t6907) * t95 / 0.2E1 - t6995 * t724 / 0
c     #.12E2 - t7017 * t415 / 0.6E1 - (t6953 * t95 * t2268 / 0.2E1 + t699
c     #2 * t95 * t2273 / 0.2E1 + t6898 * t95 * t6907) * t95 / 0.2E1) * t1
c     #85),int(t2 + (t2240 * t415 / 0.3E1 + t2262 * t95 / 0.2E1 + t2071 *
c     # t415 * t6907 - t3077 * t415 / 0.3E1 - t3099 * t95 / 0.2E1 - t2980
c     # * t415 * t6907 + t2166 * t415 * t2268 / 0.2E1 + t2233 * t415 * t2
c     #273 / 0.2E1 - t3035 * t415 * t2268 / 0.2E1 - t3074 * t415 * t2273 
c     #/ 0.2E1) * t8 + (t4361 * t415 / 0.3E1 + t4383 * t95 / 0.2E1 + t423
c     #9 * t415 * t6907 - t5124 * t415 / 0.3E1 - t5146 * t95 / 0.2E1 - t5
c     #027 * t415 * t6907 + t4309 * t415 * t2268 / 0.2E1 + t4358 * t415 *
c     # t2273 / 0.2E1 - t5082 * t415 * t2268 / 0.2E1 - t5121 * t415 * t22
c     #73 / 0.2E1) * t130 + (-t6995 * t415 / 0.3E1 - t7017 * t95 / 0.2E1 
c     #- t6953 * t415 * t2268 / 0.2E1 - t6992 * t415 * t2273 / 0.2E1 - t6
c     #898 * t415 * t6907 + t6292 * t415 / 0.3E1 + t6314 * t95 / 0.2E1 + 
c     #t6240 * t415 * t2268 / 0.2E1 + t6289 * t415 * t2273 / 0.2E1 + t617
c     #0 * t415 * t6907) * t185))

        return
      end
