      subroutine duStepWaveGen2d4rc_tz( 
     *   nd1a,nd1b,nd2a,nd2b,
     *   n1a,n1b,n2a,n2b,
     *   ndf4a,ndf4b,nComp,
     *   u,ut,unew,utnew,
     *   src,
     *   dx,dy,dt,cc,
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
      real dx,dy,dt,cc
c
c.. generated code to follow
c
      real t1
        real t10
        integer t100
        real t1002
        real t1004
        real t1005
        real t1007
        real t1009
        real t1012
        real t1013
        real t1016
        real t1017
        real t1018
        real t1020
        real t1023
        real t1027
        real t103
        real t1030
        real t1032
        real t104
        real t105
        integer t1053
        real t1055
        integer t106
        real t1067
        real t1075
        real t1078
        real t1080
        real t1084
        real t1085
        real t1086
        real t109
        real t1093
        real t1094
        real t1096
        real t1099
        real t11
        real t110
        real t1101
        real t1105
        real t112
        real t1138
        real t114
        real t1146
        real t115
        real t1156
        real t1157
        real t1159
        real t116
        real t1160
        real t1163
        real t1171
        real t118
        real t119
        real t12
        real t120
        real t1200
        real t122
        real t1221
        real t1224
        real t1228
        real t1231
        real t1232
        real t1239
        real t124
        real t1240
        real t1241
        real t1244
        real t1246
        real t1252
        real t1253
        real t1255
        real t1256
        real t1258
        real t1260
        real t1263
        real t1264
        real t1267
        real t1268
        real t1269
        real t127
        real t1270
        real t1273
        real t1275
        real t1277
        real t1279
        real t128
        real t1280
        real t1284
        real t1286
        real t1288
        real t1290
        real t13
        real t1303
        real t1305
        real t1307
        real t1308
        real t131
        real t1311
        real t1313
        real t1319
        real t132
        real t1321
        real t1329
        real t133
        real t1332
        real t1336
        real t1339
        real t1343
        real t1346
        real t1348
        real t135
        integer t1369
        real t1371
        real t138
        real t1383
        real t139
        real t1391
        real t1395
        real t1399
        integer t14
        real t140
        real t1400
        real t1407
        real t1408
        real t1410
        real t1413
        real t1415
        real t1419
        real t143
        integer t144
        real t1452
        real t146
        real t1460
        real t1470
        real t1471
        real t1473
        real t1476
        real t148
        real t1484
        real t15
        real t151
        real t1513
        real t153
        real t1536
        real t1540
        real t1543
        real t1547
        real t1549
        real t1551
        real t1553
        real t1565
        real t1568
        real t157
        real t1570
        real t1576
        real t1578
        real t1588
        real t159
        real t1590
        real t16
        real t160
        real t162
        real t168
        real t17
        real t171
        real t172
        real t178
        integer t179
        real t180
        real t181
        real t183
        real t186
        real t188
        real t19
        integer t192
        real t193
        real t194
        real t2
        integer t20
        real t206
        real t21
        real t212
        real t22
        real t220
        real t223
        real t225
        real t229
        real t23
        real t230
        real t231
        real t238
        real t239
        real t243
        real t249
        real t25
        real t252
        real t258
        real t259
        real t26
        real t261
        real t264
        real t266
        real t270
        real t271
        real t297
        real t30
        real t300
        real t304
        real t309
        real t310
        real t312
        real t313
        real t316
        real t32
        real t324
        real t325
        real t33
        real t331
        real t334
        real t337
        real t339
        real t34
        real t341
        real t345
        real t348
        real t35
        real t350
        real t352
        real t36
        real t363
        real t366
        real t37
        real t370
        real t378
        real t381
        real t385
        real t39
        real t4
        real t40
        real t401
        real t404
        real t408
        integer t411
        real t412
        real t413
        real t414
        real t416
        real t417
        real t419
        real t42
        real t423
        real t425
        real t426
        real t427
        real t433
        real t434
        real t435
        real t436
        real t438
        real t439
        real t44
        real t441
        real t442
        real t444
        real t445
        real t446
        real t447
        real t449
        integer t45
        real t450
        real t452
        real t456
        real t458
        real t459
        real t46
        real t460
        real t462
        real t464
        real t465
        real t466
        real t47
        real t472
        real t473
        real t474
        real t475
        real t476
        real t477
        real t479
        real t480
        real t481
        real t488
        real t49
        real t490
        real t494
        real t496
        real t497
        real t498
        real t5
        real t50
        real t504
        real t505
        real t506
        real t507
        real t509
        integer t51
        real t510
        real t512
        real t513
        real t515
        real t516
        real t517
        real t518
        real t52
        real t520
        real t521
        real t523
        real t527
        real t529
        real t53
        real t530
        real t531
        real t533
        real t535
        real t536
        real t537
        real t543
        real t545
        real t546
        real t547
        real t549
        real t55
        real t550
        real t551
        real t553
        real t555
        real t556
        real t557
        real t558
        real t561
        real t563
        real t569
        real t57
        real t572
        real t575
        real t577
        real t579
        real t58
        real t582
        real t583
        real t585
        real t587
        real t59
        real t590
        real t595
        real t598
        real t6
        real t60
        real t601
        real t602
        real t605
        real t610
        real t612
        real t613
        real t614
        real t616
        real t617
        real t618
        real t62
        real t620
        real t622
        real t625
        real t626
        real t629
        real t630
        real t631
        real t632
        real t635
        real t637
        real t639
        real t64
        real t641
        real t642
        real t644
        real t647
        real t648
        real t649
        real t65
        real t650
        real t651
        real t653
        real t654
        real t655
        real t657
        real t658
        real t66
        real t660
        real t662
        real t667
        real t674
        real t676
        real t678
        real t68
        real t680
        real t682
        real t684
        real t685
        real t688
        real t69
        real t690
        real t696
        real t698
        real t7
        real t70
        real t706
        real t709
        real t713
        real t716
        integer t719
        real t72
        real t721
        real t733
        real t74
        real t741
        real t742
        real t744
        real t747
        real t749
        real t75
        real t753
        real t754
        real t76
        real t766
        real t772
        real t78
        real t780
        real t784
        real t788
        real t789
        real t796
        real t8
        real t804
        real t81
        real t812
        real t813
        real t815
        real t818
        real t82
        real t820
        real t824
        real t825
        real t83
        real t85
        real t851
        real t854
        real t858
        real t863
        real t864
        real t866
        real t869
        real t87
        real t877
        real t883
        real t887
        real t89
        real t891
        real t895
        integer t9
        real t90
        real t906
        real t91
        real t910
        real t918
        real t921
        real t925
        real t93
        real t94
        real t943
        real t947
        real t95
        real t950
        real t954
        real t956
        real t958
        real t960
        real t97
        real t972
        real t975
        real t977
        real t983
        real t985
        real t99
        real t995
        real t997
        real t999
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
        t143 = dx ** 2
        t144 = i + 3
        t146 = u(t144,j,n) - t35
        t148 = t37 * t12
        t151 = t40 * t12
        t153 = (t148 - t151) * t12
        t157 = t60 * t12
        t159 = (t151 - t157) * t12
        t160 = t153 - t159
        t162 = t4 * t160 * t12
        t168 = (t4 * t146 * t12 - t39) * t12
        t171 = t44 - t64
        t172 = t171 * t12
        t178 = dy ** 2
        t179 = j + 2
        t180 = u(t9,t179,n)
        t181 = t180 - t46
        t183 = t47 * t49
        t186 = t53 * t49
        t188 = (t183 - t186) * t49
        t192 = j - 2
        t193 = u(t9,t192,n)
        t194 = t52 - t193
        t206 = (t4 * t181 * t49 - t50) * t49
        t212 = (t55 - t4 * t194 * t49) * t49
        t220 = t44 - t143 * ((t4 * ((t146 * t12 - t148) * t12 - t153) * 
     #t12 - t162) * t12 + ((t168 - t44) * t12 - t172) * t12) / 0.24E2 + 
     #t57 - t178 * ((t4 * ((t181 * t49 - t183) * t49 - t188) * t49 - t4 
     #* (t188 - (t186 - t194 * t49) * t49) * t49) * t49 + ((t206 - t57) 
     #* t49 - (t57 - t212) * t49) * t49) / 0.24E2 + t58
        t223 = t13 / 0.2E1
        t225 = ut(t144,j,n) - t15
        t229 = (t225 * t12 - t17) * t12 - t19
        t230 = t229 * t12
        t231 = t26 * t12
        t238 = dx * (t17 / 0.2E1 + t223 - t143 * (t230 / 0.2E1 + t231 / 
     #0.2E1) / 0.6E1) / 0.2E1
        t239 = t32 * t34
        t243 = t4 * t26 * t12
        t249 = (t4 * t225 * t12 - t85) * t12
        t252 = t139 * t12
        t258 = ut(t9,t179,n)
        t259 = t258 - t90
        t261 = t91 * t49
        t264 = t95 * t49
        t266 = (t261 - t264) * t49
        t270 = ut(t9,t192,n)
        t271 = t94 - t270
        t297 = t89 - t143 * ((t4 * t229 * t12 - t243) * t12 + ((t249 - t
     #89) * t12 - t252) * t12) / 0.24E2 + t99 - t178 * ((t4 * ((t259 * t
     #49 - t261) * t49 - t266) * t49 - t4 * (t266 - (t264 - t271 * t49) 
     #* t49) * t49) * t49 + (((t4 * t259 * t49 - t93) * t49 - t99) * t49
     # - (t99 - (t97 - t4 * t271 * t49) * t49) * t49) * t49) / 0.24E2 + 
     #t105 + t110
        t300 = u(t14,t45,n)
        t304 = u(t14,t51,n)
        t309 = (t4 * (t300 - t35) * t49 - t4 * (t35 - t304) * t49) * t49
        t310 = src(t14,j,nComp,n)
        t312 = (t168 + t309 + t310 - t44 - t57 - t58) * t12
        t313 = t76 * t12
        t316 = dx * (t312 / 0.2E1 + t313 / 0.2E1)
        t324 = t143 * (t19 - dx * (t230 - t231) / 0.12E2) / 0.12E2
        t325 = t81 * t83
        t331 = t4 * (t44 + t57 - t64 - t74) * t12
        t334 = t300 - t46
        t337 = t46 - t65
        t339 = t4 * t337 * t12
        t341 = (t4 * t334 * t12 - t339) * t12
        t345 = t304 - t52
        t348 = t52 - t69
        t350 = t4 * t348 * t12
        t352 = (t4 * t345 * t12 - t350) * t12
        t363 = t4 * (t58 - t75) * t12
        t366 = src(t9,t45,nComp,n)
        t370 = src(t9,t51,nComp,n)
        t378 = (t4 * (t168 + t309 - t44 - t57) * t12 - t331) * t12 + (t4
     # * (t341 + t206 - t44 - t57) * t49 - t4 * (t44 + t57 - t352 - t212
     #) * t49) * t49 + (t4 * (t310 - t58) * t12 - t363) * t12 + (t4 * (t
     #366 - t58) * t49 - t4 * (t58 - t370) * t49) * t49 + (t104 - t109) 
     #* t103
        t381 = ut(t14,t45,n)
        t385 = ut(t14,t51,n)
        t401 = t133 * t12
        t404 = dx * ((t249 + (t4 * (t381 - t15) * t49 - t4 * (t15 - t385
     #) * t49) * t49 + (src(t14,j,nComp,t100) - t310) * t103 / 0.2E1 + (
     #t310 - src(t14,j,nComp,t106)) * t103 / 0.2E1 - t89 - t99 - t105 - 
     #t110) * t12 / 0.2E1 + t401 / 0.2E1)
        t408 = dx * (t312 - t313)
        t411 = i - 2
        t412 = u(t411,j,n)
        t413 = t59 - t412
        t414 = t413 * t12
        t416 = (t157 - t414) * t12
        t417 = t159 - t416
        t419 = t4 * t417 * t12
        t423 = t4 * t413 * t12
        t425 = (t62 - t423) * t12
        t426 = t64 - t425
        t427 = t426 * t12
        t433 = u(i,t179,n)
        t434 = t433 - t65
        t435 = t434 * t49
        t436 = t66 * t49
        t438 = (t435 - t436) * t49
        t439 = t70 * t49
        t441 = (t436 - t439) * t49
        t442 = t438 - t441
        t444 = t4 * t442 * t49
        t445 = u(i,t192,n)
        t446 = t69 - t445
        t447 = t446 * t49
        t449 = (t439 - t447) * t49
        t450 = t441 - t449
        t452 = t4 * t450 * t49
        t456 = t4 * t434 * t49
        t458 = (t456 - t68) * t49
        t459 = t458 - t74
        t460 = t459 * t49
        t462 = t4 * t446 * t49
        t464 = (t72 - t462) * t49
        t465 = t74 - t464
        t466 = t465 * t49
        t472 = t64 - t143 * ((t162 - t419) * t12 + (t172 - t427) * t12) 
     #/ 0.24E2 + t74 - t178 * ((t444 - t452) * t49 + (t460 - t466) * t49
     #) / 0.24E2 + t75
        t473 = t138 * t472
        t474 = t23 / 0.2E1
        t475 = ut(t411,j,n)
        t476 = t21 - t475
        t477 = t476 * t12
        t479 = (t23 - t477) * t12
        t480 = t25 - t479
        t481 = t480 * t12
        t488 = dx * (t223 + t474 - t143 * (t231 / 0.2E1 + t481 / 0.2E1) 
     #/ 0.6E1) / 0.2E1
        t490 = t4 * t480 * t12
        t494 = t4 * t476 * t12
        t496 = (t112 - t494) * t12
        t497 = t114 - t496
        t498 = t497 * t12
        t504 = ut(i,t179,n)
        t505 = t504 - t115
        t506 = t505 * t49
        t507 = t116 * t49
        t509 = (t506 - t507) * t49
        t510 = t120 * t49
        t512 = (t507 - t510) * t49
        t513 = t509 - t512
        t515 = t4 * t513 * t49
        t516 = ut(i,t192,n)
        t517 = t119 - t516
        t518 = t517 * t49
        t520 = (t510 - t518) * t49
        t521 = t512 - t520
        t523 = t4 * t521 * t49
        t527 = t4 * t505 * t49
        t529 = (t527 - t118) * t49
        t530 = t529 - t124
        t531 = t530 * t49
        t533 = t4 * t517 * t49
        t535 = (t122 - t533) * t49
        t536 = t124 - t535
        t537 = t536 * t49
        t543 = t114 - t143 * ((t243 - t490) * t12 + (t252 - t498) * t12)
     # / 0.24E2 + t124 - t178 * ((t515 - t523) * t49 + (t531 - t537) * t
     #49) / 0.24E2 + t128 + t132
        t545 = t239 * t543 / 0.2E1
        t546 = u(t20,t45,n)
        t547 = t546 - t59
        t549 = t4 * t547 * t49
        t550 = u(t20,t51,n)
        t551 = t59 - t550
        t553 = t4 * t551 * t49
        t555 = (t549 - t553) * t49
        t556 = src(t20,j,nComp,n)
        t557 = t64 + t74 + t75 - t425 - t555 - t556
        t558 = t557 * t12
        t561 = dx * (t313 / 0.2E1 + t558 / 0.2E1)
        t563 = t138 * t561 / 0.2E1
        t569 = t143 * (t25 - dx * (t231 - t481) / 0.12E2) / 0.12E2
        t572 = t4 * (t64 + t74 - t425 - t555) * t12
        t575 = t65 - t546
        t577 = t4 * t575 * t12
        t579 = (t339 - t577) * t12
        t582 = t4 * (t579 + t458 - t64 - t74) * t49
        t583 = t69 - t550
        t585 = t4 * t583 * t12
        t587 = (t350 - t585) * t12
        t590 = t4 * (t64 + t74 - t587 - t464) * t49
        t595 = t4 * (t75 - t556) * t12
        t598 = src(i,t45,nComp,n)
        t601 = t4 * (t598 - t75) * t49
        t602 = src(i,t51,nComp,n)
        t605 = t4 * (t75 - t602) * t49
        t610 = (t331 - t572) * t12 + (t582 - t590) * t49 + (t363 - t595)
     # * t12 + (t601 - t605) * t49 + (t127 - t131) * t103
        t612 = t325 * t610 / 0.6E1
        t613 = ut(t20,t45,n)
        t614 = t613 - t21
        t616 = t4 * t614 * t49
        t617 = ut(t20,t51,n)
        t618 = t21 - t617
        t620 = t4 * t618 * t49
        t622 = (t616 - t620) * t49
        t625 = (src(t20,j,nComp,t100) - t556) * t103
        t626 = t625 / 0.2E1
        t629 = (t556 - src(t20,j,nComp,t106)) * t103
        t630 = t629 / 0.2E1
        t631 = t114 + t124 + t128 + t132 - t496 - t622 - t626 - t630
        t632 = t631 * t12
        t635 = dx * (t401 / 0.2E1 + t632 / 0.2E1)
        t637 = t239 * t635 / 0.4E1
        t639 = dx * (t313 - t558)
        t641 = t138 * t639 / 0.12E2
        t642 = t10 + t138 * t220 - t238 + t239 * t297 / 0.2E1 - t138 * t
     #316 / 0.2E1 + t324 + t325 * t378 / 0.6E1 - t239 * t404 / 0.4E1 + t
     #138 * t408 / 0.12E2 - t2 - t473 - t488 - t545 - t563 - t569 - t612
     # - t637 - t641
        t644 = sqrt(0.16E2)
        t647 = 0.1E1 / 0.2E1 - t6
        t648 = t4 * t647
        t649 = t648 * t30
        t650 = t647 ** 2
        t651 = t4 * t650
        t653 = t651 * t78 / 0.2E1
        t654 = t650 * t647
        t655 = t4 * t654
        t657 = t655 * t135 / 0.6E1
        t658 = t647 * dt
        t660 = t658 * t140 / 0.24E2
        t662 = t650 * t34
        t667 = t654 * t83
        t674 = t658 * t472
        t676 = t662 * t543 / 0.2E1
        t678 = t658 * t561 / 0.2E1
        t680 = t667 * t610 / 0.6E1
        t682 = t662 * t635 / 0.4E1
        t684 = t658 * t639 / 0.12E2
        t685 = t10 + t658 * t220 - t238 + t662 * t297 / 0.2E1 - t658 * t
     #316 / 0.2E1 + t324 + t667 * t378 / 0.6E1 - t662 * t404 / 0.4E1 + t
     #658 * t408 / 0.12E2 - t2 - t674 - t488 - t676 - t678 - t569 - t680
     # - t682 - t684
        t688 = cc * t685 * t644 / 0.8E1
        t690 = (t8 * t30 + t33 * t78 / 0.2E1 + t82 * t135 / 0.6E1 - t138
     # * t140 / 0.24E2 + cc * t642 * t644 / 0.8E1 - t649 - t653 - t657 +
     # t660 - t688) * t5
        t696 = t4 * (t151 - dx * t160 / 0.24E2)
        t698 = dx * t171 / 0.24E2
        t706 = dt * (t23 - dx * t480 / 0.24E2)
        t709 = t34 * t557 * t12
        t713 = t83 * t631 * t12
        t716 = dx * t497
        t719 = i - 3
        t721 = t412 - u(t719,j,n)
        t733 = (t423 - t4 * t721 * t12) * t12
        t741 = u(t20,t179,n)
        t742 = t741 - t546
        t744 = t547 * t49
        t747 = t551 * t49
        t749 = (t744 - t747) * t49
        t753 = u(t20,t192,n)
        t754 = t550 - t753
        t766 = (t4 * t742 * t49 - t549) * t49
        t772 = (t553 - t4 * t754 * t49) * t49
        t780 = t425 - t143 * ((t419 - t4 * (t416 - (t414 - t721 * t12) *
     # t12) * t12) * t12 + (t427 - (t425 - t733) * t12) * t12) / 0.24E2 
     #+ t555 - t178 * ((t4 * ((t742 * t49 - t744) * t49 - t749) * t49 - 
     #t4 * (t749 - (t747 - t754 * t49) * t49) * t49) * t49 + ((t766 - t5
     #55) * t49 - (t555 - t772) * t49) * t49) / 0.24E2 + t556
        t784 = t475 - ut(t719,j,n)
        t788 = t479 - (t477 - t784 * t12) * t12
        t789 = t788 * t12
        t796 = dx * (t474 + t477 / 0.2E1 - t143 * (t481 / 0.2E1 + t789 /
     # 0.2E1) / 0.6E1) / 0.2E1
        t804 = (t494 - t4 * t784 * t12) * t12
        t812 = ut(t20,t179,n)
        t813 = t812 - t613
        t815 = t614 * t49
        t818 = t618 * t49
        t820 = (t815 - t818) * t49
        t824 = ut(t20,t192,n)
        t825 = t617 - t824
        t851 = t496 - t143 * ((t490 - t4 * t788 * t12) * t12 + (t498 - (
     #t496 - t804) * t12) * t12) / 0.24E2 + t622 - t178 * ((t4 * ((t813 
     #* t49 - t815) * t49 - t820) * t49 - t4 * (t820 - (t818 - t825 * t4
     #9) * t49) * t49) * t49 + (((t4 * t813 * t49 - t616) * t49 - t622) 
     #* t49 - (t622 - (t620 - t4 * t825 * t49) * t49) * t49) * t49) / 0.
     #24E2 + t626 + t630
        t854 = u(t411,t45,n)
        t858 = u(t411,t51,n)
        t863 = (t4 * (t854 - t412) * t49 - t4 * (t412 - t858) * t49) * t
     #49
        t864 = src(t411,j,nComp,n)
        t866 = (t425 + t555 + t556 - t733 - t863 - t864) * t12
        t869 = dx * (t558 / 0.2E1 + t866 / 0.2E1)
        t877 = t143 * (t479 - dx * (t481 - t789) / 0.12E2) / 0.12E2
        t883 = t546 - t854
        t887 = (t577 - t4 * t883 * t12) * t12
        t891 = t550 - t858
        t895 = (t585 - t4 * t891 * t12) * t12
        t906 = src(t20,t45,nComp,n)
        t910 = src(t20,t51,nComp,n)
        t918 = (t572 - t4 * (t425 + t555 - t733 - t863) * t12) * t12 + (
     #t4 * (t887 + t766 - t425 - t555) * t49 - t4 * (t425 + t555 - t895 
     #- t772) * t49) * t49 + (t595 - t4 * (t556 - t864) * t12) * t12 + (
     #t4 * (t906 - t556) * t49 - t4 * (t556 - t910) * t49) * t49 + (t625
     # - t629) * t103
        t921 = ut(t411,t45,n)
        t925 = ut(t411,t51,n)
        t943 = dx * (t632 / 0.2E1 + (t496 + t622 + t626 + t630 - t804 - 
     #(t4 * (t921 - t475) * t49 - t4 * (t475 - t925) * t49) * t49 - (src
     #(t411,j,nComp,t100) - t864) * t103 / 0.2E1 - (t864 - src(t411,j,nC
     #omp,t106)) * t103 / 0.2E1) * t12 / 0.2E1)
        t947 = dx * (t558 - t866)
        t950 = t2 + t473 - t488 + t545 - t563 + t569 + t612 - t637 + t64
     #1 - t21 - t138 * t780 - t796 - t239 * t851 / 0.2E1 - t138 * t869 /
     # 0.2E1 - t877 - t325 * t918 / 0.6E1 - t239 * t943 / 0.4E1 - t138 *
     # t947 / 0.12E2
        t954 = t648 * t706
        t956 = t651 * t709 / 0.2E1
        t958 = t655 * t713 / 0.6E1
        t960 = t658 * t716 / 0.24E2
        t972 = t2 + t674 - t488 + t676 - t678 + t569 + t680 - t682 + t68
     #4 - t21 - t658 * t780 - t796 - t662 * t851 / 0.2E1 - t658 * t869 /
     # 0.2E1 - t877 - t667 * t918 / 0.6E1 - t662 * t943 / 0.4E1 - t658 *
     # t947 / 0.12E2
        t975 = cc * t972 * t644 / 0.8E1
        t977 = (t8 * t706 + t33 * t709 / 0.2E1 + t82 * t713 / 0.6E1 - t1
     #38 * t716 / 0.24E2 + cc * t950 * t644 / 0.8E1 - t954 - t956 - t958
     # + t960 - t975) * t5
        t983 = t4 * (t157 - dx * t417 / 0.24E2)
        t985 = dx * t426 / 0.24E2
        t995 = dt * (t507 - dy * t513 / 0.24E2)
        t997 = t579 + t458 + t598 - t64 - t74 - t75
        t999 = t34 * t997 * t49
        t1002 = t90 - t115
        t1004 = t4 * t1002 * t12
        t1005 = t115 - t613
        t1007 = t4 * t1005 * t12
        t1009 = (t1004 - t1007) * t12
        t1012 = (src(i,t45,nComp,t100) - t598) * t103
        t1013 = t1012 / 0.2E1
        t1016 = (t598 - src(i,t45,nComp,t106)) * t103
        t1017 = t1016 / 0.2E1
        t1018 = t1009 + t529 + t1013 + t1017 - t114 - t124 - t128 - t132
        t1020 = t83 * t1018 * t49
        t1023 = dy * t530
        t1027 = t337 * t12
        t1030 = t575 * t12
        t1032 = (t1027 - t1030) * t12
        t1053 = j + 3
        t1055 = u(i,t1053,n) - t433
        t1067 = (t4 * t1055 * t49 - t456) * t49
        t1075 = t579 - t143 * ((t4 * ((t334 * t12 - t1027) * t12 - t1032
     #) * t12 - t4 * (t1032 - (t1030 - t883 * t12) * t12) * t12) * t12 +
     # ((t341 - t579) * t12 - (t579 - t887) * t12) * t12) / 0.24E2 + t45
     #8 - t178 * ((t4 * ((t1055 * t49 - t435) * t49 - t438) * t49 - t444
     #) * t49 + ((t1067 - t458) * t49 - t460) * t49) / 0.24E2 + t598
        t1078 = t507 / 0.2E1
        t1080 = ut(i,t1053,n) - t504
        t1084 = (t1080 * t49 - t506) * t49 - t509
        t1085 = t1084 * t49
        t1086 = t513 * t49
        t1093 = dy * (t506 / 0.2E1 + t1078 - t178 * (t1085 / 0.2E1 + t10
     #86 / 0.2E1) / 0.6E1) / 0.2E1
        t1094 = t381 - t90
        t1096 = t1002 * t12
        t1099 = t1005 * t12
        t1101 = (t1096 - t1099) * t12
        t1105 = t613 - t921
        t1138 = (t4 * t1080 * t49 - t527) * t49
        t1146 = t1009 - t143 * ((t4 * ((t1094 * t12 - t1096) * t12 - t11
     #01) * t12 - t4 * (t1101 - (t1099 - t1105 * t12) * t12) * t12) * t1
     #2 + (((t4 * t1094 * t12 - t1004) * t12 - t1009) * t12 - (t1009 - (
     #t1007 - t4 * t1105 * t12) * t12) * t12) * t12) / 0.24E2 + t529 - t
     #178 * ((t4 * t1084 * t49 - t515) * t49 + ((t1138 - t529) * t49 - t
     #531) * t49) / 0.24E2 + t1013 + t1017
        t1156 = (t4 * (t180 - t433) * t12 - t4 * (t433 - t741) * t12) * 
     #t12
        t1157 = src(i,t179,nComp,n)
        t1159 = (t1156 + t1067 + t1157 - t579 - t458 - t598) * t49
        t1160 = t997 * t49
        t1163 = dy * (t1159 / 0.2E1 + t1160 / 0.2E1)
        t1171 = t178 * (t509 - dy * (t1085 - t1086) / 0.12E2) / 0.12E2
        t1200 = (t4 * (t341 + t206 - t579 - t458) * t12 - t4 * (t579 + t
     #458 - t887 - t766) * t12) * t12 + (t4 * (t1156 + t1067 - t579 - t4
     #58) * t49 - t582) * t49 + (t4 * (t366 - t598) * t12 - t4 * (t598 -
     # t906) * t12) * t12 + (t4 * (t1157 - t598) * t49 - t601) * t49 + (
     #t1012 - t1016) * t103
        t1221 = t1018 * t49
        t1224 = dy * (((t4 * (t258 - t504) * t12 - t4 * (t504 - t812) * 
     #t12) * t12 + t1138 + (src(i,t179,nComp,t100) - t1157) * t103 / 0.2
     #E1 + (t1157 - src(i,t179,nComp,t106)) * t103 / 0.2E1 - t1009 - t52
     #9 - t1013 - t1017) * t49 / 0.2E1 + t1221 / 0.2E1)
        t1228 = dy * (t1159 - t1160)
        t1231 = t510 / 0.2E1
        t1232 = t521 * t49
        t1239 = dy * (t1078 + t1231 - t178 * (t1086 / 0.2E1 + t1232 / 0.
     #2E1) / 0.6E1) / 0.2E1
        t1240 = t64 + t74 + t75 - t587 - t464 - t602
        t1241 = t1240 * t49
        t1244 = dy * (t1160 / 0.2E1 + t1241 / 0.2E1)
        t1246 = t138 * t1244 / 0.2E1
        t1252 = t178 * (t512 - dy * (t1086 - t1232) / 0.12E2) / 0.12E2
        t1253 = t94 - t119
        t1255 = t4 * t1253 * t12
        t1256 = t119 - t617
        t1258 = t4 * t1256 * t12
        t1260 = (t1255 - t1258) * t12
        t1263 = (src(i,t51,nComp,t100) - t602) * t103
        t1264 = t1263 / 0.2E1
        t1267 = (t602 - src(i,t51,nComp,t106)) * t103
        t1268 = t1267 / 0.2E1
        t1269 = t114 + t124 + t128 + t132 - t1260 - t535 - t1264 - t1268
        t1270 = t1269 * t49
        t1273 = dy * (t1221 / 0.2E1 + t1270 / 0.2E1)
        t1275 = t239 * t1273 / 0.4E1
        t1277 = dy * (t1160 - t1241)
        t1279 = t138 * t1277 / 0.12E2
        t1280 = t115 + t138 * t1075 - t1093 + t239 * t1146 / 0.2E1 - t13
     #8 * t1163 / 0.2E1 + t1171 + t325 * t1200 / 0.6E1 - t239 * t1224 / 
     #0.4E1 + t138 * t1228 / 0.12E2 - t2 - t473 - t1239 - t545 - t1246 -
     # t1252 - t612 - t1275 - t1279
        t1284 = t648 * t995
        t1286 = t651 * t999 / 0.2E1
        t1288 = t655 * t1020 / 0.6E1
        t1290 = t658 * t1023 / 0.24E2
        t1303 = t658 * t1244 / 0.2E1
        t1305 = t662 * t1273 / 0.4E1
        t1307 = t658 * t1277 / 0.12E2
        t1308 = t115 + t658 * t1075 - t1093 + t662 * t1146 / 0.2E1 - t65
     #8 * t1163 / 0.2E1 + t1171 + t667 * t1200 / 0.6E1 - t662 * t1224 / 
     #0.4E1 + t658 * t1228 / 0.12E2 - t2 - t674 - t1239 - t676 - t1303 -
     # t1252 - t680 - t1305 - t1307
        t1311 = cc * t1308 * t644 / 0.8E1
        t1313 = (t8 * t995 + t33 * t999 / 0.2E1 + t82 * t1020 / 0.6E1 - 
     #t138 * t1023 / 0.24E2 + cc * t1280 * t644 / 0.8E1 - t1284 - t1286 
     #- t1288 + t1290 - t1311) * t5
        t1319 = t4 * (t436 - dy * t442 / 0.24E2)
        t1321 = dy * t459 / 0.24E2
        t1329 = dt * (t510 - dy * t521 / 0.24E2)
        t1332 = t34 * t1240 * t49
        t1336 = t83 * t1269 * t49
        t1339 = dy * t536
        t1343 = t348 * t12
        t1346 = t583 * t12
        t1348 = (t1343 - t1346) * t12
        t1369 = j - 3
        t1371 = t445 - u(i,t1369,n)
        t1383 = (t462 - t4 * t1371 * t49) * t49
        t1391 = t587 - t143 * ((t4 * ((t345 * t12 - t1343) * t12 - t1348
     #) * t12 - t4 * (t1348 - (t1346 - t891 * t12) * t12) * t12) * t12 +
     # ((t352 - t587) * t12 - (t587 - t895) * t12) * t12) / 0.24E2 + t46
     #4 - t178 * ((t452 - t4 * (t449 - (t447 - t1371 * t49) * t49) * t49
     #) * t49 + (t466 - (t464 - t1383) * t49) * t49) / 0.24E2 + t602
        t1395 = t516 - ut(i,t1369,n)
        t1399 = t520 - (t518 - t1395 * t49) * t49
        t1400 = t1399 * t49
        t1407 = dy * (t1231 + t518 / 0.2E1 - t178 * (t1232 / 0.2E1 + t14
     #00 / 0.2E1) / 0.6E1) / 0.2E1
        t1408 = t385 - t94
        t1410 = t1253 * t12
        t1413 = t1256 * t12
        t1415 = (t1410 - t1413) * t12
        t1419 = t617 - t925
        t1452 = (t533 - t4 * t1395 * t49) * t49
        t1460 = t1260 - t143 * ((t4 * ((t1408 * t12 - t1410) * t12 - t14
     #15) * t12 - t4 * (t1415 - (t1413 - t1419 * t12) * t12) * t12) * t1
     #2 + (((t4 * t1408 * t12 - t1255) * t12 - t1260) * t12 - (t1260 - (
     #t1258 - t4 * t1419 * t12) * t12) * t12) * t12) / 0.24E2 + t535 - t
     #178 * ((t523 - t4 * t1399 * t49) * t49 + (t537 - (t535 - t1452) * 
     #t49) * t49) / 0.24E2 + t1264 + t1268
        t1470 = (t4 * (t193 - t445) * t12 - t4 * (t445 - t753) * t12) * 
     #t12
        t1471 = src(i,t192,nComp,n)
        t1473 = (t587 + t464 + t602 - t1470 - t1383 - t1471) * t49
        t1476 = dy * (t1241 / 0.2E1 + t1473 / 0.2E1)
        t1484 = t178 * (t520 - dy * (t1232 - t1400) / 0.12E2) / 0.12E2
        t1513 = (t4 * (t352 + t212 - t587 - t464) * t12 - t4 * (t587 + t
     #464 - t895 - t772) * t12) * t12 + (t590 - t4 * (t587 + t464 - t147
     #0 - t1383) * t49) * t49 + (t4 * (t370 - t602) * t12 - t4 * (t602 -
     # t910) * t12) * t12 + (t605 - t4 * (t602 - t1471) * t49) * t49 + (
     #t1263 - t1267) * t103
        t1536 = dy * (t1270 / 0.2E1 + (t1260 + t535 + t1264 + t1268 - (t
     #4 * (t270 - t516) * t12 - t4 * (t516 - t824) * t12) * t12 - t1452 
     #- (src(i,t192,nComp,t100) - t1471) * t103 / 0.2E1 - (t1471 - src(i
     #,t192,nComp,t106)) * t103 / 0.2E1) * t49 / 0.2E1)
        t1540 = dy * (t1241 - t1473)
        t1543 = t2 + t473 - t1239 + t545 - t1246 + t1252 + t612 - t1275 
     #+ t1279 - t119 - t138 * t1391 - t1407 - t239 * t1460 / 0.2E1 - t13
     #8 * t1476 / 0.2E1 - t1484 - t325 * t1513 / 0.6E1 - t239 * t1536 / 
     #0.4E1 - t138 * t1540 / 0.12E2
        t1547 = t648 * t1329
        t1549 = t651 * t1332 / 0.2E1
        t1551 = t655 * t1336 / 0.6E1
        t1553 = t658 * t1339 / 0.24E2
        t1565 = t2 + t674 - t1239 + t676 - t1303 + t1252 + t680 - t1305 
     #+ t1307 - t119 - t658 * t1391 - t1407 - t662 * t1460 / 0.2E1 - t65
     #8 * t1476 / 0.2E1 - t1484 - t667 * t1513 / 0.6E1 - t662 * t1536 / 
     #0.4E1 - t658 * t1540 / 0.12E2
        t1568 = cc * t1565 * t644 / 0.8E1
        t1570 = (t8 * t1329 + t33 * t1332 / 0.2E1 + t82 * t1336 / 0.6E1 
     #- t138 * t1339 / 0.24E2 + cc * t1543 * t644 / 0.8E1 - t1547 - t154
     #9 - t1551 + t1553 - t1568) * t5
        t1576 = t4 * (t439 - dy * t450 / 0.24E2)
        t1578 = dy * t465 / 0.24E2

        t1588 = src(i,j,nComp,n + 2)
        t1590 = (src(i,j,nComp,n + 3) - t1588) * t5

        unew(i,j) = t1 + dt * t2 + (t690 * t34 / 0.6E1 + (t696 + t6
     #49 + t653 - t698 + t657 - t660 + t688 - t690 * t647) * t34 / 0.2E1
     # - t977 * t34 / 0.6E1 - (t983 + t954 + t956 - t985 + t958 - t960 +
     # t975 - t977 * t647) * t34 / 0.2E1) * t12 + (t1313 * t34 / 0.6E1 +
     # (t1319 + t1284 + t1286 - t1321 + t1288 - t1290 + t1311 - t1313 * 
     #t647) * t34 / 0.2E1 - t1570 * t34 / 0.6E1 - (t1576 + t1547 + t1549
     # - t1578 + t1551 - t1553 + t1568 - t1570 * t647) * t34 / 0.2E1) * 
     #t49 + t1590 * t34 / 0.6E1 + (t1588 - t1590 * t647) * t34 / 0.2E1

        utnew(i,j) = 
     #t2 + (t690 * dt / 0.2E1 + (t696 + t649 + t653 - t698 + t657 - 
     #t660 + t688) * dt - t690 * t658 - t977 * dt / 0.2E1 - (t983 + t954
     # + t956 - t985 + t958 - t960 + t975) * dt + t977 * t658) * t12 + (
     #t1313 * dt / 0.2E1 + (t1319 + t1284 + t1286 - t1321 + t1288 - t129
     #0 + t1311) * dt - t1313 * t658 - t1570 * dt / 0.2E1 - (t1576 + t15
     #47 + t1549 - t1578 + t1551 - t1553 + t1568) * dt + t1570 * t658) *
     # t49 + t1590 * dt / 0.2E1 + t1588 * dt - t1590 * t658

c        blah = array(int(t1 + dt * t2 + (t690 * t34 / 0.6E1 + (t696 + t6
c     #49 + t653 - t698 + t657 - t660 + t688 - t690 * t647) * t34 / 0.2E1
c     # - t977 * t34 / 0.6E1 - (t983 + t954 + t956 - t985 + t958 - t960 +
c     # t975 - t977 * t647) * t34 / 0.2E1) * t12 + (t1313 * t34 / 0.6E1 +
c     # (t1319 + t1284 + t1286 - t1321 + t1288 - t1290 + t1311 - t1313 * 
c     #t647) * t34 / 0.2E1 - t1570 * t34 / 0.6E1 - (t1576 + t1547 + t1549
c     # - t1578 + t1551 - t1553 + t1568 - t1570 * t647) * t34 / 0.2E1) * 
c     #t49 + t1590 * t34 / 0.6E1 + (t1588 - t1590 * t647) * t34 / 0.2E1),
c     #int(t2 + (t690 * dt / 0.2E1 + (t696 + t649 + t653 - t698 + t657 - 
c     #t660 + t688) * dt - t690 * t658 - t977 * dt / 0.2E1 - (t983 + t954
c     # + t956 - t985 + t958 - t960 + t975) * dt + t977 * t658) * t12 + (
c     #t1313 * dt / 0.2E1 + (t1319 + t1284 + t1286 - t1321 + t1288 - t129
c     #0 + t1311) * dt - t1313 * t658 - t1570 * dt / 0.2E1 - (t1576 + t15
c     #47 + t1549 - t1578 + t1551 - t1553 + t1568) * dt + t1570 * t658) *
c     # t49 + t1590 * dt / 0.2E1 + t1588 * dt - t1590 * t658))


        return
      end
