      subroutine duStepWaveGen2d2cc_tzOLD( 
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
        real t104
        real t105
        real t107
        real t108
        real t110
        real t111
        real t114
        real t117
        real t118
        real t119
        real t12
        real t121
        real t122
        real t125
        real t13
        real t130
        real t133
        real t14
        real t141
        real t144
        real t145
        real t146
        real t148
        real t149
        real t15
        real t152
        real t160
        real t168
        real t169
        real t17
        real t172
        real t173
        real t175
        real t18
        real t180
        real t181
        real t19
        real t199
        real t2
        integer t204
        real t205
        real t206
        real t208
        real t209
        real t21
        real t211
        real t212
        real t213
        real t214
        real t216
        real t218
        real t219
        real t22
        real t220
        real t222
        real t223
        real t227
        real t229
        real t230
        real t232
        real t233
        real t235
        real t239
        real t24
        real t242
        real t243
        real t244
        real t245
        real t246
        real t247
        real t249
        real t25
        real t250
        real t254
        real t256
        real t26
        real t260
        real t264
        real t267
        real t268
        real t269
        real t27
        real t271
        real t272
        real t274
        real t275
        real t279
        real t281
        real t285
        real t288
        real t289
        real t29
        real t290
        real t292
        real t293
        real t294
        real t296
        real t298
        real t299
        real t300
        real t301
        real t302
        real t304
        real t306
        real t307
        real t308
        real t31
        real t316
        real t317
        real t319
        real t32
        real t323
        real t327
        real t33
        real t331
        real t332
        real t335
        real t338
        real t342
        real t345
        real t348
        real t35
        real t350
        real t351
        real t353
        real t359
        real t36
        integer t366
        real t367
        real t368
        real t37
        real t370
        real t371
        real t374
        real t375
        real t376
        real t38
        real t382
        real t384
        real t392
        real t395
        real t4
        real t405
        real t406
        real t408
        real t409
        real t412
        real t420
        real t426
        real t430
        real t431
        real t432
        real t433
        real t434
        real t437
        integer t45
        real t451
        real t452
        real t455
        real t456
        real t458
        real t46
        real t463
        real t464
        real t47
        real t489
        real t49
        real t493
        real t498
        integer t5
        real t50
        real t502
        real t505
        real t513
        real t52
        real t522
        real t53
        real t535
        real t54
        real t542
        real t543
        real t546
        real t547
        real t549
        real t55
        real t554
        real t555
        integer t564
        real t565
        real t572
        real t574
        real t578
        real t582
        real t592
        real t593
        real t595
        real t596
        real t599
        real t6
        real t61
        real t615
        real t616
        real t63
        real t64
        real t641
        real t645
        real t649
        real t654
        real t667
        real t674
        real t675
        real t678
        real t679
        real t681
        real t686
        real t687
        integer t696
        real t697
        real t7
        real t704
        real t706
        integer t71
        real t710
        real t714
        real t72
        real t724
        real t725
        real t727
        real t728
        real t731
        real t74
        real t747
        real t748
        integer t76
        real t77
        real t773
        real t777
        real t784
        real t87
        real t88
        real t9
        real t90
        real t91
        real t93
        real t97
        t1 = u(i,j,n)
        t2 = ut(i,j,n)
        t4 = cc ** 2
        t5 = i + 1
        t6 = rx(t5,j,0,0)
        t7 = rx(t5,j,1,1)
        t9 = rx(t5,j,1,0)
        t10 = rx(t5,j,0,1)
        t12 = t6 * t7 - t9 * t10
        t13 = 0.1E1 / t12
        t14 = t6 ** 2
        t15 = t10 ** 2
        t17 = t13 * (t14 + t15)
        t18 = rx(i,j,0,0)
        t19 = rx(i,j,1,1)
        t21 = rx(i,j,1,0)
        t22 = rx(i,j,0,1)
        t24 = t18 * t19 - t21 * t22
        t25 = 0.1E1 / t24
        t26 = t18 ** 2
        t27 = t22 ** 2
        t29 = t25 * (t26 + t27)
        t31 = t17 / 0.2E1 + t29 / 0.2E1
        t32 = t4 * t31
        t33 = u(t5,j,n)
        t35 = 0.1E1 / dx
        t36 = (t33 - t1) * t35
        t37 = t32 * t36
        t38 = ut(t5,j,n)
        t45 = i + 2
        t46 = rx(t45,j,0,0)
        t47 = rx(t45,j,1,1)
        t49 = rx(t45,j,1,0)
        t50 = rx(t45,j,0,1)
        t53 = 0.1E1 / (t46 * t47 - t49 * t50)
        t54 = t46 ** 2
        t55 = t50 ** 2
        t61 = u(t45,j,n)
        t63 = (t61 - t33) * t35
        t71 = j + 1
        t72 = u(t45,t71,n)
        t74 = 0.1E1 / dy
        t76 = j - 1
        t77 = u(t45,t76,n)
        t87 = t6 * t9 + t10 * t7
        t88 = u(t5,t71,n)
        t90 = (t88 - t33) * t74
        t91 = u(t5,t76,n)
        t93 = (t33 - t91) * t74
        t52 = t4 * t13 * t87
        t97 = t52 * (t90 / 0.2E1 + t93 / 0.2E1)
        t104 = t18 * t21 + t22 * t19
        t105 = u(i,t71,n)
        t107 = (t105 - t1) * t74
        t108 = u(i,t76,n)
        t110 = (t1 - t108) * t74
        t64 = t4 * t25 * t104
        t114 = t64 * (t107 / 0.2E1 + t110 / 0.2E1)
        t117 = (t97 - t114) * t35 / 0.2E1
        t118 = rx(t5,t71,0,0)
        t119 = rx(t5,t71,1,1)
        t121 = rx(t5,t71,1,0)
        t122 = rx(t5,t71,0,1)
        t125 = 0.1E1 / (t118 * t119 - t121 * t122)
        t133 = (t88 - t105) * t35
        t141 = t52 * (t63 / 0.2E1 + t36 / 0.2E1)
        t145 = rx(t5,t76,0,0)
        t146 = rx(t5,t76,1,1)
        t148 = rx(t5,t76,1,0)
        t149 = rx(t5,t76,0,1)
        t152 = 0.1E1 / (t145 * t146 - t148 * t149)
        t160 = (t91 - t108) * t35
        t168 = t121 ** 2
        t169 = t119 ** 2
        t172 = t9 ** 2
        t173 = t7 ** 2
        t175 = t13 * (t172 + t173)
        t180 = t148 ** 2
        t181 = t146 ** 2
        t199 = (t38 - t2) * t35
        t204 = i - 1
        t205 = rx(t204,j,0,0)
        t206 = rx(t204,j,1,1)
        t208 = rx(t204,j,1,0)
        t209 = rx(t204,j,0,1)
        t211 = t205 * t206 - t208 * t209
        t212 = 0.1E1 / t211
        t213 = t205 ** 2
        t214 = t209 ** 2
        t216 = t212 * (t213 + t214)
        t218 = t29 / 0.2E1 + t216 / 0.2E1
        t219 = t4 * t218
        t220 = u(t204,j,n)
        t222 = (t1 - t220) * t35
        t223 = t219 * t222
        t229 = t205 * t208 + t209 * t206
        t230 = u(t204,t71,n)
        t232 = (t230 - t220) * t74
        t233 = u(t204,t76,n)
        t235 = (t220 - t233) * t74
        t111 = t4 * t212 * t229
        t239 = t111 * (t232 / 0.2E1 + t235 / 0.2E1)
        t242 = (t114 - t239) * t35 / 0.2E1
        t243 = rx(i,t71,0,0)
        t244 = rx(i,t71,1,1)
        t246 = rx(i,t71,1,0)
        t247 = rx(i,t71,0,1)
        t249 = t243 * t244 - t246 * t247
        t250 = 0.1E1 / t249
        t254 = t243 * t246 + t247 * t244
        t256 = (t105 - t230) * t35
        t130 = t4 * t250 * t254
        t260 = t130 * (t133 / 0.2E1 + t256 / 0.2E1)
        t264 = t64 * (t36 / 0.2E1 + t222 / 0.2E1)
        t267 = (t260 - t264) * t74 / 0.2E1
        t268 = rx(i,t76,0,0)
        t269 = rx(i,t76,1,1)
        t271 = rx(i,t76,1,0)
        t272 = rx(i,t76,0,1)
        t274 = t268 * t269 - t271 * t272
        t275 = 0.1E1 / t274
        t279 = t268 * t271 + t272 * t269
        t281 = (t108 - t233) * t35
        t144 = t4 * t275 * t279
        t285 = t144 * (t160 / 0.2E1 + t281 / 0.2E1)
        t288 = (t264 - t285) * t74 / 0.2E1
        t289 = t246 ** 2
        t290 = t244 ** 2
        t292 = t250 * (t289 + t290)
        t293 = t21 ** 2
        t294 = t19 ** 2
        t296 = t25 * (t293 + t294)
        t298 = t292 / 0.2E1 + t296 / 0.2E1
        t299 = t4 * t298
        t300 = t299 * t107
        t301 = t271 ** 2
        t302 = t269 ** 2
        t304 = t275 * (t301 + t302)
        t306 = t296 / 0.2E1 + t304 / 0.2E1
        t307 = t4 * t306
        t308 = t307 * t110
        t316 = dt * (((t37 - t223) * t35 + t117 + t242 + t267 + t288 + (
     #t300 - t308) * t74) * t24 + src(i,j,nComp,n)) / 0.2E1
        t317 = ut(t204,j,n)
        t319 = (t2 - t317) * t35
        t323 = dx * (t199 / 0.2E1 + t319 / 0.2E1) / 0.2E1
        t327 = sqrt(0.2E1 * t14 + 0.2E1 * t15 + 0.2E1 * t26 + 0.2E1 * t2
     #7)
        t227 = t4 * t125 * (t118 * t121 + t122 * t119)
        t245 = t4 * t152 * (t145 * t148 + t149 * t146)
        t331 = t37 + t32 * dt * t199 / 0.2E1 + cc * t31 * (t38 + dt * ((
     #(t4 * (t53 * (t54 + t55) / 0.2E1 + t17 / 0.2E1) * t63 - t37) * t35
     # + (t4 * t53 * (t46 * t49 + t50 * t47) * ((t72 - t61) * t74 / 0.2E
     #1 + (t61 - t77) * t74 / 0.2E1) - t97) * t35 / 0.2E1 + t117 + (t227
     # * ((t72 - t88) * t35 / 0.2E1 + t133 / 0.2E1) - t141) * t74 / 0.2E
     #1 + (t141 - t245 * ((t77 - t91) * t35 / 0.2E1 + t160 / 0.2E1)) * t
     #74 / 0.2E1 + (t4 * (t125 * (t168 + t169) / 0.2E1 + t175 / 0.2E1) *
     # t90 - t4 * (t175 / 0.2E1 + t152 * (t180 + t181) / 0.2E1) * t93) *
     # t74) * t12 + src(t5,j,nComp,n)) / 0.2E1 - dx * ((ut(t45,j,n) - t3
     #8) * t35 / 0.2E1 + t199 / 0.2E1) / 0.2E1 - t2 - t316 - t323) / t32
     #7
        t332 = dt ** 2
        t335 = t25 * t104
        t338 = t4 * (t13 * t87 / 0.2E1 + t335 / 0.2E1)
        t342 = ut(t5,t71,n)
        t345 = ut(t5,t76,n)
        t348 = ut(i,t71,n)
        t350 = (t348 - t2) * t74
        t351 = ut(i,t76,n)
        t353 = (t2 - t351) * t74
        t359 = t338 * (t90 / 0.4E1 + t93 / 0.4E1 + t107 / 0.4E1 + t110 /
     # 0.4E1) + t338 * dt * ((t342 - t38) * t74 / 0.4E1 + (t38 - t345) *
     # t74 / 0.4E1 + t350 / 0.4E1 + t353 / 0.4E1) / 0.2E1
        t366 = i - 2
        t367 = rx(t366,j,0,0)
        t368 = rx(t366,j,1,1)
        t370 = rx(t366,j,1,0)
        t371 = rx(t366,j,0,1)
        t374 = 0.1E1 / (t367 * t368 - t370 * t371)
        t375 = t367 ** 2
        t376 = t371 ** 2
        t382 = u(t366,j,n)
        t384 = (t220 - t382) * t35
        t392 = u(t366,t71,n)
        t395 = u(t366,t76,n)
        t405 = rx(t204,t71,0,0)
        t406 = rx(t204,t71,1,1)
        t408 = rx(t204,t71,1,0)
        t409 = rx(t204,t71,0,1)
        t412 = 0.1E1 / (t405 * t406 - t408 * t409)
        t426 = t111 * (t222 / 0.2E1 + t384 / 0.2E1)
        t430 = rx(t204,t76,0,0)
        t431 = rx(t204,t76,1,1)
        t433 = rx(t204,t76,1,0)
        t434 = rx(t204,t76,0,1)
        t437 = 0.1E1 / (t430 * t431 - t433 * t434)
        t451 = t408 ** 2
        t452 = t406 ** 2
        t455 = t208 ** 2
        t456 = t206 ** 2
        t458 = t212 * (t455 + t456)
        t463 = t433 ** 2
        t464 = t431 ** 2
        t489 = sqrt(0.2E1 * t26 + 0.2E1 * t27 + 0.2E1 * t213 + 0.2E1 * t
     #214)
        t420 = t4 * t412 * (t405 * t408 + t409 * t406)
        t432 = t4 * t437 * (t430 * t433 + t434 * t431)
        t493 = t223 + t219 * dt * t319 / 0.2E1 + cc * t218 * (t2 + t316 
     #- t323 - t317 - dt * (((t223 - t4 * (t216 / 0.2E1 + t374 * (t375 +
     # t376) / 0.2E1) * t384) * t35 + t242 + (t239 - t4 * t374 * (t367 *
     # t370 + t371 * t368) * ((t392 - t382) * t74 / 0.2E1 + (t382 - t395
     #) * t74 / 0.2E1)) * t35 / 0.2E1 + (t420 * (t256 / 0.2E1 + (t230 - 
     #t392) * t35 / 0.2E1) - t426) * t74 / 0.2E1 + (t426 - t432 * (t281 
     #/ 0.2E1 + (t233 - t395) * t35 / 0.2E1)) * t74 / 0.2E1 + (t4 * (t41
     #2 * (t451 + t452) / 0.2E1 + t458 / 0.2E1) * t232 - t4 * (t458 / 0.
     #2E1 + t437 * (t463 + t464) / 0.2E1) * t235) * t74) * t211 + src(t2
     #04,j,nComp,n)) / 0.2E1 - dx * (t319 / 0.2E1 + (t317 - ut(t366,j,n)
     #) * t35 / 0.2E1) / 0.2E1) / t489
        t498 = t4 * (t335 / 0.2E1 + t212 * t229 / 0.2E1)
        t502 = ut(t204,t71,n)
        t505 = ut(t204,t76,n)
        t513 = t498 * (t107 / 0.4E1 + t110 / 0.4E1 + t232 / 0.4E1 + t235
     # / 0.4E1) + t498 * dt * (t350 / 0.4E1 + t353 / 0.4E1 + (t502 - t31
     #7) * t74 / 0.4E1 + (t317 - t505) * t74 / 0.4E1) / 0.2E1
        t522 = t4 * (t250 * t254 / 0.2E1 + t335 / 0.2E1)
        t535 = t522 * (t133 / 0.4E1 + t256 / 0.4E1 + t36 / 0.4E1 + t222 
     #/ 0.4E1) + t522 * dt * ((t342 - t348) * t35 / 0.4E1 + (t348 - t502
     #) * t35 / 0.4E1 + t199 / 0.4E1 + t319 / 0.4E1) / 0.2E1
        t542 = t118 ** 2
        t543 = t122 ** 2
        t546 = t243 ** 2
        t547 = t247 ** 2
        t549 = t250 * (t546 + t547)
        t554 = t405 ** 2
        t555 = t409 ** 2
        t564 = j + 2
        t565 = u(t5,t564,n)
        t572 = u(i,t564,n)
        t574 = (t572 - t105) * t74
        t578 = t130 * (t574 / 0.2E1 + t107 / 0.2E1)
        t582 = u(t204,t564,n)
        t592 = rx(i,t564,0,0)
        t593 = rx(i,t564,1,1)
        t595 = rx(i,t564,1,0)
        t596 = rx(i,t564,0,1)
        t599 = 0.1E1 / (t592 * t593 - t595 * t596)
        t615 = t595 ** 2
        t616 = t593 ** 2
        t641 = dy * (t350 / 0.2E1 + t353 / 0.2E1) / 0.2E1
        t645 = sqrt(0.2E1 * t289 + 0.2E1 * t290 + 0.2E1 * t293 + 0.2E1 *
     # t294)
        t649 = t300 + t299 * dt * t350 / 0.2E1 + cc * t298 * (t348 + dt 
     #* (((t4 * (t125 * (t542 + t543) / 0.2E1 + t549 / 0.2E1) * t133 - t
     #4 * (t549 / 0.2E1 + t412 * (t554 + t555) / 0.2E1) * t256) * t35 + 
     #(t227 * ((t565 - t88) * t74 / 0.2E1 + t90 / 0.2E1) - t578) * t35 /
     # 0.2E1 + (t578 - t420 * ((t582 - t230) * t74 / 0.2E1 + t232 / 0.2E
     #1)) * t35 / 0.2E1 + (t4 * t599 * (t592 * t595 + t596 * t593) * ((t
     #565 - t572) * t35 / 0.2E1 + (t572 - t582) * t35 / 0.2E1) - t260) *
     # t74 / 0.2E1 + t267 + (t4 * (t599 * (t615 + t616) / 0.2E1 + t292 /
     # 0.2E1) * t574 - t300) * t74) * t249 + src(i,t71,nComp,n)) / 0.2E1
     # - dy * ((ut(i,t564,n) - t348) * t74 / 0.2E1 + t350 / 0.2E1) / 0.2
     #E1 - t2 - t316 - t641) / t645
        t654 = t4 * (t335 / 0.2E1 + t275 * t279 / 0.2E1)
        t667 = t654 * (t36 / 0.4E1 + t222 / 0.4E1 + t160 / 0.4E1 + t281 
     #/ 0.4E1) + t654 * dt * (t199 / 0.4E1 + t319 / 0.4E1 + (t345 - t351
     #) * t35 / 0.4E1 + (t351 - t505) * t35 / 0.4E1) / 0.2E1
        t674 = t145 ** 2
        t675 = t149 ** 2
        t678 = t268 ** 2
        t679 = t272 ** 2
        t681 = t275 * (t678 + t679)
        t686 = t430 ** 2
        t687 = t434 ** 2
        t696 = j - 2
        t697 = u(t5,t696,n)
        t704 = u(i,t696,n)
        t706 = (t108 - t704) * t74
        t710 = t144 * (t110 / 0.2E1 + t706 / 0.2E1)
        t714 = u(t204,t696,n)
        t724 = rx(i,t696,0,0)
        t725 = rx(i,t696,1,1)
        t727 = rx(i,t696,1,0)
        t728 = rx(i,t696,0,1)
        t731 = 0.1E1 / (t724 * t725 - t727 * t728)
        t747 = t727 ** 2
        t748 = t725 ** 2
        t773 = sqrt(0.2E1 * t293 + 0.2E1 * t294 + 0.2E1 * t301 + 0.2E1 *
     # t302)
        t777 = t308 + t307 * dt * t353 / 0.2E1 + cc * t306 * (t2 + t316 
     #- t641 - t351 - dt * (((t4 * (t152 * (t674 + t675) / 0.2E1 + t681 
     #/ 0.2E1) * t160 - t4 * (t681 / 0.2E1 + t437 * (t686 + t687) / 0.2E
     #1) * t281) * t35 + (t245 * (t93 / 0.2E1 + (t91 - t697) * t74 / 0.2
     #E1) - t710) * t35 / 0.2E1 + (t710 - t432 * (t235 / 0.2E1 + (t233 -
     # t714) * t74 / 0.2E1)) * t35 / 0.2E1 + t288 + (t285 - t4 * t731 * 
     #(t724 * t727 + t728 * t725) * ((t697 - t704) * t35 / 0.2E1 + (t704
     # - t714) * t35 / 0.2E1)) * t74 / 0.2E1 + (t308 - t4 * (t304 / 0.2E
     #1 + t731 * (t747 + t748) / 0.2E1) * t706) * t74) * t274 + src(i,t7
     #6,nComp,n)) / 0.2E1 - dy * (t353 / 0.2E1 + (t351 - ut(i,t696,n)) *
     # t74 / 0.2E1) / 0.2E1) / t773

        t784 = src(i,j,nComp,n + 1)

        unew(i,j) = t1 + dt * t2 + (t331 * t332 / 0.2E1 + t359 * t3
     #32 / 0.2E1 - t493 * t332 / 0.2E1 - t513 * t332 / 0.2E1) * t24 * t3
     #5 + (t535 * t332 / 0.2E1 + t649 * t332 / 0.2E1 - t667 * t332 / 0.2
     #E1 - t777 * t332 / 0.2E1) * t24 * t74 + t784 * t332 / 0.2E1

        utnew(i,j) = t
     #2 + (t331 * dt + t359 * dt - t493 * dt - t513 * dt) * t24 * t35 + 
     #(t535 * dt + t649 * dt - t667 * dt - t777 * dt) * t24 * t74 + t784
     # * dt

c        blah = array(int(t1 + dt * t2 + (t331 * t332 / 0.2E1 + t359 * t3
c     #32 / 0.2E1 - t493 * t332 / 0.2E1 - t513 * t332 / 0.2E1) * t24 * t3
c     #5 + (t535 * t332 / 0.2E1 + t649 * t332 / 0.2E1 - t667 * t332 / 0.2
c     #E1 - t777 * t332 / 0.2E1) * t24 * t74 + t784 * t332 / 0.2E1),int(t
c     #2 + (t331 * dt + t359 * dt - t493 * dt - t513 * dt) * t24 * t35 + 
c     #(t535 * dt + t649 * dt - t667 * dt - t777 * dt) * t24 * t74 + t784
c     # * dt))

        return
      end
