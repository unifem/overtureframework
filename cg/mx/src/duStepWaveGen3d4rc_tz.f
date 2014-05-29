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
        real t10
        real t1000
        real t1001
        real t1003
        real t1004
        real t1005
        real t1007
        real t1009
        real t101
        real t1012
        real t1013
        real t1016
        real t1017
        real t1018
        real t1019
        real t1022
        real t1024
        real t1026
        real t1028
        real t1029
        real t1031
        real t1034
        real t1035
        real t1036
        real t1037
        real t1038
        real t104
        real t1040
        real t1041
        real t1042
        real t1044
        real t1045
        real t1047
        real t1048
        real t105
        real t1050
        real t1055
        real t106
        real t1062
        real t1064
        real t1066
        real t1068
        real t1070
        real t1072
        real t1073
        real t1076
        real t1078
        real t108
        real t1084
        real t1086
        real t1094
        real t1097
        real t11
        real t110
        real t1101
        real t1104
        real t1107
        real t1108
        real t1110
        real t1113
        real t1115
        real t1119
        real t112
        real t1120
        real t113
        real t1132
        real t1138
        real t114
        integer t1146
        real t1148
        real t116
        real t1160
        real t1168
        real t1169
        real t117
        real t1171
        real t1174
        real t1176
        real t118
        real t1180
        real t1181
        real t1193
        real t1199
        real t12
        real t120
        real t1208
        real t1212
        real t1216
        real t1217
        real t122
        real t1224
        real t1225
        real t1226
        real t1228
        real t123
        real t1231
        real t1233
        real t1237
        real t1238
        real t124
        real t126
        real t1264
        real t1265
        real t1267
        real t127
        real t1270
        real t1272
        real t1276
        real t1277
        real t128
        real t13
        real t130
        real t1310
        real t1319
        real t132
        real t1322
        real t1326
        integer t133
        real t1331
        real t1332
        real t1336
        real t1341
        real t1342
        real t1344
        real t1347
        real t1355
        real t136
        real t1361
        real t1365
        real t1366
        real t137
        real t1370
        real t1375
        real t1379
        real t138
        real t1383
        real t1384
        real t1388
        integer t139
        real t1393
        real t1399
        integer t14
        real t1403
        real t1411
        real t1415
        real t1419
        real t142
        real t1427
        real t143
        real t1438
        real t1442
        real t1448
        real t145
        real t1452
        real t1461
        real t1464
        real t1468
        real t147
        real t1474
        real t1478
        real t148
        real t149
        real t1496
        real t15
        real t1500
        real t1503
        real t1507
        real t1509
        real t151
        real t1511
        real t1513
        real t152
        real t1525
        real t1528
        real t153
        real t1530
        real t1536
        real t1538
        real t1548
        real t155
        real t1550
        real t1552
        real t1555
        real t1557
        real t1558
        real t1560
        real t1562
        real t1563
        real t1564
        real t1566
        real t1567
        real t1568
        real t157
        real t1570
        real t1572
        real t1575
        real t1576
        real t1579
        real t158
        real t1580
        real t1581
        real t1583
        real t1586
        integer t1589
        real t159
        real t1591
        real t16
        real t1603
        real t161
        real t1611
        real t1612
        real t1614
        real t1617
        real t1619
        real t162
        real t1623
        real t1624
        real t163
        real t1636
        real t1642
        real t165
        real t1651
        real t1654
        real t1656
        real t167
        real t1678
        real t1681
        real t1683
        real t1687
        real t1688
        real t1689
        real t1696
        real t1697
        real t1699
        real t17
        real t170
        real t1702
        real t1704
        real t1708
        real t171
        real t174
        real t1741
        real t1749
        real t175
        real t1750
        real t1752
        real t1755
        real t1757
        real t176
        real t1761
        real t1762
        real t178
        real t1789
        real t1792
        real t1800
        real t1801
        real t1805
        real t181
        real t1810
        real t1811
        real t1813
        real t1814
        real t1817
        real t182
        real t1825
        real t183
        real t1846
        real t1847
        real t1851
        real t186
        real t1862
        real t1863
        real t1867
        real t187
        integer t188
        real t1886
        real t1890
        real t1899
        real t19
        real t190
        real t1902
        real t1911
        real t1915
        real t192
        real t1931
        real t1934
        real t1938
        real t1941
        real t1942
        real t1949
        real t195
        real t1950
        real t1951
        real t1954
        real t1956
        real t1962
        real t1963
        real t1965
        real t1966
        real t1968
        real t197
        real t1970
        real t1971
        real t1972
        real t1974
        real t1975
        real t1976
        real t1978
        real t1980
        real t1983
        real t1984
        real t1987
        real t1988
        real t1989
        real t1990
        real t1993
        real t1995
        real t1997
        real t1999
        real t2
        integer t20
        real t2000
        real t2004
        real t2006
        real t2008
        real t201
        real t2010
        real t2023
        real t2025
        real t2027
        real t2028
        real t203
        real t2031
        real t2033
        real t2039
        real t204
        real t2041
        real t2049
        real t2052
        real t2056
        real t2059
        real t206
        integer t2062
        real t2064
        real t2076
        real t2085
        real t2088
        real t2090
        real t21
        real t2111
        real t2112
        real t2114
        real t2117
        real t2119
        real t212
        real t2123
        real t2124
        real t2136
        real t2142
        real t215
        real t2151
        real t2155
        real t2159
        real t216
        real t2160
        real t2167
        real t2168
        real t2170
        real t2173
        real t2175
        real t2179
        real t22
        real t2205
        real t2206
        real t2208
        real t2211
        real t2213
        real t2217
        real t2218
        real t222
        integer t223
        real t224
        real t225
        real t2251
        real t2260
        real t227
        real t2270
        real t2271
        real t2275
        real t2280
        real t2281
        real t2283
        real t2286
        real t2294
        real t23
        real t230
        real t2315
        real t2316
        real t232
        real t2320
        real t2331
        real t2332
        real t2336
        real t2355
        real t2359
        integer t236
        real t2368
        real t237
        real t2379
        real t238
        real t2383
        real t2401
        real t2405
        real t2408
        real t2412
        real t2414
        real t2416
        real t2418
        real t2430
        real t2433
        real t2435
        real t2441
        real t2443
        real t2453
        real t2455
        real t2457
        real t2460
        real t2462
        real t2463
        real t2465
        real t2467
        real t2468
        real t2470
        real t2471
        real t2473
        real t2475
        real t2478
        real t2479
        real t2482
        real t2483
        real t2484
        real t2486
        real t2489
        real t2493
        real t2496
        real t2498
        real t25
        real t250
        real t2520
        real t2523
        real t2525
        integer t2546
        real t2548
        real t256
        real t2560
        real t2569
        real t2572
        real t2574
        real t2578
        real t2579
        real t2580
        real t2587
        real t2588
        real t2590
        real t2593
        real t2595
        real t2599
        real t26
        real t2625
        real t2627
        real t2630
        real t2632
        real t2636
        real t264
        integer t265
        real t266
        real t2669
        real t267
        real t2678
        real t2681
        real t2689
        real t269
        real t2697
        real t2698
        real t2700
        real t2701
        real t2704
        real t2712
        real t272
        real t274
        real t2758
        real t2761
        integer t278
        real t2788
        real t279
        real t2791
        real t2795
        real t2798
        real t2799
        real t280
        real t2806
        real t2807
        real t2808
        real t2811
        real t2813
        real t2819
        real t2820
        real t2822
        real t2823
        real t2825
        real t2827
        real t2828
        real t2830
        real t2831
        real t2833
        real t2835
        real t2838
        real t2839
        real t2842
        real t2843
        real t2844
        real t2845
        real t2848
        real t2850
        real t2852
        real t2854
        real t2855
        real t2859
        real t2861
        real t2863
        real t2865
        real t2878
        real t2880
        real t2882
        real t2883
        real t2886
        real t2888
        real t2894
        real t2896
        real t2904
        real t2907
        real t2911
        real t2914
        real t2918
        real t292
        real t2921
        real t2923
        integer t2944
        real t2946
        real t2958
        real t2967
        real t2970
        real t2972
        real t298
        real t2994
        real t2998
        real t30
        real t3002
        real t3003
        real t3010
        real t3011
        real t3013
        real t3016
        real t3018
        real t3022
        real t3055
        real t3063
        real t3065
        real t3068
        real t307
        real t3070
        real t3074
        real t310
        real t3101
        real t3111
        real t3119
        real t312
        real t3120
        real t3122
        real t3125
        real t3133
        real t316
        real t317
        real t3179
        real t318
        real t32
        real t3210
        real t3214
        real t3217
        real t3221
        real t3223
        real t3225
        real t3227
        real t3239
        real t3242
        real t3244
        real t325
        real t3250
        real t3252
        real t326
        real t3262
        real t3264
        real t327
        real t328
        real t329
        real t33
        real t331
        real t334
        real t336
        real t34
        real t340
        real t341
        real t35
        real t36
        real t367
        real t368
        real t37
        real t370
        real t373
        real t375
        real t379
        real t380
        real t39
        real t4
        real t40
        real t409
        real t415
        real t418
        real t42
        real t425
        real t428
        real t429
        real t433
        real t438
        real t439
        real t44
        real t443
        real t448
        real t449
        integer t45
        real t451
        real t452
        real t455
        real t46
        real t463
        real t464
        real t465
        real t47
        real t471
        real t474
        real t477
        real t479
        real t481
        real t482
        real t486
        real t49
        real t491
        real t495
        real t498
        real t5
        real t50
        real t500
        real t502
        real t503
        real t507
        integer t51
        real t512
        real t518
        real t52
        real t521
        real t523
        real t525
        real t53
        real t533
        real t537
        real t540
        real t542
        real t544
        real t55
        real t552
        real t563
        real t566
        real t57
        real t570
        real t576
        integer t58
        real t580
        real t589
        real t59
        real t592
        real t593
        real t597
        real t6
        real t60
        real t603
        real t607
        real t62
        real t623
        real t626
        real t63
        real t630
        integer t633
        real t634
        real t635
        real t636
        real t638
        real t639
        integer t64
        real t641
        real t645
        real t647
        real t648
        real t649
        real t65
        real t655
        real t656
        real t657
        real t658
        real t66
        real t660
        real t661
        real t663
        real t664
        real t666
        real t667
        real t668
        real t669
        real t671
        real t672
        real t674
        real t678
        real t68
        real t680
        real t681
        real t682
        real t684
        real t686
        real t687
        real t688
        real t694
        real t695
        real t696
        real t697
        real t699
        real t7
        real t70
        real t700
        real t702
        real t703
        real t705
        real t706
        real t707
        real t708
        real t71
        real t710
        real t711
        real t713
        real t717
        real t719
        real t72
        real t720
        real t721
        real t723
        real t725
        real t726
        real t727
        real t73
        real t734
        real t735
        real t736
        real t737
        real t738
        real t739
        real t741
        real t742
        real t743
        real t75
        real t750
        real t751
        real t752
        real t753
        real t754
        real t756
        real t757
        real t759
        real t760
        real t762
        real t763
        real t764
        real t765
        real t767
        real t768
        real t77
        real t770
        real t774
        real t776
        real t777
        real t778
        real t78
        real t780
        real t782
        real t783
        real t784
        real t79
        real t791
        real t795
        real t797
        real t798
        real t799
        real t8
        real t805
        real t806
        real t807
        real t808
        real t81
        real t810
        real t811
        real t813
        real t814
        real t816
        real t817
        real t818
        real t819
        real t82
        real t821
        real t822
        real t824
        real t828
        real t83
        real t830
        real t831
        real t832
        real t834
        real t836
        real t837
        real t838
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
        real t861
        real t862
        real t863
        real t865
        real t867
        real t868
        real t869
        real t87
        real t870
        real t873
        real t875
        real t88
        real t881
        real t884
        real t887
        real t889
        real t89
        real t891
        real t892
        real t893
        real t895
        real t896
        real t897
        real t899
        integer t9
        real t901
        real t904
        real t905
        real t907
        real t909
        real t91
        real t910
        real t911
        real t913
        real t914
        real t915
        real t917
        real t919
        real t92
        real t922
        real t925
        real t927
        real t929
        real t93
        real t930
        real t932
        real t933
        real t935
        real t937
        real t940
        real t941
        real t943
        real t945
        real t946
        real t948
        real t949
        real t95
        real t951
        real t953
        real t956
        real t961
        real t964
        real t967
        real t968
        real t97
        real t971
        real t974
        real t977
        real t978
        real t98
        real t981
        real t987
        real t989
        real t99
        real t990
        real t991
        real t993
        real t994
        real t995
        real t997
        real t999
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
        t186 = beta * t7
        t187 = dx ** 2
        t188 = i + 3
        t190 = u(t188,j,k,n) - t35
        t192 = t37 * t12
        t195 = t40 * t12
        t197 = (t192 - t195) * t12
        t201 = t73 * t12
        t203 = (t195 - t201) * t12
        t204 = t197 - t203
        t206 = t4 * t204 * t12
        t212 = (t4 * t190 * t12 - t39) * t12
        t215 = t44 - t77
        t216 = t215 * t12
        t222 = dy ** 2
        t223 = j + 2
        t224 = u(t9,t223,k,n)
        t225 = t224 - t46
        t227 = t47 * t49
        t230 = t53 * t49
        t232 = (t227 - t230) * t49
        t236 = j - 2
        t237 = u(t9,t236,k,n)
        t238 = t52 - t237
        t250 = (t4 * t225 * t49 - t50) * t49
        t256 = (t55 - t4 * t238 * t49) * t49
        t264 = dz ** 2
        t265 = k + 2
        t266 = u(t9,j,t265,n)
        t267 = t266 - t59
        t269 = t62 * t60
        t272 = t66 * t62
        t274 = (t269 - t272) * t62
        t278 = k - 2
        t279 = u(t9,j,t278,n)
        t280 = t65 - t279
        t292 = (t4 * t267 * t62 - t63) * t62
        t298 = (t68 - t4 * t280 * t62) * t62
        t307 = dt * (-t187 * ((t4 * ((t190 * t12 - t192) * t12 - t197) *
     # t12 - t206) * t12 + ((t212 - t44) * t12 - t216) * t12) / 0.24E2 +
     # t70 + t57 - t222 * ((t4 * ((t225 * t49 - t227) * t49 - t232) * t4
     #9 - t4 * (t232 - (t230 - t238 * t49) * t49) * t49) * t49 + ((t250 
     #- t57) * t49 - (t57 - t256) * t49) * t49) / 0.24E2 - t264 * ((t4 *
     # ((t267 * t62 - t269) * t62 - t274) * t62 - t4 * (t274 - (t272 - t
     #280 * t62) * t62) * t62) * t62 + ((t292 - t70) * t62 - (t70 - t298
     #) * t62) * t62) / 0.24E2 + t44 + t71)
        t310 = t13 / 0.2E1
        t312 = ut(t188,j,k,n) - t15
        t316 = (t312 * t12 - t17) * t12 - t19
        t317 = t316 * t12
        t318 = t26 * t12
        t325 = dx * (t17 / 0.2E1 + t310 - t187 * (t317 / 0.2E1 + t318 / 
     #0.2E1) / 0.6E1) / 0.2E1
        t326 = beta ** 2
        t327 = t326 * t32
        t328 = ut(t9,j,t265,n)
        t329 = t328 - t123
        t331 = t124 * t62
        t334 = t128 * t62
        t336 = (t331 - t334) * t62
        t340 = ut(t9,j,t278,n)
        t341 = t127 - t340
        t367 = ut(t9,t223,k,n)
        t368 = t367 - t113
        t370 = t114 * t49
        t373 = t118 * t49
        t375 = (t370 - t373) * t49
        t379 = ut(t9,t236,k,n)
        t380 = t117 - t379
        t409 = t4 * t26 * t12
        t415 = (t4 * t312 * t12 - t108) * t12
        t418 = t182 * t12
        t425 = t34 * (t132 - t264 * ((t4 * ((t329 * t62 - t331) * t62 - 
     #t336) * t62 - t4 * (t336 - (t334 - t341 * t62) * t62) * t62) * t62
     # + (((t4 * t329 * t62 - t126) * t62 - t132) * t62 - (t132 - (t130 
     #- t4 * t341 * t62) * t62) * t62) * t62) / 0.24E2 - t222 * ((t4 * (
     #(t368 * t49 - t370) * t49 - t375) * t49 - t4 * (t375 - (t373 - t38
     #0 * t49) * t49) * t49) * t49 + (((t4 * t368 * t49 - t116) * t49 - 
     #t122) * t49 - (t122 - (t120 - t4 * t380 * t49) * t49) * t49) * t49
     #) / 0.24E2 - t187 * ((t4 * t316 * t12 - t409) * t12 + ((t415 - t11
     #2) * t12 - t418) * t12) / 0.24E2 + t122 + t112 + t138 + t143)
        t428 = dt * dx
        t429 = u(t14,t45,k,n)
        t433 = u(t14,t51,k,n)
        t438 = (t4 * (t429 - t35) * t49 - t4 * (t35 - t433) * t49) * t49
        t439 = u(t14,j,t58,n)
        t443 = u(t14,j,t64,n)
        t448 = (t4 * (t439 - t35) * t62 - t4 * (t35 - t443) * t62) * t62
        t449 = src(t14,j,k,nComp,n)
        t451 = (t212 + t438 + t448 + t449 - t44 - t57 - t70 - t71) * t12
        t452 = t99 * t12
        t455 = t428 * (t451 / 0.2E1 + t452 / 0.2E1)
        t463 = t187 * (t19 - dx * (t317 - t318) / 0.12E2) / 0.12E2
        t464 = t326 * beta
        t465 = t464 * t104
        t471 = t4 * (t44 + t57 + t70 - t77 - t87 - t97) * t12
        t474 = t429 - t46
        t477 = t46 - t78
        t479 = t4 * t477 * t12
        t481 = (t4 * t474 * t12 - t479) * t12
        t482 = u(t9,t45,t58,n)
        t486 = u(t9,t45,t64,n)
        t491 = (t4 * (t482 - t46) * t62 - t4 * (t46 - t486) * t62) * t62
        t495 = t433 - t52
        t498 = t52 - t82
        t500 = t4 * t498 * t12
        t502 = (t4 * t495 * t12 - t500) * t12
        t503 = u(t9,t51,t58,n)
        t507 = u(t9,t51,t64,n)
        t512 = (t4 * (t503 - t52) * t62 - t4 * (t52 - t507) * t62) * t62
        t518 = t439 - t59
        t521 = t59 - t88
        t523 = t4 * t521 * t12
        t525 = (t4 * t518 * t12 - t523) * t12
        t533 = (t4 * (t482 - t59) * t49 - t4 * (t59 - t503) * t49) * t49
        t537 = t443 - t65
        t540 = t65 - t92
        t542 = t4 * t540 * t12
        t544 = (t4 * t537 * t12 - t542) * t12
        t552 = (t4 * (t486 - t65) * t49 - t4 * (t65 - t507) * t49) * t49
        t563 = t4 * (t71 - t98) * t12
        t566 = src(t9,t45,k,nComp,n)
        t570 = src(t9,t51,k,nComp,n)
        t576 = src(t9,j,t58,nComp,n)
        t580 = src(t9,j,t64,nComp,n)
        t589 = t106 * ((t4 * (t212 + t438 + t448 - t44 - t57 - t70) * t1
     #2 - t471) * t12 + (t4 * (t481 + t250 + t491 - t44 - t57 - t70) * t
     #49 - t4 * (t44 + t57 + t70 - t502 - t256 - t512) * t49) * t49 + (t
     #4 * (t525 + t533 + t292 - t44 - t57 - t70) * t62 - t4 * (t44 + t57
     # + t70 - t544 - t552 - t298) * t62) * t62 + (t4 * (t449 - t71) * t
     #12 - t563) * t12 + (t4 * (t566 - t71) * t49 - t4 * (t71 - t570) * 
     #t49) * t49 + (t4 * (t576 - t71) * t62 - t4 * (t71 - t580) * t62) *
     # t62 + (t137 - t142) * t136)
        t592 = t34 * dx
        t593 = ut(t14,t45,k,n)
        t597 = ut(t14,t51,k,n)
        t603 = ut(t14,j,t58,n)
        t607 = ut(t14,j,t64,n)
        t623 = t176 * t12
        t626 = t592 * ((t415 + (t4 * (t593 - t15) * t49 - t4 * (t15 - t5
     #97) * t49) * t49 + (t4 * (t603 - t15) * t62 - t4 * (t15 - t607) * 
     #t62) * t62 + (src(t14,j,k,nComp,t133) - t449) * t136 / 0.2E1 + (t4
     #49 - src(t14,j,k,nComp,t139)) * t136 / 0.2E1 - t112 - t122 - t132 
     #- t138 - t143) * t12 / 0.2E1 + t623 / 0.2E1)
        t630 = t428 * (t451 - t452)
        t633 = i - 2
        t634 = u(t633,j,k,n)
        t635 = t72 - t634
        t636 = t635 * t12
        t638 = (t201 - t636) * t12
        t639 = t203 - t638
        t641 = t4 * t639 * t12
        t645 = t4 * t635 * t12
        t647 = (t75 - t645) * t12
        t648 = t77 - t647
        t649 = t648 * t12
        t655 = u(i,t223,k,n)
        t656 = t655 - t78
        t657 = t656 * t49
        t658 = t79 * t49
        t660 = (t657 - t658) * t49
        t661 = t83 * t49
        t663 = (t658 - t661) * t49
        t664 = t660 - t663
        t666 = t4 * t664 * t49
        t667 = u(i,t236,k,n)
        t668 = t82 - t667
        t669 = t668 * t49
        t671 = (t661 - t669) * t49
        t672 = t663 - t671
        t674 = t4 * t672 * t49
        t678 = t4 * t656 * t49
        t680 = (t678 - t81) * t49
        t681 = t680 - t87
        t682 = t681 * t49
        t684 = t4 * t668 * t49
        t686 = (t85 - t684) * t49
        t687 = t87 - t686
        t688 = t687 * t49
        t694 = u(i,j,t265,n)
        t695 = t694 - t88
        t696 = t695 * t62
        t697 = t89 * t62
        t699 = (t696 - t697) * t62
        t700 = t93 * t62
        t702 = (t697 - t700) * t62
        t703 = t699 - t702
        t705 = t4 * t703 * t62
        t706 = u(i,j,t278,n)
        t707 = t92 - t706
        t708 = t707 * t62
        t710 = (t700 - t708) * t62
        t711 = t702 - t710
        t713 = t4 * t711 * t62
        t717 = t4 * t695 * t62
        t719 = (t717 - t91) * t62
        t720 = t719 - t97
        t721 = t720 * t62
        t723 = t4 * t707 * t62
        t725 = (t95 - t723) * t62
        t726 = t97 - t725
        t727 = t726 * t62
        t734 = dt * (t87 + t77 - t187 * ((t206 - t641) * t12 + (t216 - t
     #649) * t12) / 0.24E2 - t222 * ((t666 - t674) * t49 + (t682 - t688)
     # * t49) / 0.24E2 + t97 - t264 * ((t705 - t713) * t62 + (t721 - t72
     #7) * t62) / 0.24E2 + t98)
        t735 = t186 * t734
        t736 = t23 / 0.2E1
        t737 = ut(t633,j,k,n)
        t738 = t21 - t737
        t739 = t738 * t12
        t741 = (t23 - t739) * t12
        t742 = t25 - t741
        t743 = t742 * t12
        t750 = dx * (t310 + t736 - t187 * (t318 / 0.2E1 + t743 / 0.2E1) 
     #/ 0.6E1) / 0.2E1
        t751 = ut(i,t223,k,n)
        t752 = t751 - t148
        t753 = t752 * t49
        t754 = t149 * t49
        t756 = (t753 - t754) * t49
        t757 = t153 * t49
        t759 = (t754 - t757) * t49
        t760 = t756 - t759
        t762 = t4 * t760 * t49
        t763 = ut(i,t236,k,n)
        t764 = t152 - t763
        t765 = t764 * t49
        t767 = (t757 - t765) * t49
        t768 = t759 - t767
        t770 = t4 * t768 * t49
        t774 = t4 * t752 * t49
        t776 = (t774 - t151) * t49
        t777 = t776 - t157
        t778 = t777 * t49
        t780 = t4 * t764 * t49
        t782 = (t155 - t780) * t49
        t783 = t157 - t782
        t784 = t783 * t49
        t791 = t4 * t742 * t12
        t795 = t4 * t738 * t12
        t797 = (t145 - t795) * t12
        t798 = t147 - t797
        t799 = t798 * t12
        t805 = ut(i,j,t265,n)
        t806 = t805 - t158
        t807 = t806 * t62
        t808 = t159 * t62
        t810 = (t807 - t808) * t62
        t811 = t163 * t62
        t813 = (t808 - t811) * t62
        t814 = t810 - t813
        t816 = t4 * t814 * t62
        t817 = ut(i,j,t278,n)
        t818 = t162 - t817
        t819 = t818 * t62
        t821 = (t811 - t819) * t62
        t822 = t813 - t821
        t824 = t4 * t822 * t62
        t828 = t4 * t806 * t62
        t830 = (t828 - t161) * t62
        t831 = t830 - t167
        t832 = t831 * t62
        t834 = t4 * t818 * t62
        t836 = (t165 - t834) * t62
        t837 = t167 - t836
        t838 = t837 * t62
        t845 = t34 * (-t222 * ((t762 - t770) * t49 + (t778 - t784) * t49
     #) / 0.24E2 + t167 - t187 * ((t409 - t791) * t12 + (t418 - t799) * 
     #t12) / 0.24E2 + t147 + t157 - t264 * ((t816 - t824) * t62 + (t832 
     #- t838) * t62) / 0.24E2 + t171 + t175)
        t847 = t327 * t845 / 0.2E1
        t848 = u(t20,t45,k,n)
        t849 = t848 - t72
        t851 = t4 * t849 * t49
        t852 = u(t20,t51,k,n)
        t853 = t72 - t852
        t855 = t4 * t853 * t49
        t857 = (t851 - t855) * t49
        t858 = u(t20,j,t58,n)
        t859 = t858 - t72
        t861 = t4 * t859 * t62
        t862 = u(t20,j,t64,n)
        t863 = t72 - t862
        t865 = t4 * t863 * t62
        t867 = (t861 - t865) * t62
        t868 = src(t20,j,k,nComp,n)
        t869 = t77 + t87 + t97 + t98 - t647 - t857 - t867 - t868
        t870 = t869 * t12
        t873 = t428 * (t452 / 0.2E1 + t870 / 0.2E1)
        t875 = t186 * t873 / 0.2E1
        t881 = t187 * (t25 - dx * (t318 - t743) / 0.12E2) / 0.12E2
        t884 = t4 * (t77 + t87 + t97 - t647 - t857 - t867) * t12
        t887 = t78 - t848
        t889 = t4 * t887 * t12
        t891 = (t479 - t889) * t12
        t892 = u(i,t45,t58,n)
        t893 = t892 - t78
        t895 = t4 * t893 * t62
        t896 = u(i,t45,t64,n)
        t897 = t78 - t896
        t899 = t4 * t897 * t62
        t901 = (t895 - t899) * t62
        t904 = t4 * (t891 + t680 + t901 - t77 - t87 - t97) * t49
        t905 = t82 - t852
        t907 = t4 * t905 * t12
        t909 = (t500 - t907) * t12
        t910 = u(i,t51,t58,n)
        t911 = t910 - t82
        t913 = t4 * t911 * t62
        t914 = u(i,t51,t64,n)
        t915 = t82 - t914
        t917 = t4 * t915 * t62
        t919 = (t913 - t917) * t62
        t922 = t4 * (t77 + t87 + t97 - t909 - t686 - t919) * t49
        t925 = t88 - t858
        t927 = t4 * t925 * t12
        t929 = (t523 - t927) * t12
        t930 = t892 - t88
        t932 = t4 * t930 * t49
        t933 = t88 - t910
        t935 = t4 * t933 * t49
        t937 = (t932 - t935) * t49
        t940 = t4 * (t929 + t937 + t719 - t77 - t87 - t97) * t62
        t941 = t92 - t862
        t943 = t4 * t941 * t12
        t945 = (t542 - t943) * t12
        t946 = t896 - t92
        t948 = t4 * t946 * t49
        t949 = t92 - t914
        t951 = t4 * t949 * t49
        t953 = (t948 - t951) * t49
        t956 = t4 * (t77 + t87 + t97 - t945 - t953 - t725) * t62
        t961 = t4 * (t98 - t868) * t12
        t964 = src(i,t45,k,nComp,n)
        t967 = t4 * (t964 - t98) * t49
        t968 = src(i,t51,k,nComp,n)
        t971 = t4 * (t98 - t968) * t49
        t974 = src(i,j,t58,nComp,n)
        t977 = t4 * (t974 - t98) * t62
        t978 = src(i,j,t64,nComp,n)
        t981 = t4 * (t98 - t978) * t62
        t987 = t106 * ((t471 - t884) * t12 + (t904 - t922) * t49 + (t940
     # - t956) * t62 + (t563 - t961) * t12 + (t967 - t971) * t49 + (t977
     # - t981) * t62 + (t170 - t174) * t136)
        t989 = t465 * t987 / 0.6E1
        t990 = ut(t20,t45,k,n)
        t991 = t990 - t21
        t993 = t4 * t991 * t49
        t994 = ut(t20,t51,k,n)
        t995 = t21 - t994
        t997 = t4 * t995 * t49
        t999 = (t993 - t997) * t49
        t1000 = ut(t20,j,t58,n)
        t1001 = t1000 - t21
        t1003 = t4 * t1001 * t62
        t1004 = ut(t20,j,t64,n)
        t1005 = t21 - t1004
        t1007 = t4 * t1005 * t62
        t1009 = (t1003 - t1007) * t62
        t1012 = (src(t20,j,k,nComp,t133) - t868) * t136
        t1013 = t1012 / 0.2E1
        t1016 = (t868 - src(t20,j,k,nComp,t139)) * t136
        t1017 = t1016 / 0.2E1
        t1018 = t147 + t157 + t167 + t171 + t175 - t797 - t999 - t1009 -
     # t1013 - t1017
        t1019 = t1018 * t12
        t1022 = t592 * (t623 / 0.2E1 + t1019 / 0.2E1)
        t1024 = t327 * t1022 / 0.4E1
        t1026 = t428 * (t452 - t870)
        t1028 = t186 * t1026 / 0.12E2
        t1029 = t10 + t186 * t307 - t325 + t327 * t425 / 0.2E1 - t186 * 
     #t455 / 0.2E1 + t463 + t465 * t589 / 0.6E1 - t327 * t626 / 0.4E1 + 
     #t186 * t630 / 0.12E2 - t2 - t735 - t750 - t847 - t875 - t881 - t98
     #9 - t1024 - t1028
        t1031 = sqrt(0.16E2)
        t1034 = 0.1E1 / 0.2E1 - t6
        t1035 = t4 * t1034
        t1036 = t1035 * t30
        t1037 = t1034 ** 2
        t1038 = t4 * t1037
        t1040 = t1038 * t101 / 0.2E1
        t1041 = t1037 * t1034
        t1042 = t4 * t1041
        t1044 = t1042 * t178 / 0.6E1
        t1045 = t1034 * dt
        t1047 = t1045 * t183 / 0.24E2
        t1048 = beta * t1034
        t1050 = t326 * t1037
        t1055 = t464 * t1041
        t1062 = t1048 * t734
        t1064 = t1050 * t845 / 0.2E1
        t1066 = t1048 * t873 / 0.2E1
        t1068 = t1055 * t987 / 0.6E1
        t1070 = t1050 * t1022 / 0.4E1
        t1072 = t1048 * t1026 / 0.12E2
        t1073 = t10 + t1048 * t307 - t325 + t1050 * t425 / 0.2E1 - t1048
     # * t455 / 0.2E1 + t463 + t1055 * t589 / 0.6E1 - t1050 * t626 / 0.4
     #E1 + t1048 * t630 / 0.12E2 - t2 - t1062 - t750 - t1064 - t1066 - t
     #881 - t1068 - t1070 - t1072
        t1076 = cc * t1073 * t1031 / 0.8E1
        t1078 = (t8 * t30 + t33 * t101 / 0.2E1 + t105 * t178 / 0.6E1 - t
     #181 * t183 / 0.24E2 + cc * t1029 * t1031 / 0.8E1 - t1036 - t1040 -
     # t1044 + t1047 - t1076) * t5
        t1084 = t4 * (t195 - dx * t204 / 0.24E2)
        t1086 = dx * t215 / 0.24E2
        t1094 = dt * (t23 - dx * t742 / 0.24E2)
        t1097 = t34 * t869 * t12
        t1101 = t106 * t1018 * t12
        t1104 = dx * t798
        t1107 = u(t20,j,t265,n)
        t1108 = t1107 - t858
        t1110 = t859 * t62
        t1113 = t863 * t62
        t1115 = (t1110 - t1113) * t62
        t1119 = u(t20,j,t278,n)
        t1120 = t862 - t1119
        t1132 = (t4 * t1108 * t62 - t861) * t62
        t1138 = (t865 - t4 * t1120 * t62) * t62
        t1146 = i - 3
        t1148 = t634 - u(t1146,j,k,n)
        t1160 = (t645 - t4 * t1148 * t12) * t12
        t1168 = u(t20,t223,k,n)
        t1169 = t1168 - t848
        t1171 = t849 * t49
        t1174 = t853 * t49
        t1176 = (t1171 - t1174) * t49
        t1180 = u(t20,t236,k,n)
        t1181 = t852 - t1180
        t1193 = (t4 * t1169 * t49 - t851) * t49
        t1199 = (t855 - t4 * t1181 * t49) * t49
        t1208 = dt * (-t264 * ((t4 * ((t1108 * t62 - t1110) * t62 - t111
     #5) * t62 - t4 * (t1115 - (t1113 - t1120 * t62) * t62) * t62) * t62
     # + ((t1132 - t867) * t62 - (t867 - t1138) * t62) * t62) / 0.24E2 +
     # t647 - t187 * ((t641 - t4 * (t638 - (t636 - t1148 * t12) * t12) *
     # t12) * t12 + (t649 - (t647 - t1160) * t12) * t12) / 0.24E2 + t867
     # + t857 - t222 * ((t4 * ((t1169 * t49 - t1171) * t49 - t1176) * t4
     #9 - t4 * (t1176 - (t1174 - t1181 * t49) * t49) * t49) * t49 + ((t1
     #193 - t857) * t49 - (t857 - t1199) * t49) * t49) / 0.24E2 + t868)
        t1212 = t737 - ut(t1146,j,k,n)
        t1216 = t741 - (t739 - t1212 * t12) * t12
        t1217 = t1216 * t12
        t1224 = dx * (t736 + t739 / 0.2E1 - t187 * (t743 / 0.2E1 + t1217
     # / 0.2E1) / 0.6E1) / 0.2E1
        t1225 = ut(t20,t223,k,n)
        t1226 = t1225 - t990
        t1228 = t991 * t49
        t1231 = t995 * t49
        t1233 = (t1228 - t1231) * t49
        t1237 = ut(t20,t236,k,n)
        t1238 = t994 - t1237
        t1264 = ut(t20,j,t265,n)
        t1265 = t1264 - t1000
        t1267 = t1001 * t62
        t1270 = t1005 * t62
        t1272 = (t1267 - t1270) * t62
        t1276 = ut(t20,j,t278,n)
        t1277 = t1004 - t1276
        t1310 = (t795 - t4 * t1212 * t12) * t12
        t1319 = t34 * (t797 + t1009 + t999 - t222 * ((t4 * ((t1226 * t49
     # - t1228) * t49 - t1233) * t49 - t4 * (t1233 - (t1231 - t1238 * t4
     #9) * t49) * t49) * t49 + (((t4 * t1226 * t49 - t993) * t49 - t999)
     # * t49 - (t999 - (t997 - t4 * t1238 * t49) * t49) * t49) * t49) / 
     #0.24E2 - t264 * ((t4 * ((t1265 * t62 - t1267) * t62 - t1272) * t62
     # - t4 * (t1272 - (t1270 - t1277 * t62) * t62) * t62) * t62 + (((t4
     # * t1265 * t62 - t1003) * t62 - t1009) * t62 - (t1009 - (t1007 - t
     #4 * t1277 * t62) * t62) * t62) * t62) / 0.24E2 - t187 * ((t791 - t
     #4 * t1216 * t12) * t12 + (t799 - (t797 - t1310) * t12) * t12) / 0.
     #24E2 + t1013 + t1017)
        t1322 = u(t633,t45,k,n)
        t1326 = u(t633,t51,k,n)
        t1331 = (t4 * (t1322 - t634) * t49 - t4 * (t634 - t1326) * t49) 
     #* t49
        t1332 = u(t633,j,t58,n)
        t1336 = u(t633,j,t64,n)
        t1341 = (t4 * (t1332 - t634) * t62 - t4 * (t634 - t1336) * t62) 
     #* t62
        t1342 = src(t633,j,k,nComp,n)
        t1344 = (t647 + t857 + t867 + t868 - t1160 - t1331 - t1341 - t13
     #42) * t12
        t1347 = t428 * (t870 / 0.2E1 + t1344 / 0.2E1)
        t1355 = t187 * (t741 - dx * (t743 - t1217) / 0.12E2) / 0.12E2
        t1361 = t848 - t1322
        t1365 = (t889 - t4 * t1361 * t12) * t12
        t1366 = u(t20,t45,t58,n)
        t1370 = u(t20,t45,t64,n)
        t1375 = (t4 * (t1366 - t848) * t62 - t4 * (t848 - t1370) * t62) 
     #* t62
        t1379 = t852 - t1326
        t1383 = (t907 - t4 * t1379 * t12) * t12
        t1384 = u(t20,t51,t58,n)
        t1388 = u(t20,t51,t64,n)
        t1393 = (t4 * (t1384 - t852) * t62 - t4 * (t852 - t1388) * t62) 
     #* t62
        t1399 = t858 - t1332
        t1403 = (t927 - t4 * t1399 * t12) * t12
        t1411 = (t4 * (t1366 - t858) * t49 - t4 * (t858 - t1384) * t49) 
     #* t49
        t1415 = t862 - t1336
        t1419 = (t943 - t4 * t1415 * t12) * t12
        t1427 = (t4 * (t1370 - t862) * t49 - t4 * (t862 - t1388) * t49) 
     #* t49
        t1438 = src(t20,t45,k,nComp,n)
        t1442 = src(t20,t51,k,nComp,n)
        t1448 = src(t20,j,t58,nComp,n)
        t1452 = src(t20,j,t64,nComp,n)
        t1461 = t106 * ((t884 - t4 * (t647 + t857 + t867 - t1160 - t1331
     # - t1341) * t12) * t12 + (t4 * (t1365 + t1193 + t1375 - t647 - t85
     #7 - t867) * t49 - t4 * (t647 + t857 + t867 - t1383 - t1199 - t1393
     #) * t49) * t49 + (t4 * (t1403 + t1411 + t1132 - t647 - t857 - t867
     #) * t62 - t4 * (t647 + t857 + t867 - t1419 - t1427 - t1138) * t62)
     # * t62 + (t961 - t4 * (t868 - t1342) * t12) * t12 + (t4 * (t1438 -
     # t868) * t49 - t4 * (t868 - t1442) * t49) * t49 + (t4 * (t1448 - t
     #868) * t62 - t4 * (t868 - t1452) * t62) * t62 + (t1012 - t1016) * 
     #t136)
        t1464 = ut(t633,t45,k,n)
        t1468 = ut(t633,t51,k,n)
        t1474 = ut(t633,j,t58,n)
        t1478 = ut(t633,j,t64,n)
        t1496 = t592 * (t1019 / 0.2E1 + (t797 + t999 + t1009 + t1013 + t
     #1017 - t1310 - (t4 * (t1464 - t737) * t49 - t4 * (t737 - t1468) * 
     #t49) * t49 - (t4 * (t1474 - t737) * t62 - t4 * (t737 - t1478) * t6
     #2) * t62 - (src(t633,j,k,nComp,t133) - t1342) * t136 / 0.2E1 - (t1
     #342 - src(t633,j,k,nComp,t139)) * t136 / 0.2E1) * t12 / 0.2E1)
        t1500 = t428 * (t870 - t1344)
        t1503 = t2 + t735 - t750 + t847 - t875 + t881 + t989 - t1024 + t
     #1028 - t21 - t186 * t1208 - t1224 - t327 * t1319 / 0.2E1 - t186 * 
     #t1347 / 0.2E1 - t1355 - t465 * t1461 / 0.6E1 - t327 * t1496 / 0.4E
     #1 - t186 * t1500 / 0.12E2
        t1507 = t1035 * t1094
        t1509 = t1038 * t1097 / 0.2E1
        t1511 = t1042 * t1101 / 0.6E1
        t1513 = t1045 * t1104 / 0.24E2
        t1525 = t2 + t1062 - t750 + t1064 - t1066 + t881 + t1068 - t1070
     # + t1072 - t21 - t1048 * t1208 - t1224 - t1050 * t1319 / 0.2E1 - t
     #1048 * t1347 / 0.2E1 - t1355 - t1055 * t1461 / 0.6E1 - t1050 * t14
     #96 / 0.4E1 - t1048 * t1500 / 0.12E2
        t1528 = cc * t1525 * t1031 / 0.8E1
        t1530 = (t8 * t1094 + t33 * t1097 / 0.2E1 + t105 * t1101 / 0.6E1
     # - t181 * t1104 / 0.24E2 + cc * t1503 * t1031 / 0.8E1 - t1507 - t1
     #509 - t1511 + t1513 - t1528) * t5
        t1536 = t4 * (t201 - dx * t639 / 0.24E2)
        t1538 = dx * t648 / 0.24E2
        t1548 = dt * (t754 - dy * t760 / 0.24E2)
        t1550 = t891 + t680 + t901 + t964 - t77 - t87 - t97 - t98
        t1552 = t34 * t1550 * t49
        t1555 = t113 - t148
        t1557 = t4 * t1555 * t12
        t1558 = t148 - t990
        t1560 = t4 * t1558 * t12
        t1562 = (t1557 - t1560) * t12
        t1563 = ut(i,t45,t58,n)
        t1564 = t1563 - t148
        t1566 = t4 * t1564 * t62
        t1567 = ut(i,t45,t64,n)
        t1568 = t148 - t1567
        t1570 = t4 * t1568 * t62
        t1572 = (t1566 - t1570) * t62
        t1575 = (src(i,t45,k,nComp,t133) - t964) * t136
        t1576 = t1575 / 0.2E1
        t1579 = (t964 - src(i,t45,k,nComp,t139)) * t136
        t1580 = t1579 / 0.2E1
        t1581 = t1562 + t776 + t1572 + t1576 + t1580 - t147 - t157 - t16
     #7 - t171 - t175
        t1583 = t106 * t1581 * t49
        t1586 = dy * t777
        t1589 = j + 3
        t1591 = u(i,t1589,k,n) - t655
        t1603 = (t4 * t1591 * t49 - t678) * t49
        t1611 = u(i,t45,t265,n)
        t1612 = t1611 - t892
        t1614 = t893 * t62
        t1617 = t897 * t62
        t1619 = (t1614 - t1617) * t62
        t1623 = u(i,t45,t278,n)
        t1624 = t896 - t1623
        t1636 = (t4 * t1612 * t62 - t895) * t62
        t1642 = (t899 - t4 * t1624 * t62) * t62
        t1651 = t477 * t12
        t1654 = t887 * t12
        t1656 = (t1651 - t1654) * t12
        t1678 = dt * (-t222 * ((t4 * ((t1591 * t49 - t657) * t49 - t660)
     # * t49 - t666) * t49 + ((t1603 - t680) * t49 - t682) * t49) / 0.24
     #E2 - t264 * ((t4 * ((t1612 * t62 - t1614) * t62 - t1619) * t62 - t
     #4 * (t1619 - (t1617 - t1624 * t62) * t62) * t62) * t62 + ((t1636 -
     # t901) * t62 - (t901 - t1642) * t62) * t62) / 0.24E2 - t187 * ((t4
     # * ((t474 * t12 - t1651) * t12 - t1656) * t12 - t4 * (t1656 - (t16
     #54 - t1361 * t12) * t12) * t12) * t12 + ((t481 - t891) * t12 - (t8
     #91 - t1365) * t12) * t12) / 0.24E2 + t891 + t680 + t901 + t964)
        t1681 = t754 / 0.2E1
        t1683 = ut(i,t1589,k,n) - t751
        t1687 = (t1683 * t49 - t753) * t49 - t756
        t1688 = t1687 * t49
        t1689 = t760 * t49
        t1696 = dy * (t753 / 0.2E1 + t1681 - t222 * (t1688 / 0.2E1 + t16
     #89 / 0.2E1) / 0.6E1) / 0.2E1
        t1697 = t593 - t113
        t1699 = t1555 * t12
        t1702 = t1558 * t12
        t1704 = (t1699 - t1702) * t12
        t1708 = t990 - t1464
        t1741 = (t4 * t1683 * t49 - t774) * t49
        t1749 = ut(i,t45,t265,n)
        t1750 = t1749 - t1563
        t1752 = t1564 * t62
        t1755 = t1568 * t62
        t1757 = (t1752 - t1755) * t62
        t1761 = ut(i,t45,t278,n)
        t1762 = t1567 - t1761
        t1789 = t34 * (t776 + t1562 - t187 * ((t4 * ((t1697 * t12 - t169
     #9) * t12 - t1704) * t12 - t4 * (t1704 - (t1702 - t1708 * t12) * t1
     #2) * t12) * t12 + (((t4 * t1697 * t12 - t1557) * t12 - t1562) * t1
     #2 - (t1562 - (t1560 - t4 * t1708 * t12) * t12) * t12) * t12) / 0.2
     #4E2 - t222 * ((t4 * t1687 * t49 - t762) * t49 + ((t1741 - t776) * 
     #t49 - t778) * t49) / 0.24E2 + t1572 - t264 * ((t4 * ((t1750 * t62 
     #- t1752) * t62 - t1757) * t62 - t4 * (t1757 - (t1755 - t1762 * t62
     #) * t62) * t62) * t62 + (((t4 * t1750 * t62 - t1566) * t62 - t1572
     #) * t62 - (t1572 - (t1570 - t4 * t1762 * t62) * t62) * t62) * t62)
     # / 0.24E2 + t1576 + t1580)
        t1792 = dt * dy
        t1800 = (t4 * (t224 - t655) * t12 - t4 * (t655 - t1168) * t12) *
     # t12
        t1801 = u(i,t223,t58,n)
        t1805 = u(i,t223,t64,n)
        t1810 = (t4 * (t1801 - t655) * t62 - t4 * (t655 - t1805) * t62) 
     #* t62
        t1811 = src(i,t223,k,nComp,n)
        t1813 = (t1800 + t1603 + t1810 + t1811 - t891 - t680 - t901 - t9
     #64) * t49
        t1814 = t1550 * t49
        t1817 = t1792 * (t1813 / 0.2E1 + t1814 / 0.2E1)
        t1825 = t222 * (t756 - dy * (t1688 - t1689) / 0.12E2) / 0.12E2
        t1846 = (t4 * (t482 - t892) * t12 - t4 * (t892 - t1366) * t12) *
     # t12
        t1847 = t1801 - t892
        t1851 = (t4 * t1847 * t49 - t932) * t49
        t1862 = (t4 * (t486 - t896) * t12 - t4 * (t896 - t1370) * t12) *
     # t12
        t1863 = t1805 - t896
        t1867 = (t4 * t1863 * t49 - t948) * t49
        t1886 = src(i,t45,t58,nComp,n)
        t1890 = src(i,t45,t64,nComp,n)
        t1899 = t106 * ((t4 * (t481 + t250 + t491 - t891 - t680 - t901) 
     #* t12 - t4 * (t891 + t680 + t901 - t1365 - t1193 - t1375) * t12) *
     # t12 + (t4 * (t1800 + t1603 + t1810 - t891 - t680 - t901) * t49 - 
     #t904) * t49 + (t4 * (t1846 + t1851 + t1636 - t891 - t680 - t901) *
     # t62 - t4 * (t891 + t680 + t901 - t1862 - t1867 - t1642) * t62) * 
     #t62 + (t4 * (t566 - t964) * t12 - t4 * (t964 - t1438) * t12) * t12
     # + (t4 * (t1811 - t964) * t49 - t967) * t49 + (t4 * (t1886 - t964)
     # * t62 - t4 * (t964 - t1890) * t62) * t62 + (t1575 - t1579) * t136
     #)
        t1902 = t34 * dy
        t1911 = ut(i,t223,t58,n)
        t1915 = ut(i,t223,t64,n)
        t1931 = t1581 * t49
        t1934 = t1902 * (((t4 * (t367 - t751) * t12 - t4 * (t751 - t1225
     #) * t12) * t12 + t1741 + (t4 * (t1911 - t751) * t62 - t4 * (t751 -
     # t1915) * t62) * t62 + (src(i,t223,k,nComp,t133) - t1811) * t136 /
     # 0.2E1 + (t1811 - src(i,t223,k,nComp,t139)) * t136 / 0.2E1 - t1562
     # - t776 - t1572 - t1576 - t1580) * t49 / 0.2E1 + t1931 / 0.2E1)
        t1938 = t1792 * (t1813 - t1814)
        t1941 = t757 / 0.2E1
        t1942 = t768 * t49
        t1949 = dy * (t1681 + t1941 - t222 * (t1689 / 0.2E1 + t1942 / 0.
     #2E1) / 0.6E1) / 0.2E1
        t1950 = t77 + t87 + t97 + t98 - t909 - t686 - t919 - t968
        t1951 = t1950 * t49
        t1954 = t1792 * (t1814 / 0.2E1 + t1951 / 0.2E1)
        t1956 = t186 * t1954 / 0.2E1
        t1962 = t222 * (t759 - dy * (t1689 - t1942) / 0.12E2) / 0.12E2
        t1963 = t117 - t152
        t1965 = t4 * t1963 * t12
        t1966 = t152 - t994
        t1968 = t4 * t1966 * t12
        t1970 = (t1965 - t1968) * t12
        t1971 = ut(i,t51,t58,n)
        t1972 = t1971 - t152
        t1974 = t4 * t1972 * t62
        t1975 = ut(i,t51,t64,n)
        t1976 = t152 - t1975
        t1978 = t4 * t1976 * t62
        t1980 = (t1974 - t1978) * t62
        t1983 = (src(i,t51,k,nComp,t133) - t968) * t136
        t1984 = t1983 / 0.2E1
        t1987 = (t968 - src(i,t51,k,nComp,t139)) * t136
        t1988 = t1987 / 0.2E1
        t1989 = t147 + t157 + t167 + t171 + t175 - t1970 - t782 - t1980 
     #- t1984 - t1988
        t1990 = t1989 * t49
        t1993 = t1902 * (t1931 / 0.2E1 + t1990 / 0.2E1)
        t1995 = t327 * t1993 / 0.4E1
        t1997 = t1792 * (t1814 - t1951)
        t1999 = t186 * t1997 / 0.12E2
        t2000 = t148 + t186 * t1678 - t1696 + t327 * t1789 / 0.2E1 - t18
     #6 * t1817 / 0.2E1 + t1825 + t465 * t1899 / 0.6E1 - t327 * t1934 / 
     #0.4E1 + t186 * t1938 / 0.12E2 - t2 - t735 - t1949 - t847 - t1956 -
     # t1962 - t989 - t1995 - t1999
        t2004 = t1035 * t1548
        t2006 = t1038 * t1552 / 0.2E1
        t2008 = t1042 * t1583 / 0.6E1
        t2010 = t1045 * t1586 / 0.24E2
        t2023 = t1048 * t1954 / 0.2E1
        t2025 = t1050 * t1993 / 0.4E1
        t2027 = t1048 * t1997 / 0.12E2
        t2028 = t148 + t1048 * t1678 - t1696 + t1050 * t1789 / 0.2E1 - t
     #1048 * t1817 / 0.2E1 + t1825 + t1055 * t1899 / 0.6E1 - t1050 * t19
     #34 / 0.4E1 + t1048 * t1938 / 0.12E2 - t2 - t1062 - t1949 - t1064 -
     # t2023 - t1962 - t1068 - t2025 - t2027
        t2031 = cc * t2028 * t1031 / 0.8E1
        t2033 = (t8 * t1548 + t33 * t1552 / 0.2E1 + t105 * t1583 / 0.6E1
     # - t181 * t1586 / 0.24E2 + cc * t2000 * t1031 / 0.8E1 - t2004 - t2
     #006 - t2008 + t2010 - t2031) * t5
        t2039 = t4 * (t658 - dy * t664 / 0.24E2)
        t2041 = dy * t681 / 0.24E2
        t2049 = dt * (t757 - dy * t768 / 0.24E2)
        t2052 = t34 * t1950 * t49
        t2056 = t106 * t1989 * t49
        t2059 = dy * t783
        t2062 = j - 3
        t2064 = t667 - u(i,t2062,k,n)
        t2076 = (t684 - t4 * t2064 * t49) * t49
        t2085 = t498 * t12
        t2088 = t905 * t12
        t2090 = (t2085 - t2088) * t12
        t2111 = u(i,t51,t265,n)
        t2112 = t2111 - t910
        t2114 = t911 * t62
        t2117 = t915 * t62
        t2119 = (t2114 - t2117) * t62
        t2123 = u(i,t51,t278,n)
        t2124 = t914 - t2123
        t2136 = (t4 * t2112 * t62 - t913) * t62
        t2142 = (t917 - t4 * t2124 * t62) * t62
        t2151 = dt * (-t222 * ((t674 - t4 * (t671 - (t669 - t2064 * t49)
     # * t49) * t49) * t49 + (t688 - (t686 - t2076) * t49) * t49) / 0.24
     #E2 - t187 * ((t4 * ((t495 * t12 - t2085) * t12 - t2090) * t12 - t4
     # * (t2090 - (t2088 - t1379 * t12) * t12) * t12) * t12 + ((t502 - t
     #909) * t12 - (t909 - t1383) * t12) * t12) / 0.24E2 - t264 * ((t4 *
     # ((t2112 * t62 - t2114) * t62 - t2119) * t62 - t4 * (t2119 - (t211
     #7 - t2124 * t62) * t62) * t62) * t62 + ((t2136 - t919) * t62 - (t9
     #19 - t2142) * t62) * t62) / 0.24E2 + t909 + t686 + t919 + t968)
        t2155 = t763 - ut(i,t2062,k,n)
        t2159 = t767 - (t765 - t2155 * t49) * t49
        t2160 = t2159 * t49
        t2167 = dy * (t1941 + t765 / 0.2E1 - t222 * (t1942 / 0.2E1 + t21
     #60 / 0.2E1) / 0.6E1) / 0.2E1
        t2168 = t597 - t117
        t2170 = t1963 * t12
        t2173 = t1966 * t12
        t2175 = (t2170 - t2173) * t12
        t2179 = t994 - t1468
        t2205 = ut(i,t51,t265,n)
        t2206 = t2205 - t1971
        t2208 = t1972 * t62
        t2211 = t1976 * t62
        t2213 = (t2208 - t2211) * t62
        t2217 = ut(i,t51,t278,n)
        t2218 = t1975 - t2217
        t2251 = (t780 - t4 * t2155 * t49) * t49
        t2260 = t34 * (t1980 + t1970 - t187 * ((t4 * ((t2168 * t12 - t21
     #70) * t12 - t2175) * t12 - t4 * (t2175 - (t2173 - t2179 * t12) * t
     #12) * t12) * t12 + (((t4 * t2168 * t12 - t1965) * t12 - t1970) * t
     #12 - (t1970 - (t1968 - t4 * t2179 * t12) * t12) * t12) * t12) / 0.
     #24E2 - t264 * ((t4 * ((t2206 * t62 - t2208) * t62 - t2213) * t62 -
     # t4 * (t2213 - (t2211 - t2218 * t62) * t62) * t62) * t62 + (((t4 *
     # t2206 * t62 - t1974) * t62 - t1980) * t62 - (t1980 - (t1978 - t4 
     #* t2218 * t62) * t62) * t62) * t62) / 0.24E2 + t782 - t222 * ((t77
     #0 - t4 * t2159 * t49) * t49 + (t784 - (t782 - t2251) * t49) * t49)
     # / 0.24E2 + t1984 + t1988)
        t2270 = (t4 * (t237 - t667) * t12 - t4 * (t667 - t1180) * t12) *
     # t12
        t2271 = u(i,t236,t58,n)
        t2275 = u(i,t236,t64,n)
        t2280 = (t4 * (t2271 - t667) * t62 - t4 * (t667 - t2275) * t62) 
     #* t62
        t2281 = src(i,t236,k,nComp,n)
        t2283 = (t909 + t686 + t919 + t968 - t2270 - t2076 - t2280 - t22
     #81) * t49
        t2286 = t1792 * (t1951 / 0.2E1 + t2283 / 0.2E1)
        t2294 = t222 * (t767 - dy * (t1942 - t2160) / 0.12E2) / 0.12E2
        t2315 = (t4 * (t503 - t910) * t12 - t4 * (t910 - t1384) * t12) *
     # t12
        t2316 = t910 - t2271
        t2320 = (t935 - t4 * t2316 * t49) * t49
        t2331 = (t4 * (t507 - t914) * t12 - t4 * (t914 - t1388) * t12) *
     # t12
        t2332 = t914 - t2275
        t2336 = (t951 - t4 * t2332 * t49) * t49
        t2355 = src(i,t51,t58,nComp,n)
        t2359 = src(i,t51,t64,nComp,n)
        t2368 = t106 * ((t4 * (t502 + t256 + t512 - t909 - t686 - t919) 
     #* t12 - t4 * (t909 + t686 + t919 - t1383 - t1199 - t1393) * t12) *
     # t12 + (t922 - t4 * (t909 + t686 + t919 - t2270 - t2076 - t2280) *
     # t49) * t49 + (t4 * (t2315 + t2320 + t2136 - t909 - t686 - t919) *
     # t62 - t4 * (t909 + t686 + t919 - t2331 - t2336 - t2142) * t62) * 
     #t62 + (t4 * (t570 - t968) * t12 - t4 * (t968 - t1442) * t12) * t12
     # + (t971 - t4 * (t968 - t2281) * t49) * t49 + (t4 * (t2355 - t968)
     # * t62 - t4 * (t968 - t2359) * t62) * t62 + (t1983 - t1987) * t136
     #)
        t2379 = ut(i,t236,t58,n)
        t2383 = ut(i,t236,t64,n)
        t2401 = t1902 * (t1990 / 0.2E1 + (t1970 + t782 + t1980 + t1984 +
     # t1988 - (t4 * (t379 - t763) * t12 - t4 * (t763 - t1237) * t12) * 
     #t12 - t2251 - (t4 * (t2379 - t763) * t62 - t4 * (t763 - t2383) * t
     #62) * t62 - (src(i,t236,k,nComp,t133) - t2281) * t136 / 0.2E1 - (t
     #2281 - src(i,t236,k,nComp,t139)) * t136 / 0.2E1) * t49 / 0.2E1)
        t2405 = t1792 * (t1951 - t2283)
        t2408 = t2 + t735 - t1949 + t847 - t1956 + t1962 + t989 - t1995 
     #+ t1999 - t152 - t186 * t2151 - t2167 - t327 * t2260 / 0.2E1 - t18
     #6 * t2286 / 0.2E1 - t2294 - t465 * t2368 / 0.6E1 - t327 * t2401 / 
     #0.4E1 - t186 * t2405 / 0.12E2
        t2412 = t1035 * t2049
        t2414 = t1038 * t2052 / 0.2E1
        t2416 = t1042 * t2056 / 0.6E1
        t2418 = t1045 * t2059 / 0.24E2
        t2430 = t2 + t1062 - t1949 + t1064 - t2023 + t1962 + t1068 - t20
     #25 + t2027 - t152 - t1048 * t2151 - t2167 - t1050 * t2260 / 0.2E1 
     #- t1048 * t2286 / 0.2E1 - t2294 - t1055 * t2368 / 0.6E1 - t1050 * 
     #t2401 / 0.4E1 - t1048 * t2405 / 0.12E2
        t2433 = cc * t2430 * t1031 / 0.8E1
        t2435 = (t8 * t2049 + t33 * t2052 / 0.2E1 + t105 * t2056 / 0.6E1
     # - t181 * t2059 / 0.24E2 + cc * t2408 * t1031 / 0.8E1 - t2412 - t2
     #414 - t2416 + t2418 - t2433) * t5
        t2441 = t4 * (t661 - dy * t672 / 0.24E2)
        t2443 = dy * t687 / 0.24E2
        t2453 = dt * (t808 - dz * t814 / 0.24E2)
        t2455 = t929 + t937 + t719 + t974 - t77 - t87 - t97 - t98
        t2457 = t34 * t2455 * t62
        t2460 = t123 - t158
        t2462 = t4 * t2460 * t12
        t2463 = t158 - t1000
        t2465 = t4 * t2463 * t12
        t2467 = (t2462 - t2465) * t12
        t2468 = t1563 - t158
        t2470 = t4 * t2468 * t49
        t2471 = t158 - t1971
        t2473 = t4 * t2471 * t49
        t2475 = (t2470 - t2473) * t49
        t2478 = (src(i,j,t58,nComp,t133) - t974) * t136
        t2479 = t2478 / 0.2E1
        t2482 = (t974 - src(i,j,t58,nComp,t139)) * t136
        t2483 = t2482 / 0.2E1
        t2484 = t2467 + t2475 + t830 + t2479 + t2483 - t147 - t157 - t16
     #7 - t171 - t175
        t2486 = t106 * t2484 * t62
        t2489 = dz * t831
        t2493 = t521 * t12
        t2496 = t925 * t12
        t2498 = (t2493 - t2496) * t12
        t2520 = t930 * t49
        t2523 = t933 * t49
        t2525 = (t2520 - t2523) * t49
        t2546 = k + 3
        t2548 = u(i,j,t2546,n) - t694
        t2560 = (t4 * t2548 * t62 - t717) * t62
        t2569 = dt * (-t187 * ((t4 * ((t518 * t12 - t2493) * t12 - t2498
     #) * t12 - t4 * (t2498 - (t2496 - t1399 * t12) * t12) * t12) * t12 
     #+ ((t525 - t929) * t12 - (t929 - t1403) * t12) * t12) / 0.24E2 - t
     #222 * ((t4 * ((t1847 * t49 - t2520) * t49 - t2525) * t49 - t4 * (t
     #2525 - (t2523 - t2316 * t49) * t49) * t49) * t49 + ((t1851 - t937)
     # * t49 - (t937 - t2320) * t49) * t49) / 0.24E2 - t264 * ((t4 * ((t
     #2548 * t62 - t696) * t62 - t699) * t62 - t705) * t62 + ((t2560 - t
     #719) * t62 - t721) * t62) / 0.24E2 + t929 + t937 + t719 + t974)
        t2572 = t808 / 0.2E1
        t2574 = ut(i,j,t2546,n) - t805
        t2578 = (t2574 * t62 - t807) * t62 - t810
        t2579 = t2578 * t62
        t2580 = t814 * t62
        t2587 = dz * (t807 / 0.2E1 + t2572 - t264 * (t2579 / 0.2E1 + t25
     #80 / 0.2E1) / 0.6E1) / 0.2E1
        t2588 = t603 - t123
        t2590 = t2460 * t12
        t2593 = t2463 * t12
        t2595 = (t2590 - t2593) * t12
        t2599 = t1000 - t1474
        t2625 = t1911 - t1563
        t2627 = t2468 * t49
        t2630 = t2471 * t49
        t2632 = (t2627 - t2630) * t49
        t2636 = t1971 - t2379
        t2669 = (t4 * t2574 * t62 - t828) * t62
        t2678 = t34 * (t2467 - t187 * ((t4 * ((t2588 * t12 - t2590) * t1
     #2 - t2595) * t12 - t4 * (t2595 - (t2593 - t2599 * t12) * t12) * t1
     #2) * t12 + (((t4 * t2588 * t12 - t2462) * t12 - t2467) * t12 - (t2
     #467 - (t2465 - t4 * t2599 * t12) * t12) * t12) * t12) / 0.24E2 + t
     #2475 - t222 * ((t4 * ((t2625 * t49 - t2627) * t49 - t2632) * t49 -
     # t4 * (t2632 - (t2630 - t2636 * t49) * t49) * t49) * t49 + (((t4 *
     # t2625 * t49 - t2470) * t49 - t2475) * t49 - (t2475 - (t2473 - t4 
     #* t2636 * t49) * t49) * t49) * t49) / 0.24E2 + t830 - t264 * ((t4 
     #* t2578 * t62 - t816) * t62 + ((t2669 - t830) * t62 - t832) * t62)
     # / 0.24E2 + t2479 + t2483)
        t2681 = dt * dz
        t2689 = (t4 * (t266 - t694) * t12 - t4 * (t694 - t1107) * t12) *
     # t12
        t2697 = (t4 * (t1611 - t694) * t49 - t4 * (t694 - t2111) * t49) 
     #* t49
        t2698 = src(i,j,t265,nComp,n)
        t2700 = (t2689 + t2697 + t2560 + t2698 - t929 - t937 - t719 - t9
     #74) * t62
        t2701 = t2455 * t62
        t2704 = t2681 * (t2700 / 0.2E1 + t2701 / 0.2E1)
        t2712 = t264 * (t810 - dz * (t2579 - t2580) / 0.12E2) / 0.12E2
        t2758 = t106 * ((t4 * (t525 + t533 + t292 - t929 - t937 - t719) 
     #* t12 - t4 * (t929 + t937 + t719 - t1403 - t1411 - t1132) * t12) *
     # t12 + (t4 * (t1846 + t1851 + t1636 - t929 - t937 - t719) * t49 - 
     #t4 * (t929 + t937 + t719 - t2315 - t2320 - t2136) * t49) * t49 + (
     #t4 * (t2689 + t2697 + t2560 - t929 - t937 - t719) * t62 - t940) * 
     #t62 + (t4 * (t576 - t974) * t12 - t4 * (t974 - t1448) * t12) * t12
     # + (t4 * (t1886 - t974) * t49 - t4 * (t974 - t2355) * t49) * t49 +
     # (t4 * (t2698 - t974) * t62 - t977) * t62 + (t2478 - t2482) * t136
     #)
        t2761 = t34 * dz
        t2788 = t2484 * t62
        t2791 = t2761 * (((t4 * (t328 - t805) * t12 - t4 * (t805 - t1264
     #) * t12) * t12 + (t4 * (t1749 - t805) * t49 - t4 * (t805 - t2205) 
     #* t49) * t49 + t2669 + (src(i,j,t265,nComp,t133) - t2698) * t136 /
     # 0.2E1 + (t2698 - src(i,j,t265,nComp,t139)) * t136 / 0.2E1 - t2467
     # - t2475 - t830 - t2479 - t2483) * t62 / 0.2E1 + t2788 / 0.2E1)
        t2795 = t2681 * (t2700 - t2701)
        t2798 = t811 / 0.2E1
        t2799 = t822 * t62
        t2806 = dz * (t2572 + t2798 - t264 * (t2580 / 0.2E1 + t2799 / 0.
     #2E1) / 0.6E1) / 0.2E1
        t2807 = t77 + t87 + t97 + t98 - t945 - t953 - t725 - t978
        t2808 = t2807 * t62
        t2811 = t2681 * (t2701 / 0.2E1 + t2808 / 0.2E1)
        t2813 = t186 * t2811 / 0.2E1
        t2819 = t264 * (t813 - dz * (t2580 - t2799) / 0.12E2) / 0.12E2
        t2820 = t127 - t162
        t2822 = t4 * t2820 * t12
        t2823 = t162 - t1004
        t2825 = t4 * t2823 * t12
        t2827 = (t2822 - t2825) * t12
        t2828 = t1567 - t162
        t2830 = t4 * t2828 * t49
        t2831 = t162 - t1975
        t2833 = t4 * t2831 * t49
        t2835 = (t2830 - t2833) * t49
        t2838 = (src(i,j,t64,nComp,t133) - t978) * t136
        t2839 = t2838 / 0.2E1
        t2842 = (t978 - src(i,j,t64,nComp,t139)) * t136
        t2843 = t2842 / 0.2E1
        t2844 = t147 + t157 + t167 + t171 + t175 - t2827 - t2835 - t836 
     #- t2839 - t2843
        t2845 = t2844 * t62
        t2848 = t2761 * (t2788 / 0.2E1 + t2845 / 0.2E1)
        t2850 = t327 * t2848 / 0.4E1
        t2852 = t2681 * (t2701 - t2808)
        t2854 = t186 * t2852 / 0.12E2
        t2855 = t158 + t186 * t2569 - t2587 + t327 * t2678 / 0.2E1 - t18
     #6 * t2704 / 0.2E1 + t2712 + t465 * t2758 / 0.6E1 - t327 * t2791 / 
     #0.4E1 + t186 * t2795 / 0.12E2 - t2 - t735 - t2806 - t847 - t2813 -
     # t2819 - t989 - t2850 - t2854
        t2859 = t1035 * t2453
        t2861 = t1038 * t2457 / 0.2E1
        t2863 = t1042 * t2486 / 0.6E1
        t2865 = t1045 * t2489 / 0.24E2
        t2878 = t1048 * t2811 / 0.2E1
        t2880 = t1050 * t2848 / 0.4E1
        t2882 = t1048 * t2852 / 0.12E2
        t2883 = t158 + t1048 * t2569 - t2587 + t1050 * t2678 / 0.2E1 - t
     #1048 * t2704 / 0.2E1 + t2712 + t1055 * t2758 / 0.6E1 - t1050 * t27
     #91 / 0.4E1 + t1048 * t2795 / 0.12E2 - t2 - t1062 - t2806 - t1064 -
     # t2878 - t2819 - t1068 - t2880 - t2882
        t2886 = cc * t2883 * t1031 / 0.8E1
        t2888 = (t8 * t2453 + t33 * t2457 / 0.2E1 + t105 * t2486 / 0.6E1
     # - t181 * t2489 / 0.24E2 + cc * t2855 * t1031 / 0.8E1 - t2859 - t2
     #861 - t2863 + t2865 - t2886) * t5
        t2894 = t4 * (t697 - dz * t703 / 0.24E2)
        t2896 = dz * t720 / 0.24E2
        t2904 = dt * (t811 - dz * t822 / 0.24E2)
        t2907 = t34 * t2807 * t62
        t2911 = t106 * t2844 * t62
        t2914 = dz * t837
        t2918 = t540 * t12
        t2921 = t941 * t12
        t2923 = (t2918 - t2921) * t12
        t2944 = k - 3
        t2946 = t706 - u(i,j,t2944,n)
        t2958 = (t723 - t4 * t2946 * t62) * t62
        t2967 = t946 * t49
        t2970 = t949 * t49
        t2972 = (t2967 - t2970) * t49
        t2994 = dt * (-t187 * ((t4 * ((t537 * t12 - t2918) * t12 - t2923
     #) * t12 - t4 * (t2923 - (t2921 - t1415 * t12) * t12) * t12) * t12 
     #+ ((t544 - t945) * t12 - (t945 - t1419) * t12) * t12) / 0.24E2 + t
     #725 - t264 * ((t713 - t4 * (t710 - (t708 - t2946 * t62) * t62) * t
     #62) * t62 + (t727 - (t725 - t2958) * t62) * t62) / 0.24E2 + t953 -
     # t222 * ((t4 * ((t1863 * t49 - t2967) * t49 - t2972) * t49 - t4 * 
     #(t2972 - (t2970 - t2332 * t49) * t49) * t49) * t49 + ((t1867 - t95
     #3) * t49 - (t953 - t2336) * t49) * t49) / 0.24E2 + t945 + t978)
        t2998 = t817 - ut(i,j,t2944,n)
        t3002 = t821 - (t819 - t2998 * t62) * t62
        t3003 = t3002 * t62
        t3010 = dz * (t2798 + t819 / 0.2E1 - t264 * (t2799 / 0.2E1 + t30
     #03 / 0.2E1) / 0.6E1) / 0.2E1
        t3011 = t1915 - t1567
        t3013 = t2828 * t49
        t3016 = t2831 * t49
        t3018 = (t3013 - t3016) * t49
        t3022 = t1975 - t2383
        t3055 = (t834 - t4 * t2998 * t62) * t62
        t3063 = t607 - t127
        t3065 = t2820 * t12
        t3068 = t2823 * t12
        t3070 = (t3065 - t3068) * t12
        t3074 = t1004 - t1478
        t3101 = t34 * (-t222 * ((t4 * ((t3011 * t49 - t3013) * t49 - t30
     #18) * t49 - t4 * (t3018 - (t3016 - t3022 * t49) * t49) * t49) * t4
     #9 + (((t4 * t3011 * t49 - t2830) * t49 - t2835) * t49 - (t2835 - (
     #t2833 - t4 * t3022 * t49) * t49) * t49) * t49) / 0.24E2 + t836 - t
     #264 * ((t824 - t4 * t3002 * t62) * t62 + (t838 - (t836 - t3055) * 
     #t62) * t62) / 0.24E2 + t2835 + t2827 - t187 * ((t4 * ((t3063 * t12
     # - t3065) * t12 - t3070) * t12 - t4 * (t3070 - (t3068 - t3074 * t1
     #2) * t12) * t12) * t12 + (((t4 * t3063 * t12 - t2822) * t12 - t282
     #7) * t12 - (t2827 - (t2825 - t4 * t3074 * t12) * t12) * t12) * t12
     #) / 0.24E2 + t2839 + t2843)
        t3111 = (t4 * (t279 - t706) * t12 - t4 * (t706 - t1119) * t12) *
     # t12
        t3119 = (t4 * (t1623 - t706) * t49 - t4 * (t706 - t2123) * t49) 
     #* t49
        t3120 = src(i,j,t278,nComp,n)
        t3122 = (t945 + t953 + t725 + t978 - t3111 - t3119 - t2958 - t31
     #20) * t62
        t3125 = t2681 * (t2808 / 0.2E1 + t3122 / 0.2E1)
        t3133 = t264 * (t821 - dz * (t2799 - t3003) / 0.12E2) / 0.12E2
        t3179 = t106 * ((t4 * (t544 + t552 + t298 - t945 - t953 - t725) 
     #* t12 - t4 * (t945 + t953 + t725 - t1419 - t1427 - t1138) * t12) *
     # t12 + (t4 * (t1862 + t1867 + t1642 - t945 - t953 - t725) * t49 - 
     #t4 * (t945 + t953 + t725 - t2331 - t2336 - t2142) * t49) * t49 + (
     #t956 - t4 * (t945 + t953 + t725 - t3111 - t3119 - t2958) * t62) * 
     #t62 + (t4 * (t580 - t978) * t12 - t4 * (t978 - t1452) * t12) * t12
     # + (t4 * (t1890 - t978) * t49 - t4 * (t978 - t2359) * t49) * t49 +
     # (t981 - t4 * (t978 - t3120) * t62) * t62 + (t2838 - t2842) * t136
     #)
        t3210 = t2761 * (t2845 / 0.2E1 + (t2827 + t2835 + t836 + t2839 +
     # t2843 - (t4 * (t340 - t817) * t12 - t4 * (t817 - t1276) * t12) * 
     #t12 - (t4 * (t1761 - t817) * t49 - t4 * (t817 - t2217) * t49) * t4
     #9 - t3055 - (src(i,j,t278,nComp,t133) - t3120) * t136 / 0.2E1 - (t
     #3120 - src(i,j,t278,nComp,t139)) * t136 / 0.2E1) * t62 / 0.2E1)
        t3214 = t2681 * (t2808 - t3122)
        t3217 = t2 + t735 - t2806 + t847 - t2813 + t2819 + t989 - t2850 
     #+ t2854 - t162 - t186 * t2994 - t3010 - t327 * t3101 / 0.2E1 - t18
     #6 * t3125 / 0.2E1 - t3133 - t465 * t3179 / 0.6E1 - t327 * t3210 / 
     #0.4E1 - t186 * t3214 / 0.12E2
        t3221 = t1035 * t2904
        t3223 = t1038 * t2907 / 0.2E1
        t3225 = t1042 * t2911 / 0.6E1
        t3227 = t1045 * t2914 / 0.24E2
        t3239 = t2 + t1062 - t2806 + t1064 - t2878 + t2819 + t1068 - t28
     #80 + t2882 - t162 - t1048 * t2994 - t3010 - t1050 * t3101 / 0.2E1 
     #- t1048 * t3125 / 0.2E1 - t3133 - t1055 * t3179 / 0.6E1 - t1050 * 
     #t3210 / 0.4E1 - t1048 * t3214 / 0.12E2
        t3242 = cc * t3239 * t1031 / 0.8E1
        t3244 = (t8 * t2904 + t33 * t2907 / 0.2E1 + t105 * t2911 / 0.6E1
     # - t181 * t2914 / 0.24E2 + cc * t3217 * t1031 / 0.8E1 - t3221 - t3
     #223 - t3225 + t3227 - t3242) * t5
        t3250 = t4 * (t700 - dz * t711 / 0.24E2)
        t3252 = dz * t726 / 0.24E2
        t3262 = src(i,j,k,nComp,n + 2)
        t3264 = (src(i,j,k,nComp,n + 3) - t3262) * t5


        unew(i,j,k) = t1 + dt * t2 + (t1078 * t34 / 0.6E1 + (t1084 + 
     #t1036 + t1040 - t1086 + t1044 - t1047 + t1076 - t1078 * t1034) * t
     #34 / 0.2E1 - t1530 * t34 / 0.6E1 - (t1536 + t1507 + t1509 - t1538 
     #+ t1511 - t1513 + t1528 - t1530 * t1034) * t34 / 0.2E1) * t12 + (t
     #2033 * t34 / 0.6E1 + (t2039 + t2004 + t2006 - t2041 + t2008 - t201
     #0 + t2031 - t2033 * t1034) * t34 / 0.2E1 - t2435 * t34 / 0.6E1 - (
     #t2441 + t2412 + t2414 - t2443 + t2416 - t2418 + t2433 - t2435 * t1
     #034) * t34 / 0.2E1) * t49 + (t2888 * t34 / 0.6E1 + (t2894 + t2859 
     #+ t2861 - t2896 + t2863 - t2865 + t2886 - t2888 * t1034) * t34 / 0
     #.2E1 - t3244 * t34 / 0.6E1 - (t3250 + t3221 + t3223 - t3252 + t322
     #5 - t3227 + t3242 - t3244 * t1034) * t34 / 0.2E1) * t62 + t3264 * 
     #t34 / 0.6E1 + (t3262 - t3264 * t1034) * t34 / 0.2E1

        utnew(i,j,k) = t2 + (t10
     #78 * dt / 0.2E1 + (t1084 + t1036 + t1040 - t1086 + t1044 - t1047 +
     # t1076) * dt - t1078 * t1045 - t1530 * dt / 0.2E1 - (t1536 + t1507
     # + t1509 - t1538 + t1511 - t1513 + t1528) * dt + t1530 * t1045) * 
     #t12 + (t2033 * dt / 0.2E1 + (t2039 + t2004 + t2006 - t2041 + t2008
     # - t2010 + t2031) * dt - t2033 * t1045 - t2435 * dt / 0.2E1 - (t24
     #41 + t2412 + t2414 - t2443 + t2416 - t2418 + t2433) * dt + t2435 *
     # t1045) * t49 + (t2888 * dt / 0.2E1 + (t2894 + t2859 + t2861 - t28
     #96 + t2863 - t2865 + t2886) * dt - t2888 * t1045 - t3244 * dt / 0.
     #2E1 - (t3250 + t3221 + t3223 - t3252 + t3225 - t3227 + t3242) * dt
     # + t3244 * t1045) * t62 + t3264 * dt / 0.2E1 + t3262 * dt - t3264 
     #* t1045

c        blah = array(int(t1 + dt * t2 + (t1078 * t34 / 0.6E1 + (t1084 + 
c     #t1036 + t1040 - t1086 + t1044 - t1047 + t1076 - t1078 * t1034) * t
c     #34 / 0.2E1 - t1530 * t34 / 0.6E1 - (t1536 + t1507 + t1509 - t1538 
c     #+ t1511 - t1513 + t1528 - t1530 * t1034) * t34 / 0.2E1) * t12 + (t
c     #2033 * t34 / 0.6E1 + (t2039 + t2004 + t2006 - t2041 + t2008 - t201
c     #0 + t2031 - t2033 * t1034) * t34 / 0.2E1 - t2435 * t34 / 0.6E1 - (
c     #t2441 + t2412 + t2414 - t2443 + t2416 - t2418 + t2433 - t2435 * t1
c     #034) * t34 / 0.2E1) * t49 + (t2888 * t34 / 0.6E1 + (t2894 + t2859 
c     #+ t2861 - t2896 + t2863 - t2865 + t2886 - t2888 * t1034) * t34 / 0
c     #.2E1 - t3244 * t34 / 0.6E1 - (t3250 + t3221 + t3223 - t3252 + t322
c     #5 - t3227 + t3242 - t3244 * t1034) * t34 / 0.2E1) * t62 + t3264 * 
c     #t34 / 0.6E1 + (t3262 - t3264 * t1034) * t34 / 0.2E1),int(t2 + (t10
c     #78 * dt / 0.2E1 + (t1084 + t1036 + t1040 - t1086 + t1044 - t1047 +
c     # t1076) * dt - t1078 * t1045 - t1530 * dt / 0.2E1 - (t1536 + t1507
c     # + t1509 - t1538 + t1511 - t1513 + t1528) * dt + t1530 * t1045) * 
c     #t12 + (t2033 * dt / 0.2E1 + (t2039 + t2004 + t2006 - t2041 + t2008
c     # - t2010 + t2031) * dt - t2033 * t1045 - t2435 * dt / 0.2E1 - (t24
c     #41 + t2412 + t2414 - t2443 + t2416 - t2418 + t2433) * dt + t2435 *
c     # t1045) * t49 + (t2888 * dt / 0.2E1 + (t2894 + t2859 + t2861 - t28
c     #96 + t2863 - t2865 + t2886) * dt - t2888 * t1045 - t3244 * dt / 0.
c     #2E1 - (t3250 + t3221 + t3223 - t3252 + t3225 - t3227 + t3242) * dt
c     # + t3244 * t1045) * t62 + t3264 * dt / 0.2E1 + t3262 * dt - t3264 
c     #* t1045))

        return
      end
