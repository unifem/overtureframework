      subroutine duStepWaveGen2d2cc_tz( 
     *   nd1a,nd1b,nd2a,nd2b,
     *   n1a,n1b,n2a,n2b,
     *   ndf4a,ndf4b,nComp,
     *   u,ut,unew,utnew,
     *   rx,src,
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
      real rx   (nd1a:nd1b,nd2a:nd2b,0:1,0:1)
      real src  (nd1a:nd1b,nd2a:nd2b,ndf4a:ndf4b,1:*)
      real dx,dy,dt,cc
c
c.. generated code to follow
c
        real t1
        real t10
        real t103
        real t110
        real t111
        real t113
        real t114
        real t116
        real t119
        real t12
        real t120
        real t123
        real t124
        real t125
        real t127
        real t128
        real t13
        real t131
        real t138
        real t139
        real t14
        real t147
        real t15
        real t151
        real t152
        real t154
        real t155
        real t156
        real t158
        real t16
        real t166
        real t17
        real t174
        real t175
        real t178
        real t179
        real t18
        real t180
        real t181
        real t186
        real t187
        real t19
        real t193
        real t2
        real t204
        real t21
        real t210
        real t211
        real t213
        real t215
        real t22
        real t220
        integer t222
        real t223
        real t224
        real t226
        real t227
        real t229
        real t230
        real t231
        real t232
        real t233
        real t234
        real t237
        real t238
        real t239
        real t24
        real t240
        real t241
        real t247
        real t248
        real t25
        real t250
        real t251
        real t253
        real t256
        real t257
        real t26
        real t260
        real t261
        real t262
        real t264
        real t265
        real t267
        real t268
        real t27
        real t272
        real t274
        real t278
        real t28
        real t282
        real t285
        real t286
        real t287
        real t289
        real t29
        real t290
        real t292
        real t293
        real t297
        real t299
        real t303
        real t306
        real t307
        real t308
        real t309
        real t310
        real t311
        real t312
        real t313
        real t314
        real t317
        real t318
        real t319
        real t32
        real t320
        real t321
        real t322
        real t325
        real t326
        real t33
        real t332
        real t335
        real t337
        real t338
        real t340
        real t342
        real t346
        real t347
        real t348
        real t35
        real t351
        real t354
        real t358
        real t36
        real t361
        real t364
        real t366
        real t367
        real t369
        real t37
        real t375
        real t38
        integer t384
        real t385
        real t386
        real t388
        real t389
        real t392
        real t393
        real t394
        real t395
        real t4
        real t400
        real t401
        real t402
        real t410
        real t413
        real t423
        real t424
        real t426
        real t427
        real t430
        real t438
        real t444
        real t448
        real t449
        real t45
        real t450
        real t451
        real t452
        real t455
        real t469
        real t47
        real t470
        real t473
        real t474
        real t476
        real t481
        real t482
        real t49
        real t499
        integer t5
        real t509
        integer t51
        real t514
        real t518
        real t52
        real t521
        real t529
        real t53
        real t538
        real t546
        real t547
        real t55
        real t553
        real t56
        real t560
        real t562
        real t565
        real t566
        real t569
        real t57
        real t570
        real t572
        real t577
        real t578
        integer t587
        real t588
        real t59
        real t595
        real t597
        real t6
        real t60
        real t601
        real t605
        real t61
        real t615
        real t616
        real t618
        real t619
        real t62
        real t622
        real t638
        real t639
        real t640
        real t656
        real t662
        real t664
        real t666
        real t67
        real t671
        real t675
        real t677
        real t679
        real t681
        real t685
        real t686
        real t69
        real t691
        real t7
        real t70
        real t704
        real t712
        real t713
        real t716
        real t717
        real t719
        real t724
        real t725
        integer t734
        real t735
        real t742
        real t744
        real t748
        real t752
        real t762
        real t763
        real t765
        real t766
        real t769
        integer t77
        real t78
        real t785
        real t786
        real t787
        real t80
        real t803
        real t813
        integer t82
        real t820
        real t83
        real t9
        real t93
        real t94
        real t96
        real t97
        real t99
        t1 = u(i,j,n)
        t2 = ut(i,j,n)
        t4 = cc ** 2
        t5 = i + 1
        t6 = rx(t5,j,0,0)
        t7 = rx(t5,j,1,1)
        t9 = rx(t5,j,0,1)
        t10 = rx(t5,j,1,0)
        t12 = -t10 * t9 + t6 * t7
        t13 = 0.1E1 / t12
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
        t51 = i + 2
        t52 = rx(t51,j,0,0)
        t53 = rx(t51,j,1,1)
        t55 = rx(t51,j,0,1)
        t56 = rx(t51,j,1,0)
        t59 = 0.1E1 / (t52 * t53 - t55 * t56)
        t60 = t52 ** 2
        t61 = t55 ** 2
        t62 = t60 + t61
        t67 = u(t51,j,n)
        t69 = (t67 - t33) * t35
        t77 = j + 1
        t78 = u(t51,t77,n)
        t80 = 0.1E1 / dy
        t82 = j - 1
        t83 = u(t51,t82,n)
        t93 = t10 * t6 + t7 * t9
        t94 = u(t5,t77,n)
        t96 = (t94 - t33) * t80
        t97 = u(t5,t82,n)
        t99 = (t33 - t97) * t80
        t57 = t4 * t13 * t93
        t103 = t57 * (t96 / 0.2E1 + t99 / 0.2E1)
        t110 = t18 * t22 + t19 * t21
        t111 = u(i,t77,n)
        t113 = (t111 - t1) * t80
        t114 = u(i,t82,n)
        t116 = (t1 - t114) * t80
        t70 = t4 * t25 * t110
        t120 = t70 * (t113 / 0.2E1 + t116 / 0.2E1)
        t123 = (t103 - t120) * t35 / 0.2E1
        t124 = rx(t5,t77,0,0)
        t125 = rx(t5,t77,1,1)
        t127 = rx(t5,t77,0,1)
        t128 = rx(t5,t77,1,0)
        t131 = 0.1E1 / (t124 * t125 - t127 * t128)
        t139 = (t94 - t111) * t35
        t147 = t57 * (t69 / 0.2E1 + t36 / 0.2E1)
        t151 = rx(t5,t82,0,0)
        t152 = rx(t5,t82,1,1)
        t154 = rx(t5,t82,0,1)
        t155 = rx(t5,t82,1,0)
        t158 = 0.1E1 / (t151 * t152 - t154 * t155)
        t166 = (t97 - t114) * t35
        t174 = t128 ** 2
        t175 = t125 ** 2
        t178 = t10 ** 2
        t179 = t7 ** 2
        t181 = t13 * (t178 + t179)
        t186 = t155 ** 2
        t187 = t152 ** 2
        t204 = sqrt(t62)
        t210 = cc * t25
        t211 = sqrt(t28)
        t213 = t210 * t211 * t2
        t215 = (-t213 + t47) * t35
        t220 = t213 / 0.2E1
        t222 = i - 1
        t223 = rx(t222,j,0,0)
        t224 = rx(t222,j,1,1)
        t226 = rx(t222,j,0,1)
        t227 = rx(t222,j,1,0)
        t229 = t223 * t224 - t226 * t227
        t230 = 0.1E1 / t229
        t231 = t223 ** 2
        t232 = t226 ** 2
        t233 = t231 + t232
        t234 = t230 * t233
        t237 = t4 * (t29 / 0.2E1 + t234 / 0.2E1)
        t238 = u(t222,j,n)
        t240 = (t1 - t238) * t35
        t241 = t237 * t240
        t247 = t223 * t227 + t224 * t226
        t248 = u(t222,t77,n)
        t250 = (t248 - t238) * t80
        t251 = u(t222,t82,n)
        t253 = (t238 - t251) * t80
        t119 = t4 * t230 * t247
        t257 = t119 * (t250 / 0.2E1 + t253 / 0.2E1)
        t260 = (t120 - t257) * t35 / 0.2E1
        t261 = rx(i,t77,0,0)
        t262 = rx(i,t77,1,1)
        t264 = rx(i,t77,0,1)
        t265 = rx(i,t77,1,0)
        t267 = t261 * t262 - t264 * t265
        t268 = 0.1E1 / t267
        t272 = t261 * t265 + t262 * t264
        t274 = (t111 - t248) * t35
        t138 = t4 * t268 * t272
        t278 = t138 * (t139 / 0.2E1 + t274 / 0.2E1)
        t282 = t70 * (t36 / 0.2E1 + t240 / 0.2E1)
        t285 = (t278 - t282) * t80 / 0.2E1
        t286 = rx(i,t82,0,0)
        t287 = rx(i,t82,1,1)
        t289 = rx(i,t82,0,1)
        t290 = rx(i,t82,1,0)
        t292 = t286 * t287 - t289 * t290
        t293 = 0.1E1 / t292
        t297 = t286 * t290 + t287 * t289
        t299 = (t114 - t251) * t35
        t156 = t4 * t293 * t297
        t303 = t156 * (t166 / 0.2E1 + t299 / 0.2E1)
        t306 = (t282 - t303) * t80 / 0.2E1
        t307 = t265 ** 2
        t308 = t262 ** 2
        t309 = t307 + t308
        t310 = t268 * t309
        t311 = t22 ** 2
        t312 = t19 ** 2
        t313 = t311 + t312
        t314 = t25 * t313
        t317 = t4 * (t310 / 0.2E1 + t314 / 0.2E1)
        t318 = t317 * t113
        t319 = t290 ** 2
        t320 = t287 ** 2
        t321 = t319 + t320
        t322 = t293 * t321
        t325 = t4 * (t314 / 0.2E1 + t322 / 0.2E1)
        t326 = t325 * t116
        t332 = ((t37 - t241) * t35 + t123 + t260 + t285 + t306 + (t318 -
     # t326) * t80) * t24 + src(i,j,nComp,n)
        t180 = t49 * t25
        t335 = t180 * t211 * t332 / 0.4E1
        t337 = sqrt(t233)
        t338 = ut(t222,j,n)
        t340 = cc * t230 * t337 * t338
        t342 = (t213 - t340) * t35
        t346 = dx * (t215 / 0.2E1 + t342 / 0.2E1) / 0.4E1
        t193 = (t38 - t2) * t35
        t239 = t4 * t131 * (t124 * t128 + t125 * t127)
        t256 = t4 * t158 * (t151 * t155 + t152 * t154)
        t347 = t37 + t32 * dt * t193 / 0.2E1 + t47 / 0.2E1 + t49 * t13 *
     # t45 * (((t4 * (t59 * t62 / 0.2E1 + t17 / 0.2E1) * t69 - t37) * t3
     #5 + (t4 * t59 * (t52 * t56 + t53 * t55) * ((t78 - t67) * t80 / 0.2
     #E1 + (t67 - t83) * t80 / 0.2E1) - t103) * t35 / 0.2E1 + t123 + (t2
     #39 * ((t78 - t94) * t35 / 0.2E1 + t139 / 0.2E1) - t147) * t80 / 0.
     #2E1 + (t147 - t256 * ((t83 - t97) * t35 / 0.2E1 + t166 / 0.2E1)) *
     # t80 / 0.2E1 + (t4 * (t131 * (t174 + t175) / 0.2E1 + t181 / 0.2E1)
     # * t96 - t4 * (t181 / 0.2E1 + t158 * (t186 + t187) / 0.2E1) * t99)
     # * t80) * t12 + src(t5,j,nComp,n)) / 0.4E1 - dx * ((cc * t204 * t5
     #9 * ut(t51,j,n) - t47) * t35 / 0.2E1 + t215 / 0.2E1) / 0.4E1 - t22
     #0 - t335 - t346
        t348 = dt ** 2
        t351 = t25 * t110
        t354 = t4 * (t13 * t93 / 0.2E1 + t351 / 0.2E1)
        t358 = ut(t5,t77,n)
        t361 = ut(t5,t82,n)
        t364 = ut(i,t77,n)
        t366 = (t364 - t2) * t80
        t367 = ut(i,t82,n)
        t369 = (t2 - t367) * t80
        t375 = t354 * (t96 / 0.4E1 + t99 / 0.4E1 + t113 / 0.4E1 + t116 /
     # 0.4E1) + t354 * dt * ((t358 - t38) * t80 / 0.4E1 + (t38 - t361) *
     # t80 / 0.4E1 + t366 / 0.4E1 + t369 / 0.4E1) / 0.2E1
        t384 = i - 2
        t385 = rx(t384,j,0,0)
        t386 = rx(t384,j,1,1)
        t388 = rx(t384,j,0,1)
        t389 = rx(t384,j,1,0)
        t392 = 0.1E1 / (t385 * t386 - t388 * t389)
        t393 = t385 ** 2
        t394 = t388 ** 2
        t395 = t393 + t394
        t400 = u(t384,j,n)
        t402 = (t238 - t400) * t35
        t410 = u(t384,t77,n)
        t413 = u(t384,t82,n)
        t423 = rx(t222,t77,0,0)
        t424 = rx(t222,t77,1,1)
        t426 = rx(t222,t77,0,1)
        t427 = rx(t222,t77,1,0)
        t430 = 0.1E1 / (t423 * t424 - t426 * t427)
        t444 = t119 * (t240 / 0.2E1 + t402 / 0.2E1)
        t448 = rx(t222,t82,0,0)
        t449 = rx(t222,t82,1,1)
        t451 = rx(t222,t82,0,1)
        t452 = rx(t222,t82,1,0)
        t455 = 0.1E1 / (t448 * t449 - t451 * t452)
        t469 = t427 ** 2
        t470 = t424 ** 2
        t473 = t227 ** 2
        t474 = t224 ** 2
        t476 = t230 * (t473 + t474)
        t481 = t452 ** 2
        t482 = t449 ** 2
        t499 = sqrt(t395)
        t401 = (t2 - t338) * t35
        t438 = t4 * t430 * (t423 * t427 + t424 * t426)
        t450 = t4 * t455 * (t448 * t452 + t449 * t451)
        t509 = t241 + t237 * dt * t401 / 0.2E1 + t220 + t335 - t346 - t3
     #40 / 0.2E1 - t49 * t230 * t337 * (((t241 - t4 * (t392 * t395 / 0.2
     #E1 + t234 / 0.2E1) * t402) * t35 + t260 + (t257 - t4 * t392 * (t38
     #5 * t389 + t386 * t388) * ((t410 - t400) * t80 / 0.2E1 + (t400 - t
     #413) * t80 / 0.2E1)) * t35 / 0.2E1 + (t438 * (t274 / 0.2E1 + (t248
     # - t410) * t35 / 0.2E1) - t444) * t80 / 0.2E1 + (t444 - t450 * (t2
     #99 / 0.2E1 + (t251 - t413) * t35 / 0.2E1)) * t80 / 0.2E1 + (t4 * (
     #t430 * (t469 + t470) / 0.2E1 + t476 / 0.2E1) * t250 - t4 * (t476 /
     # 0.2E1 + t455 * (t481 + t482) / 0.2E1) * t253) * t80) * t229 + src
     #(t222,j,nComp,n)) / 0.4E1 - dx * (t342 / 0.2E1 + (-cc * t392 * t49
     #9 * ut(t384,j,n) + t340) * t35 / 0.2E1) / 0.4E1
        t514 = t4 * (t230 * t247 / 0.2E1 + t351 / 0.2E1)
        t518 = ut(t222,t77,n)
        t521 = ut(t222,t82,n)
        t529 = t514 * (t113 / 0.4E1 + t116 / 0.4E1 + t250 / 0.4E1 + t253
     # / 0.4E1) + t514 * dt * (t366 / 0.4E1 + t369 / 0.4E1 + (t518 - t33
     #8) * t80 / 0.4E1 + (t338 - t521) * t80 / 0.4E1) / 0.2E1
        t538 = t4 * (t268 * t272 / 0.2E1 + t351 / 0.2E1)
        t546 = t193
        t547 = t401
        t553 = t538 * (t139 / 0.4E1 + t274 / 0.4E1 + t36 / 0.4E1 + t240 
     #/ 0.4E1) + t538 * dt * ((t358 - t364) * t35 / 0.4E1 + (t364 - t518
     #) * t35 / 0.4E1 + t546 / 0.4E1 + t547 / 0.4E1) / 0.2E1
        t560 = sqrt(t309)
        t562 = cc * t268 * t560 * t364
        t565 = t124 ** 2
        t566 = t127 ** 2
        t569 = t261 ** 2
        t570 = t264 ** 2
        t572 = t268 * (t569 + t570)
        t577 = t423 ** 2
        t578 = t426 ** 2
        t587 = j + 2
        t588 = u(t5,t587,n)
        t595 = u(i,t587,n)
        t597 = (t595 - t111) * t80
        t601 = t138 * (t597 / 0.2E1 + t113 / 0.2E1)
        t605 = u(t222,t587,n)
        t615 = rx(i,t587,0,0)
        t616 = rx(i,t587,1,1)
        t618 = rx(i,t587,0,1)
        t619 = rx(i,t587,1,0)
        t622 = 0.1E1 / (t615 * t616 - t618 * t619)
        t638 = t619 ** 2
        t639 = t616 ** 2
        t640 = t638 + t639
        t656 = sqrt(t640)
        t662 = sqrt(t313)
        t664 = t210 * t662 * t2
        t666 = (-t664 + t562) * t80
        t671 = t664 / 0.2E1
        t675 = t180 * t662 * t332 / 0.4E1
        t677 = sqrt(t321)
        t679 = cc * t293 * t677 * t367
        t681 = (t664 - t679) * t80
        t685 = dy * (t666 / 0.2E1 + t681 / 0.2E1) / 0.4E1
        t686 = t318 + t317 * dt * t366 / 0.2E1 + t562 / 0.2E1 + t49 * t2
     #68 * t560 * (((t4 * (t131 * (t565 + t566) / 0.2E1 + t572 / 0.2E1) 
     #* t139 - t4 * (t572 / 0.2E1 + t430 * (t577 + t578) / 0.2E1) * t274
     #) * t35 + (t239 * ((t588 - t94) * t80 / 0.2E1 + t96 / 0.2E1) - t60
     #1) * t35 / 0.2E1 + (t601 - t438 * ((t605 - t248) * t80 / 0.2E1 + t
     #250 / 0.2E1)) * t35 / 0.2E1 + (t4 * t622 * (t615 * t619 + t616 * t
     #618) * ((t588 - t595) * t35 / 0.2E1 + (t595 - t605) * t35 / 0.2E1)
     # - t278) * t80 / 0.2E1 + t285 + (t4 * (t622 * t640 / 0.2E1 + t310 
     #/ 0.2E1) * t597 - t318) * t80) * t267 + src(i,t77,nComp,n)) / 0.4E
     #1 - dy * ((cc * t622 * t656 * ut(i,t587,n) - t562) * t80 / 0.2E1 +
     # t666 / 0.2E1) / 0.4E1 - t671 - t675 - t685
        t691 = t4 * (t293 * t297 / 0.2E1 + t351 / 0.2E1)
        t704 = t691 * (t36 / 0.4E1 + t240 / 0.4E1 + t166 / 0.4E1 + t299 
     #/ 0.4E1) + t691 * dt * (t546 / 0.4E1 + t547 / 0.4E1 + (t361 - t367
     #) * t35 / 0.4E1 + (t367 - t521) * t35 / 0.4E1) / 0.2E1
        t712 = t151 ** 2
        t713 = t154 ** 2
        t716 = t286 ** 2
        t717 = t289 ** 2
        t719 = t293 * (t716 + t717)
        t724 = t448 ** 2
        t725 = t451 ** 2
        t734 = j - 2
        t735 = u(t5,t734,n)
        t742 = u(i,t734,n)
        t744 = (t114 - t742) * t80
        t748 = t156 * (t116 / 0.2E1 + t744 / 0.2E1)
        t752 = u(t222,t734,n)
        t762 = rx(i,t734,0,0)
        t763 = rx(i,t734,1,1)
        t765 = rx(i,t734,0,1)
        t766 = rx(i,t734,1,0)
        t769 = 0.1E1 / (t762 * t763 - t765 * t766)
        t785 = t766 ** 2
        t786 = t763 ** 2
        t787 = t785 + t786
        t803 = sqrt(t787)
        t813 = t326 + t325 * dt * t369 / 0.2E1 + t671 + t675 - t685 - t6
     #79 / 0.2E1 - t49 * t293 * t677 * (((t4 * (t158 * (t712 + t713) / 0
     #.2E1 + t719 / 0.2E1) * t166 - t4 * (t719 / 0.2E1 + t455 * (t724 + 
     #t725) / 0.2E1) * t299) * t35 + (t256 * (t99 / 0.2E1 + (t97 - t735)
     # * t80 / 0.2E1) - t748) * t35 / 0.2E1 + (t748 - t450 * (t253 / 0.2
     #E1 + (t251 - t752) * t80 / 0.2E1)) * t35 / 0.2E1 + t306 + (t303 - 
     #t4 * t769 * (t762 * t766 + t763 * t765) * ((t735 - t742) * t35 / 0
     #.2E1 + (t742 - t752) * t35 / 0.2E1)) * t80 / 0.2E1 + (t326 - t4 * 
     #(t769 * t787 / 0.2E1 + t322 / 0.2E1) * t744) * t80) * t292 + src(i
     #,t82,nComp,n)) / 0.4E1 - dy * (t681 / 0.2E1 + (-cc * t769 * t803 *
     # ut(i,t734,n) + t679) * t80 / 0.2E1) / 0.4E1
        t820 = src(i,j,nComp,n + 1)

        unew(i,j) = t1 + dt * t2 + (t347 * t348 / 0.2E1 + t375 * t3
     #48 / 0.2E1 - t509 * t348 / 0.2E1 - t529 * t348 / 0.2E1) * t24 * t3
     #5 + (t553 * t348 / 0.2E1 + t686 * t348 / 0.2E1 - t704 * t348 / 0.2
     #E1 - t813 * t348 / 0.2E1) * t24 * t80 + t820 * t348 / 0.2E1
        
        utnew(i,j) = t
     #2 + (dt * t347 + dt * t375 - dt * t509 - dt * t529) * t24 * t35 + 
     #(dt * t553 + dt * t686 - dt * t704 - dt * t813) * t24 * t80 + t820
     # * dt


        return
      end
