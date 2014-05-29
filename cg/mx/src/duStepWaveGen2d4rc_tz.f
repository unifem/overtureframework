      subroutine duStepWaveGen2d4rc_tz( 
     *   nd1a,nd1b,nd2a,nd2b,
     *   n1a,n1b,n2a,n2b,
     *   ndf4a,ndf4b,nComp,
     *   u,ut,unew,utnew,
     *   src,
     *   dx,dy,dt,cc,beta,
     *   i,j,n )

      implicit none
c
c.. declarations of incoming variables      
      integer nd1a,nd1b,nd2a,nd2b
      integer n1a,n1b,n2a,n2b
      integer ndf4a,ndf4b,nComp
      integer i,j,n

      real u    (nd1a:nd1b,nd2a:nd2b,*)
      real ut   (nd1a:nd1b,nd2a:nd2b,*)
      real unew (nd1a:nd1b,nd2a:nd2b)
      real utnew(nd1a:nd1b,nd2a:nd2b)
      real src  (nd1a:nd1b,nd2a:nd2b,ndf4a:ndf4b,0:*)
      real dx,dy,dt,cc,beta
c
c.. generated code to follow
c
       real t1
        real t10
        integer t100
        real t1000
        real t1010
        real t1012
        real t1014
        real t1017
        real t1019
        real t1020
        real t1022
        real t1024
        real t1027
        real t1028
        real t103
        real t1031
        real t1032
        real t1033
        real t1035
        real t1038
        real t104
        real t1042
        real t1045
        real t1047
        real t105
        integer t106
        integer t1068
        real t1070
        real t1082
        real t109
        real t1091
        real t1094
        real t1096
        real t11
        real t110
        real t1100
        real t1101
        real t1102
        real t1109
        real t1110
        real t1112
        real t1115
        real t1117
        real t112
        real t1121
        real t114
        real t115
        real t1154
        real t116
        real t1163
        real t1166
        real t1174
        real t1175
        real t1177
        real t1178
        real t118
        real t1181
        real t1189
        real t119
        real t12
        real t120
        real t1219
        real t122
        real t1222
        real t124
        real t1241
        real t1244
        real t1248
        real t1251
        real t1252
        real t1259
        real t1260
        real t1261
        real t1264
        real t1266
        real t127
        real t1272
        real t1273
        real t1275
        real t1276
        real t1278
        real t128
        real t1280
        real t1283
        real t1284
        real t1287
        real t1288
        real t1289
        real t1290
        real t1293
        real t1295
        real t1297
        real t1299
        real t13
        real t1300
        real t1304
        real t1306
        real t1308
        real t131
        real t1310
        real t132
        real t1323
        real t1325
        real t1327
        real t1328
        real t133
        real t1331
        real t1333
        real t1339
        real t1341
        real t1349
        real t135
        real t1352
        real t1356
        real t1359
        real t1363
        real t1366
        real t1368
        real t138
        integer t1389
        real t139
        real t1391
        integer t14
        real t140
        real t1403
        real t1412
        real t1416
        real t1420
        real t1421
        real t1428
        real t1429
        real t143
        real t1431
        real t1434
        real t1436
        real t144
        real t1440
        integer t145
        real t147
        real t1473
        real t1482
        real t149
        real t1492
        real t1493
        real t1495
        real t1498
        real t15
        real t1506
        real t152
        real t1536
        real t154
        real t1559
        real t1563
        real t1566
        real t1570
        real t1572
        real t1574
        real t1576
        real t158
        real t1588
        real t1591
        real t1593
        real t1599
        real t16
        real t160
        real t1601
        real t161
        real t1611
        real t1613
        real t163
        real t169
        real t17
        real t172
        real t173
        real t179
        integer t180
        real t181
        real t182
        real t184
        real t187
        real t189
        real t19
        integer t193
        real t194
        real t195
        real t2
        integer t20
        real t207
        real t21
        real t213
        real t22
        real t222
        real t225
        real t227
        real t23
        real t231
        real t232
        real t233
        real t240
        real t241
        real t242
        real t246
        real t25
        real t252
        real t255
        real t26
        real t261
        real t262
        real t264
        real t267
        real t269
        real t273
        real t274
        real t30
        real t301
        real t304
        real t305
        real t309
        real t314
        real t315
        real t317
        real t318
        real t32
        real t321
        real t329
        real t33
        real t330
        real t331
        real t337
        real t34
        real t340
        real t343
        real t345
        real t347
        real t35
        real t351
        real t354
        real t356
        real t358
        real t36
        real t369
        real t37
        real t372
        real t376
        real t385
        real t388
        real t389
        real t39
        real t393
        real t4
        real t40
        real t409
        real t412
        real t416
        integer t419
        real t42
        real t420
        real t421
        real t422
        real t424
        real t425
        real t427
        real t431
        real t433
        real t434
        real t435
        real t44
        real t441
        real t442
        real t443
        real t444
        real t446
        real t447
        real t449
        integer t45
        real t450
        real t452
        real t453
        real t454
        real t455
        real t457
        real t458
        real t46
        real t460
        real t464
        real t466
        real t467
        real t468
        real t47
        real t470
        real t472
        real t473
        real t474
        real t481
        real t482
        real t483
        real t484
        real t485
        real t486
        real t488
        real t489
        real t49
        real t490
        real t497
        real t499
        real t5
        real t50
        real t503
        real t505
        real t506
        real t507
        integer t51
        real t513
        real t514
        real t515
        real t516
        real t518
        real t519
        real t52
        real t521
        real t522
        real t524
        real t525
        real t526
        real t527
        real t529
        real t53
        real t530
        real t532
        real t536
        real t538
        real t539
        real t540
        real t542
        real t544
        real t545
        real t546
        real t55
        real t553
        real t555
        real t556
        real t557
        real t559
        real t560
        real t561
        real t563
        real t565
        real t566
        real t567
        real t568
        real t57
        real t571
        real t573
        real t579
        real t58
        real t582
        real t585
        real t587
        real t589
        real t59
        real t592
        real t593
        real t595
        real t597
        real t6
        real t60
        real t600
        real t605
        real t608
        real t611
        real t612
        real t615
        real t62
        real t621
        real t623
        real t624
        real t625
        real t627
        real t628
        real t629
        real t631
        real t633
        real t636
        real t637
        real t64
        real t640
        real t641
        real t642
        real t643
        real t646
        real t648
        real t65
        real t650
        real t652
        real t653
        real t655
        real t658
        real t659
        real t66
        real t660
        real t661
        real t662
        real t664
        real t665
        real t666
        real t668
        real t669
        real t671
        real t672
        real t674
        real t679
        real t68
        real t686
        real t688
        real t69
        real t690
        real t692
        real t694
        real t696
        real t697
        real t7
        real t70
        real t700
        real t702
        real t708
        real t710
        real t718
        real t72
        real t721
        real t725
        real t728
        integer t731
        real t733
        real t74
        real t745
        real t75
        real t753
        real t754
        real t756
        real t759
        real t76
        real t761
        real t765
        real t766
        real t778
        real t78
        real t784
        real t793
        real t797
        real t8
        real t801
        real t802
        real t809
        real t81
        real t817
        real t82
        real t825
        real t826
        real t828
        real t83
        real t831
        real t833
        real t837
        real t838
        real t85
        real t865
        real t868
        real t87
        real t872
        real t877
        real t878
        real t880
        real t883
        real t89
        real t891
        real t897
        integer t9
        real t90
        real t901
        real t905
        real t909
        real t91
        real t920
        real t924
        real t93
        real t933
        real t936
        real t94
        real t940
        real t95
        real t958
        real t962
        real t965
        real t969
        real t97
        real t971
        real t973
        real t975
        real t987
        real t99
        real t990
        real t992
        real t998
        t1 = u(i,j,n)
        t2 = ut(i,j,n)
        t4 = cc ** 2
        t5 = sqrt(0.3E1)
        t6 = t5 / 0.6E1
        t7 = 0.1E1 / 0.2E1 + t6
        t8 = t4 * t7
        t9 = i + 1
        t10 = ut(t9,j,n)
        t11 = t10 - t2
        t12 = 0.1E1 / dx
        t13 = t11 * t12
        t14 = i + 2
        t15 = ut(t14,j,n)
        t16 = t15 - t10
        t17 = t16 * t12
        t19 = (t17 - t13) * t12
        t20 = i - 1
        t21 = ut(t20,j,n)
        t22 = t2 - t21
        t23 = t22 * t12
        t25 = (t13 - t23) * t12
        t26 = t19 - t25
        t30 = dt * (t13 - dx * t26 / 0.24E2)
        t32 = t7 ** 2
        t33 = t4 * t32
        t34 = dt ** 2
        t35 = u(t14,j,n)
        t36 = u(t9,j,n)
        t37 = t35 - t36
        t39 = t4 * t37 * t12
        t40 = t36 - t1
        t42 = t4 * t40 * t12
        t44 = (t39 - t42) * t12
        t45 = j + 1
        t46 = u(t9,t45,n)
        t47 = t46 - t36
        t49 = 0.1E1 / dy
        t50 = t4 * t47 * t49
        t51 = j - 1
        t52 = u(t9,t51,n)
        t53 = t36 - t52
        t55 = t4 * t53 * t49
        t57 = (t50 - t55) * t49
        t58 = src(t9,j,nComp,n)
        t59 = u(t20,j,n)
        t60 = t1 - t59
        t62 = t4 * t60 * t12
        t64 = (t42 - t62) * t12
        t65 = u(i,t45,n)
        t66 = t65 - t1
        t68 = t4 * t66 * t49
        t69 = u(i,t51,n)
        t70 = t1 - t69
        t72 = t4 * t70 * t49
        t74 = (t68 - t72) * t49
        t75 = src(i,j,nComp,n)
        t76 = t44 + t57 + t58 - t64 - t74 - t75
        t78 = t34 * t76 * t12
        t81 = t32 * t7
        t82 = t4 * t81
        t83 = t34 * dt
        t85 = t4 * t16 * t12
        t87 = t4 * t11 * t12
        t89 = (t85 - t87) * t12
        t90 = ut(t9,t45,n)
        t91 = t90 - t10
        t93 = t4 * t91 * t49
        t94 = ut(t9,t51,n)
        t95 = t10 - t94
        t97 = t4 * t95 * t49
        t99 = (t93 - t97) * t49
        t100 = n + 1
        t103 = 0.1E1 / dt
        t104 = (src(t9,j,nComp,t100) - t58) * t103
        t105 = t104 / 0.2E1
        t106 = n - 1
        t109 = (t58 - src(t9,j,nComp,t106)) * t103
        t110 = t109 / 0.2E1
        t112 = t4 * t22 * t12
        t114 = (t87 - t112) * t12
        t115 = ut(i,t45,n)
        t116 = t115 - t2
        t118 = t4 * t116 * t49
        t119 = ut(i,t51,n)
        t120 = t2 - t119
        t122 = t4 * t120 * t49
        t124 = (t118 - t122) * t49
        t127 = (src(i,j,nComp,t100) - t75) * t103
        t128 = t127 / 0.2E1
        t131 = (t75 - src(i,j,nComp,t106)) * t103
        t132 = t131 / 0.2E1
        t133 = t89 + t99 + t105 + t110 - t114 - t124 - t128 - t132
        t135 = t83 * t133 * t12
        t138 = t7 * dt
        t139 = t89 - t114
        t140 = dx * t139
        t143 = beta * t7
        t144 = dx ** 2
        t145 = i + 3
        t147 = u(t145,j,n) - t35
        t149 = t37 * t12
        t152 = t40 * t12
        t154 = (t149 - t152) * t12
        t158 = t60 * t12
        t160 = (t152 - t158) * t12
        t161 = t154 - t160
        t163 = t4 * t161 * t12
        t169 = (t4 * t147 * t12 - t39) * t12
        t172 = t44 - t64
        t173 = t172 * t12
        t179 = dy ** 2
        t180 = j + 2
        t181 = u(t9,t180,n)
        t182 = t181 - t46
        t184 = t47 * t49
        t187 = t53 * t49
        t189 = (t184 - t187) * t49
        t193 = j - 2
        t194 = u(t9,t193,n)
        t195 = t52 - t194
        t207 = (t4 * t182 * t49 - t50) * t49
        t213 = (t55 - t4 * t195 * t49) * t49
        t222 = dt * (t44 - t144 * ((t4 * ((t147 * t12 - t149) * t12 - t1
     #54) * t12 - t163) * t12 + ((t169 - t44) * t12 - t173) * t12) / 0.2
     #4E2 + t57 - t179 * ((t4 * ((t182 * t49 - t184) * t49 - t189) * t49
     # - t4 * (t189 - (t187 - t195 * t49) * t49) * t49) * t49 + ((t207 -
     # t57) * t49 - (t57 - t213) * t49) * t49) / 0.24E2 + t58)
        t225 = t13 / 0.2E1
        t227 = ut(t145,j,n) - t15
        t231 = (t227 * t12 - t17) * t12 - t19
        t232 = t231 * t12
        t233 = t26 * t12
        t240 = dx * (t17 / 0.2E1 + t225 - t144 * (t232 / 0.2E1 + t233 / 
     #0.2E1) / 0.6E1) / 0.2E1
        t241 = beta ** 2
        t242 = t241 * t32
        t246 = t4 * t26 * t12
        t252 = (t4 * t227 * t12 - t85) * t12
        t255 = t139 * t12
        t261 = ut(t9,t180,n)
        t262 = t261 - t90
        t264 = t91 * t49
        t267 = t95 * t49
        t269 = (t264 - t267) * t49
        t273 = ut(t9,t193,n)
        t274 = t94 - t273
        t301 = t34 * (t89 - t144 * ((t4 * t231 * t12 - t246) * t12 + ((t
     #252 - t89) * t12 - t255) * t12) / 0.24E2 + t99 - t179 * ((t4 * ((t
     #262 * t49 - t264) * t49 - t269) * t49 - t4 * (t269 - (t267 - t274 
     #* t49) * t49) * t49) * t49 + (((t4 * t262 * t49 - t93) * t49 - t99
     #) * t49 - (t99 - (t97 - t4 * t274 * t49) * t49) * t49) * t49) / 0.
     #24E2 + t105 + t110)
        t304 = dt * dx
        t305 = u(t14,t45,n)
        t309 = u(t14,t51,n)
        t314 = (t4 * (t305 - t35) * t49 - t4 * (t35 - t309) * t49) * t49
        t315 = src(t14,j,nComp,n)
        t317 = (t169 + t314 + t315 - t44 - t57 - t58) * t12
        t318 = t76 * t12
        t321 = t304 * (t317 / 0.2E1 + t318 / 0.2E1)
        t329 = t144 * (t19 - dx * (t232 - t233) / 0.12E2) / 0.12E2
        t330 = t241 * beta
        t331 = t330 * t81
        t337 = t4 * (t44 + t57 - t64 - t74) * t12
        t340 = t305 - t46
        t343 = t46 - t65
        t345 = t4 * t343 * t12
        t347 = (t4 * t340 * t12 - t345) * t12
        t351 = t309 - t52
        t354 = t52 - t69
        t356 = t4 * t354 * t12
        t358 = (t4 * t351 * t12 - t356) * t12
        t369 = t4 * (t58 - t75) * t12
        t372 = src(t9,t45,nComp,n)
        t376 = src(t9,t51,nComp,n)
        t385 = t83 * ((t4 * (t169 + t314 - t44 - t57) * t12 - t337) * t1
     #2 + (t4 * (t347 + t207 - t44 - t57) * t49 - t4 * (t44 + t57 - t358
     # - t213) * t49) * t49 + (t4 * (t315 - t58) * t12 - t369) * t12 + (
     #t4 * (t372 - t58) * t49 - t4 * (t58 - t376) * t49) * t49 + (t104 -
     # t109) * t103)
        t388 = t34 * dx
        t389 = ut(t14,t45,n)
        t393 = ut(t14,t51,n)
        t409 = t133 * t12
        t412 = t388 * ((t252 + (t4 * (t389 - t15) * t49 - t4 * (t15 - t3
     #93) * t49) * t49 + (src(t14,j,nComp,t100) - t315) * t103 / 0.2E1 +
     # (t315 - src(t14,j,nComp,t106)) * t103 / 0.2E1 - t89 - t99 - t105 
     #- t110) * t12 / 0.2E1 + t409 / 0.2E1)
        t416 = t304 * (t317 - t318)
        t419 = i - 2
        t420 = u(t419,j,n)
        t421 = t59 - t420
        t422 = t421 * t12
        t424 = (t158 - t422) * t12
        t425 = t160 - t424
        t427 = t4 * t425 * t12
        t431 = t4 * t421 * t12
        t433 = (t62 - t431) * t12
        t434 = t64 - t433
        t435 = t434 * t12
        t441 = u(i,t180,n)
        t442 = t441 - t65
        t443 = t442 * t49
        t444 = t66 * t49
        t446 = (t443 - t444) * t49
        t447 = t70 * t49
        t449 = (t444 - t447) * t49
        t450 = t446 - t449
        t452 = t4 * t450 * t49
        t453 = u(i,t193,n)
        t454 = t69 - t453
        t455 = t454 * t49
        t457 = (t447 - t455) * t49
        t458 = t449 - t457
        t460 = t4 * t458 * t49
        t464 = t4 * t442 * t49
        t466 = (t464 - t68) * t49
        t467 = t466 - t74
        t468 = t467 * t49
        t470 = t4 * t454 * t49
        t472 = (t72 - t470) * t49
        t473 = t74 - t472
        t474 = t473 * t49
        t481 = dt * (t64 - t144 * ((t163 - t427) * t12 + (t173 - t435) *
     # t12) / 0.24E2 + t74 - t179 * ((t452 - t460) * t49 + (t468 - t474)
     # * t49) / 0.24E2 + t75)
        t482 = t143 * t481
        t483 = t23 / 0.2E1
        t484 = ut(t419,j,n)
        t485 = t21 - t484
        t486 = t485 * t12
        t488 = (t23 - t486) * t12
        t489 = t25 - t488
        t490 = t489 * t12
        t497 = dx * (t225 + t483 - t144 * (t233 / 0.2E1 + t490 / 0.2E1) 
     #/ 0.6E1) / 0.2E1
        t499 = t4 * t489 * t12
        t503 = t4 * t485 * t12
        t505 = (t112 - t503) * t12
        t506 = t114 - t505
        t507 = t506 * t12
        t513 = ut(i,t180,n)
        t514 = t513 - t115
        t515 = t514 * t49
        t516 = t116 * t49
        t518 = (t515 - t516) * t49
        t519 = t120 * t49
        t521 = (t516 - t519) * t49
        t522 = t518 - t521
        t524 = t4 * t522 * t49
        t525 = ut(i,t193,n)
        t526 = t119 - t525
        t527 = t526 * t49
        t529 = (t519 - t527) * t49
        t530 = t521 - t529
        t532 = t4 * t530 * t49
        t536 = t4 * t514 * t49
        t538 = (t536 - t118) * t49
        t539 = t538 - t124
        t540 = t539 * t49
        t542 = t4 * t526 * t49
        t544 = (t122 - t542) * t49
        t545 = t124 - t544
        t546 = t545 * t49
        t553 = t34 * (t114 - t144 * ((t246 - t499) * t12 + (t255 - t507)
     # * t12) / 0.24E2 + t124 - t179 * ((t524 - t532) * t49 + (t540 - t5
     #46) * t49) / 0.24E2 + t128 + t132)
        t555 = t242 * t553 / 0.2E1
        t556 = u(t20,t45,n)
        t557 = t556 - t59
        t559 = t4 * t557 * t49
        t560 = u(t20,t51,n)
        t561 = t59 - t560
        t563 = t4 * t561 * t49
        t565 = (t559 - t563) * t49
        t566 = src(t20,j,nComp,n)
        t567 = t64 + t74 + t75 - t433 - t565 - t566
        t568 = t567 * t12
        t571 = t304 * (t318 / 0.2E1 + t568 / 0.2E1)
        t573 = t143 * t571 / 0.2E1
        t579 = t144 * (t25 - dx * (t233 - t490) / 0.12E2) / 0.12E2
        t582 = t4 * (t64 + t74 - t433 - t565) * t12
        t585 = t65 - t556
        t587 = t4 * t585 * t12
        t589 = (t345 - t587) * t12
        t592 = t4 * (t589 + t466 - t64 - t74) * t49
        t593 = t69 - t560
        t595 = t4 * t593 * t12
        t597 = (t356 - t595) * t12
        t600 = t4 * (t64 + t74 - t597 - t472) * t49
        t605 = t4 * (t75 - t566) * t12
        t608 = src(i,t45,nComp,n)
        t611 = t4 * (t608 - t75) * t49
        t612 = src(i,t51,nComp,n)
        t615 = t4 * (t75 - t612) * t49
        t621 = t83 * ((t337 - t582) * t12 + (t592 - t600) * t49 + (t369 
     #- t605) * t12 + (t611 - t615) * t49 + (t127 - t131) * t103)
        t623 = t331 * t621 / 0.6E1
        t624 = ut(t20,t45,n)
        t625 = t624 - t21
        t627 = t4 * t625 * t49
        t628 = ut(t20,t51,n)
        t629 = t21 - t628
        t631 = t4 * t629 * t49
        t633 = (t627 - t631) * t49
        t636 = (src(t20,j,nComp,t100) - t566) * t103
        t637 = t636 / 0.2E1
        t640 = (t566 - src(t20,j,nComp,t106)) * t103
        t641 = t640 / 0.2E1
        t642 = t114 + t124 + t128 + t132 - t505 - t633 - t637 - t641
        t643 = t642 * t12
        t646 = t388 * (t409 / 0.2E1 + t643 / 0.2E1)
        t648 = t242 * t646 / 0.4E1
        t650 = t304 * (t318 - t568)
        t652 = t143 * t650 / 0.12E2
        t653 = t10 + t143 * t222 - t240 + t242 * t301 / 0.2E1 - t143 * t
     #321 / 0.2E1 + t329 + t331 * t385 / 0.6E1 - t242 * t412 / 0.4E1 + t
     #143 * t416 / 0.12E2 - t2 - t482 - t497 - t555 - t573 - t579 - t623
     # - t648 - t652
        t655 = sqrt(0.16E2)
        t658 = 0.1E1 / 0.2E1 - t6
        t659 = t4 * t658
        t660 = t659 * t30
        t661 = t658 ** 2
        t662 = t4 * t661
        t664 = t662 * t78 / 0.2E1
        t665 = t661 * t658
        t666 = t4 * t665
        t668 = t666 * t135 / 0.6E1
        t669 = t658 * dt
        t671 = t669 * t140 / 0.24E2
        t672 = beta * t658
        t674 = t241 * t661
        t679 = t330 * t665
        t686 = t672 * t481
        t688 = t674 * t553 / 0.2E1
        t690 = t672 * t571 / 0.2E1
        t692 = t679 * t621 / 0.6E1
        t694 = t674 * t646 / 0.4E1
        t696 = t672 * t650 / 0.12E2
        t697 = t10 + t672 * t222 - t240 + t674 * t301 / 0.2E1 - t672 * t
     #321 / 0.2E1 + t329 + t679 * t385 / 0.6E1 - t674 * t412 / 0.4E1 + t
     #672 * t416 / 0.12E2 - t2 - t686 - t497 - t688 - t690 - t579 - t692
     # - t694 - t696
        t700 = cc * t697 * t655 / 0.8E1
        t702 = (t8 * t30 + t33 * t78 / 0.2E1 + t82 * t135 / 0.6E1 - t138
     # * t140 / 0.24E2 + cc * t653 * t655 / 0.8E1 - t660 - t664 - t668 +
     # t671 - t700) * t5
        t708 = t4 * (t152 - dx * t161 / 0.24E2)
        t710 = dx * t172 / 0.24E2
        t718 = dt * (t23 - dx * t489 / 0.24E2)
        t721 = t34 * t567 * t12
        t725 = t83 * t642 * t12
        t728 = dx * t506
        t731 = i - 3
        t733 = t420 - u(t731,j,n)
        t745 = (t431 - t4 * t733 * t12) * t12
        t753 = u(t20,t180,n)
        t754 = t753 - t556
        t756 = t557 * t49
        t759 = t561 * t49
        t761 = (t756 - t759) * t49
        t765 = u(t20,t193,n)
        t766 = t560 - t765
        t778 = (t4 * t754 * t49 - t559) * t49
        t784 = (t563 - t4 * t766 * t49) * t49
        t793 = dt * (t433 - t144 * ((t427 - t4 * (t424 - (t422 - t733 * 
     #t12) * t12) * t12) * t12 + (t435 - (t433 - t745) * t12) * t12) / 0
     #.24E2 + t565 - t179 * ((t4 * ((t754 * t49 - t756) * t49 - t761) * 
     #t49 - t4 * (t761 - (t759 - t766 * t49) * t49) * t49) * t49 + ((t77
     #8 - t565) * t49 - (t565 - t784) * t49) * t49) / 0.24E2 + t566)
        t797 = t484 - ut(t731,j,n)
        t801 = t488 - (t486 - t797 * t12) * t12
        t802 = t801 * t12
        t809 = dx * (t483 + t486 / 0.2E1 - t144 * (t490 / 0.2E1 + t802 /
     # 0.2E1) / 0.6E1) / 0.2E1
        t817 = (t503 - t4 * t797 * t12) * t12
        t825 = ut(t20,t180,n)
        t826 = t825 - t624
        t828 = t625 * t49
        t831 = t629 * t49
        t833 = (t828 - t831) * t49
        t837 = ut(t20,t193,n)
        t838 = t628 - t837
        t865 = t34 * (t505 - t144 * ((t499 - t4 * t801 * t12) * t12 + (t
     #507 - (t505 - t817) * t12) * t12) / 0.24E2 + t633 - t179 * ((t4 * 
     #((t826 * t49 - t828) * t49 - t833) * t49 - t4 * (t833 - (t831 - t8
     #38 * t49) * t49) * t49) * t49 + (((t4 * t826 * t49 - t627) * t49 -
     # t633) * t49 - (t633 - (t631 - t4 * t838 * t49) * t49) * t49) * t4
     #9) / 0.24E2 + t637 + t641)
        t868 = u(t419,t45,n)
        t872 = u(t419,t51,n)
        t877 = (t4 * (t868 - t420) * t49 - t4 * (t420 - t872) * t49) * t
     #49
        t878 = src(t419,j,nComp,n)
        t880 = (t433 + t565 + t566 - t745 - t877 - t878) * t12
        t883 = t304 * (t568 / 0.2E1 + t880 / 0.2E1)
        t891 = t144 * (t488 - dx * (t490 - t802) / 0.12E2) / 0.12E2
        t897 = t556 - t868
        t901 = (t587 - t4 * t897 * t12) * t12
        t905 = t560 - t872
        t909 = (t595 - t4 * t905 * t12) * t12
        t920 = src(t20,t45,nComp,n)
        t924 = src(t20,t51,nComp,n)
        t933 = t83 * ((t582 - t4 * (t433 + t565 - t745 - t877) * t12) * 
     #t12 + (t4 * (t901 + t778 - t433 - t565) * t49 - t4 * (t433 + t565 
     #- t909 - t784) * t49) * t49 + (t605 - t4 * (t566 - t878) * t12) * 
     #t12 + (t4 * (t920 - t566) * t49 - t4 * (t566 - t924) * t49) * t49 
     #+ (t636 - t640) * t103)
        t936 = ut(t419,t45,n)
        t940 = ut(t419,t51,n)
        t958 = t388 * (t643 / 0.2E1 + (t505 + t633 + t637 + t641 - t817 
     #- (t4 * (t936 - t484) * t49 - t4 * (t484 - t940) * t49) * t49 - (s
     #rc(t419,j,nComp,t100) - t878) * t103 / 0.2E1 - (t878 - src(t419,j,
     #nComp,t106)) * t103 / 0.2E1) * t12 / 0.2E1)
        t962 = t304 * (t568 - t880)
        t965 = t2 + t482 - t497 + t555 - t573 + t579 + t623 - t648 + t65
     #2 - t21 - t143 * t793 - t809 - t242 * t865 / 0.2E1 - t143 * t883 /
     # 0.2E1 - t891 - t331 * t933 / 0.6E1 - t242 * t958 / 0.4E1 - t143 *
     # t962 / 0.12E2
        t969 = t659 * t718
        t971 = t662 * t721 / 0.2E1
        t973 = t666 * t725 / 0.6E1
        t975 = t669 * t728 / 0.24E2
        t987 = t2 + t686 - t497 + t688 - t690 + t579 + t692 - t694 + t69
     #6 - t21 - t672 * t793 - t809 - t674 * t865 / 0.2E1 - t672 * t883 /
     # 0.2E1 - t891 - t679 * t933 / 0.6E1 - t674 * t958 / 0.4E1 - t672 *
     # t962 / 0.12E2
        t990 = cc * t987 * t655 / 0.8E1
        t992 = (t8 * t718 + t33 * t721 / 0.2E1 + t82 * t725 / 0.6E1 - t1
     #38 * t728 / 0.24E2 + cc * t965 * t655 / 0.8E1 - t969 - t971 - t973
     # + t975 - t990) * t5
        t998 = t4 * (t158 - dx * t425 / 0.24E2)
        t1000 = dx * t434 / 0.24E2
        t1010 = dt * (t516 - dy * t522 / 0.24E2)
        t1012 = t589 + t466 + t608 - t64 - t74 - t75
        t1014 = t34 * t1012 * t49
        t1017 = t90 - t115
        t1019 = t4 * t1017 * t12
        t1020 = t115 - t624
        t1022 = t4 * t1020 * t12
        t1024 = (t1019 - t1022) * t12
        t1027 = (src(i,t45,nComp,t100) - t608) * t103
        t1028 = t1027 / 0.2E1
        t1031 = (t608 - src(i,t45,nComp,t106)) * t103
        t1032 = t1031 / 0.2E1
        t1033 = t1024 + t538 + t1028 + t1032 - t114 - t124 - t128 - t132
        t1035 = t83 * t1033 * t49
        t1038 = dy * t539
        t1042 = t343 * t12
        t1045 = t585 * t12
        t1047 = (t1042 - t1045) * t12
        t1068 = j + 3
        t1070 = u(i,t1068,n) - t441
        t1082 = (t4 * t1070 * t49 - t464) * t49
        t1091 = dt * (t589 - t144 * ((t4 * ((t340 * t12 - t1042) * t12 -
     # t1047) * t12 - t4 * (t1047 - (t1045 - t897 * t12) * t12) * t12) *
     # t12 + ((t347 - t589) * t12 - (t589 - t901) * t12) * t12) / 0.24E2
     # + t466 - t179 * ((t4 * ((t1070 * t49 - t443) * t49 - t446) * t49 
     #- t452) * t49 + ((t1082 - t466) * t49 - t468) * t49) / 0.24E2 + t6
     #08)
        t1094 = t516 / 0.2E1
        t1096 = ut(i,t1068,n) - t513
        t1100 = (t1096 * t49 - t515) * t49 - t518
        t1101 = t1100 * t49
        t1102 = t522 * t49
        t1109 = dy * (t515 / 0.2E1 + t1094 - t179 * (t1101 / 0.2E1 + t11
     #02 / 0.2E1) / 0.6E1) / 0.2E1
        t1110 = t389 - t90
        t1112 = t1017 * t12
        t1115 = t1020 * t12
        t1117 = (t1112 - t1115) * t12
        t1121 = t624 - t936
        t1154 = (t4 * t1096 * t49 - t536) * t49
        t1163 = t34 * (t1024 - t144 * ((t4 * ((t1110 * t12 - t1112) * t1
     #2 - t1117) * t12 - t4 * (t1117 - (t1115 - t1121 * t12) * t12) * t1
     #2) * t12 + (((t4 * t1110 * t12 - t1019) * t12 - t1024) * t12 - (t1
     #024 - (t1022 - t4 * t1121 * t12) * t12) * t12) * t12) / 0.24E2 + t
     #538 - t179 * ((t4 * t1100 * t49 - t524) * t49 + ((t1154 - t538) * 
     #t49 - t540) * t49) / 0.24E2 + t1028 + t1032)
        t1166 = dt * dy
        t1174 = (t4 * (t181 - t441) * t12 - t4 * (t441 - t753) * t12) * 
     #t12
        t1175 = src(i,t180,nComp,n)
        t1177 = (t1174 + t1082 + t1175 - t589 - t466 - t608) * t49
        t1178 = t1012 * t49
        t1181 = t1166 * (t1177 / 0.2E1 + t1178 / 0.2E1)
        t1189 = t179 * (t518 - dy * (t1101 - t1102) / 0.12E2) / 0.12E2
        t1219 = t83 * ((t4 * (t347 + t207 - t589 - t466) * t12 - t4 * (t
     #589 + t466 - t901 - t778) * t12) * t12 + (t4 * (t1174 + t1082 - t5
     #89 - t466) * t49 - t592) * t49 + (t4 * (t372 - t608) * t12 - t4 * 
     #(t608 - t920) * t12) * t12 + (t4 * (t1175 - t608) * t49 - t611) * 
     #t49 + (t1027 - t1031) * t103)
        t1222 = t34 * dy
        t1241 = t1033 * t49
        t1244 = t1222 * (((t4 * (t261 - t513) * t12 - t4 * (t513 - t825)
     # * t12) * t12 + t1154 + (src(i,t180,nComp,t100) - t1175) * t103 / 
     #0.2E1 + (t1175 - src(i,t180,nComp,t106)) * t103 / 0.2E1 - t1024 - 
     #t538 - t1028 - t1032) * t49 / 0.2E1 + t1241 / 0.2E1)
        t1248 = t1166 * (t1177 - t1178)
        t1251 = t519 / 0.2E1
        t1252 = t530 * t49
        t1259 = dy * (t1094 + t1251 - t179 * (t1102 / 0.2E1 + t1252 / 0.
     #2E1) / 0.6E1) / 0.2E1
        t1260 = t64 + t74 + t75 - t597 - t472 - t612
        t1261 = t1260 * t49
        t1264 = t1166 * (t1178 / 0.2E1 + t1261 / 0.2E1)
        t1266 = t143 * t1264 / 0.2E1
        t1272 = t179 * (t521 - dy * (t1102 - t1252) / 0.12E2) / 0.12E2
        t1273 = t94 - t119
        t1275 = t4 * t1273 * t12
        t1276 = t119 - t628
        t1278 = t4 * t1276 * t12
        t1280 = (t1275 - t1278) * t12
        t1283 = (src(i,t51,nComp,t100) - t612) * t103
        t1284 = t1283 / 0.2E1
        t1287 = (t612 - src(i,t51,nComp,t106)) * t103
        t1288 = t1287 / 0.2E1
        t1289 = t114 + t124 + t128 + t132 - t1280 - t544 - t1284 - t1288
        t1290 = t1289 * t49
        t1293 = t1222 * (t1241 / 0.2E1 + t1290 / 0.2E1)
        t1295 = t242 * t1293 / 0.4E1
        t1297 = t1166 * (t1178 - t1261)
        t1299 = t143 * t1297 / 0.12E2
        t1300 = t115 + t143 * t1091 - t1109 + t242 * t1163 / 0.2E1 - t14
     #3 * t1181 / 0.2E1 + t1189 + t331 * t1219 / 0.6E1 - t242 * t1244 / 
     #0.4E1 + t143 * t1248 / 0.12E2 - t2 - t482 - t1259 - t555 - t1266 -
     # t1272 - t623 - t1295 - t1299
        t1304 = t659 * t1010
        t1306 = t662 * t1014 / 0.2E1
        t1308 = t666 * t1035 / 0.6E1
        t1310 = t669 * t1038 / 0.24E2
        t1323 = t672 * t1264 / 0.2E1
        t1325 = t674 * t1293 / 0.4E1
        t1327 = t672 * t1297 / 0.12E2
        t1328 = t115 + t672 * t1091 - t1109 + t674 * t1163 / 0.2E1 - t67
     #2 * t1181 / 0.2E1 + t1189 + t679 * t1219 / 0.6E1 - t674 * t1244 / 
     #0.4E1 + t672 * t1248 / 0.12E2 - t2 - t686 - t1259 - t688 - t1323 -
     # t1272 - t692 - t1325 - t1327
        t1331 = cc * t1328 * t655 / 0.8E1
        t1333 = (t8 * t1010 + t33 * t1014 / 0.2E1 + t82 * t1035 / 0.6E1 
     #- t138 * t1038 / 0.24E2 + cc * t1300 * t655 / 0.8E1 - t1304 - t130
     #6 - t1308 + t1310 - t1331) * t5
        t1339 = t4 * (t444 - dy * t450 / 0.24E2)
        t1341 = dy * t467 / 0.24E2
        t1349 = dt * (t519 - dy * t530 / 0.24E2)
        t1352 = t34 * t1260 * t49
        t1356 = t83 * t1289 * t49
        t1359 = dy * t545
        t1363 = t354 * t12
        t1366 = t593 * t12
        t1368 = (t1363 - t1366) * t12
        t1389 = j - 3
        t1391 = t453 - u(i,t1389,n)
        t1403 = (t470 - t4 * t1391 * t49) * t49
        t1412 = dt * (t597 - t144 * ((t4 * ((t351 * t12 - t1363) * t12 -
     # t1368) * t12 - t4 * (t1368 - (t1366 - t905 * t12) * t12) * t12) *
     # t12 + ((t358 - t597) * t12 - (t597 - t909) * t12) * t12) / 0.24E2
     # + t472 - t179 * ((t460 - t4 * (t457 - (t455 - t1391 * t49) * t49)
     # * t49) * t49 + (t474 - (t472 - t1403) * t49) * t49) / 0.24E2 + t6
     #12)
        t1416 = t525 - ut(i,t1389,n)
        t1420 = t529 - (t527 - t1416 * t49) * t49
        t1421 = t1420 * t49
        t1428 = dy * (t1251 + t527 / 0.2E1 - t179 * (t1252 / 0.2E1 + t14
     #21 / 0.2E1) / 0.6E1) / 0.2E1
        t1429 = t393 - t94
        t1431 = t1273 * t12
        t1434 = t1276 * t12
        t1436 = (t1431 - t1434) * t12
        t1440 = t628 - t940
        t1473 = (t542 - t4 * t1416 * t49) * t49
        t1482 = t34 * (t1280 - t144 * ((t4 * ((t1429 * t12 - t1431) * t1
     #2 - t1436) * t12 - t4 * (t1436 - (t1434 - t1440 * t12) * t12) * t1
     #2) * t12 + (((t4 * t1429 * t12 - t1275) * t12 - t1280) * t12 - (t1
     #280 - (t1278 - t4 * t1440 * t12) * t12) * t12) * t12) / 0.24E2 + t
     #544 - t179 * ((t532 - t4 * t1420 * t49) * t49 + (t546 - (t544 - t1
     #473) * t49) * t49) / 0.24E2 + t1284 + t1288)
        t1492 = (t4 * (t194 - t453) * t12 - t4 * (t453 - t765) * t12) * 
     #t12
        t1493 = src(i,t193,nComp,n)
        t1495 = (t597 + t472 + t612 - t1492 - t1403 - t1493) * t49
        t1498 = t1166 * (t1261 / 0.2E1 + t1495 / 0.2E1)
        t1506 = t179 * (t529 - dy * (t1252 - t1421) / 0.12E2) / 0.12E2
        t1536 = t83 * ((t4 * (t358 + t213 - t597 - t472) * t12 - t4 * (t
     #597 + t472 - t909 - t784) * t12) * t12 + (t600 - t4 * (t597 + t472
     # - t1492 - t1403) * t49) * t49 + (t4 * (t376 - t612) * t12 - t4 * 
     #(t612 - t924) * t12) * t12 + (t615 - t4 * (t612 - t1493) * t49) * 
     #t49 + (t1283 - t1287) * t103)
        t1559 = t1222 * (t1290 / 0.2E1 + (t1280 + t544 + t1284 + t1288 -
     # (t4 * (t273 - t525) * t12 - t4 * (t525 - t837) * t12) * t12 - t14
     #73 - (src(i,t193,nComp,t100) - t1493) * t103 / 0.2E1 - (t1493 - sr
     #c(i,t193,nComp,t106)) * t103 / 0.2E1) * t49 / 0.2E1)
        t1563 = t1166 * (t1261 - t1495)
        t1566 = t2 + t482 - t1259 + t555 - t1266 + t1272 + t623 - t1295 
     #+ t1299 - t119 - t143 * t1412 - t1428 - t242 * t1482 / 0.2E1 - t14
     #3 * t1498 / 0.2E1 - t1506 - t331 * t1536 / 0.6E1 - t242 * t1559 / 
     #0.4E1 - t143 * t1563 / 0.12E2
        t1570 = t659 * t1349
        t1572 = t662 * t1352 / 0.2E1
        t1574 = t666 * t1356 / 0.6E1
        t1576 = t669 * t1359 / 0.24E2
        t1588 = t2 + t686 - t1259 + t688 - t1323 + t1272 + t692 - t1325 
     #+ t1327 - t119 - t672 * t1412 - t1428 - t674 * t1482 / 0.2E1 - t67
     #2 * t1498 / 0.2E1 - t1506 - t679 * t1536 / 0.6E1 - t674 * t1559 / 
     #0.4E1 - t672 * t1563 / 0.12E2
        t1591 = cc * t1588 * t655 / 0.8E1
        t1593 = (t8 * t1349 + t33 * t1352 / 0.2E1 + t82 * t1356 / 0.6E1 
     #- t138 * t1359 / 0.24E2 + cc * t1566 * t655 / 0.8E1 - t1570 - t157
     #2 - t1574 + t1576 - t1591) * t5
        t1599 = t4 * (t447 - dy * t458 / 0.24E2)
        t1601 = dy * t473 / 0.24E2
        t1611 = src(i,j,nComp,n + 2)
        t1613 = (src(i,j,nComp,n + 3) - t1611) * t5

        unew(i,j) = t1 + dt * t2 + (t702 * t34 / 0.6E1 + (t708 + t6
     #60 + t664 - t710 + t668 - t671 + t700 - t702 * t658) * t34 / 0.2E1
     # - t992 * t34 / 0.6E1 - (t998 + t969 + t971 - t1000 + t973 - t975 
     #+ t990 - t992 * t658) * t34 / 0.2E1) * t12 + (t1333 * t34 / 0.6E1 
     #+ (t1339 + t1304 + t1306 - t1341 + t1308 - t1310 + t1331 - t1333 *
     # t658) * t34 / 0.2E1 - t1593 * t34 / 0.6E1 - (t1599 + t1570 + t157
     #2 - t1601 + t1574 - t1576 + t1591 - t1593 * t658) * t34 / 0.2E1) *
     # t49 + t1613 * t34 / 0.6E1 + (t1611 - t1613 * t658) * t34 / 0.2E1

        utnew(i,j) = 
     #t2 + (t702 * dt / 0.2E1 + (t708 + t660 + t664 - t710 + t668 -
     # t671 + t700) * dt - t702 * t669 - t992 * dt / 0.2E1 - (t998 + t96
     #9 + t971 - t1000 + t973 - t975 + t990) * dt + t992 * t669) * t12 +
     # (t1333 * dt / 0.2E1 + (t1339 + t1304 + t1306 - t1341 + t1308 - t1
     #310 + t1331) * dt - t1333 * t669 - t1593 * dt / 0.2E1 - (t1599 + t
     #1570 + t1572 - t1601 + t1574 - t1576 + t1591) * dt + t1593 * t669)
     # * t49 + t1613 * dt / 0.2E1 + t1611 * dt - t1613 * t669

c        blah = array(int(t1 + dt * t2 + (t702 * t34 / 0.6E1 + (t708 + t6
c     #60 + t664 - t710 + t668 - t671 + t700 - t702 * t658) * t34 / 0.2E1
c     # - t992 * t34 / 0.6E1 - (t998 + t969 + t971 - t1000 + t973 - t975 
c     #+ t990 - t992 * t658) * t34 / 0.2E1) * t12 + (t1333 * t34 / 0.6E1 
c     #+ (t1339 + t1304 + t1306 - t1341 + t1308 - t1310 + t1331 - t1333 *
c     # t658) * t34 / 0.2E1 - t1593 * t34 / 0.6E1 - (t1599 + t1570 + t157
c     #2 - t1601 + t1574 - t1576 + t1591 - t1593 * t658) * t34 / 0.2E1) *
c     # t49 + t1613 * t34 / 0.6E1 + (t1611 - t1613 * t658) * t34 / 0.2E1)
c     #,int(t2 + (t702 * dt / 0.2E1 + (t708 + t660 + t664 - t710 + t668 -
c     # t671 + t700) * dt - t702 * t669 - t992 * dt / 0.2E1 - (t998 + t96
c     #9 + t971 - t1000 + t973 - t975 + t990) * dt + t992 * t669) * t12 +
c     # (t1333 * dt / 0.2E1 + (t1339 + t1304 + t1306 - t1341 + t1308 - t1
c     #310 + t1331) * dt - t1333 * t669 - t1593 * dt / 0.2E1 - (t1599 + t
c     #1570 + t1572 - t1601 + t1574 - t1576 + t1591) * dt + t1593 * t669)
c     # * t49 + t1613 * dt / 0.2E1 + t1611 * dt - t1613 * t669))


        return
      end
