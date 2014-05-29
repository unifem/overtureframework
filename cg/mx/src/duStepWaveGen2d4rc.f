      subroutine duStepWaveGen2d4rc( 
     *   nd1a,nd1b,nd2a,nd2b,
     *   n1a,n1b,n2a,n2b,
     *   u,ut,unew,utnew,
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
      real dx,dy,dt,cc,beta
c
c.. generated code to follow
c
        real t1
        real t10
        real t1000
        real t1002
        real t1006
        real t101
        real t102
        real t103
        real t1039
        real t1048
        real t105
        real t1051
        real t106
        real t1060
        real t1061
        real t1062
        real t1065
        real t107
        real t1073
        real t1087
        real t109
        real t1090
        real t11
        real t1101
        real t1104
        real t1108
        real t111
        real t1111
        real t1112
        real t1119
        real t112
        real t1120
        real t1123
        real t1125
        real t1131
        real t1132
        real t1134
        real t1135
        real t1137
        real t1139
        real t114
        real t1140
        real t1141
        real t1144
        real t1146
        real t1148
        real t1150
        real t1151
        real t1155
        real t1157
        real t1159
        real t1161
        real t117
        real t1174
        real t1176
        real t1178
        real t1179
        real t118
        real t1182
        real t1184
        real t119
        real t1190
        real t1192
        real t12
        real t1200
        real t1203
        real t1207
        real t1210
        real t1214
        real t1217
        real t1219
        real t122
        real t123
        integer t124
        integer t1240
        real t1242
        real t1254
        real t126
        real t1263
        real t1267
        real t1271
        real t1272
        real t1279
        real t128
        real t1280
        real t1282
        real t1285
        real t1287
        real t1291
        real t13
        real t131
        real t1324
        real t133
        real t1333
        real t1344
        real t1345
        real t1348
        real t1356
        real t137
        real t1370
        real t1385
        real t1389
        real t139
        real t1392
        real t1396
        real t1398
        integer t14
        real t140
        real t1400
        real t1402
        real t1414
        real t1417
        real t1419
        real t142
        real t1425
        real t1427
        real t148
        real t15
        real t151
        real t152
        real t158
        integer t159
        real t16
        real t160
        real t161
        real t163
        real t166
        real t168
        real t17
        integer t172
        real t173
        real t174
        real t186
        real t19
        real t192
        real t2
        integer t20
        real t201
        real t204
        real t206
        real t21
        real t210
        real t211
        real t212
        real t219
        real t22
        real t220
        real t221
        real t225
        real t23
        real t231
        real t234
        real t240
        real t241
        real t243
        real t246
        real t248
        real t25
        real t252
        real t253
        real t26
        real t280
        real t283
        real t284
        real t288
        real t294
        real t295
        real t296
        real t299
        real t30
        real t307
        real t308
        real t309
        real t313
        real t316
        real t319
        real t32
        real t321
        real t323
        real t327
        real t33
        real t330
        real t332
        real t334
        real t34
        real t341
        real t344
        real t345
        real t349
        real t35
        real t357
        real t36
        real t360
        real t364
        integer t367
        real t368
        real t369
        real t37
        real t370
        real t372
        real t373
        real t375
        real t379
        real t381
        real t382
        real t383
        real t389
        real t39
        real t390
        real t391
        real t392
        real t394
        real t395
        real t397
        real t398
        real t4
        real t40
        real t400
        real t401
        real t402
        real t403
        real t405
        real t406
        real t408
        real t412
        real t414
        real t415
        real t416
        real t418
        real t42
        real t420
        real t421
        real t422
        real t429
        real t430
        real t431
        real t432
        real t433
        real t434
        real t436
        real t437
        real t438
        real t44
        real t445
        real t447
        integer t45
        real t451
        real t453
        real t454
        real t455
        real t46
        real t461
        real t462
        real t463
        real t464
        real t466
        real t467
        real t469
        real t47
        real t470
        real t472
        real t473
        real t474
        real t475
        real t477
        real t478
        real t480
        real t484
        real t486
        real t487
        real t488
        real t49
        real t490
        real t492
        real t493
        real t494
        real t5
        real t50
        real t501
        real t503
        real t504
        real t505
        real t507
        real t508
        real t509
        integer t51
        real t511
        real t513
        real t514
        real t515
        real t518
        real t52
        real t520
        real t526
        real t528
        real t53
        real t531
        real t533
        real t535
        real t536
        real t538
        real t539
        real t541
        real t543
        real t544
        real t546
        real t55
        real t550
        real t552
        real t553
        real t554
        real t556
        real t557
        real t558
        real t560
        real t562
        real t563
        real t564
        real t567
        real t569
        real t57
        real t571
        real t573
        real t574
        real t576
        real t579
        real t58
        real t580
        real t581
        real t582
        real t583
        real t585
        real t586
        real t587
        real t589
        real t59
        real t590
        real t592
        real t593
        real t595
        real t6
        real t600
        real t607
        real t609
        real t61
        real t611
        real t613
        real t615
        real t617
        real t618
        real t621
        real t623
        real t629
        real t63
        real t631
        real t639
        real t64
        real t642
        real t646
        real t649
        real t65
        integer t652
        real t654
        real t666
        real t67
        real t674
        real t675
        real t677
        real t68
        real t680
        real t682
        real t686
        real t687
        real t69
        real t699
        real t7
        real t705
        real t71
        real t714
        real t718
        real t722
        real t723
        real t73
        real t730
        real t738
        real t74
        real t746
        real t747
        real t749
        real t752
        real t754
        real t758
        real t759
        real t76
        real t786
        real t789
        real t79
        real t793
        real t799
        real t8
        real t80
        real t800
        real t803
        real t81
        real t811
        real t816
        real t820
        real t824
        real t828
        real t83
        real t835
        real t838
        real t842
        real t85
        real t852
        real t856
        real t859
        real t863
        real t865
        real t867
        real t869
        real t87
        real t88
        real t881
        real t884
        real t886
        real t89
        real t892
        real t894
        integer t9
        real t904
        real t907
        real t91
        real t910
        real t912
        real t913
        real t915
        real t917
        real t918
        real t92
        real t920
        real t923
        real t927
        real t93
        real t930
        real t932
        real t95
        integer t953
        real t955
        real t967
        real t97
        real t976
        real t979
        real t981
        real t985
        real t986
        real t987
        real t99
        real t994
        real t995
        real t997
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
        t58 = u(t20,j,n)
        t59 = t1 - t58
        t61 = t4 * t59 * t12
        t63 = (t42 - t61) * t12
        t64 = u(i,t45,n)
        t65 = t64 - t1
        t67 = t4 * t65 * t49
        t68 = u(i,t51,n)
        t69 = t1 - t68
        t71 = t4 * t69 * t49
        t73 = (t67 - t71) * t49
        t74 = t44 + t57 - t63 - t73
        t76 = t34 * t74 * t12
        t79 = t32 * t7
        t80 = t4 * t79
        t81 = t34 * dt
        t83 = t4 * t16 * t12
        t85 = t4 * t11 * t12
        t87 = (t83 - t85) * t12
        t88 = ut(t9,t45,n)
        t89 = t88 - t10
        t91 = t4 * t89 * t49
        t92 = ut(t9,t51,n)
        t93 = t10 - t92
        t95 = t4 * t93 * t49
        t97 = (t91 - t95) * t49
        t99 = t4 * t22 * t12
        t101 = (t85 - t99) * t12
        t102 = ut(i,t45,n)
        t103 = t102 - t2
        t105 = t4 * t103 * t49
        t106 = ut(i,t51,n)
        t107 = t2 - t106
        t109 = t4 * t107 * t49
        t111 = (t105 - t109) * t49
        t112 = t87 + t97 - t101 - t111
        t114 = t81 * t112 * t12
        t117 = t7 * dt
        t118 = t87 - t101
        t119 = dx * t118
        t122 = beta * t7
        t123 = dx ** 2
        t124 = i + 3
        t126 = u(t124,j,n) - t35
        t128 = t37 * t12
        t131 = t40 * t12
        t133 = (t128 - t131) * t12
        t137 = t59 * t12
        t139 = (t131 - t137) * t12
        t140 = t133 - t139
        t142 = t4 * t140 * t12
        t148 = (t4 * t126 * t12 - t39) * t12
        t151 = t44 - t63
        t152 = t151 * t12
        t158 = dy ** 2
        t159 = j + 2
        t160 = u(t9,t159,n)
        t161 = t160 - t46
        t163 = t47 * t49
        t166 = t53 * t49
        t168 = (t163 - t166) * t49
        t172 = j - 2
        t173 = u(t9,t172,n)
        t174 = t52 - t173
        t186 = (t4 * t161 * t49 - t50) * t49
        t192 = (t55 - t4 * t174 * t49) * t49
        t201 = dt * (t44 - t123 * ((t4 * ((t126 * t12 - t128) * t12 - t1
     #33) * t12 - t142) * t12 + ((t148 - t44) * t12 - t152) * t12) / 0.2
     #4E2 + t57 - t158 * ((t4 * ((t161 * t49 - t163) * t49 - t168) * t49
     # - t4 * (t168 - (t166 - t174 * t49) * t49) * t49) * t49 + ((t186 -
     # t57) * t49 - (t57 - t192) * t49) * t49) / 0.24E2)
        t204 = t13 / 0.2E1
        t206 = ut(t124,j,n) - t15
        t210 = (t206 * t12 - t17) * t12 - t19
        t211 = t210 * t12
        t212 = t26 * t12
        t219 = dx * (t17 / 0.2E1 + t204 - t123 * (t211 / 0.2E1 + t212 / 
     #0.2E1) / 0.6E1) / 0.2E1
        t220 = beta ** 2
        t221 = t220 * t32
        t225 = t4 * t26 * t12
        t231 = (t4 * t206 * t12 - t83) * t12
        t234 = t118 * t12
        t240 = ut(t9,t159,n)
        t241 = t240 - t88
        t243 = t89 * t49
        t246 = t93 * t49
        t248 = (t243 - t246) * t49
        t252 = ut(t9,t172,n)
        t253 = t92 - t252
        t280 = t34 * (t87 - t123 * ((t4 * t210 * t12 - t225) * t12 + ((t
     #231 - t87) * t12 - t234) * t12) / 0.24E2 + t97 - t158 * ((t4 * ((t
     #241 * t49 - t243) * t49 - t248) * t49 - t4 * (t248 - (t246 - t253 
     #* t49) * t49) * t49) * t49 + (((t4 * t241 * t49 - t91) * t49 - t97
     #) * t49 - (t97 - (t95 - t4 * t253 * t49) * t49) * t49) * t49) / 0.
     #24E2)
        t283 = dt * dx
        t284 = u(t14,t45,n)
        t288 = u(t14,t51,n)
        t294 = t148 + (t4 * (t284 - t35) * t49 - t4 * (t35 - t288) * t49
     #) * t49 - t44 - t57
        t295 = t294 * t12
        t296 = t74 * t12
        t299 = t283 * (t295 / 0.2E1 + t296 / 0.2E1)
        t307 = t123 * (t19 - dx * (t211 - t212) / 0.12E2) / 0.12E2
        t308 = t220 * beta
        t309 = t308 * t79
        t313 = t4 * t74 * t12
        t316 = t284 - t46
        t319 = t46 - t64
        t321 = t4 * t319 * t12
        t323 = (t4 * t316 * t12 - t321) * t12
        t327 = t288 - t52
        t330 = t52 - t68
        t332 = t4 * t330 * t12
        t334 = (t4 * t327 * t12 - t332) * t12
        t341 = t81 * ((t4 * t294 * t12 - t313) * t12 + (t4 * (t323 + t18
     #6 - t44 - t57) * t49 - t4 * (t44 + t57 - t334 - t192) * t49) * t49
     #)
        t344 = t34 * dx
        t345 = ut(t14,t45,n)
        t349 = ut(t14,t51,n)
        t357 = t112 * t12
        t360 = t344 * ((t231 + (t4 * (t345 - t15) * t49 - t4 * (t15 - t3
     #49) * t49) * t49 - t87 - t97) * t12 / 0.2E1 + t357 / 0.2E1)
        t364 = t283 * (t295 - t296)
        t367 = i - 2
        t368 = u(t367,j,n)
        t369 = t58 - t368
        t370 = t369 * t12
        t372 = (t137 - t370) * t12
        t373 = t139 - t372
        t375 = t4 * t373 * t12
        t379 = t4 * t369 * t12
        t381 = (t61 - t379) * t12
        t382 = t63 - t381
        t383 = t382 * t12
        t389 = u(i,t159,n)
        t390 = t389 - t64
        t391 = t390 * t49
        t392 = t65 * t49
        t394 = (t391 - t392) * t49
        t395 = t69 * t49
        t397 = (t392 - t395) * t49
        t398 = t394 - t397
        t400 = t4 * t398 * t49
        t401 = u(i,t172,n)
        t402 = t68 - t401
        t403 = t402 * t49
        t405 = (t395 - t403) * t49
        t406 = t397 - t405
        t408 = t4 * t406 * t49
        t412 = t4 * t390 * t49
        t414 = (t412 - t67) * t49
        t415 = t414 - t73
        t416 = t415 * t49
        t418 = t4 * t402 * t49
        t420 = (t71 - t418) * t49
        t421 = t73 - t420
        t422 = t421 * t49
        t429 = dt * (t63 - t123 * ((t142 - t375) * t12 + (t152 - t383) *
     # t12) / 0.24E2 + t73 - t158 * ((t400 - t408) * t49 + (t416 - t422)
     # * t49) / 0.24E2)
        t430 = t122 * t429
        t431 = t23 / 0.2E1
        t432 = ut(t367,j,n)
        t433 = t21 - t432
        t434 = t433 * t12
        t436 = (t23 - t434) * t12
        t437 = t25 - t436
        t438 = t437 * t12
        t445 = dx * (t204 + t431 - t123 * (t212 / 0.2E1 + t438 / 0.2E1) 
     #/ 0.6E1) / 0.2E1
        t447 = t4 * t437 * t12
        t451 = t4 * t433 * t12
        t453 = (t99 - t451) * t12
        t454 = t101 - t453
        t455 = t454 * t12
        t461 = ut(i,t159,n)
        t462 = t461 - t102
        t463 = t462 * t49
        t464 = t103 * t49
        t466 = (t463 - t464) * t49
        t467 = t107 * t49
        t469 = (t464 - t467) * t49
        t470 = t466 - t469
        t472 = t4 * t470 * t49
        t473 = ut(i,t172,n)
        t474 = t106 - t473
        t475 = t474 * t49
        t477 = (t467 - t475) * t49
        t478 = t469 - t477
        t480 = t4 * t478 * t49
        t484 = t4 * t462 * t49
        t486 = (t484 - t105) * t49
        t487 = t486 - t111
        t488 = t487 * t49
        t490 = t4 * t474 * t49
        t492 = (t109 - t490) * t49
        t493 = t111 - t492
        t494 = t493 * t49
        t501 = t34 * (t101 - t123 * ((t225 - t447) * t12 + (t234 - t455)
     # * t12) / 0.24E2 + t111 - t158 * ((t472 - t480) * t49 + (t488 - t4
     #94) * t49) / 0.24E2)
        t503 = t221 * t501 / 0.2E1
        t504 = u(t20,t45,n)
        t505 = t504 - t58
        t507 = t4 * t505 * t49
        t508 = u(t20,t51,n)
        t509 = t58 - t508
        t511 = t4 * t509 * t49
        t513 = (t507 - t511) * t49
        t514 = t63 + t73 - t381 - t513
        t515 = t514 * t12
        t518 = t283 * (t296 / 0.2E1 + t515 / 0.2E1)
        t520 = t122 * t518 / 0.2E1
        t526 = t123 * (t25 - dx * (t212 - t438) / 0.12E2) / 0.12E2
        t528 = t4 * t514 * t12
        t531 = t64 - t504
        t533 = t4 * t531 * t12
        t535 = (t321 - t533) * t12
        t536 = t535 + t414 - t63 - t73
        t538 = t4 * t536 * t49
        t539 = t68 - t508
        t541 = t4 * t539 * t12
        t543 = (t332 - t541) * t12
        t544 = t63 + t73 - t543 - t420
        t546 = t4 * t544 * t49
        t550 = t81 * ((t313 - t528) * t12 + (t538 - t546) * t49)
        t552 = t309 * t550 / 0.6E1
        t553 = ut(t20,t45,n)
        t554 = t553 - t21
        t556 = t4 * t554 * t49
        t557 = ut(t20,t51,n)
        t558 = t21 - t557
        t560 = t4 * t558 * t49
        t562 = (t556 - t560) * t49
        t563 = t101 + t111 - t453 - t562
        t564 = t563 * t12
        t567 = t344 * (t357 / 0.2E1 + t564 / 0.2E1)
        t569 = t221 * t567 / 0.4E1
        t571 = t283 * (t296 - t515)
        t573 = t122 * t571 / 0.12E2
        t574 = t10 + t122 * t201 - t219 + t221 * t280 / 0.2E1 - t122 * t
     #299 / 0.2E1 + t307 + t309 * t341 / 0.6E1 - t221 * t360 / 0.4E1 + t
     #122 * t364 / 0.12E2 - t2 - t430 - t445 - t503 - t520 - t526 - t552
     # - t569 - t573
        t576 = sqrt(0.16E2)
        t579 = 0.1E1 / 0.2E1 - t6
        t580 = t4 * t579
        t581 = t580 * t30
        t582 = t579 ** 2
        t583 = t4 * t582
        t585 = t583 * t76 / 0.2E1
        t586 = t582 * t579
        t587 = t4 * t586
        t589 = t587 * t114 / 0.6E1
        t590 = t579 * dt
        t592 = t590 * t119 / 0.24E2
        t593 = beta * t579
        t595 = t220 * t582
        t600 = t308 * t586
        t607 = t593 * t429
        t609 = t595 * t501 / 0.2E1
        t611 = t593 * t518 / 0.2E1
        t613 = t600 * t550 / 0.6E1
        t615 = t595 * t567 / 0.4E1
        t617 = t593 * t571 / 0.12E2
        t618 = t10 + t593 * t201 - t219 + t595 * t280 / 0.2E1 - t593 * t
     #299 / 0.2E1 + t307 + t600 * t341 / 0.6E1 - t595 * t360 / 0.4E1 + t
     #593 * t364 / 0.12E2 - t2 - t607 - t445 - t609 - t611 - t526 - t613
     # - t615 - t617
        t621 = cc * t618 * t576 / 0.8E1
        t623 = (t8 * t30 + t33 * t76 / 0.2E1 + t80 * t114 / 0.6E1 - t117
     # * t119 / 0.24E2 + cc * t574 * t576 / 0.8E1 - t581 - t585 - t589 +
     # t592 - t621) * t5
        t629 = t4 * (t131 - dx * t140 / 0.24E2)
        t631 = dx * t151 / 0.24E2
        t639 = dt * (t23 - dx * t437 / 0.24E2)
        t642 = t34 * t514 * t12
        t646 = t81 * t563 * t12
        t649 = dx * t454
        t652 = i - 3
        t654 = t368 - u(t652,j,n)
        t666 = (t379 - t4 * t654 * t12) * t12
        t674 = u(t20,t159,n)
        t675 = t674 - t504
        t677 = t505 * t49
        t680 = t509 * t49
        t682 = (t677 - t680) * t49
        t686 = u(t20,t172,n)
        t687 = t508 - t686
        t699 = (t4 * t675 * t49 - t507) * t49
        t705 = (t511 - t4 * t687 * t49) * t49
        t714 = dt * (t381 - t123 * ((t375 - t4 * (t372 - (t370 - t654 * 
     #t12) * t12) * t12) * t12 + (t383 - (t381 - t666) * t12) * t12) / 0
     #.24E2 + t513 - t158 * ((t4 * ((t675 * t49 - t677) * t49 - t682) * 
     #t49 - t4 * (t682 - (t680 - t687 * t49) * t49) * t49) * t49 + ((t69
     #9 - t513) * t49 - (t513 - t705) * t49) * t49) / 0.24E2)
        t718 = t432 - ut(t652,j,n)
        t722 = t436 - (t434 - t718 * t12) * t12
        t723 = t722 * t12
        t730 = dx * (t431 + t434 / 0.2E1 - t123 * (t438 / 0.2E1 + t723 /
     # 0.2E1) / 0.6E1) / 0.2E1
        t738 = (t451 - t4 * t718 * t12) * t12
        t746 = ut(t20,t159,n)
        t747 = t746 - t553
        t749 = t554 * t49
        t752 = t558 * t49
        t754 = (t749 - t752) * t49
        t758 = ut(t20,t172,n)
        t759 = t557 - t758
        t786 = t34 * (t453 - t123 * ((t447 - t4 * t722 * t12) * t12 + (t
     #455 - (t453 - t738) * t12) * t12) / 0.24E2 + t562 - t158 * ((t4 * 
     #((t747 * t49 - t749) * t49 - t754) * t49 - t4 * (t754 - (t752 - t7
     #59 * t49) * t49) * t49) * t49 + (((t4 * t747 * t49 - t556) * t49 -
     # t562) * t49 - (t562 - (t560 - t4 * t759 * t49) * t49) * t49) * t4
     #9) / 0.24E2)
        t789 = u(t367,t45,n)
        t793 = u(t367,t51,n)
        t799 = t381 + t513 - t666 - (t4 * (t789 - t368) * t49 - t4 * (t3
     #68 - t793) * t49) * t49
        t800 = t799 * t12
        t803 = t283 * (t515 / 0.2E1 + t800 / 0.2E1)
        t811 = t123 * (t436 - dx * (t438 - t723) / 0.12E2) / 0.12E2
        t816 = t504 - t789
        t820 = (t533 - t4 * t816 * t12) * t12
        t824 = t508 - t793
        t828 = (t541 - t4 * t824 * t12) * t12
        t835 = t81 * ((t528 - t4 * t799 * t12) * t12 + (t4 * (t820 + t69
     #9 - t381 - t513) * t49 - t4 * (t381 + t513 - t828 - t705) * t49) *
     # t49)
        t838 = ut(t367,t45,n)
        t842 = ut(t367,t51,n)
        t852 = t344 * (t564 / 0.2E1 + (t453 + t562 - t738 - (t4 * (t838 
     #- t432) * t49 - t4 * (t432 - t842) * t49) * t49) * t12 / 0.2E1)
        t856 = t283 * (t515 - t800)
        t859 = t2 + t430 - t445 + t503 - t520 + t526 + t552 - t569 + t57
     #3 - t21 - t122 * t714 - t730 - t221 * t786 / 0.2E1 - t122 * t803 /
     # 0.2E1 - t811 - t309 * t835 / 0.6E1 - t221 * t852 / 0.4E1 - t122 *
     # t856 / 0.12E2
        t863 = t580 * t639
        t865 = t583 * t642 / 0.2E1
        t867 = t587 * t646 / 0.6E1
        t869 = t590 * t649 / 0.24E2
        t881 = t2 + t607 - t445 + t609 - t611 + t526 + t613 - t615 + t61
     #7 - t21 - t593 * t714 - t730 - t595 * t786 / 0.2E1 - t593 * t803 /
     # 0.2E1 - t811 - t600 * t835 / 0.6E1 - t595 * t852 / 0.4E1 - t593 *
     # t856 / 0.12E2
        t884 = cc * t881 * t576 / 0.8E1
        t886 = (t8 * t639 + t33 * t642 / 0.2E1 + t80 * t646 / 0.6E1 - t1
     #17 * t649 / 0.24E2 + cc * t859 * t576 / 0.8E1 - t863 - t865 - t867
     # + t869 - t884) * t5
        t892 = t4 * (t137 - dx * t373 / 0.24E2)
        t894 = dx * t382 / 0.24E2
        t904 = dt * (t464 - dy * t470 / 0.24E2)
        t907 = t34 * t536 * t49
        t910 = t88 - t102
        t912 = t4 * t910 * t12
        t913 = t102 - t553
        t915 = t4 * t913 * t12
        t917 = (t912 - t915) * t12
        t918 = t917 + t486 - t101 - t111
        t920 = t81 * t918 * t49
        t923 = dy * t487
        t927 = t319 * t12
        t930 = t531 * t12
        t932 = (t927 - t930) * t12
        t953 = j + 3
        t955 = u(i,t953,n) - t389
        t967 = (t4 * t955 * t49 - t412) * t49
        t976 = dt * (t535 - t123 * ((t4 * ((t316 * t12 - t927) * t12 - t
     #932) * t12 - t4 * (t932 - (t930 - t816 * t12) * t12) * t12) * t12 
     #+ ((t323 - t535) * t12 - (t535 - t820) * t12) * t12) / 0.24E2 + t4
     #14 - t158 * ((t4 * ((t955 * t49 - t391) * t49 - t394) * t49 - t400
     #) * t49 + ((t967 - t414) * t49 - t416) * t49) / 0.24E2)
        t979 = t464 / 0.2E1
        t981 = ut(i,t953,n) - t461
        t985 = (t981 * t49 - t463) * t49 - t466
        t986 = t985 * t49
        t987 = t470 * t49
        t994 = dy * (t463 / 0.2E1 + t979 - t158 * (t986 / 0.2E1 + t987 /
     # 0.2E1) / 0.6E1) / 0.2E1
        t995 = t345 - t88
        t997 = t910 * t12
        t1000 = t913 * t12
        t1002 = (t997 - t1000) * t12
        t1006 = t553 - t838
        t1039 = (t4 * t981 * t49 - t484) * t49
        t1048 = t34 * (t917 - t123 * ((t4 * ((t995 * t12 - t997) * t12 -
     # t1002) * t12 - t4 * (t1002 - (t1000 - t1006 * t12) * t12) * t12) 
     #* t12 + (((t4 * t995 * t12 - t912) * t12 - t917) * t12 - (t917 - (
     #t915 - t4 * t1006 * t12) * t12) * t12) * t12) / 0.24E2 + t486 - t1
     #58 * ((t4 * t985 * t49 - t472) * t49 + ((t1039 - t486) * t49 - t48
     #8) * t49) / 0.24E2)
        t1051 = dt * dy
        t1060 = (t4 * (t160 - t389) * t12 - t4 * (t389 - t674) * t12) * 
     #t12 + t967 - t535 - t414
        t1061 = t1060 * t49
        t1062 = t536 * t49
        t1065 = t1051 * (t1061 / 0.2E1 + t1062 / 0.2E1)
        t1073 = t158 * (t466 - dy * (t986 - t987) / 0.12E2) / 0.12E2
        t1087 = t81 * ((t4 * (t323 + t186 - t535 - t414) * t12 - t4 * (t
     #535 + t414 - t820 - t699) * t12) * t12 + (t4 * t1060 * t49 - t538)
     # * t49)
        t1090 = t34 * dy
        t1101 = t918 * t49
        t1104 = t1090 * (((t4 * (t240 - t461) * t12 - t4 * (t461 - t746)
     # * t12) * t12 + t1039 - t917 - t486) * t49 / 0.2E1 + t1101 / 0.2E1
     #)
        t1108 = t1051 * (t1061 - t1062)
        t1111 = t467 / 0.2E1
        t1112 = t478 * t49
        t1119 = dy * (t979 + t1111 - t158 * (t987 / 0.2E1 + t1112 / 0.2E
     #1) / 0.6E1) / 0.2E1
        t1120 = t544 * t49
        t1123 = t1051 * (t1062 / 0.2E1 + t1120 / 0.2E1)
        t1125 = t122 * t1123 / 0.2E1
        t1131 = t158 * (t469 - dy * (t987 - t1112) / 0.12E2) / 0.12E2
        t1132 = t92 - t106
        t1134 = t4 * t1132 * t12
        t1135 = t106 - t557
        t1137 = t4 * t1135 * t12
        t1139 = (t1134 - t1137) * t12
        t1140 = t101 + t111 - t1139 - t492
        t1141 = t1140 * t49
        t1144 = t1090 * (t1101 / 0.2E1 + t1141 / 0.2E1)
        t1146 = t221 * t1144 / 0.4E1
        t1148 = t1051 * (t1062 - t1120)
        t1150 = t122 * t1148 / 0.12E2
        t1151 = t102 + t122 * t976 - t994 + t221 * t1048 / 0.2E1 - t122 
     #* t1065 / 0.2E1 + t1073 + t309 * t1087 / 0.6E1 - t221 * t1104 / 0.
     #4E1 + t122 * t1108 / 0.12E2 - t2 - t430 - t1119 - t503 - t1125 - t
     #1131 - t552 - t1146 - t1150
        t1155 = t580 * t904
        t1157 = t583 * t907 / 0.2E1
        t1159 = t587 * t920 / 0.6E1
        t1161 = t590 * t923 / 0.24E2
        t1174 = t593 * t1123 / 0.2E1
        t1176 = t595 * t1144 / 0.4E1
        t1178 = t593 * t1148 / 0.12E2
        t1179 = t102 + t593 * t976 - t994 + t595 * t1048 / 0.2E1 - t593 
     #* t1065 / 0.2E1 + t1073 + t600 * t1087 / 0.6E1 - t595 * t1104 / 0.
     #4E1 + t593 * t1108 / 0.12E2 - t2 - t607 - t1119 - t609 - t1174 - t
     #1131 - t613 - t1176 - t1178
        t1182 = cc * t1179 * t576 / 0.8E1
        t1184 = (t8 * t904 + t33 * t907 / 0.2E1 + t80 * t920 / 0.6E1 - t
     #117 * t923 / 0.24E2 + cc * t1151 * t576 / 0.8E1 - t1155 - t1157 - 
     #t1159 + t1161 - t1182) * t5
        t1190 = t4 * (t392 - dy * t398 / 0.24E2)
        t1192 = dy * t415 / 0.24E2
        t1200 = dt * (t467 - dy * t478 / 0.24E2)
        t1203 = t34 * t544 * t49
        t1207 = t81 * t1140 * t49
        t1210 = dy * t493
        t1214 = t330 * t12
        t1217 = t539 * t12
        t1219 = (t1214 - t1217) * t12
        t1240 = j - 3
        t1242 = t401 - u(i,t1240,n)
        t1254 = (t418 - t4 * t1242 * t49) * t49
        t1263 = dt * (t543 - t123 * ((t4 * ((t327 * t12 - t1214) * t12 -
     # t1219) * t12 - t4 * (t1219 - (t1217 - t824 * t12) * t12) * t12) *
     # t12 + ((t334 - t543) * t12 - (t543 - t828) * t12) * t12) / 0.24E2
     # + t420 - t158 * ((t408 - t4 * (t405 - (t403 - t1242 * t49) * t49)
     # * t49) * t49 + (t422 - (t420 - t1254) * t49) * t49) / 0.24E2)
        t1267 = t473 - ut(i,t1240,n)
        t1271 = t477 - (t475 - t1267 * t49) * t49
        t1272 = t1271 * t49
        t1279 = dy * (t1111 + t475 / 0.2E1 - t158 * (t1112 / 0.2E1 + t12
     #72 / 0.2E1) / 0.6E1) / 0.2E1
        t1280 = t349 - t92
        t1282 = t1132 * t12
        t1285 = t1135 * t12
        t1287 = (t1282 - t1285) * t12
        t1291 = t557 - t842
        t1324 = (t490 - t4 * t1267 * t49) * t49
        t1333 = t34 * (t1139 - t123 * ((t4 * ((t1280 * t12 - t1282) * t1
     #2 - t1287) * t12 - t4 * (t1287 - (t1285 - t1291 * t12) * t12) * t1
     #2) * t12 + (((t4 * t1280 * t12 - t1134) * t12 - t1139) * t12 - (t1
     #139 - (t1137 - t4 * t1291 * t12) * t12) * t12) * t12) / 0.24E2 + t
     #492 - t158 * ((t480 - t4 * t1271 * t49) * t49 + (t494 - (t492 - t1
     #324) * t49) * t49) / 0.24E2)
        t1344 = t543 + t420 - (t4 * (t173 - t401) * t12 - t4 * (t401 - t
     #686) * t12) * t12 - t1254
        t1345 = t1344 * t49
        t1348 = t1051 * (t1120 / 0.2E1 + t1345 / 0.2E1)
        t1356 = t158 * (t477 - dy * (t1112 - t1272) / 0.12E2) / 0.12E2
        t1370 = t81 * ((t4 * (t334 + t192 - t543 - t420) * t12 - t4 * (t
     #543 + t420 - t828 - t705) * t12) * t12 + (t546 - t4 * t1344 * t49)
     # * t49)
        t1385 = t1090 * (t1141 / 0.2E1 + (t1139 + t492 - (t4 * (t252 - t
     #473) * t12 - t4 * (t473 - t758) * t12) * t12 - t1324) * t49 / 0.2E
     #1)
        t1389 = t1051 * (t1120 - t1345)
        t1392 = t2 + t430 - t1119 + t503 - t1125 + t1131 + t552 - t1146 
     #+ t1150 - t106 - t122 * t1263 - t1279 - t221 * t1333 / 0.2E1 - t12
     #2 * t1348 / 0.2E1 - t1356 - t309 * t1370 / 0.6E1 - t221 * t1385 / 
     #0.4E1 - t122 * t1389 / 0.12E2
        t1396 = t580 * t1200
        t1398 = t583 * t1203 / 0.2E1
        t1400 = t587 * t1207 / 0.6E1
        t1402 = t590 * t1210 / 0.24E2
        t1414 = t2 + t607 - t1119 + t609 - t1174 + t1131 + t613 - t1176 
     #+ t1178 - t106 - t593 * t1263 - t1279 - t595 * t1333 / 0.2E1 - t59
     #3 * t1348 / 0.2E1 - t1356 - t600 * t1370 / 0.6E1 - t595 * t1385 / 
     #0.4E1 - t593 * t1389 / 0.12E2
        t1417 = cc * t1414 * t576 / 0.8E1
        t1419 = (t8 * t1200 + t33 * t1203 / 0.2E1 + t80 * t1207 / 0.6E1 
     #- t117 * t1210 / 0.24E2 + cc * t1392 * t576 / 0.8E1 - t1396 - t139
     #8 - t1400 + t1402 - t1417) * t5
        t1425 = t4 * (t395 - dy * t406 / 0.24E2)
        t1427 = dy * t421 / 0.24E2


        unew(i,j) = t1 + dt * t2 + (t623 * t34 / 0.6E1 + (t629 + t5
     #81 + t585 - t631 + t589 - t592 + t621 - t623 * t579) * t34 / 0.2E1
     # - t886 * t34 / 0.6E1 - (t892 + t863 + t865 - t894 + t867 - t869 +
     # t884 - t886 * t579) * t34 / 0.2E1) * t12 + (t1184 * t34 / 0.6E1 +
     # (t1190 + t1155 + t1157 - t1192 + t1159 - t1161 + t1182 - t1184 * 
     #t579) * t34 / 0.2E1 - t1419 * t34 / 0.6E1 - (t1425 + t1396 + t1398
     # - t1427 + t1400 - t1402 + t1417 - t1419 * t579) * t34 / 0.2E1) * 
     #t49

        utnew(i,j) = 
     #t2 + (t623 * dt / 0.2E1 + (t629 + t581 + t585 - t631 + t5
     #89 - t592 + t621) * dt - t623 * t590 - t886 * dt / 0.2E1 - (t892 +
     # t863 + t865 - t894 + t867 - t869 + t884) * dt + t886 * t590) * t1
     #2 + (t1184 * dt / 0.2E1 + (t1190 + t1155 + t1157 - t1192 + t1159 -
     # t1161 + t1182) * dt - t1184 * t590 - t1419 * dt / 0.2E1 - (t1425 
     #+ t1396 + t1398 - t1427 + t1400 - t1402 + t1417) * dt + t1419 * t5
     #90) * t49

c        blah = array(int(t1 + dt * t2 + (t623 * t34 / 0.6E1 + (t629 + t5
c     #81 + t585 - t631 + t589 - t592 + t621 - t623 * t579) * t34 / 0.2E1
c     # - t886 * t34 / 0.6E1 - (t892 + t863 + t865 - t894 + t867 - t869 +
c     # t884 - t886 * t579) * t34 / 0.2E1) * t12 + (t1184 * t34 / 0.6E1 +
c     # (t1190 + t1155 + t1157 - t1192 + t1159 - t1161 + t1182 - t1184 * 
c     #t579) * t34 / 0.2E1 - t1419 * t34 / 0.6E1 - (t1425 + t1396 + t1398
c     # - t1427 + t1400 - t1402 + t1417 - t1419 * t579) * t34 / 0.2E1) * 
c     #t49),int(t2 + (t623 * dt / 0.2E1 + (t629 + t581 + t585 - t631 + t5
c     #89 - t592 + t621) * dt - t623 * t590 - t886 * dt / 0.2E1 - (t892 +
c     # t863 + t865 - t894 + t867 - t869 + t884) * dt + t886 * t590) * t1
c     #2 + (t1184 * dt / 0.2E1 + (t1190 + t1155 + t1157 - t1192 + t1159 -
c     # t1161 + t1182) * dt - t1184 * t590 - t1419 * dt / 0.2E1 - (t1425 
c     #+ t1396 + t1398 - t1427 + t1400 - t1402 + t1417) * dt + t1419 * t5
c     #90) * t49))

        return
      end
