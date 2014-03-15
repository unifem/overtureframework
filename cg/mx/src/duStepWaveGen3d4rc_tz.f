      subroutine duStepWaveGen3d4rc_tz( 
     *   nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *   n1a,n1b,n2a,n2b,n3a,n3b,
     *   ndf4a,ndf4b,nComp,
     *   u,ut,unew,utnew,
     *   src,
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
      real src  (nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,ndf4a:ndf4b,0:*)
      real dx,dy,dz,dt,cc
c
c.. generated code to follow
c
        real t1
        real t10
        real t1001
        real t1002
        real t1005
        real t1006
        real t1007
        real t1008
        real t101
        real t1011
        real t1013
        real t1015
        real t1017
        real t1018
        real t1020
        real t1023
        real t1024
        real t1025
        real t1026
        real t1027
        real t1029
        real t1030
        real t1031
        real t1033
        real t1034
        real t1036
        real t1038
        real t104
        real t1043
        real t105
        real t1050
        real t1052
        real t1054
        real t1056
        real t1058
        real t106
        real t1060
        real t1061
        real t1064
        real t1066
        real t1072
        real t1074
        real t108
        real t1082
        real t1085
        real t1089
        real t1092
        real t1095
        real t1096
        real t1098
        real t11
        real t110
        real t1101
        real t1103
        real t1107
        real t1108
        real t112
        real t1120
        real t1126
        real t113
        real t1134
        real t1135
        real t1137
        real t114
        real t1140
        real t1142
        real t1146
        real t1147
        real t1159
        real t116
        real t1165
        real t117
        integer t1173
        real t1175
        real t118
        real t1187
        real t1195
        real t1199
        real t12
        real t120
        real t1203
        real t1204
        real t1211
        real t1219
        real t122
        real t1227
        real t1228
        real t123
        real t1230
        real t1233
        real t1235
        real t1239
        real t124
        real t1240
        real t126
        real t1266
        real t1267
        real t1269
        real t127
        real t1272
        real t1274
        real t1278
        real t1279
        real t128
        real t13
        real t130
        real t1305
        real t1308
        real t1312
        real t1317
        real t1318
        real t132
        real t1322
        real t1327
        real t1328
        integer t133
        real t1330
        real t1333
        real t1341
        real t1347
        real t1351
        real t1352
        real t1356
        real t136
        real t1361
        real t1365
        real t1369
        real t137
        real t1370
        real t1374
        real t1379
        real t138
        real t1385
        real t1389
        integer t139
        real t1397
        integer t14
        real t1401
        real t1405
        real t1413
        real t142
        real t1424
        real t1428
        real t143
        real t1434
        real t1438
        real t1446
        real t1449
        real t145
        real t1453
        real t1459
        real t1463
        real t147
        real t148
        real t1481
        real t1485
        real t1488
        real t149
        real t1492
        real t1494
        real t1496
        real t1498
        real t15
        real t151
        real t1510
        real t1513
        real t1515
        real t152
        real t1521
        real t1523
        real t153
        real t1533
        real t1535
        real t1537
        real t1540
        real t1542
        real t1543
        real t1545
        real t1547
        real t1548
        real t1549
        real t155
        real t1551
        real t1552
        real t1553
        real t1555
        real t1557
        real t1560
        real t1561
        real t1564
        real t1565
        real t1566
        real t1568
        real t157
        real t1571
        real t1574
        real t1575
        real t1577
        real t158
        real t1580
        real t1582
        real t1586
        real t1587
        real t159
        real t1599
        real t16
        real t1605
        real t161
        real t1614
        real t1617
        real t1619
        real t162
        real t163
        integer t1640
        real t1642
        real t165
        real t1654
        real t1662
        real t1665
        real t1667
        real t167
        real t1671
        real t1672
        real t1673
        real t1680
        real t1681
        real t1682
        real t1684
        real t1687
        real t1689
        real t1693
        real t1694
        real t17
        real t170
        real t171
        real t1720
        real t1722
        real t1725
        real t1727
        real t1731
        real t174
        real t175
        real t176
        real t1764
        real t1772
        real t178
        real t1782
        real t1783
        real t1787
        real t1792
        real t1793
        real t1795
        real t1796
        real t1799
        real t1807
        real t181
        real t182
        real t1828
        real t1829
        real t183
        real t1833
        real t1844
        real t1845
        real t1849
        real t186
        real t1868
        integer t187
        real t1872
        real t188
        real t1880
        real t189
        real t1891
        real t1895
        real t19
        real t191
        real t1911
        real t1914
        real t1918
        real t1921
        real t1922
        real t1929
        real t1930
        real t1931
        real t1934
        real t1936
        real t194
        real t1942
        real t1943
        real t1945
        real t1946
        real t1948
        real t1950
        real t1951
        real t1952
        real t1954
        real t1955
        real t1956
        real t1958
        real t196
        real t1960
        real t1963
        real t1964
        real t1967
        real t1968
        real t1969
        real t1970
        real t1973
        real t1975
        real t1977
        real t1979
        real t1980
        real t1984
        real t1986
        real t1988
        real t1990
        real t2
        integer t20
        integer t200
        real t2003
        real t2005
        real t2007
        real t2008
        real t201
        real t2011
        real t2013
        real t2019
        real t202
        real t2021
        real t2029
        real t2032
        real t2036
        real t2039
        real t2043
        real t2046
        real t2048
        real t2069
        real t2070
        real t2072
        real t2075
        real t2077
        real t2081
        real t2082
        real t2094
        real t21
        real t2100
        integer t2108
        real t2110
        real t2122
        real t2130
        real t2134
        real t2138
        real t2139
        real t214
        real t2146
        real t2147
        real t2149
        real t2152
        real t2154
        real t2158
        real t2191
        real t2199
        real t22
        real t220
        real t2200
        real t2202
        real t2205
        real t2207
        real t2211
        real t2212
        real t2238
        real t2248
        real t2249
        real t2253
        real t2258
        real t2259
        real t2261
        real t2264
        real t2272
        real t228
        integer t229
        real t2293
        real t2294
        real t2298
        real t23
        real t230
        real t2309
        real t231
        real t2310
        real t2314
        real t233
        real t2333
        real t2337
        real t2345
        real t2356
        real t236
        real t2360
        real t2378
        real t238
        real t2382
        real t2385
        real t2389
        real t2391
        real t2393
        real t2395
        real t2407
        real t2410
        real t2412
        real t2418
        integer t242
        real t2420
        real t243
        real t2430
        real t2432
        real t2434
        real t2437
        real t2439
        real t244
        real t2440
        real t2442
        real t2444
        real t2445
        real t2447
        real t2448
        real t2450
        real t2452
        real t2455
        real t2456
        real t2459
        real t2460
        real t2461
        real t2463
        real t2466
        real t2470
        real t2473
        real t2475
        integer t2496
        real t2498
        real t25
        real t2510
        real t2519
        real t2522
        real t2524
        real t2545
        real t2548
        real t2550
        real t2554
        real t2555
        real t2556
        real t256
        real t2563
        real t2564
        real t2566
        real t2569
        real t2571
        real t2575
        real t26
        real t2601
        real t2603
        real t2606
        real t2608
        real t2612
        real t262
        real t2645
        real t2653
        real t2663
        real t2671
        real t2672
        real t2674
        real t2675
        real t2678
        real t2686
        real t270
        integer t271
        real t273
        real t2731
        real t275
        real t2760
        real t2763
        real t2767
        real t2770
        real t2771
        real t2778
        real t2779
        real t278
        real t2780
        real t2783
        real t2785
        real t2791
        real t2792
        real t2794
        real t2795
        real t2797
        real t2799
        real t280
        real t2800
        real t2802
        real t2803
        real t2805
        real t2807
        real t2810
        real t2811
        real t2814
        real t2815
        real t2816
        real t2817
        real t2820
        real t2822
        real t2824
        real t2826
        real t2827
        real t2831
        real t2833
        real t2835
        real t2837
        real t284
        real t2850
        real t2852
        real t2854
        real t2855
        real t2858
        real t286
        real t2860
        real t2866
        real t2868
        real t287
        real t2876
        real t2879
        real t2883
        real t2886
        real t289
        real t2890
        real t2893
        real t2895
        integer t2916
        real t2918
        real t2930
        real t2939
        real t2942
        real t2944
        real t295
        real t2965
        real t2969
        real t2973
        real t2974
        real t298
        real t2981
        real t2989
        real t299
        real t2997
        real t2999
        real t30
        real t3002
        real t3004
        real t3008
        real t3034
        real t3036
        real t3039
        real t3041
        real t3045
        real t305
        real t3071
        real t308
        real t3081
        real t3089
        real t3090
        real t3092
        real t3095
        real t310
        real t3103
        real t314
        real t3148
        real t315
        real t316
        real t3179
        real t3183
        real t3186
        real t3190
        real t3192
        real t3194
        real t3196
        real t32
        real t3208
        real t3211
        real t3213
        real t3219
        real t3221
        real t323
        real t3231
        real t3233
        real t324
        real t325
        real t326
        real t328
        real t33
        real t331
        real t333
        real t337
        real t338
        real t34
        real t35
        real t36
        real t367
        real t37
        real t373
        real t376
        real t382
        real t383
        real t385
        real t388
        real t39
        real t390
        real t394
        real t395
        real t4
        real t40
        real t42
        real t421
        real t424
        real t428
        real t433
        real t434
        real t438
        real t44
        real t443
        real t444
        real t446
        real t447
        integer t45
        real t450
        real t458
        real t459
        real t46
        real t465
        real t468
        real t47
        real t471
        real t473
        real t475
        real t476
        real t480
        real t485
        real t489
        real t49
        real t492
        real t494
        real t496
        real t497
        real t5
        real t50
        real t501
        real t506
        integer t51
        real t512
        real t515
        real t517
        real t519
        real t52
        real t527
        real t53
        real t531
        real t534
        real t536
        real t538
        real t546
        real t55
        real t557
        real t560
        real t564
        real t57
        real t570
        real t574
        integer t58
        real t582
        real t585
        real t589
        real t59
        real t595
        real t599
        real t6
        real t60
        real t615
        real t618
        real t62
        real t622
        integer t625
        real t626
        real t627
        real t628
        real t63
        real t630
        real t631
        real t633
        real t637
        real t639
        integer t64
        real t640
        real t641
        real t647
        real t648
        real t649
        real t65
        real t650
        real t652
        real t653
        real t655
        real t656
        real t658
        real t659
        real t66
        real t660
        real t661
        real t663
        real t664
        real t666
        real t670
        real t672
        real t673
        real t674
        real t676
        real t678
        real t679
        real t68
        real t680
        real t686
        real t687
        real t688
        real t689
        real t691
        real t692
        real t694
        real t695
        real t697
        real t698
        real t699
        real t7
        real t70
        real t700
        real t702
        real t703
        real t705
        real t709
        real t71
        real t711
        real t712
        real t713
        real t715
        real t717
        real t718
        real t719
        real t72
        real t725
        real t726
        real t727
        real t728
        real t729
        real t73
        real t730
        real t732
        real t733
        real t734
        real t741
        real t743
        real t747
        real t749
        real t75
        real t750
        real t751
        real t757
        real t758
        real t759
        real t760
        real t762
        real t763
        real t765
        real t766
        real t768
        real t769
        real t77
        real t770
        real t771
        real t773
        real t774
        real t776
        real t78
        real t780
        real t782
        real t783
        real t784
        real t786
        real t788
        real t789
        real t79
        real t790
        real t796
        real t797
        real t798
        real t799
        real t8
        real t801
        real t802
        real t804
        real t805
        real t807
        real t808
        real t809
        real t81
        real t810
        real t812
        real t813
        real t815
        real t819
        real t82
        real t821
        real t822
        real t823
        real t825
        real t827
        real t828
        real t829
        real t83
        real t835
        real t837
        real t838
        real t839
        real t841
        real t842
        real t843
        real t845
        real t847
        real t848
        real t849
        real t85
        real t851
        real t852
        real t853
        real t855
        real t857
        real t858
        real t859
        real t860
        real t863
        real t865
        real t87
        real t871
        real t874
        real t877
        real t879
        real t88
        real t881
        real t882
        real t883
        real t885
        real t886
        real t887
        real t889
        real t89
        real t891
        real t894
        real t895
        real t897
        real t899
        integer t9
        real t900
        real t901
        real t903
        real t904
        real t905
        real t907
        real t909
        real t91
        real t912
        real t915
        real t917
        real t919
        real t92
        real t920
        real t922
        real t923
        real t925
        real t927
        real t93
        real t930
        real t931
        real t933
        real t935
        real t936
        real t938
        real t939
        real t941
        real t943
        real t946
        real t95
        real t951
        real t954
        real t957
        real t958
        real t961
        real t964
        real t967
        real t968
        real t97
        real t971
        real t976
        real t978
        real t979
        real t98
        real t980
        real t982
        real t983
        real t984
        real t986
        real t988
        real t989
        real t99
        real t990
        real t992
        real t993
        real t994
        real t996
        real t998
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
        t71 = src(t9,j,k,nComp,n)
        t72 = u(t20,j,k,n)
        t73 = t1 - t72
        t75 = t4 * t73 * t12
        t77 = (t42 - t75) * t12
        t78 = u(i,t45,k,n)
        t79 = t78 - t1
        t81 = t4 * t79 * t49
        t82 = u(i,t51,k,n)
        t83 = t1 - t82
        t85 = t4 * t83 * t49
        t87 = (t81 - t85) * t49
        t88 = u(i,j,t58,n)
        t89 = t88 - t1
        t91 = t4 * t89 * t62
        t92 = u(i,j,t64,n)
        t93 = t1 - t92
        t95 = t4 * t93 * t62
        t97 = (t91 - t95) * t62
        t98 = src(i,j,k,nComp,n)
        t99 = t44 + t57 + t70 + t71 - t77 - t87 - t97 - t98
        t101 = t34 * t99 * t12
        t104 = t32 * t7
        t105 = t4 * t104
        t106 = t34 * dt
        t108 = t4 * t16 * t12
        t110 = t4 * t11 * t12
        t112 = (t108 - t110) * t12
        t113 = ut(t9,t45,k,n)
        t114 = t113 - t10
        t116 = t4 * t114 * t49
        t117 = ut(t9,t51,k,n)
        t118 = t10 - t117
        t120 = t4 * t118 * t49
        t122 = (t116 - t120) * t49
        t123 = ut(t9,j,t58,n)
        t124 = t123 - t10
        t126 = t4 * t124 * t62
        t127 = ut(t9,j,t64,n)
        t128 = t10 - t127
        t130 = t4 * t128 * t62
        t132 = (t126 - t130) * t62
        t133 = n + 1
        t136 = 0.1E1 / dt
        t137 = (src(t9,j,k,nComp,t133) - t71) * t136
        t138 = t137 / 0.2E1
        t139 = n - 1
        t142 = (t71 - src(t9,j,k,nComp,t139)) * t136
        t143 = t142 / 0.2E1
        t145 = t4 * t22 * t12
        t147 = (t110 - t145) * t12
        t148 = ut(i,t45,k,n)
        t149 = t148 - t2
        t151 = t4 * t149 * t49
        t152 = ut(i,t51,k,n)
        t153 = t2 - t152
        t155 = t4 * t153 * t49
        t157 = (t151 - t155) * t49
        t158 = ut(i,j,t58,n)
        t159 = t158 - t2
        t161 = t4 * t159 * t62
        t162 = ut(i,j,t64,n)
        t163 = t2 - t162
        t165 = t4 * t163 * t62
        t167 = (t161 - t165) * t62
        t170 = (src(i,j,k,nComp,t133) - t98) * t136
        t171 = t170 / 0.2E1
        t174 = (t98 - src(i,j,k,nComp,t139)) * t136
        t175 = t174 / 0.2E1
        t176 = t112 + t122 + t132 + t138 + t143 - t147 - t157 - t167 - t
     #171 - t175
        t178 = t106 * t176 * t12
        t181 = t7 * dt
        t182 = t112 - t147
        t183 = dx * t182
        t186 = dz ** 2
        t187 = k + 2
        t188 = u(t9,j,t187,n)
        t189 = t188 - t59
        t191 = t60 * t62
        t194 = t66 * t62
        t196 = (t191 - t194) * t62
        t200 = k - 2
        t201 = u(t9,j,t200,n)
        t202 = t65 - t201
        t214 = (t4 * t189 * t62 - t63) * t62
        t220 = (t68 - t4 * t202 * t62) * t62
        t228 = dy ** 2
        t229 = j + 2
        t230 = u(t9,t229,k,n)
        t231 = t230 - t46
        t233 = t47 * t49
        t236 = t53 * t49
        t238 = (t233 - t236) * t49
        t242 = j - 2
        t243 = u(t9,t242,k,n)
        t244 = t52 - t243
        t256 = (t4 * t231 * t49 - t50) * t49
        t262 = (t55 - t4 * t244 * t49) * t49
        t270 = dx ** 2
        t271 = i + 3
        t273 = u(t271,j,k,n) - t35
        t275 = t37 * t12
        t278 = t40 * t12
        t280 = (t275 - t278) * t12
        t284 = t73 * t12
        t286 = (t278 - t284) * t12
        t287 = t280 - t286
        t289 = t4 * t287 * t12
        t295 = (t4 * t273 * t12 - t39) * t12
        t298 = t44 - t77
        t299 = t298 * t12
        t305 = t44 + t57 + t70 - t186 * ((t4 * ((t189 * t62 - t191) * t6
     #2 - t196) * t62 - t4 * (t196 - (t194 - t202 * t62) * t62) * t62) *
     # t62 + ((t214 - t70) * t62 - (t70 - t220) * t62) * t62) / 0.24E2 -
     # t228 * ((t4 * ((t231 * t49 - t233) * t49 - t238) * t49 - t4 * (t2
     #38 - (t236 - t244 * t49) * t49) * t49) * t49 + ((t256 - t57) * t49
     # - (t57 - t262) * t49) * t49) / 0.24E2 - t270 * ((t4 * ((t273 * t1
     #2 - t275) * t12 - t280) * t12 - t289) * t12 + ((t295 - t44) * t12 
     #- t299) * t12) / 0.24E2 + t71
        t308 = t13 / 0.2E1
        t310 = ut(t271,j,k,n) - t15
        t314 = (t310 * t12 - t17) * t12 - t19
        t315 = t314 * t12
        t316 = t26 * t12
        t323 = dx * (t17 / 0.2E1 + t308 - t270 * (t315 / 0.2E1 + t316 / 
     #0.2E1) / 0.6E1) / 0.2E1
        t324 = t32 * t34
        t325 = ut(t9,j,t187,n)
        t326 = t325 - t123
        t328 = t124 * t62
        t331 = t128 * t62
        t333 = (t328 - t331) * t62
        t337 = ut(t9,j,t200,n)
        t338 = t127 - t337
        t367 = t4 * t26 * t12
        t373 = (t4 * t310 * t12 - t108) * t12
        t376 = t182 * t12
        t382 = ut(t9,t229,k,n)
        t383 = t382 - t113
        t385 = t114 * t49
        t388 = t118 * t49
        t390 = (t385 - t388) * t49
        t394 = ut(t9,t242,k,n)
        t395 = t117 - t394
        t421 = t112 + t122 + t132 - t186 * ((t4 * ((t326 * t62 - t328) *
     # t62 - t333) * t62 - t4 * (t333 - (t331 - t338 * t62) * t62) * t62
     #) * t62 + (((t4 * t326 * t62 - t126) * t62 - t132) * t62 - (t132 -
     # (t130 - t4 * t338 * t62) * t62) * t62) * t62) / 0.24E2 - t270 * (
     #(t4 * t314 * t12 - t367) * t12 + ((t373 - t112) * t12 - t376) * t1
     #2) / 0.24E2 - t228 * ((t4 * ((t383 * t49 - t385) * t49 - t390) * t
     #49 - t4 * (t390 - (t388 - t395 * t49) * t49) * t49) * t49 + (((t4 
     #* t383 * t49 - t116) * t49 - t122) * t49 - (t122 - (t120 - t4 * t3
     #95 * t49) * t49) * t49) * t49) / 0.24E2 + t138 + t143
        t424 = u(t14,t45,k,n)
        t428 = u(t14,t51,k,n)
        t433 = (t4 * (t424 - t35) * t49 - t4 * (t35 - t428) * t49) * t49
        t434 = u(t14,j,t58,n)
        t438 = u(t14,j,t64,n)
        t443 = (t4 * (t434 - t35) * t62 - t4 * (t35 - t438) * t62) * t62
        t444 = src(t14,j,k,nComp,n)
        t446 = (t295 + t433 + t443 + t444 - t44 - t57 - t70 - t71) * t12
        t447 = t99 * t12
        t450 = dx * (t446 / 0.2E1 + t447 / 0.2E1)
        t458 = t270 * (t19 - dx * (t315 - t316) / 0.12E2) / 0.12E2
        t459 = t104 * t106
        t465 = t4 * (t44 + t57 + t70 - t77 - t87 - t97) * t12
        t468 = t424 - t46
        t471 = t46 - t78
        t473 = t4 * t471 * t12
        t475 = (t4 * t468 * t12 - t473) * t12
        t476 = u(t9,t45,t58,n)
        t480 = u(t9,t45,t64,n)
        t485 = (t4 * (t476 - t46) * t62 - t4 * (t46 - t480) * t62) * t62
        t489 = t428 - t52
        t492 = t52 - t82
        t494 = t4 * t492 * t12
        t496 = (t4 * t489 * t12 - t494) * t12
        t497 = u(t9,t51,t58,n)
        t501 = u(t9,t51,t64,n)
        t506 = (t4 * (t497 - t52) * t62 - t4 * (t52 - t501) * t62) * t62
        t512 = t434 - t59
        t515 = t59 - t88
        t517 = t4 * t515 * t12
        t519 = (t4 * t512 * t12 - t517) * t12
        t527 = (t4 * (t476 - t59) * t49 - t4 * (t59 - t497) * t49) * t49
        t531 = t438 - t65
        t534 = t65 - t92
        t536 = t4 * t534 * t12
        t538 = (t4 * t531 * t12 - t536) * t12
        t546 = (t4 * (t480 - t65) * t49 - t4 * (t65 - t501) * t49) * t49
        t557 = t4 * (t71 - t98) * t12
        t560 = src(t9,t45,k,nComp,n)
        t564 = src(t9,t51,k,nComp,n)
        t570 = src(t9,j,t58,nComp,n)
        t574 = src(t9,j,t64,nComp,n)
        t582 = (t4 * (t295 + t433 + t443 - t44 - t57 - t70) * t12 - t465
     #) * t12 + (t4 * (t475 + t256 + t485 - t44 - t57 - t70) * t49 - t4 
     #* (t44 + t57 + t70 - t496 - t262 - t506) * t49) * t49 + (t4 * (t51
     #9 + t527 + t214 - t44 - t57 - t70) * t62 - t4 * (t44 + t57 + t70 -
     # t538 - t546 - t220) * t62) * t62 + (t4 * (t444 - t71) * t12 - t55
     #7) * t12 + (t4 * (t560 - t71) * t49 - t4 * (t71 - t564) * t49) * t
     #49 + (t4 * (t570 - t71) * t62 - t4 * (t71 - t574) * t62) * t62 + (
     #t137 - t142) * t136
        t585 = ut(t14,t45,k,n)
        t589 = ut(t14,t51,k,n)
        t595 = ut(t14,j,t58,n)
        t599 = ut(t14,j,t64,n)
        t615 = t176 * t12
        t618 = dx * ((t373 + (t4 * (t585 - t15) * t49 - t4 * (t15 - t589
     #) * t49) * t49 + (t4 * (t595 - t15) * t62 - t4 * (t15 - t599) * t6
     #2) * t62 + (src(t14,j,k,nComp,t133) - t444) * t136 / 0.2E1 + (t444
     # - src(t14,j,k,nComp,t139)) * t136 / 0.2E1 - t112 - t122 - t132 - 
     #t138 - t143) * t12 / 0.2E1 + t615 / 0.2E1)
        t622 = dx * (t446 - t447)
        t625 = i - 2
        t626 = u(t625,j,k,n)
        t627 = t72 - t626
        t628 = t627 * t12
        t630 = (t284 - t628) * t12
        t631 = t286 - t630
        t633 = t4 * t631 * t12
        t637 = t4 * t627 * t12
        t639 = (t75 - t637) * t12
        t640 = t77 - t639
        t641 = t640 * t12
        t647 = u(i,t229,k,n)
        t648 = t647 - t78
        t649 = t648 * t49
        t650 = t79 * t49
        t652 = (t649 - t650) * t49
        t653 = t83 * t49
        t655 = (t650 - t653) * t49
        t656 = t652 - t655
        t658 = t4 * t656 * t49
        t659 = u(i,t242,k,n)
        t660 = t82 - t659
        t661 = t660 * t49
        t663 = (t653 - t661) * t49
        t664 = t655 - t663
        t666 = t4 * t664 * t49
        t670 = t4 * t648 * t49
        t672 = (t670 - t81) * t49
        t673 = t672 - t87
        t674 = t673 * t49
        t676 = t4 * t660 * t49
        t678 = (t85 - t676) * t49
        t679 = t87 - t678
        t680 = t679 * t49
        t686 = u(i,j,t187,n)
        t687 = t686 - t88
        t688 = t687 * t62
        t689 = t89 * t62
        t691 = (t688 - t689) * t62
        t692 = t93 * t62
        t694 = (t689 - t692) * t62
        t695 = t691 - t694
        t697 = t4 * t695 * t62
        t698 = u(i,j,t200,n)
        t699 = t92 - t698
        t700 = t699 * t62
        t702 = (t692 - t700) * t62
        t703 = t694 - t702
        t705 = t4 * t703 * t62
        t709 = t4 * t687 * t62
        t711 = (t709 - t91) * t62
        t712 = t711 - t97
        t713 = t712 * t62
        t715 = t4 * t699 * t62
        t717 = (t95 - t715) * t62
        t718 = t97 - t717
        t719 = t718 * t62
        t725 = t77 - t270 * ((t289 - t633) * t12 + (t299 - t641) * t12) 
     #/ 0.24E2 + t87 - t228 * ((t658 - t666) * t49 + (t674 - t680) * t49
     #) / 0.24E2 + t97 - t186 * ((t697 - t705) * t62 + (t713 - t719) * t
     #62) / 0.24E2 + t98
        t726 = t181 * t725
        t727 = t23 / 0.2E1
        t728 = ut(t625,j,k,n)
        t729 = t21 - t728
        t730 = t729 * t12
        t732 = (t23 - t730) * t12
        t733 = t25 - t732
        t734 = t733 * t12
        t741 = dx * (t308 + t727 - t270 * (t316 / 0.2E1 + t734 / 0.2E1) 
     #/ 0.6E1) / 0.2E1
        t743 = t4 * t733 * t12
        t747 = t4 * t729 * t12
        t749 = (t145 - t747) * t12
        t750 = t147 - t749
        t751 = t750 * t12
        t757 = ut(i,t229,k,n)
        t758 = t757 - t148
        t759 = t758 * t49
        t760 = t149 * t49
        t762 = (t759 - t760) * t49
        t763 = t153 * t49
        t765 = (t760 - t763) * t49
        t766 = t762 - t765
        t768 = t4 * t766 * t49
        t769 = ut(i,t242,k,n)
        t770 = t152 - t769
        t771 = t770 * t49
        t773 = (t763 - t771) * t49
        t774 = t765 - t773
        t776 = t4 * t774 * t49
        t780 = t4 * t758 * t49
        t782 = (t780 - t151) * t49
        t783 = t782 - t157
        t784 = t783 * t49
        t786 = t4 * t770 * t49
        t788 = (t155 - t786) * t49
        t789 = t157 - t788
        t790 = t789 * t49
        t796 = ut(i,j,t187,n)
        t797 = t796 - t158
        t798 = t797 * t62
        t799 = t159 * t62
        t801 = (t798 - t799) * t62
        t802 = t163 * t62
        t804 = (t799 - t802) * t62
        t805 = t801 - t804
        t807 = t4 * t805 * t62
        t808 = ut(i,j,t200,n)
        t809 = t162 - t808
        t810 = t809 * t62
        t812 = (t802 - t810) * t62
        t813 = t804 - t812
        t815 = t4 * t813 * t62
        t819 = t4 * t797 * t62
        t821 = (t819 - t161) * t62
        t822 = t821 - t167
        t823 = t822 * t62
        t825 = t4 * t809 * t62
        t827 = (t165 - t825) * t62
        t828 = t167 - t827
        t829 = t828 * t62
        t835 = t147 + t157 + t167 - t270 * ((t367 - t743) * t12 + (t376 
     #- t751) * t12) / 0.24E2 - t228 * ((t768 - t776) * t49 + (t784 - t7
     #90) * t49) / 0.24E2 - t186 * ((t807 - t815) * t62 + (t823 - t829) 
     #* t62) / 0.24E2 + t171 + t175
        t837 = t324 * t835 / 0.2E1
        t838 = u(t20,t45,k,n)
        t839 = t838 - t72
        t841 = t4 * t839 * t49
        t842 = u(t20,t51,k,n)
        t843 = t72 - t842
        t845 = t4 * t843 * t49
        t847 = (t841 - t845) * t49
        t848 = u(t20,j,t58,n)
        t849 = t848 - t72
        t851 = t4 * t849 * t62
        t852 = u(t20,j,t64,n)
        t853 = t72 - t852
        t855 = t4 * t853 * t62
        t857 = (t851 - t855) * t62
        t858 = src(t20,j,k,nComp,n)
        t859 = t77 + t87 + t97 + t98 - t639 - t847 - t857 - t858
        t860 = t859 * t12
        t863 = dx * (t447 / 0.2E1 + t860 / 0.2E1)
        t865 = t181 * t863 / 0.2E1
        t871 = t270 * (t25 - dx * (t316 - t734) / 0.12E2) / 0.12E2
        t874 = t4 * (t77 + t87 + t97 - t639 - t847 - t857) * t12
        t877 = t78 - t838
        t879 = t4 * t877 * t12
        t881 = (t473 - t879) * t12
        t882 = u(i,t45,t58,n)
        t883 = t882 - t78
        t885 = t4 * t883 * t62
        t886 = u(i,t45,t64,n)
        t887 = t78 - t886
        t889 = t4 * t887 * t62
        t891 = (t885 - t889) * t62
        t894 = t4 * (t881 + t672 + t891 - t77 - t87 - t97) * t49
        t895 = t82 - t842
        t897 = t4 * t895 * t12
        t899 = (t494 - t897) * t12
        t900 = u(i,t51,t58,n)
        t901 = t900 - t82
        t903 = t4 * t901 * t62
        t904 = u(i,t51,t64,n)
        t905 = t82 - t904
        t907 = t4 * t905 * t62
        t909 = (t903 - t907) * t62
        t912 = t4 * (t77 + t87 + t97 - t899 - t678 - t909) * t49
        t915 = t88 - t848
        t917 = t4 * t915 * t12
        t919 = (t517 - t917) * t12
        t920 = t882 - t88
        t922 = t4 * t920 * t49
        t923 = t88 - t900
        t925 = t4 * t923 * t49
        t927 = (t922 - t925) * t49
        t930 = t4 * (t919 + t927 + t711 - t77 - t87 - t97) * t62
        t931 = t92 - t852
        t933 = t4 * t931 * t12
        t935 = (t536 - t933) * t12
        t936 = t886 - t92
        t938 = t4 * t936 * t49
        t939 = t92 - t904
        t941 = t4 * t939 * t49
        t943 = (t938 - t941) * t49
        t946 = t4 * (t77 + t87 + t97 - t935 - t943 - t717) * t62
        t951 = t4 * (t98 - t858) * t12
        t954 = src(i,t45,k,nComp,n)
        t957 = t4 * (t954 - t98) * t49
        t958 = src(i,t51,k,nComp,n)
        t961 = t4 * (t98 - t958) * t49
        t964 = src(i,j,t58,nComp,n)
        t967 = t4 * (t964 - t98) * t62
        t968 = src(i,j,t64,nComp,n)
        t971 = t4 * (t98 - t968) * t62
        t976 = (t465 - t874) * t12 + (t894 - t912) * t49 + (t930 - t946)
     # * t62 + (t557 - t951) * t12 + (t957 - t961) * t49 + (t967 - t971)
     # * t62 + (t170 - t174) * t136
        t978 = t459 * t976 / 0.6E1
        t979 = ut(t20,t45,k,n)
        t980 = t979 - t21
        t982 = t4 * t980 * t49
        t983 = ut(t20,t51,k,n)
        t984 = t21 - t983
        t986 = t4 * t984 * t49
        t988 = (t982 - t986) * t49
        t989 = ut(t20,j,t58,n)
        t990 = t989 - t21
        t992 = t4 * t990 * t62
        t993 = ut(t20,j,t64,n)
        t994 = t21 - t993
        t996 = t4 * t994 * t62
        t998 = (t992 - t996) * t62
        t1001 = (src(t20,j,k,nComp,t133) - t858) * t136
        t1002 = t1001 / 0.2E1
        t1005 = (t858 - src(t20,j,k,nComp,t139)) * t136
        t1006 = t1005 / 0.2E1
        t1007 = t147 + t157 + t167 + t171 + t175 - t749 - t988 - t998 - 
     #t1002 - t1006
        t1008 = t1007 * t12
        t1011 = dx * (t615 / 0.2E1 + t1008 / 0.2E1)
        t1013 = t324 * t1011 / 0.4E1
        t1015 = dx * (t447 - t860)
        t1017 = t181 * t1015 / 0.12E2
        t1018 = t10 + t181 * t305 - t323 + t324 * t421 / 0.2E1 - t181 * 
     #t450 / 0.2E1 + t458 + t459 * t582 / 0.6E1 - t324 * t618 / 0.4E1 + 
     #t181 * t622 / 0.12E2 - t2 - t726 - t741 - t837 - t865 - t871 - t97
     #8 - t1013 - t1017
        t1020 = sqrt(0.16E2)
        t1023 = 0.1E1 / 0.2E1 - t6
        t1024 = t4 * t1023
        t1025 = t1024 * t30
        t1026 = t1023 ** 2
        t1027 = t4 * t1026
        t1029 = t1027 * t101 / 0.2E1
        t1030 = t1026 * t1023
        t1031 = t4 * t1030
        t1033 = t1031 * t178 / 0.6E1
        t1034 = t1023 * dt
        t1036 = t1034 * t183 / 0.24E2
        t1038 = t1026 * t34
        t1043 = t1030 * t106
        t1050 = t1034 * t725
        t1052 = t1038 * t835 / 0.2E1
        t1054 = t1034 * t863 / 0.2E1
        t1056 = t1043 * t976 / 0.6E1
        t1058 = t1038 * t1011 / 0.4E1
        t1060 = t1034 * t1015 / 0.12E2
        t1061 = t10 + t1034 * t305 - t323 + t1038 * t421 / 0.2E1 - t1034
     # * t450 / 0.2E1 + t458 + t1043 * t582 / 0.6E1 - t1038 * t618 / 0.4
     #E1 + t1034 * t622 / 0.12E2 - t2 - t1050 - t741 - t1052 - t1054 - t
     #871 - t1056 - t1058 - t1060
        t1064 = cc * t1061 * t1020 / 0.8E1
        t1066 = (t8 * t30 + t33 * t101 / 0.2E1 + t105 * t178 / 0.6E1 - t
     #181 * t183 / 0.24E2 + cc * t1018 * t1020 / 0.8E1 - t1025 - t1029 -
     # t1033 + t1036 - t1064) * t5
        t1072 = t4 * (t278 - dx * t287 / 0.24E2)
        t1074 = dx * t298 / 0.24E2
        t1082 = dt * (t23 - dx * t733 / 0.24E2)
        t1085 = t34 * t859 * t12
        t1089 = t106 * t1007 * t12
        t1092 = dx * t750
        t1095 = u(t20,t229,k,n)
        t1096 = t1095 - t838
        t1098 = t839 * t49
        t1101 = t843 * t49
        t1103 = (t1098 - t1101) * t49
        t1107 = u(t20,t242,k,n)
        t1108 = t842 - t1107
        t1120 = (t4 * t1096 * t49 - t841) * t49
        t1126 = (t845 - t4 * t1108 * t49) * t49
        t1134 = u(t20,j,t187,n)
        t1135 = t1134 - t848
        t1137 = t849 * t62
        t1140 = t853 * t62
        t1142 = (t1137 - t1140) * t62
        t1146 = u(t20,j,t200,n)
        t1147 = t852 - t1146
        t1159 = (t4 * t1135 * t62 - t851) * t62
        t1165 = (t855 - t4 * t1147 * t62) * t62
        t1173 = i - 3
        t1175 = t626 - u(t1173,j,k,n)
        t1187 = (t637 - t4 * t1175 * t12) * t12
        t1195 = -t228 * ((t4 * ((t1096 * t49 - t1098) * t49 - t1103) * t
     #49 - t4 * (t1103 - (t1101 - t1108 * t49) * t49) * t49) * t49 + ((t
     #1120 - t847) * t49 - (t847 - t1126) * t49) * t49) / 0.24E2 - t186 
     #* ((t4 * ((t1135 * t62 - t1137) * t62 - t1142) * t62 - t4 * (t1142
     # - (t1140 - t1147 * t62) * t62) * t62) * t62 + ((t1159 - t857) * t
     #62 - (t857 - t1165) * t62) * t62) / 0.24E2 - t270 * ((t633 - t4 * 
     #(t630 - (t628 - t1175 * t12) * t12) * t12) * t12 + (t641 - (t639 -
     # t1187) * t12) * t12) / 0.24E2 + t639 + t847 + t857 + t858
        t1199 = t728 - ut(t1173,j,k,n)
        t1203 = t732 - (t730 - t1199 * t12) * t12
        t1204 = t1203 * t12
        t1211 = dx * (t727 + t730 / 0.2E1 - t270 * (t734 / 0.2E1 + t1204
     # / 0.2E1) / 0.6E1) / 0.2E1
        t1219 = (t747 - t4 * t1199 * t12) * t12
        t1227 = ut(t20,t229,k,n)
        t1228 = t1227 - t979
        t1230 = t980 * t49
        t1233 = t984 * t49
        t1235 = (t1230 - t1233) * t49
        t1239 = ut(t20,t242,k,n)
        t1240 = t983 - t1239
        t1266 = ut(t20,j,t187,n)
        t1267 = t1266 - t989
        t1269 = t990 * t62
        t1272 = t994 * t62
        t1274 = (t1269 - t1272) * t62
        t1278 = ut(t20,j,t200,n)
        t1279 = t993 - t1278
        t1305 = -t270 * ((t743 - t4 * t1203 * t12) * t12 + (t751 - (t749
     # - t1219) * t12) * t12) / 0.24E2 - t228 * ((t4 * ((t1228 * t49 - t
     #1230) * t49 - t1235) * t49 - t4 * (t1235 - (t1233 - t1240 * t49) *
     # t49) * t49) * t49 + (((t4 * t1228 * t49 - t982) * t49 - t988) * t
     #49 - (t988 - (t986 - t4 * t1240 * t49) * t49) * t49) * t49) / 0.24
     #E2 - t186 * ((t4 * ((t1267 * t62 - t1269) * t62 - t1274) * t62 - t
     #4 * (t1274 - (t1272 - t1279 * t62) * t62) * t62) * t62 + (((t4 * t
     #1267 * t62 - t992) * t62 - t998) * t62 - (t998 - (t996 - t4 * t127
     #9 * t62) * t62) * t62) * t62) / 0.24E2 + t749 + t988 + t998 + t100
     #2 + t1006
        t1308 = u(t625,t45,k,n)
        t1312 = u(t625,t51,k,n)
        t1317 = (t4 * (t1308 - t626) * t49 - t4 * (t626 - t1312) * t49) 
     #* t49
        t1318 = u(t625,j,t58,n)
        t1322 = u(t625,j,t64,n)
        t1327 = (t4 * (t1318 - t626) * t62 - t4 * (t626 - t1322) * t62) 
     #* t62
        t1328 = src(t625,j,k,nComp,n)
        t1330 = (t639 + t847 + t857 + t858 - t1187 - t1317 - t1327 - t13
     #28) * t12
        t1333 = dx * (t860 / 0.2E1 + t1330 / 0.2E1)
        t1341 = t270 * (t732 - dx * (t734 - t1204) / 0.12E2) / 0.12E2
        t1347 = t838 - t1308
        t1351 = (t879 - t4 * t1347 * t12) * t12
        t1352 = u(t20,t45,t58,n)
        t1356 = u(t20,t45,t64,n)
        t1361 = (t4 * (t1352 - t838) * t62 - t4 * (t838 - t1356) * t62) 
     #* t62
        t1365 = t842 - t1312
        t1369 = (t897 - t4 * t1365 * t12) * t12
        t1370 = u(t20,t51,t58,n)
        t1374 = u(t20,t51,t64,n)
        t1379 = (t4 * (t1370 - t842) * t62 - t4 * (t842 - t1374) * t62) 
     #* t62
        t1385 = t848 - t1318
        t1389 = (t917 - t4 * t1385 * t12) * t12
        t1397 = (t4 * (t1352 - t848) * t49 - t4 * (t848 - t1370) * t49) 
     #* t49
        t1401 = t852 - t1322
        t1405 = (t933 - t4 * t1401 * t12) * t12
        t1413 = (t4 * (t1356 - t852) * t49 - t4 * (t852 - t1374) * t49) 
     #* t49
        t1424 = src(t20,t45,k,nComp,n)
        t1428 = src(t20,t51,k,nComp,n)
        t1434 = src(t20,j,t58,nComp,n)
        t1438 = src(t20,j,t64,nComp,n)
        t1446 = (t874 - t4 * (t639 + t847 + t857 - t1187 - t1317 - t1327
     #) * t12) * t12 + (t4 * (t1351 + t1120 + t1361 - t639 - t847 - t857
     #) * t49 - t4 * (t639 + t847 + t857 - t1369 - t1126 - t1379) * t49)
     # * t49 + (t4 * (t1389 + t1397 + t1159 - t639 - t847 - t857) * t62 
     #- t4 * (t639 + t847 + t857 - t1405 - t1413 - t1165) * t62) * t62 +
     # (t951 - t4 * (t858 - t1328) * t12) * t12 + (t4 * (t1424 - t858) *
     # t49 - t4 * (t858 - t1428) * t49) * t49 + (t4 * (t1434 - t858) * t
     #62 - t4 * (t858 - t1438) * t62) * t62 + (t1001 - t1005) * t136
        t1449 = ut(t625,t45,k,n)
        t1453 = ut(t625,t51,k,n)
        t1459 = ut(t625,j,t58,n)
        t1463 = ut(t625,j,t64,n)
        t1481 = dx * (t1008 / 0.2E1 + (t749 + t988 + t998 + t1002 + t100
     #6 - t1219 - (t4 * (t1449 - t728) * t49 - t4 * (t728 - t1453) * t49
     #) * t49 - (t4 * (t1459 - t728) * t62 - t4 * (t728 - t1463) * t62) 
     #* t62 - (src(t625,j,k,nComp,t133) - t1328) * t136 / 0.2E1 - (t1328
     # - src(t625,j,k,nComp,t139)) * t136 / 0.2E1) * t12 / 0.2E1)
        t1485 = dx * (t860 - t1330)
        t1488 = t2 + t726 - t741 + t837 - t865 + t871 + t978 - t1013 + t
     #1017 - t21 - t181 * t1195 - t1211 - t324 * t1305 / 0.2E1 - t181 * 
     #t1333 / 0.2E1 - t1341 - t459 * t1446 / 0.6E1 - t324 * t1481 / 0.4E
     #1 - t181 * t1485 / 0.12E2
        t1492 = t1024 * t1082
        t1494 = t1027 * t1085 / 0.2E1
        t1496 = t1031 * t1089 / 0.6E1
        t1498 = t1034 * t1092 / 0.24E2
        t1510 = t2 + t1050 - t741 + t1052 - t1054 + t871 + t1056 - t1058
     # + t1060 - t21 - t1034 * t1195 - t1211 - t1038 * t1305 / 0.2E1 - t
     #1034 * t1333 / 0.2E1 - t1341 - t1043 * t1446 / 0.6E1 - t1038 * t14
     #81 / 0.4E1 - t1034 * t1485 / 0.12E2
        t1513 = cc * t1510 * t1020 / 0.8E1
        t1515 = (t8 * t1082 + t33 * t1085 / 0.2E1 + t105 * t1089 / 0.6E1
     # - t181 * t1092 / 0.24E2 + cc * t1488 * t1020 / 0.8E1 - t1492 - t1
     #494 - t1496 + t1498 - t1513) * t5
        t1521 = t4 * (t284 - dx * t631 / 0.24E2)
        t1523 = dx * t640 / 0.24E2
        t1533 = dt * (t760 - dy * t766 / 0.24E2)
        t1535 = t881 + t672 + t891 + t954 - t77 - t87 - t97 - t98
        t1537 = t34 * t1535 * t49
        t1540 = t113 - t148
        t1542 = t4 * t1540 * t12
        t1543 = t148 - t979
        t1545 = t4 * t1543 * t12
        t1547 = (t1542 - t1545) * t12
        t1548 = ut(i,t45,t58,n)
        t1549 = t1548 - t148
        t1551 = t4 * t1549 * t62
        t1552 = ut(i,t45,t64,n)
        t1553 = t148 - t1552
        t1555 = t4 * t1553 * t62
        t1557 = (t1551 - t1555) * t62
        t1560 = (src(i,t45,k,nComp,t133) - t954) * t136
        t1561 = t1560 / 0.2E1
        t1564 = (t954 - src(i,t45,k,nComp,t139)) * t136
        t1565 = t1564 / 0.2E1
        t1566 = t1547 + t782 + t1557 + t1561 + t1565 - t147 - t157 - t16
     #7 - t171 - t175
        t1568 = t106 * t1566 * t49
        t1571 = dy * t783
        t1574 = u(i,t45,t187,n)
        t1575 = t1574 - t882
        t1577 = t883 * t62
        t1580 = t887 * t62
        t1582 = (t1577 - t1580) * t62
        t1586 = u(i,t45,t200,n)
        t1587 = t886 - t1586
        t1599 = (t4 * t1575 * t62 - t885) * t62
        t1605 = (t889 - t4 * t1587 * t62) * t62
        t1614 = t471 * t12
        t1617 = t877 * t12
        t1619 = (t1614 - t1617) * t12
        t1640 = j + 3
        t1642 = u(i,t1640,k,n) - t647
        t1654 = (t4 * t1642 * t49 - t670) * t49
        t1662 = -t186 * ((t4 * ((t1575 * t62 - t1577) * t62 - t1582) * t
     #62 - t4 * (t1582 - (t1580 - t1587 * t62) * t62) * t62) * t62 + ((t
     #1599 - t891) * t62 - (t891 - t1605) * t62) * t62) / 0.24E2 - t270 
     #* ((t4 * ((t468 * t12 - t1614) * t12 - t1619) * t12 - t4 * (t1619 
     #- (t1617 - t1347 * t12) * t12) * t12) * t12 + ((t475 - t881) * t12
     # - (t881 - t1351) * t12) * t12) / 0.24E2 + t881 + t672 + t891 - t2
     #28 * ((t4 * ((t1642 * t49 - t649) * t49 - t652) * t49 - t658) * t4
     #9 + ((t1654 - t672) * t49 - t674) * t49) / 0.24E2 + t954
        t1665 = t760 / 0.2E1
        t1667 = ut(i,t1640,k,n) - t757
        t1671 = (t1667 * t49 - t759) * t49 - t762
        t1672 = t1671 * t49
        t1673 = t766 * t49
        t1680 = dy * (t759 / 0.2E1 + t1665 - t228 * (t1672 / 0.2E1 + t16
     #73 / 0.2E1) / 0.6E1) / 0.2E1
        t1681 = ut(i,t45,t187,n)
        t1682 = t1681 - t1548
        t1684 = t1549 * t62
        t1687 = t1553 * t62
        t1689 = (t1684 - t1687) * t62
        t1693 = ut(i,t45,t200,n)
        t1694 = t1552 - t1693
        t1720 = t585 - t113
        t1722 = t1540 * t12
        t1725 = t1543 * t12
        t1727 = (t1722 - t1725) * t12
        t1731 = t979 - t1449
        t1764 = (t4 * t1667 * t49 - t780) * t49
        t1772 = -t186 * ((t4 * ((t1682 * t62 - t1684) * t62 - t1689) * t
     #62 - t4 * (t1689 - (t1687 - t1694 * t62) * t62) * t62) * t62 + (((
     #t4 * t1682 * t62 - t1551) * t62 - t1557) * t62 - (t1557 - (t1555 -
     # t4 * t1694 * t62) * t62) * t62) * t62) / 0.24E2 - t270 * ((t4 * (
     #(t1720 * t12 - t1722) * t12 - t1727) * t12 - t4 * (t1727 - (t1725 
     #- t1731 * t12) * t12) * t12) * t12 + (((t4 * t1720 * t12 - t1542) 
     #* t12 - t1547) * t12 - (t1547 - (t1545 - t4 * t1731 * t12) * t12) 
     #* t12) * t12) / 0.24E2 - t228 * ((t4 * t1671 * t49 - t768) * t49 +
     # ((t1764 - t782) * t49 - t784) * t49) / 0.24E2 + t1547 + t1557 + t
     #782 + t1561 + t1565
        t1782 = (t4 * (t230 - t647) * t12 - t4 * (t647 - t1095) * t12) *
     # t12
        t1783 = u(i,t229,t58,n)
        t1787 = u(i,t229,t64,n)
        t1792 = (t4 * (t1783 - t647) * t62 - t4 * (t647 - t1787) * t62) 
     #* t62
        t1793 = src(i,t229,k,nComp,n)
        t1795 = (t1782 + t1654 + t1792 + t1793 - t881 - t672 - t891 - t9
     #54) * t49
        t1796 = t1535 * t49
        t1799 = dy * (t1795 / 0.2E1 + t1796 / 0.2E1)
        t1807 = t228 * (t762 - dy * (t1672 - t1673) / 0.12E2) / 0.12E2
        t1828 = (t4 * (t476 - t882) * t12 - t4 * (t882 - t1352) * t12) *
     # t12
        t1829 = t1783 - t882
        t1833 = (t4 * t1829 * t49 - t922) * t49
        t1844 = (t4 * (t480 - t886) * t12 - t4 * (t886 - t1356) * t12) *
     # t12
        t1845 = t1787 - t886
        t1849 = (t4 * t1845 * t49 - t938) * t49
        t1868 = src(i,t45,t58,nComp,n)
        t1872 = src(i,t45,t64,nComp,n)
        t1880 = (t4 * (t475 + t256 + t485 - t881 - t672 - t891) * t12 - 
     #t4 * (t881 + t672 + t891 - t1351 - t1120 - t1361) * t12) * t12 + (
     #t4 * (t1782 + t1654 + t1792 - t881 - t672 - t891) * t49 - t894) * 
     #t49 + (t4 * (t1828 + t1833 + t1599 - t881 - t672 - t891) * t62 - t
     #4 * (t881 + t672 + t891 - t1844 - t1849 - t1605) * t62) * t62 + (t
     #4 * (t560 - t954) * t12 - t4 * (t954 - t1424) * t12) * t12 + (t4 *
     # (t1793 - t954) * t49 - t957) * t49 + (t4 * (t1868 - t954) * t62 -
     # t4 * (t954 - t1872) * t62) * t62 + (t1560 - t1564) * t136
        t1891 = ut(i,t229,t58,n)
        t1895 = ut(i,t229,t64,n)
        t1911 = t1566 * t49
        t1914 = dy * (((t4 * (t382 - t757) * t12 - t4 * (t757 - t1227) *
     # t12) * t12 + t1764 + (t4 * (t1891 - t757) * t62 - t4 * (t757 - t1
     #895) * t62) * t62 + (src(i,t229,k,nComp,t133) - t1793) * t136 / 0.
     #2E1 + (t1793 - src(i,t229,k,nComp,t139)) * t136 / 0.2E1 - t1547 - 
     #t782 - t1557 - t1561 - t1565) * t49 / 0.2E1 + t1911 / 0.2E1)
        t1918 = dy * (t1795 - t1796)
        t1921 = t763 / 0.2E1
        t1922 = t774 * t49
        t1929 = dy * (t1665 + t1921 - t228 * (t1673 / 0.2E1 + t1922 / 0.
     #2E1) / 0.6E1) / 0.2E1
        t1930 = t77 + t87 + t97 + t98 - t899 - t678 - t909 - t958
        t1931 = t1930 * t49
        t1934 = dy * (t1796 / 0.2E1 + t1931 / 0.2E1)
        t1936 = t181 * t1934 / 0.2E1
        t1942 = t228 * (t765 - dy * (t1673 - t1922) / 0.12E2) / 0.12E2
        t1943 = t117 - t152
        t1945 = t4 * t1943 * t12
        t1946 = t152 - t983
        t1948 = t4 * t1946 * t12
        t1950 = (t1945 - t1948) * t12
        t1951 = ut(i,t51,t58,n)
        t1952 = t1951 - t152
        t1954 = t4 * t1952 * t62
        t1955 = ut(i,t51,t64,n)
        t1956 = t152 - t1955
        t1958 = t4 * t1956 * t62
        t1960 = (t1954 - t1958) * t62
        t1963 = (src(i,t51,k,nComp,t133) - t958) * t136
        t1964 = t1963 / 0.2E1
        t1967 = (t958 - src(i,t51,k,nComp,t139)) * t136
        t1968 = t1967 / 0.2E1
        t1969 = t147 + t157 + t167 + t171 + t175 - t1950 - t788 - t1960 
     #- t1964 - t1968
        t1970 = t1969 * t49
        t1973 = dy * (t1911 / 0.2E1 + t1970 / 0.2E1)
        t1975 = t324 * t1973 / 0.4E1
        t1977 = dy * (t1796 - t1931)
        t1979 = t181 * t1977 / 0.12E2
        t1980 = t148 + t181 * t1662 - t1680 + t324 * t1772 / 0.2E1 - t18
     #1 * t1799 / 0.2E1 + t1807 + t459 * t1880 / 0.6E1 - t324 * t1914 / 
     #0.4E1 + t181 * t1918 / 0.12E2 - t2 - t726 - t1929 - t837 - t1936 -
     # t1942 - t978 - t1975 - t1979
        t1984 = t1024 * t1533
        t1986 = t1027 * t1537 / 0.2E1
        t1988 = t1031 * t1568 / 0.6E1
        t1990 = t1034 * t1571 / 0.24E2
        t2003 = t1034 * t1934 / 0.2E1
        t2005 = t1038 * t1973 / 0.4E1
        t2007 = t1034 * t1977 / 0.12E2
        t2008 = t148 + t1034 * t1662 - t1680 + t1038 * t1772 / 0.2E1 - t
     #1034 * t1799 / 0.2E1 + t1807 + t1043 * t1880 / 0.6E1 - t1038 * t19
     #14 / 0.4E1 + t1034 * t1918 / 0.12E2 - t2 - t1050 - t1929 - t1052 -
     # t2003 - t1942 - t1056 - t2005 - t2007
        t2011 = cc * t2008 * t1020 / 0.8E1
        t2013 = (t8 * t1533 + t33 * t1537 / 0.2E1 + t105 * t1568 / 0.6E1
     # - t181 * t1571 / 0.24E2 + cc * t1980 * t1020 / 0.8E1 - t1984 - t1
     #986 - t1988 + t1990 - t2011) * t5
        t2019 = t4 * (t650 - dy * t656 / 0.24E2)
        t2021 = dy * t673 / 0.24E2
        t2029 = dt * (t763 - dy * t774 / 0.24E2)
        t2032 = t34 * t1930 * t49
        t2036 = t106 * t1969 * t49
        t2039 = dy * t789
        t2043 = t492 * t12
        t2046 = t895 * t12
        t2048 = (t2043 - t2046) * t12
        t2069 = u(i,t51,t187,n)
        t2070 = t2069 - t900
        t2072 = t901 * t62
        t2075 = t905 * t62
        t2077 = (t2072 - t2075) * t62
        t2081 = u(i,t51,t200,n)
        t2082 = t904 - t2081
        t2094 = (t4 * t2070 * t62 - t903) * t62
        t2100 = (t907 - t4 * t2082 * t62) * t62
        t2108 = j - 3
        t2110 = t659 - u(i,t2108,k,n)
        t2122 = (t676 - t4 * t2110 * t49) * t49
        t2130 = -t270 * ((t4 * ((t489 * t12 - t2043) * t12 - t2048) * t1
     #2 - t4 * (t2048 - (t2046 - t1365 * t12) * t12) * t12) * t12 + ((t4
     #96 - t899) * t12 - (t899 - t1369) * t12) * t12) / 0.24E2 - t186 * 
     #((t4 * ((t2070 * t62 - t2072) * t62 - t2077) * t62 - t4 * (t2077 -
     # (t2075 - t2082 * t62) * t62) * t62) * t62 + ((t2094 - t909) * t62
     # - (t909 - t2100) * t62) * t62) / 0.24E2 - t228 * ((t666 - t4 * (t
     #663 - (t661 - t2110 * t49) * t49) * t49) * t49 + (t680 - (t678 - t
     #2122) * t49) * t49) / 0.24E2 + t899 + t678 + t909 + t958
        t2134 = t769 - ut(i,t2108,k,n)
        t2138 = t773 - (t771 - t2134 * t49) * t49
        t2139 = t2138 * t49
        t2146 = dy * (t1921 + t771 / 0.2E1 - t228 * (t1922 / 0.2E1 + t21
     #39 / 0.2E1) / 0.6E1) / 0.2E1
        t2147 = t589 - t117
        t2149 = t1943 * t12
        t2152 = t1946 * t12
        t2154 = (t2149 - t2152) * t12
        t2158 = t983 - t1453
        t2191 = (t786 - t4 * t2134 * t49) * t49
        t2199 = ut(i,t51,t187,n)
        t2200 = t2199 - t1951
        t2202 = t1952 * t62
        t2205 = t1956 * t62
        t2207 = (t2202 - t2205) * t62
        t2211 = ut(i,t51,t200,n)
        t2212 = t1955 - t2211
        t2238 = -t270 * ((t4 * ((t2147 * t12 - t2149) * t12 - t2154) * t
     #12 - t4 * (t2154 - (t2152 - t2158 * t12) * t12) * t12) * t12 + (((
     #t4 * t2147 * t12 - t1945) * t12 - t1950) * t12 - (t1950 - (t1948 -
     # t4 * t2158 * t12) * t12) * t12) * t12) / 0.24E2 - t228 * ((t776 -
     # t4 * t2138 * t49) * t49 + (t790 - (t788 - t2191) * t49) * t49) / 
     #0.24E2 - t186 * ((t4 * ((t2200 * t62 - t2202) * t62 - t2207) * t62
     # - t4 * (t2207 - (t2205 - t2212 * t62) * t62) * t62) * t62 + (((t4
     # * t2200 * t62 - t1954) * t62 - t1960) * t62 - (t1960 - (t1958 - t
     #4 * t2212 * t62) * t62) * t62) * t62) / 0.24E2 + t1950 + t1960 + t
     #788 + t1964 + t1968
        t2248 = (t4 * (t243 - t659) * t12 - t4 * (t659 - t1107) * t12) *
     # t12
        t2249 = u(i,t242,t58,n)
        t2253 = u(i,t242,t64,n)
        t2258 = (t4 * (t2249 - t659) * t62 - t4 * (t659 - t2253) * t62) 
     #* t62
        t2259 = src(i,t242,k,nComp,n)
        t2261 = (t899 + t678 + t909 + t958 - t2248 - t2122 - t2258 - t22
     #59) * t49
        t2264 = dy * (t1931 / 0.2E1 + t2261 / 0.2E1)
        t2272 = t228 * (t773 - dy * (t1922 - t2139) / 0.12E2) / 0.12E2
        t2293 = (t4 * (t497 - t900) * t12 - t4 * (t900 - t1370) * t12) *
     # t12
        t2294 = t900 - t2249
        t2298 = (t925 - t4 * t2294 * t49) * t49
        t2309 = (t4 * (t501 - t904) * t12 - t4 * (t904 - t1374) * t12) *
     # t12
        t2310 = t904 - t2253
        t2314 = (t941 - t4 * t2310 * t49) * t49
        t2333 = src(i,t51,t58,nComp,n)
        t2337 = src(i,t51,t64,nComp,n)
        t2345 = (t4 * (t496 + t262 + t506 - t899 - t678 - t909) * t12 - 
     #t4 * (t899 + t678 + t909 - t1369 - t1126 - t1379) * t12) * t12 + (
     #t912 - t4 * (t899 + t678 + t909 - t2248 - t2122 - t2258) * t49) * 
     #t49 + (t4 * (t2293 + t2298 + t2094 - t899 - t678 - t909) * t62 - t
     #4 * (t899 + t678 + t909 - t2309 - t2314 - t2100) * t62) * t62 + (t
     #4 * (t564 - t958) * t12 - t4 * (t958 - t1428) * t12) * t12 + (t961
     # - t4 * (t958 - t2259) * t49) * t49 + (t4 * (t2333 - t958) * t62 -
     # t4 * (t958 - t2337) * t62) * t62 + (t1963 - t1967) * t136
        t2356 = ut(i,t242,t58,n)
        t2360 = ut(i,t242,t64,n)
        t2378 = dy * (t1970 / 0.2E1 + (t1950 + t788 + t1960 + t1964 + t1
     #968 - (t4 * (t394 - t769) * t12 - t4 * (t769 - t1239) * t12) * t12
     # - t2191 - (t4 * (t2356 - t769) * t62 - t4 * (t769 - t2360) * t62)
     # * t62 - (src(i,t242,k,nComp,t133) - t2259) * t136 / 0.2E1 - (t225
     #9 - src(i,t242,k,nComp,t139)) * t136 / 0.2E1) * t49 / 0.2E1)
        t2382 = dy * (t1931 - t2261)
        t2385 = t2 + t726 - t1929 + t837 - t1936 + t1942 + t978 - t1975 
     #+ t1979 - t152 - t181 * t2130 - t2146 - t324 * t2238 / 0.2E1 - t18
     #1 * t2264 / 0.2E1 - t2272 - t459 * t2345 / 0.6E1 - t324 * t2378 / 
     #0.4E1 - t181 * t2382 / 0.12E2
        t2389 = t1024 * t2029
        t2391 = t1027 * t2032 / 0.2E1
        t2393 = t1031 * t2036 / 0.6E1
        t2395 = t1034 * t2039 / 0.24E2
        t2407 = t2 + t1050 - t1929 + t1052 - t2003 + t1942 + t1056 - t20
     #05 + t2007 - t152 - t1034 * t2130 - t2146 - t1038 * t2238 / 0.2E1 
     #- t1034 * t2264 / 0.2E1 - t2272 - t1043 * t2345 / 0.6E1 - t1038 * 
     #t2378 / 0.4E1 - t1034 * t2382 / 0.12E2
        t2410 = cc * t2407 * t1020 / 0.8E1
        t2412 = (t8 * t2029 + t33 * t2032 / 0.2E1 + t105 * t2036 / 0.6E1
     # - t181 * t2039 / 0.24E2 + cc * t2385 * t1020 / 0.8E1 - t2389 - t2
     #391 - t2393 + t2395 - t2410) * t5
        t2418 = t4 * (t653 - dy * t664 / 0.24E2)
        t2420 = dy * t679 / 0.24E2
        t2430 = dt * (t799 - dz * t805 / 0.24E2)
        t2432 = t919 + t927 + t711 + t964 - t77 - t87 - t97 - t98
        t2434 = t34 * t2432 * t62
        t2437 = t123 - t158
        t2439 = t4 * t2437 * t12
        t2440 = t158 - t989
        t2442 = t4 * t2440 * t12
        t2444 = (t2439 - t2442) * t12
        t2445 = t1548 - t158
        t2447 = t4 * t2445 * t49
        t2448 = t158 - t1951
        t2450 = t4 * t2448 * t49
        t2452 = (t2447 - t2450) * t49
        t2455 = (src(i,j,t58,nComp,t133) - t964) * t136
        t2456 = t2455 / 0.2E1
        t2459 = (t964 - src(i,j,t58,nComp,t139)) * t136
        t2460 = t2459 / 0.2E1
        t2461 = t2444 + t2452 + t821 + t2456 + t2460 - t147 - t157 - t16
     #7 - t171 - t175
        t2463 = t106 * t2461 * t62
        t2466 = dz * t822
        t2470 = t515 * t12
        t2473 = t915 * t12
        t2475 = (t2470 - t2473) * t12
        t2496 = k + 3
        t2498 = u(i,j,t2496,n) - t686
        t2510 = (t4 * t2498 * t62 - t709) * t62
        t2519 = t920 * t49
        t2522 = t923 * t49
        t2524 = (t2519 - t2522) * t49
        t2545 = t919 + t927 + t711 - t270 * ((t4 * ((t512 * t12 - t2470)
     # * t12 - t2475) * t12 - t4 * (t2475 - (t2473 - t1385 * t12) * t12)
     # * t12) * t12 + ((t519 - t919) * t12 - (t919 - t1389) * t12) * t12
     #) / 0.24E2 - t186 * ((t4 * ((t2498 * t62 - t688) * t62 - t691) * t
     #62 - t697) * t62 + ((t2510 - t711) * t62 - t713) * t62) / 0.24E2 -
     # t228 * ((t4 * ((t1829 * t49 - t2519) * t49 - t2524) * t49 - t4 * 
     #(t2524 - (t2522 - t2294 * t49) * t49) * t49) * t49 + ((t1833 - t92
     #7) * t49 - (t927 - t2298) * t49) * t49) / 0.24E2 + t964
        t2548 = t799 / 0.2E1
        t2550 = ut(i,j,t2496,n) - t796
        t2554 = (t2550 * t62 - t798) * t62 - t801
        t2555 = t2554 * t62
        t2556 = t805 * t62
        t2563 = dz * (t798 / 0.2E1 + t2548 - t186 * (t2555 / 0.2E1 + t25
     #56 / 0.2E1) / 0.6E1) / 0.2E1
        t2564 = t595 - t123
        t2566 = t2437 * t12
        t2569 = t2440 * t12
        t2571 = (t2566 - t2569) * t12
        t2575 = t989 - t1459
        t2601 = t1891 - t1548
        t2603 = t2445 * t49
        t2606 = t2448 * t49
        t2608 = (t2603 - t2606) * t49
        t2612 = t1951 - t2356
        t2645 = (t4 * t2550 * t62 - t819) * t62
        t2653 = t2444 + t2452 - t270 * ((t4 * ((t2564 * t12 - t2566) * t
     #12 - t2571) * t12 - t4 * (t2571 - (t2569 - t2575 * t12) * t12) * t
     #12) * t12 + (((t4 * t2564 * t12 - t2439) * t12 - t2444) * t12 - (t
     #2444 - (t2442 - t4 * t2575 * t12) * t12) * t12) * t12) / 0.24E2 - 
     #t228 * ((t4 * ((t2601 * t49 - t2603) * t49 - t2608) * t49 - t4 * (
     #t2608 - (t2606 - t2612 * t49) * t49) * t49) * t49 + (((t4 * t2601 
     #* t49 - t2447) * t49 - t2452) * t49 - (t2452 - (t2450 - t4 * t2612
     # * t49) * t49) * t49) * t49) / 0.24E2 - t186 * ((t4 * t2554 * t62 
     #- t807) * t62 + ((t2645 - t821) * t62 - t823) * t62) / 0.24E2 + t8
     #21 + t2456 + t2460
        t2663 = (t4 * (t188 - t686) * t12 - t4 * (t686 - t1134) * t12) *
     # t12
        t2671 = (t4 * (t1574 - t686) * t49 - t4 * (t686 - t2069) * t49) 
     #* t49
        t2672 = src(i,j,t187,nComp,n)
        t2674 = (t2663 + t2671 + t2510 + t2672 - t919 - t927 - t711 - t9
     #64) * t62
        t2675 = t2432 * t62
        t2678 = dz * (t2674 / 0.2E1 + t2675 / 0.2E1)
        t2686 = t186 * (t801 - dz * (t2555 - t2556) / 0.12E2) / 0.12E2
        t2731 = (t4 * (t519 + t527 + t214 - t919 - t927 - t711) * t12 - 
     #t4 * (t919 + t927 + t711 - t1389 - t1397 - t1159) * t12) * t12 + (
     #t4 * (t1828 + t1833 + t1599 - t919 - t927 - t711) * t49 - t4 * (t9
     #19 + t927 + t711 - t2293 - t2298 - t2094) * t49) * t49 + (t4 * (t2
     #663 + t2671 + t2510 - t919 - t927 - t711) * t62 - t930) * t62 + (t
     #4 * (t570 - t964) * t12 - t4 * (t964 - t1434) * t12) * t12 + (t4 *
     # (t1868 - t964) * t49 - t4 * (t964 - t2333) * t49) * t49 + (t4 * (
     #t2672 - t964) * t62 - t967) * t62 + (t2455 - t2459) * t136
        t2760 = t2461 * t62
        t2763 = dz * (((t4 * (t325 - t796) * t12 - t4 * (t796 - t1266) *
     # t12) * t12 + (t4 * (t1681 - t796) * t49 - t4 * (t796 - t2199) * t
     #49) * t49 + t2645 + (src(i,j,t187,nComp,t133) - t2672) * t136 / 0.
     #2E1 + (t2672 - src(i,j,t187,nComp,t139)) * t136 / 0.2E1 - t2444 - 
     #t2452 - t821 - t2456 - t2460) * t62 / 0.2E1 + t2760 / 0.2E1)
        t2767 = dz * (t2674 - t2675)
        t2770 = t802 / 0.2E1
        t2771 = t813 * t62
        t2778 = dz * (t2548 + t2770 - t186 * (t2556 / 0.2E1 + t2771 / 0.
     #2E1) / 0.6E1) / 0.2E1
        t2779 = t77 + t87 + t97 + t98 - t935 - t943 - t717 - t968
        t2780 = t2779 * t62
        t2783 = dz * (t2675 / 0.2E1 + t2780 / 0.2E1)
        t2785 = t181 * t2783 / 0.2E1
        t2791 = t186 * (t804 - dz * (t2556 - t2771) / 0.12E2) / 0.12E2
        t2792 = t127 - t162
        t2794 = t4 * t2792 * t12
        t2795 = t162 - t993
        t2797 = t4 * t2795 * t12
        t2799 = (t2794 - t2797) * t12
        t2800 = t1552 - t162
        t2802 = t4 * t2800 * t49
        t2803 = t162 - t1955
        t2805 = t4 * t2803 * t49
        t2807 = (t2802 - t2805) * t49
        t2810 = (src(i,j,t64,nComp,t133) - t968) * t136
        t2811 = t2810 / 0.2E1
        t2814 = (t968 - src(i,j,t64,nComp,t139)) * t136
        t2815 = t2814 / 0.2E1
        t2816 = t147 + t157 + t167 + t171 + t175 - t2799 - t2807 - t827 
     #- t2811 - t2815
        t2817 = t2816 * t62
        t2820 = dz * (t2760 / 0.2E1 + t2817 / 0.2E1)
        t2822 = t324 * t2820 / 0.4E1
        t2824 = dz * (t2675 - t2780)
        t2826 = t181 * t2824 / 0.12E2
        t2827 = t158 + t181 * t2545 - t2563 + t324 * t2653 / 0.2E1 - t18
     #1 * t2678 / 0.2E1 + t2686 + t459 * t2731 / 0.6E1 - t324 * t2763 / 
     #0.4E1 + t181 * t2767 / 0.12E2 - t2 - t726 - t2778 - t837 - t2785 -
     # t2791 - t978 - t2822 - t2826
        t2831 = t1024 * t2430
        t2833 = t1027 * t2434 / 0.2E1
        t2835 = t1031 * t2463 / 0.6E1
        t2837 = t1034 * t2466 / 0.24E2
        t2850 = t1034 * t2783 / 0.2E1
        t2852 = t1038 * t2820 / 0.4E1
        t2854 = t1034 * t2824 / 0.12E2
        t2855 = t158 + t1034 * t2545 - t2563 + t1038 * t2653 / 0.2E1 - t
     #1034 * t2678 / 0.2E1 + t2686 + t1043 * t2731 / 0.6E1 - t1038 * t27
     #63 / 0.4E1 + t1034 * t2767 / 0.12E2 - t2 - t1050 - t2778 - t1052 -
     # t2850 - t2791 - t1056 - t2852 - t2854
        t2858 = cc * t2855 * t1020 / 0.8E1
        t2860 = (t8 * t2430 + t33 * t2434 / 0.2E1 + t105 * t2463 / 0.6E1
     # - t181 * t2466 / 0.24E2 + cc * t2827 * t1020 / 0.8E1 - t2831 - t2
     #833 - t2835 + t2837 - t2858) * t5
        t2866 = t4 * (t689 - dz * t695 / 0.24E2)
        t2868 = dz * t712 / 0.24E2
        t2876 = dt * (t802 - dz * t813 / 0.24E2)
        t2879 = t34 * t2779 * t62
        t2883 = t106 * t2816 * t62
        t2886 = dz * t828
        t2890 = t936 * t49
        t2893 = t939 * t49
        t2895 = (t2890 - t2893) * t49
        t2916 = k - 3
        t2918 = t698 - u(i,j,t2916,n)
        t2930 = (t715 - t4 * t2918 * t62) * t62
        t2939 = t534 * t12
        t2942 = t931 * t12
        t2944 = (t2939 - t2942) * t12
        t2965 = t943 - t228 * ((t4 * ((t1845 * t49 - t2890) * t49 - t289
     #5) * t49 - t4 * (t2895 - (t2893 - t2310 * t49) * t49) * t49) * t49
     # + ((t1849 - t943) * t49 - (t943 - t2314) * t49) * t49) / 0.24E2 +
     # t935 + t717 - t186 * ((t705 - t4 * (t702 - (t700 - t2918 * t62) *
     # t62) * t62) * t62 + (t719 - (t717 - t2930) * t62) * t62) / 0.24E2
     # - t270 * ((t4 * ((t531 * t12 - t2939) * t12 - t2944) * t12 - t4 *
     # (t2944 - (t2942 - t1401 * t12) * t12) * t12) * t12 + ((t538 - t93
     #5) * t12 - (t935 - t1405) * t12) * t12) / 0.24E2 + t968
        t2969 = t808 - ut(i,j,t2916,n)
        t2973 = t812 - (t810 - t2969 * t62) * t62
        t2974 = t2973 * t62
        t2981 = dz * (t2770 + t810 / 0.2E1 - t186 * (t2771 / 0.2E1 + t29
     #74 / 0.2E1) / 0.6E1) / 0.2E1
        t2989 = (t825 - t4 * t2969 * t62) * t62
        t2997 = t599 - t127
        t2999 = t2792 * t12
        t3002 = t2795 * t12
        t3004 = (t2999 - t3002) * t12
        t3008 = t993 - t1463
        t3034 = t1895 - t1552
        t3036 = t2800 * t49
        t3039 = t2803 * t49
        t3041 = (t3036 - t3039) * t49
        t3045 = t1955 - t2360
        t3071 = t2799 + t2807 + t827 - t186 * ((t815 - t4 * t2973 * t62)
     # * t62 + (t829 - (t827 - t2989) * t62) * t62) / 0.24E2 - t270 * ((
     #t4 * ((t2997 * t12 - t2999) * t12 - t3004) * t12 - t4 * (t3004 - (
     #t3002 - t3008 * t12) * t12) * t12) * t12 + (((t4 * t2997 * t12 - t
     #2794) * t12 - t2799) * t12 - (t2799 - (t2797 - t4 * t3008 * t12) *
     # t12) * t12) * t12) / 0.24E2 - t228 * ((t4 * ((t3034 * t49 - t3036
     #) * t49 - t3041) * t49 - t4 * (t3041 - (t3039 - t3045 * t49) * t49
     #) * t49) * t49 + (((t4 * t3034 * t49 - t2802) * t49 - t2807) * t49
     # - (t2807 - (t2805 - t4 * t3045 * t49) * t49) * t49) * t49) / 0.24
     #E2 + t2811 + t2815
        t3081 = (t4 * (t201 - t698) * t12 - t4 * (t698 - t1146) * t12) *
     # t12
        t3089 = (t4 * (t1586 - t698) * t49 - t4 * (t698 - t2081) * t49) 
     #* t49
        t3090 = src(i,j,t200,nComp,n)
        t3092 = (t935 + t943 + t717 + t968 - t3081 - t3089 - t2930 - t30
     #90) * t62
        t3095 = dz * (t2780 / 0.2E1 + t3092 / 0.2E1)
        t3103 = t186 * (t812 - dz * (t2771 - t2974) / 0.12E2) / 0.12E2
        t3148 = (t4 * (t538 + t546 + t220 - t935 - t943 - t717) * t12 - 
     #t4 * (t935 + t943 + t717 - t1405 - t1413 - t1165) * t12) * t12 + (
     #t4 * (t1844 + t1849 + t1605 - t935 - t943 - t717) * t49 - t4 * (t9
     #35 + t943 + t717 - t2309 - t2314 - t2100) * t49) * t49 + (t946 - t
     #4 * (t935 + t943 + t717 - t3081 - t3089 - t2930) * t62) * t62 + (t
     #4 * (t574 - t968) * t12 - t4 * (t968 - t1438) * t12) * t12 + (t4 *
     # (t1872 - t968) * t49 - t4 * (t968 - t2337) * t49) * t49 + (t971 -
     # t4 * (t968 - t3090) * t62) * t62 + (t2810 - t2814) * t136
        t3179 = dz * (t2817 / 0.2E1 + (t2799 + t2807 + t827 + t2811 + t2
     #815 - (t4 * (t337 - t808) * t12 - t4 * (t808 - t1278) * t12) * t12
     # - (t4 * (t1693 - t808) * t49 - t4 * (t808 - t2211) * t49) * t49 -
     # t2989 - (src(i,j,t200,nComp,t133) - t3090) * t136 / 0.2E1 - (t309
     #0 - src(i,j,t200,nComp,t139)) * t136 / 0.2E1) * t62 / 0.2E1)
        t3183 = dz * (t2780 - t3092)
        t3186 = t2 + t726 - t2778 + t837 - t2785 + t2791 + t978 - t2822 
     #+ t2826 - t162 - t181 * t2965 - t2981 - t324 * t3071 / 0.2E1 - t18
     #1 * t3095 / 0.2E1 - t3103 - t459 * t3148 / 0.6E1 - t324 * t3179 / 
     #0.4E1 - t181 * t3183 / 0.12E2
        t3190 = t1024 * t2876
        t3192 = t1027 * t2879 / 0.2E1
        t3194 = t1031 * t2883 / 0.6E1
        t3196 = t1034 * t2886 / 0.24E2
        t3208 = t2 + t1050 - t2778 + t1052 - t2850 + t2791 + t1056 - t28
     #52 + t2854 - t162 - t1034 * t2965 - t2981 - t1038 * t3071 / 0.2E1 
     #- t1034 * t3095 / 0.2E1 - t3103 - t1043 * t3148 / 0.6E1 - t1038 * 
     #t3179 / 0.4E1 - t1034 * t3183 / 0.12E2
        t3211 = cc * t3208 * t1020 / 0.8E1
        t3213 = (t8 * t2876 + t33 * t2879 / 0.2E1 + t105 * t2883 / 0.6E1
     # - t181 * t2886 / 0.24E2 + cc * t3186 * t1020 / 0.8E1 - t3190 - t3
     #192 - t3194 + t3196 - t3211) * t5
        t3219 = t4 * (t692 - dz * t703 / 0.24E2)
        t3221 = dz * t718 / 0.24E2
        t3231 = src(i,j,k,nComp,n + 2)
        t3233 = (src(i,j,k,nComp,n + 3) - t3231) * t5

        unew(i,j,k) = t1 + dt * t2 + (t1066 * t34 / 0.6E1 + (t1072 + 
     #t1025 + t1029 - t1074 + t1033 - t1036 + t1064 - t1066 * t1023) * t
     #34 / 0.2E1 - t1515 * t34 / 0.6E1 - (t1521 + t1492 + t1494 - t1523 
     #+ t1496 - t1498 + t1513 - t1515 * t1023) * t34 / 0.2E1) * t12 + (t
     #2013 * t34 / 0.6E1 + (t2019 + t1984 + t1986 - t2021 + t1988 - t199
     #0 + t2011 - t2013 * t1023) * t34 / 0.2E1 - t2412 * t34 / 0.6E1 - (
     #t2418 + t2389 + t2391 - t2420 + t2393 - t2395 + t2410 - t2412 * t1
     #023) * t34 / 0.2E1) * t49 + (t2860 * t34 / 0.6E1 + (t2866 + t2831 
     #+ t2833 - t2868 + t2835 - t2837 + t2858 - t2860 * t1023) * t34 / 0
     #.2E1 - t3213 * t34 / 0.6E1 - (t3219 + t3190 + t3192 - t3221 + t319
     #4 - t3196 + t3211 - t3213 * t1023) * t34 / 0.2E1) * t62 + t3233 * 
     #t34 / 0.6E1 + (t3231 - t3233 * t1023) * t34 / 0.2E1

        utnew(i,j,k) = t2 + (t10
     #66 * dt / 0.2E1 + (t1072 + t1025 + t1029 - t1074 + t1033 - t1036 +
     # t1064) * dt - t1066 * t1034 - t1515 * dt / 0.2E1 - (t1521 + t1492
     # + t1494 - t1523 + t1496 - t1498 + t1513) * dt + t1515 * t1034) * 
     #t12 + (t2013 * dt / 0.2E1 + (t2019 + t1984 + t1986 - t2021 + t1988
     # - t1990 + t2011) * dt - t2013 * t1034 - t2412 * dt / 0.2E1 - (t24
     #18 + t2389 + t2391 - t2420 + t2393 - t2395 + t2410) * dt + t2412 *
     # t1034) * t49 + (t2860 * dt / 0.2E1 + (t2866 + t2831 + t2833 - t28
     #68 + t2835 - t2837 + t2858) * dt - t2860 * t1034 - t3213 * dt / 0.
     #2E1 - (t3219 + t3190 + t3192 - t3221 + t3194 - t3196 + t3211) * dt
     # + t3213 * t1034) * t62 + t3233 * dt / 0.2E1 + t3231 * dt - t3233 
     #* t1034

c        blah = array(int(t1 + dt * t2 + (t1066 * t34 / 0.6E1 + (t1072 + 
c     #t1025 + t1029 - t1074 + t1033 - t1036 + t1064 - t1066 * t1023) * t
c     #34 / 0.2E1 - t1515 * t34 / 0.6E1 - (t1521 + t1492 + t1494 - t1523 
c     #+ t1496 - t1498 + t1513 - t1515 * t1023) * t34 / 0.2E1) * t12 + (t
c     #2013 * t34 / 0.6E1 + (t2019 + t1984 + t1986 - t2021 + t1988 - t199
c     #0 + t2011 - t2013 * t1023) * t34 / 0.2E1 - t2412 * t34 / 0.6E1 - (
c     #t2418 + t2389 + t2391 - t2420 + t2393 - t2395 + t2410 - t2412 * t1
c     #023) * t34 / 0.2E1) * t49 + (t2860 * t34 / 0.6E1 + (t2866 + t2831 
c     #+ t2833 - t2868 + t2835 - t2837 + t2858 - t2860 * t1023) * t34 / 0
c     #.2E1 - t3213 * t34 / 0.6E1 - (t3219 + t3190 + t3192 - t3221 + t319
c     #4 - t3196 + t3211 - t3213 * t1023) * t34 / 0.2E1) * t62 + t3233 * 
c     #t34 / 0.6E1 + (t3231 - t3233 * t1023) * t34 / 0.2E1),int(t2 + (t10
c     #66 * dt / 0.2E1 + (t1072 + t1025 + t1029 - t1074 + t1033 - t1036 +
c     # t1064) * dt - t1066 * t1034 - t1515 * dt / 0.2E1 - (t1521 + t1492
c     # + t1494 - t1523 + t1496 - t1498 + t1513) * dt + t1515 * t1034) * 
c     #t12 + (t2013 * dt / 0.2E1 + (t2019 + t1984 + t1986 - t2021 + t1988
c     # - t1990 + t2011) * dt - t2013 * t1034 - t2412 * dt / 0.2E1 - (t24
c     #18 + t2389 + t2391 - t2420 + t2393 - t2395 + t2410) * dt + t2412 *
c     # t1034) * t49 + (t2860 * dt / 0.2E1 + (t2866 + t2831 + t2833 - t28
c     #68 + t2835 - t2837 + t2858) * dt - t2860 * t1034 - t3213 * dt / 0.
c     #2E1 - (t3219 + t3190 + t3192 - t3221 + t3194 - t3196 + t3211) * dt
c     # + t3213 * t1034) * t62 + t3233 * dt / 0.2E1 + t3231 * dt - t3233 
c     #* t1034))

        return
      end
