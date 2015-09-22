      subroutine duStepWaveGen2d2cc( 
     *   nd1a,nd1b,nd2a,nd2b,
     *   n1a,n1b,n2a,n2b,
     *   u,ut,unew,utnew,rx,
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
      real rx   (nd1a:nd1b,nd2a:nd2b,0:1,0:1)
      real dx,dy,dt,cc
c
c.. generated code to follow
c
        real t1
        real t10
        real t102
        real t109
        real t110
        real t112
        real t113
        real t115
        real t119
        real t122
        real t123
        real t124
        real t126
        real t127
        real t13
        real t130
        real t138
        real t14
        real t141
        real t146
        real t15
        real t150
        real t151
        real t153
        real t154
        real t157
        real t159
        real t16
        real t165
        real t17
        real t173
        real t174
        real t176
        real t177
        real t178
        real t18
        real t180
        real t185
        real t186
        real t19
        real t2
        real t200
        real t203
        real t205
        real t207
        real t208
        real t21
        real t210
        real t212
        real t213
        real t214
        integer t215
        real t216
        real t219
        real t22
        real t224
        real t225
        real t226
        real t228
        real t237
        real t24
        integer t240
        real t241
        real t242
        real t244
        real t245
        real t248
        real t25
        real t250
        real t251
        real t252
        real t253
        real t254
        real t256
        real t258
        real t26
        real t260
        real t262
        real t27
        real t270
        real t271
        real t272
        real t274
        real t275
        real t277
        real t278
        real t28
        real t284
        real t285
        real t287
        real t288
        real t29
        real t290
        real t291
        real t294
        real t297
        real t298
        real t299
        real t301
        real t302
        real t305
        real t309
        real t311
        real t315
        real t319
        real t32
        real t322
        real t323
        real t324
        real t326
        real t327
        real t33
        real t330
        real t334
        real t336
        real t340
        real t343
        real t344
        real t345
        real t346
        real t347
        real t348
        real t349
        real t35
        real t350
        real t351
        real t354
        real t355
        real t356
        real t357
        real t358
        real t359
        real t36
        real t362
        real t363
        real t366
        real t369
        real t37
        real t370
        integer t371
        real t372
        real t373
        real t375
        real t376
        real t379
        real t38
        real t381
        real t382
        real t383
        real t384
        real t387
        real t389
        real t391
        real t393
        real t4
        real t400
        real t401
        real t402
        real t405
        real t408
        real t412
        real t415
        real t418
        real t420
        real t421
        real t423
        real t429
        real t441
        real t443
        real t445
        real t45
        real t451
        real t454
        real t464
        real t465
        real t467
        real t468
        real t47
        real t471
        real t481
        real t485
        real t489
        real t49
        real t490
        real t492
        real t493
        real t495
        real t496
        integer t5
        integer t50
        real t51
        real t510
        real t511
        real t514
        real t515
        real t517
        real t52
        real t522
        real t523
        integer t537
        real t538
        real t54
        real t541
        real t547
        real t548
        real t55
        real t550
        real t567
        real t57
        real t572
        real t576
        real t579
        real t58
        real t587
        real t59
        real t596
        real t6
        real t60
        real t604
        real t605
        real t61
        real t611
        real t618
        real t620
        real t622
        real t623
        real t626
        real t627
        real t629
        real t634
        real t635
        integer t644
        real t645
        real t652
        real t654
        real t658
        real t66
        real t662
        real t672
        real t673
        real t675
        real t676
        real t679
        real t68
        real t695
        real t696
        real t697
        real t7
        real t70
        real t710
        real t713
        real t715
        real t717
        real t719
        real t721
        real t722
        real t723
        integer t724
        real t726
        real t729
        real t734
        real t735
        real t737
        real t746
        real t750
        real t752
        real t754
        real t756
        real t758
        integer t76
        real t766
        real t769
        real t77
        real t770
        integer t771
        real t772
        real t773
        real t775
        real t776
        real t779
        real t781
        real t782
        real t783
        real t784
        real t787
        real t789
        real t79
        real t791
        real t793
        real t800
        real t801
        real t806
        integer t81
        real t819
        real t82
        real t826
        real t827
        real t830
        real t831
        real t833
        real t838
        real t839
        real t848
        real t855
        real t857
        real t861
        real t865
        real t9
        integer t902
        real t904
        real t907
        real t912
        real t913
        real t915
        real t92
        real t93
        real t932
        real t95
        real t96
        real t98
        t1 = u(i,j,n)
        t2 = ut(i,j,n)
        t4 = cc ** 2
        t5 = i + 1
        t6 = rx(t5,j,0,0)
        t7 = rx(t5,j,1,1)
        t9 = rx(t5,j,0,1)
        t10 = rx(t5,j,1,0)
        t13 = 0.1E1 / (-t10 * t9 + t6 * t7)
        t14 = t6 ** 2
        t15 = t9 ** 2
        t16 = t14 + t15
        t17 = t13 * t16
        t18 = rx(i,j,0,0)
        t19 = rx(i,j,1,1)
        t21 = rx(i,j,0,1)
        t22 = rx(i,j,1,0)
        t24 = t18 * t19 - t21 * t22
        t25 = 0.1E1 / t24
        t26 = t18 ** 2
        t27 = t21 ** 2
        t28 = t26 + t27
        t29 = t25 * t28
        t32 = t4 * (t17 / 0.2E1 + t29 / 0.2E1)
        t33 = u(t5,j,n)
        t35 = 0.1E1 / dx
        t36 = (t33 - t1) * t35
        t37 = t32 * t36
        t38 = ut(t5,j,n)
        t45 = sqrt(t16)
        t47 = cc * t13 * t45 * t38
        t49 = dt * cc
        t50 = i + 2
        t51 = rx(t50,j,0,0)
        t52 = rx(t50,j,1,1)
        t54 = rx(t50,j,0,1)
        t55 = rx(t50,j,1,0)
        t58 = 0.1E1 / (t51 * t52 - t54 * t55)
        t59 = t51 ** 2
        t60 = t54 ** 2
        t61 = t59 + t60
        t66 = u(t50,j,n)
        t68 = (t66 - t33) * t35
        t76 = j + 1
        t77 = u(t50,t76,n)
        t79 = 0.1E1 / dy
        t81 = j - 1
        t82 = u(t50,t81,n)
        t92 = t10 * t6 + t7 * t9
        t93 = u(t5,t76,n)
        t95 = (t93 - t33) * t79
        t96 = u(t5,t81,n)
        t98 = (t33 - t96) * t79
        t57 = t4 * t13 * t92
        t102 = t57 * (t95 / 0.2E1 + t98 / 0.2E1)
        t109 = t18 * t22 + t19 * t21
        t110 = u(i,t76,n)
        t112 = (t110 - t1) * t79
        t113 = u(i,t81,n)
        t115 = (t1 - t113) * t79
        t70 = t4 * t25 * t109
        t119 = t70 * (t112 / 0.2E1 + t115 / 0.2E1)
        t122 = (t102 - t119) * t35 / 0.2E1
        t123 = rx(t5,t76,0,0)
        t124 = rx(t5,t76,1,1)
        t126 = rx(t5,t76,0,1)
        t127 = rx(t5,t76,1,0)
        t130 = 0.1E1 / (t123 * t124 - t126 * t127)
        t138 = (t93 - t110) * t35
        t146 = t57 * (t68 / 0.2E1 + t36 / 0.2E1)
        t150 = rx(t5,t81,0,0)
        t151 = rx(t5,t81,1,1)
        t153 = rx(t5,t81,0,1)
        t154 = rx(t5,t81,1,0)
        t157 = 0.1E1 / (t150 * t151 - t153 * t154)
        t165 = (t96 - t113) * t35
        t173 = t127 ** 2
        t174 = t124 ** 2
        t177 = t10 ** 2
        t178 = t7 ** 2
        t180 = t13 * (t177 + t178)
        t185 = t154 ** 2
        t186 = t151 ** 2
        t200 = sqrt(t61)
        t203 = cc * t58 * t200 * ut(t50,j,n)
        t205 = (-t47 + t203) * t35
        t207 = cc * t25
        t208 = sqrt(t28)
        t210 = t207 * t208 * t2
        t212 = (-t210 + t47) * t35
        t213 = t212 / 0.2E1
        t214 = dx ** 2
        t215 = i + 3
        t216 = rx(t215,j,0,0)
        t219 = rx(t215,j,0,1)
        t225 = t216 ** 2
        t226 = t219 ** 2
        t228 = sqrt(t225 + t226)
        t237 = (t205 - t212) * t35
        t240 = i - 1
        t241 = rx(t240,j,0,0)
        t242 = rx(t240,j,1,1)
        t244 = rx(t240,j,0,1)
        t245 = rx(t240,j,1,0)
        t248 = 0.1E1 / (t241 * t242 - t244 * t245)
        t250 = t241 ** 2
        t251 = t244 ** 2
        t252 = t250 + t251
        t253 = sqrt(t252)
        t254 = ut(t240,j,n)
        t256 = cc * t248 * t253 * t254
        t258 = (t210 - t256) * t35
        t260 = (t212 - t258) * t35
        t262 = (t237 - t260) * t35
        t270 = t210 / 0.2E1
        t271 = t248 * t252
        t274 = t4 * (t29 / 0.2E1 + t271 / 0.2E1)
        t275 = u(t240,j,n)
        t277 = (t1 - t275) * t35
        t278 = t274 * t277
        t284 = t241 * t245 + t242 * t244
        t285 = u(t240,t76,n)
        t287 = (t285 - t275) * t79
        t288 = u(t240,t81,n)
        t290 = (t275 - t288) * t79
        t141 = t4 * t248 * t284
        t294 = t141 * (t287 / 0.2E1 + t290 / 0.2E1)
        t297 = (t119 - t294) * t35 / 0.2E1
        t298 = rx(i,t76,0,0)
        t299 = rx(i,t76,1,1)
        t301 = rx(i,t76,0,1)
        t302 = rx(i,t76,1,0)
        t305 = 0.1E1 / (t298 * t299 - t301 * t302)
        t309 = t298 * t302 + t299 * t301
        t311 = (t110 - t285) * t35
        t159 = t4 * t305 * t309
        t315 = t159 * (t138 / 0.2E1 + t311 / 0.2E1)
        t319 = t70 * (t36 / 0.2E1 + t277 / 0.2E1)
        t322 = (t315 - t319) * t79 / 0.2E1
        t323 = rx(i,t81,0,0)
        t324 = rx(i,t81,1,1)
        t326 = rx(i,t81,0,1)
        t327 = rx(i,t81,1,0)
        t330 = 0.1E1 / (t323 * t324 - t326 * t327)
        t334 = t323 * t327 + t324 * t326
        t336 = (t113 - t288) * t35
        t176 = t4 * t330 * t334
        t340 = t176 * (t165 / 0.2E1 + t336 / 0.2E1)
        t343 = (t319 - t340) * t79 / 0.2E1
        t344 = t302 ** 2
        t345 = t299 ** 2
        t346 = t344 + t345
        t347 = t305 * t346
        t348 = t22 ** 2
        t349 = t19 ** 2
        t350 = t348 + t349
        t351 = t25 * t350
        t354 = t4 * (t347 / 0.2E1 + t351 / 0.2E1)
        t355 = t354 * t112
        t356 = t327 ** 2
        t357 = t324 ** 2
        t358 = t356 + t357
        t359 = t330 * t358
        t362 = t4 * (t351 / 0.2E1 + t359 / 0.2E1)
        t363 = t362 * t115
        t366 = (t37 - t278) * t35 + t122 + t297 + t322 + t343 + (t355 - 
     #t363) * t79
        t369 = t49 * t208 * t366 / 0.4E1
        t370 = t258 / 0.2E1
        t371 = i - 2
        t372 = rx(t371,j,0,0)
        t373 = rx(t371,j,1,1)
        t375 = rx(t371,j,0,1)
        t376 = rx(t371,j,1,0)
        t379 = 0.1E1 / (t372 * t373 - t375 * t376)
        t381 = t372 ** 2
        t382 = t375 ** 2
        t383 = t381 + t382
        t384 = sqrt(t383)
        t387 = cc * t379 * t384 * ut(t371,j,n)
        t389 = (-t387 + t256) * t35
        t391 = (t258 - t389) * t35
        t393 = (t260 - t391) * t35
        t400 = dx * (t213 + t370 - t214 * (t262 / 0.2E1 + t393 / 0.2E1) 
     #/ 0.6E1) / 0.4E1
        t224 = (t38 - t2) * t35
        t272 = t4 * t130 * (t123 * t127 + t124 * t126)
        t291 = t4 * t157 * (t150 * t154 + t151 * t153)
        t401 = t37 + t32 * dt * t224 / 0.2E1 + t47 / 0.2E1 + t49 * t45 *
     # ((t4 * (t58 * t61 / 0.2E1 + t17 / 0.2E1) * t68 - t37) * t35 + (t4
     # * t58 * (t51 * t55 + t52 * t54) * ((t77 - t66) * t79 / 0.2E1 + (t
     #66 - t82) * t79 / 0.2E1) - t102) * t35 / 0.2E1 + t122 + (t272 * ((
     #t77 - t93) * t35 / 0.2E1 + t138 / 0.2E1) - t146) * t79 / 0.2E1 + (
     #t146 - t291 * ((t82 - t96) * t35 / 0.2E1 + t165 / 0.2E1)) * t79 / 
     #0.2E1 + (t4 * (t130 * (t173 + t174) / 0.2E1 + t180 / 0.2E1) * t95 
     #- t4 * (t180 / 0.2E1 + t157 * (t185 + t186) / 0.2E1) * t98) * t79)
     # / 0.4E1 - dx * (t205 / 0.2E1 + t213 - t214 * ((((-t203 + cc / (t2
     #16 * rx(t215,j,1,1) - t219 * rx(t215,j,1,0)) * t228 * ut(t215,j,n)
     #) * t35 - t205) * t35 - t237) * t35 / 0.2E1 + t262 / 0.2E1) / 0.6E
     #1) / 0.4E1 - t270 - t369 - t400
        t402 = dt ** 2
        t405 = t25 * t109
        t408 = t4 * (t13 * t92 / 0.2E1 + t405 / 0.2E1)
        t412 = ut(t5,t76,n)
        t415 = ut(t5,t81,n)
        t418 = ut(i,t76,n)
        t420 = (t418 - t2) * t79
        t421 = ut(i,t81,n)
        t423 = (t2 - t421) * t79
        t429 = t408 * (t95 / 0.4E1 + t98 / 0.4E1 + t112 / 0.4E1 + t115 /
     # 0.4E1) + t408 * dt * ((t412 - t38) * t79 / 0.4E1 + (t38 - t415) *
     # t79 / 0.4E1 + t420 / 0.4E1 + t423 / 0.4E1) / 0.2E1
        t441 = u(t371,j,n)
        t443 = (t275 - t441) * t35
        t451 = u(t371,t76,n)
        t454 = u(t371,t81,n)
        t464 = rx(t240,t76,0,0)
        t465 = rx(t240,t76,1,1)
        t467 = rx(t240,t76,0,1)
        t468 = rx(t240,t76,1,0)
        t471 = 0.1E1 / (t464 * t465 - t467 * t468)
        t485 = t141 * (t277 / 0.2E1 + t443 / 0.2E1)
        t489 = rx(t240,t81,0,0)
        t490 = rx(t240,t81,1,1)
        t492 = rx(t240,t81,0,1)
        t493 = rx(t240,t81,1,0)
        t496 = 0.1E1 / (t489 * t490 - t492 * t493)
        t510 = t468 ** 2
        t511 = t465 ** 2
        t514 = t245 ** 2
        t515 = t242 ** 2
        t517 = t248 * (t514 + t515)
        t522 = t493 ** 2
        t523 = t490 ** 2
        t537 = i - 3
        t538 = rx(t537,j,0,0)
        t541 = rx(t537,j,0,1)
        t547 = t538 ** 2
        t548 = t541 ** 2
        t550 = sqrt(t547 + t548)
        t445 = (t2 - t254) * t35
        t481 = t4 * t471 * (t464 * t468 + t465 * t467)
        t495 = t4 * t496 * (t489 * t493 + t490 * t492)
        t567 = t278 + t274 * dt * t445 / 0.2E1 + t270 + t369 - t400 - t2
     #56 / 0.2E1 - t49 * t253 * ((t278 - t4 * (t379 * t383 / 0.2E1 + t27
     #1 / 0.2E1) * t443) * t35 + t297 + (t294 - t4 * t379 * (t372 * t376
     # + t373 * t375) * ((t451 - t441) * t79 / 0.2E1 + (t441 - t454) * t
     #79 / 0.2E1)) * t35 / 0.2E1 + (t481 * (t311 / 0.2E1 + (t285 - t451)
     # * t35 / 0.2E1) - t485) * t79 / 0.2E1 + (t485 - t495 * (t336 / 0.2
     #E1 + (t288 - t454) * t35 / 0.2E1)) * t79 / 0.2E1 + (t4 * (t471 * (
     #t510 + t511) / 0.2E1 + t517 / 0.2E1) * t287 - t4 * (t517 / 0.2E1 +
     # t496 * (t522 + t523) / 0.2E1) * t290) * t79) / 0.4E1 - dx * (t370
     # + t389 / 0.2E1 - t214 * (t393 / 0.2E1 + (t391 - (t389 - (-cc / (t
     #538 * rx(t537,j,1,1) - t541 * rx(t537,j,1,0)) * t550 * ut(t537,j,n
     #) + t387) * t35) * t35) * t35 / 0.2E1) / 0.6E1) / 0.4E1
        t572 = t4 * (t248 * t284 / 0.2E1 + t405 / 0.2E1)
        t576 = ut(t240,t76,n)
        t579 = ut(t240,t81,n)
        t587 = t572 * (t112 / 0.4E1 + t115 / 0.4E1 + t287 / 0.4E1 + t290
     # / 0.4E1) + t572 * dt * (t420 / 0.4E1 + t423 / 0.4E1 + (t576 - t25
     #4) * t79 / 0.4E1 + (t254 - t579) * t79 / 0.4E1) / 0.2E1
        t596 = t4 * (t305 * t309 / 0.2E1 + t405 / 0.2E1)
        t604 = t224
        t605 = t445
        t611 = t596 * (t138 / 0.4E1 + t311 / 0.4E1 + t36 / 0.4E1 + t277 
     #/ 0.4E1) + t596 * dt * ((t412 - t418) * t35 / 0.4E1 + (t418 - t576
     #) * t35 / 0.4E1 + t604 / 0.4E1 + t605 / 0.4E1) / 0.2E1
        t618 = sqrt(t346)
        t620 = cc * t305 * t618 * t418
        t622 = t123 ** 2
        t623 = t126 ** 2
        t626 = t298 ** 2
        t627 = t301 ** 2
        t629 = t305 * (t626 + t627)
        t634 = t464 ** 2
        t635 = t467 ** 2
        t644 = j + 2
        t645 = u(t5,t644,n)
        t652 = u(i,t644,n)
        t654 = (t652 - t110) * t79
        t658 = t159 * (t654 / 0.2E1 + t112 / 0.2E1)
        t662 = u(t240,t644,n)
        t672 = rx(i,t644,0,0)
        t673 = rx(i,t644,1,1)
        t675 = rx(i,t644,0,1)
        t676 = rx(i,t644,1,0)
        t679 = 0.1E1 / (t672 * t673 - t675 * t676)
        t695 = t676 ** 2
        t696 = t673 ** 2
        t697 = t695 + t696
        t710 = sqrt(t697)
        t713 = cc * t679 * t710 * ut(i,t644,n)
        t715 = (-t620 + t713) * t79
        t717 = sqrt(t350)
        t719 = t207 * t717 * t2
        t721 = (-t719 + t620) * t79
        t722 = t721 / 0.2E1
        t723 = dy ** 2
        t724 = j + 3
        t726 = rx(i,t724,1,1)
        t729 = rx(i,t724,1,0)
        t734 = t729 ** 2
        t735 = t726 ** 2
        t737 = sqrt(t734 + t735)
        t746 = (t715 - t721) * t79
        t750 = sqrt(t358)
        t752 = cc * t330 * t750 * t421
        t754 = (t719 - t752) * t79
        t756 = (t721 - t754) * t79
        t758 = (t746 - t756) * t79
        t766 = t719 / 0.2E1
        t769 = t49 * t717 * t366 / 0.4E1
        t770 = t754 / 0.2E1
        t771 = j - 2
        t772 = rx(i,t771,0,0)
        t773 = rx(i,t771,1,1)
        t775 = rx(i,t771,0,1)
        t776 = rx(i,t771,1,0)
        t779 = 0.1E1 / (t772 * t773 - t775 * t776)
        t781 = t776 ** 2
        t782 = t773 ** 2
        t783 = t781 + t782
        t784 = sqrt(t783)
        t787 = cc * t779 * t784 * ut(i,t771,n)
        t789 = (-t787 + t752) * t79
        t791 = (t754 - t789) * t79
        t793 = (t756 - t791) * t79
        t800 = dy * (t722 + t770 - t723 * (t758 / 0.2E1 + t793 / 0.2E1) 
     #/ 0.6E1) / 0.4E1
        t801 = t355 + t354 * dt * t420 / 0.2E1 + t620 / 0.2E1 + t49 * t6
     #18 * ((t4 * (t130 * (t622 + t623) / 0.2E1 + t629 / 0.2E1) * t138 -
     # t4 * (t629 / 0.2E1 + t471 * (t634 + t635) / 0.2E1) * t311) * t35 
     #+ (t272 * ((t645 - t93) * t79 / 0.2E1 + t95 / 0.2E1) - t658) * t35
     # / 0.2E1 + (t658 - t481 * ((t662 - t285) * t79 / 0.2E1 + t287 / 0.
     #2E1)) * t35 / 0.2E1 + (t4 * t679 * (t672 * t676 + t673 * t675) * (
     #(t645 - t652) * t35 / 0.2E1 + (t652 - t662) * t35 / 0.2E1) - t315)
     # * t79 / 0.2E1 + t322 + (t4 * (t679 * t697 / 0.2E1 + t347 / 0.2E1)
     # * t654 - t355) * t79) / 0.4E1 - dy * (t715 / 0.2E1 + t722 - t723 
     #* ((((-t713 + cc / (t726 * rx(i,t724,0,0) - t729 * rx(i,t724,0,1))
     # * t737 * ut(i,t724,n)) * t79 - t715) * t79 - t746) * t79 / 0.2E1 
     #+ t758 / 0.2E1) / 0.6E1) / 0.4E1 - t766 - t769 - t800
        t806 = t4 * (t330 * t334 / 0.2E1 + t405 / 0.2E1)
        t819 = t806 * (t36 / 0.4E1 + t277 / 0.4E1 + t165 / 0.4E1 + t336 
     #/ 0.4E1) + t806 * dt * (t604 / 0.4E1 + t605 / 0.4E1 + (t415 - t421
     #) * t35 / 0.4E1 + (t421 - t579) * t35 / 0.4E1) / 0.2E1
        t826 = t150 ** 2
        t827 = t153 ** 2
        t830 = t323 ** 2
        t831 = t326 ** 2
        t833 = t330 * (t830 + t831)
        t838 = t489 ** 2
        t839 = t492 ** 2
        t848 = u(t5,t771,n)
        t855 = u(i,t771,n)
        t857 = (t113 - t855) * t79
        t861 = t176 * (t115 / 0.2E1 + t857 / 0.2E1)
        t865 = u(t240,t771,n)
        t902 = j - 3
        t904 = rx(i,t902,1,1)
        t907 = rx(i,t902,1,0)
        t912 = t907 ** 2
        t913 = t904 ** 2
        t915 = sqrt(t912 + t913)
        t932 = t363 + t362 * dt * t423 / 0.2E1 + t766 + t769 - t800 - t7
     #52 / 0.2E1 - t49 * t750 * ((t4 * (t157 * (t826 + t827) / 0.2E1 + t
     #833 / 0.2E1) * t165 - t4 * (t833 / 0.2E1 + t496 * (t838 + t839) / 
     #0.2E1) * t336) * t35 + (t291 * (t98 / 0.2E1 + (t96 - t848) * t79 /
     # 0.2E1) - t861) * t35 / 0.2E1 + (t861 - t495 * (t290 / 0.2E1 + (t2
     #88 - t865) * t79 / 0.2E1)) * t35 / 0.2E1 + t343 + (t340 - t4 * t77
     #9 * (t772 * t776 + t773 * t775) * ((t848 - t855) * t35 / 0.2E1 + (
     #t855 - t865) * t35 / 0.2E1)) * t79 / 0.2E1 + (t363 - t4 * (t779 * 
     #t783 / 0.2E1 + t359 / 0.2E1) * t857) * t79) / 0.4E1 - dy * (t770 +
     # t789 / 0.2E1 - t723 * (t793 / 0.2E1 + (t791 - (t789 - (-cc / (t90
     #4 * rx(i,t902,0,0) - t907 * rx(i,t902,0,1)) * t915 * ut(i,t902,n) 
     #+ t787) * t79) * t79) * t79 / 0.2E1) / 0.6E1) / 0.4E1

        unew(i,j) = t1 + dt * t2 + (t401 * t402 / 0.2E1 + t429 * t4
     #02 / 0.2E1 - t567 * t402 / 0.2E1 - t587 * t402 / 0.2E1) * t24 * t3
     #5 + (t611 * t402 / 0.2E1 + t801 * t402 / 0.2E1 - t819 * t402 / 0.2
     #E1 - t932 * t402 / 0.2E1) * t24 * t79

        utnew(i,j) = t2 + (dt * t401 + dt * 
     #t429 - dt * t567 - dt * t587) * t24 * t35 + (dt * t611 + dt * t801
     # - dt * t819 - dt * t932) * t24 * t79

        return
      end
