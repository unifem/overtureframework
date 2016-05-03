      subroutine duStepWaveGen3d4cc( 
     *   nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *   n1a,n1b,n2a,n2b,n3a,n3b,
     *   u,ut,unew,utnew,rx,
     *   dx,dy,dz,dt,cc,beta,
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
      real rx   (nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:2,0:2)
      real dx,dy,dz,dt,cc,beta
c
c.. generated code to follow
c
        real t1
        real t100
        real t1000
        real t10004
        real t10005
        real t10009
        real t10018
        real t10019
        real t10022
        real t10024
        real t10026
        real t10028
        real t10029
        real t10031
        real t10033
        real t10035
        real t10036
        real t10038
        real t1004
        real t10040
        real t10042
        real t10043
        real t10045
        real t10047
        real t10048
        real t10049
        real t1005
        real t10050
        real t10052
        real t10054
        real t10056
        real t10058
        real t10060
        real t10061
        real t10063
        real t10065
        real t10067
        real t10068
        real t10069
        real t1007
        real t10070
        real t10071
        real t10072
        real t10073
        real t10074
        real t10075
        real t10077
        real t10079
        real t10081
        real t10082
        real t10084
        real t10086
        real t10088
        real t10089
        real t10091
        real t10093
        real t10095
        real t10096
        real t10098
        real t101
        real t1010
        real t10100
        real t10101
        real t10102
        real t10103
        real t10105
        real t10107
        real t10109
        real t10111
        real t10113
        real t10114
        real t10116
        real t10118
        real t10120
        real t10121
        real t10122
        real t10123
        real t10124
        real t10125
        real t10126
        real t10127
        real t10129
        real t10135
        real t10139
        real t1014
        real t10142
        real t10145
        real t10147
        real t10149
        real t10156
        real t10158
        real t10159
        real t10162
        real t10163
        real t10169
        real t1017
        real t1018
        real t10180
        real t10181
        real t10182
        real t10185
        real t10186
        real t10187
        real t10188
        real t10189
        real t10192
        real t10193
        real t10195
        real t10198
        real t10199
        real t1020
        real t10202
        real t10203
        real t10207
        real t1021
        real t10211
        real t10215
        real t1022
        real t10221
        real t10222
        real t10223
        real t10224
        real t10229
        real t10231
        integer t10232
        real t10233
        real t10235
        real t1024
        real t10243
        real t10245
        real t10252
        real t10255
        real t10257
        real t10265
        real t10271
        real t10272
        real t10274
        real t10276
        real t10278
        real t10280
        real t10281
        real t10285
        real t10287
        real t1029
        real t10293
        real t10294
        real t10299
        real t1030
        real t10301
        real t10302
        real t10304
        real t10310
        real t1032
        real t10335
        real t10337
        real t10341
        real t10343
        real t10344
        real t10349
        real t1035
        real t10361
        real t10373
        real t10383
        real t10395
        real t104
        real t10400
        real t10410
        real t10414
        real t10431
        real t1045
        real t10454
        real t1046
        integer t1047
        real t1048
        real t10489
        real t1049
        real t105
        real t1051
        real t10512
        real t1053
        real t10534
        real t10542
        real t10546
        real t1055
        real t1057
        real t10575
        real t10576
        real t10577
        real t10578
        real t10579
        real t1058
        real t10587
        real t10592
        real t106
        real t10602
        real t10614
        real t1062
        real t10627
        real t10630
        real t10638
        real t1064
        real t10640
        real t10642
        real t10645
        real t10649
        real t10650
        real t10654
        real t10656
        real t10658
        real t10660
        real t10663
        real t10665
        real t10667
        real t10670
        real t10671
        real t10672
        real t10673
        real t10675
        real t10676
        real t10677
        real t10678
        real t10680
        real t10683
        real t10684
        real t10685
        real t10686
        real t10687
        real t10689
        real t1069
        real t10692
        real t10693
        real t10699
        real t1070
        real t10701
        real t10707
        real t1071
        real t10710
        real t10714
        real t10716
        real t10719
        real t10721
        real t10723
        real t10725
        real t10728
        real t10730
        real t10732
        real t10735
        real t10739
        real t10741
        real t10743
        real t10746
        real t10750
        real t10752
        real t10755
        real t10756
        real t10757
        real t10758
        real t1076
        real t10760
        real t10761
        real t10762
        real t10763
        real t10765
        real t10768
        real t10769
        real t1077
        real t10770
        real t10771
        real t10772
        real t10774
        real t10777
        real t10778
        real t10781
        real t10784
        real t10786
        real t10788
        real t10789
        real t1079
        real t10791
        real t10799
        real t108
        real t1080
        real t10801
        real t10808
        real t10811
        real t10813
        real t1082
        real t10827
        real t10830
        real t10838
        real t1084
        real t10847
        real t10849
        real t10853
        real t10855
        real t1086
        real t10865
        real t10868
        real t10870
        real t10874
        real t10876
        real t1088
        real t10890
        real t10897
        real t109
        real t1090
        real t10902
        real t10910
        real t10912
        real t10918
        real t1092
        real t10922
        real t10925
        real t10927
        real t10933
        real t1094
        real t10947
        real t10957
        real t1096
        real t10961
        real t10972
        real t10975
        real t10978
        real t1098
        real t10982
        real t10986
        real t10990
        real t10993
        real t10996
        real t11
        real t110
        real t11000
        real t11004
        real t11007
        real t11012
        real t11017
        real t11018
        real t1102
        real t11022
        real t11024
        real t11026
        real t11034
        real t11038
        real t1104
        real t11052
        real t11057
        real t1106
        real t11067
        real t11071
        real t11076
        real t1108
        real t11096
        real t11101
        integer t1111
        real t11110
        real t11115
        real t1112
        real t1113
        real t11132
        real t11134
        real t11140
        real t11144
        real t11147
        real t11149
        real t1115
        real t11155
        real t1117
        real t11173
        real t11182
        real t11187
        real t11188
        real t1119
        real t112
        real t1121
        real t11210
        real t11215
        real t11219
        real t1122
        real t11221
        real t11222
        real t11225
        real t11228
        real t11233
        real t11234
        real t11235
        real t11250
        real t11253
        real t11257
        real t1126
        real t11261
        real t11265
        real t11268
        real t11272
        real t1128
        real t11283
        real t11299
        real t113
        real t11303
        real t11312
        real t11322
        real t11328
        real t1133
        real t11330
        real t11331
        real t11334
        real t11336
        real t11339
        real t1134
        real t11342
        real t11344
        real t11348
        real t11349
        real t1135
        real t11351
        real t11353
        real t11355
        real t11357
        real t11358
        real t11362
        real t11364
        real t11370
        real t11371
        real t11372
        real t11373
        real t11382
        real t11387
        real t1140
        real t11401
        real t11404
        real t11406
        real t1141
        real t11414
        real t11415
        real t11417
        real t11419
        real t11421
        real t11423
        real t11424
        real t11428
        real t1143
        real t11430
        real t11436
        real t11437
        real t1144
        real t11451
        real t11452
        real t11453
        real t1146
        real t11466
        real t11469
        real t11473
        real t11479
        real t1148
        real t11480
        real t11482
        real t11484
        real t11486
        real t11488
        real t11489
        real t11493
        real t11495
        real t115
        real t1150
        real t11501
        real t11502
        real t1151
        real t11510
        real t11514
        real t11518
        real t11519
        real t1152
        real t11521
        real t11523
        real t11525
        real t11527
        real t11528
        real t11532
        real t11534
        real t11540
        real t11541
        real t11549
        real t11562
        real t11566
        real t11577
        real t11583
        real t11584
        real t11585
        real t11588
        real t11589
        real t11590
        real t11592
        real t11597
        real t11598
        real t11599
        real t116
        real t11607
        real t11608
        real t11609
        real t1161
        real t11612
        real t11613
        real t11615
        real t11617
        real t11619
        real t1162
        real t11621
        real t11622
        real t11626
        real t11628
        real t1163
        real t11634
        real t11635
        real t11636
        real t11637
        real t1164
        real t11646
        real t1165
        real t11651
        real t11665
        real t11668
        real t1167
        real t11678
        real t11679
        real t11681
        real t11683
        real t11685
        real t11687
        real t11688
        real t11692
        real t11694
        real t11700
        real t11701
        real t11715
        real t11716
        real t11717
        real t11730
        real t11733
        real t11737
        real t11738
        real t11743
        real t11744
        real t11746
        real t11748
        real t11750
        real t11752
        real t11753
        real t11757
        real t11759
        real t11760
        real t11765
        real t11766
        real t11767
        real t1177
        real t11773
        real t11774
        real t11778
        real t11782
        real t11783
        real t11785
        real t11787
        real t11789
        real t1179
        real t11791
        real t11792
        real t11796
        real t11798
        real t11804
        real t11805
        real t11813
        real t11826
        real t11830
        real t1184
        real t11841
        real t11847
        real t11848
        real t11849
        real t1185
        real t11852
        real t11853
        real t11854
        real t11856
        real t1186
        real t11861
        real t11862
        real t11863
        real t11872
        real t11873
        real t1188
        real t11883
        real t11884
        real t11886
        real t11888
        real t11889
        real t11890
        real t11892
        real t11893
        real t11897
        real t11899
        real t11905
        real t11906
        real t11907
        real t11908
        real t11911
        real t11916
        real t11917
        real t11922
        real t11937
        real t11950
        real t11954
        real t1196
        real t11961
        real t11967
        real t11968
        real t11969
        real t11972
        real t11973
        real t11974
        real t11976
        real t1198
        real t11981
        real t11982
        real t11983
        real t11992
        real t11996
        real t120
        real t12000
        real t12004
        real t12008
        real t12014
        real t12015
        real t12017
        real t12019
        real t12021
        real t12023
        real t12024
        real t12028
        real t1203
        real t12030
        real t12035
        real t12036
        real t12037
        real t1205
        real t12051
        real t12056
        real t12060
        real t12062
        real t12066
        real t12067
        real t12068
        real t1207
        real t12077
        real t12078
        real t12081
        real t12082
        real t12084
        real t12086
        real t12088
        real t1209
        real t12090
        real t12091
        real t12095
        real t12097
        integer t121
        real t12103
        real t12104
        real t12105
        real t12106
        real t1211
        real t12115
        real t1213
        real t12135
        real t12148
        real t1215
        real t12152
        real t12159
        real t1216
        real t12165
        real t12166
        real t12167
        real t12168
        real t12170
        real t12171
        real t12172
        real t12174
        real t12179
        real t1218
        real t12180
        real t12181
        real t12188
        real t12190
        real t12194
        real t12195
        real t12198
        real t122
        real t1220
        real t12201
        real t12202
        real t12206
        real t12212
        real t12213
        real t12215
        real t12217
        real t12219
        real t1222
        real t12221
        real t12222
        real t12226
        real t12228
        real t12234
        real t12235
        real t1224
        real t12258
        real t1226
        real t12264
        real t12265
        real t12266
        real t1227
        real t12275
        real t12276
        real t1228
        real t1229
        real t12292
        real t12293
        real t12295
        real t12299
        real t1231
        real t12312
        real t12313
        real t12314
        real t1232
        real t12323
        real t12328
        real t12333
        real t12334
        real t12336
        real t12338
        real t1234
        real t12340
        real t12342
        real t12343
        real t12347
        real t12349
        real t12355
        real t12356
        real t1236
        real t12364
        real t12370
        real t12371
        real t12372
        real t1238
        real t12385
        real t12389
        real t12395
        real t12396
        real t12398
        real t124
        real t1240
        real t12400
        real t12402
        real t12404
        real t12405
        real t12409
        real t12411
        real t12417
        real t12418
        real t1242
        real t12426
        real t1243
        real t12439
        real t1244
        real t12445
        real t12446
        real t12447
        real t12456
        real t12457
        real t1246
        real t12460
        real t12461
        real t12462
        real t1247
        real t12480
        real t12481
        real t12482
        real t12484
        real t12486
        real t12488
        real t1249
        real t12490
        real t12491
        real t12492
        real t12495
        real t12497
        real t12499
        real t125
        real t12503
        real t12504
        real t12506
        real t1251
        real t12512
        real t12518
        real t12519
        real t12520
        real t1253
        real t12533
        real t12537
        real t12542
        real t12543
        real t12544
        real t12546
        real t12548
        real t1255
        real t12550
        real t12552
        real t12553
        real t12554
        real t12557
        real t12559
        real t1256
        real t12561
        real t12565
        real t12566
        real t12568
        real t12574
        real t1258
        real t12587
        real t12593
        real t12594
        real t12595
        integer t126
        real t1260
        real t12604
        real t12605
        real t12613
        real t12617
        real t12618
        real t12619
        real t1262
        real t12638
        real t12639
        real t1264
        real t12641
        real t12643
        real t12645
        real t12647
        real t12648
        real t12652
        real t12654
        real t1266
        real t12660
        real t12661
        real t12669
        real t12675
        real t12676
        real t12677
        real t1268
        real t1269
        real t12690
        real t12694
        real t127
        real t1270
        real t12700
        real t12701
        real t12703
        real t12705
        real t12707
        real t12709
        real t1271
        real t12710
        real t12714
        real t12716
        real t1272
        real t12722
        real t12723
        real t12731
        real t12743
        real t12744
        real t12749
        real t12750
        real t12751
        real t12752
        real t12761
        real t12762
        real t12765
        real t12766
        real t12767
        real t1277
        real t12786
        real t12787
        real t12789
        real t1279
        real t12791
        real t12793
        real t12794
        real t12795
        real t12796
        real t128
        real t1280
        real t12800
        real t12802
        real t12803
        real t12808
        real t12809
        real t12817
        real t1282
        real t12823
        real t12824
        real t12825
        real t12838
        real t1284
        real t12842
        real t12848
        real t12849
        real t12851
        real t12853
        real t12855
        real t12857
        real t12858
        real t1286
        real t12862
        real t12864
        real t12870
        real t12871
        real t12879
        real t1288
        real t12892
        real t12898
        real t12899
        real t129
        real t12900
        real t12909
        real t12910
        real t1292
        real t12927
        real t12949
        real t12968
        real t12969
        real t12974
        real t12976
        real t12985
        real t12987
        real t12988
        real t1299
        real t12990
        real t12994
        real t12995
        real t12997
        real t12999
        real t13
        real t13007
        real t1301
        real t13013
        real t1302
        real t13021
        real t13025
        real t13026
        real t1304
        real t13041
        real t13052
        real t1306
        real t13069
        real t13074
        real t13076
        real t1308
        real t13082
        real t13088
        real t13093
        real t13098
        real t131
        real t1310
        real t13104
        real t13109
        real t13122
        real t13137
        real t13138
        real t13142
        real t13143
        real t13147
        real t13149
        real t13154
        real t13155
        real t13159
        real t13165
        real t13170
        real t13182
        real t13183
        real t13198
        real t13199
        real t1320
        real t13200
        real t13203
        real t13209
        real t13210
        real t13213
        real t13216
        real t13218
        real t13219
        real t13221
        real t13223
        real t13236
        real t1325
        real t13254
        real t13255
        real t13258
        real t13274
        real t1328
        real t13285
        real t133
        real t13302
        real t13307
        real t13309
        real t13315
        real t13319
        real t13326
        real t13331
        real t13349
        real t1335
        real t13362
        real t13363
        real t13368
        real t13372
        real t13379
        real t13380
        real t13384
        real t13385
        real t1339
        real t134
        real t13402
        real t1341
        real t13415
        real t13416
        real t13420
        real t13426
        real t13430
        real t13433
        real t13436
        real t13438
        real t13440
        real t13453
        real t1346
        real t13471
        real t13475
        real t1348
        real t13480
        real t13483
        real t13488
        real t13496
        real t13502
        real t13504
        real t13508
        real t1351
        real t13511
        real t13513
        real t13514
        real t13517
        real t13518
        real t13524
        real t13535
        real t13536
        real t13537
        real t13540
        real t13541
        real t13542
        real t13543
        real t13544
        real t13548
        real t13550
        real t13554
        real t13557
        real t13558
        real t13565
        real t13570
        real t13572
        real t13580
        real t13581
        real t13583
        real t13589
        real t13593
        real t13596
        real t13599
        real t13601
        real t13603
        real t13611
        real t13613
        real t13617
        real t1362
        real t13620
        real t13622
        real t13623
        real t13626
        real t13627
        real t1363
        real t13633
        integer t1364
        real t13644
        real t13645
        real t13646
        real t13649
        real t1365
        real t13650
        real t13651
        real t13652
        real t13653
        real t13656
        real t13657
        real t13659
        real t13663
        real t13666
        real t13667
        real t1367
        real t13675
        real t13679
        real t13684
        real t13685
        real t13686
        real t13688
        real t13689
        real t13693
        real t13694
        real t13695
        real t13696
        real t13697
        real t13698
        real t13700
        real t13701
        real t13702
        real t13703
        real t13705
        real t13707
        real t13709
        real t1371
        real t13710
        real t13712
        real t13715
        real t13717
        real t13720
        real t13722
        real t1373
        real t13730
        real t13733
        real t13737
        real t13740
        real t13742
        real t13743
        real t13744
        real t13745
        real t13746
        real t13747
        real t13749
        integer t1375
        real t13750
        real t13751
        real t13752
        real t13754
        real t13757
        real t13758
        real t13759
        real t1376
        real t13760
        real t13761
        real t13763
        real t13766
        real t13767
        integer t13770
        real t13771
        real t13773
        real t13775
        real t13777
        real t13778
        real t1378
        real t13780
        real t13782
        real t13784
        real t13787
        real t13788
        real t13790
        real t13792
        real t13794
        real t13797
        real t138
        real t13803
        real t13809
        real t13812
        real t13816
        real t13817
        real t13818
        real t1382
        real t13821
        real t13822
        real t13823
        real t13825
        real t13826
        real t13827
        real t13829
        real t13831
        real t13832
        real t13836
        real t13838
        real t13844
        real t13845
        real t13851
        real t13853
        real t13859
        real t1386
        real t13861
        real t13862
        real t13863
        real t13864
        real t13865
        real t13868
        real t1387
        real t13871
        real t13876
        real t13878
        real t13879
        real t13881
        real t13887
        real t1389
        real t13892
        real t13894
        real t13896
        real t13899
        real t139
        real t13903
        real t13905
        real t13908
        real t1391
        real t13910
        real t13912
        real t13914
        real t13916
        real t13919
        real t13921
        real t13923
        real t13925
        real t13928
        real t13929
        real t1393
        real t13930
        real t13931
        real t13933
        real t13934
        real t13935
        real t13936
        real t13938
        real t13941
        real t13942
        real t13943
        real t13944
        real t13945
        real t13947
        real t1395
        real t13950
        real t13951
        real t13954
        real t13957
        real t1396
        real t13960
        real t13962
        real t13978
        real t1400
        real t14002
        real t14009
        real t1402
        real t14020
        real t14033
        real t14063
        real t14065
        real t1407
        real t14070
        real t1408
        real t14086
        real t1409
        real t14098
        real t141
        real t1410
        real t14108
        real t1411
        real t14120
        real t1413
        real t14157
        real t1416
        real t1417
        real t1418
        real t14197
        real t142
        real t1420
        real t1421
        real t14210
        real t14220
        real t1423
        real t14232
        real t14239
        real t14259
        real t1426
        real t14261
        real t14266
        real t1427
        real t14271
        real t14276
        real t14288
        real t1429
        real t14290
        real t14292
        real t14293
        real t14295
        real t14297
        real t143
        real t1430
        real t14301
        real t14302
        real t14304
        real t1431
        real t14310
        real t14314
        real t14317
        real t14319
        real t14325
        real t1433
        real t14334
        real t14336
        real t14344
        real t14346
        real t1435
        real t14353
        real t14356
        real t14358
        real t1436
        real t14377
        real t14381
        real t14385
        real t14393
        real t14397
        real t144
        real t1440
        real t14418
        real t1442
        real t14440
        real t14442
        real t14447
        real t14453
        real t14458
        real t14466
        real t1447
        real t14476
        real t1448
        real t1449
        real t14495
        real t1450
        real t14506
        real t14508
        real t1451
        real t14518
        real t14521
        real t14523
        real t1453
        real t14559
        real t1456
        real t1457
        real t14571
        real t1459
        real t14590
        real t146
        real t14610
        real t1462
        real t14628
        real t14637
        real t14642
        real t14656
        real t14658
        real t14661
        real t14663
        real t14664
        real t14665
        real t14666
        real t14668
        real t14669
        real t14671
        real t14674
        real t14676
        real t14678
        real t1468
        real t1469
        real t14690
        real t14692
        real t1470
        real t14706
        real t1471
        real t1472
        real t14720
        real t14722
        real t14726
        real t1473
        real t14735
        real t14746
        real t14761
        real t14767
        real t1477
        real t14771
        real t14773
        real t14775
        real t14777
        real t14782
        real t14783
        real t14784
        real t14793
        real t148
        real t14812
        real t14813
        real t14814
        real t14815
        real t14817
        real t14819
        real t14821
        real t14822
        real t14824
        real t14826
        real t14828
        real t1483
        real t14834
        real t14835
        real t14849
        real t1485
        real t14850
        real t14851
        real t14864
        real t14867
        real t14884
        real t1489
        real t14904
        real t14917
        real t14918
        real t14919
        real t14922
        real t14923
        real t14924
        real t14926
        real t14931
        real t14932
        real t14933
        real t14942
        real t14943
        real t1495
        real t14950
        real t14952
        real t14954
        real t14956
        real t1496
        real t14960
        real t14961
        real t14962
        real t14971
        real t1499
        real t14990
        real t14991
        real t14993
        real t14995
        real t14997
        real t14999
        real t15
        real t150
        real t15000
        real t15004
        real t15006
        real t15012
        real t15013
        real t15027
        real t15028
        real t15029
        real t1504
        real t15042
        real t15045
        real t1505
        real t15058
        real t1506
        real t15062
        real t15069
        real t1507
        real t15078
        real t15082
        real t15089
        real t1509
        real t15095
        real t15096
        real t15097
        real t151
        real t15100
        real t15101
        real t15102
        real t15104
        real t15109
        real t1511
        real t15110
        real t15111
        real t15120
        real t15121
        real t1513
        real t15134
        real t1515
        real t15157
        real t15158
        real t15159
        real t15162
        real t15163
        real t15164
        real t15166
        real t1517
        real t15171
        real t15172
        real t15173
        real t15185
        real t1519
        real t15197
        real t152
        real t1520
        real t15206
        real t15207
        real t15209
        real t1521
        real t15211
        real t15213
        real t15215
        real t15216
        real t15220
        real t15222
        real t15228
        real t15229
        real t1523
        real t15245
        real t15246
        real t15247
        real t1525
        real t15260
        real t1527
        real t15270
        real t15271
        real t15273
        real t15275
        real t15277
        real t15279
        real t15280
        real t15284
        real t15286
        real t15292
        real t15293
        real t15303
        real t1531
        real t15322
        real t15323
        real t15324
        real t1533
        real t15333
        real t15334
        real t15337
        real t15338
        real t15339
        real t1534
        real t15342
        real t15343
        real t15344
        real t15346
        real t1535
        real t15351
        real t15352
        real t15353
        real t15364
        real t15365
        real t1537
        real t15377
        real t15386
        real t15387
        real t15389
        real t1539
        real t15391
        real t15393
        real t15395
        real t15396
        real t15400
        real t15402
        real t15408
        real t15409
        real t1541
        real t15425
        real t15426
        real t15427
        real t1544
        real t15440
        real t15450
        real t15451
        real t15453
        real t15455
        real t15457
        real t15459
        real t15460
        real t15464
        real t15466
        real t15469
        real t15472
        real t15473
        real t15483
        real t15487
        real t1549
        real t15502
        real t15503
        real t15504
        real t1551
        real t15513
        real t15514
        real t1552
        real t15533
        real t15535
        real t15539
        real t1554
        real t1555
        real t15555
        real t1556
        real t15572
        real t15573
        real t15575
        real t15579
        real t1558
        real t15580
        real t15582
        real t15588
        real t15589
        real t15593
        real t15594
        real t15596
        real t15597
        real t15599
        real t156
        real t1560
        real t15602
        real t15604
        real t15606
        real t15609
        real t15611
        real t15613
        real t15615
        real t15617
        real t15619
        real t15621
        real t15623
        real t15625
        real t15631
        real t15632
        real t15633
        real t15640
        real t15642
        real t15650
        real t15656
        real t15664
        real t15666
        real t15667
        real t15671
        real t15673
        real t15674
        real t15678
        real t15686
        real t15691
        real t15698
        real t157
        real t1570
        real t15709
        real t15710
        real t15711
        real t15714
        real t15715
        real t15719
        real t15721
        real t15725
        real t15728
        real t15729
        real t15736
        real t15741
        real t15743
        real t15754
        real t15757
        real t15766
        real t15778
        real t15782
        real t15788
        real t15792
        real t15793
        real t1580
        real t15803
        real t15815
        real t15827
        real t15831
        real t15837
        real t15841
        real t15842
        real t15846
        real t15852
        real t15856
        real t15859
        real t15862
        real t15864
        real t15866
        real t15873
        real t15880
        real t15891
        real t15892
        real t15893
        real t15896
        real t15897
        real t159
        real t15901
        real t15903
        real t15907
        real t15910
        real t15911
        real t15919
        real t1592
        real t15923
        real t15939
        real t15950
        real t1596
        real t15967
        real t15972
        real t15974
        real t1598
        real t15983
        real t15989
        real t15993
        real t15996
        real t15999
        real t16
        real t160
        real t16001
        real t16003
        real t16016
        real t1602
        real t1603
        real t16034
        real t16038
        real t1604
        real t16045
        real t16047
        real t16050
        real t16052
        real t16054
        real t16057
        real t16058
        real t16063
        real t16075
        real t16085
        real t16097
        real t161
        real t16102
        real t16106
        real t16107
        real t16121
        integer t16132
        real t16133
        real t16134
        real t16136
        real t16138
        real t1614
        real t16140
        real t16142
        real t16143
        real t16147
        real t16149
        real t16155
        real t16156
        real t16161
        real t16162
        real t16164
        real t16165
        real t16167
        real t1617
        real t16173
        real t16183
        real t16184
        real t16185
        real t16186
        real t16187
        real t16195
        real t16199
        real t162
        real t1621
        real t16225
        real t16228
        real t16231
        real t16236
        real t1624
        real t16242
        real t16251
        real t16253
        real t16254
        real t16256
        real t1626
        real t16262
        real t16276
        real t16282
        real t16288
        real t16289
        real t1629
        real t16298
        real t1630
        real t16301
        real t16312
        real t1632
        real t16324
        real t16338
        real t16342
        real t1635
        real t16359
        real t16384
        real t1639
        real t16394
        real t164
        real t1640
        real t16406
        real t1641
        real t16411
        real t16421
        real t16425
        real t16452
        real t16454
        real t16457
        real t16459
        real t16469
        real t16477
        real t16479
        real t16483
        real t16485
        real t16500
        real t16506
        real t1651
        real t16510
        real t16514
        real t16520
        real t1653
        real t16533
        real t16535
        real t16549
        real t1655
        real t16552
        real t16554
        real t1657
        real t16573
        real t16577
        real t16579
        real t16581
        real t16589
        real t1659
        real t16593
        real t166
        real t16602
        real t16604
        real t16614
        real t16617
        real t16619
        real t1663
        real t1665
        real t16669
        real t1667
        real t16680
        real t16686
        real t1669
        real t16701
        real t16719
        real t16721
        real t16739
        real t16744
        real t16764
        real t1677
        real t1678
        real t1679
        real t16792
        real t168
        real t16801
        real t16803
        real t16806
        real t16807
        real t16808
        real t1681
        real t16811
        real t16814
        real t16816
        real t16817
        real t16818
        real t16821
        real t16822
        real t16823
        real t16825
        real t1683
        real t16830
        real t16831
        real t16832
        real t16834
        real t16837
        real t16838
        real t16841
        real t16849
        real t1685
        real t16851
        real t16856
        real t16858
        real t16861
        real t16873
        real t16882
        real t16885
        real t16886
        real t16887
        real t16889
        real t16891
        real t16893
        real t16895
        real t16896
        real t169
        real t16900
        real t16902
        real t16908
        real t16909
        real t16923
        real t16924
        real t16925
        real t16938
        real t16941
        real t1695
        real t16958
        real t16978
        real t1699
        real t16991
        real t16992
        real t16993
        real t16996
        real t16997
        real t16998
        real t17000
        real t17005
        real t17006
        real t17007
        real t1701
        real t17016
        real t17017
        real t17024
        real t17025
        real t17026
        real t17028
        real t17031
        real t17032
        real t17036
        real t17038
        real t1704
        real t17041
        real t17045
        real t17047
        real t17050
        real t17056
        real t17058
        real t17060
        real t17063
        real t17067
        real t17069
        real t17072
        real t17074
        real t17076
        real t17078
        real t1708
        real t17081
        real t17083
        real t17085
        real t17088
        real t17089
        real t17090
        real t17091
        real t17093
        real t17094
        real t17095
        real t17096
        real t17098
        real t17101
        real t17102
        real t17103
        real t17104
        real t17105
        real t17107
        real t17110
        real t17111
        real t17114
        real t17115
        real t17117
        real t17119
        real t17121
        real t17125
        real t17126
        real t17127
        real t17136
        real t1714
        real t1715
        real t17155
        real t17156
        real t17158
        real t1716
        real t17160
        real t17162
        real t17164
        real t17165
        real t17169
        real t17171
        real t17177
        real t17178
        real t1718
        real t1719
        real t17192
        real t17193
        real t17194
        real t17207
        real t1721
        real t17210
        real t17223
        real t17227
        real t17234
        real t17243
        real t17247
        real t17254
        real t1726
        real t17260
        real t17261
        real t17262
        real t17265
        real t17266
        real t17267
        real t17269
        real t1727
        real t17274
        real t17275
        real t17276
        real t17285
        real t17286
        real t1729
        real t17299
        real t173
        real t17322
        real t17323
        real t17324
        real t17327
        real t17328
        real t17329
        real t17331
        real t17336
        real t17337
        real t17338
        real t17350
        real t17362
        real t17371
        real t17372
        real t17374
        real t17376
        real t17378
        real t1738
        real t17380
        real t17381
        real t17385
        real t17387
        real t17393
        real t17394
        integer t174
        real t17410
        real t17411
        real t17412
        real t17425
        real t1743
        real t17435
        real t17436
        real t17438
        real t17440
        real t17442
        real t17444
        real t17445
        real t17449
        real t17451
        real t17457
        real t17458
        real t17468
        real t17487
        real t17488
        real t17489
        real t17498
        real t17499
        real t175
        real t17502
        real t17503
        real t17504
        real t17507
        real t17508
        real t17509
        real t17511
        real t17516
        real t17517
        real t17518
        real t1753
        real t17530
        real t17542
        real t17551
        real t17552
        real t17554
        real t17556
        real t17558
        real t17560
        real t17561
        real t17565
        real t17567
        real t1757
        real t17573
        real t17574
        real t17590
        real t17591
        real t17592
        real t17605
        real t17615
        real t17616
        real t17618
        real t1762
        real t17620
        real t17622
        real t17624
        real t17625
        real t17629
        real t17631
        real t17637
        real t17638
        real t17648
        real t17650
        real t17654
        real t17661
        real t17667
        real t17668
        real t17669
        real t17678
        real t17679
        real t17682
        real t17688
        real t17698
        real t177
        real t17720
        real t17728
        real t17737
        real t17738
        real t17740
        real t17741
        real t17751
        real t17753
        real t17767
        real t17769
        real t1777
        real t17783
        real t17787
        real t17796
        real t178
        real t17807
        real t1781
        real t17822
        real t17828
        real t1783
        real t17833
        real t17836
        real t1784
        real t17843
        real t17845
        real t17847
        real t17849
        real t17851
        real t17855
        real t17856
        real t17860
        real t17861
        real t17863
        real t17866
        real t17867
        real t17869
        real t17877
        real t17883
        real t1789
        real t17891
        real t17893
        real t17897
        real t17899
        integer t179
        real t17914
        real t17925
        real t1793
        real t17942
        real t17947
        real t17949
        real t17960
        real t1797
        real t17972
        real t17984
        real t17988
        real t1799
        real t17994
        real t17998
        real t17999
        real t180
        real t18009
        real t1801
        real t18021
        real t1803
        real t18033
        real t18037
        real t18043
        real t18047
        real t18048
        real t1805
        real t18052
        real t18058
        real t18062
        real t18065
        real t18068
        real t1807
        real t18070
        real t18072
        real t1808
        real t18085
        real t1809
        real t181
        real t18103
        real t18107
        real t18112
        real t18115
        real t18120
        real t18128
        real t1813
        real t18134
        real t18136
        real t18140
        real t18143
        real t1815
        real t18150
        real t18154
        real t18161
        real t18162
        real t18163
        real t18166
        real t18167
        real t18171
        real t18173
        real t18177
        real t18180
        real t18181
        real t18188
        real t18193
        real t18195
        real t182
        real t1820
        real t18204
        real t18210
        real t18214
        real t18217
        real t18220
        real t18222
        real t18224
        real t18232
        real t18234
        real t18238
        real t18241
        real t18248
        real t18259
        real t18260
        real t18261
        real t18264
        real t18265
        real t18269
        real t18271
        real t18275
        real t18278
        real t18279
        real t18287
        real t18291
        real t18296
        real t1830
        real t18301
        real t18309
        real t18315
        real t18317
        real t18321
        real t18324
        real t1833
        real t18331
        real t18342
        real t18343
        real t18344
        real t18347
        real t18348
        real t18352
        real t18354
        real t18358
        real t18361
        real t18362
        real t18369
        real t1837
        real t18374
        real t18376
        real t18385
        real t18391
        real t18395
        real t18398
        real t184
        real t18401
        real t18403
        real t18405
        real t1841
        real t18413
        real t18415
        real t18419
        real t18422
        real t18429
        real t18440
        real t18441
        real t18442
        real t18445
        real t18446
        real t18450
        real t18452
        real t18456
        real t18459
        real t1846
        real t18460
        real t18468
        real t18472
        real t18477
        real t18478
        real t18479
        real t18480
        real t18481
        real t18482
        real t18484
        real t18485
        real t18486
        real t18487
        real t18489
        real t18492
        real t18493
        real t18494
        real t18495
        real t18496
        real t18498
        real t18501
        real t18502
        real t18510
        real t18516
        real t18519
        real t18523
        real t18525
        real t18528
        integer t18529
        real t18530
        real t18532
        real t18534
        real t18536
        real t18537
        real t18539
        real t18541
        real t18543
        real t18546
        real t18547
        real t18549
        real t18551
        real t18553
        real t18556
        real t18560
        real t18562
        real t18564
        real t18567
        real t18571
        real t18573
        real t18576
        real t18577
        real t18578
        real t18579
        real t18581
        real t18582
        real t18583
        real t18584
        real t18586
        real t18589
        real t18590
        real t18591
        real t18592
        real t18593
        real t18595
        real t18598
        real t18599
        real t186
        real t18602
        real t18604
        real t18606
        real t18608
        real t18610
        real t18613
        real t18614
        real t18616
        real t18618
        real t18620
        real t18623
        real t18624
        real t18625
        real t18627
        real t18629
        real t1863
        real t18631
        real t18633
        real t18634
        real t18638
        real t18640
        real t18646
        real t18647
        real t18649
        real t18653
        real t18655
        real t18657
        real t18661
        real t18668
        real t18670
        real t18676
        real t18678
        real t18679
        real t1868
        real t18680
        real t18681
        real t18682
        real t18685
        real t18687
        real t18688
        real t18689
        real t1869
        real t18691
        real t18692
        real t18693
        real t18695
        real t18696
        real t18697
        real t18698
        real t1870
        real t18700
        real t18702
        real t18703
        real t18704
        real t18705
        real t18707
        real t1871
        real t18715
        real t18717
        real t18724
        real t18727
        real t18729
        real t18744
        real t18754
        real t1876
        real t18766
        real t18773
        real t1878
        real t18789
        real t1880
        real t1881
        real t18817
        real t18829
        real t1883
        real t18839
        real t18851
        real t1886
        real t18860
        real t18875
        real t1888
        real t1889
        real t1890
        real t18905
        real t18916
        real t18917
        real t18919
        real t18929
        real t18932
        real t18934
        real t1897
        real t18970
        real t190
        real t19004
        real t1902
        real t19022
        real t19038
        real t19042
        real t19044
        real t19046
        real t19049
        real t19054
        real t19058
        real t19059
        real t19061
        real t19062
        real t19065
        real t19066
        real t19067
        real t19069
        real t19072
        real t19074
        real t191
        real t19100
        real t19114
        real t1912
        real t19137
        real t19153
        real t1916
        real t19181
        real t1919
        real t19201
        real t1921
        real t19216
        real t19236
        real t1924
        real t1928
        real t193
        real t1930
        real t19308
        real t19319
        real t19321
        real t19324
        real t19332
        real t19346
        real t19348
        real t1935
        real t1936
        real t19362
        real t1937
        real t1938
        real t19380
        real t19393
        real t19395
        real t19398
        real t194
        real t1940
        real t19400
        real t19403
        real t19405
        real t19408
        real t19412
        real t19414
        real t19416
        real t19417
        real t19419
        real t1942
        real t19422
        real t19426
        real t19427
        real t1943
        real t19430
        real t19432
        real t19436
        real t19437
        real t19439
        real t1944
        real t19443
        real t19445
        real t1945
        real t19453
        real t1946
        real t19462
        real t19463
        real t19464
        real t1947
        real t1948
        real t19482
        real t1949
        real t19499
        real t195
        real t1951
        real t19512
        real t19513
        real t19514
        real t19517
        real t19518
        real t19519
        real t19521
        real t19526
        real t19527
        real t19528
        real t19531
        real t19535
        real t19537
        real t1954
        real t19543
        real t19545
        real t19549
        real t1955
        real t19559
        real t19560
        real t19562
        real t19564
        real t19566
        real t19567
        real t19568
        real t19569
        real t19573
        real t19575
        real t19577
        real t1958
        real t19581
        real t19582
        real t196
        real t19611
        real t19612
        real t19613
        real t19618
        real t19622
        real t19623
        real t19630
        real t19631
        real t19632
        real t19634
        real t19636
        real t19640
        real t19641
        real t19642
        real t1966
        real t19660
        real t19673
        real t19677
        real t1968
        real t19684
        real t19690
        real t19691
        real t19692
        real t19695
        real t19696
        real t19697
        real t19699
        real t19704
        real t19705
        real t19706
        real t1971
        real t19715
        real t19719
        real t19723
        real t19727
        real t19731
        real t19737
        real t19738
        real t19740
        real t19742
        real t19744
        real t19746
        real t19747
        real t19751
        real t19753
        real t19759
        real t1976
        real t19760
        real t19776
        real t19789
        real t1979
        real t19790
        real t19791
        real t198
        real t19800
        real t19801
        real t1981
        real t19814
        real t1982
        real t19827
        real t19828
        real t19829
        real t19832
        real t19833
        real t19834
        real t19836
        real t1984
        real t19841
        real t19842
        real t19843
        real t19855
        real t1986
        real t19867
        real t1988
        real t19885
        real t19886
        real t19887
        real t19896
        real t1990
        real t19906
        real t19907
        real t19909
        real t1991
        real t19911
        real t19913
        real t19915
        real t19916
        real t19920
        real t19922
        real t19928
        real t19929
        real t1995
        real t19958
        real t19959
        real t19960
        real t19969
        real t1997
        real t19970
        real t19978
        real t19982
        real t19983
        real t19984
        real t19987
        real t19988
        real t19989
        real t19991
        real t19996
        real t19997
        real t19998
        real t2
        real t20
        real t200
        real t20010
        real t2002
        real t20022
        real t2003
        real t2004
        real t20040
        real t20041
        real t20042
        real t20051
        real t20061
        real t20062
        real t20064
        real t20066
        real t20068
        real t20070
        real t20071
        real t20075
        real t20077
        real t20083
        real t20084
        real t2010
        real t20105
        real t20113
        real t20114
        real t20115
        real t20124
        real t20125
        real t2016
        real t20160
        real t20161
        real t20165
        real t20166
        real t20168
        real t20171
        real t20173
        real t20175
        real t20177
        real t20179
        real t2018
        real t20182
        real t20185
        real t20187
        real t20193
        real t20194
        real t20197
        real t20198
        real t202
        real t20201
        real t20202
        real t20204
        real t20207
        real t20209
        real t20211
        real t20214
        real t20216
        real t20218
        real t2022
        real t20220
        real t20222
        real t20224
        real t20226
        real t20228
        real t2023
        real t20230
        real t20236
        real t20237
        real t20238
        real t20245
        real t20247
        real t2025
        real t20255
        real t20261
        real t20269
        real t2027
        real t20271
        real t20272
        real t20287
        real t2029
        real t20298
        real t203
        real t2031
        real t20315
        real t2032
        real t20320
        real t20322
        real t20331
        real t20337
        real t20341
        real t20344
        real t20347
        real t20349
        real t20351
        real t2036
        real t20364
        real t2038
        real t20382
        real t20386
        real t20402
        real t20413
        real t2043
        real t20430
        real t20433
        real t20435
        real t20437
        real t2044
        real t20440
        real t20446
        real t2045
        real t20452
        real t20456
        real t20459
        real t20462
        real t20464
        real t20465
        real t20466
        real t20471
        real t20479
        real t20497
        real t20501
        real t20506
        real t20507
        real t20508
        real t20509
        real t2051
        real t20511
        real t20512
        real t20513
        real t20514
        real t20516
        real t20519
        real t20520
        real t20521
        real t20522
        real t20523
        real t20525
        real t20528
        real t20529
        real t20537
        real t20543
        real t20546
        real t20550
        real t20552
        real t20555
        integer t20556
        real t20557
        real t20559
        real t20561
        real t20563
        real t20564
        real t20566
        real t20568
        real t20570
        real t20573
        real t20574
        real t20576
        real t20578
        real t20580
        real t20583
        real t20587
        real t20589
        real t2059
        real t20591
        real t20594
        real t20598
        real t2060
        real t20600
        real t20603
        real t20604
        real t20605
        real t20606
        real t20608
        real t20609
        real t2061
        real t20610
        real t20611
        real t20613
        real t20616
        real t20617
        real t20618
        real t20619
        real t20620
        real t20622
        real t20625
        real t20626
        real t20629
        real t2063
        real t20631
        real t20633
        real t20635
        real t20637
        real t2064
        real t20640
        real t20641
        real t20643
        real t20645
        real t20647
        real t2065
        real t20650
        real t20651
        real t20652
        real t20654
        real t20656
        real t20658
        real t2066
        real t20660
        real t20661
        real t20665
        real t20667
        real t20673
        real t20674
        real t2068
        real t20680
        real t20682
        real t20688
        real t20695
        real t20697
        real t207
        real t20703
        real t20705
        real t20706
        real t20707
        real t20708
        real t20709
        real t2071
        real t20712
        real t20715
        real t20716
        real t20719
        real t20721
        real t20723
        real t2073
        real t20731
        real t2074
        real t20740
        real t20742
        real t20747
        real t20749
        real t2075
        real t20751
        real t20753
        real t20757
        real t20759
        real t2077
        real t20770
        real t20783
        real t20785
        real t20791
        real t20795
        real t20797
        real t208
        real t2080
        real t20814
        real t20825
        real t20829
        real t20830
        real t20836
        real t20838
        real t20841
        real t20843
        real t20847
        real t20871
        real t2088
        real t2089
        real t20903
        real t2091
        real t20942
        real t2095
        real t20968
        real t20978
        real t2098
        real t20990
        real t20994
        real t20998
        real t210
        real t21018
        real t2103
        real t21037
        real t21064
        real t21076
        real t21086
        real t21098
        real t211
        real t2111
        real t2113
        real t21131
        real t2114
        real t21143
        real t21145
        real t21155
        real t21179
        real t2118
        real t212
        real t2120
        real t21208
        real t2124
        real t2125
        real t21260
        real t2127
        real t2128
        real t2129
        real t21297
        real t213
        real t2131
        real t21312
        real t2133
        real t21332
        real t2134
        real t21351
        real t21369
        real t2138
        real t21396
        real t21398
        real t2140
        real t21400
        real t21402
        real t2141
        real t21410
        real t21419
        real t21420
        real t21421
        real t21439
        real t2145
        real t21456
        real t2146
        real t21469
        real t2147
        real t21470
        real t21471
        real t21474
        real t21475
        real t21476
        real t21478
        real t21483
        real t21484
        real t21485
        real t21494
        real t215
        real t21502
        real t21506
        real t21516
        real t21517
        real t21519
        real t2152
        real t21521
        real t21523
        real t21525
        real t21526
        real t2153
        real t21530
        real t21532
        real t21538
        real t21539
        real t21568
        real t21569
        real t21570
        real t21579
        real t2158
        real t21580
        real t21587
        real t21589
        real t2159
        real t21591
        real t21593
        real t21597
        real t21598
        real t21599
        real t21617
        real t2163
        real t21630
        real t21634
        real t2164
        real t21641
        real t21647
        real t21648
        real t21649
        real t21652
        real t21653
        real t21654
        real t21656
        real t2166
        real t21661
        real t21662
        real t21663
        real t21672
        real t21676
        real t2168
        real t21680
        real t21684
        real t21688
        real t21694
        real t21695
        real t21697
        real t21699
        real t217
        real t2170
        real t21701
        real t21703
        real t21704
        real t21708
        real t21710
        real t21716
        real t21717
        real t2172
        real t2173
        real t21746
        real t21747
        real t21748
        real t21757
        real t21758
        real t2177
        real t21771
        real t21784
        real t21785
        real t21786
        real t21789
        real t2179
        real t21790
        real t21791
        real t21793
        real t21798
        real t21799
        real t21800
        real t21812
        real t21824
        real t2184
        real t21842
        real t21843
        real t21844
        real t2185
        real t21853
        real t2186
        real t21863
        real t21864
        real t21866
        real t21868
        real t21870
        real t21872
        real t21873
        real t21877
        real t21879
        real t21885
        real t21886
        real t219
        real t21915
        real t21916
        real t21917
        real t2192
        real t21926
        real t21927
        real t21935
        real t21939
        real t21940
        real t21941
        real t21944
        real t21945
        real t21946
        real t21948
        real t21953
        real t21954
        real t21955
        real t21967
        real t21979
        real t21997
        real t21998
        real t21999
        real t22
        real t220
        real t22008
        real t22018
        real t22019
        real t22021
        real t22023
        real t22025
        real t22027
        real t22028
        real t22032
        real t22034
        real t22040
        real t22041
        real t22070
        real t22071
        real t22072
        real t22081
        real t22082
        real t2209
        real t221
        real t22117
        real t22118
        real t22120
        real t22121
        real t22124
        real t22127
        real t22129
        real t2213
        real t22134
        real t22137
        real t22139
        real t22147
        real t22151
        real t22156
        real t22158
        real t22161
        real t22162
        real t22167
        real t22170
        real t22178
        real t22185
        real t22191
        real t22193
        real t22194
        real t22198
        real t222
        real t22204
        real t22205
        real t22221
        real t22222
        real t22236
        real t22246
        real t22247
        real t2226
        real t22263
        real t22264
        real t22273
        real t22288
        real t22289
        real t22305
        real t22306
        real t22310
        real t2232
        real t2233
        real t2234
        real t2236
        real t2237
        real t2238
        real t2239
        real t224
        real t2241
        real t2244
        real t2246
        real t2247
        real t2248
        real t2250
        real t2253
        real t2257
        real t2259
        real t226
        real t2261
        real t2262
        real t2263
        real t2264
        real t2266
        real t2269
        real t2271
        real t2273
        real t2274
        real t2275
        real t2276
        real t2278
        real t228
        real t2280
        real t2281
        real t2282
        real t2284
        real t2286
        real t2287
        real t2289
        real t2291
        real t2293
        real t2294
        real t2295
        real t2296
        real t2298
        real t2299
        real t230
        real t2301
        real t2302
        real t2304
        real t2306
        real t2308
        real t231
        real t2310
        real t2312
        real t2313
        real t2314
        real t2316
        real t2317
        real t2319
        real t2321
        real t2323
        real t2325
        real t2326
        real t2328
        real t2329
        real t2330
        real t2332
        real t2334
        real t2335
        real t2337
        real t2339
        real t2341
        real t2342
        real t2344
        real t2346
        real t2348
        real t235
        real t2350
        real t2352
        real t2354
        real t2355
        real t2357
        real t2359
        real t2361
        real t2363
        real t2365
        real t2366
        real t2367
        real t2368
        real t237
        real t2370
        real t2371
        real t2372
        real t2373
        real t2375
        real t2377
        real t2378
        real t2379
        real t2380
        real t2381
        real t2383
        real t2384
        real t2385
        integer t2386
        real t2387
        real t2388
        real t2390
        real t2392
        real t2394
        real t2396
        real t2397
        real t2401
        real t2403
        real t2408
        real t2409
        real t2410
        real t2411
        real t2412
        real t2413
        real t2414
        real t2417
        real t2418
        real t2419
        real t242
        real t2420
        real t2421
        real t2423
        real t2424
        real t2427
        real t2428
        real t2429
        real t243
        real t2431
        real t2432
        real t2434
        real t2436
        real t2438
        real t244
        real t2440
        real t2441
        real t2444
        real t2445
        real t2446
        real t2448
        real t2449
        real t2451
        real t2453
        real t2455
        real t2457
        real t2458
        real t2459
        real t2460
        real t2462
        real t2464
        real t2466
        real t2468
        real t2469
        real t2473
        real t2475
        real t2480
        real t2481
        real t2482
        real t2484
        real t2488
        real t2490
        real t2492
        real t2494
        real t2496
        real t2498
        real t2499
        real t250
        real t2500
        real t2501
        real t2503
        real t2505
        real t2507
        real t2509
        real t251
        real t2510
        real t2514
        real t2516
        real t252
        real t2520
        real t2521
        real t2522
        real t2523
        real t2529
        real t2531
        real t2533
        real t2535
        real t2536
        real t2537
        real t2538
        real t2539
        real t254
        real t2540
        real t2541
        real t2542
        real t2543
        real t2544
        real t2546
        real t2549
        real t2550
        real t2551
        real t2552
        real t2553
        real t2555
        real t2558
        real t2559
        real t256
        real t2561
        real t2562
        real t2565
        real t2566
        real t2568
        real t2569
        real t2571
        real t2573
        real t2575
        real t258
        real t2581
        real t2583
        real t2584
        real t2588
        real t2589
        real t2591
        real t2592
        real t2594
        real t2596
        real t2598
        real t2599
        real t260
        real t2600
        real t2601
        real t2602
        real t2603
        real t2605
        real t2607
        real t2608
        real t2609
        real t2611
        real t2612
        real t2615
        real t2616
        real t2618
        real t262
        real t2623
        real t2624
        real t2625
        real t263
        real t2631
        real t2633
        real t2635
        real t2637
        real t2639
        real t264
        real t2640
        real t2641
        real t2642
        real t2644
        real t2646
        real t2648
        real t265
        real t2650
        real t2651
        real t2655
        real t2657
        real t2662
        real t2663
        real t2664
        real t267
        real t2670
        real t2672
        real t2674
        real t2676
        real t2677
        real t2681
        real t2683
        real t2685
        real t2687
        real t2689
        real t269
        real t2691
        real t2692
        real t2693
        real t2694
        real t2698
        real t27
        real t2700
        real t2702
        real t2704
        real t2706
        real t2708
        real t2709
        real t271
        real t2710
        real t2711
        real t2712
        real t2714
        real t2715
        real t2716
        real t2717
        real t2719
        real t2720
        real t2722
        real t2723
        real t2724
        real t2725
        real t2726
        real t2728
        real t273
        real t2731
        real t2732
        real t2734
        real t2735
        real t2736
        real t2738
        real t274
        real t2741
        real t2743
        real t2745
        real t2746
        real t2747
        real t2748
        real t2750
        real t2751
        real t2753
        real t2755
        real t2761
        real t2762
        real t2766
        real t2769
        real t2773
        real t2775
        real t2776
        real t2778
        real t278
        real t2782
        real t2784
        real t2788
        real t2790
        real t2792
        real t2793
        real t2794
        real t2795
        real t2797
        real t2798
        real t2799
        real t28
        real t280
        real t2800
        real t2803
        real t2804
        real t2806
        real t2808
        real t2810
        real t2812
        real t2813
        real t2817
        real t2819
        real t2824
        real t2825
        real t2826
        real t2827
        real t2828
        real t2829
        real t2830
        real t2833
        real t2834
        real t2836
        real t2837
        real t2838
        real t2839
        real t2840
        real t2842
        real t2844
        real t2846
        real t2848
        real t2849
        real t285
        real t2853
        real t2855
        real t286
        real t2860
        real t2861
        real t2862
        real t2863
        real t2864
        real t2865
        real t2866
        real t2869
        real t287
        real t2870
        real t2872
        real t2873
        real t2874
        real t2880
        real t2884
        real t2885
        real t2887
        real t2888
        real t2890
        real t2892
        real t2894
        real t2896
        real t2898
        real t29
        real t2900
        real t2902
        real t2903
        real t2907
        real t2908
        real t2910
        real t2911
        real t2913
        real t2915
        real t2917
        real t2919
        real t292
        real t2921
        real t2923
        real t2929
        real t293
        real t2931
        real t2936
        real t2938
        real t2941
        real t2942
        real t2944
        real t2947
        real t2949
        real t295
        real t2951
        real t2954
        real t2956
        real t2957
        real t2959
        real t2962
        real t2963
        real t2965
        real t2967
        real t2968
        real t297
        real t2972
        real t2974
        real t2980
        real t2983
        real t2984
        real t2985
        real t2988
        real t299
        real t2992
        real t2995
        real t2997
        real t2998
        real t2999
        real t30
        real t3002
        real t3004
        real t3007
        real t301
        real t3010
        real t3011
        real t3013
        real t3019
        real t302
        real t3021
        real t3022
        real t3023
        real t3024
        real t3026
        real t3027
        real t3028
        real t3029
        real t303
        real t3031
        real t3032
        real t3033
        real t3035
        real t3037
        real t3039
        real t304
        real t3041
        real t3042
        real t3046
        real t3048
        real t305
        real t3053
        real t3054
        real t3055
        real t3056
        real t3057
        real t3058
        real t3059
        real t3062
        real t3063
        real t3065
        real t3066
        real t3067
        real t3068
        real t3069
        real t307
        real t3071
        real t3073
        real t3075
        real t3077
        real t3078
        real t308
        real t3081
        real t3082
        real t3084
        real t3087
        real t3089
        real t309
        real t3090
        real t3091
        real t3092
        real t3093
        real t3094
        real t3095
        real t3098
        real t3099
        real t31
        real t310
        real t3101
        real t3102
        real t3103
        real t3109
        real t3110
        real t3114
        real t3115
        real t3117
        real t3119
        real t312
        real t3120
        real t3121
        real t3123
        real t3125
        real t3127
        real t3129
        real t3130
        real t3134
        real t3135
        real t3137
        real t3139
        real t3141
        real t3142
        real t3143
        real t3145
        real t3147
        real t315
        real t3153
        real t3154
        real t3155
        real t3156
        real t3159
        real t316
        real t3160
        real t3166
        real t3168
        real t317
        real t3171
        real t3172
        real t3174
        real t3177
        real t318
        real t3181
        real t3183
        real t3189
        real t319
        real t3192
        real t3194
        real t3197
        real t32
        real t3201
        real t3203
        real t321
        real t3211
        real t3213
        real t3215
        real t3217
        real t3219
        real t3221
        real t3223
        real t3225
        real t3229
        real t3231
        real t3233
        real t3235
        real t3237
        real t3239
        real t324
        real t3241
        real t3246
        real t3247
        real t325
        real t3251
        real t3257
        real t3258
        real t3259
        real t3261
        real t3269
        real t327
        real t3270
        real t3275
        real t3279
        real t3282
        real t3284
        real t3287
        real t3291
        real t3293
        real t3298
        real t33
        real t3306
        real t3307
        real t331
        real t3313
        real t3315
        real t3317
        real t3319
        real t332
        real t3321
        real t3323
        real t3325
        real t3327
        real t3329
        real t333
        real t3333
        real t3335
        real t3336
        real t3337
        real t3339
        real t334
        real t3341
        real t3343
        real t3344
        real t3345
        real t3347
        real t335
        real t3352
        real t3353
        real t3357
        real t3363
        real t3364
        real t3365
        real t3367
        real t337
        real t3375
        real t3376
        real t3379
        real t3380
        real t3381
        real t3383
        real t3384
        real t3386
        real t339
        real t3390
        real t3392
        real t3394
        real t3396
        real t3399
        real t34
        real t3401
        real t3403
        real t3406
        real t3407
        real t3408
        real t341
        real t3411
        real t3413
        real t3414
        real t3416
        real t3418
        real t342
        real t3420
        real t3422
        real t3425
        real t3426
        real t3428
        real t3429
        real t3431
        real t3433
        real t3435
        real t3438
        real t3440
        real t3442
        real t3444
        real t3447
        real t3449
        real t3451
        real t3454
        real t3456
        real t3458
        real t3460
        real t3462
        real t3464
        real t3467
        real t3469
        real t347
        real t3471
        real t3473
        real t3475
        real t3478
        real t3479
        real t3480
        real t3483
        real t3484
        real t3486
        real t3488
        real t3489
        real t349
        real t3492
        real t3493
        real t3495
        real t3497
        real t3499
        real t35
        real t350
        real t3501
        real t3502
        real t3506
        real t3508
        real t3514
        real t3515
        real t3516
        real t3517
        real t3520
        real t3521
        real t3522
        real t3524
        real t3529
        real t3530
        real t3531
        real t3533
        real t3536
        real t3537
        real t354
        real t3540
        real t3545
        real t3548
        real t355
        real t3553
        real t3555
        real t356
        real t3560
        real t3562
        real t3564
        real t3565
        real t357
        real t3570
        real t3573
        real t358
        real t3582
        real t3583
        real t3585
        real t3589
        real t3592
        real t3594
        real t3596
        real t3597
        real t3598
        real t3599
        real t360
        real t3600
        real t3601
        real t3603
        real t3605
        real t3607
        real t3608
        real t3610
        real t3612
        real t3614
        real t362
        real t3620
        real t3621
        real t3635
        real t3636
        real t3637
        real t364
        real t3650
        real t3653
        real t3657
        real t366
        real t3663
        real t3664
        real t3665
        real t3666
        real t3668
        real t367
        real t3670
        real t3672
        real t3673
        real t3674
        real t3677
        real t3679
        real t368
        real t3685
        real t3686
        real t369
        real t3694
        real t3696
        real t37
        real t3700
        real t3704
        real t3705
        real t3707
        real t3709
        real t371
        real t3711
        real t3713
        real t3714
        real t3718
        real t3720
        real t3726
        real t3727
        real t373
        real t3735
        real t3737
        real t375
        real t3750
        real t3754
        real t3765
        real t377
        real t3771
        real t3772
        real t3773
        real t3776
        real t3777
        real t3778
        real t378
        real t3780
        real t3785
        real t3786
        real t3787
        real t3796
        real t3797
        real t3800
        real t3801
        real t3803
        real t3805
        real t3807
        real t3809
        real t3810
        real t3814
        real t3816
        real t382
        real t3822
        real t3823
        real t3824
        real t3825
        real t3828
        real t3829
        real t3830
        real t3832
        real t3837
        real t3838
        real t3839
        real t384
        real t3841
        real t3844
        real t3845
        real t3848
        real t3849
        real t3853
        real t3861
        real t3863
        real t3868
        real t3870
        real t3872
        real t3873
        real t3878
        real t3881
        real t3884
        real t3889
        real t389
        real t3891
        real t3893
        real t3896
        real t39
        real t390
        real t3900
        real t3902
        real t3903
        real t3904
        real t3905
        real t3906
        real t3907
        real t3909
        real t391
        real t3911
        real t3913
        real t3915
        real t3916
        real t3920
        real t3922
        real t3928
        real t3929
        real t3943
        real t3944
        real t3945
        real t3948
        real t3958
        real t396
        real t3961
        real t3965
        real t397
        real t3970
        real t3971
        real t3972
        real t3974
        real t3976
        real t3978
        real t3980
        real t3981
        real t3985
        real t3987
        real t399
        real t3993
        real t3994
        real t4
        real t4002
        real t4003
        real t4004
        real t4008
        real t401
        real t4012
        real t4013
        real t4014
        real t4015
        real t4017
        real t4019
        real t4021
        real t4022
        real t4026
        real t4027
        real t4028
        real t403
        real t4034
        real t4035
        real t4043
        real t4045
        real t405
        real t4056
        real t4058
        real t4062
        real t407
        real t4073
        real t4079
        real t408
        real t4080
        real t4081
        real t4082
        real t4084
        real t4085
        real t4086
        real t4088
        real t409
        real t4093
        real t4094
        real t4095
        real t41
        real t410
        real t4104
        real t4105
        real t4111
        real t4112
        real t4113
        real t4114
        real t4116
        real t4119
        real t412
        real t4120
        real t4122
        real t4124
        real t4126
        real t4127
        real t4128
        real t4129
        real t4135
        real t4137
        real t4138
        real t4139
        real t414
        real t4140
        real t4141
        real t4142
        real t4144
        real t4145
        real t4146
        real t4148
        real t4150
        real t4151
        real t4155
        real t4157
        real t416
        real t4162
        real t4163
        real t4164
        real t4170
        real t4172
        real t4174
        real t4176
        real t4179
        real t418
        real t4180
        real t4181
        real t4183
        real t4185
        real t4187
        real t4188
        real t4189
        real t419
        real t4190
        real t4194
        real t4196
        real t4201
        real t4202
        real t4203
        real t4207
        real t4209
        real t4211
        real t4213
        real t4216
        real t4219
        real t4222
        real t4224
        real t4226
        real t4229
        real t423
        real t4233
        real t4235
        real t4237
        real t4240
        real t4241
        real t4242
        real t4243
        real t4245
        real t4246
        real t4247
        real t4248
        real t425
        real t4250
        real t4253
        real t4254
        real t4255
        real t4256
        real t4257
        real t4259
        real t4261
        real t4262
        real t4263
        real t4266
        real t4267
        real t4269
        real t4270
        real t4271
        real t4272
        real t4274
        real t4276
        real t4277
        real t4278
        real t4280
        real t4282
        real t4284
        real t4286
        real t4287
        real t4289
        real t4293
        real t4295
        real t4296
        real t4297
        real t4298
        real t4299
        real t43
        real t430
        real t4300
        real t4302
        real t4304
        real t4306
        real t4308
        real t4309
        real t431
        real t4313
        real t4315
        real t432
        real t4320
        real t4321
        real t4322
        real t4327
        real t4328
        real t4330
        real t4332
        real t4334
        real t4337
        real t4338
        real t4339
        real t4341
        real t4343
        real t4345
        real t4347
        real t4348
        real t435
        real t4352
        real t4354
        real t4359
        real t4360
        real t4361
        real t4363
        real t4365
        real t4367
        real t4369
        real t4371
        real t4374
        real t438
        real t4380
        real t4382
        real t4384
        real t4387
        real t4391
        real t4393
        real t4394
        real t4395
        real t4398
        real t4399
        real t44
        real t440
        real t4400
        real t4401
        real t4403
        real t4404
        real t4405
        real t4406
        real t4408
        real t4410
        real t4411
        real t4412
        real t4413
        real t4414
        real t4415
        real t4417
        real t442
        real t4420
        real t4421
        real t4424
        real t4425
        real t4426
        real t4427
        real t4429
        real t4431
        real t4435
        real t4436
        real t4437
        real t4439
        real t444
        real t4442
        real t4443
        real t4445
        real t4447
        real t4449
        real t4451
        real t4452
        real t4456
        real t4458
        real t446
        real t4460
        real t4461
        real t4462
        real t4463
        real t4464
        real t4465
        real t4467
        real t4469
        real t447
        real t4471
        real t4473
        real t4474
        real t4478
        real t4480
        real t4485
        real t4486
        real t4487
        real t4491
        real t4492
        real t4493
        real t4495
        real t4497
        real t4499
        real t450
        real t4500
        real t4501
        real t4502
        real t4503
        real t4504
        real t4506
        real t4508
        real t451
        real t4510
        real t4512
        real t4513
        real t4517
        real t4519
        real t4524
        real t4525
        real t4526
        real t453
        real t4530
        real t4532
        real t4534
        real t4536
        real t4538
        real t4539
        real t4545
        real t4547
        real t4549
        real t455
        real t4551
        real t4552
        real t4558
        real t4560
        real t4562
        real t4563
        real t4564
        real t4565
        real t4566
        real t4568
        real t4569
        real t457
        real t4570
        real t4571
        real t4573
        real t4576
        real t4577
        real t4578
        real t4579
        real t4580
        real t4582
        real t4585
        real t4586
        real t4588
        real t4589
        real t459
        real t4590
        real t4591
        real t4592
        real t4593
        real t4594
        real t4595
        real t4597
        real t4600
        real t4601
        real t4603
        real t4605
        real t4607
        real t4609
        real t461
        real t4610
        real t4614
        real t4616
        real t4618
        real t4619
        real t4620
        real t4621
        real t4622
        real t4623
        real t4625
        real t4627
        real t4629
        real t463
        real t4631
        real t4632
        real t4636
        real t4638
        real t464
        real t4643
        real t4644
        real t4645
        real t4649
        real t4651
        real t4653
        real t4655
        real t4657
        real t4659
        real t466
        real t4660
        real t4661
        real t4662
        real t4664
        real t4666
        real t4668
        real t4670
        real t4671
        real t4675
        real t4677
        real t468
        real t4682
        real t4683
        real t4684
        real t4688
        real t4690
        real t4692
        real t4694
        real t4696
        real t4697
        real t470
        real t4703
        real t4705
        real t4707
        real t4709
        real t4710
        real t4716
        real t4718
        real t472
        real t4720
        real t4721
        real t4722
        real t4723
        real t4724
        real t4726
        real t4727
        real t4728
        real t4729
        real t4731
        real t4732
        real t4734
        real t4735
        real t4736
        real t4737
        real t4738
        real t474
        real t4740
        real t4743
        real t4744
        real t4746
        real t4747
        real t4748
        real t4749
        real t4750
        real t4752
        real t4754
        real t4757
        real t4758
        real t4759
        real t476
        real t4761
        real t4763
        real t4765
        real t4767
        real t4768
        real t4772
        real t4774
        real t478
        real t4780
        real t4781
        real t4782
        real t4783
        real t4786
        real t4787
        real t4788
        real t479
        real t4790
        real t4795
        real t4796
        real t4797
        real t4799
        real t48
        real t480
        real t4802
        real t4803
        real t4806
        real t481
        real t4818
        real t482
        real t4822
        real t4823
        real t4824
        real t4831
        real t4833
        real t4835
        real t4836
        real t484
        real t4841
        real t4849
        real t485
        real t4851
        real t4856
        real t4858
        real t486
        real t4860
        real t4861
        real t4865
        real t4869
        real t487
        real t4876
        real t4882
        real t4883
        real t4884
        real t4887
        real t4888
        real t4889
        real t489
        real t4891
        real t4896
        real t4897
        real t4898
        real t4907
        real t4911
        real t4915
        real t4919
        real t492
        real t4923
        real t4929
        real t493
        real t4930
        real t4932
        real t4934
        real t4936
        real t4938
        real t4939
        real t494
        real t4943
        real t4945
        real t495
        real t4951
        real t4952
        real t496
        real t4972
        real t4975
        real t498
        real t4981
        real t4982
        real t4983
        real t4992
        real t4993
        real t4996
        real t4997
        real t4999
        integer t5
        real t50
        real t5001
        real t5003
        real t5005
        real t5006
        real t501
        real t5010
        real t5012
        real t5018
        real t5019
        real t502
        real t5020
        real t5021
        real t5024
        real t5025
        real t5026
        real t5027
        real t5028
        real t5033
        real t5034
        real t5035
        real t5037
        real t5039
        real t504
        real t5040
        real t5041
        real t5044
        real t5048
        real t505
        real t506
        real t5060
        real t5062
        real t5069
        integer t507
        real t5071
        real t5073
        real t5074
        real t5079
        real t508
        real t5081
        real t5087
        real t5089
        real t509
        real t5090
        real t5094
        real t5096
        real t5098
        real t5099
        real t5100
        real t5103
        real t5107
        real t511
        real t5114
        real t5120
        real t5121
        real t5122
        real t5125
        real t5126
        real t5127
        real t5129
        real t513
        real t5134
        real t5135
        real t5136
        real t5140
        real t5145
        real t5148
        real t5149
        real t515
        real t5153
        real t5155
        real t5157
        real t5161
        real t5167
        real t5168
        real t517
        real t5170
        real t5172
        real t5174
        real t5176
        real t5177
        real t518
        real t5181
        real t5183
        real t5187
        real t5189
        real t5190
        real t5197
        real t5202
        real t5213
        real t5219
        real t522
        real t5220
        real t5221
        real t5230
        real t5231
        real t5238
        real t5239
        real t524
        real t5240
        real t5242
        real t5245
        real t5246
        real t5248
        real t5254
        real t5256
        real t5257
        real t5259
        real t5261
        real t5263
        real t5264
        real t5270
        real t5272
        real t5275
        real t5281
        real t5284
        real t5285
        real t5286
        real t5287
        real t5289
        real t529
        real t5290
        real t5291
        real t5292
        real t5294
        real t5297
        real t5298
        real t5299
        real t530
        real t5300
        real t5301
        real t5303
        real t5306
        real t5307
        real t531
        real t5311
        real t5313
        real t5315
        real t5318
        real t532
        real t5320
        real t5322
        real t5325
        real t5326
        real t5327
        real t5328
        real t5329
        real t533
        real t5331
        real t5332
        real t5333
        real t5334
        real t5336
        real t5339
        real t534
        real t5340
        real t5342
        real t5348
        real t535
        real t5350
        real t5351
        real t5353
        real t5355
        real t5357
        real t5358
        real t5364
        real t5366
        real t5369
        real t5375
        real t5378
        real t5379
        real t538
        real t5380
        real t5381
        real t5383
        real t5384
        real t5385
        real t5386
        real t5388
        real t539
        real t5391
        real t5392
        real t5393
        real t5394
        real t5395
        real t5397
        real t5400
        real t5401
        real t5405
        real t5407
        real t5409
        real t541
        real t5412
        real t5414
        real t5416
        real t5419
        real t542
        real t5420
        real t5421
        real t5422
        real t5423
        real t5425
        real t5427
        real t5429
        real t5433
        real t5434
        real t5435
        real t5437
        real t544
        real t5440
        real t5441
        real t5443
        real t5447
        real t5449
        real t545
        real t5451
        real t5452
        real t5454
        real t5456
        real t5458
        real t5459
        real t5463
        real t5465
        real t5467
        real t5469
        real t547
        real t5470
        real t5474
        real t5476
        real t5478
        real t5479
        real t5480
        real t5481
        real t5482
        real t5484
        real t5485
        real t5486
        real t5487
        real t5489
        real t549
        real t5492
        real t5493
        real t5494
        real t5495
        real t5496
        real t5498
        real t55
        real t550
        real t5501
        real t5502
        real t5504
        real t5506
        real t5508
        real t5510
        real t5512
        real t5513
        real t5515
        real t5517
        real t5519
        real t552
        real t5520
        real t5521
        real t5522
        real t5523
        real t5524
        real t5525
        real t5526
        real t5527
        real t5528
        real t5529
        real t553
        real t5531
        real t5534
        real t5535
        real t5537
        real t5541
        real t5543
        real t5545
        real t5546
        real t5548
        real t555
        real t5550
        real t5552
        real t5553
        real t5557
        real t5559
        real t5561
        real t5563
        real t5564
        real t5568
        real t557
        real t5570
        real t5572
        real t5573
        real t5574
        real t5575
        real t5576
        real t5578
        real t5579
        real t5580
        real t5581
        real t5583
        real t5586
        real t5587
        real t5588
        real t5589
        real t559
        real t5590
        real t5592
        real t5595
        real t5596
        real t5598
        real t56
        real t5600
        real t5602
        real t5604
        real t5606
        real t5607
        real t5609
        real t561
        real t5611
        real t5613
        real t5614
        real t5615
        real t5616
        real t5617
        real t5618
        real t5619
        real t562
        real t5620
        real t5622
        real t5624
        real t5627
        real t5631
        real t5637
        real t5639
        real t564
        real t5646
        real t5658
        real t5659
        real t566
        real t5660
        real t5663
        real t5664
        real t5665
        real t5667
        real t567
        real t5672
        real t5673
        real t5674
        real t5676
        real t5679
        real t5680
        real t5686
        real t569
        real t5691
        real t5694
        real t5698
        real t57
        real t570
        real t5703
        real t5706
        real t5707
        real t5708
        real t5710
        real t5712
        real t5714
        real t5716
        real t5717
        real t572
        real t5721
        real t5723
        real t5729
        real t5730
        real t5734
        real t5738
        real t574
        real t5740
        real t5746
        real t5747
        real t5748
        real t576
        real t5760
        real t5761
        real t5765
        real t5771
        real t5772
        real t5774
        real t5776
        real t5778
        real t578
        real t5780
        real t5781
        real t5785
        real t5787
        real t579
        real t5793
        real t5794
        real t5798
        real t58
        real t580
        real t5802
        real t5804
        real t581
        real t5813
        real t5817
        real t5823
        real t5824
        real t5825
        real t583
        real t5834
        real t5835
        real t5838
        real t5839
        real t5840
        real t5843
        real t5844
        real t5845
        real t5847
        real t585
        real t5852
        real t5853
        real t5854
        real t5856
        real t5859
        real t5860
        real t5866
        real t587
        real t5871
        real t5874
        real t5878
        real t5883
        real t5886
        real t5887
        real t5888
        real t589
        real t5890
        real t5892
        real t5894
        real t5896
        real t5897
        real t59
        real t590
        real t5901
        real t5903
        real t5909
        real t5910
        real t5914
        real t5918
        real t5920
        real t5926
        real t5927
        real t5928
        real t594
        real t5940
        real t5941
        real t5945
        real t5951
        real t5952
        real t5954
        real t5956
        real t5958
        real t596
        real t5960
        real t5961
        real t5965
        real t5967
        real t5973
        real t5974
        real t5978
        real t5982
        real t5984
        real t5993
        real t5997
        real t6
        real t60
        real t6003
        real t6004
        real t6005
        real t601
        real t6014
        real t6015
        real t6019
        real t602
        real t6023
        real t6027
        real t6028
        real t6029
        real t603
        real t6032
        real t6033
        real t6034
        real t6036
        real t604
        real t6041
        real t6042
        real t6043
        real t6045
        real t6048
        real t6049
        real t6055
        real t6060
        real t6063
        real t6067
        real t607
        real t6072
        real t6075
        real t6076
        real t6077
        real t6079
        real t6081
        real t6083
        real t6085
        real t6086
        real t609
        real t6090
        real t6092
        real t6098
        real t6099
        real t61
        real t6103
        real t6107
        real t6109
        real t611
        real t6115
        real t6116
        real t6117
        real t6129
        real t613
        real t6130
        real t6134
        real t6140
        real t6141
        real t6143
        real t6145
        real t6147
        real t6149
        real t615
        real t6150
        real t6154
        real t6156
        real t6162
        real t6163
        real t6167
        real t617
        real t6171
        real t6173
        real t6182
        real t6183
        real t6186
        real t619
        real t6190
        real t6192
        real t6193
        real t6194
        real t620
        real t6200
        real t6203
        real t6204
        real t6207
        real t6208
        real t6209
        real t621
        real t6212
        real t6213
        real t6214
        real t6216
        real t622
        real t6220
        real t6221
        real t6222
        real t6223
        real t6225
        real t6228
        real t6229
        real t6233
        real t6235
        real t624
        real t6240
        real t6243
        real t6247
        real t6252
        real t6255
        real t6256
        real t6257
        real t6259
        real t626
        real t6261
        real t6263
        real t6265
        real t6266
        real t6267
        real t6270
        real t6272
        real t6276
        real t6278
        real t6279
        real t628
        real t6283
        real t6287
        real t6288
        real t6289
        real t6295
        real t6296
        real t6297
        real t630
        real t6309
        real t631
        real t6310
        real t6311
        real t6314
        real t6319
        real t6320
        real t6321
        real t6323
        real t6325
        real t6327
        real t6329
        real t6330
        real t6334
        real t6335
        real t6336
        real t6342
        real t6343
        real t6347
        real t635
        real t6351
        real t6353
        real t6362
        real t6366
        real t637
        real t6372
        real t6373
        real t6374
        real t6383
        real t6384
        real t6388
        real t6397
        real t64
        real t6403
        real t641
        real t6410
        real t642
        real t6423
        real t6427
        real t643
        real t6436
        real t644
        real t6445
        real t6446
        real t6447
        real t6450
        real t6453
        real t6454
        real t6456
        real t6459
        real t6461
        real t6463
        real t6466
        real t6467
        real t6469
        real t6472
        real t6476
        real t6478
        real t648
        real t6481
        real t6482
        real t6484
        real t6487
        real t6491
        real t6493
        real t6497
        real t6499
        real t65
        real t650
        real t6501
        real t6502
        real t6503
        real t6504
        real t6506
        real t6507
        real t6508
        real t6509
        real t6512
        real t6513
        real t6515
        real t6516
        real t6517
        real t6519
        real t652
        real t6525
        real t6526
        real t6528
        real t6531
        real t6535
        real t6538
        real t654
        real t6540
        real t6543
        real t6544
        real t6546
        real t6549
        real t6553
        real t6555
        real t656
        real t6560
        real t6561
        real t6563
        real t6564
        real t6566
        real t6568
        real t657
        real t6570
        real t6572
        real t6574
        real t6576
        real t6578
        real t6579
        real t658
        real t6580
        real t6582
        real t6583
        real t6585
        real t6587
        real t6589
        real t659
        real t6591
        real t6593
        real t6595
        real t66
        real t660
        real t6600
        real t6602
        real t6605
        real t6606
        real t6608
        real t661
        real t6611
        real t6615
        real t6616
        real t6618
        real t662
        real t6621
        real t6622
        real t6624
        real t6627
        real t663
        real t6631
        real t6633
        real t6634
        real t6636
        real t6639
        real t664
        real t6640
        real t6642
        real t6645
        real t6649
        real t665
        real t6651
        real t6657
        real t6659
        real t666
        real t6661
        real t6663
        real t6665
        real t6667
        real t6669
        real t667
        real t6671
        real t6673
        real t6675
        real t6680
        real t6682
        real t6685
        real t6686
        real t6688
        real t6691
        real t6695
        real t6697
        real t67
        real t670
        real t6700
        real t6702
        real t6705
        real t6709
        real t671
        real t6711
        real t6712
        real t6714
        real t6717
        real t6718
        real t672
        real t6720
        real t6723
        real t6727
        real t6729
        real t673
        real t6732
        real t6734
        real t6735
        real t6738
        real t674
        real t6740
        real t6741
        real t6743
        real t6745
        real t6747
        real t6749
        real t675
        real t6751
        real t6753
        real t6755
        real t6756
        real t6758
        real t6759
        real t676
        real t6761
        real t6763
        real t6765
        real t6767
        real t6769
        real t6771
        real t6776
        real t6777
        real t6781
        real t6783
        real t6784
        real t6785
        real t6786
        real t6788
        real t6789
        real t679
        real t6790
        real t6791
        real t6794
        real t6796
        real t6797
        real t6798
        real t6799
        real t68
        real t680
        real t6801
        real t6802
        real t6803
        real t6809
        real t6811
        real t6813
        real t6815
        real t6817
        real t6819
        real t682
        real t6821
        real t6823
        real t6825
        real t6827
        real t6829
        real t683
        real t6831
        real t6833
        real t6835
        real t6836
        real t6837
        real t6839
        real t6843
        real t6844
        real t6845
        real t6849
        real t6851
        real t6852
        real t6853
        real t6854
        real t6856
        real t6857
        real t6858
        real t6859
        real t686
        real t6862
        real t6864
        real t6865
        real t6866
        real t6867
        real t6869
        real t687
        real t6870
        real t6871
        real t6878
        real t6881
        real t6883
        real t6886
        real t689
        real t6890
        real t6893
        real t6895
        real t6897
        real t69
        real t690
        real t6900
        real t6902
        real t6905
        real t6909
        real t691
        real t6911
        real t6917
        real t6919
        real t692
        real t6921
        real t6923
        real t6925
        real t6927
        real t6929
        real t6931
        real t6933
        real t6935
        real t6937
        real t6939
        real t694
        real t6941
        real t6943
        real t6945
        real t6951
        real t6954
        real t6956
        real t6959
        real t696
        real t6963
        real t6966
        real t6968
        real t6970
        real t6973
        real t6975
        real t6978
        real t6982
        real t6984
        real t6990
        real t6992
        real t6994
        real t6996
        real t6998
        real t7
        real t700
        real t7000
        real t7002
        real t7004
        real t7006
        real t7008
        real t7013
        real t7014
        real t7015
        real t7017
        real t7018
        real t702
        real t7023
        real t7026
        real t7028
        real t7029
        real t7038
        real t704
        real t7041
        real t7045
        real t7049
        real t705
        real t7053
        real t7056
        real t7059
        real t706
        real t7063
        real t7067
        real t7079
        real t7081
        real t7086
        real t709
        real t7090
        real t7092
        real t7096
        real t7097
        real t71
        real t710
        real t7105
        real t7108
        real t7112
        real t7116
        real t712
        real t7120
        real t7123
        real t7126
        real t713
        real t7130
        real t7134
        real t7148
        real t715
        real t7153
        real t7157
        real t7163
        real t7167
        real t717
        real t7172
        real t7176
        real t7182
        real t7189
        real t719
        real t7191
        real t7192
        real t7196
        real t72
        real t7202
        real t7207
        real t721
        real t7214
        real t722
        real t7226
        real t723
        real t7236
        real t724
        real t7247
        real t726
        real t7266
        real t7267
        real t7269
        real t7270
        real t7275
        real t7279
        real t728
        real t7280
        real t7282
        real t7284
        real t7290
        real t7291
        integer t73
        real t730
        real t7303
        real t7305
        real t7311
        real t7315
        real t7318
        real t732
        real t7320
        real t7326
        real t733
        real t7332
        real t7339
        real t7341
        real t7344
        real t7359
        real t7362
        real t7364
        real t7368
        real t737
        real t7374
        real t7377
        real t7378
        real t7383
        real t7387
        real t739
        real t7399
        real t74
        real t7406
        real t7420
        real t7423
        real t7428
        real t743
        real t7430
        real t7438
        real t744
        real t7442
        real t7444
        real t7448
        real t7449
        real t745
        real t7451
        real t7453
        real t7454
        real t7455
        real t7456
        real t7457
        real t746
        real t7460
        real t7461
        real t7463
        real t7465
        real t7467
        real t7469
        real t7470
        real t7474
        real t7476
        real t7481
        real t7482
        real t7483
        real t7484
        real t7485
        real t7487
        real t7490
        real t7491
        real t7493
        real t7494
        real t7499
        real t75
        real t750
        real t7501
        real t7503
        real t7505
        real t7507
        real t7508
        real t7512
        real t7513
        real t7514
        real t7515
        real t7516
        real t7518
        real t752
        real t7520
        real t7522
        real t7524
        real t7525
        real t7526
        real t7527
        real t7529
        real t7531
        real t7532
        real t7533
        real t7535
        real t7536
        real t754
        real t7540
        real t7541
        real t7542
        real t7547
        real t7548
        real t7549
        real t7551
        real t7555
        real t7557
        real t7559
        real t756
        real t7561
        real t7562
        real t7563
        real t7564
        real t7565
        real t7567
        real t7570
        real t7571
        real t7573
        real t7577
        real t7578
        real t758
        real t7580
        real t7581
        real t7583
        real t7585
        real t7587
        real t7589
        real t7590
        real t7591
        real t7592
        real t7594
        real t7596
        real t7598
        real t760
        real t7600
        real t7601
        real t7605
        real t7607
        real t761
        real t7612
        real t7613
        real t7614
        real t7618
        real t762
        real t7620
        real t7622
        real t7623
        real t7624
        real t7626
        real t7629
        real t763
        real t7630
        real t7631
        real t7633
        real t7635
        real t7637
        real t7639
        real t7640
        real t7641
        real t7644
        real t7646
        real t765
        real t7651
        real t7652
        real t7653
        real t7657
        real t7659
        real t7661
        real t7663
        real t7666
        real t767
        real t7670
        real t7672
        real t7674
        real t7676
        real t7678
        real t7681
        real t7685
        real t7687
        real t7689
        real t769
        real t7691
        real t7692
        real t7694
        real t7695
        real t7696
        real t7697
        real t7699
        real t77
        real t7700
        real t7701
        real t7702
        real t7704
        real t7707
        real t7708
        real t7709
        real t771
        real t7710
        real t7711
        real t7713
        real t7714
        real t7716
        real t7717
        real t772
        real t7720
        real t7721
        real t7723
        real t7724
        real t7725
        real t7726
        real t7727
        real t7729
        real t7731
        real t7733
        real t7734
        real t7737
        real t7738
        real t7740
        real t7745
        real t7746
        real t7747
        real t7748
        real t7749
        real t7751
        real t7754
        real t7755
        real t7757
        real t7758
        real t776
        real t7763
        real t7765
        real t7767
        real t7769
        real t7771
        real t7772
        real t7776
        real t7777
        real t7779
        real t778
        real t7780
        real t7782
        real t7784
        real t7786
        real t7788
        real t7789
        real t7790
        real t7791
        real t7793
        real t7795
        real t7797
        real t7799
        real t7800
        real t7804
        real t7806
        real t7809
        real t781
        real t7811
        real t7812
        real t7813
        real t7819
        real t7821
        real t7823
        real t7824
        real t7825
        real t7826
        real t7827
        real t7828
        real t7829
        real t783
        real t7831
        real t7834
        real t7835
        real t7837
        real t784
        real t7841
        real t7842
        real t7844
        real t7845
        real t7847
        real t7849
        real t785
        real t7851
        real t7853
        real t7854
        real t7855
        real t7856
        real t7858
        real t7860
        real t7862
        real t7864
        real t7865
        real t7869
        real t7871
        real t7876
        real t7877
        real t7878
        real t7882
        real t7884
        real t7886
        real t7888
        real t789
        real t7890
        real t7893
        real t7894
        real t7895
        real t7897
        real t7899
        real t79
        real t7901
        real t7903
        real t7904
        real t7908
        real t791
        real t7910
        real t7914
        real t7915
        real t7916
        real t7917
        real t7921
        real t7923
        real t7925
        real t7927
        real t793
        real t7930
        real t7933
        real t7934
        real t7936
        real t7938
        real t7940
        real t7942
        real t7945
        real t7949
        real t795
        real t7951
        real t7953
        real t7955
        real t7958
        real t7959
        real t796
        real t7960
        real t7961
        real t7963
        real t7964
        real t7965
        real t7966
        real t7968
        real t797
        real t7971
        real t7972
        real t7973
        real t7974
        real t7975
        real t7976
        real t7977
        real t798
        real t7980
        real t7981
        real t7984
        real t7985
        real t7987
        real t7989
        real t7990
        real t7991
        real t7994
        real t7995
        real t7996
        real t7998
        real t8000
        real t8002
        real t8004
        real t8005
        real t8009
        real t8011
        real t8016
        real t8017
        real t8018
        real t8019
        real t802
        real t8020
        real t8022
        real t8025
        real t8026
        real t8028
        real t8029
        real t8033
        real t8035
        real t8037
        real t8039
        real t804
        real t8041
        real t8043
        real t8044
        real t8049
        real t8051
        real t8053
        real t8055
        real t8056
        real t8057
        real t8058
        real t806
        real t8062
        real t8064
        real t8066
        real t8069
        real t8073
        real t8074
        real t8075
        real t8078
        real t8079
        real t808
        real t8080
        real t8081
        real t8083
        real t8084
        real t8085
        real t8086
        real t8088
        real t8091
        real t8092
        real t8093
        real t8094
        real t8095
        real t8097
        real t81
        real t810
        real t8100
        real t8101
        real t8104
        real t8106
        real t8108
        real t811
        real t8110
        real t8112
        real t8115
        real t8116
        real t8118
        real t8119
        real t812
        real t8120
        real t8122
        real t8125
        real t8126
        real t8127
        real t8129
        real t8131
        real t8133
        real t8134
        real t8135
        real t8136
        real t814
        real t8140
        real t8142
        real t8147
        real t8148
        real t8149
        real t815
        real t8155
        real t8157
        real t8159
        real t8161
        real t8162
        real t8166
        real t8168
        real t8170
        real t8172
        real t8174
        real t8176
        real t8177
        real t8178
        real t8179
        real t8180
        real t8182
        real t8185
        real t8186
        real t8188
        real t8189
        real t819
        real t8190
        real t8192
        real t8193
        real t8194
        real t8196
        real t8198
        real t8200
        real t8202
        real t8203
        real t8207
        real t8208
        real t8209
        real t821
        real t8214
        real t8215
        real t8216
        real t8217
        real t8218
        real t8220
        real t8223
        real t8224
        real t8226
        real t8227
        real t823
        real t8230
        real t8231
        real t8233
        real t8235
        real t8237
        real t8239
        real t8241
        real t8242
        real t8247
        real t8249
        real t825
        real t8251
        real t8253
        real t8255
        real t8256
        real t8260
        real t8262
        real t8264
        real t8266
        real t8267
        real t827
        real t8271
        real t8273
        real t8276
        real t8277
        real t8278
        real t8279
        real t8280
        real t8281
        real t8282
        real t8283
        real t8284
        real t8286
        real t8289
        real t829
        real t8290
        real t8291
        real t8292
        real t8293
        real t8295
        real t8298
        real t8299
        real t83
        real t830
        real t8302
        real t8304
        real t8306
        real t8308
        real t831
        real t8310
        real t8313
        real t8314
        real t8316
        real t8318
        real t832
        real t8320
        real t8323
        real t8324
        real t8325
        real t8327
        real t8329
        real t833
        real t8331
        real t8333
        real t8334
        real t8338
        real t834
        real t8340
        real t8345
        real t8346
        real t8347
        real t8349
        real t835
        real t8353
        real t8355
        real t8357
        real t8359
        real t836
        real t8360
        real t8364
        real t8366
        real t8368
        real t8369
        real t837
        real t8370
        real t8372
        real t8374
        real t8375
        real t8376
        real t8377
        real t8378
        real t838
        real t8380
        real t8383
        real t8384
        real t8386
        real t8387
        real t8388
        real t839
        real t8390
        real t8392
        real t8394
        real t8397
        real t8399
        real t84
        real t840
        real t8401
        real t8403
        real t8405
        real t8407
        real t8410
        real t8411
        real t8412
        real t8414
        real t8416
        real t8419
        real t8420
        real t8421
        real t8424
        real t8425
        real t8426
        real t8427
        real t8428
        real t843
        real t8431
        real t8432
        real t8436
        real t8439
        real t844
        real t8441
        real t8444
        real t8445
        real t8446
        real t8448
        real t845
        real t8450
        real t8452
        real t8454
        real t8455
        real t8459
        real t846
        real t8461
        real t8466
        real t8467
        real t8468
        real t847
        real t8472
        real t8474
        real t8476
        real t8478
        real t848
        real t8480
        real t8481
        real t8482
        real t8483
        real t8484
        real t8486
        real t8489
        real t849
        real t8490
        real t8492
        real t8497
        real t8499
        real t8501
        real t8503
        real t8505
        real t8506
        real t8507
        real t8508
        real t8510
        real t8512
        real t8514
        real t8516
        real t8517
        real t852
        real t8521
        real t8523
        real t8528
        real t8529
        real t853
        real t8530
        real t8534
        real t8536
        real t8538
        real t8540
        real t8542
        real t8543
        real t8549
        real t855
        real t8551
        real t8553
        real t8555
        real t8556
        real t8557
        real t8558
        real t8559
        real t856
        real t8561
        real t8564
        real t8565
        real t8567
        real t8568
        real t8569
        real t857
        real t8571
        real t8572
        real t8573
        real t8574
        real t8576
        real t8579
        real t858
        real t8580
        real t8584
        real t8587
        real t8589
        real t8592
        real t8593
        real t8594
        real t8596
        real t8598
        real t860
        real t8600
        real t8602
        real t8603
        real t8607
        real t8609
        real t8614
        real t8615
        real t8616
        real t8620
        real t8622
        real t8624
        real t8626
        real t8628
        real t8629
        real t863
        real t8630
        real t8631
        real t8632
        real t8634
        real t8637
        real t8638
        real t864
        real t8640
        real t8645
        real t8647
        real t8649
        real t865
        real t8651
        real t8653
        real t8654
        real t8655
        real t8656
        real t8658
        real t866
        real t8660
        real t8662
        real t8664
        real t8665
        real t8669
        real t867
        real t8671
        real t8676
        real t8677
        real t8678
        real t868
        real t8682
        real t8684
        real t8686
        real t8688
        integer t869
        real t8690
        real t8691
        real t8697
        real t8699
        real t870
        real t8701
        real t8703
        real t8704
        real t8705
        real t8706
        real t8707
        real t8709
        real t871
        real t8712
        real t8713
        real t8715
        real t8716
        real t8717
        real t8719
        real t8721
        real t8723
        real t8725
        real t8728
        real t8729
        real t873
        real t8730
        real t8731
        real t8733
        real t8736
        real t8737
        real t8741
        real t8744
        real t8746
        real t8749
        real t875
        real t8750
        real t8751
        real t8753
        real t8755
        real t8757
        real t8759
        real t8760
        real t8764
        real t8766
        real t877
        real t8771
        real t8772
        real t8773
        real t8777
        real t8779
        real t8781
        real t8783
        real t8785
        real t8786
        real t8787
        real t8788
        real t8789
        real t879
        real t8791
        real t8794
        real t8795
        real t8797
        real t88
        real t880
        real t8802
        real t8804
        real t8806
        real t8808
        real t8810
        real t8811
        real t8812
        real t8813
        real t8815
        real t8817
        real t8819
        real t8821
        real t8822
        real t8826
        real t8828
        real t8833
        real t8834
        real t8835
        real t8839
        real t884
        real t8841
        real t8843
        real t8845
        real t8847
        real t8848
        real t8854
        real t8856
        real t8858
        real t886
        real t8860
        real t8861
        real t8862
        real t8863
        real t8864
        real t8866
        real t8869
        real t8870
        real t8872
        real t8873
        real t8874
        real t8876
        real t8877
        real t8878
        real t8879
        real t8881
        real t8884
        real t8885
        real t8889
        real t8892
        real t8894
        real t8897
        real t8898
        real t8899
        real t890
        real t8901
        real t8903
        real t8905
        real t8907
        real t8908
        real t8912
        real t8914
        real t8919
        real t892
        real t8920
        real t8921
        real t8925
        real t8927
        real t8929
        real t893
        real t8931
        real t8933
        real t8934
        real t8935
        real t8936
        real t8937
        real t8939
        real t8942
        real t8943
        real t8945
        real t8950
        real t8952
        real t8954
        real t8956
        real t8958
        real t8959
        real t8960
        real t8961
        real t8963
        real t8965
        real t8967
        real t8969
        real t8970
        real t8974
        real t8976
        real t898
        real t8981
        real t8982
        real t8983
        real t8987
        real t8989
        real t899
        real t8991
        real t8993
        real t8995
        real t8996
        real t9
        real t90
        real t9002
        real t9004
        real t9006
        real t9008
        real t9009
        real t901
        real t9010
        real t9011
        real t9012
        real t9014
        real t9017
        real t9018
        real t902
        real t9020
        real t9021
        real t9022
        real t9024
        real t9026
        real t9028
        real t9031
        real t9033
        real t9035
        real t9037
        real t9039
        real t904
        real t9042
        real t9044
        real t9046
        real t9048
        real t9051
        real t9053
        real t9055
        real t9057
        real t9059
        real t9061
        real t9064
        real t9066
        real t9068
        real t9070
        real t9072
        real t9075
        real t9076
        real t9077
        real t9080
        real t9081
        real t9083
        real t9084
        real t9086
        real t9089
        real t909
        real t9091
        real t9094
        real t9098
        real t910
        real t9100
        real t9105
        real t9108
        real t9110
        real t9112
        real t9114
        real t9116
        real t9117
        real t9118
        real t912
        real t9120
        real t9122
        real t9124
        real t9126
        real t9128
        real t9129
        real t913
        real t9131
        real t9133
        real t9135
        real t9136
        real t9137
        real t9138
        real t9140
        real t9141
        real t9143
        real t9144
        real t9146
        real t9148
        real t915
        real t9150
        real t9152
        real t9154
        real t9155
        real t9156
        real t9158
        real t9159
        real t9161
        real t9163
        real t9165
        real t9167
        real t9168
        real t917
        real t9170
        real t9172
        real t9174
        real t9176
        real t9177
        real t9179
        real t9181
        real t9183
        real t9184
        real t9186
        real t9188
        real t919
        real t9190
        real t9192
        real t9194
        real t9196
        real t9197
        real t9199
        real t9201
        real t9203
        real t9205
        real t9207
        real t9208
        real t9209
        real t921
        real t9210
        real t9212
        real t9213
        real t9214
        real t9216
        real t9219
        real t922
        real t9221
        real t9223
        real t9227
        real t9229
        real t923
        real t9230
        real t9232
        real t9235
        real t9237
        real t9238
        real t9241
        real t9242
        real t9243
        real t9248
        real t925
        real t9250
        real t9251
        real t9254
        real t9256
        real t9257
        real t926
        real t9260
        real t9262
        real t9264
        real t9266
        real t9268
        real t9270
        real t9272
        real t9274
        real t9276
        real t9278
        real t928
        real t9285
        real t9291
        real t9293
        real t930
        real t9301
        real t9307
        real t9309
        real t9317
        real t9318
        real t932
        real t9322
        real t9324
        real t9325
        real t9329
        real t9337
        real t934
        real t9342
        real t9344
        real t9345
        real t9348
        real t9349
        real t9355
        real t9366
        real t9367
        real t9368
        real t937
        real t9371
        real t9372
        real t9373
        real t9374
        real t9375
        real t9379
        real t9381
        real t9385
        real t9388
        real t9389
        real t939
        real t9396
        real t940
        real t9401
        real t9402
        real t9404
        real t9408
        real t9410
        real t9412
        real t9418
        real t942
        real t9420
        real t9422
        real t9425
        real t9427
        real t9429
        real t9430
        real t9431
        real t9434
        real t9436
        real t9438
        real t9439
        real t944
        real t9441
        real t9442
        real t9446
        real t9448
        real t9450
        real t9454
        real t9459
        real t946
        real t9461
        real t9463
        real t9470
        real t9474
        real t9479
        real t948
        real t9489
        real t9490
        real t9494
        real t9496
        real t95
        real t950
        real t9502
        real t9504
        real t9506
        real t9509
        real t9511
        real t9513
        real t9514
        real t9518
        real t9520
        real t9523
        real t9525
        real t9526
        real t953
        real t9530
        real t9532
        real t9534
        real t9538
        real t954
        real t9543
        real t9545
        real t9547
        real t9554
        real t9558
        real t956
        real t9563
        real t957
        real t9573
        real t9574
        real t9577
        real t9579
        real t9581
        real t9583
        real t9585
        real t9586
        real t9588
        real t9589
        real t959
        real t9590
        real t9591
        real t9592
        real t9593
        real t9595
        real t9597
        real t9599
        real t96
        real t9601
        real t9603
        real t9604
        real t9606
        real t9608
        real t961
        real t9610
        real t9612
        real t9613
        real t9615
        real t9617
        real t9619
        real t9621
        real t9622
        real t9624
        real t9626
        real t9628
        real t9629
        real t963
        real t9630
        real t9631
        real t9633
        real t9634
        real t9635
        real t9636
        real t9637
        real t9638
        real t964
        real t9640
        real t9642
        real t9643
        real t9644
        real t9646
        real t9647
        real t9649
        real t965
        real t9651
        real t9652
        real t9653
        real t9654
        real t9656
        real t9658
        real t9660
        real t9662
        real t9664
        real t9665
        real t9667
        real t9669
        real t967
        real t9671
        real t9673
        real t9674
        real t9676
        real t9678
        real t9680
        real t9682
        real t9683
        real t9685
        real t9687
        real t9689
        real t969
        real t9690
        real t9691
        real t9692
        real t9694
        real t9695
        real t9696
        real t9697
        real t9698
        real t97
        real t9700
        real t9706
        real t9710
        real t9711
        real t9714
        real t9715
        real t9718
        real t9720
        real t9722
        real t9729
        real t9731
        real t9732
        real t9735
        real t9736
        real t974
        real t9742
        real t9746
        real t975
        real t9753
        real t9754
        real t9755
        real t9758
        real t9759
        real t9760
        real t9761
        real t9762
        real t9766
        real t9768
        real t977
        real t9772
        real t9775
        real t9776
        real t978
        real t9784
        real t9788
        real t979
        real t9793
        real t9795
        real t9796
        real t98
        real t9800
        real t9808
        real t981
        real t9813
        real t9815
        real t9816
        real t9819
        real t9820
        real t9826
        real t9837
        real t9838
        real t9839
        real t9842
        real t9843
        real t9844
        real t9845
        real t9846
        real t9850
        real t9852
        real t9856
        real t9859
        real t986
        real t9860
        real t9867
        real t987
        real t9872
        real t9874
        real t9879
        real t9881
        real t9885
        real t9887
        real t989
        real t9890
        real t9892
        real t9893
        real t9899
        real t99
        real t9901
        real t9903
        real t9906
        real t9908
        real t9910
        real t9911
        real t9915
        real t992
        real t9929
        real t9933
        real t9938
        real t9946
        real t9947
        real t9951
        real t9952
        real t9953
        real t9957
        real t9958
        real t9959
        real t9962
        real t9964
        real t9965
        real t9971
        real t9973
        real t9975
        real t9978
        real t998
        real t9980
        real t9982
        real t9983
        real t9987
        t1 = u(i,j,k,n)
        t2 = ut(i,j,k,n)
        t4 = cc ** 2
        t5 = i + 1
        t6 = rx(t5,j,k,0,0)
        t7 = rx(t5,j,k,1,1)
        t9 = rx(t5,j,k,2,2)
        t11 = rx(t5,j,k,1,2)
        t13 = rx(t5,j,k,2,1)
        t15 = rx(t5,j,k,0,1)
        t16 = rx(t5,j,k,1,0)
        t20 = rx(t5,j,k,2,0)
        t22 = rx(t5,j,k,0,2)
        t27 = -t11 * t13 * t6 + t11 * t15 * t20 + t13 * t16 * t22 - t15 
     #* t16 * t9 - t20 * t22 * t7 + t6 * t7 * t9
        t28 = 0.1E1 / t27
        t29 = t6 ** 2
        t30 = t15 ** 2
        t31 = t22 ** 2
        t32 = t29 + t30 + t31
        t33 = t28 * t32
        t34 = rx(i,j,k,0,0)
        t35 = rx(i,j,k,1,1)
        t37 = rx(i,j,k,2,2)
        t39 = rx(i,j,k,1,2)
        t41 = rx(i,j,k,2,1)
        t43 = rx(i,j,k,0,1)
        t44 = rx(i,j,k,1,0)
        t48 = rx(i,j,k,2,0)
        t50 = rx(i,j,k,0,2)
        t55 = t34 * t35 * t37 - t34 * t39 * t41 - t35 * t48 * t50 - t37 
     #* t43 * t44 + t39 * t43 * t48 + t41 * t44 * t50
        t56 = 0.1E1 / t55
        t57 = t34 ** 2
        t58 = t43 ** 2
        t59 = t50 ** 2
        t60 = t57 + t58 + t59
        t61 = t56 * t60
        t64 = t4 * (t33 / 0.2E1 + t61 / 0.2E1)
        t65 = sqrt(0.3E1)
        t66 = t65 / 0.6E1
        t67 = 0.1E1 / 0.2E1 + t66
        t68 = t67 ** 2
        t69 = t68 * t67
        t71 = dt ** 2
        t72 = t71 * dt
        t73 = i + 2
        t74 = rx(t73,j,k,0,0)
        t75 = rx(t73,j,k,1,1)
        t77 = rx(t73,j,k,2,2)
        t79 = rx(t73,j,k,1,2)
        t81 = rx(t73,j,k,2,1)
        t83 = rx(t73,j,k,0,1)
        t84 = rx(t73,j,k,1,0)
        t88 = rx(t73,j,k,2,0)
        t90 = rx(t73,j,k,0,2)
        t95 = t74 * t75 * t77 - t74 * t79 * t81 - t75 * t88 * t90 - t77 
     #* t83 * t84 + t79 * t83 * t88 + t81 * t84 * t90
        t96 = 0.1E1 / t95
        t97 = t74 ** 2
        t98 = t83 ** 2
        t99 = t90 ** 2
        t100 = t97 + t98 + t99
        t101 = t96 * t100
        t104 = t4 * (t101 / 0.2E1 + t33 / 0.2E1)
        t105 = ut(t73,j,k,n)
        t106 = ut(t5,j,k,n)
        t108 = 0.1E1 / dx
        t109 = (t105 - t106) * t108
        t110 = t104 * t109
        t112 = (t106 - t2) * t108
        t113 = t64 * t112
        t115 = (t110 - t113) * t108
        t116 = t4 * t96
        t120 = t74 * t84 + t75 * t83 + t79 * t90
        t121 = j + 1
        t122 = ut(t73,t121,k,n)
        t124 = 0.1E1 / dy
        t125 = (t122 - t105) * t124
        t126 = j - 1
        t127 = ut(t73,t126,k,n)
        t129 = (t105 - t127) * t124
        t131 = t125 / 0.2E1 + t129 / 0.2E1
        t128 = t116 * t120
        t133 = t128 * t131
        t134 = t4 * t28
        t138 = t11 * t22 + t15 * t7 + t16 * t6
        t139 = ut(t5,t121,k,n)
        t141 = (t139 - t106) * t124
        t142 = ut(t5,t126,k,n)
        t144 = (t106 - t142) * t124
        t146 = t141 / 0.2E1 + t144 / 0.2E1
        t143 = t134 * t138
        t148 = t143 * t146
        t150 = (t133 - t148) * t108
        t151 = t150 / 0.2E1
        t152 = t4 * t56
        t156 = t34 * t44 + t35 * t43 + t39 * t50
        t157 = ut(i,t121,k,n)
        t159 = (t157 - t2) * t124
        t160 = ut(i,t126,k,n)
        t162 = (t2 - t160) * t124
        t164 = t159 / 0.2E1 + t162 / 0.2E1
        t161 = t152 * t156
        t166 = t161 * t164
        t168 = (t148 - t166) * t108
        t169 = t168 / 0.2E1
        t173 = t74 * t88 + t77 * t90 + t81 * t83
        t174 = k + 1
        t175 = ut(t73,j,t174,n)
        t177 = 0.1E1 / dz
        t178 = (t175 - t105) * t177
        t179 = k - 1
        t180 = ut(t73,j,t179,n)
        t182 = (t105 - t180) * t177
        t184 = t178 / 0.2E1 + t182 / 0.2E1
        t181 = t116 * t173
        t186 = t181 * t184
        t190 = t13 * t15 + t20 * t6 + t22 * t9
        t191 = ut(t5,j,t174,n)
        t193 = (t191 - t106) * t177
        t194 = ut(t5,j,t179,n)
        t196 = (t106 - t194) * t177
        t198 = t193 / 0.2E1 + t196 / 0.2E1
        t195 = t134 * t190
        t200 = t195 * t198
        t202 = (t186 - t200) * t108
        t203 = t202 / 0.2E1
        t207 = t34 * t48 + t37 * t50 + t41 * t43
        t208 = ut(i,j,t174,n)
        t210 = (t208 - t2) * t177
        t211 = ut(i,j,t179,n)
        t213 = (t2 - t211) * t177
        t215 = t210 / 0.2E1 + t213 / 0.2E1
        t212 = t152 * t207
        t217 = t212 * t215
        t219 = (t200 - t217) * t108
        t220 = t219 / 0.2E1
        t221 = rx(t5,t121,k,0,0)
        t222 = rx(t5,t121,k,1,1)
        t224 = rx(t5,t121,k,2,2)
        t226 = rx(t5,t121,k,1,2)
        t228 = rx(t5,t121,k,2,1)
        t230 = rx(t5,t121,k,0,1)
        t231 = rx(t5,t121,k,1,0)
        t235 = rx(t5,t121,k,2,0)
        t237 = rx(t5,t121,k,0,2)
        t242 = t221 * t222 * t224 - t221 * t226 * t228 - t222 * t235 * t
     #237 - t224 * t230 * t231 + t226 * t230 * t235 + t228 * t231 * t237
        t243 = 0.1E1 / t242
        t244 = t4 * t243
        t250 = (t122 - t139) * t108
        t252 = (t139 - t157) * t108
        t254 = t250 / 0.2E1 + t252 / 0.2E1
        t251 = t244 * (t221 * t231 + t222 * t230 + t226 * t237)
        t256 = t251 * t254
        t258 = t109 / 0.2E1 + t112 / 0.2E1
        t260 = t143 * t258
        t262 = (t256 - t260) * t124
        t263 = t262 / 0.2E1
        t264 = rx(t5,t126,k,0,0)
        t265 = rx(t5,t126,k,1,1)
        t267 = rx(t5,t126,k,2,2)
        t269 = rx(t5,t126,k,1,2)
        t271 = rx(t5,t126,k,2,1)
        t273 = rx(t5,t126,k,0,1)
        t274 = rx(t5,t126,k,1,0)
        t278 = rx(t5,t126,k,2,0)
        t280 = rx(t5,t126,k,0,2)
        t285 = t264 * t265 * t267 - t264 * t269 * t271 - t265 * t278 * t
     #280 - t267 * t273 * t274 + t269 * t273 * t278 + t271 * t274 * t280
        t286 = 0.1E1 / t285
        t287 = t4 * t286
        t293 = (t127 - t142) * t108
        t295 = (t142 - t160) * t108
        t297 = t293 / 0.2E1 + t295 / 0.2E1
        t292 = t287 * (t264 * t274 + t265 * t273 + t269 * t280)
        t299 = t292 * t297
        t301 = (t260 - t299) * t124
        t302 = t301 / 0.2E1
        t303 = t231 ** 2
        t304 = t222 ** 2
        t305 = t226 ** 2
        t307 = t243 * (t303 + t304 + t305)
        t308 = t16 ** 2
        t309 = t7 ** 2
        t310 = t11 ** 2
        t312 = t28 * (t308 + t309 + t310)
        t315 = t4 * (t307 / 0.2E1 + t312 / 0.2E1)
        t316 = t315 * t141
        t317 = t274 ** 2
        t318 = t265 ** 2
        t319 = t269 ** 2
        t321 = t286 * (t317 + t318 + t319)
        t324 = t4 * (t312 / 0.2E1 + t321 / 0.2E1)
        t325 = t324 * t144
        t327 = (t316 - t325) * t124
        t331 = t222 * t228 + t224 * t226 + t231 * t235
        t332 = ut(t5,t121,t174,n)
        t334 = (t332 - t139) * t177
        t335 = ut(t5,t121,t179,n)
        t337 = (t139 - t335) * t177
        t339 = t334 / 0.2E1 + t337 / 0.2E1
        t333 = t244 * t331
        t341 = t333 * t339
        t342 = t134 * (t11 * t9 + t13 * t7 + t16 * t20)
        t347 = t342 * t198
        t349 = (t341 - t347) * t124
        t350 = t349 / 0.2E1
        t354 = t265 * t271 + t267 * t269 + t274 * t278
        t355 = ut(t5,t126,t174,n)
        t357 = (t355 - t142) * t177
        t358 = ut(t5,t126,t179,n)
        t360 = (t142 - t358) * t177
        t362 = t357 / 0.2E1 + t360 / 0.2E1
        t356 = t287 * t354
        t364 = t356 * t362
        t366 = (t347 - t364) * t124
        t367 = t366 / 0.2E1
        t368 = rx(t5,j,t174,0,0)
        t369 = rx(t5,j,t174,1,1)
        t371 = rx(t5,j,t174,2,2)
        t373 = rx(t5,j,t174,1,2)
        t375 = rx(t5,j,t174,2,1)
        t377 = rx(t5,j,t174,0,1)
        t378 = rx(t5,j,t174,1,0)
        t382 = rx(t5,j,t174,2,0)
        t384 = rx(t5,j,t174,0,2)
        t389 = t368 * t369 * t371 - t368 * t373 * t375 - t369 * t382 * t
     #384 - t371 * t377 * t378 + t373 * t377 * t382 + t375 * t378 * t384
        t390 = 0.1E1 / t389
        t391 = t4 * t390
        t397 = (t175 - t191) * t108
        t399 = (t191 - t208) * t108
        t401 = t397 / 0.2E1 + t399 / 0.2E1
        t396 = t391 * (t368 * t382 + t371 * t384 + t375 * t377)
        t403 = t396 * t401
        t405 = t195 * t258
        t407 = (t403 - t405) * t177
        t408 = t407 / 0.2E1
        t409 = rx(t5,j,t179,0,0)
        t410 = rx(t5,j,t179,1,1)
        t412 = rx(t5,j,t179,2,2)
        t414 = rx(t5,j,t179,1,2)
        t416 = rx(t5,j,t179,2,1)
        t418 = rx(t5,j,t179,0,1)
        t419 = rx(t5,j,t179,1,0)
        t423 = rx(t5,j,t179,2,0)
        t425 = rx(t5,j,t179,0,2)
        t430 = t409 * t410 * t412 - t409 * t414 * t416 - t410 * t423 * t
     #425 - t412 * t418 * t419 + t414 * t418 * t423 + t416 * t419 * t425
        t431 = 0.1E1 / t430
        t432 = t4 * t431
        t438 = (t180 - t194) * t108
        t440 = (t194 - t211) * t108
        t442 = t438 / 0.2E1 + t440 / 0.2E1
        t435 = t432 * (t409 * t423 + t412 * t425 + t416 * t418)
        t444 = t435 * t442
        t446 = (t405 - t444) * t177
        t447 = t446 / 0.2E1
        t451 = t369 * t375 + t371 * t373 + t378 * t382
        t453 = (t332 - t191) * t124
        t455 = (t191 - t355) * t124
        t457 = t453 / 0.2E1 + t455 / 0.2E1
        t450 = t391 * t451
        t459 = t450 * t457
        t461 = t342 * t146
        t463 = (t459 - t461) * t177
        t464 = t463 / 0.2E1
        t468 = t410 * t416 + t412 * t414 + t419 * t423
        t470 = (t335 - t194) * t124
        t472 = (t194 - t358) * t124
        t474 = t470 / 0.2E1 + t472 / 0.2E1
        t466 = t432 * t468
        t476 = t466 * t474
        t478 = (t461 - t476) * t177
        t479 = t478 / 0.2E1
        t480 = t382 ** 2
        t481 = t375 ** 2
        t482 = t371 ** 2
        t484 = t390 * (t480 + t481 + t482)
        t485 = t20 ** 2
        t486 = t13 ** 2
        t487 = t9 ** 2
        t489 = t28 * (t485 + t486 + t487)
        t492 = t4 * (t484 / 0.2E1 + t489 / 0.2E1)
        t493 = t492 * t193
        t494 = t423 ** 2
        t495 = t416 ** 2
        t496 = t412 ** 2
        t498 = t431 * (t494 + t495 + t496)
        t501 = t4 * (t489 / 0.2E1 + t498 / 0.2E1)
        t502 = t501 * t196
        t504 = (t493 - t502) * t177
        t505 = t115 + t151 + t169 + t203 + t220 + t263 + t302 + t327 + t
     #350 + t367 + t408 + t447 + t464 + t479 + t504
        t506 = t505 * t27
        t507 = i - 1
        t508 = rx(t507,j,k,0,0)
        t509 = rx(t507,j,k,1,1)
        t511 = rx(t507,j,k,2,2)
        t513 = rx(t507,j,k,1,2)
        t515 = rx(t507,j,k,2,1)
        t517 = rx(t507,j,k,0,1)
        t518 = rx(t507,j,k,1,0)
        t522 = rx(t507,j,k,2,0)
        t524 = rx(t507,j,k,0,2)
        t529 = t508 * t509 * t511 - t508 * t513 * t515 - t509 * t522 * t
     #524 - t511 * t517 * t518 + t513 * t517 * t522 + t515 * t518 * t524
        t530 = 0.1E1 / t529
        t531 = t508 ** 2
        t532 = t517 ** 2
        t533 = t524 ** 2
        t534 = t531 + t532 + t533
        t535 = t530 * t534
        t538 = t4 * (t61 / 0.2E1 + t535 / 0.2E1)
        t539 = ut(t507,j,k,n)
        t541 = (t2 - t539) * t108
        t542 = t538 * t541
        t544 = (t113 - t542) * t108
        t545 = t4 * t530
        t549 = t508 * t518 + t509 * t517 + t513 * t524
        t550 = ut(t507,t121,k,n)
        t552 = (t550 - t539) * t124
        t553 = ut(t507,t126,k,n)
        t555 = (t539 - t553) * t124
        t557 = t552 / 0.2E1 + t555 / 0.2E1
        t547 = t545 * t549
        t559 = t547 * t557
        t561 = (t166 - t559) * t108
        t562 = t561 / 0.2E1
        t566 = t508 * t522 + t511 * t524 + t515 * t517
        t567 = ut(t507,j,t174,n)
        t569 = (t567 - t539) * t177
        t570 = ut(t507,j,t179,n)
        t572 = (t539 - t570) * t177
        t574 = t569 / 0.2E1 + t572 / 0.2E1
        t564 = t545 * t566
        t576 = t564 * t574
        t578 = (t217 - t576) * t108
        t579 = t578 / 0.2E1
        t580 = rx(i,t121,k,0,0)
        t581 = rx(i,t121,k,1,1)
        t583 = rx(i,t121,k,2,2)
        t585 = rx(i,t121,k,1,2)
        t587 = rx(i,t121,k,2,1)
        t589 = rx(i,t121,k,0,1)
        t590 = rx(i,t121,k,1,0)
        t594 = rx(i,t121,k,2,0)
        t596 = rx(i,t121,k,0,2)
        t601 = t580 * t581 * t583 - t580 * t585 * t587 - t581 * t594 * t
     #596 - t583 * t589 * t590 + t585 * t589 * t594 + t587 * t590 * t596
        t602 = 0.1E1 / t601
        t603 = t4 * t602
        t607 = t580 * t590 + t581 * t589 + t585 * t596
        t609 = (t157 - t550) * t108
        t611 = t252 / 0.2E1 + t609 / 0.2E1
        t604 = t603 * t607
        t613 = t604 * t611
        t615 = t112 / 0.2E1 + t541 / 0.2E1
        t617 = t161 * t615
        t619 = (t613 - t617) * t124
        t620 = t619 / 0.2E1
        t621 = rx(i,t126,k,0,0)
        t622 = rx(i,t126,k,1,1)
        t624 = rx(i,t126,k,2,2)
        t626 = rx(i,t126,k,1,2)
        t628 = rx(i,t126,k,2,1)
        t630 = rx(i,t126,k,0,1)
        t631 = rx(i,t126,k,1,0)
        t635 = rx(i,t126,k,2,0)
        t637 = rx(i,t126,k,0,2)
        t642 = t621 * t622 * t624 - t621 * t626 * t628 - t622 * t635 * t
     #637 - t624 * t630 * t631 + t626 * t630 * t635 + t628 * t631 * t637
        t643 = 0.1E1 / t642
        t644 = t4 * t643
        t648 = t621 * t631 + t622 * t630 + t626 * t637
        t650 = (t160 - t553) * t108
        t652 = t295 / 0.2E1 + t650 / 0.2E1
        t641 = t644 * t648
        t654 = t641 * t652
        t656 = (t617 - t654) * t124
        t657 = t656 / 0.2E1
        t658 = t590 ** 2
        t659 = t581 ** 2
        t660 = t585 ** 2
        t661 = t658 + t659 + t660
        t662 = t602 * t661
        t663 = t44 ** 2
        t664 = t35 ** 2
        t665 = t39 ** 2
        t666 = t663 + t664 + t665
        t667 = t56 * t666
        t670 = t4 * (t662 / 0.2E1 + t667 / 0.2E1)
        t671 = t670 * t159
        t672 = t631 ** 2
        t673 = t622 ** 2
        t674 = t626 ** 2
        t675 = t672 + t673 + t674
        t676 = t643 * t675
        t679 = t4 * (t667 / 0.2E1 + t676 / 0.2E1)
        t680 = t679 * t162
        t682 = (t671 - t680) * t124
        t686 = t581 * t587 + t583 * t585 + t590 * t594
        t687 = ut(i,t121,t174,n)
        t689 = (t687 - t157) * t177
        t690 = ut(i,t121,t179,n)
        t692 = (t157 - t690) * t177
        t694 = t689 / 0.2E1 + t692 / 0.2E1
        t683 = t603 * t686
        t696 = t683 * t694
        t700 = t35 * t41 + t37 * t39 + t44 * t48
        t691 = t152 * t700
        t702 = t691 * t215
        t704 = (t696 - t702) * t124
        t705 = t704 / 0.2E1
        t709 = t622 * t628 + t624 * t626 + t631 * t635
        t710 = ut(i,t126,t174,n)
        t712 = (t710 - t160) * t177
        t713 = ut(i,t126,t179,n)
        t715 = (t160 - t713) * t177
        t717 = t712 / 0.2E1 + t715 / 0.2E1
        t706 = t644 * t709
        t719 = t706 * t717
        t721 = (t702 - t719) * t124
        t722 = t721 / 0.2E1
        t723 = rx(i,j,t174,0,0)
        t724 = rx(i,j,t174,1,1)
        t726 = rx(i,j,t174,2,2)
        t728 = rx(i,j,t174,1,2)
        t730 = rx(i,j,t174,2,1)
        t732 = rx(i,j,t174,0,1)
        t733 = rx(i,j,t174,1,0)
        t737 = rx(i,j,t174,2,0)
        t739 = rx(i,j,t174,0,2)
        t744 = t723 * t724 * t726 - t723 * t728 * t730 - t724 * t737 * t
     #739 - t726 * t732 * t733 + t728 * t732 * t737 + t730 * t733 * t739
        t745 = 0.1E1 / t744
        t746 = t4 * t745
        t750 = t723 * t737 + t726 * t739 + t730 * t732
        t752 = (t208 - t567) * t108
        t754 = t399 / 0.2E1 + t752 / 0.2E1
        t743 = t746 * t750
        t756 = t743 * t754
        t758 = t212 * t615
        t760 = (t756 - t758) * t177
        t761 = t760 / 0.2E1
        t762 = rx(i,j,t179,0,0)
        t763 = rx(i,j,t179,1,1)
        t765 = rx(i,j,t179,2,2)
        t767 = rx(i,j,t179,1,2)
        t769 = rx(i,j,t179,2,1)
        t771 = rx(i,j,t179,0,1)
        t772 = rx(i,j,t179,1,0)
        t776 = rx(i,j,t179,2,0)
        t778 = rx(i,j,t179,0,2)
        t783 = t762 * t763 * t765 - t762 * t767 * t769 - t763 * t776 * t
     #778 - t765 * t771 * t772 + t767 * t771 * t776 + t769 * t772 * t778
        t784 = 0.1E1 / t783
        t785 = t4 * t784
        t789 = t762 * t776 + t765 * t778 + t769 * t771
        t791 = (t211 - t570) * t108
        t793 = t440 / 0.2E1 + t791 / 0.2E1
        t781 = t785 * t789
        t795 = t781 * t793
        t797 = (t758 - t795) * t177
        t798 = t797 / 0.2E1
        t802 = t724 * t730 + t726 * t728 + t733 * t737
        t804 = (t687 - t208) * t124
        t806 = (t208 - t710) * t124
        t808 = t804 / 0.2E1 + t806 / 0.2E1
        t796 = t746 * t802
        t810 = t796 * t808
        t812 = t691 * t164
        t814 = (t810 - t812) * t177
        t815 = t814 / 0.2E1
        t819 = t763 * t769 + t765 * t767 + t772 * t776
        t821 = (t690 - t211) * t124
        t823 = (t211 - t713) * t124
        t825 = t821 / 0.2E1 + t823 / 0.2E1
        t811 = t785 * t819
        t827 = t811 * t825
        t829 = (t812 - t827) * t177
        t830 = t829 / 0.2E1
        t831 = t737 ** 2
        t832 = t730 ** 2
        t833 = t726 ** 2
        t834 = t831 + t832 + t833
        t835 = t745 * t834
        t836 = t48 ** 2
        t837 = t41 ** 2
        t838 = t37 ** 2
        t839 = t836 + t837 + t838
        t840 = t56 * t839
        t843 = t4 * (t835 / 0.2E1 + t840 / 0.2E1)
        t844 = t843 * t210
        t845 = t776 ** 2
        t846 = t769 ** 2
        t847 = t765 ** 2
        t848 = t845 + t846 + t847
        t849 = t784 * t848
        t852 = t4 * (t840 / 0.2E1 + t849 / 0.2E1)
        t853 = t852 * t213
        t855 = (t844 - t853) * t177
        t856 = t544 + t169 + t562 + t220 + t579 + t620 + t657 + t682 + t
     #705 + t722 + t761 + t798 + t815 + t830 + t855
        t857 = t856 * t55
        t858 = t506 - t857
        t860 = t72 * t858 * t108
        t863 = 0.1E1 / 0.2E1 - t66
        t864 = beta * t863
        t865 = t864 * dt
        t866 = sqrt(t32)
        t867 = cc * t866
        t868 = dx ** 2
        t869 = i + 3
        t870 = rx(t869,j,k,0,0)
        t871 = rx(t869,j,k,1,1)
        t873 = rx(t869,j,k,2,2)
        t875 = rx(t869,j,k,1,2)
        t877 = rx(t869,j,k,2,1)
        t879 = rx(t869,j,k,0,1)
        t880 = rx(t869,j,k,1,0)
        t884 = rx(t869,j,k,2,0)
        t886 = rx(t869,j,k,0,2)
        t892 = 0.1E1 / (t870 * t871 * t873 - t870 * t875 * t877 - t871 *
     # t884 * t886 - t873 * t879 * t880 + t875 * t879 * t884 + t877 * t8
     #80 * t886)
        t893 = t4 * t892
        t898 = u(t869,j,t174,n)
        t899 = u(t869,j,k,n)
        t901 = (t898 - t899) * t177
        t902 = u(t869,j,t179,n)
        t904 = (t899 - t902) * t177
        t909 = u(t73,j,t174,n)
        t910 = u(t73,j,k,n)
        t912 = (t909 - t910) * t177
        t913 = u(t73,j,t179,n)
        t915 = (t910 - t913) * t177
        t917 = t912 / 0.2E1 + t915 / 0.2E1
        t919 = t181 * t917
        t890 = t893 * (t870 * t884 + t873 * t886 + t877 * t879)
        t921 = (t890 * (t901 / 0.2E1 + t904 / 0.2E1) - t919) * t108
        t922 = u(t5,j,t174,n)
        t923 = u(t5,j,k,n)
        t925 = (t922 - t923) * t177
        t926 = u(t5,j,t179,n)
        t928 = (t923 - t926) * t177
        t930 = t925 / 0.2E1 + t928 / 0.2E1
        t932 = t195 * t930
        t934 = (t919 - t932) * t108
        t937 = u(i,j,t174,n)
        t939 = (t937 - t1) * t177
        t940 = u(i,j,t179,n)
        t942 = (t1 - t940) * t177
        t944 = t939 / 0.2E1 + t942 / 0.2E1
        t946 = t212 * t944
        t948 = (t932 - t946) * t108
        t950 = (t934 - t948) * t108
        t953 = u(t507,j,t174,n)
        t954 = u(t507,j,k,n)
        t956 = (t953 - t954) * t177
        t957 = u(t507,j,t179,n)
        t959 = (t954 - t957) * t177
        t961 = t956 / 0.2E1 + t959 / 0.2E1
        t963 = t564 * t961
        t965 = (t946 - t963) * t108
        t967 = (t948 - t965) * t108
        t969 = (t950 - t967) * t108
        t974 = u(t869,t121,k,n)
        t975 = u(t73,t121,k,n)
        t977 = (t974 - t975) * t108
        t978 = u(t5,t121,k,n)
        t979 = u(i,t121,k,n)
        t981 = (t978 - t979) * t108
        t986 = (t975 - t978) * t108
        t987 = u(t507,t121,k,n)
        t989 = (t979 - t987) * t108
        t992 = (t986 / 0.2E1 - t989 / 0.2E1) * t108
        t998 = (t899 - t910) * t108
        t1000 = (t923 - t1) * t108
        t1005 = (t910 - t923) * t108
        t1007 = (t1 - t954) * t108
        t1010 = (t1005 / 0.2E1 - t1007 / 0.2E1) * t108
        t964 = ((t998 / 0.2E1 - t1000 / 0.2E1) * t108 - t1010) * t108
        t1014 = t143 * t964
        t1017 = u(t869,t126,k,n)
        t1018 = u(t73,t126,k,n)
        t1020 = (t1017 - t1018) * t108
        t1021 = u(t5,t126,k,n)
        t1022 = u(i,t126,k,n)
        t1024 = (t1021 - t1022) * t108
        t1029 = (t1018 - t1021) * t108
        t1030 = u(t507,t126,k,n)
        t1032 = (t1022 - t1030) * t108
        t1035 = (t1029 / 0.2E1 - t1032 / 0.2E1) * t108
        t1046 = dy ** 2
        t1047 = j + 2
        t1048 = rx(t5,t1047,k,0,0)
        t1049 = rx(t5,t1047,k,1,1)
        t1051 = rx(t5,t1047,k,2,2)
        t1053 = rx(t5,t1047,k,1,2)
        t1055 = rx(t5,t1047,k,2,1)
        t1057 = rx(t5,t1047,k,0,1)
        t1058 = rx(t5,t1047,k,1,0)
        t1062 = rx(t5,t1047,k,2,0)
        t1064 = rx(t5,t1047,k,0,2)
        t1069 = t1048 * t1049 * t1051 - t1048 * t1053 * t1055 - t1049 * 
     #t1062 * t1064 - t1051 * t1057 * t1058 + t1053 * t1057 * t1062 + t1
     #055 * t1058 * t1064
        t1070 = 0.1E1 / t1069
        t1071 = t4 * t1070
        t1076 = u(t73,t1047,k,n)
        t1077 = u(t5,t1047,k,n)
        t1079 = (t1076 - t1077) * t108
        t1080 = u(i,t1047,k,n)
        t1082 = (t1077 - t1080) * t108
        t1084 = t1079 / 0.2E1 + t1082 / 0.2E1
        t1004 = t1071 * (t1048 * t1058 + t1049 * t1057 + t1053 * t1064)
        t1086 = t1004 * t1084
        t1088 = t986 / 0.2E1 + t981 / 0.2E1
        t1090 = t251 * t1088
        t1092 = (t1086 - t1090) * t124
        t1094 = t1005 / 0.2E1 + t1000 / 0.2E1
        t1096 = t143 * t1094
        t1098 = (t1090 - t1096) * t124
        t1102 = t1029 / 0.2E1 + t1024 / 0.2E1
        t1104 = t292 * t1102
        t1106 = (t1096 - t1104) * t124
        t1108 = (t1098 - t1106) * t124
        t1111 = j - 2
        t1112 = rx(t5,t1111,k,0,0)
        t1113 = rx(t5,t1111,k,1,1)
        t1115 = rx(t5,t1111,k,2,2)
        t1117 = rx(t5,t1111,k,1,2)
        t1119 = rx(t5,t1111,k,2,1)
        t1121 = rx(t5,t1111,k,0,1)
        t1122 = rx(t5,t1111,k,1,0)
        t1126 = rx(t5,t1111,k,2,0)
        t1128 = rx(t5,t1111,k,0,2)
        t1133 = t1112 * t1113 * t1115 - t1112 * t1117 * t1119 - t1113 * 
     #t1126 * t1128 - t1115 * t1121 * t1122 + t1117 * t1121 * t1126 + t1
     #119 * t1122 * t1128
        t1134 = 0.1E1 / t1133
        t1135 = t4 * t1134
        t1140 = u(t73,t1111,k,n)
        t1141 = u(t5,t1111,k,n)
        t1143 = (t1140 - t1141) * t108
        t1144 = u(i,t1111,k,n)
        t1146 = (t1141 - t1144) * t108
        t1148 = t1143 / 0.2E1 + t1146 / 0.2E1
        t1045 = t1135 * (t1112 * t1122 + t1113 * t1121 + t1117 * t1128)
        t1150 = t1045 * t1148
        t1152 = (t1104 - t1150) * t124
        t1162 = t312 / 0.2E1
        t1163 = t1058 ** 2
        t1164 = t1049 ** 2
        t1165 = t1053 ** 2
        t1167 = t1070 * (t1163 + t1164 + t1165)
        t1177 = t4 * (t307 / 0.2E1 + t1162 - dy * ((t1167 - t307) * t124
     # / 0.2E1 - (t312 - t321) * t124 / 0.2E1) / 0.8E1)
        t1179 = (t978 - t923) * t124
        t1184 = t1122 ** 2
        t1185 = t1113 ** 2
        t1186 = t1117 ** 2
        t1188 = t1134 * (t1184 + t1185 + t1186)
        t1196 = t4 * (t1162 + t321 / 0.2E1 - dy * ((t307 - t312) * t124 
     #/ 0.2E1 - (t321 - t1188) * t124 / 0.2E1) / 0.8E1)
        t1198 = (t923 - t1021) * t124
        t1203 = (t975 - t910) * t124
        t1205 = (t910 - t1018) * t124
        t1207 = t1203 / 0.2E1 + t1205 / 0.2E1
        t1209 = t128 * t1207
        t1211 = t1179 / 0.2E1 + t1198 / 0.2E1
        t1213 = t143 * t1211
        t1215 = (t1209 - t1213) * t108
        t1216 = t1215 / 0.2E1
        t1218 = (t979 - t1) * t124
        t1220 = (t1 - t1022) * t124
        t1222 = t1218 / 0.2E1 + t1220 / 0.2E1
        t1224 = t161 * t1222
        t1226 = (t1213 - t1224) * t108
        t1227 = t1226 / 0.2E1
        t1228 = t934 / 0.2E1
        t1229 = u(t5,t121,t174,n)
        t1231 = (t1229 - t978) * t177
        t1232 = u(t5,t121,t179,n)
        t1234 = (t978 - t1232) * t177
        t1236 = t1231 / 0.2E1 + t1234 / 0.2E1
        t1238 = t333 * t1236
        t1240 = t342 * t930
        t1242 = (t1238 - t1240) * t124
        t1243 = t1242 / 0.2E1
        t1244 = u(t5,t126,t174,n)
        t1246 = (t1244 - t1021) * t177
        t1247 = u(t5,t126,t179,n)
        t1249 = (t1021 - t1247) * t177
        t1251 = t1246 / 0.2E1 + t1249 / 0.2E1
        t1253 = t356 * t1251
        t1255 = (t1240 - t1253) * t124
        t1256 = t1255 / 0.2E1
        t1258 = (t909 - t922) * t108
        t1260 = (t922 - t937) * t108
        t1262 = t1258 / 0.2E1 + t1260 / 0.2E1
        t1264 = t396 * t1262
        t1266 = t195 * t1094
        t1268 = (t1264 - t1266) * t177
        t1269 = t1268 / 0.2E1
        t1270 = t948 / 0.2E1
        t1271 = t1098 / 0.2E1
        t1272 = t1106 / 0.2E1
        t1277 = u(t5,t1047,t174,n)
        t1279 = (t1277 - t1077) * t177
        t1280 = u(t5,t1047,t179,n)
        t1282 = (t1077 - t1280) * t177
        t1284 = t1279 / 0.2E1 + t1282 / 0.2E1
        t1151 = t1071 * (t1049 * t1055 + t1051 * t1053 + t1058 * t1062)
        t1286 = t1151 * t1284
        t1288 = (t1286 - t1238) * t124
        t1292 = (t1242 - t1255) * t124
        t1299 = u(t5,t1111,t174,n)
        t1301 = (t1299 - t1141) * t177
        t1302 = u(t5,t1111,t179,n)
        t1304 = (t1141 - t1302) * t177
        t1306 = t1301 / 0.2E1 + t1304 / 0.2E1
        t1161 = t1135 * (t1113 * t1119 + t1115 * t1117 + t1122 * t1126)
        t1308 = t1161 * t1306
        t1310 = (t1253 - t1308) * t124
        t1320 = (t898 - t909) * t108
        t1325 = (t937 - t953) * t108
        t1328 = (t1258 / 0.2E1 - t1325 / 0.2E1) * t108
        t1335 = t195 * t964
        t1339 = (t902 - t913) * t108
        t1341 = (t926 - t940) * t108
        t1346 = (t913 - t926) * t108
        t1348 = (t940 - t957) * t108
        t1351 = (t1346 / 0.2E1 - t1348 / 0.2E1) * t108
        t1362 = -t868 * (((t921 - t934) * t108 - t950) * t108 / 0.2E1 + 
     #t969 / 0.2E1) / 0.6E1 - t868 * ((t251 * ((t977 / 0.2E1 - t981 / 0.
     #2E1) * t108 - t992) * t108 - t1014) * t124 / 0.2E1 + (t1014 - t292
     # * ((t1020 / 0.2E1 - t1024 / 0.2E1) * t108 - t1035) * t108) * t124
     # / 0.2E1) / 0.6E1 - t1046 * (((t1092 - t1098) * t124 - t1108) * t1
     #24 / 0.2E1 + (t1108 - (t1106 - t1152) * t124) * t124 / 0.2E1) / 0.
     #6E1 + (t1177 * t1179 - t1196 * t1198) * t124 + t1216 + t1227 + t12
     #28 + t1243 + t1256 + t1269 + t1270 + t1271 + t1272 - t1046 * (((t1
     #288 - t1242) * t124 - t1292) * t124 / 0.2E1 + (t1292 - (t1255 - t1
     #310) * t124) * t124 / 0.2E1) / 0.6E1 - t868 * ((t396 * ((t1320 / 0
     #.2E1 - t1260 / 0.2E1) * t108 - t1328) * t108 - t1335) * t177 / 0.2
     #E1 + (t1335 - t435 * ((t1339 / 0.2E1 - t1341 / 0.2E1) * t108 - t13
     #51) * t108) * t177 / 0.2E1) / 0.6E1
        t1363 = dz ** 2
        t1364 = k + 2
        t1365 = u(t5,j,t1364,n)
        t1367 = (t1365 - t922) * t177
        t1371 = (t925 - t928) * t177
        t1373 = ((t1367 - t925) * t177 - t1371) * t177
        t1375 = k - 2
        t1376 = u(t5,j,t1375,n)
        t1378 = (t926 - t1376) * t177
        t1382 = (t1371 - (t928 - t1378) * t177) * t177
        t1386 = rx(t5,j,t1364,0,0)
        t1387 = rx(t5,j,t1364,1,1)
        t1389 = rx(t5,j,t1364,2,2)
        t1391 = rx(t5,j,t1364,1,2)
        t1393 = rx(t5,j,t1364,2,1)
        t1395 = rx(t5,j,t1364,0,1)
        t1396 = rx(t5,j,t1364,1,0)
        t1400 = rx(t5,j,t1364,2,0)
        t1402 = rx(t5,j,t1364,0,2)
        t1407 = t1386 * t1387 * t1389 - t1386 * t1391 * t1393 - t1387 * 
     #t1400 * t1402 - t1389 * t1395 * t1396 + t1391 * t1395 * t1400 + t1
     #393 * t1396 * t1402
        t1408 = 0.1E1 / t1407
        t1409 = t1400 ** 2
        t1410 = t1393 ** 2
        t1411 = t1389 ** 2
        t1413 = t1408 * (t1409 + t1410 + t1411)
        t1416 = t4 * (t1413 / 0.2E1 + t484 / 0.2E1)
        t1417 = t1416 * t1367
        t1418 = t492 * t925
        t1420 = (t1417 - t1418) * t177
        t1421 = t501 * t928
        t1423 = (t1418 - t1421) * t177
        t1426 = rx(t5,j,t1375,0,0)
        t1427 = rx(t5,j,t1375,1,1)
        t1429 = rx(t5,j,t1375,2,2)
        t1431 = rx(t5,j,t1375,1,2)
        t1433 = rx(t5,j,t1375,2,1)
        t1435 = rx(t5,j,t1375,0,1)
        t1436 = rx(t5,j,t1375,1,0)
        t1440 = rx(t5,j,t1375,2,0)
        t1442 = rx(t5,j,t1375,0,2)
        t1447 = t1426 * t1427 * t1429 - t1426 * t1431 * t1433 - t1427 * 
     #t1440 * t1442 - t1429 * t1435 * t1436 + t1431 * t1435 * t1440 + t1
     #433 * t1436 * t1442
        t1448 = 0.1E1 / t1447
        t1449 = t1440 ** 2
        t1450 = t1433 ** 2
        t1451 = t1429 ** 2
        t1453 = t1448 * (t1449 + t1450 + t1451)
        t1456 = t4 * (t498 / 0.2E1 + t1453 / 0.2E1)
        t1457 = t1456 * t1378
        t1459 = (t1421 - t1457) * t177
        t1468 = t33 / 0.2E1
        t1469 = t870 ** 2
        t1470 = t879 ** 2
        t1471 = t886 ** 2
        t1472 = t1469 + t1470 + t1471
        t1473 = t892 * t1472
        t1477 = (t33 - t61) * t108
        t1483 = t4 * (t101 / 0.2E1 + t1468 - dx * ((t1473 - t101) * t108
     # / 0.2E1 - t1477 / 0.2E1) / 0.8E1)
        t1485 = t61 / 0.2E1
        t1489 = (t61 - t535) * t108
        t1495 = t4 * (t1468 + t1485 - dx * ((t101 - t33) * t108 / 0.2E1 
     #- t1489 / 0.2E1) / 0.8E1)
        t1496 = t1495 * t1000
        t1499 = t4 * t1408
        t1504 = u(t5,t121,t1364,n)
        t1506 = (t1504 - t1365) * t124
        t1507 = u(t5,t126,t1364,n)
        t1509 = (t1365 - t1507) * t124
        t1511 = t1506 / 0.2E1 + t1509 / 0.2E1
        t1430 = t1499 * (t1387 * t1393 + t1389 * t1391 + t1396 * t1400)
        t1513 = t1430 * t1511
        t1515 = (t1229 - t922) * t124
        t1517 = (t922 - t1244) * t124
        t1519 = t1515 / 0.2E1 + t1517 / 0.2E1
        t1521 = t450 * t1519
        t1523 = (t1513 - t1521) * t177
        t1525 = t342 * t1211
        t1527 = (t1521 - t1525) * t177
        t1531 = (t1232 - t926) * t124
        t1533 = (t926 - t1247) * t124
        t1535 = t1531 / 0.2E1 + t1533 / 0.2E1
        t1537 = t466 * t1535
        t1539 = (t1525 - t1537) * t177
        t1541 = (t1527 - t1539) * t177
        t1544 = t4 * t1448
        t1549 = u(t5,t121,t1375,n)
        t1551 = (t1549 - t1376) * t124
        t1552 = u(t5,t126,t1375,n)
        t1554 = (t1376 - t1552) * t124
        t1556 = t1551 / 0.2E1 + t1554 / 0.2E1
        t1462 = t1544 * (t1427 * t1433 + t1429 * t1431 + t1436 * t1440)
        t1558 = t1462 * t1556
        t1560 = (t1537 - t1558) * t177
        t1570 = t489 / 0.2E1
        t1580 = t4 * (t484 / 0.2E1 + t1570 - dz * ((t1413 - t484) * t177
     # / 0.2E1 - (t489 - t498) * t177 / 0.2E1) / 0.8E1)
        t1592 = t4 * (t1570 + t498 / 0.2E1 - dz * ((t484 - t489) * t177 
     #/ 0.2E1 - (t498 - t1453) * t177 / 0.2E1) / 0.8E1)
        t1596 = u(t73,j,t1364,n)
        t1598 = (t1596 - t909) * t177
        t1602 = u(t73,j,t1375,n)
        t1604 = (t913 - t1602) * t177
        t1614 = (t1367 / 0.2E1 - t928 / 0.2E1) * t177
        t1617 = (t925 / 0.2E1 - t1378 / 0.2E1) * t177
        t1505 = (t1614 - t1617) * t177
        t1621 = t195 * t1505
        t1624 = u(i,j,t1364,n)
        t1626 = (t1624 - t937) * t177
        t1629 = (t1626 / 0.2E1 - t942 / 0.2E1) * t177
        t1630 = u(i,j,t1375,n)
        t1632 = (t940 - t1630) * t177
        t1635 = (t939 / 0.2E1 - t1632 / 0.2E1) * t177
        t1520 = (t1629 - t1635) * t177
        t1639 = t212 * t1520
        t1641 = (t1621 - t1639) * t108
        t1651 = (t1596 - t1365) * t108
        t1653 = (t1365 - t1624) * t108
        t1655 = t1651 / 0.2E1 + t1653 / 0.2E1
        t1534 = t1499 * (t1386 * t1400 + t1389 * t1402 + t1393 * t1395)
        t1657 = t1534 * t1655
        t1659 = (t1657 - t1264) * t177
        t1663 = t1346 / 0.2E1 + t1341 / 0.2E1
        t1665 = t435 * t1663
        t1667 = (t1266 - t1665) * t177
        t1669 = (t1268 - t1667) * t177
        t1677 = (t1602 - t1376) * t108
        t1679 = (t1376 - t1630) * t108
        t1681 = t1677 / 0.2E1 + t1679 / 0.2E1
        t1555 = t1544 * (t1426 * t1440 + t1429 * t1442 + t1433 * t1435)
        t1683 = t1555 * t1681
        t1685 = (t1665 - t1683) * t177
        t1695 = (t1077 - t978) * t124
        t1699 = (t1179 - t1198) * t124
        t1701 = ((t1695 - t1179) * t124 - t1699) * t124
        t1704 = (t1021 - t1141) * t124
        t1708 = (t1699 - (t1198 - t1704) * t124) * t124
        t1714 = t4 * (t1167 / 0.2E1 + t307 / 0.2E1)
        t1715 = t1714 * t1695
        t1716 = t315 * t1179
        t1718 = (t1715 - t1716) * t124
        t1719 = t324 * t1198
        t1721 = (t1716 - t1719) * t124
        t1726 = t4 * (t321 / 0.2E1 + t1188 / 0.2E1)
        t1727 = t1726 * t1704
        t1729 = (t1719 - t1727) * t124
        t1738 = (t1504 - t1229) * t177
        t1743 = (t1232 - t1549) * t177
        t1753 = t342 * t1505
        t1757 = (t1507 - t1244) * t177
        t1762 = (t1247 - t1552) * t177
        t1781 = (t974 - t899) * t124
        t1783 = (t899 - t1017) * t124
        t1603 = t893 * (t870 * t880 + t871 * t879 + t875 * t886)
        t1789 = (t1603 * (t1781 / 0.2E1 + t1783 / 0.2E1) - t1209) * t108
        t1793 = (t1215 - t1226) * t108
        t1797 = (t987 - t954) * t124
        t1799 = (t954 - t1030) * t124
        t1801 = t1797 / 0.2E1 + t1799 / 0.2E1
        t1803 = t547 * t1801
        t1805 = (t1224 - t1803) * t108
        t1807 = (t1226 - t1805) * t108
        t1809 = (t1793 - t1807) * t108
        t1815 = (t1277 - t1229) * t124
        t1820 = (t1244 - t1299) * t124
        t1830 = (t1695 / 0.2E1 - t1198 / 0.2E1) * t124
        t1833 = (t1179 / 0.2E1 - t1704 / 0.2E1) * t124
        t1640 = (t1830 - t1833) * t124
        t1837 = t342 * t1640
        t1841 = (t1280 - t1232) * t124
        t1846 = (t1247 - t1302) * t124
        t1863 = (t1005 - t1000) * t108
        t1868 = (t1000 - t1007) * t108
        t1869 = t1863 - t1868
        t1870 = t1869 * t108
        t1871 = t64 * t1870
        t1876 = t4 * (t1473 / 0.2E1 + t101 / 0.2E1)
        t1878 = t104 * t1005
        t1880 = (t1876 * t998 - t1878) * t108
        t1881 = t64 * t1000
        t1883 = (t1878 - t1881) * t108
        t1886 = t538 * t1007
        t1888 = (t1881 - t1886) * t108
        t1889 = t1883 - t1888
        t1890 = t1889 * t108
        t1897 = (t1076 - t975) * t124
        t1902 = (t1018 - t1140) * t124
        t1912 = t143 * t1640
        t1916 = (t1080 - t979) * t124
        t1919 = (t1916 / 0.2E1 - t1220 / 0.2E1) * t124
        t1921 = (t1022 - t1144) * t124
        t1924 = (t1218 / 0.2E1 - t1921 / 0.2E1) * t124
        t1678 = (t1919 - t1924) * t124
        t1928 = t161 * t1678
        t1930 = (t1912 - t1928) * t108
        t1935 = t1667 / 0.2E1
        t1936 = t1527 / 0.2E1
        t1937 = t1539 / 0.2E1
        t1777 = ((t1738 / 0.2E1 - t1234 / 0.2E1) * t177 - (t1231 / 0.2E1
     # - t1743 / 0.2E1) * t177) * t177
        t1784 = ((t1757 / 0.2E1 - t1249 / 0.2E1) * t177 - (t1246 / 0.2E1
     # - t1762 / 0.2E1) * t177) * t177
        t1808 = t124 * ((t1815 / 0.2E1 - t1517 / 0.2E1) * t124 - (t1515 
     #/ 0.2E1 - t1820 / 0.2E1) * t124)
        t1813 = t124 * ((t1841 / 0.2E1 - t1533 / 0.2E1) * t124 - (t1531 
     #/ 0.2E1 - t1846 / 0.2E1) * t124)
        t1938 = -t1363 * ((t1373 * t492 - t1382 * t501) * t177 + ((t1420
     # - t1423) * t177 - (t1423 - t1459) * t177) * t177) / 0.24E2 + (t10
     #05 * t1483 - t1496) * t108 - t1363 * (((t1523 - t1527) * t177 - t1
     #541) * t177 / 0.2E1 + (t1541 - (t1539 - t1560) * t177) * t177 / 0.
     #2E1) / 0.6E1 + (t1580 * t925 - t1592 * t928) * t177 - t1363 * ((t1
     #81 * ((t1598 / 0.2E1 - t915 / 0.2E1) * t177 - (t912 / 0.2E1 - t160
     #4 / 0.2E1) * t177) * t177 - t1621) * t108 / 0.2E1 + t1641 / 0.2E1)
     # / 0.6E1 - t1363 * (((t1659 - t1268) * t177 - t1669) * t177 / 0.2E
     #1 + (t1669 - (t1667 - t1685) * t177) * t177 / 0.2E1) / 0.6E1 - t10
     #46 * ((t1701 * t315 - t1708 * t324) * t124 + ((t1718 - t1721) * t1
     #24 - (t1721 - t1729) * t124) * t124) / 0.24E2 - t1363 * ((t1777 * 
     #t333 - t1753) * t124 / 0.2E1 + (-t1784 * t356 + t1753) * t124 / 0.
     #2E1) / 0.6E1 - t868 * (((t1789 - t1215) * t108 - t1793) * t108 / 0
     #.2E1 + t1809 / 0.2E1) / 0.6E1 - t1046 * ((t1808 * t450 - t1837) * 
     #t177 / 0.2E1 + (-t1813 * t466 + t1837) * t177 / 0.2E1) / 0.6E1 - t
     #868 * ((t104 * ((t998 - t1005) * t108 - t1863) * t108 - t1871) * t
     #108 + ((t1880 - t1883) * t108 - t1890) * t108) / 0.24E2 - t1046 * 
     #((t128 * ((t1897 / 0.2E1 - t1205 / 0.2E1) * t124 - (t1203 / 0.2E1 
     #- t1902 / 0.2E1) * t124) * t124 - t1912) * t108 / 0.2E1 + t1930 / 
     #0.2E1) / 0.6E1 + t1935 + t1936 + t1937
        t1940 = t867 * (t1362 + t1938)
        t1942 = t865 * t1940 / 0.2E1
        t1943 = beta ** 2
        t1944 = t863 ** 2
        t1945 = t1943 * t1944
        t1946 = t71 * dx
        t1947 = sqrt(t100)
        t1948 = cc * t1947
        t1949 = ut(t869,j,k,n)
        t1951 = (t1949 - t105) * t108
        t1954 = (t1876 * t1951 - t110) * t108
        t1955 = ut(t869,t121,k,n)
        t1958 = ut(t869,t126,k,n)
        t1966 = (t1603 * ((t1955 - t1949) * t124 / 0.2E1 + (t1949 - t195
     #8) * t124 / 0.2E1) - t133) * t108
        t1968 = ut(t869,j,t174,n)
        t1971 = ut(t869,j,t179,n)
        t1979 = (t890 * ((t1968 - t1949) * t177 / 0.2E1 + (t1949 - t1971
     #) * t177 / 0.2E1) - t186) * t108
        t1981 = rx(t73,t121,k,0,0)
        t1982 = rx(t73,t121,k,1,1)
        t1984 = rx(t73,t121,k,2,2)
        t1986 = rx(t73,t121,k,1,2)
        t1988 = rx(t73,t121,k,2,1)
        t1990 = rx(t73,t121,k,0,1)
        t1991 = rx(t73,t121,k,1,0)
        t1995 = rx(t73,t121,k,2,0)
        t1997 = rx(t73,t121,k,0,2)
        t2002 = t1981 * t1982 * t1984 - t1981 * t1986 * t1988 - t1982 * 
     #t1995 * t1997 - t1984 * t1990 * t1991 + t1986 * t1990 * t1995 + t1
     #988 * t1991 * t1997
        t2003 = 0.1E1 / t2002
        t2004 = t4 * t2003
        t2010 = (t1955 - t122) * t108
        t2016 = t1951 / 0.2E1 + t109 / 0.2E1
        t2018 = t128 * t2016
        t2022 = rx(t73,t126,k,0,0)
        t2023 = rx(t73,t126,k,1,1)
        t2025 = rx(t73,t126,k,2,2)
        t2027 = rx(t73,t126,k,1,2)
        t2029 = rx(t73,t126,k,2,1)
        t2031 = rx(t73,t126,k,0,1)
        t2032 = rx(t73,t126,k,1,0)
        t2036 = rx(t73,t126,k,2,0)
        t2038 = rx(t73,t126,k,0,2)
        t2043 = t2022 * t2023 * t2025 - t2022 * t2027 * t2029 - t2023 * 
     #t2036 * t2038 - t2025 * t2031 * t2032 + t2027 * t2031 * t2036 + t2
     #029 * t2032 * t2038
        t2044 = 0.1E1 / t2043
        t2045 = t4 * t2044
        t2051 = (t1958 - t127) * t108
        t2059 = t1991 ** 2
        t2060 = t1982 ** 2
        t2061 = t1986 ** 2
        t2063 = t2003 * (t2059 + t2060 + t2061)
        t2064 = t84 ** 2
        t2065 = t75 ** 2
        t2066 = t79 ** 2
        t2068 = t96 * (t2064 + t2065 + t2066)
        t2071 = t4 * (t2063 / 0.2E1 + t2068 / 0.2E1)
        t2073 = t2032 ** 2
        t2074 = t2023 ** 2
        t2075 = t2027 ** 2
        t2077 = t2044 * (t2073 + t2074 + t2075)
        t2080 = t4 * (t2068 / 0.2E1 + t2077 / 0.2E1)
        t2088 = ut(t73,t121,t174,n)
        t2091 = ut(t73,t121,t179,n)
        t2095 = (t2088 - t122) * t177 / 0.2E1 + (t122 - t2091) * t177 / 
     #0.2E1
        t1976 = t116 * (t75 * t81 + t77 * t79 + t84 * t88)
        t2103 = t1976 * t184
        t2111 = ut(t73,t126,t174,n)
        t2114 = ut(t73,t126,t179,n)
        t2118 = (t2111 - t127) * t177 / 0.2E1 + (t127 - t2114) * t177 / 
     #0.2E1
        t2124 = rx(t73,j,t174,0,0)
        t2125 = rx(t73,j,t174,1,1)
        t2127 = rx(t73,j,t174,2,2)
        t2129 = rx(t73,j,t174,1,2)
        t2131 = rx(t73,j,t174,2,1)
        t2133 = rx(t73,j,t174,0,1)
        t2134 = rx(t73,j,t174,1,0)
        t2138 = rx(t73,j,t174,2,0)
        t2140 = rx(t73,j,t174,0,2)
        t2145 = t2124 * t2125 * t2127 - t2124 * t2129 * t2131 - t2125 * 
     #t2138 * t2140 - t2127 * t2133 * t2134 + t2129 * t2133 * t2138 + t2
     #131 * t2134 * t2140
        t2146 = 0.1E1 / t2145
        t2147 = t4 * t2146
        t2153 = (t1968 - t175) * t108
        t2159 = t181 * t2016
        t2163 = rx(t73,j,t179,0,0)
        t2164 = rx(t73,j,t179,1,1)
        t2166 = rx(t73,j,t179,2,2)
        t2168 = rx(t73,j,t179,1,2)
        t2170 = rx(t73,j,t179,2,1)
        t2172 = rx(t73,j,t179,0,1)
        t2173 = rx(t73,j,t179,1,0)
        t2177 = rx(t73,j,t179,2,0)
        t2179 = rx(t73,j,t179,0,2)
        t2184 = t2163 * t2164 * t2166 - t2163 * t2168 * t2170 - t2164 * 
     #t2177 * t2179 - t2166 * t2172 * t2173 + t2168 * t2172 * t2177 + t2
     #170 * t2173 * t2179
        t2185 = 0.1E1 / t2184
        t2186 = t4 * t2185
        t2192 = (t1971 - t180) * t108
        t2209 = (t2088 - t175) * t124 / 0.2E1 + (t175 - t2111) * t124 / 
     #0.2E1
        t2213 = t1976 * t131
        t2226 = (t2091 - t180) * t124 / 0.2E1 + (t180 - t2114) * t124 / 
     #0.2E1
        t2232 = t2138 ** 2
        t2233 = t2131 ** 2
        t2234 = t2127 ** 2
        t2236 = t2146 * (t2232 + t2233 + t2234)
        t2237 = t88 ** 2
        t2238 = t81 ** 2
        t2239 = t77 ** 2
        t2241 = t96 * (t2237 + t2238 + t2239)
        t2244 = t4 * (t2236 / 0.2E1 + t2241 / 0.2E1)
        t2246 = t2177 ** 2
        t2247 = t2170 ** 2
        t2248 = t2166 ** 2
        t2250 = t2185 * (t2246 + t2247 + t2248)
        t2253 = t4 * (t2241 / 0.2E1 + t2250 / 0.2E1)
        t2089 = t2004 * (t1981 * t1991 + t1982 * t1990 + t1986 * t1997)
        t2098 = t2045 * (t2022 * t2032 + t2023 * t2031 + t2027 * t2038)
        t2113 = t2004 * (t1982 * t1988 + t1984 * t1986 + t1991 * t1995)
        t2120 = t2045 * (t2023 * t2029 + t2025 * t2027 + t2032 * t2036)
        t2128 = t2147 * (t2124 * t2138 + t2127 * t2140 + t2131 * t2133)
        t2141 = t2186 * (t2163 * t2177 + t2166 * t2179 + t2170 * t2172)
        t2152 = t2147 * (t2125 * t2131 + t2127 * t2129 + t2134 * t2138)
        t2158 = t2186 * (t2164 * t2170 + t2166 * t2168 + t2173 * t2177)
        t2257 = t1954 + t1966 / 0.2E1 + t151 + t1979 / 0.2E1 + t203 + (t
     #2089 * (t2010 / 0.2E1 + t250 / 0.2E1) - t2018) * t124 / 0.2E1 + (t
     #2018 - t2098 * (t2051 / 0.2E1 + t293 / 0.2E1)) * t124 / 0.2E1 + (t
     #125 * t2071 - t129 * t2080) * t124 + (t2095 * t2113 - t2103) * t12
     #4 / 0.2E1 + (-t2118 * t2120 + t2103) * t124 / 0.2E1 + (t2128 * (t2
     #153 / 0.2E1 + t397 / 0.2E1) - t2159) * t177 / 0.2E1 + (t2159 - t21
     #41 * (t2192 / 0.2E1 + t438 / 0.2E1)) * t177 / 0.2E1 + (t2152 * t22
     #09 - t2213) * t177 / 0.2E1 + (-t2158 * t2226 + t2213) * t177 / 0.2
     #E1 + (t178 * t2244 - t182 * t2253) * t177
        t2259 = t867 * t505
        t2262 = sqrt(t60)
        t2263 = cc * t2262
        t2264 = t2263 * t856
        t2266 = (t2259 - t2264) * t108
        t2269 = t1946 * ((t1948 * t2257 - t2259) * t108 / 0.2E1 + t2266 
     #/ 0.2E1)
        t2271 = t1945 * t2269 / 0.8E1
        t2273 = t1883 + t1216 + t1227 + t1228 + t1270 + t1271 + t1272 + 
     #t1721 + t1243 + t1256 + t1269 + t1935 + t1936 + t1937 + t1423
        t2274 = t2273 * t27
        t2275 = t1805 / 0.2E1
        t2276 = t965 / 0.2E1
        t2278 = t981 / 0.2E1 + t989 / 0.2E1
        t2280 = t604 * t2278
        t2282 = t1000 / 0.2E1 + t1007 / 0.2E1
        t2284 = t161 * t2282
        t2286 = (t2280 - t2284) * t124
        t2287 = t2286 / 0.2E1
        t2289 = t1024 / 0.2E1 + t1032 / 0.2E1
        t2291 = t641 * t2289
        t2293 = (t2284 - t2291) * t124
        t2294 = t2293 / 0.2E1
        t2295 = t670 * t1218
        t2296 = t679 * t1220
        t2298 = (t2295 - t2296) * t124
        t2299 = u(i,t121,t174,n)
        t2301 = (t2299 - t979) * t177
        t2302 = u(i,t121,t179,n)
        t2304 = (t979 - t2302) * t177
        t2306 = t2301 / 0.2E1 + t2304 / 0.2E1
        t2308 = t683 * t2306
        t2310 = t691 * t944
        t2312 = (t2308 - t2310) * t124
        t2313 = t2312 / 0.2E1
        t2314 = u(i,t126,t174,n)
        t2316 = (t2314 - t1022) * t177
        t2317 = u(i,t126,t179,n)
        t2319 = (t1022 - t2317) * t177
        t2321 = t2316 / 0.2E1 + t2319 / 0.2E1
        t2323 = t706 * t2321
        t2325 = (t2310 - t2323) * t124
        t2326 = t2325 / 0.2E1
        t2328 = t1260 / 0.2E1 + t1325 / 0.2E1
        t2330 = t743 * t2328
        t2332 = t212 * t2282
        t2334 = (t2330 - t2332) * t177
        t2335 = t2334 / 0.2E1
        t2337 = t1341 / 0.2E1 + t1348 / 0.2E1
        t2339 = t781 * t2337
        t2341 = (t2332 - t2339) * t177
        t2342 = t2341 / 0.2E1
        t2344 = (t2299 - t937) * t124
        t2346 = (t937 - t2314) * t124
        t2348 = t2344 / 0.2E1 + t2346 / 0.2E1
        t2350 = t796 * t2348
        t2352 = t691 * t1222
        t2354 = (t2350 - t2352) * t177
        t2355 = t2354 / 0.2E1
        t2357 = (t2302 - t940) * t124
        t2359 = (t940 - t2317) * t124
        t2361 = t2357 / 0.2E1 + t2359 / 0.2E1
        t2363 = t811 * t2361
        t2365 = (t2352 - t2363) * t177
        t2366 = t2365 / 0.2E1
        t2367 = t843 * t939
        t2368 = t852 * t942
        t2370 = (t2367 - t2368) * t177
        t2371 = t1888 + t1227 + t2275 + t1270 + t2276 + t2287 + t2294 + 
     #t2298 + t2313 + t2326 + t2335 + t2342 + t2355 + t2366 + t2370
        t2372 = t2371 * t55
        t2373 = t2274 - t2372
        t2375 = t71 * t2373 * t108
        t2378 = beta * t67
        t2379 = dt * dx
        t2380 = t867 * t2273
        t2381 = t2263 * t2371
        t2383 = (t2380 - t2381) * t108
        t2384 = sqrt(t534)
        t2385 = cc * t2384
        t2386 = i - 2
        t2387 = rx(t2386,j,k,0,0)
        t2388 = rx(t2386,j,k,1,1)
        t2390 = rx(t2386,j,k,2,2)
        t2392 = rx(t2386,j,k,1,2)
        t2394 = rx(t2386,j,k,2,1)
        t2396 = rx(t2386,j,k,0,1)
        t2397 = rx(t2386,j,k,1,0)
        t2401 = rx(t2386,j,k,2,0)
        t2403 = rx(t2386,j,k,0,2)
        t2408 = t2387 * t2388 * t2390 - t2387 * t2392 * t2394 - t2388 * 
     #t2401 * t2403 - t2390 * t2396 * t2397 + t2392 * t2396 * t2401 + t2
     #394 * t2397 * t2403
        t2409 = 0.1E1 / t2408
        t2410 = t2387 ** 2
        t2411 = t2396 ** 2
        t2412 = t2403 ** 2
        t2413 = t2410 + t2411 + t2412
        t2414 = t2409 * t2413
        t2417 = t4 * (t535 / 0.2E1 + t2414 / 0.2E1)
        t2418 = u(t2386,j,k,n)
        t2420 = (t954 - t2418) * t108
        t2421 = t2417 * t2420
        t2423 = (t1886 - t2421) * t108
        t2424 = t4 * t2409
        t2428 = t2387 * t2397 + t2388 * t2396 + t2392 * t2403
        t2429 = u(t2386,t121,k,n)
        t2431 = (t2429 - t2418) * t124
        t2432 = u(t2386,t126,k,n)
        t2434 = (t2418 - t2432) * t124
        t2436 = t2431 / 0.2E1 + t2434 / 0.2E1
        t2261 = t2424 * t2428
        t2438 = t2261 * t2436
        t2440 = (t1803 - t2438) * t108
        t2441 = t2440 / 0.2E1
        t2445 = t2387 * t2401 + t2390 * t2403 + t2394 * t2396
        t2446 = u(t2386,j,t174,n)
        t2448 = (t2446 - t2418) * t177
        t2449 = u(t2386,j,t179,n)
        t2451 = (t2418 - t2449) * t177
        t2453 = t2448 / 0.2E1 + t2451 / 0.2E1
        t2281 = t2424 * t2445
        t2455 = t2281 * t2453
        t2457 = (t963 - t2455) * t108
        t2458 = t2457 / 0.2E1
        t2459 = rx(t507,t121,k,0,0)
        t2460 = rx(t507,t121,k,1,1)
        t2462 = rx(t507,t121,k,2,2)
        t2464 = rx(t507,t121,k,1,2)
        t2466 = rx(t507,t121,k,2,1)
        t2468 = rx(t507,t121,k,0,1)
        t2469 = rx(t507,t121,k,1,0)
        t2473 = rx(t507,t121,k,2,0)
        t2475 = rx(t507,t121,k,0,2)
        t2480 = t2459 * t2460 * t2462 - t2459 * t2464 * t2466 - t2460 * 
     #t2473 * t2475 - t2462 * t2468 * t2469 + t2464 * t2468 * t2473 + t2
     #466 * t2469 * t2475
        t2481 = 0.1E1 / t2480
        t2482 = t4 * t2481
        t2488 = (t987 - t2429) * t108
        t2490 = t989 / 0.2E1 + t2488 / 0.2E1
        t2329 = t2482 * (t2459 * t2469 + t2460 * t2468 + t2464 * t2475)
        t2492 = t2329 * t2490
        t2494 = t1007 / 0.2E1 + t2420 / 0.2E1
        t2496 = t547 * t2494
        t2498 = (t2492 - t2496) * t124
        t2499 = t2498 / 0.2E1
        t2500 = rx(t507,t126,k,0,0)
        t2501 = rx(t507,t126,k,1,1)
        t2503 = rx(t507,t126,k,2,2)
        t2505 = rx(t507,t126,k,1,2)
        t2507 = rx(t507,t126,k,2,1)
        t2509 = rx(t507,t126,k,0,1)
        t2510 = rx(t507,t126,k,1,0)
        t2514 = rx(t507,t126,k,2,0)
        t2516 = rx(t507,t126,k,0,2)
        t2521 = t2500 * t2501 * t2503 - t2500 * t2505 * t2507 - t2501 * 
     #t2514 * t2516 - t2503 * t2509 * t2510 + t2505 * t2509 * t2514 + t2
     #507 * t2510 * t2516
        t2522 = 0.1E1 / t2521
        t2523 = t4 * t2522
        t2529 = (t1030 - t2432) * t108
        t2531 = t1032 / 0.2E1 + t2529 / 0.2E1
        t2377 = t2523 * (t2500 * t2510 + t2501 * t2509 + t2505 * t2516)
        t2533 = t2377 * t2531
        t2535 = (t2496 - t2533) * t124
        t2536 = t2535 / 0.2E1
        t2537 = t2469 ** 2
        t2538 = t2460 ** 2
        t2539 = t2464 ** 2
        t2541 = t2481 * (t2537 + t2538 + t2539)
        t2542 = t518 ** 2
        t2543 = t509 ** 2
        t2544 = t513 ** 2
        t2546 = t530 * (t2542 + t2543 + t2544)
        t2549 = t4 * (t2541 / 0.2E1 + t2546 / 0.2E1)
        t2550 = t2549 * t1797
        t2551 = t2510 ** 2
        t2552 = t2501 ** 2
        t2553 = t2505 ** 2
        t2555 = t2522 * (t2551 + t2552 + t2553)
        t2558 = t4 * (t2546 / 0.2E1 + t2555 / 0.2E1)
        t2559 = t2558 * t1799
        t2561 = (t2550 - t2559) * t124
        t2565 = t2460 * t2466 + t2462 * t2464 + t2469 * t2473
        t2566 = u(t507,t121,t174,n)
        t2568 = (t2566 - t987) * t177
        t2569 = u(t507,t121,t179,n)
        t2571 = (t987 - t2569) * t177
        t2573 = t2568 / 0.2E1 + t2571 / 0.2E1
        t2419 = t2482 * t2565
        t2575 = t2419 * t2573
        t2427 = t545 * (t509 * t515 + t511 * t513 + t518 * t522)
        t2581 = t2427 * t961
        t2583 = (t2575 - t2581) * t124
        t2584 = t2583 / 0.2E1
        t2588 = t2501 * t2507 + t2503 * t2505 + t2510 * t2514
        t2589 = u(t507,t126,t174,n)
        t2591 = (t2589 - t1030) * t177
        t2592 = u(t507,t126,t179,n)
        t2594 = (t1030 - t2592) * t177
        t2596 = t2591 / 0.2E1 + t2594 / 0.2E1
        t2444 = t2523 * t2588
        t2598 = t2444 * t2596
        t2600 = (t2581 - t2598) * t124
        t2601 = t2600 / 0.2E1
        t2602 = rx(t507,j,t174,0,0)
        t2603 = rx(t507,j,t174,1,1)
        t2605 = rx(t507,j,t174,2,2)
        t2607 = rx(t507,j,t174,1,2)
        t2609 = rx(t507,j,t174,2,1)
        t2611 = rx(t507,j,t174,0,1)
        t2612 = rx(t507,j,t174,1,0)
        t2616 = rx(t507,j,t174,2,0)
        t2618 = rx(t507,j,t174,0,2)
        t2623 = t2602 * t2603 * t2605 - t2602 * t2607 * t2609 - t2603 * 
     #t2616 * t2618 - t2605 * t2611 * t2612 + t2607 * t2611 * t2616 + t2
     #609 * t2612 * t2618
        t2624 = 0.1E1 / t2623
        t2625 = t4 * t2624
        t2631 = (t953 - t2446) * t108
        t2633 = t1325 / 0.2E1 + t2631 / 0.2E1
        t2484 = t2625 * (t2602 * t2616 + t2605 * t2618 + t2609 * t2611)
        t2635 = t2484 * t2633
        t2637 = t564 * t2494
        t2639 = (t2635 - t2637) * t177
        t2640 = t2639 / 0.2E1
        t2641 = rx(t507,j,t179,0,0)
        t2642 = rx(t507,j,t179,1,1)
        t2644 = rx(t507,j,t179,2,2)
        t2646 = rx(t507,j,t179,1,2)
        t2648 = rx(t507,j,t179,2,1)
        t2650 = rx(t507,j,t179,0,1)
        t2651 = rx(t507,j,t179,1,0)
        t2655 = rx(t507,j,t179,2,0)
        t2657 = rx(t507,j,t179,0,2)
        t2662 = t2641 * t2642 * t2644 - t2641 * t2646 * t2648 - t2642 * 
     #t2655 * t2657 - t2644 * t2650 * t2651 + t2646 * t2650 * t2655 + t2
     #648 * t2651 * t2657
        t2663 = 0.1E1 / t2662
        t2664 = t4 * t2663
        t2670 = (t957 - t2449) * t108
        t2672 = t1348 / 0.2E1 + t2670 / 0.2E1
        t2520 = t2664 * (t2641 * t2655 + t2644 * t2657 + t2648 * t2650)
        t2674 = t2520 * t2672
        t2676 = (t2637 - t2674) * t177
        t2677 = t2676 / 0.2E1
        t2681 = t2603 * t2609 + t2605 * t2607 + t2612 * t2616
        t2683 = (t2566 - t953) * t124
        t2685 = (t953 - t2589) * t124
        t2687 = t2683 / 0.2E1 + t2685 / 0.2E1
        t2540 = t2625 * t2681
        t2689 = t2540 * t2687
        t2691 = t2427 * t1801
        t2693 = (t2689 - t2691) * t177
        t2694 = t2693 / 0.2E1
        t2698 = t2642 * t2648 + t2644 * t2646 + t2651 * t2655
        t2700 = (t2569 - t957) * t124
        t2702 = (t957 - t2592) * t124
        t2704 = t2700 / 0.2E1 + t2702 / 0.2E1
        t2562 = t2664 * t2698
        t2706 = t2562 * t2704
        t2708 = (t2691 - t2706) * t177
        t2709 = t2708 / 0.2E1
        t2710 = t2616 ** 2
        t2711 = t2609 ** 2
        t2712 = t2605 ** 2
        t2714 = t2624 * (t2710 + t2711 + t2712)
        t2715 = t522 ** 2
        t2716 = t515 ** 2
        t2717 = t511 ** 2
        t2719 = t530 * (t2715 + t2716 + t2717)
        t2722 = t4 * (t2714 / 0.2E1 + t2719 / 0.2E1)
        t2723 = t2722 * t956
        t2724 = t2655 ** 2
        t2725 = t2648 ** 2
        t2726 = t2644 ** 2
        t2728 = t2663 * (t2724 + t2725 + t2726)
        t2731 = t4 * (t2719 / 0.2E1 + t2728 / 0.2E1)
        t2732 = t2731 * t959
        t2734 = (t2723 - t2732) * t177
        t2735 = t2423 + t2275 + t2441 + t2276 + t2458 + t2499 + t2536 + 
     #t2561 + t2584 + t2601 + t2640 + t2677 + t2694 + t2709 + t2734
        t2736 = t2385 * t2735
        t2738 = (t2381 - t2736) * t108
        t2741 = t2379 * (t2383 / 0.2E1 + t2738 / 0.2E1)
        t2743 = t2378 * t2741 / 0.4E1
        t2745 = t864 * t2741 / 0.4E1
        t2746 = t67 * dt
        t2747 = t115 - t544
        t2748 = dx * t2747
        t2751 = t2378 * dt
        t2753 = (t965 - t2457) * t108
        t2755 = (t967 - t2753) * t108
        t2762 = (t1260 / 0.2E1 - t2631 / 0.2E1) * t108
        t2599 = (t1328 - t2762) * t108
        t2766 = t743 * t2599
        t2769 = (t1000 / 0.2E1 - t2420 / 0.2E1) * t108
        t2608 = (t1010 - t2769) * t108
        t2773 = t212 * t2608
        t2775 = (t2766 - t2773) * t177
        t2778 = (t1341 / 0.2E1 - t2670 / 0.2E1) * t108
        t2615 = (t1351 - t2778) * t108
        t2782 = t781 * t2615
        t2784 = (t2773 - t2782) * t177
        t2790 = (t1916 - t1218) * t124
        t2792 = (t1218 - t1220) * t124
        t2793 = t2790 - t2792
        t2794 = t2793 * t124
        t2795 = t670 * t2794
        t2797 = (t1220 - t1921) * t124
        t2798 = t2792 - t2797
        t2799 = t2798 * t124
        t2800 = t679 * t2799
        t2803 = rx(i,t1047,k,0,0)
        t2804 = rx(i,t1047,k,1,1)
        t2806 = rx(i,t1047,k,2,2)
        t2808 = rx(i,t1047,k,1,2)
        t2810 = rx(i,t1047,k,2,1)
        t2812 = rx(i,t1047,k,0,1)
        t2813 = rx(i,t1047,k,1,0)
        t2817 = rx(i,t1047,k,2,0)
        t2819 = rx(i,t1047,k,0,2)
        t2824 = t2803 * t2804 * t2806 - t2803 * t2808 * t2810 - t2804 * 
     #t2817 * t2819 - t2806 * t2812 * t2813 + t2808 * t2812 * t2817 + t2
     #810 * t2813 * t2819
        t2825 = 0.1E1 / t2824
        t2826 = t2813 ** 2
        t2827 = t2804 ** 2
        t2828 = t2808 ** 2
        t2829 = t2826 + t2827 + t2828
        t2830 = t2825 * t2829
        t2833 = t4 * (t2830 / 0.2E1 + t662 / 0.2E1)
        t2834 = t2833 * t1916
        t2836 = (t2834 - t2295) * t124
        t2837 = t2836 - t2298
        t2838 = t2837 * t124
        t2839 = rx(i,t1111,k,0,0)
        t2840 = rx(i,t1111,k,1,1)
        t2842 = rx(i,t1111,k,2,2)
        t2844 = rx(i,t1111,k,1,2)
        t2846 = rx(i,t1111,k,2,1)
        t2848 = rx(i,t1111,k,0,1)
        t2849 = rx(i,t1111,k,1,0)
        t2853 = rx(i,t1111,k,2,0)
        t2855 = rx(i,t1111,k,0,2)
        t2860 = t2839 * t2840 * t2842 - t2839 * t2844 * t2846 - t2840 * 
     #t2853 * t2855 - t2842 * t2848 * t2849 + t2844 * t2848 * t2853 + t2
     #846 * t2849 * t2855
        t2861 = 0.1E1 / t2860
        t2862 = t2849 ** 2
        t2863 = t2840 ** 2
        t2864 = t2844 ** 2
        t2865 = t2862 + t2863 + t2864
        t2866 = t2861 * t2865
        t2869 = t4 * (t676 / 0.2E1 + t2866 / 0.2E1)
        t2870 = t2869 * t1921
        t2872 = (t2296 - t2870) * t124
        t2873 = t2298 - t2872
        t2874 = t2873 * t124
        t2880 = t4 * t2825
        t2884 = t2804 * t2810 + t2806 * t2808 + t2813 * t2817
        t2885 = u(i,t1047,t174,n)
        t2887 = (t2885 - t1080) * t177
        t2888 = u(i,t1047,t179,n)
        t2890 = (t1080 - t2888) * t177
        t2892 = t2887 / 0.2E1 + t2890 / 0.2E1
        t2692 = t2880 * t2884
        t2894 = t2692 * t2892
        t2896 = (t2894 - t2308) * t124
        t2898 = (t2896 - t2312) * t124
        t2900 = (t2312 - t2325) * t124
        t2902 = (t2898 - t2900) * t124
        t2903 = t4 * t2861
        t2907 = t2840 * t2846 + t2842 * t2844 + t2849 * t2853
        t2908 = u(i,t1111,t174,n)
        t2910 = (t2908 - t1144) * t177
        t2911 = u(i,t1111,t179,n)
        t2913 = (t1144 - t2911) * t177
        t2915 = t2910 / 0.2E1 + t2913 / 0.2E1
        t2720 = t2903 * t2907
        t2917 = t2720 * t2915
        t2919 = (t2323 - t2917) * t124
        t2921 = (t2325 - t2919) * t124
        t2923 = (t2900 - t2921) * t124
        t2929 = (t1805 - t2440) * t108
        t2931 = (t1807 - t2929) * t108
        t2936 = u(i,t121,t1364,n)
        t2938 = (t2936 - t2299) * t177
        t2941 = (t2938 / 0.2E1 - t2304 / 0.2E1) * t177
        t2942 = u(i,t121,t1375,n)
        t2944 = (t2302 - t2942) * t177
        t2947 = (t2301 / 0.2E1 - t2944 / 0.2E1) * t177
        t2750 = (t2941 - t2947) * t177
        t2951 = t683 * t2750
        t2954 = t691 * t1520
        t2956 = (t2951 - t2954) * t124
        t2957 = u(i,t126,t1364,n)
        t2959 = (t2957 - t2314) * t177
        t2962 = (t2959 / 0.2E1 - t2319 / 0.2E1) * t177
        t2963 = u(i,t126,t1375,n)
        t2965 = (t2317 - t2963) * t177
        t2968 = (t2316 / 0.2E1 - t2965 / 0.2E1) * t177
        t2761 = (t2962 - t2968) * t177
        t2972 = t706 * t2761
        t2974 = (t2954 - t2972) * t124
        t2980 = (t2885 - t2299) * t124
        t2983 = (t2980 / 0.2E1 - t2346 / 0.2E1) * t124
        t2985 = (t2314 - t2908) * t124
        t2988 = (t2344 / 0.2E1 - t2985 / 0.2E1) * t124
        t2776 = (t2983 - t2988) * t124
        t2992 = t796 * t2776
        t2995 = t691 * t1678
        t2997 = (t2992 - t2995) * t177
        t2999 = (t2888 - t2302) * t124
        t3002 = (t2999 / 0.2E1 - t2359 / 0.2E1) * t124
        t3004 = (t2317 - t2911) * t124
        t3007 = (t2357 / 0.2E1 - t3004 / 0.2E1) * t124
        t2788 = (t3002 - t3007) * t124
        t3011 = t811 * t2788
        t3013 = (t2995 - t3011) * t177
        t3019 = (t1626 - t939) * t177
        t3021 = (t939 - t942) * t177
        t3022 = t3019 - t3021
        t3023 = t3022 * t177
        t3024 = t843 * t3023
        t3026 = (t942 - t1632) * t177
        t3027 = t3021 - t3026
        t3028 = t3027 * t177
        t3029 = t852 * t3028
        t3032 = rx(i,j,t1364,0,0)
        t3033 = rx(i,j,t1364,1,1)
        t3035 = rx(i,j,t1364,2,2)
        t3037 = rx(i,j,t1364,1,2)
        t3039 = rx(i,j,t1364,2,1)
        t3041 = rx(i,j,t1364,0,1)
        t3042 = rx(i,j,t1364,1,0)
        t3046 = rx(i,j,t1364,2,0)
        t3048 = rx(i,j,t1364,0,2)
        t3053 = t3032 * t3033 * t3035 - t3032 * t3037 * t3039 - t3033 * 
     #t3046 * t3048 - t3035 * t3041 * t3042 + t3037 * t3041 * t3046 + t3
     #039 * t3042 * t3048
        t3054 = 0.1E1 / t3053
        t3055 = t3046 ** 2
        t3056 = t3039 ** 2
        t3057 = t3035 ** 2
        t3058 = t3055 + t3056 + t3057
        t3059 = t3054 * t3058
        t3062 = t4 * (t3059 / 0.2E1 + t835 / 0.2E1)
        t3063 = t3062 * t1626
        t3065 = (t3063 - t2367) * t177
        t3066 = t3065 - t2370
        t3067 = t3066 * t177
        t3068 = rx(i,j,t1375,0,0)
        t3069 = rx(i,j,t1375,1,1)
        t3071 = rx(i,j,t1375,2,2)
        t3073 = rx(i,j,t1375,1,2)
        t3075 = rx(i,j,t1375,2,1)
        t3077 = rx(i,j,t1375,0,1)
        t3078 = rx(i,j,t1375,1,0)
        t3082 = rx(i,j,t1375,2,0)
        t3084 = rx(i,j,t1375,0,2)
        t3089 = t3068 * t3069 * t3071 - t3068 * t3073 * t3075 - t3069 * 
     #t3082 * t3084 - t3071 * t3077 * t3078 + t3073 * t3077 * t3082 + t3
     #075 * t3078 * t3084
        t3090 = 0.1E1 / t3089
        t3091 = t3082 ** 2
        t3092 = t3075 ** 2
        t3093 = t3071 ** 2
        t3094 = t3091 + t3092 + t3093
        t3095 = t3090 * t3094
        t3098 = t4 * (t849 / 0.2E1 + t3095 / 0.2E1)
        t3099 = t3098 * t1632
        t3101 = (t2368 - t3099) * t177
        t3102 = t2370 - t3101
        t3103 = t3102 * t177
        t3109 = -t868 * (t969 / 0.2E1 + t2755 / 0.2E1) / 0.6E1 - t868 * 
     #(t2775 / 0.2E1 + t2784 / 0.2E1) / 0.6E1 - t1046 * ((t2795 - t2800)
     # * t124 + (t2838 - t2874) * t124) / 0.24E2 - t1046 * (t2902 / 0.2E
     #1 + t2923 / 0.2E1) / 0.6E1 + t2275 - t868 * (t1809 / 0.2E1 + t2931
     # / 0.2E1) / 0.6E1 - t1363 * (t2956 / 0.2E1 + t2974 / 0.2E1) / 0.6E
     #1 + t1227 - t1046 * (t2997 / 0.2E1 + t3013 / 0.2E1) / 0.6E1 - t136
     #3 * ((t3024 - t3029) * t177 + (t3067 - t3103) * t177) / 0.24E2 + t
     #2313 + t2326 + t2335 + t2342 + t1270
        t3110 = t4 * t3054
        t3114 = t3032 * t3046 + t3035 * t3048 + t3039 * t3041
        t3115 = u(t507,j,t1364,n)
        t3117 = (t1624 - t3115) * t108
        t3119 = t1653 / 0.2E1 + t3117 / 0.2E1
        t2949 = t3110 * t3114
        t3121 = t2949 * t3119
        t3123 = (t3121 - t2330) * t177
        t3125 = (t3123 - t2334) * t177
        t3127 = (t2334 - t2341) * t177
        t3129 = (t3125 - t3127) * t177
        t3130 = t4 * t3090
        t3134 = t3068 * t3082 + t3071 * t3084 + t3075 * t3077
        t3135 = u(t507,j,t1375,n)
        t3137 = (t1630 - t3135) * t108
        t3139 = t1679 / 0.2E1 + t3137 / 0.2E1
        t2967 = t3130 * t3134
        t3141 = t2967 * t3139
        t3143 = (t2339 - t3141) * t177
        t3145 = (t2341 - t3143) * t177
        t3147 = (t3127 - t3145) * t177
        t3153 = (t1007 - t2420) * t108
        t3154 = t1868 - t3153
        t3155 = t3154 * t108
        t3156 = t538 * t3155
        t3159 = t1888 - t2423
        t3160 = t3159 * t108
        t3166 = u(t507,t1047,k,n)
        t3168 = (t3166 - t987) * t124
        t3171 = (t3168 / 0.2E1 - t1799 / 0.2E1) * t124
        t3172 = u(t507,t1111,k,n)
        t3174 = (t1030 - t3172) * t124
        t3177 = (t1797 / 0.2E1 - t3174 / 0.2E1) * t124
        t2984 = (t3171 - t3177) * t124
        t3181 = t547 * t2984
        t3183 = (t1928 - t3181) * t108
        t3189 = (t3115 - t953) * t177
        t3192 = (t3189 / 0.2E1 - t959 / 0.2E1) * t177
        t3194 = (t957 - t3135) * t177
        t3197 = (t956 / 0.2E1 - t3194 / 0.2E1) * t177
        t2998 = (t3192 - t3197) * t177
        t3201 = t564 * t2998
        t3203 = (t1639 - t3201) * t108
        t3211 = t2803 * t2813 + t2804 * t2812 + t2808 * t2819
        t3213 = (t1080 - t3166) * t108
        t3215 = t1082 / 0.2E1 + t3213 / 0.2E1
        t3010 = t2880 * t3211
        t3217 = t3010 * t3215
        t3219 = (t3217 - t2280) * t124
        t3221 = (t3219 - t2286) * t124
        t3223 = (t2286 - t2293) * t124
        t3225 = (t3221 - t3223) * t124
        t3229 = t2839 * t2849 + t2840 * t2848 + t2844 * t2855
        t3231 = (t1144 - t3172) * t108
        t3233 = t1146 / 0.2E1 + t3231 / 0.2E1
        t3031 = t2903 * t3229
        t3235 = t3031 * t3233
        t3237 = (t2291 - t3235) * t124
        t3239 = (t2293 - t3237) * t124
        t3241 = (t3223 - t3239) * t124
        t3246 = t662 / 0.2E1
        t3247 = t667 / 0.2E1
        t3251 = (t667 - t676) * t124
        t3257 = t4 * (t3246 + t3247 - dy * ((t2830 - t662) * t124 / 0.2E
     #1 - t3251 / 0.2E1) / 0.8E1)
        t3258 = t3257 * t1218
        t3259 = t676 / 0.2E1
        t3261 = (t662 - t667) * t124
        t3269 = t4 * (t3247 + t3259 - dy * (t3261 / 0.2E1 - (t676 - t286
     #6) * t124 / 0.2E1) / 0.8E1)
        t3270 = t3269 * t1220
        t3275 = (t981 / 0.2E1 - t2488 / 0.2E1) * t108
        t3081 = (t992 - t3275) * t108
        t3279 = t604 * t3081
        t3282 = t161 * t2608
        t3284 = (t3279 - t3282) * t124
        t3287 = (t1024 / 0.2E1 - t2529 / 0.2E1) * t108
        t3087 = (t1035 - t3287) * t108
        t3291 = t641 * t3087
        t3293 = (t3282 - t3291) * t124
        t3298 = t535 / 0.2E1
        t3306 = t4 * (t1485 + t3298 - dx * (t1477 / 0.2E1 - (t535 - t241
     #4) * t108 / 0.2E1) / 0.8E1)
        t3307 = t3306 * t1007
        t3313 = t3033 * t3039 + t3035 * t3037 + t3042 * t3046
        t3315 = (t2936 - t1624) * t124
        t3317 = (t1624 - t2957) * t124
        t3319 = t3315 / 0.2E1 + t3317 / 0.2E1
        t3120 = t3110 * t3313
        t3321 = t3120 * t3319
        t3323 = (t3321 - t2350) * t177
        t3325 = (t3323 - t2354) * t177
        t3327 = (t2354 - t2365) * t177
        t3329 = (t3325 - t3327) * t177
        t3333 = t3069 * t3075 + t3071 * t3073 + t3078 * t3082
        t3335 = (t2942 - t1630) * t124
        t3337 = (t1630 - t2963) * t124
        t3339 = t3335 / 0.2E1 + t3337 / 0.2E1
        t3142 = t3130 * t3333
        t3341 = t3142 * t3339
        t3343 = (t2363 - t3341) * t177
        t3345 = (t2365 - t3343) * t177
        t3347 = (t3327 - t3345) * t177
        t3352 = t835 / 0.2E1
        t3353 = t840 / 0.2E1
        t3357 = (t840 - t849) * t177
        t3363 = t4 * (t3352 + t3353 - dz * ((t3059 - t835) * t177 / 0.2E
     #1 - t3357 / 0.2E1) / 0.8E1)
        t3364 = t3363 * t939
        t3365 = t849 / 0.2E1
        t3367 = (t835 - t840) * t177
        t3375 = t4 * (t3353 + t3365 - dz * (t3367 / 0.2E1 - (t849 - t309
     #5) * t177 / 0.2E1) / 0.8E1)
        t3376 = t3375 * t942
        t3379 = -t1363 * (t3129 / 0.2E1 + t3147 / 0.2E1) / 0.6E1 - t868 
     #* ((t1871 - t3156) * t108 + (t1890 - t3160) * t108) / 0.24E2 - t10
     #46 * (t1930 / 0.2E1 + t3183 / 0.2E1) / 0.6E1 - t1363 * (t1641 / 0.
     #2E1 + t3203 / 0.2E1) / 0.6E1 + t2355 + t2366 - t1046 * (t3225 / 0.
     #2E1 + t3241 / 0.2E1) / 0.6E1 + (t3258 - t3270) * t124 + t2276 + t2
     #287 + t2294 - t868 * (t3284 / 0.2E1 + t3293 / 0.2E1) / 0.6E1 + (t1
     #496 - t3307) * t108 - t1363 * (t3329 / 0.2E1 + t3347 / 0.2E1) / 0.
     #6E1 + (t3364 - t3376) * t177
        t3380 = t3109 + t3379
        t3381 = t2263 * t3380
        t3383 = t2751 * t3381 / 0.2E1
        t3384 = t1943 * beta
        t3386 = t3384 * t69 * t72
        t3390 = t977 / 0.2E1 + t986 / 0.2E1
        t3392 = t2089 * t3390
        t3394 = t998 / 0.2E1 + t1005 / 0.2E1
        t3396 = t128 * t3394
        t3399 = (t3392 - t3396) * t124 / 0.2E1
        t3401 = t1020 / 0.2E1 + t1029 / 0.2E1
        t3403 = t2098 * t3401
        t3406 = (t3396 - t3403) * t124 / 0.2E1
        t3407 = t2071 * t1203
        t3408 = t2080 * t1205
        t3411 = u(t73,t121,t174,n)
        t3413 = (t3411 - t975) * t177
        t3414 = u(t73,t121,t179,n)
        t3416 = (t975 - t3414) * t177
        t3418 = t3413 / 0.2E1 + t3416 / 0.2E1
        t3420 = t2113 * t3418
        t3422 = t1976 * t917
        t3425 = (t3420 - t3422) * t124 / 0.2E1
        t3426 = u(t73,t126,t174,n)
        t3428 = (t3426 - t1018) * t177
        t3429 = u(t73,t126,t179,n)
        t3431 = (t1018 - t3429) * t177
        t3433 = t3428 / 0.2E1 + t3431 / 0.2E1
        t3435 = t2120 * t3433
        t3438 = (t3422 - t3435) * t124 / 0.2E1
        t3440 = t1320 / 0.2E1 + t1258 / 0.2E1
        t3442 = t2128 * t3440
        t3444 = t181 * t3394
        t3447 = (t3442 - t3444) * t177 / 0.2E1
        t3449 = t1339 / 0.2E1 + t1346 / 0.2E1
        t3451 = t2141 * t3449
        t3454 = (t3444 - t3451) * t177 / 0.2E1
        t3456 = (t3411 - t909) * t124
        t3458 = (t909 - t3426) * t124
        t3460 = t3456 / 0.2E1 + t3458 / 0.2E1
        t3462 = t2152 * t3460
        t3464 = t1976 * t1207
        t3467 = (t3462 - t3464) * t177 / 0.2E1
        t3469 = (t3414 - t913) * t124
        t3471 = (t913 - t3429) * t124
        t3473 = t3469 / 0.2E1 + t3471 / 0.2E1
        t3475 = t2158 * t3473
        t3478 = (t3464 - t3475) * t177 / 0.2E1
        t3479 = t2244 * t912
        t3480 = t2253 * t915
        t3483 = t1880 + t1789 / 0.2E1 + t1216 + t921 / 0.2E1 + t1228 + t
     #3399 + t3406 + (t3407 - t3408) * t124 + t3425 + t3438 + t3447 + t3
     #454 + t3467 + t3478 + (t3479 - t3480) * t177
        t3484 = t3483 * t95
        t3486 = (t3484 - t2274) * t108
        t3488 = t2373 * t108
        t3489 = t64 * t3488
        t3492 = rx(t869,t121,k,0,0)
        t3493 = rx(t869,t121,k,1,1)
        t3495 = rx(t869,t121,k,2,2)
        t3497 = rx(t869,t121,k,1,2)
        t3499 = rx(t869,t121,k,2,1)
        t3501 = rx(t869,t121,k,0,1)
        t3502 = rx(t869,t121,k,1,0)
        t3506 = rx(t869,t121,k,2,0)
        t3508 = rx(t869,t121,k,0,2)
        t3514 = 0.1E1 / (t3492 * t3493 * t3495 - t3492 * t3497 * t3499 -
     # t3493 * t3506 * t3508 - t3495 * t3501 * t3502 + t3497 * t3501 * t
     #3506 + t3499 * t3502 * t3508)
        t3515 = t3492 ** 2
        t3516 = t3501 ** 2
        t3517 = t3508 ** 2
        t3520 = t1981 ** 2
        t3521 = t1990 ** 2
        t3522 = t1997 ** 2
        t3524 = t2003 * (t3520 + t3521 + t3522)
        t3529 = t221 ** 2
        t3530 = t230 ** 2
        t3531 = t237 ** 2
        t3533 = t243 * (t3529 + t3530 + t3531)
        t3536 = t4 * (t3524 / 0.2E1 + t3533 / 0.2E1)
        t3537 = t3536 * t986
        t3540 = t4 * t3514
        t3545 = u(t869,t1047,k,n)
        t3553 = t1897 / 0.2E1 + t1203 / 0.2E1
        t3555 = t2089 * t3553
        t3560 = t1695 / 0.2E1 + t1179 / 0.2E1
        t3562 = t251 * t3560
        t3564 = (t3555 - t3562) * t108
        t3565 = t3564 / 0.2E1
        t3570 = u(t869,t121,t174,n)
        t3573 = u(t869,t121,t179,n)
        t3583 = t1981 * t1995 + t1984 * t1997 + t1988 * t1990
        t3336 = t2004 * t3583
        t3585 = t3336 * t3418
        t3592 = t221 * t235 + t224 * t237 + t228 * t230
        t3344 = t244 * t3592
        t3594 = t3344 * t1236
        t3596 = (t3585 - t3594) * t108
        t3597 = t3596 / 0.2E1
        t3598 = rx(t73,t1047,k,0,0)
        t3599 = rx(t73,t1047,k,1,1)
        t3601 = rx(t73,t1047,k,2,2)
        t3603 = rx(t73,t1047,k,1,2)
        t3605 = rx(t73,t1047,k,2,1)
        t3607 = rx(t73,t1047,k,0,1)
        t3608 = rx(t73,t1047,k,1,0)
        t3612 = rx(t73,t1047,k,2,0)
        t3614 = rx(t73,t1047,k,0,2)
        t3620 = 0.1E1 / (t3598 * t3599 * t3601 - t3598 * t3603 * t3605 -
     # t3599 * t3612 * t3614 - t3601 * t3607 * t3608 + t3603 * t3607 * t
     #3612 + t3605 * t3608 * t3614)
        t3621 = t4 * t3620
        t3635 = t3608 ** 2
        t3636 = t3599 ** 2
        t3637 = t3603 ** 2
        t3650 = u(t73,t1047,t174,n)
        t3653 = u(t73,t1047,t179,n)
        t3657 = (t3650 - t1076) * t177 / 0.2E1 + (t1076 - t3653) * t177 
     #/ 0.2E1
        t3663 = rx(t73,t121,t174,0,0)
        t3664 = rx(t73,t121,t174,1,1)
        t3666 = rx(t73,t121,t174,2,2)
        t3668 = rx(t73,t121,t174,1,2)
        t3670 = rx(t73,t121,t174,2,1)
        t3672 = rx(t73,t121,t174,0,1)
        t3673 = rx(t73,t121,t174,1,0)
        t3677 = rx(t73,t121,t174,2,0)
        t3679 = rx(t73,t121,t174,0,2)
        t3685 = 0.1E1 / (t3663 * t3664 * t3666 - t3663 * t3668 * t3670 -
     # t3664 * t3677 * t3679 - t3666 * t3672 * t3673 + t3668 * t3672 * t
     #3677 + t3670 * t3673 * t3679)
        t3686 = t4 * t3685
        t3694 = (t3411 - t1229) * t108
        t3696 = (t3570 - t3411) * t108 / 0.2E1 + t3694 / 0.2E1
        t3700 = t3336 * t3390
        t3704 = rx(t73,t121,t179,0,0)
        t3705 = rx(t73,t121,t179,1,1)
        t3707 = rx(t73,t121,t179,2,2)
        t3709 = rx(t73,t121,t179,1,2)
        t3711 = rx(t73,t121,t179,2,1)
        t3713 = rx(t73,t121,t179,0,1)
        t3714 = rx(t73,t121,t179,1,0)
        t3718 = rx(t73,t121,t179,2,0)
        t3720 = rx(t73,t121,t179,0,2)
        t3726 = 0.1E1 / (t3704 * t3705 * t3707 - t3704 * t3709 * t3711 -
     # t3705 * t3718 * t3720 - t3707 * t3713 * t3714 + t3709 * t3713 * t
     #3718 + t3711 * t3714 * t3720)
        t3727 = t4 * t3726
        t3735 = (t3414 - t1232) * t108
        t3737 = (t3573 - t3414) * t108 / 0.2E1 + t3735 / 0.2E1
        t3750 = (t3650 - t3411) * t124 / 0.2E1 + t3456 / 0.2E1
        t3754 = t2113 * t3553
        t3765 = (t3653 - t3414) * t124 / 0.2E1 + t3469 / 0.2E1
        t3771 = t3677 ** 2
        t3772 = t3670 ** 2
        t3773 = t3666 ** 2
        t3776 = t1995 ** 2
        t3777 = t1988 ** 2
        t3778 = t1984 ** 2
        t3780 = t2003 * (t3776 + t3777 + t3778)
        t3785 = t3718 ** 2
        t3786 = t3711 ** 2
        t3787 = t3707 ** 2
        t3548 = t3621 * (t3598 * t3608 + t3599 * t3607 + t3603 * t3614)
        t3582 = t3686 * (t3663 * t3677 + t3666 * t3679 + t3670 * t3672)
        t3589 = t3727 * (t3704 * t3718 + t3707 * t3720 + t3711 * t3713)
        t3600 = t3686 * (t3664 * t3670 + t3666 * t3668 + t3673 * t3677)
        t3610 = t3727 * (t3705 * t3711 + t3707 * t3709 + t3714 * t3718)
        t3796 = (t4 * (t3514 * (t3515 + t3516 + t3517) / 0.2E1 + t3524 /
     # 0.2E1) * t977 - t3537) * t108 + (t3540 * (t3492 * t3502 + t3493 *
     # t3501 + t3497 * t3508) * ((t3545 - t974) * t124 / 0.2E1 + t1781 /
     # 0.2E1) - t3555) * t108 / 0.2E1 + t3565 + (t3540 * (t3492 * t3506 
     #+ t3495 * t3508 + t3499 * t3501) * ((t3570 - t974) * t177 / 0.2E1 
     #+ (t974 - t3573) * t177 / 0.2E1) - t3585) * t108 / 0.2E1 + t3597 +
     # (t3548 * ((t3545 - t1076) * t108 / 0.2E1 + t1079 / 0.2E1) - t3392
     #) * t124 / 0.2E1 + t3399 + (t4 * (t3620 * (t3635 + t3636 + t3637) 
     #/ 0.2E1 + t2063 / 0.2E1) * t1897 - t3407) * t124 + (t3621 * (t3599
     # * t3605 + t3601 * t3603 + t3608 * t3612) * t3657 - t3420) * t124 
     #/ 0.2E1 + t3425 + (t3582 * t3696 - t3700) * t177 / 0.2E1 + (-t3589
     # * t3737 + t3700) * t177 / 0.2E1 + (t3600 * t3750 - t3754) * t177 
     #/ 0.2E1 + (-t3610 * t3765 + t3754) * t177 / 0.2E1 + (t4 * (t3685 *
     # (t3771 + t3772 + t3773) / 0.2E1 + t3780 / 0.2E1) * t3413 - t4 * (
     #t3780 / 0.2E1 + t3726 * (t3785 + t3786 + t3787) / 0.2E1) * t3416) 
     #* t177
        t3797 = t3796 * t2002
        t3800 = rx(t869,t126,k,0,0)
        t3801 = rx(t869,t126,k,1,1)
        t3803 = rx(t869,t126,k,2,2)
        t3805 = rx(t869,t126,k,1,2)
        t3807 = rx(t869,t126,k,2,1)
        t3809 = rx(t869,t126,k,0,1)
        t3810 = rx(t869,t126,k,1,0)
        t3814 = rx(t869,t126,k,2,0)
        t3816 = rx(t869,t126,k,0,2)
        t3822 = 0.1E1 / (t3800 * t3801 * t3803 - t3800 * t3805 * t3807 -
     # t3801 * t3814 * t3816 - t3803 * t3809 * t3810 + t3805 * t3809 * t
     #3814 + t3807 * t3810 * t3816)
        t3823 = t3800 ** 2
        t3824 = t3809 ** 2
        t3825 = t3816 ** 2
        t3828 = t2022 ** 2
        t3829 = t2031 ** 2
        t3830 = t2038 ** 2
        t3832 = t2044 * (t3828 + t3829 + t3830)
        t3837 = t264 ** 2
        t3838 = t273 ** 2
        t3839 = t280 ** 2
        t3841 = t286 * (t3837 + t3838 + t3839)
        t3844 = t4 * (t3832 / 0.2E1 + t3841 / 0.2E1)
        t3845 = t3844 * t1029
        t3848 = t4 * t3822
        t3853 = u(t869,t1111,k,n)
        t3861 = t1205 / 0.2E1 + t1902 / 0.2E1
        t3863 = t2098 * t3861
        t3868 = t1198 / 0.2E1 + t1704 / 0.2E1
        t3870 = t292 * t3868
        t3872 = (t3863 - t3870) * t108
        t3873 = t3872 / 0.2E1
        t3878 = u(t869,t126,t174,n)
        t3881 = u(t869,t126,t179,n)
        t3891 = t2022 * t2036 + t2025 * t2038 + t2029 * t2031
        t3665 = t2045 * t3891
        t3893 = t3665 * t3433
        t3900 = t264 * t278 + t267 * t280 + t271 * t273
        t3674 = t287 * t3900
        t3902 = t3674 * t1251
        t3904 = (t3893 - t3902) * t108
        t3905 = t3904 / 0.2E1
        t3906 = rx(t73,t1111,k,0,0)
        t3907 = rx(t73,t1111,k,1,1)
        t3909 = rx(t73,t1111,k,2,2)
        t3911 = rx(t73,t1111,k,1,2)
        t3913 = rx(t73,t1111,k,2,1)
        t3915 = rx(t73,t1111,k,0,1)
        t3916 = rx(t73,t1111,k,1,0)
        t3920 = rx(t73,t1111,k,2,0)
        t3922 = rx(t73,t1111,k,0,2)
        t3928 = 0.1E1 / (t3906 * t3907 * t3909 - t3906 * t3911 * t3913 -
     # t3907 * t3920 * t3922 - t3909 * t3915 * t3916 + t3911 * t3915 * t
     #3920 + t3913 * t3916 * t3922)
        t3929 = t4 * t3928
        t3943 = t3916 ** 2
        t3944 = t3907 ** 2
        t3945 = t3911 ** 2
        t3958 = u(t73,t1111,t174,n)
        t3961 = u(t73,t1111,t179,n)
        t3965 = (t3958 - t1140) * t177 / 0.2E1 + (t1140 - t3961) * t177 
     #/ 0.2E1
        t3971 = rx(t73,t126,t174,0,0)
        t3972 = rx(t73,t126,t174,1,1)
        t3974 = rx(t73,t126,t174,2,2)
        t3976 = rx(t73,t126,t174,1,2)
        t3978 = rx(t73,t126,t174,2,1)
        t3980 = rx(t73,t126,t174,0,1)
        t3981 = rx(t73,t126,t174,1,0)
        t3985 = rx(t73,t126,t174,2,0)
        t3987 = rx(t73,t126,t174,0,2)
        t3993 = 0.1E1 / (t3971 * t3972 * t3974 - t3971 * t3976 * t3978 -
     # t3972 * t3985 * t3987 - t3974 * t3980 * t3981 + t3976 * t3980 * t
     #3985 + t3978 * t3981 * t3987)
        t3994 = t4 * t3993
        t4002 = (t3426 - t1244) * t108
        t4004 = (t3878 - t3426) * t108 / 0.2E1 + t4002 / 0.2E1
        t4008 = t3665 * t3401
        t4012 = rx(t73,t126,t179,0,0)
        t4013 = rx(t73,t126,t179,1,1)
        t4015 = rx(t73,t126,t179,2,2)
        t4017 = rx(t73,t126,t179,1,2)
        t4019 = rx(t73,t126,t179,2,1)
        t4021 = rx(t73,t126,t179,0,1)
        t4022 = rx(t73,t126,t179,1,0)
        t4026 = rx(t73,t126,t179,2,0)
        t4028 = rx(t73,t126,t179,0,2)
        t4034 = 0.1E1 / (t4012 * t4013 * t4015 - t4012 * t4017 * t4019 -
     # t4013 * t4026 * t4028 - t4015 * t4021 * t4022 + t4017 * t4021 * t
     #4026 + t4019 * t4022 * t4028)
        t4035 = t4 * t4034
        t4043 = (t3429 - t1247) * t108
        t4045 = (t3881 - t3429) * t108 / 0.2E1 + t4043 / 0.2E1
        t4058 = t3458 / 0.2E1 + (t3426 - t3958) * t124 / 0.2E1
        t4062 = t2120 * t3861
        t4073 = t3471 / 0.2E1 + (t3429 - t3961) * t124 / 0.2E1
        t4079 = t3985 ** 2
        t4080 = t3978 ** 2
        t4081 = t3974 ** 2
        t4084 = t2036 ** 2
        t4085 = t2029 ** 2
        t4086 = t2025 ** 2
        t4088 = t2044 * (t4084 + t4085 + t4086)
        t4093 = t4026 ** 2
        t4094 = t4019 ** 2
        t4095 = t4015 ** 2
        t3849 = t3929 * (t3906 * t3916 + t3907 * t3915 + t3911 * t3922)
        t3884 = t3994 * (t3971 * t3985 + t3974 * t3987 + t3978 * t3980)
        t3889 = t4035 * (t4012 * t4026 + t4015 * t4028 + t4019 * t4021)
        t3896 = t3994 * (t3972 * t3978 + t3974 * t3976 + t3981 * t3985)
        t3903 = t4035 * (t4013 * t4019 + t4015 * t4017 + t4022 * t4026)
        t4104 = (t4 * (t3822 * (t3823 + t3824 + t3825) / 0.2E1 + t3832 /
     # 0.2E1) * t1020 - t3845) * t108 + (t3848 * (t3800 * t3810 + t3801 
     #* t3809 + t3805 * t3816) * (t1783 / 0.2E1 + (t1017 - t3853) * t124
     # / 0.2E1) - t3863) * t108 / 0.2E1 + t3873 + (t3848 * (t3800 * t381
     #4 + t3803 * t3816 + t3807 * t3809) * ((t3878 - t1017) * t177 / 0.2
     #E1 + (t1017 - t3881) * t177 / 0.2E1) - t3893) * t108 / 0.2E1 + t39
     #05 + t3406 + (t3403 - t3849 * ((t3853 - t1140) * t108 / 0.2E1 + t1
     #143 / 0.2E1)) * t124 / 0.2E1 + (t3408 - t4 * (t2077 / 0.2E1 + t392
     #8 * (t3943 + t3944 + t3945) / 0.2E1) * t1902) * t124 + t3438 + (t3
     #435 - t3929 * (t3907 * t3913 + t3909 * t3911 + t3916 * t3920) * t3
     #965) * t124 / 0.2E1 + (t3884 * t4004 - t4008) * t177 / 0.2E1 + (-t
     #3889 * t4045 + t4008) * t177 / 0.2E1 + (t3896 * t4058 - t4062) * t
     #177 / 0.2E1 + (-t3903 * t4073 + t4062) * t177 / 0.2E1 + (t4 * (t39
     #93 * (t4079 + t4080 + t4081) / 0.2E1 + t4088 / 0.2E1) * t3428 - t4
     # * (t4088 / 0.2E1 + t4034 * (t4093 + t4094 + t4095) / 0.2E1) * t34
     #31) * t177
        t4105 = t4104 * t2043
        t4112 = t580 ** 2
        t4113 = t589 ** 2
        t4114 = t596 ** 2
        t4116 = t602 * (t4112 + t4113 + t4114)
        t4119 = t4 * (t3533 / 0.2E1 + t4116 / 0.2E1)
        t4120 = t4119 * t981
        t4122 = (t3537 - t4120) * t108
        t4124 = t1916 / 0.2E1 + t1218 / 0.2E1
        t4126 = t604 * t4124
        t4128 = (t3562 - t4126) * t108
        t4129 = t4128 / 0.2E1
        t3948 = t603 * (t580 * t594 + t583 * t596 + t587 * t589)
        t4135 = t3948 * t2306
        t4137 = (t3594 - t4135) * t108
        t4138 = t4137 / 0.2E1
        t4139 = t1092 / 0.2E1
        t4140 = t1288 / 0.2E1
        t4141 = rx(t5,t121,t174,0,0)
        t4142 = rx(t5,t121,t174,1,1)
        t4144 = rx(t5,t121,t174,2,2)
        t4146 = rx(t5,t121,t174,1,2)
        t4148 = rx(t5,t121,t174,2,1)
        t4150 = rx(t5,t121,t174,0,1)
        t4151 = rx(t5,t121,t174,1,0)
        t4155 = rx(t5,t121,t174,2,0)
        t4157 = rx(t5,t121,t174,0,2)
        t4162 = t4141 * t4142 * t4144 - t4141 * t4146 * t4148 - t4142 * 
     #t4155 * t4157 - t4144 * t4150 * t4151 + t4146 * t4150 * t4155 + t4
     #148 * t4151 * t4157
        t4163 = 0.1E1 / t4162
        t4164 = t4 * t4163
        t4170 = (t1229 - t2299) * t108
        t4172 = t3694 / 0.2E1 + t4170 / 0.2E1
        t3970 = t4164 * (t4141 * t4155 + t4144 * t4157 + t4148 * t4150)
        t4174 = t3970 * t4172
        t4176 = t3344 * t1088
        t4179 = (t4174 - t4176) * t177 / 0.2E1
        t4180 = rx(t5,t121,t179,0,0)
        t4181 = rx(t5,t121,t179,1,1)
        t4183 = rx(t5,t121,t179,2,2)
        t4185 = rx(t5,t121,t179,1,2)
        t4187 = rx(t5,t121,t179,2,1)
        t4189 = rx(t5,t121,t179,0,1)
        t4190 = rx(t5,t121,t179,1,0)
        t4194 = rx(t5,t121,t179,2,0)
        t4196 = rx(t5,t121,t179,0,2)
        t4201 = t4180 * t4181 * t4183 - t4180 * t4185 * t4187 - t4181 * 
     #t4194 * t4196 - t4183 * t4189 * t4190 + t4185 * t4189 * t4194 + t4
     #187 * t4190 * t4196
        t4202 = 0.1E1 / t4201
        t4203 = t4 * t4202
        t4207 = t4180 * t4194 + t4183 * t4196 + t4187 * t4189
        t4209 = (t1232 - t2302) * t108
        t4211 = t3735 / 0.2E1 + t4209 / 0.2E1
        t4003 = t4203 * t4207
        t4213 = t4003 * t4211
        t4216 = (t4176 - t4213) * t177 / 0.2E1
        t4222 = t1815 / 0.2E1 + t1515 / 0.2E1
        t4014 = t4164 * (t4142 * t4148 + t4144 * t4146 + t4151 * t4155)
        t4224 = t4014 * t4222
        t4226 = t333 * t3560
        t4229 = (t4224 - t4226) * t177 / 0.2E1
        t4233 = t4181 * t4187 + t4183 * t4185 + t4190 * t4194
        t4235 = t1841 / 0.2E1 + t1531 / 0.2E1
        t4027 = t4203 * t4233
        t4237 = t4027 * t4235
        t4240 = (t4226 - t4237) * t177 / 0.2E1
        t4241 = t4155 ** 2
        t4242 = t4148 ** 2
        t4243 = t4144 ** 2
        t4245 = t4163 * (t4241 + t4242 + t4243)
        t4246 = t235 ** 2
        t4247 = t228 ** 2
        t4248 = t224 ** 2
        t4250 = t243 * (t4246 + t4247 + t4248)
        t4253 = t4 * (t4245 / 0.2E1 + t4250 / 0.2E1)
        t4254 = t4253 * t1231
        t4255 = t4194 ** 2
        t4256 = t4187 ** 2
        t4257 = t4183 ** 2
        t4259 = t4202 * (t4255 + t4256 + t4257)
        t4262 = t4 * (t4250 / 0.2E1 + t4259 / 0.2E1)
        t4263 = t4262 * t1234
        t4266 = t4122 + t3565 + t4129 + t3597 + t4138 + t4139 + t1271 + 
     #t1718 + t4140 + t1243 + t4179 + t4216 + t4229 + t4240 + (t4254 - t
     #4263) * t177
        t4267 = t4266 * t242
        t4269 = (t4267 - t2274) * t124
        t4270 = t621 ** 2
        t4271 = t630 ** 2
        t4272 = t637 ** 2
        t4274 = t643 * (t4270 + t4271 + t4272)
        t4277 = t4 * (t3841 / 0.2E1 + t4274 / 0.2E1)
        t4278 = t4277 * t1024
        t4280 = (t3845 - t4278) * t108
        t4282 = t1220 / 0.2E1 + t1921 / 0.2E1
        t4284 = t641 * t4282
        t4286 = (t3870 - t4284) * t108
        t4287 = t4286 / 0.2E1
        t4056 = t644 * (t621 * t635 + t624 * t637 + t628 * t630)
        t4293 = t4056 * t2321
        t4295 = (t3902 - t4293) * t108
        t4296 = t4295 / 0.2E1
        t4297 = t1152 / 0.2E1
        t4298 = t1310 / 0.2E1
        t4299 = rx(t5,t126,t174,0,0)
        t4300 = rx(t5,t126,t174,1,1)
        t4302 = rx(t5,t126,t174,2,2)
        t4304 = rx(t5,t126,t174,1,2)
        t4306 = rx(t5,t126,t174,2,1)
        t4308 = rx(t5,t126,t174,0,1)
        t4309 = rx(t5,t126,t174,1,0)
        t4313 = rx(t5,t126,t174,2,0)
        t4315 = rx(t5,t126,t174,0,2)
        t4320 = t4299 * t4300 * t4302 - t4299 * t4304 * t4306 - t4300 * 
     #t4313 * t4315 - t4302 * t4308 * t4309 + t4304 * t4308 * t4313 + t4
     #306 * t4309 * t4315
        t4321 = 0.1E1 / t4320
        t4322 = t4 * t4321
        t4328 = (t1244 - t2314) * t108
        t4330 = t4002 / 0.2E1 + t4328 / 0.2E1
        t4082 = t4322 * (t4299 * t4313 + t4302 * t4315 + t4306 * t4308)
        t4332 = t4082 * t4330
        t4334 = t3674 * t1102
        t4337 = (t4332 - t4334) * t177 / 0.2E1
        t4338 = rx(t5,t126,t179,0,0)
        t4339 = rx(t5,t126,t179,1,1)
        t4341 = rx(t5,t126,t179,2,2)
        t4343 = rx(t5,t126,t179,1,2)
        t4345 = rx(t5,t126,t179,2,1)
        t4347 = rx(t5,t126,t179,0,1)
        t4348 = rx(t5,t126,t179,1,0)
        t4352 = rx(t5,t126,t179,2,0)
        t4354 = rx(t5,t126,t179,0,2)
        t4359 = t4338 * t4339 * t4341 - t4338 * t4343 * t4345 - t4339 * 
     #t4352 * t4354 - t4341 * t4347 * t4348 + t4343 * t4347 * t4352 + t4
     #345 * t4348 * t4354
        t4360 = 0.1E1 / t4359
        t4361 = t4 * t4360
        t4365 = t4338 * t4352 + t4341 * t4354 + t4345 * t4347
        t4367 = (t1247 - t2317) * t108
        t4369 = t4043 / 0.2E1 + t4367 / 0.2E1
        t4111 = t4361 * t4365
        t4371 = t4111 * t4369
        t4374 = (t4334 - t4371) * t177 / 0.2E1
        t4380 = t1517 / 0.2E1 + t1820 / 0.2E1
        t4127 = t4322 * (t4300 * t4306 + t4302 * t4304 + t4309 * t4313)
        t4382 = t4127 * t4380
        t4384 = t356 * t3868
        t4387 = (t4382 - t4384) * t177 / 0.2E1
        t4391 = t4339 * t4345 + t4341 * t4343 + t4348 * t4352
        t4393 = t1533 / 0.2E1 + t1846 / 0.2E1
        t4145 = t4361 * t4391
        t4395 = t4145 * t4393
        t4398 = (t4384 - t4395) * t177 / 0.2E1
        t4399 = t4313 ** 2
        t4400 = t4306 ** 2
        t4401 = t4302 ** 2
        t4403 = t4321 * (t4399 + t4400 + t4401)
        t4404 = t278 ** 2
        t4405 = t271 ** 2
        t4406 = t267 ** 2
        t4408 = t286 * (t4404 + t4405 + t4406)
        t4411 = t4 * (t4403 / 0.2E1 + t4408 / 0.2E1)
        t4412 = t4411 * t1246
        t4413 = t4352 ** 2
        t4414 = t4345 ** 2
        t4415 = t4341 ** 2
        t4417 = t4360 * (t4413 + t4414 + t4415)
        t4420 = t4 * (t4408 / 0.2E1 + t4417 / 0.2E1)
        t4421 = t4420 * t1249
        t4424 = t4280 + t3873 + t4287 + t3905 + t4296 + t1272 + t4297 + 
     #t1729 + t1256 + t4298 + t4337 + t4374 + t4387 + t4398 + (t4412 - t
     #4421) * t177
        t4425 = t4424 * t285
        t4427 = (t2274 - t4425) * t124
        t4429 = t4269 / 0.2E1 + t4427 / 0.2E1
        t4431 = t143 * t4429
        t4435 = t2459 ** 2
        t4436 = t2468 ** 2
        t4437 = t2475 ** 2
        t4439 = t2481 * (t4435 + t4436 + t4437)
        t4442 = t4 * (t4116 / 0.2E1 + t4439 / 0.2E1)
        t4443 = t4442 * t989
        t4445 = (t4120 - t4443) * t108
        t4447 = t3168 / 0.2E1 + t1797 / 0.2E1
        t4449 = t2329 * t4447
        t4451 = (t4126 - t4449) * t108
        t4452 = t4451 / 0.2E1
        t4456 = t2459 * t2473 + t2462 * t2475 + t2466 * t2468
        t4188 = t2482 * t4456
        t4458 = t4188 * t2573
        t4460 = (t4135 - t4458) * t108
        t4461 = t4460 / 0.2E1
        t4462 = t3219 / 0.2E1
        t4463 = t2896 / 0.2E1
        t4464 = rx(i,t121,t174,0,0)
        t4465 = rx(i,t121,t174,1,1)
        t4467 = rx(i,t121,t174,2,2)
        t4469 = rx(i,t121,t174,1,2)
        t4471 = rx(i,t121,t174,2,1)
        t4473 = rx(i,t121,t174,0,1)
        t4474 = rx(i,t121,t174,1,0)
        t4478 = rx(i,t121,t174,2,0)
        t4480 = rx(i,t121,t174,0,2)
        t4485 = t4464 * t4465 * t4467 - t4464 * t4469 * t4471 - t4465 * 
     #t4478 * t4480 - t4467 * t4473 * t4474 + t4469 * t4473 * t4478 + t4
     #471 * t4474 * t4480
        t4486 = 0.1E1 / t4485
        t4487 = t4 * t4486
        t4491 = t4464 * t4478 + t4467 * t4480 + t4471 * t4473
        t4493 = (t2299 - t2566) * t108
        t4495 = t4170 / 0.2E1 + t4493 / 0.2E1
        t4219 = t4487 * t4491
        t4497 = t4219 * t4495
        t4499 = t3948 * t2278
        t4501 = (t4497 - t4499) * t177
        t4502 = t4501 / 0.2E1
        t4503 = rx(i,t121,t179,0,0)
        t4504 = rx(i,t121,t179,1,1)
        t4506 = rx(i,t121,t179,2,2)
        t4508 = rx(i,t121,t179,1,2)
        t4510 = rx(i,t121,t179,2,1)
        t4512 = rx(i,t121,t179,0,1)
        t4513 = rx(i,t121,t179,1,0)
        t4517 = rx(i,t121,t179,2,0)
        t4519 = rx(i,t121,t179,0,2)
        t4524 = t4503 * t4504 * t4506 - t4503 * t4508 * t4510 - t4504 * 
     #t4517 * t4519 - t4506 * t4512 * t4513 + t4508 * t4512 * t4517 + t4
     #510 * t4513 * t4519
        t4525 = 0.1E1 / t4524
        t4526 = t4 * t4525
        t4530 = t4503 * t4517 + t4506 * t4519 + t4510 * t4512
        t4532 = (t2302 - t2569) * t108
        t4534 = t4209 / 0.2E1 + t4532 / 0.2E1
        t4261 = t4526 * t4530
        t4536 = t4261 * t4534
        t4538 = (t4499 - t4536) * t177
        t4539 = t4538 / 0.2E1
        t4545 = t2980 / 0.2E1 + t2344 / 0.2E1
        t4276 = t4487 * (t4465 * t4471 + t4467 * t4469 + t4474 * t4478)
        t4547 = t4276 * t4545
        t4549 = t683 * t4124
        t4551 = (t4547 - t4549) * t177
        t4552 = t4551 / 0.2E1
        t4558 = t2999 / 0.2E1 + t2357 / 0.2E1
        t4289 = t4526 * (t4504 * t4510 + t4506 * t4508 + t4513 * t4517)
        t4560 = t4289 * t4558
        t4562 = (t4549 - t4560) * t177
        t4563 = t4562 / 0.2E1
        t4564 = t4478 ** 2
        t4565 = t4471 ** 2
        t4566 = t4467 ** 2
        t4568 = t4486 * (t4564 + t4565 + t4566)
        t4569 = t594 ** 2
        t4570 = t587 ** 2
        t4571 = t583 ** 2
        t4573 = t602 * (t4569 + t4570 + t4571)
        t4576 = t4 * (t4568 / 0.2E1 + t4573 / 0.2E1)
        t4577 = t4576 * t2301
        t4578 = t4517 ** 2
        t4579 = t4510 ** 2
        t4580 = t4506 ** 2
        t4582 = t4525 * (t4578 + t4579 + t4580)
        t4585 = t4 * (t4573 / 0.2E1 + t4582 / 0.2E1)
        t4586 = t4585 * t2304
        t4588 = (t4577 - t4586) * t177
        t4589 = t4445 + t4129 + t4452 + t4138 + t4461 + t4462 + t2287 + 
     #t2836 + t4463 + t2313 + t4502 + t4539 + t4552 + t4563 + t4588
        t4590 = t4589 * t601
        t4591 = t4590 - t2372
        t4592 = t4591 * t124
        t4593 = t2500 ** 2
        t4594 = t2509 ** 2
        t4595 = t2516 ** 2
        t4597 = t2522 * (t4593 + t4594 + t4595)
        t4600 = t4 * (t4274 / 0.2E1 + t4597 / 0.2E1)
        t4601 = t4600 * t1032
        t4603 = (t4278 - t4601) * t108
        t4605 = t1799 / 0.2E1 + t3174 / 0.2E1
        t4607 = t2377 * t4605
        t4609 = (t4284 - t4607) * t108
        t4610 = t4609 / 0.2E1
        t4614 = t2500 * t2514 + t2503 * t2516 + t2507 * t2509
        t4327 = t2523 * t4614
        t4616 = t4327 * t2596
        t4618 = (t4293 - t4616) * t108
        t4619 = t4618 / 0.2E1
        t4620 = t3237 / 0.2E1
        t4621 = t2919 / 0.2E1
        t4622 = rx(i,t126,t174,0,0)
        t4623 = rx(i,t126,t174,1,1)
        t4625 = rx(i,t126,t174,2,2)
        t4627 = rx(i,t126,t174,1,2)
        t4629 = rx(i,t126,t174,2,1)
        t4631 = rx(i,t126,t174,0,1)
        t4632 = rx(i,t126,t174,1,0)
        t4636 = rx(i,t126,t174,2,0)
        t4638 = rx(i,t126,t174,0,2)
        t4643 = t4622 * t4623 * t4625 - t4622 * t4627 * t4629 - t4623 * 
     #t4636 * t4638 - t4625 * t4631 * t4632 + t4627 * t4631 * t4636 + t4
     #629 * t4632 * t4638
        t4644 = 0.1E1 / t4643
        t4645 = t4 * t4644
        t4649 = t4622 * t4636 + t4625 * t4638 + t4629 * t4631
        t4651 = (t2314 - t2589) * t108
        t4653 = t4328 / 0.2E1 + t4651 / 0.2E1
        t4363 = t4645 * t4649
        t4655 = t4363 * t4653
        t4657 = t4056 * t2289
        t4659 = (t4655 - t4657) * t177
        t4660 = t4659 / 0.2E1
        t4661 = rx(i,t126,t179,0,0)
        t4662 = rx(i,t126,t179,1,1)
        t4664 = rx(i,t126,t179,2,2)
        t4666 = rx(i,t126,t179,1,2)
        t4668 = rx(i,t126,t179,2,1)
        t4670 = rx(i,t126,t179,0,1)
        t4671 = rx(i,t126,t179,1,0)
        t4675 = rx(i,t126,t179,2,0)
        t4677 = rx(i,t126,t179,0,2)
        t4682 = t4661 * t4662 * t4664 - t4661 * t4666 * t4668 - t4662 * 
     #t4675 * t4677 - t4664 * t4670 * t4671 + t4666 * t4670 * t4675 + t4
     #668 * t4671 * t4677
        t4683 = 0.1E1 / t4682
        t4684 = t4 * t4683
        t4688 = t4661 * t4675 + t4664 * t4677 + t4668 * t4670
        t4690 = (t2317 - t2592) * t108
        t4692 = t4367 / 0.2E1 + t4690 / 0.2E1
        t4394 = t4684 * t4688
        t4694 = t4394 * t4692
        t4696 = (t4657 - t4694) * t177
        t4697 = t4696 / 0.2E1
        t4703 = t2346 / 0.2E1 + t2985 / 0.2E1
        t4410 = t4645 * (t4623 * t4629 + t4625 * t4627 + t4632 * t4636)
        t4705 = t4410 * t4703
        t4707 = t706 * t4282
        t4709 = (t4705 - t4707) * t177
        t4710 = t4709 / 0.2E1
        t4716 = t2359 / 0.2E1 + t3004 / 0.2E1
        t4426 = t4684 * (t4662 * t4668 + t4664 * t4666 + t4671 * t4675)
        t4718 = t4426 * t4716
        t4720 = (t4707 - t4718) * t177
        t4721 = t4720 / 0.2E1
        t4722 = t4636 ** 2
        t4723 = t4629 ** 2
        t4724 = t4625 ** 2
        t4726 = t4644 * (t4722 + t4723 + t4724)
        t4727 = t635 ** 2
        t4728 = t628 ** 2
        t4729 = t624 ** 2
        t4731 = t643 * (t4727 + t4728 + t4729)
        t4734 = t4 * (t4726 / 0.2E1 + t4731 / 0.2E1)
        t4735 = t4734 * t2316
        t4736 = t4675 ** 2
        t4737 = t4668 ** 2
        t4738 = t4664 ** 2
        t4740 = t4683 * (t4736 + t4737 + t4738)
        t4743 = t4 * (t4731 / 0.2E1 + t4740 / 0.2E1)
        t4744 = t4743 * t2319
        t4746 = (t4735 - t4744) * t177
        t4747 = t4603 + t4287 + t4610 + t4296 + t4619 + t2294 + t4620 + 
     #t2872 + t2326 + t4621 + t4660 + t4697 + t4710 + t4721 + t4746
        t4748 = t4747 * t642
        t4749 = t2372 - t4748
        t4750 = t4749 * t124
        t4752 = t4592 / 0.2E1 + t4750 / 0.2E1
        t4754 = t161 * t4752
        t4757 = (t4431 - t4754) * t108 / 0.2E1
        t4758 = rx(t869,j,t174,0,0)
        t4759 = rx(t869,j,t174,1,1)
        t4761 = rx(t869,j,t174,2,2)
        t4763 = rx(t869,j,t174,1,2)
        t4765 = rx(t869,j,t174,2,1)
        t4767 = rx(t869,j,t174,0,1)
        t4768 = rx(t869,j,t174,1,0)
        t4772 = rx(t869,j,t174,2,0)
        t4774 = rx(t869,j,t174,0,2)
        t4780 = 0.1E1 / (t4758 * t4759 * t4761 - t4758 * t4763 * t4765 -
     # t4759 * t4772 * t4774 - t4761 * t4767 * t4768 + t4763 * t4767 * t
     #4772 + t4765 * t4768 * t4774)
        t4781 = t4758 ** 2
        t4782 = t4767 ** 2
        t4783 = t4774 ** 2
        t4786 = t2124 ** 2
        t4787 = t2133 ** 2
        t4788 = t2140 ** 2
        t4790 = t2146 * (t4786 + t4787 + t4788)
        t4795 = t368 ** 2
        t4796 = t377 ** 2
        t4797 = t384 ** 2
        t4799 = t390 * (t4795 + t4796 + t4797)
        t4802 = t4 * (t4790 / 0.2E1 + t4799 / 0.2E1)
        t4803 = t4802 * t1258
        t4806 = t4 * t4780
        t4822 = t2124 * t2134 + t2125 * t2133 + t2129 * t2140
        t4492 = t2147 * t4822
        t4824 = t4492 * t3460
        t4831 = t368 * t378 + t369 * t377 + t373 * t384
        t4500 = t391 * t4831
        t4833 = t4500 * t1519
        t4835 = (t4824 - t4833) * t108
        t4836 = t4835 / 0.2E1
        t4841 = u(t869,j,t1364,n)
        t4849 = t1598 / 0.2E1 + t912 / 0.2E1
        t4851 = t2128 * t4849
        t4856 = t1367 / 0.2E1 + t925 / 0.2E1
        t4858 = t396 * t4856
        t4860 = (t4851 - t4858) * t108
        t4861 = t4860 / 0.2E1
        t4865 = t3663 * t3673 + t3664 * t3672 + t3668 * t3679
        t4869 = t4492 * t3440
        t4876 = t3971 * t3981 + t3972 * t3980 + t3976 * t3987
        t4882 = t3673 ** 2
        t4883 = t3664 ** 2
        t4884 = t3668 ** 2
        t4887 = t2134 ** 2
        t4888 = t2125 ** 2
        t4889 = t2129 ** 2
        t4891 = t2146 * (t4887 + t4888 + t4889)
        t4896 = t3981 ** 2
        t4897 = t3972 ** 2
        t4898 = t3976 ** 2
        t4907 = u(t73,t121,t1364,n)
        t4911 = (t4907 - t3411) * t177 / 0.2E1 + t3413 / 0.2E1
        t4915 = t2152 * t4849
        t4919 = u(t73,t126,t1364,n)
        t4923 = (t4919 - t3426) * t177 / 0.2E1 + t3428 / 0.2E1
        t4929 = rx(t73,j,t1364,0,0)
        t4930 = rx(t73,j,t1364,1,1)
        t4932 = rx(t73,j,t1364,2,2)
        t4934 = rx(t73,j,t1364,1,2)
        t4936 = rx(t73,j,t1364,2,1)
        t4938 = rx(t73,j,t1364,0,1)
        t4939 = rx(t73,j,t1364,1,0)
        t4943 = rx(t73,j,t1364,2,0)
        t4945 = rx(t73,j,t1364,0,2)
        t4951 = 0.1E1 / (t4929 * t4930 * t4932 - t4929 * t4934 * t4936 -
     # t4930 * t4943 * t4945 - t4932 * t4938 * t4939 + t4934 * t4938 * t
     #4943 + t4936 * t4939 * t4945)
        t4952 = t4 * t4951
        t4975 = (t4907 - t1596) * t124 / 0.2E1 + (t1596 - t4919) * t124 
     #/ 0.2E1
        t4981 = t4943 ** 2
        t4982 = t4936 ** 2
        t4983 = t4932 ** 2
        t4732 = t4952 * (t4929 * t4943 + t4932 * t4945 + t4936 * t4938)
        t4992 = (t4 * (t4780 * (t4781 + t4782 + t4783) / 0.2E1 + t4790 /
     # 0.2E1) * t1320 - t4803) * t108 + (t4806 * (t4758 * t4768 + t4759 
     #* t4767 + t4763 * t4774) * ((t3570 - t898) * t124 / 0.2E1 + (t898 
     #- t3878) * t124 / 0.2E1) - t4824) * t108 / 0.2E1 + t4836 + (t4806 
     #* (t4758 * t4772 + t4761 * t4774 + t4765 * t4767) * ((t4841 - t898
     #) * t177 / 0.2E1 + t901 / 0.2E1) - t4851) * t108 / 0.2E1 + t4861 +
     # (t3686 * t3696 * t4865 - t4869) * t124 / 0.2E1 + (-t3994 * t4004 
     #* t4876 + t4869) * t124 / 0.2E1 + (t4 * (t3685 * (t4882 + t4883 + 
     #t4884) / 0.2E1 + t4891 / 0.2E1) * t3456 - t4 * (t4891 / 0.2E1 + t3
     #993 * (t4896 + t4897 + t4898) / 0.2E1) * t3458) * t124 + (t3600 * 
     #t4911 - t4915) * t124 / 0.2E1 + (-t3896 * t4923 + t4915) * t124 / 
     #0.2E1 + (t4732 * ((t4841 - t1596) * t108 / 0.2E1 + t1651 / 0.2E1) 
     #- t3442) * t177 / 0.2E1 + t3447 + (t4952 * (t4930 * t4936 + t4932 
     #* t4934 + t4939 * t4943) * t4975 - t3462) * t177 / 0.2E1 + t3467 +
     # (t4 * (t4951 * (t4981 + t4982 + t4983) / 0.2E1 + t2236 / 0.2E1) *
     # t1598 - t3479) * t177
        t4993 = t4992 * t2145
        t4996 = rx(t869,j,t179,0,0)
        t4997 = rx(t869,j,t179,1,1)
        t4999 = rx(t869,j,t179,2,2)
        t5001 = rx(t869,j,t179,1,2)
        t5003 = rx(t869,j,t179,2,1)
        t5005 = rx(t869,j,t179,0,1)
        t5006 = rx(t869,j,t179,1,0)
        t5010 = rx(t869,j,t179,2,0)
        t5012 = rx(t869,j,t179,0,2)
        t5018 = 0.1E1 / (t4996 * t4997 * t4999 - t4996 * t5001 * t5003 -
     # t4997 * t5010 * t5012 - t4999 * t5005 * t5006 + t5001 * t5005 * t
     #5010 + t5003 * t5006 * t5012)
        t5019 = t4996 ** 2
        t5020 = t5005 ** 2
        t5021 = t5012 ** 2
        t5024 = t2163 ** 2
        t5025 = t2172 ** 2
        t5026 = t2179 ** 2
        t5028 = t2185 * (t5024 + t5025 + t5026)
        t5033 = t409 ** 2
        t5034 = t418 ** 2
        t5035 = t425 ** 2
        t5037 = t431 * (t5033 + t5034 + t5035)
        t5040 = t4 * (t5028 / 0.2E1 + t5037 / 0.2E1)
        t5041 = t5040 * t1346
        t5044 = t4 * t5018
        t5060 = t2163 * t2173 + t2164 * t2172 + t2168 * t2179
        t4818 = t2186 * t5060
        t5062 = t4818 * t3473
        t5069 = t409 * t419 + t410 * t418 + t414 * t425
        t4823 = t432 * t5069
        t5071 = t4823 * t1535
        t5073 = (t5062 - t5071) * t108
        t5074 = t5073 / 0.2E1
        t5079 = u(t869,j,t1375,n)
        t5087 = t915 / 0.2E1 + t1604 / 0.2E1
        t5089 = t2141 * t5087
        t5094 = t928 / 0.2E1 + t1378 / 0.2E1
        t5096 = t435 * t5094
        t5098 = (t5089 - t5096) * t108
        t5099 = t5098 / 0.2E1
        t5103 = t3704 * t3714 + t3705 * t3713 + t3709 * t3720
        t5107 = t4818 * t3449
        t5114 = t4012 * t4022 + t4013 * t4021 + t4017 * t4028
        t5120 = t3714 ** 2
        t5121 = t3705 ** 2
        t5122 = t3709 ** 2
        t5125 = t2173 ** 2
        t5126 = t2164 ** 2
        t5127 = t2168 ** 2
        t5129 = t2185 * (t5125 + t5126 + t5127)
        t5134 = t4022 ** 2
        t5135 = t4013 ** 2
        t5136 = t4017 ** 2
        t5145 = u(t73,t121,t1375,n)
        t5149 = t3416 / 0.2E1 + (t3414 - t5145) * t177 / 0.2E1
        t5153 = t2158 * t5087
        t5157 = u(t73,t126,t1375,n)
        t5161 = t3431 / 0.2E1 + (t3429 - t5157) * t177 / 0.2E1
        t5167 = rx(t73,j,t1375,0,0)
        t5168 = rx(t73,j,t1375,1,1)
        t5170 = rx(t73,j,t1375,2,2)
        t5172 = rx(t73,j,t1375,1,2)
        t5174 = rx(t73,j,t1375,2,1)
        t5176 = rx(t73,j,t1375,0,1)
        t5177 = rx(t73,j,t1375,1,0)
        t5181 = rx(t73,j,t1375,2,0)
        t5183 = rx(t73,j,t1375,0,2)
        t5189 = 0.1E1 / (t5167 * t5168 * t5170 - t5167 * t5172 * t5174 -
     # t5168 * t5181 * t5183 - t5170 * t5176 * t5177 + t5172 * t5176 * t
     #5181 + t5174 * t5177 * t5183)
        t5190 = t4 * t5189
        t5213 = (t5145 - t1602) * t124 / 0.2E1 + (t1602 - t5157) * t124 
     #/ 0.2E1
        t5219 = t5181 ** 2
        t5220 = t5174 ** 2
        t5221 = t5170 ** 2
        t4972 = t5190 * (t5167 * t5181 + t5170 * t5183 + t5174 * t5176)
        t5230 = (t4 * (t5018 * (t5019 + t5020 + t5021) / 0.2E1 + t5028 /
     # 0.2E1) * t1339 - t5041) * t108 + (t5044 * (t4996 * t5006 + t4997 
     #* t5005 + t5001 * t5012) * ((t3573 - t902) * t124 / 0.2E1 + (t902 
     #- t3881) * t124 / 0.2E1) - t5062) * t108 / 0.2E1 + t5074 + (t5044 
     #* (t4996 * t5010 + t4999 * t5012 + t5003 * t5005) * (t904 / 0.2E1 
     #+ (t902 - t5079) * t177 / 0.2E1) - t5089) * t108 / 0.2E1 + t5099 +
     # (t3727 * t3737 * t5103 - t5107) * t124 / 0.2E1 + (-t4035 * t4045 
     #* t5114 + t5107) * t124 / 0.2E1 + (t4 * (t3726 * (t5120 + t5121 + 
     #t5122) / 0.2E1 + t5129 / 0.2E1) * t3469 - t4 * (t5129 / 0.2E1 + t4
     #034 * (t5134 + t5135 + t5136) / 0.2E1) * t3471) * t124 + (t3610 * 
     #t5149 - t5153) * t124 / 0.2E1 + (-t3903 * t5161 + t5153) * t124 / 
     #0.2E1 + t3454 + (t3451 - t4972 * ((t5079 - t1602) * t108 / 0.2E1 +
     # t1677 / 0.2E1)) * t177 / 0.2E1 + t3478 + (t3475 - t5190 * (t5168 
     #* t5174 + t5170 * t5172 + t5177 * t5181) * t5213) * t177 / 0.2E1 +
     # (t3480 - t4 * (t2250 / 0.2E1 + t5189 * (t5219 + t5220 + t5221) / 
     #0.2E1) * t1604) * t177
        t5231 = t5230 * t2184
        t5238 = t723 ** 2
        t5239 = t732 ** 2
        t5240 = t739 ** 2
        t5242 = t745 * (t5238 + t5239 + t5240)
        t5245 = t4 * (t4799 / 0.2E1 + t5242 / 0.2E1)
        t5246 = t5245 * t1260
        t5248 = (t4803 - t5246) * t108
        t5027 = t746 * (t723 * t733 + t724 * t732 + t728 * t739)
        t5254 = t5027 * t2348
        t5256 = (t4833 - t5254) * t108
        t5257 = t5256 / 0.2E1
        t5259 = t1626 / 0.2E1 + t939 / 0.2E1
        t5261 = t743 * t5259
        t5263 = (t4858 - t5261) * t108
        t5264 = t5263 / 0.2E1
        t5039 = t4164 * (t4141 * t4151 + t4142 * t4150 + t4146 * t4157)
        t5270 = t5039 * t4172
        t5272 = t4500 * t1262
        t5275 = (t5270 - t5272) * t124 / 0.2E1
        t5048 = t4322 * (t4299 * t4309 + t4300 * t4308 + t4304 * t4315)
        t5281 = t5048 * t4330
        t5284 = (t5272 - t5281) * t124 / 0.2E1
        t5285 = t4151 ** 2
        t5286 = t4142 ** 2
        t5287 = t4146 ** 2
        t5289 = t4163 * (t5285 + t5286 + t5287)
        t5290 = t378 ** 2
        t5291 = t369 ** 2
        t5292 = t373 ** 2
        t5294 = t390 * (t5290 + t5291 + t5292)
        t5297 = t4 * (t5289 / 0.2E1 + t5294 / 0.2E1)
        t5298 = t5297 * t1515
        t5299 = t4309 ** 2
        t5300 = t4300 ** 2
        t5301 = t4304 ** 2
        t5303 = t4321 * (t5299 + t5300 + t5301)
        t5306 = t4 * (t5294 / 0.2E1 + t5303 / 0.2E1)
        t5307 = t5306 * t1517
        t5311 = t1738 / 0.2E1 + t1231 / 0.2E1
        t5313 = t4014 * t5311
        t5315 = t450 * t4856
        t5318 = (t5313 - t5315) * t124 / 0.2E1
        t5320 = t1757 / 0.2E1 + t1246 / 0.2E1
        t5322 = t4127 * t5320
        t5325 = (t5315 - t5322) * t124 / 0.2E1
        t5326 = t1659 / 0.2E1
        t5327 = t1523 / 0.2E1
        t5328 = t5248 + t4836 + t5257 + t4861 + t5264 + t5275 + t5284 + 
     #(t5298 - t5307) * t124 + t5318 + t5325 + t5326 + t1269 + t5327 + t
     #1936 + t1420
        t5329 = t5328 * t389
        t5331 = (t5329 - t2274) * t177
        t5332 = t762 ** 2
        t5333 = t771 ** 2
        t5334 = t778 ** 2
        t5336 = t784 * (t5332 + t5333 + t5334)
        t5339 = t4 * (t5037 / 0.2E1 + t5336 / 0.2E1)
        t5340 = t5339 * t1341
        t5342 = (t5041 - t5340) * t108
        t5081 = t785 * (t762 * t772 + t763 * t771 + t767 * t778)
        t5348 = t5081 * t2361
        t5350 = (t5071 - t5348) * t108
        t5351 = t5350 / 0.2E1
        t5353 = t942 / 0.2E1 + t1632 / 0.2E1
        t5355 = t781 * t5353
        t5357 = (t5096 - t5355) * t108
        t5358 = t5357 / 0.2E1
        t5090 = t4203 * (t4180 * t4190 + t4181 * t4189 + t4185 * t4196)
        t5364 = t5090 * t4211
        t5366 = t4823 * t1663
        t5369 = (t5364 - t5366) * t124 / 0.2E1
        t5100 = t4361 * (t4338 * t4348 + t4339 * t4347 + t4343 * t4354)
        t5375 = t5100 * t4369
        t5378 = (t5366 - t5375) * t124 / 0.2E1
        t5379 = t4190 ** 2
        t5380 = t4181 ** 2
        t5381 = t4185 ** 2
        t5383 = t4202 * (t5379 + t5380 + t5381)
        t5384 = t419 ** 2
        t5385 = t410 ** 2
        t5386 = t414 ** 2
        t5388 = t431 * (t5384 + t5385 + t5386)
        t5391 = t4 * (t5383 / 0.2E1 + t5388 / 0.2E1)
        t5392 = t5391 * t1531
        t5393 = t4348 ** 2
        t5394 = t4339 ** 2
        t5395 = t4343 ** 2
        t5397 = t4360 * (t5393 + t5394 + t5395)
        t5400 = t4 * (t5388 / 0.2E1 + t5397 / 0.2E1)
        t5401 = t5400 * t1533
        t5405 = t1234 / 0.2E1 + t1743 / 0.2E1
        t5407 = t4027 * t5405
        t5409 = t466 * t5094
        t5412 = (t5407 - t5409) * t124 / 0.2E1
        t5414 = t1249 / 0.2E1 + t1762 / 0.2E1
        t5416 = t4145 * t5414
        t5419 = (t5409 - t5416) * t124 / 0.2E1
        t5420 = t1685 / 0.2E1
        t5421 = t1560 / 0.2E1
        t5422 = t5342 + t5074 + t5351 + t5099 + t5358 + t5369 + t5378 + 
     #(t5392 - t5401) * t124 + t5412 + t5419 + t1935 + t5420 + t1937 + t
     #5421 + t1459
        t5423 = t5422 * t430
        t5425 = (t2274 - t5423) * t177
        t5427 = t5331 / 0.2E1 + t5425 / 0.2E1
        t5429 = t195 * t5427
        t5433 = t2602 ** 2
        t5434 = t2611 ** 2
        t5435 = t2618 ** 2
        t5437 = t2624 * (t5433 + t5434 + t5435)
        t5440 = t4 * (t5242 / 0.2E1 + t5437 / 0.2E1)
        t5441 = t5440 * t1325
        t5443 = (t5246 - t5441) * t108
        t5447 = t2602 * t2612 + t2603 * t2611 + t2607 * t2618
        t5140 = t2625 * t5447
        t5449 = t5140 * t2687
        t5451 = (t5254 - t5449) * t108
        t5452 = t5451 / 0.2E1
        t5454 = t3189 / 0.2E1 + t956 / 0.2E1
        t5456 = t2484 * t5454
        t5458 = (t5261 - t5456) * t108
        t5459 = t5458 / 0.2E1
        t5463 = t4464 * t4474 + t4465 * t4473 + t4469 * t4480
        t5148 = t4487 * t5463
        t5465 = t5148 * t4495
        t5467 = t5027 * t2328
        t5469 = (t5465 - t5467) * t124
        t5470 = t5469 / 0.2E1
        t5474 = t4622 * t4632 + t4623 * t4631 + t4627 * t4638
        t5155 = t4645 * t5474
        t5476 = t5155 * t4653
        t5478 = (t5467 - t5476) * t124
        t5479 = t5478 / 0.2E1
        t5480 = t4474 ** 2
        t5481 = t4465 ** 2
        t5482 = t4469 ** 2
        t5484 = t4486 * (t5480 + t5481 + t5482)
        t5485 = t733 ** 2
        t5486 = t724 ** 2
        t5487 = t728 ** 2
        t5489 = t745 * (t5485 + t5486 + t5487)
        t5492 = t4 * (t5484 / 0.2E1 + t5489 / 0.2E1)
        t5493 = t5492 * t2344
        t5494 = t4632 ** 2
        t5495 = t4623 ** 2
        t5496 = t4627 ** 2
        t5498 = t4644 * (t5494 + t5495 + t5496)
        t5501 = t4 * (t5489 / 0.2E1 + t5498 / 0.2E1)
        t5502 = t5501 * t2346
        t5504 = (t5493 - t5502) * t124
        t5506 = t2938 / 0.2E1 + t2301 / 0.2E1
        t5508 = t4276 * t5506
        t5510 = t796 * t5259
        t5512 = (t5508 - t5510) * t124
        t5513 = t5512 / 0.2E1
        t5515 = t2959 / 0.2E1 + t2316 / 0.2E1
        t5517 = t4410 * t5515
        t5519 = (t5510 - t5517) * t124
        t5520 = t5519 / 0.2E1
        t5521 = t3123 / 0.2E1
        t5522 = t3323 / 0.2E1
        t5523 = t5443 + t5257 + t5452 + t5264 + t5459 + t5470 + t5479 + 
     #t5504 + t5513 + t5520 + t5521 + t2335 + t5522 + t2355 + t3065
        t5524 = t5523 * t744
        t5525 = t5524 - t2372
        t5526 = t5525 * t177
        t5527 = t2641 ** 2
        t5528 = t2650 ** 2
        t5529 = t2657 ** 2
        t5531 = t2663 * (t5527 + t5528 + t5529)
        t5534 = t4 * (t5336 / 0.2E1 + t5531 / 0.2E1)
        t5535 = t5534 * t1348
        t5537 = (t5340 - t5535) * t108
        t5541 = t2641 * t2651 + t2642 * t2650 + t2646 * t2657
        t5187 = t2664 * t5541
        t5543 = t5187 * t2704
        t5545 = (t5348 - t5543) * t108
        t5546 = t5545 / 0.2E1
        t5548 = t959 / 0.2E1 + t3194 / 0.2E1
        t5550 = t2520 * t5548
        t5552 = (t5355 - t5550) * t108
        t5553 = t5552 / 0.2E1
        t5557 = t4503 * t4513 + t4504 * t4512 + t4508 * t4519
        t5197 = t4526 * t5557
        t5559 = t5197 * t4534
        t5561 = t5081 * t2337
        t5563 = (t5559 - t5561) * t124
        t5564 = t5563 / 0.2E1
        t5568 = t4661 * t4671 + t4662 * t4670 + t4666 * t4677
        t5202 = t4684 * t5568
        t5570 = t5202 * t4692
        t5572 = (t5561 - t5570) * t124
        t5573 = t5572 / 0.2E1
        t5574 = t4513 ** 2
        t5575 = t4504 ** 2
        t5576 = t4508 ** 2
        t5578 = t4525 * (t5574 + t5575 + t5576)
        t5579 = t772 ** 2
        t5580 = t763 ** 2
        t5581 = t767 ** 2
        t5583 = t784 * (t5579 + t5580 + t5581)
        t5586 = t4 * (t5578 / 0.2E1 + t5583 / 0.2E1)
        t5587 = t5586 * t2357
        t5588 = t4671 ** 2
        t5589 = t4662 ** 2
        t5590 = t4666 ** 2
        t5592 = t4683 * (t5588 + t5589 + t5590)
        t5595 = t4 * (t5583 / 0.2E1 + t5592 / 0.2E1)
        t5596 = t5595 * t2359
        t5598 = (t5587 - t5596) * t124
        t5600 = t2304 / 0.2E1 + t2944 / 0.2E1
        t5602 = t4289 * t5600
        t5604 = t811 * t5353
        t5606 = (t5602 - t5604) * t124
        t5607 = t5606 / 0.2E1
        t5609 = t2319 / 0.2E1 + t2965 / 0.2E1
        t5611 = t4426 * t5609
        t5613 = (t5604 - t5611) * t124
        t5614 = t5613 / 0.2E1
        t5615 = t3143 / 0.2E1
        t5616 = t3343 / 0.2E1
        t5617 = t5537 + t5351 + t5546 + t5358 + t5553 + t5564 + t5573 + 
     #t5598 + t5607 + t5614 + t2342 + t5615 + t2366 + t5616 + t3101
        t5618 = t5617 * t783
        t5619 = t2372 - t5618
        t5620 = t5619 * t177
        t5622 = t5526 / 0.2E1 + t5620 / 0.2E1
        t5624 = t212 * t5622
        t5627 = (t5429 - t5624) * t108 / 0.2E1
        t5631 = (t4267 - t4590) * t108
        t5637 = t3486 / 0.2E1 + t3488 / 0.2E1
        t5639 = t143 * t5637
        t5646 = (t4425 - t4748) * t108
        t5658 = t3663 ** 2
        t5659 = t3672 ** 2
        t5660 = t3679 ** 2
        t5663 = t4141 ** 2
        t5664 = t4150 ** 2
        t5665 = t4157 ** 2
        t5667 = t4163 * (t5663 + t5664 + t5665)
        t5672 = t4464 ** 2
        t5673 = t4473 ** 2
        t5674 = t4480 ** 2
        t5676 = t4486 * (t5672 + t5673 + t5674)
        t5679 = t4 * (t5667 / 0.2E1 + t5676 / 0.2E1)
        t5680 = t5679 * t4170
        t5686 = t5039 * t4222
        t5691 = t5148 * t4545
        t5694 = (t5686 - t5691) * t108 / 0.2E1
        t5698 = t3970 * t5311
        t5703 = t4219 * t5506
        t5706 = (t5698 - t5703) * t108 / 0.2E1
        t5707 = rx(t5,t1047,t174,0,0)
        t5708 = rx(t5,t1047,t174,1,1)
        t5710 = rx(t5,t1047,t174,2,2)
        t5712 = rx(t5,t1047,t174,1,2)
        t5714 = rx(t5,t1047,t174,2,1)
        t5716 = rx(t5,t1047,t174,0,1)
        t5717 = rx(t5,t1047,t174,1,0)
        t5721 = rx(t5,t1047,t174,2,0)
        t5723 = rx(t5,t1047,t174,0,2)
        t5729 = 0.1E1 / (t5707 * t5708 * t5710 - t5707 * t5712 * t5714 -
     # t5708 * t5721 * t5723 - t5710 * t5716 * t5717 + t5712 * t5716 * t
     #5721 + t5714 * t5717 * t5723)
        t5730 = t4 * t5729
        t5734 = t5707 * t5717 + t5708 * t5716 + t5712 * t5723
        t5738 = (t1277 - t2885) * t108
        t5740 = (t3650 - t1277) * t108 / 0.2E1 + t5738 / 0.2E1
        t5746 = t5717 ** 2
        t5747 = t5708 ** 2
        t5748 = t5712 ** 2
        t5760 = t5708 * t5714 + t5710 * t5712 + t5717 * t5721
        t5761 = u(t5,t1047,t1364,n)
        t5765 = (t5761 - t1277) * t177 / 0.2E1 + t1279 / 0.2E1
        t5771 = rx(t5,t121,t1364,0,0)
        t5772 = rx(t5,t121,t1364,1,1)
        t5774 = rx(t5,t121,t1364,2,2)
        t5776 = rx(t5,t121,t1364,1,2)
        t5778 = rx(t5,t121,t1364,2,1)
        t5780 = rx(t5,t121,t1364,0,1)
        t5781 = rx(t5,t121,t1364,1,0)
        t5785 = rx(t5,t121,t1364,2,0)
        t5787 = rx(t5,t121,t1364,0,2)
        t5793 = 0.1E1 / (t5771 * t5772 * t5774 - t5771 * t5776 * t5778 -
     # t5772 * t5785 * t5787 - t5774 * t5780 * t5781 + t5776 * t5780 * t
     #5785 + t5778 * t5781 * t5787)
        t5794 = t4 * t5793
        t5798 = t5771 * t5785 + t5774 * t5787 + t5778 * t5780
        t5802 = (t1504 - t2936) * t108
        t5804 = (t4907 - t1504) * t108 / 0.2E1 + t5802 / 0.2E1
        t5813 = t5772 * t5778 + t5774 * t5776 + t5781 * t5785
        t5817 = (t5761 - t1504) * t124 / 0.2E1 + t1506 / 0.2E1
        t5823 = t5785 ** 2
        t5824 = t5778 ** 2
        t5825 = t5774 ** 2
        t5834 = (t4 * (t3685 * (t5658 + t5659 + t5660) / 0.2E1 + t5667 /
     # 0.2E1) * t3694 - t5680) * t108 + (t3686 * t3750 * t4865 - t5686) 
     #* t108 / 0.2E1 + t5694 + (t3582 * t4911 - t5698) * t108 / 0.2E1 + 
     #t5706 + (t5730 * t5734 * t5740 - t5270) * t124 / 0.2E1 + t5275 + (
     #t4 * (t5729 * (t5746 + t5747 + t5748) / 0.2E1 + t5289 / 0.2E1) * t
     #1815 - t5298) * t124 + (t5730 * t5760 * t5765 - t5313) * t124 / 0.
     #2E1 + t5318 + (t5794 * t5798 * t5804 - t4174) * t177 / 0.2E1 + t41
     #79 + (t5794 * t5813 * t5817 - t4224) * t177 / 0.2E1 + t4229 + (t4 
     #* (t5793 * (t5823 + t5824 + t5825) / 0.2E1 + t4245 / 0.2E1) * t173
     #8 - t4254) * t177
        t5835 = t5834 * t4162
        t5838 = t3704 ** 2
        t5839 = t3713 ** 2
        t5840 = t3720 ** 2
        t5843 = t4180 ** 2
        t5844 = t4189 ** 2
        t5845 = t4196 ** 2
        t5847 = t4202 * (t5843 + t5844 + t5845)
        t5852 = t4503 ** 2
        t5853 = t4512 ** 2
        t5854 = t4519 ** 2
        t5856 = t4525 * (t5852 + t5853 + t5854)
        t5859 = t4 * (t5847 / 0.2E1 + t5856 / 0.2E1)
        t5860 = t5859 * t4209
        t5866 = t5090 * t4235
        t5871 = t5197 * t4558
        t5874 = (t5866 - t5871) * t108 / 0.2E1
        t5878 = t4003 * t5405
        t5883 = t4261 * t5600
        t5886 = (t5878 - t5883) * t108 / 0.2E1
        t5887 = rx(t5,t1047,t179,0,0)
        t5888 = rx(t5,t1047,t179,1,1)
        t5890 = rx(t5,t1047,t179,2,2)
        t5892 = rx(t5,t1047,t179,1,2)
        t5894 = rx(t5,t1047,t179,2,1)
        t5896 = rx(t5,t1047,t179,0,1)
        t5897 = rx(t5,t1047,t179,1,0)
        t5901 = rx(t5,t1047,t179,2,0)
        t5903 = rx(t5,t1047,t179,0,2)
        t5909 = 0.1E1 / (t5887 * t5888 * t5890 - t5887 * t5892 * t5894 -
     # t5888 * t5901 * t5903 - t5890 * t5896 * t5897 + t5892 * t5896 * t
     #5901 + t5894 * t5897 * t5903)
        t5910 = t4 * t5909
        t5914 = t5887 * t5897 + t5888 * t5896 + t5892 * t5903
        t5918 = (t1280 - t2888) * t108
        t5920 = (t3653 - t1280) * t108 / 0.2E1 + t5918 / 0.2E1
        t5926 = t5897 ** 2
        t5927 = t5888 ** 2
        t5928 = t5892 ** 2
        t5940 = t5888 * t5894 + t5890 * t5892 + t5897 * t5901
        t5941 = u(t5,t1047,t1375,n)
        t5945 = t1282 / 0.2E1 + (t1280 - t5941) * t177 / 0.2E1
        t5951 = rx(t5,t121,t1375,0,0)
        t5952 = rx(t5,t121,t1375,1,1)
        t5954 = rx(t5,t121,t1375,2,2)
        t5956 = rx(t5,t121,t1375,1,2)
        t5958 = rx(t5,t121,t1375,2,1)
        t5960 = rx(t5,t121,t1375,0,1)
        t5961 = rx(t5,t121,t1375,1,0)
        t5965 = rx(t5,t121,t1375,2,0)
        t5967 = rx(t5,t121,t1375,0,2)
        t5973 = 0.1E1 / (t5951 * t5952 * t5954 - t5951 * t5956 * t5958 -
     # t5952 * t5965 * t5967 - t5954 * t5960 * t5961 + t5956 * t5960 * t
     #5965 + t5958 * t5961 * t5967)
        t5974 = t4 * t5973
        t5978 = t5951 * t5965 + t5954 * t5967 + t5958 * t5960
        t5982 = (t1549 - t2942) * t108
        t5984 = (t5145 - t1549) * t108 / 0.2E1 + t5982 / 0.2E1
        t5993 = t5952 * t5958 + t5954 * t5956 + t5961 * t5965
        t5997 = (t5941 - t1549) * t124 / 0.2E1 + t1551 / 0.2E1
        t6003 = t5965 ** 2
        t6004 = t5958 ** 2
        t6005 = t5954 ** 2
        t6014 = (t4 * (t3726 * (t5838 + t5839 + t5840) / 0.2E1 + t5847 /
     # 0.2E1) * t3735 - t5860) * t108 + (t3727 * t3765 * t5103 - t5866) 
     #* t108 / 0.2E1 + t5874 + (t3589 * t5149 - t5878) * t108 / 0.2E1 + 
     #t5886 + (t5910 * t5914 * t5920 - t5364) * t124 / 0.2E1 + t5369 + (
     #t4 * (t5909 * (t5926 + t5927 + t5928) / 0.2E1 + t5383 / 0.2E1) * t
     #1841 - t5392) * t124 + (t5910 * t5940 * t5945 - t5407) * t124 / 0.
     #2E1 + t5412 + t4216 + (-t5974 * t5978 * t5984 + t4213) * t177 / 0.
     #2E1 + t4240 + (-t5974 * t5993 * t5997 + t4237) * t177 / 0.2E1 + (t
     #4263 - t4 * (t4259 / 0.2E1 + t5973 * (t6003 + t6004 + t6005) / 0.2
     #E1) * t1743) * t177
        t6015 = t6014 * t4201
        t6019 = (t5835 - t4267) * t177 / 0.2E1 + (t4267 - t6015) * t177 
     #/ 0.2E1
        t6023 = t342 * t5427
        t6027 = t3971 ** 2
        t6028 = t3980 ** 2
        t6029 = t3987 ** 2
        t6032 = t4299 ** 2
        t6033 = t4308 ** 2
        t6034 = t4315 ** 2
        t6036 = t4321 * (t6032 + t6033 + t6034)
        t6041 = t4622 ** 2
        t6042 = t4631 ** 2
        t6043 = t4638 ** 2
        t6045 = t4644 * (t6041 + t6042 + t6043)
        t6048 = t4 * (t6036 / 0.2E1 + t6045 / 0.2E1)
        t6049 = t6048 * t4328
        t6055 = t5048 * t4380
        t6060 = t5155 * t4703
        t6063 = (t6055 - t6060) * t108 / 0.2E1
        t6067 = t4082 * t5320
        t6072 = t4363 * t5515
        t6075 = (t6067 - t6072) * t108 / 0.2E1
        t6076 = rx(t5,t1111,t174,0,0)
        t6077 = rx(t5,t1111,t174,1,1)
        t6079 = rx(t5,t1111,t174,2,2)
        t6081 = rx(t5,t1111,t174,1,2)
        t6083 = rx(t5,t1111,t174,2,1)
        t6085 = rx(t5,t1111,t174,0,1)
        t6086 = rx(t5,t1111,t174,1,0)
        t6090 = rx(t5,t1111,t174,2,0)
        t6092 = rx(t5,t1111,t174,0,2)
        t6098 = 0.1E1 / (t6076 * t6077 * t6079 - t6076 * t6081 * t6083 -
     # t6077 * t6090 * t6092 - t6079 * t6085 * t6086 + t6081 * t6085 * t
     #6090 + t6083 * t6086 * t6092)
        t6099 = t4 * t6098
        t6103 = t6076 * t6086 + t6077 * t6085 + t6081 * t6092
        t6107 = (t1299 - t2908) * t108
        t6109 = (t3958 - t1299) * t108 / 0.2E1 + t6107 / 0.2E1
        t6115 = t6086 ** 2
        t6116 = t6077 ** 2
        t6117 = t6081 ** 2
        t6129 = t6077 * t6083 + t6079 * t6081 + t6086 * t6090
        t6130 = u(t5,t1111,t1364,n)
        t6134 = (t6130 - t1299) * t177 / 0.2E1 + t1301 / 0.2E1
        t6140 = rx(t5,t126,t1364,0,0)
        t6141 = rx(t5,t126,t1364,1,1)
        t6143 = rx(t5,t126,t1364,2,2)
        t6145 = rx(t5,t126,t1364,1,2)
        t6147 = rx(t5,t126,t1364,2,1)
        t6149 = rx(t5,t126,t1364,0,1)
        t6150 = rx(t5,t126,t1364,1,0)
        t6154 = rx(t5,t126,t1364,2,0)
        t6156 = rx(t5,t126,t1364,0,2)
        t6162 = 0.1E1 / (t6140 * t6141 * t6143 - t6140 * t6145 * t6147 -
     # t6141 * t6154 * t6156 - t6143 * t6149 * t6150 + t6145 * t6149 * t
     #6154 + t6147 * t6150 * t6156)
        t6163 = t4 * t6162
        t6167 = t6140 * t6154 + t6143 * t6156 + t6147 * t6149
        t6171 = (t1507 - t2957) * t108
        t6173 = (t4919 - t1507) * t108 / 0.2E1 + t6171 / 0.2E1
        t6182 = t6141 * t6147 + t6143 * t6145 + t6150 * t6154
        t6186 = t1509 / 0.2E1 + (t1507 - t6130) * t124 / 0.2E1
        t6192 = t6154 ** 2
        t6193 = t6147 ** 2
        t6194 = t6143 ** 2
        t6203 = (t4 * (t3993 * (t6027 + t6028 + t6029) / 0.2E1 + t6036 /
     # 0.2E1) * t4002 - t6049) * t108 + (t3994 * t4058 * t4876 - t6055) 
     #* t108 / 0.2E1 + t6063 + (t3884 * t4923 - t6067) * t108 / 0.2E1 + 
     #t6075 + t5284 + (-t6099 * t6103 * t6109 + t5281) * t124 / 0.2E1 + 
     #(t5307 - t4 * (t5303 / 0.2E1 + t6098 * (t6115 + t6116 + t6117) / 0
     #.2E1) * t1820) * t124 + t5325 + (-t6099 * t6129 * t6134 + t5322) *
     # t124 / 0.2E1 + (t6163 * t6167 * t6173 - t4332) * t177 / 0.2E1 + t
     #4337 + (t6163 * t6182 * t6186 - t4382) * t177 / 0.2E1 + t4387 + (t
     #4 * (t6162 * (t6192 + t6193 + t6194) / 0.2E1 + t4403 / 0.2E1) * t1
     #757 - t4412) * t177
        t6204 = t6203 * t4320
        t6207 = t4012 ** 2
        t6208 = t4021 ** 2
        t6209 = t4028 ** 2
        t6212 = t4338 ** 2
        t6213 = t4347 ** 2
        t6214 = t4354 ** 2
        t6216 = t4360 * (t6212 + t6213 + t6214)
        t6221 = t4661 ** 2
        t6222 = t4670 ** 2
        t6223 = t4677 ** 2
        t6225 = t4683 * (t6221 + t6222 + t6223)
        t6228 = t4 * (t6216 / 0.2E1 + t6225 / 0.2E1)
        t6229 = t6228 * t4367
        t6235 = t5100 * t4393
        t6240 = t5202 * t4716
        t6243 = (t6235 - t6240) * t108 / 0.2E1
        t6247 = t4111 * t5414
        t6252 = t4394 * t5609
        t6255 = (t6247 - t6252) * t108 / 0.2E1
        t6256 = rx(t5,t1111,t179,0,0)
        t6257 = rx(t5,t1111,t179,1,1)
        t6259 = rx(t5,t1111,t179,2,2)
        t6261 = rx(t5,t1111,t179,1,2)
        t6263 = rx(t5,t1111,t179,2,1)
        t6265 = rx(t5,t1111,t179,0,1)
        t6266 = rx(t5,t1111,t179,1,0)
        t6270 = rx(t5,t1111,t179,2,0)
        t6272 = rx(t5,t1111,t179,0,2)
        t6278 = 0.1E1 / (t6256 * t6257 * t6259 - t6256 * t6261 * t6263 -
     # t6257 * t6270 * t6272 - t6259 * t6265 * t6266 + t6261 * t6265 * t
     #6270 + t6263 * t6266 * t6272)
        t6279 = t4 * t6278
        t6283 = t6256 * t6266 + t6257 * t6265 + t6261 * t6272
        t6287 = (t1302 - t2911) * t108
        t6289 = (t3961 - t1302) * t108 / 0.2E1 + t6287 / 0.2E1
        t6295 = t6266 ** 2
        t6296 = t6257 ** 2
        t6297 = t6261 ** 2
        t6309 = t6257 * t6263 + t6259 * t6261 + t6266 * t6270
        t6310 = u(t5,t1111,t1375,n)
        t6314 = t1304 / 0.2E1 + (t1302 - t6310) * t177 / 0.2E1
        t6320 = rx(t5,t126,t1375,0,0)
        t6321 = rx(t5,t126,t1375,1,1)
        t6323 = rx(t5,t126,t1375,2,2)
        t6325 = rx(t5,t126,t1375,1,2)
        t6327 = rx(t5,t126,t1375,2,1)
        t6329 = rx(t5,t126,t1375,0,1)
        t6330 = rx(t5,t126,t1375,1,0)
        t6334 = rx(t5,t126,t1375,2,0)
        t6336 = rx(t5,t126,t1375,0,2)
        t6342 = 0.1E1 / (t6320 * t6321 * t6323 - t6320 * t6325 * t6327 -
     # t6321 * t6334 * t6336 - t6323 * t6329 * t6330 + t6325 * t6329 * t
     #6334 + t6327 * t6330 * t6336)
        t6343 = t4 * t6342
        t6347 = t6320 * t6334 + t6323 * t6336 + t6327 * t6329
        t6351 = (t1552 - t2963) * t108
        t6353 = (t5157 - t1552) * t108 / 0.2E1 + t6351 / 0.2E1
        t6362 = t6321 * t6327 + t6323 * t6325 + t6330 * t6334
        t6366 = t1554 / 0.2E1 + (t1552 - t6310) * t124 / 0.2E1
        t6372 = t6334 ** 2
        t6373 = t6327 ** 2
        t6374 = t6323 ** 2
        t6383 = (t4 * (t4034 * (t6207 + t6208 + t6209) / 0.2E1 + t6216 /
     # 0.2E1) * t4043 - t6229) * t108 + (t4035 * t4073 * t5114 - t6235) 
     #* t108 / 0.2E1 + t6243 + (t3889 * t5161 - t6247) * t108 / 0.2E1 + 
     #t6255 + t5378 + (-t6279 * t6283 * t6289 + t5375) * t124 / 0.2E1 + 
     #(t5401 - t4 * (t5397 / 0.2E1 + t6278 * (t6295 + t6296 + t6297) / 0
     #.2E1) * t1846) * t124 + t5419 + (-t6279 * t6309 * t6314 + t5416) *
     # t124 / 0.2E1 + t4374 + (-t6343 * t6347 * t6353 + t4371) * t177 / 
     #0.2E1 + t4398 + (-t6343 * t6362 * t6366 + t4395) * t177 / 0.2E1 + 
     #(t4421 - t4 * (t4417 / 0.2E1 + t6342 * (t6372 + t6373 + t6374) / 0
     #.2E1) * t1762) * t177
        t6384 = t6383 * t4359
        t6388 = (t6204 - t4425) * t177 / 0.2E1 + (t4425 - t6384) * t177 
     #/ 0.2E1
        t6397 = (t5329 - t5524) * t108
        t6403 = t195 * t5637
        t6410 = (t5423 - t5618) * t108
        t6423 = (t5835 - t5329) * t124 / 0.2E1 + (t5329 - t6204) * t124 
     #/ 0.2E1
        t6427 = t342 * t4429
        t6436 = (t6015 - t5423) * t124 / 0.2E1 + (t5423 - t6384) * t124 
     #/ 0.2E1
        t6446 = (t104 * t3486 - t3489) * t108 + (t128 * ((t3797 - t3484)
     # * t124 / 0.2E1 + (t3484 - t4105) * t124 / 0.2E1) - t4431) * t108 
     #/ 0.2E1 + t4757 + (t181 * ((t4993 - t3484) * t177 / 0.2E1 + (t3484
     # - t5231) * t177 / 0.2E1) - t5429) * t108 / 0.2E1 + t5627 + (t251 
     #* ((t3797 - t4267) * t108 / 0.2E1 + t5631 / 0.2E1) - t5639) * t124
     # / 0.2E1 + (t5639 - t292 * ((t4105 - t4425) * t108 / 0.2E1 + t5646
     # / 0.2E1)) * t124 / 0.2E1 + (t315 * t4269 - t324 * t4427) * t124 +
     # (t333 * t6019 - t6023) * t124 / 0.2E1 + (-t356 * t6388 + t6023) *
     # t124 / 0.2E1 + (t396 * ((t4993 - t5329) * t108 / 0.2E1 + t6397 / 
     #0.2E1) - t6403) * t177 / 0.2E1 + (t6403 - t435 * ((t5231 - t5423) 
     #* t108 / 0.2E1 + t6410 / 0.2E1)) * t177 / 0.2E1 + (t450 * t6423 - 
     #t6427) * t177 / 0.2E1 + (-t466 * t6436 + t6427) * t177 / 0.2E1 + (
     #t492 * t5331 - t501 * t5425) * t177
        t6447 = t867 * t6446
        t6450 = t1945 * t71
        t6453 = (t397 / 0.2E1 - t752 / 0.2E1) * t108
        t6454 = ut(t2386,j,t174,n)
        t6456 = (t567 - t6454) * t108
        t6459 = (t399 / 0.2E1 - t6456 / 0.2E1) * t108
        t6183 = (t6453 - t6459) * t108
        t6463 = t743 * t6183
        t6466 = (t109 / 0.2E1 - t541 / 0.2E1) * t108
        t6467 = ut(t2386,j,k,n)
        t6469 = (t539 - t6467) * t108
        t6472 = (t112 / 0.2E1 - t6469 / 0.2E1) * t108
        t6190 = (t6466 - t6472) * t108
        t6476 = t212 * t6190
        t6478 = (t6463 - t6476) * t177
        t6481 = (t438 / 0.2E1 - t791 / 0.2E1) * t108
        t6482 = ut(t2386,j,t179,n)
        t6484 = (t570 - t6482) * t108
        t6487 = (t440 / 0.2E1 - t6484 / 0.2E1) * t108
        t6200 = (t6481 - t6487) * t108
        t6491 = t781 * t6200
        t6493 = (t6476 - t6491) * t177
        t6499 = (t109 - t112) * t108
        t6501 = (t112 - t541) * t108
        t6502 = t6499 - t6501
        t6503 = t6502 * t108
        t6504 = t64 * t6503
        t6506 = (t541 - t6469) * t108
        t6507 = t6501 - t6506
        t6508 = t6507 * t108
        t6509 = t538 * t6508
        t6512 = t2747 * t108
        t6513 = t2417 * t6469
        t6515 = (t542 - t6513) * t108
        t6516 = t544 - t6515
        t6517 = t6516 * t108
        t6525 = (t250 / 0.2E1 - t609 / 0.2E1) * t108
        t6526 = ut(t2386,t121,k,n)
        t6528 = (t550 - t6526) * t108
        t6531 = (t252 / 0.2E1 - t6528 / 0.2E1) * t108
        t6220 = (t6525 - t6531) * t108
        t6535 = t604 * t6220
        t6538 = t161 * t6190
        t6540 = (t6535 - t6538) * t124
        t6543 = (t293 / 0.2E1 - t650 / 0.2E1) * t108
        t6544 = ut(t2386,t126,k,n)
        t6546 = (t553 - t6544) * t108
        t6549 = (t295 / 0.2E1 - t6546 / 0.2E1) * t108
        t6233 = (t6543 - t6549) * t108
        t6553 = t641 * t6233
        t6555 = (t6538 - t6553) * t124
        t6560 = ut(t5,j,t1364,n)
        t6561 = ut(i,j,t1364,n)
        t6563 = (t6560 - t6561) * t108
        t6564 = ut(t507,j,t1364,n)
        t6566 = (t6561 - t6564) * t108
        t6568 = t6563 / 0.2E1 + t6566 / 0.2E1
        t6570 = t2949 * t6568
        t6572 = (t6570 - t756) * t177
        t6574 = (t6572 - t760) * t177
        t6576 = (t760 - t797) * t177
        t6578 = (t6574 - t6576) * t177
        t6579 = ut(t5,j,t1375,n)
        t6580 = ut(i,j,t1375,n)
        t6582 = (t6579 - t6580) * t108
        t6583 = ut(t507,j,t1375,n)
        t6585 = (t6580 - t6583) * t108
        t6587 = t6582 / 0.2E1 + t6585 / 0.2E1
        t6589 = t2967 * t6587
        t6591 = (t795 - t6589) * t177
        t6593 = (t797 - t6591) * t177
        t6595 = (t6576 - t6593) * t177
        t6600 = ut(i,t1047,t174,n)
        t6602 = (t6600 - t687) * t124
        t6605 = (t6602 / 0.2E1 - t806 / 0.2E1) * t124
        t6606 = ut(i,t1111,t174,n)
        t6608 = (t710 - t6606) * t124
        t6611 = (t804 / 0.2E1 - t6608 / 0.2E1) * t124
        t6267 = (t6605 - t6611) * t124
        t6615 = t796 * t6267
        t6616 = ut(i,t1047,k,n)
        t6618 = (t6616 - t157) * t124
        t6621 = (t6618 / 0.2E1 - t162 / 0.2E1) * t124
        t6622 = ut(i,t1111,k,n)
        t6624 = (t160 - t6622) * t124
        t6627 = (t159 / 0.2E1 - t6624 / 0.2E1) * t124
        t6276 = (t6621 - t6627) * t124
        t6631 = t691 * t6276
        t6633 = (t6615 - t6631) * t177
        t6634 = ut(i,t1047,t179,n)
        t6636 = (t6634 - t690) * t124
        t6639 = (t6636 / 0.2E1 - t823 / 0.2E1) * t124
        t6640 = ut(i,t1111,t179,n)
        t6642 = (t713 - t6640) * t124
        t6645 = (t821 / 0.2E1 - t6642 / 0.2E1) * t124
        t6288 = (t6639 - t6645) * t124
        t6649 = t811 * t6288
        t6651 = (t6631 - t6649) * t177
        t6657 = (t202 - t219) * t108
        t6659 = (t219 - t578) * t108
        t6661 = (t6657 - t6659) * t108
        t6663 = (t6454 - t6467) * t177
        t6665 = (t6467 - t6482) * t177
        t6667 = t6663 / 0.2E1 + t6665 / 0.2E1
        t6669 = t2281 * t6667
        t6671 = (t576 - t6669) * t108
        t6673 = (t578 - t6671) * t108
        t6675 = (t6659 - t6673) * t108
        t6680 = ut(i,t121,t1364,n)
        t6682 = (t6680 - t687) * t177
        t6685 = (t6682 / 0.2E1 - t692 / 0.2E1) * t177
        t6686 = ut(i,t121,t1375,n)
        t6688 = (t690 - t6686) * t177
        t6691 = (t689 / 0.2E1 - t6688 / 0.2E1) * t177
        t6311 = (t6685 - t6691) * t177
        t6695 = t683 * t6311
        t6697 = (t6561 - t208) * t177
        t6700 = (t6697 / 0.2E1 - t213 / 0.2E1) * t177
        t6702 = (t211 - t6580) * t177
        t6705 = (t210 / 0.2E1 - t6702 / 0.2E1) * t177
        t6319 = (t6700 - t6705) * t177
        t6709 = t691 * t6319
        t6711 = (t6695 - t6709) * t124
        t6712 = ut(i,t126,t1364,n)
        t6714 = (t6712 - t710) * t177
        t6717 = (t6714 / 0.2E1 - t715 / 0.2E1) * t177
        t6718 = ut(i,t126,t1375,n)
        t6720 = (t713 - t6718) * t177
        t6723 = (t712 / 0.2E1 - t6720 / 0.2E1) * t177
        t6335 = (t6717 - t6723) * t177
        t6727 = t706 * t6335
        t6729 = (t6709 - t6727) * t124
        t6734 = t1495 * t112
        t6735 = t3306 * t541
        t6738 = ut(t5,t1047,k,n)
        t6740 = (t6738 - t6616) * t108
        t6741 = ut(t507,t1047,k,n)
        t6743 = (t6616 - t6741) * t108
        t6745 = t6740 / 0.2E1 + t6743 / 0.2E1
        t6747 = t3010 * t6745
        t6749 = (t6747 - t613) * t124
        t6751 = (t6749 - t619) * t124
        t6753 = (t619 - t656) * t124
        t6755 = (t6751 - t6753) * t124
        t6756 = ut(t5,t1111,k,n)
        t6758 = (t6756 - t6622) * t108
        t6759 = ut(t507,t1111,k,n)
        t6761 = (t6622 - t6759) * t108
        t6763 = t6758 / 0.2E1 + t6761 / 0.2E1
        t6765 = t3031 * t6763
        t6767 = (t654 - t6765) * t124
        t6769 = (t656 - t6767) * t124
        t6771 = (t6753 - t6769) * t124
        t6776 = t3257 * t159
        t6777 = t3269 * t162
        t6781 = (t6697 - t210) * t177
        t6783 = (t210 - t213) * t177
        t6784 = t6781 - t6783
        t6785 = t6784 * t177
        t6786 = t843 * t6785
        t6788 = (t213 - t6702) * t177
        t6789 = t6783 - t6788
        t6790 = t6789 * t177
        t6791 = t852 * t6790
        t6794 = t3062 * t6697
        t6796 = (t6794 - t844) * t177
        t6797 = t6796 - t855
        t6798 = t6797 * t177
        t6799 = t3098 * t6702
        t6801 = (t853 - t6799) * t177
        t6802 = t855 - t6801
        t6803 = t6802 * t177
        t6809 = -t868 * (t6478 / 0.2E1 + t6493 / 0.2E1) / 0.6E1 - t868 *
     # ((t6504 - t6509) * t108 + (t6512 - t6517) * t108) / 0.24E2 - t868
     # * (t6540 / 0.2E1 + t6555 / 0.2E1) / 0.6E1 + t579 + t620 + t657 + 
     #t705 - t1363 * (t6578 / 0.2E1 + t6595 / 0.2E1) / 0.6E1 - t1046 * (
     #t6633 / 0.2E1 + t6651 / 0.2E1) / 0.6E1 - t868 * (t6661 / 0.2E1 + t
     #6675 / 0.2E1) / 0.6E1 - t1363 * (t6711 / 0.2E1 + t6729 / 0.2E1) / 
     #0.6E1 + (t6734 - t6735) * t108 - t1046 * (t6755 / 0.2E1 + t6771 / 
     #0.2E1) / 0.6E1 + (t6776 - t6777) * t124 - t1363 * ((t6786 - t6791)
     # * t177 + (t6798 - t6803) * t177) / 0.24E2
        t6811 = (t6680 - t6561) * t124
        t6813 = (t6561 - t6712) * t124
        t6815 = t6811 / 0.2E1 + t6813 / 0.2E1
        t6817 = t3120 * t6815
        t6819 = (t6817 - t810) * t177
        t6821 = (t6819 - t814) * t177
        t6823 = (t814 - t829) * t177
        t6825 = (t6821 - t6823) * t177
        t6827 = (t6686 - t6580) * t124
        t6829 = (t6580 - t6718) * t124
        t6831 = t6827 / 0.2E1 + t6829 / 0.2E1
        t6833 = t3142 * t6831
        t6835 = (t827 - t6833) * t177
        t6837 = (t829 - t6835) * t177
        t6839 = (t6823 - t6837) * t177
        t6844 = t3363 * t210
        t6845 = t3375 * t213
        t6849 = (t6618 - t159) * t124
        t6851 = (t159 - t162) * t124
        t6852 = t6849 - t6851
        t6853 = t6852 * t124
        t6854 = t670 * t6853
        t6856 = (t162 - t6624) * t124
        t6857 = t6851 - t6856
        t6858 = t6857 * t124
        t6859 = t679 * t6858
        t6862 = t2833 * t6618
        t6864 = (t6862 - t671) * t124
        t6865 = t6864 - t682
        t6866 = t6865 * t124
        t6867 = t2869 * t6624
        t6869 = (t680 - t6867) * t124
        t6870 = t682 - t6869
        t6871 = t6870 * t124
        t6878 = (t6560 - t191) * t177
        t6881 = (t6878 / 0.2E1 - t196 / 0.2E1) * t177
        t6883 = (t194 - t6579) * t177
        t6886 = (t193 / 0.2E1 - t6883 / 0.2E1) * t177
        t6445 = (t6881 - t6886) * t177
        t6890 = t195 * t6445
        t6893 = t212 * t6319
        t6895 = (t6890 - t6893) * t108
        t6897 = (t6564 - t567) * t177
        t6900 = (t6897 / 0.2E1 - t572 / 0.2E1) * t177
        t6902 = (t570 - t6583) * t177
        t6905 = (t569 / 0.2E1 - t6902 / 0.2E1) * t177
        t6461 = (t6900 - t6905) * t177
        t6909 = t564 * t6461
        t6911 = (t6893 - t6909) * t108
        t6917 = (t6600 - t6616) * t177
        t6919 = (t6616 - t6634) * t177
        t6921 = t6917 / 0.2E1 + t6919 / 0.2E1
        t6923 = t2692 * t6921
        t6925 = (t6923 - t696) * t124
        t6927 = (t6925 - t704) * t124
        t6929 = (t704 - t721) * t124
        t6931 = (t6927 - t6929) * t124
        t6933 = (t6606 - t6622) * t177
        t6935 = (t6622 - t6640) * t177
        t6937 = t6933 / 0.2E1 + t6935 / 0.2E1
        t6939 = t2720 * t6937
        t6941 = (t719 - t6939) * t124
        t6943 = (t721 - t6941) * t124
        t6945 = (t6929 - t6943) * t124
        t6951 = (t6738 - t139) * t124
        t6954 = (t6951 / 0.2E1 - t144 / 0.2E1) * t124
        t6956 = (t142 - t6756) * t124
        t6959 = (t141 / 0.2E1 - t6956 / 0.2E1) * t124
        t6497 = (t6954 - t6959) * t124
        t6963 = t143 * t6497
        t6966 = t161 * t6276
        t6968 = (t6963 - t6966) * t108
        t6970 = (t6741 - t550) * t124
        t6973 = (t6970 / 0.2E1 - t555 / 0.2E1) * t124
        t6975 = (t553 - t6759) * t124
        t6978 = (t552 / 0.2E1 - t6975 / 0.2E1) * t124
        t6519 = (t6973 - t6978) * t124
        t6982 = t547 * t6519
        t6984 = (t6966 - t6982) * t108
        t6990 = (t150 - t168) * t108
        t6992 = (t168 - t561) * t108
        t6994 = (t6990 - t6992) * t108
        t6996 = (t6526 - t6467) * t124
        t6998 = (t6467 - t6544) * t124
        t7000 = t6996 / 0.2E1 + t6998 / 0.2E1
        t7002 = t2261 * t7000
        t7004 = (t559 - t7002) * t108
        t7006 = (t561 - t7004) * t108
        t7008 = (t6992 - t7006) * t108
        t7013 = t169 + t220 - t1363 * (t6825 / 0.2E1 + t6839 / 0.2E1) / 
     #0.6E1 + (t6844 - t6845) * t177 + t722 + t761 + t798 + t815 - t1046
     # * ((t6854 - t6859) * t124 + (t6866 - t6871) * t124) / 0.24E2 - t1
     #363 * (t6895 / 0.2E1 + t6911 / 0.2E1) / 0.6E1 + t830 - t1046 * (t6
     #931 / 0.2E1 + t6945 / 0.2E1) / 0.6E1 + t562 - t1046 * (t6968 / 0.2
     #E1 + t6984 / 0.2E1) / 0.6E1 - t868 * (t6994 / 0.2E1 + t7008 / 0.2E
     #1) / 0.6E1
        t7014 = t6809 + t7013
        t7015 = t2263 * t7014
        t7017 = t6450 * t7015 / 0.4E1
        t7018 = t1943 * t68
        t7023 = (t1948 * t3483 - t2380) * t108
        t7026 = t2379 * (t7023 / 0.2E1 + t2383 / 0.2E1)
        t7028 = t864 * t7026 / 0.4E1
        t7029 = t7018 * t71
        t7038 = ut(t5,t1047,t174,n)
        t7041 = ut(t5,t1047,t179,n)
        t7045 = (t7038 - t6738) * t177 / 0.2E1 + (t6738 - t7041) * t177 
     #/ 0.2E1
        t7049 = (t1151 * t7045 - t341) * t124
        t7053 = (t349 - t366) * t124
        t7056 = ut(t5,t1111,t174,n)
        t7059 = ut(t5,t1111,t179,n)
        t7063 = (t7056 - t6756) * t177 / 0.2E1 + (t6756 - t7059) * t177 
     #/ 0.2E1
        t7067 = (-t1161 * t7063 + t364) * t124
        t7079 = (t193 - t196) * t177
        t7081 = ((t6878 - t193) * t177 - t7079) * t177
        t7086 = (t7079 - (t196 - t6883) * t177) * t177
        t7092 = (t1416 * t6878 - t493) * t177
        t7097 = (-t1456 * t6883 + t502) * t177
        t7105 = ut(t5,t121,t1364,n)
        t7108 = ut(t5,t126,t1364,n)
        t7112 = (t7105 - t6560) * t124 / 0.2E1 + (t6560 - t7108) * t124 
     #/ 0.2E1
        t7116 = (t1430 * t7112 - t459) * t177
        t7120 = (t463 - t478) * t177
        t7123 = ut(t5,t121,t1375,n)
        t7126 = ut(t5,t126,t1375,n)
        t7130 = (t7123 - t6579) * t124 / 0.2E1 + (t6579 - t7126) * t124 
     #/ 0.2E1
        t7134 = (-t1462 * t7130 + t476) * t177
        t7148 = (t7105 - t332) * t177
        t7153 = (t335 - t7123) * t177
        t7157 = (t7148 / 0.2E1 - t337 / 0.2E1) * t177 - (t334 / 0.2E1 - 
     #t7153 / 0.2E1) * t177
        t7163 = t342 * t6445
        t7167 = (t7108 - t355) * t177
        t7172 = (t358 - t7126) * t177
        t7176 = (t7167 / 0.2E1 - t360 / 0.2E1) * t177 - (t357 / 0.2E1 - 
     #t7172 / 0.2E1) * t177
        t7189 = (t141 - t144) * t124
        t7191 = ((t6951 - t141) * t124 - t7189) * t124
        t7196 = (t7189 - (t144 - t6956) * t124) * t124
        t7202 = (t1714 * t6951 - t316) * t124
        t7207 = (-t1726 * t6956 + t325) * t124
        t6732 = ((t1951 / 0.2E1 - t112 / 0.2E1) * t108 - t6466) * t108
        t7236 = t143 * t6732
        t6836 = t177 * t244
        t6843 = t177 * t287
        t7266 = -t868 * (((t1966 - t150) * t108 - t6990) * t108 / 0.2E1 
     #+ t6994 / 0.2E1) / 0.6E1 - t1046 * (((t7049 - t349) * t124 - t7053
     #) * t124 / 0.2E1 + (t7053 - (t366 - t7067) * t124) * t124 / 0.2E1)
     # / 0.6E1 - t1363 * ((t492 * t7081 - t501 * t7086) * t177 + ((t7092
     # - t504) * t177 - (t504 - t7097) * t177) * t177) / 0.24E2 - t1363 
     #* (((t7116 - t463) * t177 - t7120) * t177 / 0.2E1 + (t7120 - (t478
     # - t7134) * t177) * t177 / 0.2E1) / 0.6E1 + (t1580 * t193 - t1592 
     #* t196) * t177 + t151 + t169 + t203 + t220 - t1363 * ((t331 * t683
     #6 * t7157 - t7163) * t124 / 0.2E1 + (-t354 * t6843 * t7176 + t7163
     #) * t124 / 0.2E1) / 0.6E1 - t1046 * ((t315 * t7191 - t324 * t7196)
     # * t124 + ((t7202 - t327) * t124 - (t327 - t7207) * t124) * t124) 
     #/ 0.24E2 - t868 * (((t1979 - t202) * t108 - t6657) * t108 / 0.2E1 
     #+ t6661 / 0.2E1) / 0.6E1 - t868 * ((t251 * ((t2010 / 0.2E1 - t252 
     #/ 0.2E1) * t108 - t6525) * t108 - t7236) * t124 / 0.2E1 + (t7236 -
     # t292 * ((t2051 / 0.2E1 - t295 / 0.2E1) * t108 - t6543) * t108) * 
     #t124 / 0.2E1) / 0.6E1 - t868 * ((t104 * ((t1951 - t109) * t108 - t
     #6499) * t108 - t6504) * t108 + ((t1954 - t115) * t108 - t6512) * t
     #108) / 0.24E2 + t367
        t7267 = ut(t73,t1047,k,n)
        t7269 = (t7267 - t6738) * t108
        t7275 = (t1004 * (t7269 / 0.2E1 + t6740 / 0.2E1) - t256) * t124
        t7279 = (t262 - t301) * t124
        t7282 = ut(t73,t1111,k,n)
        t7284 = (t7282 - t6756) * t108
        t7290 = (t299 - t1045 * (t7284 / 0.2E1 + t6758 / 0.2E1)) * t124
        t7303 = ut(t73,j,t1364,n)
        t7305 = (t7303 - t6560) * t108
        t7311 = (t1534 * (t7305 / 0.2E1 + t6563 / 0.2E1) - t403) * t177
        t7315 = (t407 - t446) * t177
        t7318 = ut(t73,j,t1375,n)
        t7320 = (t7318 - t6579) * t108
        t7326 = (t444 - t1555 * (t7320 / 0.2E1 + t6582 / 0.2E1)) * t177
        t7339 = (t7267 - t122) * t124
        t7344 = (t127 - t7282) * t124
        t7359 = (t7038 - t332) * t124
        t7364 = (t355 - t7056) * t124
        t7368 = (t7359 / 0.2E1 - t455 / 0.2E1) * t124 - (t453 / 0.2E1 - 
     #t7364 / 0.2E1) * t124
        t7374 = t342 * t6497
        t7378 = (t7041 - t335) * t124
        t7383 = (t358 - t7059) * t124
        t7387 = (t7378 / 0.2E1 - t472 / 0.2E1) * t124 - (t470 / 0.2E1 - 
     #t7383 / 0.2E1) * t124
        t7406 = t195 * t6732
        t7423 = (t7303 - t175) * t177
        t7428 = (t180 - t7318) * t177
        t7090 = t124 * t391
        t7096 = t124 * t432
        t7442 = t408 + t447 + t464 + t479 - t1046 * (((t7275 - t262) * t
     #124 - t7279) * t124 / 0.2E1 + (t7279 - (t301 - t7290) * t124) * t1
     #24 / 0.2E1) / 0.6E1 + (t1177 * t141 - t1196 * t144) * t124 - t1363
     # * (((t7311 - t407) * t177 - t7315) * t177 / 0.2E1 + (t7315 - (t44
     #6 - t7326) * t177) * t177 / 0.2E1) / 0.6E1 + t263 + t302 + t350 + 
     #(t109 * t1483 - t6734) * t108 - t1046 * ((t128 * ((t7339 / 0.2E1 -
     # t129 / 0.2E1) * t124 - (t125 / 0.2E1 - t7344 / 0.2E1) * t124) * t
     #124 - t6963) * t108 / 0.2E1 + t6968 / 0.2E1) / 0.6E1 - t1046 * ((t
     #451 * t7090 * t7368 - t7374) * t177 / 0.2E1 + (-t468 * t7096 * t73
     #87 + t7374) * t177 / 0.2E1) / 0.6E1 - t868 * ((t396 * ((t2153 / 0.
     #2E1 - t399 / 0.2E1) * t108 - t6453) * t108 - t7406) * t177 / 0.2E1
     # + (t7406 - t435 * ((t2192 / 0.2E1 - t440 / 0.2E1) * t108 - t6481)
     # * t108) * t177 / 0.2E1) / 0.6E1 - t1363 * ((t181 * ((t7423 / 0.2E
     #1 - t182 / 0.2E1) * t177 - (t178 / 0.2E1 - t7428 / 0.2E1) * t177) 
     #* t177 - t6890) * t108 / 0.2E1 + t6895 / 0.2E1) / 0.6E1
        t7444 = t867 * (t7266 + t7442)
        t7448 = t6450 * t7444 / 0.4E1
        t7449 = t1944 * t863
        t7451 = t3384 * t7449 * t72
        t7453 = t7451 * t6447 / 0.12E2
        t7454 = t2735 * t529
        t7455 = t2372 - t7454
        t7456 = t7455 * t108
        t7457 = t538 * t7456
        t7460 = rx(t2386,t121,k,0,0)
        t7461 = rx(t2386,t121,k,1,1)
        t7463 = rx(t2386,t121,k,2,2)
        t7465 = rx(t2386,t121,k,1,2)
        t7467 = rx(t2386,t121,k,2,1)
        t7469 = rx(t2386,t121,k,0,1)
        t7470 = rx(t2386,t121,k,1,0)
        t7474 = rx(t2386,t121,k,2,0)
        t7476 = rx(t2386,t121,k,0,2)
        t7481 = t7460 * t7461 * t7463 - t7460 * t7465 * t7467 - t7461 * 
     #t7474 * t7476 - t7463 * t7469 * t7470 + t7465 * t7469 * t7474 + t7
     #467 * t7470 * t7476
        t7482 = 0.1E1 / t7481
        t7483 = t7460 ** 2
        t7484 = t7469 ** 2
        t7485 = t7476 ** 2
        t7487 = t7482 * (t7483 + t7484 + t7485)
        t7490 = t4 * (t4439 / 0.2E1 + t7487 / 0.2E1)
        t7491 = t7490 * t2488
        t7493 = (t4443 - t7491) * t108
        t7494 = t4 * t7482
        t7499 = u(t2386,t1047,k,n)
        t7501 = (t7499 - t2429) * t124
        t7503 = t7501 / 0.2E1 + t2431 / 0.2E1
        t7182 = t7494 * (t7460 * t7470 + t7461 * t7469 + t7465 * t7476)
        t7505 = t7182 * t7503
        t7507 = (t4449 - t7505) * t108
        t7508 = t7507 / 0.2E1
        t7512 = t7460 * t7474 + t7463 * t7476 + t7467 * t7469
        t7513 = u(t2386,t121,t174,n)
        t7515 = (t7513 - t2429) * t177
        t7516 = u(t2386,t121,t179,n)
        t7518 = (t2429 - t7516) * t177
        t7520 = t7515 / 0.2E1 + t7518 / 0.2E1
        t7192 = t7494 * t7512
        t7522 = t7192 * t7520
        t7524 = (t4458 - t7522) * t108
        t7525 = t7524 / 0.2E1
        t7526 = rx(t507,t1047,k,0,0)
        t7527 = rx(t507,t1047,k,1,1)
        t7529 = rx(t507,t1047,k,2,2)
        t7531 = rx(t507,t1047,k,1,2)
        t7533 = rx(t507,t1047,k,2,1)
        t7535 = rx(t507,t1047,k,0,1)
        t7536 = rx(t507,t1047,k,1,0)
        t7540 = rx(t507,t1047,k,2,0)
        t7542 = rx(t507,t1047,k,0,2)
        t7547 = t7526 * t7527 * t7529 - t7526 * t7531 * t7533 - t7527 * 
     #t7540 * t7542 - t7529 * t7535 * t7536 + t7531 * t7535 * t7540 + t7
     #533 * t7536 * t7542
        t7548 = 0.1E1 / t7547
        t7549 = t4 * t7548
        t7555 = (t3166 - t7499) * t108
        t7557 = t3213 / 0.2E1 + t7555 / 0.2E1
        t7214 = t7549 * (t7526 * t7536 + t7527 * t7535 + t7531 * t7542)
        t7559 = t7214 * t7557
        t7561 = (t7559 - t2492) * t124
        t7562 = t7561 / 0.2E1
        t7563 = t7536 ** 2
        t7564 = t7527 ** 2
        t7565 = t7531 ** 2
        t7567 = t7548 * (t7563 + t7564 + t7565)
        t7570 = t4 * (t7567 / 0.2E1 + t2541 / 0.2E1)
        t7571 = t7570 * t3168
        t7573 = (t7571 - t2550) * t124
        t7577 = t7527 * t7533 + t7529 * t7531 + t7536 * t7540
        t7578 = u(t507,t1047,t174,n)
        t7580 = (t7578 - t3166) * t177
        t7581 = u(t507,t1047,t179,n)
        t7583 = (t3166 - t7581) * t177
        t7585 = t7580 / 0.2E1 + t7583 / 0.2E1
        t7226 = t7549 * t7577
        t7587 = t7226 * t7585
        t7589 = (t7587 - t2575) * t124
        t7590 = t7589 / 0.2E1
        t7591 = rx(t507,t121,t174,0,0)
        t7592 = rx(t507,t121,t174,1,1)
        t7594 = rx(t507,t121,t174,2,2)
        t7596 = rx(t507,t121,t174,1,2)
        t7598 = rx(t507,t121,t174,2,1)
        t7600 = rx(t507,t121,t174,0,1)
        t7601 = rx(t507,t121,t174,1,0)
        t7605 = rx(t507,t121,t174,2,0)
        t7607 = rx(t507,t121,t174,0,2)
        t7612 = t7591 * t7592 * t7594 - t7591 * t7596 * t7598 - t7592 * 
     #t7605 * t7607 - t7594 * t7600 * t7601 + t7596 * t7600 * t7605 + t7
     #598 * t7601 * t7607
        t7613 = 0.1E1 / t7612
        t7614 = t4 * t7613
        t7618 = t7591 * t7605 + t7594 * t7607 + t7598 * t7600
        t7620 = (t2566 - t7513) * t108
        t7622 = t4493 / 0.2E1 + t7620 / 0.2E1
        t7247 = t7614 * t7618
        t7624 = t7247 * t7622
        t7626 = t4188 * t2490
        t7629 = (t7624 - t7626) * t177 / 0.2E1
        t7630 = rx(t507,t121,t179,0,0)
        t7631 = rx(t507,t121,t179,1,1)
        t7633 = rx(t507,t121,t179,2,2)
        t7635 = rx(t507,t121,t179,1,2)
        t7637 = rx(t507,t121,t179,2,1)
        t7639 = rx(t507,t121,t179,0,1)
        t7640 = rx(t507,t121,t179,1,0)
        t7644 = rx(t507,t121,t179,2,0)
        t7646 = rx(t507,t121,t179,0,2)
        t7651 = t7630 * t7631 * t7633 - t7630 * t7635 * t7637 - t7631 * 
     #t7644 * t7646 - t7633 * t7639 * t7640 + t7635 * t7639 * t7644 + t7
     #637 * t7640 * t7646
        t7652 = 0.1E1 / t7651
        t7653 = t4 * t7652
        t7657 = t7630 * t7644 + t7633 * t7646 + t7637 * t7639
        t7659 = (t2569 - t7516) * t108
        t7661 = t4532 / 0.2E1 + t7659 / 0.2E1
        t7270 = t7653 * t7657
        t7663 = t7270 * t7661
        t7666 = (t7626 - t7663) * t177 / 0.2E1
        t7670 = t7592 * t7598 + t7594 * t7596 + t7601 * t7605
        t7672 = (t7578 - t2566) * t124
        t7674 = t7672 / 0.2E1 + t2683 / 0.2E1
        t7280 = t7614 * t7670
        t7676 = t7280 * t7674
        t7678 = t2419 * t4447
        t7681 = (t7676 - t7678) * t177 / 0.2E1
        t7685 = t7631 * t7637 + t7633 * t7635 + t7640 * t7644
        t7687 = (t7581 - t2569) * t124
        t7689 = t7687 / 0.2E1 + t2700 / 0.2E1
        t7291 = t7653 * t7685
        t7691 = t7291 * t7689
        t7694 = (t7678 - t7691) * t177 / 0.2E1
        t7695 = t7605 ** 2
        t7696 = t7598 ** 2
        t7697 = t7594 ** 2
        t7699 = t7613 * (t7695 + t7696 + t7697)
        t7700 = t2473 ** 2
        t7701 = t2466 ** 2
        t7702 = t2462 ** 2
        t7704 = t2481 * (t7700 + t7701 + t7702)
        t7707 = t4 * (t7699 / 0.2E1 + t7704 / 0.2E1)
        t7708 = t7707 * t2568
        t7709 = t7644 ** 2
        t7710 = t7637 ** 2
        t7711 = t7633 ** 2
        t7713 = t7652 * (t7709 + t7710 + t7711)
        t7716 = t4 * (t7704 / 0.2E1 + t7713 / 0.2E1)
        t7717 = t7716 * t2571
        t7720 = t7493 + t4452 + t7508 + t4461 + t7525 + t7562 + t2499 + 
     #t7573 + t7590 + t2584 + t7629 + t7666 + t7681 + t7694 + (t7708 - t
     #7717) * t177
        t7721 = t7720 * t2480
        t7723 = (t7721 - t7454) * t124
        t7724 = rx(t2386,t126,k,0,0)
        t7725 = rx(t2386,t126,k,1,1)
        t7727 = rx(t2386,t126,k,2,2)
        t7729 = rx(t2386,t126,k,1,2)
        t7731 = rx(t2386,t126,k,2,1)
        t7733 = rx(t2386,t126,k,0,1)
        t7734 = rx(t2386,t126,k,1,0)
        t7738 = rx(t2386,t126,k,2,0)
        t7740 = rx(t2386,t126,k,0,2)
        t7745 = t7724 * t7725 * t7727 - t7724 * t7729 * t7731 - t7725 * 
     #t7738 * t7740 - t7727 * t7733 * t7734 + t7729 * t7733 * t7738 + t7
     #731 * t7734 * t7740
        t7746 = 0.1E1 / t7745
        t7747 = t7724 ** 2
        t7748 = t7733 ** 2
        t7749 = t7740 ** 2
        t7751 = t7746 * (t7747 + t7748 + t7749)
        t7754 = t4 * (t4597 / 0.2E1 + t7751 / 0.2E1)
        t7755 = t7754 * t2529
        t7757 = (t4601 - t7755) * t108
        t7758 = t4 * t7746
        t7763 = u(t2386,t1111,k,n)
        t7765 = (t2432 - t7763) * t124
        t7767 = t2434 / 0.2E1 + t7765 / 0.2E1
        t7332 = t7758 * (t7724 * t7734 + t7725 * t7733 + t7729 * t7740)
        t7769 = t7332 * t7767
        t7771 = (t4607 - t7769) * t108
        t7772 = t7771 / 0.2E1
        t7776 = t7724 * t7738 + t7727 * t7740 + t7731 * t7733
        t7777 = u(t2386,t126,t174,n)
        t7779 = (t7777 - t2432) * t177
        t7780 = u(t2386,t126,t179,n)
        t7782 = (t2432 - t7780) * t177
        t7784 = t7779 / 0.2E1 + t7782 / 0.2E1
        t7341 = t7758 * t7776
        t7786 = t7341 * t7784
        t7788 = (t4616 - t7786) * t108
        t7789 = t7788 / 0.2E1
        t7790 = rx(t507,t1111,k,0,0)
        t7791 = rx(t507,t1111,k,1,1)
        t7793 = rx(t507,t1111,k,2,2)
        t7795 = rx(t507,t1111,k,1,2)
        t7797 = rx(t507,t1111,k,2,1)
        t7799 = rx(t507,t1111,k,0,1)
        t7800 = rx(t507,t1111,k,1,0)
        t7804 = rx(t507,t1111,k,2,0)
        t7806 = rx(t507,t1111,k,0,2)
        t7811 = t7790 * t7791 * t7793 - t7790 * t7795 * t7797 - t7791 * 
     #t7804 * t7806 - t7793 * t7799 * t7800 + t7795 * t7799 * t7804 + t7
     #797 * t7800 * t7806
        t7812 = 0.1E1 / t7811
        t7813 = t4 * t7812
        t7819 = (t3172 - t7763) * t108
        t7821 = t3231 / 0.2E1 + t7819 / 0.2E1
        t7362 = t7813 * (t7790 * t7800 + t7791 * t7799 + t7795 * t7806)
        t7823 = t7362 * t7821
        t7825 = (t2533 - t7823) * t124
        t7826 = t7825 / 0.2E1
        t7827 = t7800 ** 2
        t7828 = t7791 ** 2
        t7829 = t7795 ** 2
        t7831 = t7812 * (t7827 + t7828 + t7829)
        t7834 = t4 * (t2555 / 0.2E1 + t7831 / 0.2E1)
        t7835 = t7834 * t3174
        t7837 = (t2559 - t7835) * t124
        t7841 = t7791 * t7797 + t7793 * t7795 + t7800 * t7804
        t7842 = u(t507,t1111,t174,n)
        t7844 = (t7842 - t3172) * t177
        t7845 = u(t507,t1111,t179,n)
        t7847 = (t3172 - t7845) * t177
        t7849 = t7844 / 0.2E1 + t7847 / 0.2E1
        t7377 = t7813 * t7841
        t7851 = t7377 * t7849
        t7853 = (t2598 - t7851) * t124
        t7854 = t7853 / 0.2E1
        t7855 = rx(t507,t126,t174,0,0)
        t7856 = rx(t507,t126,t174,1,1)
        t7858 = rx(t507,t126,t174,2,2)
        t7860 = rx(t507,t126,t174,1,2)
        t7862 = rx(t507,t126,t174,2,1)
        t7864 = rx(t507,t126,t174,0,1)
        t7865 = rx(t507,t126,t174,1,0)
        t7869 = rx(t507,t126,t174,2,0)
        t7871 = rx(t507,t126,t174,0,2)
        t7876 = t7855 * t7856 * t7858 - t7855 * t7860 * t7862 - t7856 * 
     #t7869 * t7871 - t7858 * t7864 * t7865 + t7860 * t7864 * t7869 + t7
     #862 * t7865 * t7871
        t7877 = 0.1E1 / t7876
        t7878 = t4 * t7877
        t7882 = t7855 * t7869 + t7858 * t7871 + t7862 * t7864
        t7884 = (t2589 - t7777) * t108
        t7886 = t4651 / 0.2E1 + t7884 / 0.2E1
        t7399 = t7878 * t7882
        t7888 = t7399 * t7886
        t7890 = t4327 * t2531
        t7893 = (t7888 - t7890) * t177 / 0.2E1
        t7894 = rx(t507,t126,t179,0,0)
        t7895 = rx(t507,t126,t179,1,1)
        t7897 = rx(t507,t126,t179,2,2)
        t7899 = rx(t507,t126,t179,1,2)
        t7901 = rx(t507,t126,t179,2,1)
        t7903 = rx(t507,t126,t179,0,1)
        t7904 = rx(t507,t126,t179,1,0)
        t7908 = rx(t507,t126,t179,2,0)
        t7910 = rx(t507,t126,t179,0,2)
        t7915 = t7894 * t7895 * t7897 - t7894 * t7899 * t7901 - t7895 * 
     #t7908 * t7910 - t7897 * t7903 * t7904 + t7899 * t7903 * t7908 + t7
     #901 * t7904 * t7910
        t7916 = 0.1E1 / t7915
        t7917 = t4 * t7916
        t7921 = t7894 * t7908 + t7897 * t7910 + t7901 * t7903
        t7923 = (t2592 - t7780) * t108
        t7925 = t4690 / 0.2E1 + t7923 / 0.2E1
        t7420 = t7917 * t7921
        t7927 = t7420 * t7925
        t7930 = (t7890 - t7927) * t177 / 0.2E1
        t7934 = t7856 * t7862 + t7858 * t7860 + t7865 * t7869
        t7936 = (t2589 - t7842) * t124
        t7938 = t2685 / 0.2E1 + t7936 / 0.2E1
        t7430 = t7878 * t7934
        t7940 = t7430 * t7938
        t7942 = t2444 * t4605
        t7945 = (t7940 - t7942) * t177 / 0.2E1
        t7949 = t7895 * t7901 + t7897 * t7899 + t7904 * t7908
        t7951 = (t2592 - t7845) * t124
        t7953 = t2702 / 0.2E1 + t7951 / 0.2E1
        t7438 = t7917 * t7949
        t7955 = t7438 * t7953
        t7958 = (t7942 - t7955) * t177 / 0.2E1
        t7959 = t7869 ** 2
        t7960 = t7862 ** 2
        t7961 = t7858 ** 2
        t7963 = t7877 * (t7959 + t7960 + t7961)
        t7964 = t2514 ** 2
        t7965 = t2507 ** 2
        t7966 = t2503 ** 2
        t7968 = t2522 * (t7964 + t7965 + t7966)
        t7971 = t4 * (t7963 / 0.2E1 + t7968 / 0.2E1)
        t7972 = t7971 * t2591
        t7973 = t7908 ** 2
        t7974 = t7901 ** 2
        t7975 = t7897 ** 2
        t7977 = t7916 * (t7973 + t7974 + t7975)
        t7980 = t4 * (t7968 / 0.2E1 + t7977 / 0.2E1)
        t7981 = t7980 * t2594
        t7984 = t7757 + t4610 + t7772 + t4619 + t7789 + t2536 + t7826 + 
     #t7837 + t2601 + t7854 + t7893 + t7930 + t7945 + t7958 + (t7972 - t
     #7981) * t177
        t7985 = t7984 * t2521
        t7987 = (t7454 - t7985) * t124
        t7989 = t7723 / 0.2E1 + t7987 / 0.2E1
        t7991 = t547 * t7989
        t7994 = (t4754 - t7991) * t108 / 0.2E1
        t7995 = rx(t2386,j,t174,0,0)
        t7996 = rx(t2386,j,t174,1,1)
        t7998 = rx(t2386,j,t174,2,2)
        t8000 = rx(t2386,j,t174,1,2)
        t8002 = rx(t2386,j,t174,2,1)
        t8004 = rx(t2386,j,t174,0,1)
        t8005 = rx(t2386,j,t174,1,0)
        t8009 = rx(t2386,j,t174,2,0)
        t8011 = rx(t2386,j,t174,0,2)
        t8016 = t7995 * t7996 * t7998 - t7995 * t8000 * t8002 - t7996 * 
     #t8009 * t8011 - t7998 * t8004 * t8005 + t8000 * t8004 * t8009 + t8
     #002 * t8005 * t8011
        t8017 = 0.1E1 / t8016
        t8018 = t7995 ** 2
        t8019 = t8004 ** 2
        t8020 = t8011 ** 2
        t8022 = t8017 * (t8018 + t8019 + t8020)
        t8025 = t4 * (t5437 / 0.2E1 + t8022 / 0.2E1)
        t8026 = t8025 * t2631
        t8028 = (t5441 - t8026) * t108
        t8029 = t4 * t8017
        t8033 = t7995 * t8005 + t7996 * t8004 + t8000 * t8011
        t8035 = (t7513 - t2446) * t124
        t8037 = (t2446 - t7777) * t124
        t8039 = t8035 / 0.2E1 + t8037 / 0.2E1
        t7514 = t8029 * t8033
        t8041 = t7514 * t8039
        t8043 = (t5449 - t8041) * t108
        t8044 = t8043 / 0.2E1
        t8049 = u(t2386,j,t1364,n)
        t8051 = (t8049 - t2446) * t177
        t8053 = t8051 / 0.2E1 + t2448 / 0.2E1
        t7532 = t8029 * (t7995 * t8009 + t7998 * t8011 + t8002 * t8004)
        t8055 = t7532 * t8053
        t8057 = (t5456 - t8055) * t108
        t8058 = t8057 / 0.2E1
        t8062 = t7591 * t7601 + t7592 * t7600 + t7596 * t7607
        t7541 = t7614 * t8062
        t8064 = t7541 * t7622
        t8066 = t5140 * t2633
        t8069 = (t8064 - t8066) * t124 / 0.2E1
        t8073 = t7855 * t7865 + t7856 * t7864 + t7860 * t7871
        t7551 = t7878 * t8073
        t8075 = t7551 * t7886
        t8078 = (t8066 - t8075) * t124 / 0.2E1
        t8079 = t7601 ** 2
        t8080 = t7592 ** 2
        t8081 = t7596 ** 2
        t8083 = t7613 * (t8079 + t8080 + t8081)
        t8084 = t2612 ** 2
        t8085 = t2603 ** 2
        t8086 = t2607 ** 2
        t8088 = t2624 * (t8084 + t8085 + t8086)
        t8091 = t4 * (t8083 / 0.2E1 + t8088 / 0.2E1)
        t8092 = t8091 * t2683
        t8093 = t7865 ** 2
        t8094 = t7856 ** 2
        t8095 = t7860 ** 2
        t8097 = t7877 * (t8093 + t8094 + t8095)
        t8100 = t4 * (t8088 / 0.2E1 + t8097 / 0.2E1)
        t8101 = t8100 * t2685
        t8104 = u(t507,t121,t1364,n)
        t8106 = (t8104 - t2566) * t177
        t8108 = t8106 / 0.2E1 + t2568 / 0.2E1
        t8110 = t7280 * t8108
        t8112 = t2540 * t5454
        t8115 = (t8110 - t8112) * t124 / 0.2E1
        t8116 = u(t507,t126,t1364,n)
        t8118 = (t8116 - t2589) * t177
        t8120 = t8118 / 0.2E1 + t2591 / 0.2E1
        t8122 = t7430 * t8120
        t8125 = (t8112 - t8122) * t124 / 0.2E1
        t8126 = rx(t507,j,t1364,0,0)
        t8127 = rx(t507,j,t1364,1,1)
        t8129 = rx(t507,j,t1364,2,2)
        t8131 = rx(t507,j,t1364,1,2)
        t8133 = rx(t507,j,t1364,2,1)
        t8135 = rx(t507,j,t1364,0,1)
        t8136 = rx(t507,j,t1364,1,0)
        t8140 = rx(t507,j,t1364,2,0)
        t8142 = rx(t507,j,t1364,0,2)
        t8147 = t8126 * t8127 * t8129 - t8126 * t8131 * t8133 - t8127 * 
     #t8140 * t8142 - t8129 * t8135 * t8136 + t8131 * t8135 * t8140 + t8
     #133 * t8136 * t8142
        t8148 = 0.1E1 / t8147
        t8149 = t4 * t8148
        t8155 = (t3115 - t8049) * t108
        t8157 = t3117 / 0.2E1 + t8155 / 0.2E1
        t7623 = t8149 * (t8126 * t8140 + t8129 * t8142 + t8133 * t8135)
        t8159 = t7623 * t8157
        t8161 = (t8159 - t2635) * t177
        t8162 = t8161 / 0.2E1
        t8166 = t8127 * t8133 + t8129 * t8131 + t8136 * t8140
        t8168 = (t8104 - t3115) * t124
        t8170 = (t3115 - t8116) * t124
        t8172 = t8168 / 0.2E1 + t8170 / 0.2E1
        t7641 = t8149 * t8166
        t8174 = t7641 * t8172
        t8176 = (t8174 - t2689) * t177
        t8177 = t8176 / 0.2E1
        t8178 = t8140 ** 2
        t8179 = t8133 ** 2
        t8180 = t8129 ** 2
        t8182 = t8148 * (t8178 + t8179 + t8180)
        t8185 = t4 * (t8182 / 0.2E1 + t2714 / 0.2E1)
        t8186 = t8185 * t3189
        t8188 = (t8186 - t2723) * t177
        t8189 = t8028 + t5452 + t8044 + t5459 + t8058 + t8069 + t8078 + 
     #(t8092 - t8101) * t124 + t8115 + t8125 + t8162 + t2640 + t8177 + t
     #2694 + t8188
        t8190 = t8189 * t2623
        t8192 = (t8190 - t7454) * t177
        t8193 = rx(t2386,j,t179,0,0)
        t8194 = rx(t2386,j,t179,1,1)
        t8196 = rx(t2386,j,t179,2,2)
        t8198 = rx(t2386,j,t179,1,2)
        t8200 = rx(t2386,j,t179,2,1)
        t8202 = rx(t2386,j,t179,0,1)
        t8203 = rx(t2386,j,t179,1,0)
        t8207 = rx(t2386,j,t179,2,0)
        t8209 = rx(t2386,j,t179,0,2)
        t8214 = t8193 * t8194 * t8196 - t8193 * t8198 * t8200 - t8194 * 
     #t8207 * t8209 - t8196 * t8202 * t8203 + t8198 * t8202 * t8207 + t8
     #200 * t8203 * t8209
        t8215 = 0.1E1 / t8214
        t8216 = t8193 ** 2
        t8217 = t8202 ** 2
        t8218 = t8209 ** 2
        t8220 = t8215 * (t8216 + t8217 + t8218)
        t8223 = t4 * (t5531 / 0.2E1 + t8220 / 0.2E1)
        t8224 = t8223 * t2670
        t8226 = (t5535 - t8224) * t108
        t8227 = t4 * t8215
        t8231 = t8193 * t8203 + t8194 * t8202 + t8198 * t8209
        t8233 = (t7516 - t2449) * t124
        t8235 = (t2449 - t7780) * t124
        t8237 = t8233 / 0.2E1 + t8235 / 0.2E1
        t7692 = t8227 * t8231
        t8239 = t7692 * t8237
        t8241 = (t5543 - t8239) * t108
        t8242 = t8241 / 0.2E1
        t8247 = u(t2386,j,t1375,n)
        t8249 = (t2449 - t8247) * t177
        t8251 = t2451 / 0.2E1 + t8249 / 0.2E1
        t7714 = t8227 * (t8193 * t8207 + t8196 * t8209 + t8200 * t8202)
        t8253 = t7714 * t8251
        t8255 = (t5550 - t8253) * t108
        t8256 = t8255 / 0.2E1
        t8260 = t7630 * t7640 + t7631 * t7639 + t7635 * t7646
        t7726 = t7653 * t8260
        t8262 = t7726 * t7661
        t8264 = t5187 * t2672
        t8267 = (t8262 - t8264) * t124 / 0.2E1
        t8271 = t7894 * t7904 + t7895 * t7903 + t7899 * t7910
        t7737 = t7917 * t8271
        t8273 = t7737 * t7925
        t8276 = (t8264 - t8273) * t124 / 0.2E1
        t8277 = t7640 ** 2
        t8278 = t7631 ** 2
        t8279 = t7635 ** 2
        t8281 = t7652 * (t8277 + t8278 + t8279)
        t8282 = t2651 ** 2
        t8283 = t2642 ** 2
        t8284 = t2646 ** 2
        t8286 = t2663 * (t8282 + t8283 + t8284)
        t8289 = t4 * (t8281 / 0.2E1 + t8286 / 0.2E1)
        t8290 = t8289 * t2700
        t8291 = t7904 ** 2
        t8292 = t7895 ** 2
        t8293 = t7899 ** 2
        t8295 = t7916 * (t8291 + t8292 + t8293)
        t8298 = t4 * (t8286 / 0.2E1 + t8295 / 0.2E1)
        t8299 = t8298 * t2702
        t8302 = u(t507,t121,t1375,n)
        t8304 = (t2569 - t8302) * t177
        t8306 = t2571 / 0.2E1 + t8304 / 0.2E1
        t8308 = t7291 * t8306
        t8310 = t2562 * t5548
        t8313 = (t8308 - t8310) * t124 / 0.2E1
        t8314 = u(t507,t126,t1375,n)
        t8316 = (t2592 - t8314) * t177
        t8318 = t2594 / 0.2E1 + t8316 / 0.2E1
        t8320 = t7438 * t8318
        t8323 = (t8310 - t8320) * t124 / 0.2E1
        t8324 = rx(t507,j,t1375,0,0)
        t8325 = rx(t507,j,t1375,1,1)
        t8327 = rx(t507,j,t1375,2,2)
        t8329 = rx(t507,j,t1375,1,2)
        t8331 = rx(t507,j,t1375,2,1)
        t8333 = rx(t507,j,t1375,0,1)
        t8334 = rx(t507,j,t1375,1,0)
        t8338 = rx(t507,j,t1375,2,0)
        t8340 = rx(t507,j,t1375,0,2)
        t8345 = t8324 * t8325 * t8327 - t8324 * t8329 * t8331 - t8325 * 
     #t8338 * t8340 - t8327 * t8333 * t8334 + t8329 * t8333 * t8338 + t8
     #331 * t8334 * t8340
        t8346 = 0.1E1 / t8345
        t8347 = t4 * t8346
        t8353 = (t3135 - t8247) * t108
        t8355 = t3137 / 0.2E1 + t8353 / 0.2E1
        t7809 = t8347 * (t8324 * t8338 + t8327 * t8340 + t8331 * t8333)
        t8357 = t7809 * t8355
        t8359 = (t2674 - t8357) * t177
        t8360 = t8359 / 0.2E1
        t8364 = t8325 * t8331 + t8327 * t8329 + t8334 * t8338
        t8366 = (t8302 - t3135) * t124
        t8368 = (t3135 - t8314) * t124
        t8370 = t8366 / 0.2E1 + t8368 / 0.2E1
        t7824 = t8347 * t8364
        t8372 = t7824 * t8370
        t8374 = (t2706 - t8372) * t177
        t8375 = t8374 / 0.2E1
        t8376 = t8338 ** 2
        t8377 = t8331 ** 2
        t8378 = t8327 ** 2
        t8380 = t8346 * (t8376 + t8377 + t8378)
        t8383 = t4 * (t2728 / 0.2E1 + t8380 / 0.2E1)
        t8384 = t8383 * t3194
        t8386 = (t2732 - t8384) * t177
        t8387 = t8226 + t5546 + t8242 + t5553 + t8256 + t8267 + t8276 + 
     #(t8290 - t8299) * t124 + t8313 + t8323 + t2677 + t8360 + t2709 + t
     #8375 + t8386
        t8388 = t8387 * t2662
        t8390 = (t7454 - t8388) * t177
        t8392 = t8192 / 0.2E1 + t8390 / 0.2E1
        t8394 = t564 * t8392
        t8397 = (t5624 - t8394) * t108 / 0.2E1
        t8399 = (t4590 - t7721) * t108
        t8401 = t5631 / 0.2E1 + t8399 / 0.2E1
        t8403 = t604 * t8401
        t8405 = t3488 / 0.2E1 + t7456 / 0.2E1
        t8407 = t161 * t8405
        t8410 = (t8403 - t8407) * t124 / 0.2E1
        t8412 = (t4748 - t7985) * t108
        t8414 = t5646 / 0.2E1 + t8412 / 0.2E1
        t8416 = t641 * t8414
        t8419 = (t8407 - t8416) * t124 / 0.2E1
        t8420 = t670 * t4592
        t8421 = t679 * t4750
        t8424 = t7591 ** 2
        t8425 = t7600 ** 2
        t8426 = t7607 ** 2
        t8428 = t7613 * (t8424 + t8425 + t8426)
        t8431 = t4 * (t5676 / 0.2E1 + t8428 / 0.2E1)
        t8432 = t8431 * t4493
        t8436 = t7541 * t7674
        t8439 = (t5691 - t8436) * t108 / 0.2E1
        t8441 = t7247 * t8108
        t8444 = (t5703 - t8441) * t108 / 0.2E1
        t8445 = rx(i,t1047,t174,0,0)
        t8446 = rx(i,t1047,t174,1,1)
        t8448 = rx(i,t1047,t174,2,2)
        t8450 = rx(i,t1047,t174,1,2)
        t8452 = rx(i,t1047,t174,2,1)
        t8454 = rx(i,t1047,t174,0,1)
        t8455 = rx(i,t1047,t174,1,0)
        t8459 = rx(i,t1047,t174,2,0)
        t8461 = rx(i,t1047,t174,0,2)
        t8466 = t8445 * t8446 * t8448 - t8445 * t8450 * t8452 - t8446 * 
     #t8459 * t8461 - t8448 * t8454 * t8455 + t8450 * t8454 * t8459 + t8
     #452 * t8455 * t8461
        t8467 = 0.1E1 / t8466
        t8468 = t4 * t8467
        t8472 = t8445 * t8455 + t8446 * t8454 + t8450 * t8461
        t8474 = (t2885 - t7578) * t108
        t8476 = t5738 / 0.2E1 + t8474 / 0.2E1
        t7914 = t8468 * t8472
        t8478 = t7914 * t8476
        t8480 = (t8478 - t5465) * t124
        t8481 = t8480 / 0.2E1
        t8482 = t8455 ** 2
        t8483 = t8446 ** 2
        t8484 = t8450 ** 2
        t8486 = t8467 * (t8482 + t8483 + t8484)
        t8489 = t4 * (t8486 / 0.2E1 + t5484 / 0.2E1)
        t8490 = t8489 * t2980
        t8492 = (t8490 - t5493) * t124
        t8497 = u(i,t1047,t1364,n)
        t8499 = (t8497 - t2885) * t177
        t8501 = t8499 / 0.2E1 + t2887 / 0.2E1
        t7933 = t8468 * (t8446 * t8452 + t8448 * t8450 + t8455 * t8459)
        t8503 = t7933 * t8501
        t8505 = (t8503 - t5508) * t124
        t8506 = t8505 / 0.2E1
        t8507 = rx(i,t121,t1364,0,0)
        t8508 = rx(i,t121,t1364,1,1)
        t8510 = rx(i,t121,t1364,2,2)
        t8512 = rx(i,t121,t1364,1,2)
        t8514 = rx(i,t121,t1364,2,1)
        t8516 = rx(i,t121,t1364,0,1)
        t8517 = rx(i,t121,t1364,1,0)
        t8521 = rx(i,t121,t1364,2,0)
        t8523 = rx(i,t121,t1364,0,2)
        t8528 = t8507 * t8508 * t8510 - t8507 * t8512 * t8514 - t8508 * 
     #t8521 * t8523 - t8510 * t8516 * t8517 + t8512 * t8516 * t8521 + t8
     #514 * t8517 * t8523
        t8529 = 0.1E1 / t8528
        t8530 = t4 * t8529
        t8534 = t8507 * t8521 + t8510 * t8523 + t8514 * t8516
        t8536 = (t2936 - t8104) * t108
        t8538 = t5802 / 0.2E1 + t8536 / 0.2E1
        t7976 = t8530 * t8534
        t8540 = t7976 * t8538
        t8542 = (t8540 - t4497) * t177
        t8543 = t8542 / 0.2E1
        t8549 = (t8497 - t2936) * t124
        t8551 = t8549 / 0.2E1 + t3315 / 0.2E1
        t7990 = t8530 * (t8508 * t8514 + t8510 * t8512 + t8517 * t8521)
        t8553 = t7990 * t8551
        t8555 = (t8553 - t4547) * t177
        t8556 = t8555 / 0.2E1
        t8557 = t8521 ** 2
        t8558 = t8514 ** 2
        t8559 = t8510 ** 2
        t8561 = t8529 * (t8557 + t8558 + t8559)
        t8564 = t4 * (t8561 / 0.2E1 + t4568 / 0.2E1)
        t8565 = t8564 * t2938
        t8567 = (t8565 - t4577) * t177
        t8568 = (t5680 - t8432) * t108 + t5694 + t8439 + t5706 + t8444 +
     # t8481 + t5470 + t8492 + t8506 + t5513 + t8543 + t4502 + t8556 + t
     #4552 + t8567
        t8569 = t8568 * t4485
        t8571 = (t8569 - t4590) * t177
        t8572 = t7630 ** 2
        t8573 = t7639 ** 2
        t8574 = t7646 ** 2
        t8576 = t7652 * (t8572 + t8573 + t8574)
        t8579 = t4 * (t5856 / 0.2E1 + t8576 / 0.2E1)
        t8580 = t8579 * t4532
        t8584 = t7726 * t7689
        t8587 = (t5871 - t8584) * t108 / 0.2E1
        t8589 = t7270 * t8306
        t8592 = (t5883 - t8589) * t108 / 0.2E1
        t8593 = rx(i,t1047,t179,0,0)
        t8594 = rx(i,t1047,t179,1,1)
        t8596 = rx(i,t1047,t179,2,2)
        t8598 = rx(i,t1047,t179,1,2)
        t8600 = rx(i,t1047,t179,2,1)
        t8602 = rx(i,t1047,t179,0,1)
        t8603 = rx(i,t1047,t179,1,0)
        t8607 = rx(i,t1047,t179,2,0)
        t8609 = rx(i,t1047,t179,0,2)
        t8614 = t8593 * t8594 * t8596 - t8593 * t8598 * t8600 - t8594 * 
     #t8607 * t8609 - t8596 * t8602 * t8603 + t8598 * t8602 * t8607 + t8
     #600 * t8603 * t8609
        t8615 = 0.1E1 / t8614
        t8616 = t4 * t8615
        t8620 = t8593 * t8603 + t8594 * t8602 + t8598 * t8609
        t8622 = (t2888 - t7581) * t108
        t8624 = t5918 / 0.2E1 + t8622 / 0.2E1
        t8056 = t8616 * t8620
        t8626 = t8056 * t8624
        t8628 = (t8626 - t5559) * t124
        t8629 = t8628 / 0.2E1
        t8630 = t8603 ** 2
        t8631 = t8594 ** 2
        t8632 = t8598 ** 2
        t8634 = t8615 * (t8630 + t8631 + t8632)
        t8637 = t4 * (t8634 / 0.2E1 + t5578 / 0.2E1)
        t8638 = t8637 * t2999
        t8640 = (t8638 - t5587) * t124
        t8645 = u(i,t1047,t1375,n)
        t8647 = (t2888 - t8645) * t177
        t8649 = t2890 / 0.2E1 + t8647 / 0.2E1
        t8074 = t8616 * (t8594 * t8600 + t8596 * t8598 + t8603 * t8607)
        t8651 = t8074 * t8649
        t8653 = (t8651 - t5602) * t124
        t8654 = t8653 / 0.2E1
        t8655 = rx(i,t121,t1375,0,0)
        t8656 = rx(i,t121,t1375,1,1)
        t8658 = rx(i,t121,t1375,2,2)
        t8660 = rx(i,t121,t1375,1,2)
        t8662 = rx(i,t121,t1375,2,1)
        t8664 = rx(i,t121,t1375,0,1)
        t8665 = rx(i,t121,t1375,1,0)
        t8669 = rx(i,t121,t1375,2,0)
        t8671 = rx(i,t121,t1375,0,2)
        t8676 = t8655 * t8656 * t8658 - t8655 * t8660 * t8662 - t8656 * 
     #t8669 * t8671 - t8658 * t8664 * t8665 + t8660 * t8664 * t8669 + t8
     #662 * t8665 * t8671
        t8677 = 0.1E1 / t8676
        t8678 = t4 * t8677
        t8682 = t8655 * t8669 + t8658 * t8671 + t8662 * t8664
        t8684 = (t2942 - t8302) * t108
        t8686 = t5982 / 0.2E1 + t8684 / 0.2E1
        t8119 = t8678 * t8682
        t8688 = t8119 * t8686
        t8690 = (t4536 - t8688) * t177
        t8691 = t8690 / 0.2E1
        t8697 = (t8645 - t2942) * t124
        t8699 = t8697 / 0.2E1 + t3335 / 0.2E1
        t8134 = t8678 * (t8656 * t8662 + t8658 * t8660 + t8665 * t8669)
        t8701 = t8134 * t8699
        t8703 = (t4560 - t8701) * t177
        t8704 = t8703 / 0.2E1
        t8705 = t8669 ** 2
        t8706 = t8662 ** 2
        t8707 = t8658 ** 2
        t8709 = t8677 * (t8705 + t8706 + t8707)
        t8712 = t4 * (t4582 / 0.2E1 + t8709 / 0.2E1)
        t8713 = t8712 * t2944
        t8715 = (t4586 - t8713) * t177
        t8716 = (t5860 - t8580) * t108 + t5874 + t8587 + t5886 + t8592 +
     # t8629 + t5564 + t8640 + t8654 + t5607 + t4539 + t8691 + t4563 + t
     #8704 + t8715
        t8717 = t8716 * t4524
        t8719 = (t4590 - t8717) * t177
        t8721 = t8571 / 0.2E1 + t8719 / 0.2E1
        t8723 = t683 * t8721
        t8725 = t691 * t5622
        t8728 = (t8723 - t8725) * t124 / 0.2E1
        t8729 = t7855 ** 2
        t8730 = t7864 ** 2
        t8731 = t7871 ** 2
        t8733 = t7877 * (t8729 + t8730 + t8731)
        t8736 = t4 * (t6045 / 0.2E1 + t8733 / 0.2E1)
        t8737 = t8736 * t4651
        t8741 = t7551 * t7938
        t8744 = (t6060 - t8741) * t108 / 0.2E1
        t8746 = t7399 * t8120
        t8749 = (t6072 - t8746) * t108 / 0.2E1
        t8750 = rx(i,t1111,t174,0,0)
        t8751 = rx(i,t1111,t174,1,1)
        t8753 = rx(i,t1111,t174,2,2)
        t8755 = rx(i,t1111,t174,1,2)
        t8757 = rx(i,t1111,t174,2,1)
        t8759 = rx(i,t1111,t174,0,1)
        t8760 = rx(i,t1111,t174,1,0)
        t8764 = rx(i,t1111,t174,2,0)
        t8766 = rx(i,t1111,t174,0,2)
        t8771 = t8750 * t8751 * t8753 - t8750 * t8755 * t8757 - t8751 * 
     #t8764 * t8766 - t8753 * t8759 * t8760 + t8755 * t8759 * t8764 + t8
     #757 * t8760 * t8766
        t8772 = 0.1E1 / t8771
        t8773 = t4 * t8772
        t8777 = t8750 * t8760 + t8751 * t8759 + t8755 * t8766
        t8779 = (t2908 - t7842) * t108
        t8781 = t6107 / 0.2E1 + t8779 / 0.2E1
        t8208 = t8773 * t8777
        t8783 = t8208 * t8781
        t8785 = (t5476 - t8783) * t124
        t8786 = t8785 / 0.2E1
        t8787 = t8760 ** 2
        t8788 = t8751 ** 2
        t8789 = t8755 ** 2
        t8791 = t8772 * (t8787 + t8788 + t8789)
        t8794 = t4 * (t5498 / 0.2E1 + t8791 / 0.2E1)
        t8795 = t8794 * t2985
        t8797 = (t5502 - t8795) * t124
        t8802 = u(i,t1111,t1364,n)
        t8804 = (t8802 - t2908) * t177
        t8806 = t8804 / 0.2E1 + t2910 / 0.2E1
        t8230 = t8773 * (t8751 * t8757 + t8753 * t8755 + t8760 * t8764)
        t8808 = t8230 * t8806
        t8810 = (t5517 - t8808) * t124
        t8811 = t8810 / 0.2E1
        t8812 = rx(i,t126,t1364,0,0)
        t8813 = rx(i,t126,t1364,1,1)
        t8815 = rx(i,t126,t1364,2,2)
        t8817 = rx(i,t126,t1364,1,2)
        t8819 = rx(i,t126,t1364,2,1)
        t8821 = rx(i,t126,t1364,0,1)
        t8822 = rx(i,t126,t1364,1,0)
        t8826 = rx(i,t126,t1364,2,0)
        t8828 = rx(i,t126,t1364,0,2)
        t8833 = t8812 * t8813 * t8815 - t8812 * t8817 * t8819 - t8813 * 
     #t8826 * t8828 - t8815 * t8821 * t8822 + t8817 * t8821 * t8826 + t8
     #819 * t8822 * t8828
        t8834 = 0.1E1 / t8833
        t8835 = t4 * t8834
        t8839 = t8812 * t8826 + t8815 * t8828 + t8819 * t8821
        t8841 = (t2957 - t8116) * t108
        t8843 = t6171 / 0.2E1 + t8841 / 0.2E1
        t8266 = t8835 * t8839
        t8845 = t8266 * t8843
        t8847 = (t8845 - t4655) * t177
        t8848 = t8847 / 0.2E1
        t8854 = (t2957 - t8802) * t124
        t8856 = t3317 / 0.2E1 + t8854 / 0.2E1
        t8280 = t8835 * (t8813 * t8819 + t8815 * t8817 + t8822 * t8826)
        t8858 = t8280 * t8856
        t8860 = (t8858 - t4705) * t177
        t8861 = t8860 / 0.2E1
        t8862 = t8826 ** 2
        t8863 = t8819 ** 2
        t8864 = t8815 ** 2
        t8866 = t8834 * (t8862 + t8863 + t8864)
        t8869 = t4 * (t8866 / 0.2E1 + t4726 / 0.2E1)
        t8870 = t8869 * t2959
        t8872 = (t8870 - t4735) * t177
        t8873 = (t6049 - t8737) * t108 + t6063 + t8744 + t6075 + t8749 +
     # t5479 + t8786 + t8797 + t5520 + t8811 + t8848 + t4660 + t8861 + t
     #4710 + t8872
        t8874 = t8873 * t4643
        t8876 = (t8874 - t4748) * t177
        t8877 = t7894 ** 2
        t8878 = t7903 ** 2
        t8879 = t7910 ** 2
        t8881 = t7916 * (t8877 + t8878 + t8879)
        t8884 = t4 * (t6225 / 0.2E1 + t8881 / 0.2E1)
        t8885 = t8884 * t4690
        t8889 = t7737 * t7953
        t8892 = (t6240 - t8889) * t108 / 0.2E1
        t8894 = t7420 * t8318
        t8897 = (t6252 - t8894) * t108 / 0.2E1
        t8898 = rx(i,t1111,t179,0,0)
        t8899 = rx(i,t1111,t179,1,1)
        t8901 = rx(i,t1111,t179,2,2)
        t8903 = rx(i,t1111,t179,1,2)
        t8905 = rx(i,t1111,t179,2,1)
        t8907 = rx(i,t1111,t179,0,1)
        t8908 = rx(i,t1111,t179,1,0)
        t8912 = rx(i,t1111,t179,2,0)
        t8914 = rx(i,t1111,t179,0,2)
        t8919 = t8898 * t8899 * t8901 - t8898 * t8903 * t8905 - t8899 * 
     #t8912 * t8914 - t8901 * t8907 * t8908 + t8903 * t8907 * t8912 + t8
     #905 * t8908 * t8914
        t8920 = 0.1E1 / t8919
        t8921 = t4 * t8920
        t8925 = t8898 * t8908 + t8899 * t8907 + t8903 * t8914
        t8927 = (t2911 - t7845) * t108
        t8929 = t6287 / 0.2E1 + t8927 / 0.2E1
        t8349 = t8921 * t8925
        t8931 = t8349 * t8929
        t8933 = (t5570 - t8931) * t124
        t8934 = t8933 / 0.2E1
        t8935 = t8908 ** 2
        t8936 = t8899 ** 2
        t8937 = t8903 ** 2
        t8939 = t8920 * (t8935 + t8936 + t8937)
        t8942 = t4 * (t5592 / 0.2E1 + t8939 / 0.2E1)
        t8943 = t8942 * t3004
        t8945 = (t5596 - t8943) * t124
        t8950 = u(i,t1111,t1375,n)
        t8952 = (t2911 - t8950) * t177
        t8954 = t2913 / 0.2E1 + t8952 / 0.2E1
        t8369 = t8921 * (t8899 * t8905 + t8901 * t8903 + t8908 * t8912)
        t8956 = t8369 * t8954
        t8958 = (t5611 - t8956) * t124
        t8959 = t8958 / 0.2E1
        t8960 = rx(i,t126,t1375,0,0)
        t8961 = rx(i,t126,t1375,1,1)
        t8963 = rx(i,t126,t1375,2,2)
        t8965 = rx(i,t126,t1375,1,2)
        t8967 = rx(i,t126,t1375,2,1)
        t8969 = rx(i,t126,t1375,0,1)
        t8970 = rx(i,t126,t1375,1,0)
        t8974 = rx(i,t126,t1375,2,0)
        t8976 = rx(i,t126,t1375,0,2)
        t8981 = t8960 * t8961 * t8963 - t8960 * t8965 * t8967 - t8961 * 
     #t8974 * t8976 - t8963 * t8969 * t8970 + t8965 * t8969 * t8974 + t8
     #967 * t8970 * t8976
        t8982 = 0.1E1 / t8981
        t8983 = t4 * t8982
        t8987 = t8960 * t8974 + t8963 * t8976 + t8967 * t8969
        t8989 = (t2963 - t8314) * t108
        t8991 = t6351 / 0.2E1 + t8989 / 0.2E1
        t8411 = t8983 * t8987
        t8993 = t8411 * t8991
        t8995 = (t4694 - t8993) * t177
        t8996 = t8995 / 0.2E1
        t9002 = (t2963 - t8950) * t124
        t9004 = t3337 / 0.2E1 + t9002 / 0.2E1
        t8427 = t8983 * (t8961 * t8967 + t8963 * t8965 + t8970 * t8974)
        t9006 = t8427 * t9004
        t9008 = (t4718 - t9006) * t177
        t9009 = t9008 / 0.2E1
        t9010 = t8974 ** 2
        t9011 = t8967 ** 2
        t9012 = t8963 ** 2
        t9014 = t8982 * (t9010 + t9011 + t9012)
        t9017 = t4 * (t4740 / 0.2E1 + t9014 / 0.2E1)
        t9018 = t9017 * t2965
        t9020 = (t4744 - t9018) * t177
        t9021 = (t6229 - t8885) * t108 + t6243 + t8892 + t6255 + t8897 +
     # t5573 + t8934 + t8945 + t5614 + t8959 + t4697 + t8996 + t4721 + t
     #9009 + t9020
        t9022 = t9021 * t4682
        t9024 = (t4748 - t9022) * t177
        t9026 = t8876 / 0.2E1 + t9024 / 0.2E1
        t9028 = t706 * t9026
        t9031 = (t8725 - t9028) * t124 / 0.2E1
        t9033 = (t5524 - t8190) * t108
        t9035 = t6397 / 0.2E1 + t9033 / 0.2E1
        t9037 = t743 * t9035
        t9039 = t212 * t8405
        t9042 = (t9037 - t9039) * t177 / 0.2E1
        t9044 = (t5618 - t8388) * t108
        t9046 = t6410 / 0.2E1 + t9044 / 0.2E1
        t9048 = t781 * t9046
        t9051 = (t9039 - t9048) * t177 / 0.2E1
        t9053 = (t8569 - t5524) * t124
        t9055 = (t5524 - t8874) * t124
        t9057 = t9053 / 0.2E1 + t9055 / 0.2E1
        t9059 = t796 * t9057
        t9061 = t691 * t4752
        t9064 = (t9059 - t9061) * t177 / 0.2E1
        t9066 = (t8717 - t5618) * t124
        t9068 = (t5618 - t9022) * t124
        t9070 = t9066 / 0.2E1 + t9068 / 0.2E1
        t9072 = t811 * t9070
        t9075 = (t9061 - t9072) * t177 / 0.2E1
        t9076 = t843 * t5526
        t9077 = t852 * t5620
        t9080 = (t3489 - t7457) * t108 + t4757 + t7994 + t5627 + t8397 +
     # t8410 + t8419 + (t8420 - t8421) * t124 + t8728 + t9031 + t9042 + 
     #t9051 + t9064 + t9075 + (t9076 - t9077) * t177
        t9081 = t2263 * t9080
        t9083 = t3386 * t9081 / 0.12E2
        t9084 = t64 * t69 * t860 / 0.6E1 - t1942 + t2271 + t64 * t68 * t
     #2375 / 0.2E1 - t2743 + t2745 - t2746 * t2748 / 0.24E2 - t3383 + t3
     #386 * t6447 / 0.12E2 + t7017 - t7018 * t2269 / 0.8E1 + t7028 + t70
     #29 * t7444 / 0.4E1 - t7448 - t7453 - t9083
        t9086 = t7451 * t9081 / 0.12E2
        t9089 = t64 * t7449 * t860 / 0.6E1
        t9091 = t7029 * t7015 / 0.4E1
        t9094 = t64 * t1944 * t2375 / 0.2E1
        t9098 = t2379 * (t7023 - t2383)
        t9100 = t864 * t9098 / 0.24E2
        t9105 = t112 - dx * t6502 / 0.24E2
        t9108 = t863 * dt
        t9110 = t1495 * t9108 * t9105
        t9112 = t2379 * (t2383 - t2738)
        t9114 = t2378 * t9112 / 0.24E2
        t9116 = t864 * t9112 / 0.24E2
        t9117 = t7004 / 0.2E1
        t9118 = t6671 / 0.2E1
        t9120 = t609 / 0.2E1 + t6528 / 0.2E1
        t9122 = t2329 * t9120
        t9124 = t541 / 0.2E1 + t6469 / 0.2E1
        t9126 = t547 * t9124
        t9128 = (t9122 - t9126) * t124
        t9129 = t9128 / 0.2E1
        t9131 = t650 / 0.2E1 + t6546 / 0.2E1
        t9133 = t2377 * t9131
        t9135 = (t9126 - t9133) * t124
        t9136 = t9135 / 0.2E1
        t9137 = t2549 * t552
        t9138 = t2558 * t555
        t9140 = (t9137 - t9138) * t124
        t9141 = ut(t507,t121,t174,n)
        t9143 = (t9141 - t550) * t177
        t9144 = ut(t507,t121,t179,n)
        t9146 = (t550 - t9144) * t177
        t9148 = t9143 / 0.2E1 + t9146 / 0.2E1
        t9150 = t2419 * t9148
        t9152 = t2427 * t574
        t9154 = (t9150 - t9152) * t124
        t9155 = t9154 / 0.2E1
        t9156 = ut(t507,t126,t174,n)
        t9158 = (t9156 - t553) * t177
        t9159 = ut(t507,t126,t179,n)
        t9161 = (t553 - t9159) * t177
        t9163 = t9158 / 0.2E1 + t9161 / 0.2E1
        t9165 = t2444 * t9163
        t9167 = (t9152 - t9165) * t124
        t9168 = t9167 / 0.2E1
        t9170 = t752 / 0.2E1 + t6456 / 0.2E1
        t9172 = t2484 * t9170
        t9174 = t564 * t9124
        t9176 = (t9172 - t9174) * t177
        t9177 = t9176 / 0.2E1
        t9179 = t791 / 0.2E1 + t6484 / 0.2E1
        t9181 = t2520 * t9179
        t9183 = (t9174 - t9181) * t177
        t9184 = t9183 / 0.2E1
        t9186 = (t9141 - t567) * t124
        t9188 = (t567 - t9156) * t124
        t9190 = t9186 / 0.2E1 + t9188 / 0.2E1
        t9192 = t2540 * t9190
        t9194 = t2427 * t557
        t9196 = (t9192 - t9194) * t177
        t9197 = t9196 / 0.2E1
        t9199 = (t9144 - t570) * t124
        t9201 = (t570 - t9159) * t124
        t9203 = t9199 / 0.2E1 + t9201 / 0.2E1
        t9205 = t2562 * t9203
        t9207 = (t9194 - t9205) * t177
        t9208 = t9207 / 0.2E1
        t9209 = t2722 * t569
        t9210 = t2731 * t572
        t9212 = (t9209 - t9210) * t177
        t9213 = t6515 + t562 + t9117 + t579 + t9118 + t9129 + t9136 + t9
     #140 + t9155 + t9168 + t9177 + t9184 + t9197 + t9208 + t9212
        t9214 = t2385 * t9213
        t9216 = (t2264 - t9214) * t108
        t9219 = t1946 * (t2266 / 0.2E1 + t9216 / 0.2E1)
        t9221 = t7018 * t9219 / 0.8E1
        t9223 = t1945 * t9219 / 0.8E1
        t9227 = t9108 * t2748 / 0.24E2
        t9229 = t865 * t3381 / 0.2E1
        t9230 = t9086 - t9089 - t9091 - t9094 - t2378 * t7026 / 0.4E1 - 
     #t9100 + t2378 * t9098 / 0.24E2 + t1495 * t2746 * t9105 - t9110 - t
     #9114 + t9116 - t9221 + t9223 + t2751 * t1940 / 0.2E1 + t9227 + t92
     #29
        t9232 = (t9084 + t9230) * t65
        t9235 = cc * t56
        t9237 = t9235 * t2262 * t2
        t9238 = t9237 / 0.2E1
        t9241 = cc * t28 * t866 * t106
        t9242 = t9241 / 0.2E1
        t9243 = t1942 - t2271 - t2745 - t9238 + t9242 - t7017 - t7028 + 
     #t7448 + t7453 - t9086 + t9089 + t9094
        t9248 = t1495 * (t1000 - dx * t1869 / 0.24E2)
        t9250 = (-t9237 + t9241) * t108
        t9251 = t9250 / 0.2E1
        t9254 = cc * t530 * t2384 * t539
        t9256 = (t9237 - t9254) * t108
        t9257 = t9256 / 0.2E1
        t9260 = cc * t96 * t1947 * t105
        t9262 = (-t9241 + t9260) * t108
        t9264 = (t9262 - t9250) * t108
        t9266 = (t9250 - t9256) * t108
        t9268 = (t9264 - t9266) * t108
        t9270 = sqrt(t2413)
        t9272 = cc * t2409 * t9270 * t6467
        t9274 = (-t9272 + t9254) * t108
        t9276 = (t9256 - t9274) * t108
        t9278 = (t9266 - t9276) * t108
        t9285 = dx * (t9251 + t9257 - t868 * (t9268 / 0.2E1 + t9278 / 0.
     #2E1) / 0.6E1) / 0.4E1
        t9291 = t868 * (t9266 - dx * (t9268 - t9278) / 0.12E2) / 0.24E2
        t9293 = sqrt(t1472)
        t9301 = (((cc * t1949 * t892 * t9293 - t9260) * t108 - t9262) * 
     #t108 - t9264) * t108
        t9307 = t868 * (t9264 - dx * (t9301 - t9268) / 0.12E2) / 0.24E2
        t9309 = dx * t1889 / 0.24E2
        t9317 = dx * (t9262 / 0.2E1 + t9251 - t868 * (t9301 / 0.2E1 + t9
     #268 / 0.2E1) / 0.6E1) / 0.4E1
        t9318 = -t863 * t9232 + t9100 + t9110 - t9116 - t9223 - t9227 - 
     #t9229 + t9248 - t9285 - t9291 + t9307 - t9309 - t9317
        t9322 = t28 * t138
        t9324 = t56 * t156
        t9325 = t9324 / 0.2E1
        t9329 = t530 * t549
        t9337 = t4 * (t9322 / 0.2E1 + t9325 - dx * ((t120 * t96 - t9322)
     # * t108 / 0.2E1 - (t9324 - t9329) * t108 / 0.2E1) / 0.8E1)
        t9342 = t1046 * (t7191 / 0.2E1 + t7196 / 0.2E1)
        t9344 = t159 / 0.4E1
        t9345 = t162 / 0.4E1
        t9348 = t1046 * (t6853 / 0.2E1 + t6858 / 0.2E1)
        t9349 = t9348 / 0.12E2
        t9355 = (t125 - t129) * t124
        t9366 = t141 / 0.2E1
        t9367 = t144 / 0.2E1
        t9368 = t9342 / 0.6E1
        t9371 = t159 / 0.2E1
        t9372 = t162 / 0.2E1
        t9373 = t9348 / 0.6E1
        t9374 = t552 / 0.2E1
        t9375 = t555 / 0.2E1
        t9379 = (t552 - t555) * t124
        t9381 = ((t6970 - t552) * t124 - t9379) * t124
        t9385 = (t9379 - (t555 - t6975) * t124) * t124
        t9388 = t1046 * (t9381 / 0.2E1 + t9385 / 0.2E1)
        t9389 = t9388 / 0.6E1
        t9396 = t141 / 0.4E1 + t144 / 0.4E1 - t9342 / 0.12E2 + t9344 + t
     #9345 - t9349 - dx * ((t125 / 0.2E1 + t129 / 0.2E1 - t1046 * (((t73
     #39 - t125) * t124 - t9355) * t124 / 0.2E1 + (t9355 - (t129 - t7344
     #) * t124) * t124 / 0.2E1) / 0.6E1 - t9366 - t9367 + t9368) * t108 
     #/ 0.2E1 - (t9371 + t9372 - t9373 - t9374 - t9375 + t9389) * t108 /
     # 0.2E1) / 0.8E1
        t9401 = t4 * (t9322 / 0.2E1 + t9324 / 0.2E1)
        t9402 = t68 * t71
        t9404 = t4269 / 0.4E1 + t4427 / 0.4E1 + t4592 / 0.4E1 + t4750 / 
     #0.4E1
        t9408 = t69 * t72
        t9410 = t4119 * t252
        t9412 = (t250 * t3536 - t9410) * t108
        t9418 = t6951 / 0.2E1 + t141 / 0.2E1
        t9420 = t251 * t9418
        t9422 = (t2089 * (t7339 / 0.2E1 + t125 / 0.2E1) - t9420) * t108
        t9425 = t6618 / 0.2E1 + t159 / 0.2E1
        t9427 = t604 * t9425
        t9429 = (t9420 - t9427) * t108
        t9430 = t9429 / 0.2E1
        t9434 = t3344 * t339
        t9436 = (t2004 * t2095 * t3583 - t9434) * t108
        t9439 = t3948 * t694
        t9441 = (t9434 - t9439) * t108
        t9442 = t9441 / 0.2E1
        t9446 = (t2088 - t332) * t108
        t9448 = (t332 - t687) * t108
        t9450 = t9446 / 0.2E1 + t9448 / 0.2E1
        t9454 = t3344 * t254
        t9459 = (t2091 - t335) * t108
        t9461 = (t335 - t690) * t108
        t9463 = t9459 / 0.2E1 + t9461 / 0.2E1
        t9470 = t7359 / 0.2E1 + t453 / 0.2E1
        t9474 = t333 * t9418
        t9479 = t7378 / 0.2E1 + t470 / 0.2E1
        t9489 = t9412 + t9422 / 0.2E1 + t9430 + t9436 / 0.2E1 + t9442 + 
     #t7275 / 0.2E1 + t263 + t7202 + t7049 / 0.2E1 + t350 + (t3970 * t94
     #50 - t9454) * t177 / 0.2E1 + (-t4003 * t9463 + t9454) * t177 / 0.2
     #E1 + (t4014 * t9470 - t9474) * t177 / 0.2E1 + (-t4027 * t9479 + t9
     #474) * t177 / 0.2E1 + (t334 * t4253 - t337 * t4262) * t177
        t9490 = t9489 * t242
        t9494 = t4277 * t295
        t9496 = (t293 * t3844 - t9494) * t108
        t9502 = t144 / 0.2E1 + t6956 / 0.2E1
        t9504 = t292 * t9502
        t9506 = (t2098 * (t129 / 0.2E1 + t7344 / 0.2E1) - t9504) * t108
        t9509 = t162 / 0.2E1 + t6624 / 0.2E1
        t9511 = t641 * t9509
        t9513 = (t9504 - t9511) * t108
        t9514 = t9513 / 0.2E1
        t9518 = t3674 * t362
        t9520 = (t2045 * t2118 * t3891 - t9518) * t108
        t9523 = t4056 * t717
        t9525 = (t9518 - t9523) * t108
        t9526 = t9525 / 0.2E1
        t9530 = (t2111 - t355) * t108
        t9532 = (t355 - t710) * t108
        t9534 = t9530 / 0.2E1 + t9532 / 0.2E1
        t9538 = t3674 * t297
        t9543 = (t2114 - t358) * t108
        t9545 = (t358 - t713) * t108
        t9547 = t9543 / 0.2E1 + t9545 / 0.2E1
        t9554 = t455 / 0.2E1 + t7364 / 0.2E1
        t9558 = t356 * t9502
        t9563 = t472 / 0.2E1 + t7383 / 0.2E1
        t9573 = t9496 + t9506 / 0.2E1 + t9514 + t9520 / 0.2E1 + t9526 + 
     #t302 + t7290 / 0.2E1 + t7207 + t367 + t7067 / 0.2E1 + (t4082 * t95
     #34 - t9538) * t177 / 0.2E1 + (-t4111 * t9547 + t9538) * t177 / 0.2
     #E1 + (t4127 * t9554 - t9558) * t177 / 0.2E1 + (-t4145 * t9563 + t9
     #558) * t177 / 0.2E1 + (t357 * t4411 - t360 * t4420) * t177
        t9574 = t9573 * t285
        t9577 = t4442 * t609
        t9579 = (t9410 - t9577) * t108
        t9581 = t6970 / 0.2E1 + t552 / 0.2E1
        t9583 = t2329 * t9581
        t9585 = (t9427 - t9583) * t108
        t9586 = t9585 / 0.2E1
        t9588 = t4188 * t9148
        t9590 = (t9439 - t9588) * t108
        t9591 = t9590 / 0.2E1
        t9592 = t6749 / 0.2E1
        t9593 = t6925 / 0.2E1
        t9595 = (t687 - t9141) * t108
        t9597 = t9448 / 0.2E1 + t9595 / 0.2E1
        t9599 = t4219 * t9597
        t9601 = t3948 * t611
        t9603 = (t9599 - t9601) * t177
        t9604 = t9603 / 0.2E1
        t9606 = (t690 - t9144) * t108
        t9608 = t9461 / 0.2E1 + t9606 / 0.2E1
        t9610 = t4261 * t9608
        t9612 = (t9601 - t9610) * t177
        t9613 = t9612 / 0.2E1
        t9615 = t6602 / 0.2E1 + t804 / 0.2E1
        t9617 = t4276 * t9615
        t9619 = t683 * t9425
        t9621 = (t9617 - t9619) * t177
        t9622 = t9621 / 0.2E1
        t9624 = t6636 / 0.2E1 + t821 / 0.2E1
        t9626 = t4289 * t9624
        t9628 = (t9619 - t9626) * t177
        t9629 = t9628 / 0.2E1
        t9630 = t4576 * t689
        t9631 = t4585 * t692
        t9633 = (t9630 - t9631) * t177
        t9634 = t9579 + t9430 + t9586 + t9442 + t9591 + t9592 + t620 + t
     #6864 + t9593 + t705 + t9604 + t9613 + t9622 + t9629 + t9633
        t9635 = t9634 * t601
        t9636 = t9635 - t857
        t9637 = t9636 * t124
        t9638 = t4600 * t650
        t9640 = (t9494 - t9638) * t108
        t9642 = t555 / 0.2E1 + t6975 / 0.2E1
        t9644 = t2377 * t9642
        t9646 = (t9511 - t9644) * t108
        t9647 = t9646 / 0.2E1
        t9649 = t4327 * t9163
        t9651 = (t9523 - t9649) * t108
        t9652 = t9651 / 0.2E1
        t9653 = t6767 / 0.2E1
        t9654 = t6941 / 0.2E1
        t9656 = (t710 - t9156) * t108
        t9658 = t9532 / 0.2E1 + t9656 / 0.2E1
        t9660 = t4363 * t9658
        t9662 = t4056 * t652
        t9664 = (t9660 - t9662) * t177
        t9665 = t9664 / 0.2E1
        t9667 = (t713 - t9159) * t108
        t9669 = t9545 / 0.2E1 + t9667 / 0.2E1
        t9671 = t4394 * t9669
        t9673 = (t9662 - t9671) * t177
        t9674 = t9673 / 0.2E1
        t9676 = t806 / 0.2E1 + t6608 / 0.2E1
        t9678 = t4410 * t9676
        t9680 = t706 * t9509
        t9682 = (t9678 - t9680) * t177
        t9683 = t9682 / 0.2E1
        t9685 = t823 / 0.2E1 + t6642 / 0.2E1
        t9687 = t4426 * t9685
        t9689 = (t9680 - t9687) * t177
        t9690 = t9689 / 0.2E1
        t9691 = t4734 * t712
        t9692 = t4743 * t715
        t9694 = (t9691 - t9692) * t177
        t9695 = t9640 + t9514 + t9647 + t9526 + t9652 + t657 + t9653 + t
     #6869 + t722 + t9654 + t9665 + t9674 + t9683 + t9690 + t9694
        t9696 = t9695 * t642
        t9697 = t857 - t9696
        t9698 = t9697 * t124
        t9700 = (t9490 - t506) * t124 / 0.4E1 + (t506 - t9574) * t124 / 
     #0.4E1 + t9637 / 0.4E1 + t9698 / 0.4E1
        t9706 = dx * (t150 / 0.2E1 - t561 / 0.2E1)
        t9710 = t9337 * t9108 * t9396
        t9711 = t1944 * t71
        t9714 = t9401 * t9711 * t9404 / 0.2E1
        t9715 = t7449 * t72
        t9718 = t9401 * t9715 * t9700 / 0.6E1
        t9720 = t9108 * t9706 / 0.24E2
        t9722 = (t9337 * t2746 * t9396 + t9401 * t9402 * t9404 / 0.2E1 +
     # t9401 * t9408 * t9700 / 0.6E1 - t2746 * t9706 / 0.24E2 - t9710 - 
     #t9714 - t9718 + t9720) * t65
        t9729 = t1046 * (t1701 / 0.2E1 + t1708 / 0.2E1)
        t9731 = t1218 / 0.4E1
        t9732 = t1220 / 0.4E1
        t9735 = t1046 * (t2794 / 0.2E1 + t2799 / 0.2E1)
        t9736 = t9735 / 0.12E2
        t9742 = (t1203 - t1205) * t124
        t9753 = t1179 / 0.2E1
        t9754 = t1198 / 0.2E1
        t9755 = t9729 / 0.6E1
        t9758 = t1218 / 0.2E1
        t9759 = t1220 / 0.2E1
        t9760 = t9735 / 0.6E1
        t9761 = t1797 / 0.2E1
        t9762 = t1799 / 0.2E1
        t9766 = (t1797 - t1799) * t124
        t9768 = ((t3168 - t1797) * t124 - t9766) * t124
        t9772 = (t9766 - (t1799 - t3174) * t124) * t124
        t9775 = t1046 * (t9768 / 0.2E1 + t9772 / 0.2E1)
        t9776 = t9775 / 0.6E1
        t9784 = t9337 * (t1179 / 0.4E1 + t1198 / 0.4E1 - t9729 / 0.12E2 
     #+ t9731 + t9732 - t9736 - dx * ((t1203 / 0.2E1 + t1205 / 0.2E1 - t
     #1046 * (((t1897 - t1203) * t124 - t9742) * t124 / 0.2E1 + (t9742 -
     # (t1205 - t1902) * t124) * t124 / 0.2E1) / 0.6E1 - t9753 - t9754 +
     # t9755) * t108 / 0.2E1 - (t9758 + t9759 - t9760 - t9761 - t9762 + 
     #t9776) * t108 / 0.2E1) / 0.8E1)
        t9788 = dx * (t1215 / 0.2E1 - t1805 / 0.2E1) / 0.24E2
        t9793 = t28 * t190
        t9795 = t56 * t207
        t9796 = t9795 / 0.2E1
        t9800 = t530 * t566
        t9808 = t4 * (t9793 / 0.2E1 + t9796 - dx * ((t173 * t96 - t9793)
     # * t108 / 0.2E1 - (t9795 - t9800) * t108 / 0.2E1) / 0.8E1)
        t9813 = t1363 * (t7081 / 0.2E1 + t7086 / 0.2E1)
        t9815 = t210 / 0.4E1
        t9816 = t213 / 0.4E1
        t9819 = t1363 * (t6785 / 0.2E1 + t6790 / 0.2E1)
        t9820 = t9819 / 0.12E2
        t9826 = (t178 - t182) * t177
        t9837 = t193 / 0.2E1
        t9838 = t196 / 0.2E1
        t9839 = t9813 / 0.6E1
        t9842 = t210 / 0.2E1
        t9843 = t213 / 0.2E1
        t9844 = t9819 / 0.6E1
        t9845 = t569 / 0.2E1
        t9846 = t572 / 0.2E1
        t9850 = (t569 - t572) * t177
        t9852 = ((t6897 - t569) * t177 - t9850) * t177
        t9856 = (t9850 - (t572 - t6902) * t177) * t177
        t9859 = t1363 * (t9852 / 0.2E1 + t9856 / 0.2E1)
        t9860 = t9859 / 0.6E1
        t9867 = t193 / 0.4E1 + t196 / 0.4E1 - t9813 / 0.12E2 + t9815 + t
     #9816 - t9820 - dx * ((t178 / 0.2E1 + t182 / 0.2E1 - t1363 * (((t74
     #23 - t178) * t177 - t9826) * t177 / 0.2E1 + (t9826 - (t182 - t7428
     #) * t177) * t177 / 0.2E1) / 0.6E1 - t9837 - t9838 + t9839) * t108 
     #/ 0.2E1 - (t9842 + t9843 - t9844 - t9845 - t9846 + t9860) * t108 /
     # 0.2E1) / 0.8E1
        t9872 = t4 * (t9793 / 0.2E1 + t9795 / 0.2E1)
        t9874 = t5331 / 0.4E1 + t5425 / 0.4E1 + t5526 / 0.4E1 + t5620 / 
     #0.4E1
        t9879 = t5245 * t399
        t9881 = (t397 * t4802 - t9879) * t108
        t9885 = t4500 * t457
        t9887 = (t2147 * t2209 * t4822 - t9885) * t108
        t9890 = t5027 * t808
        t9892 = (t9885 - t9890) * t108
        t9893 = t9892 / 0.2E1
        t9899 = t6878 / 0.2E1 + t193 / 0.2E1
        t9901 = t396 * t9899
        t9903 = (t2128 * (t7423 / 0.2E1 + t178 / 0.2E1) - t9901) * t108
        t9906 = t6697 / 0.2E1 + t210 / 0.2E1
        t9908 = t743 * t9906
        t9910 = (t9901 - t9908) * t108
        t9911 = t9910 / 0.2E1
        t9915 = t4500 * t401
        t9929 = t7148 / 0.2E1 + t334 / 0.2E1
        t9933 = t450 * t9899
        t9938 = t7167 / 0.2E1 + t357 / 0.2E1
        t9946 = t9881 + t9887 / 0.2E1 + t9893 + t9903 / 0.2E1 + t9911 + 
     #(t5039 * t9450 - t9915) * t124 / 0.2E1 + (-t5048 * t9534 + t9915) 
     #* t124 / 0.2E1 + (t453 * t5297 - t455 * t5306) * t124 + (t4014 * t
     #9929 - t9933) * t124 / 0.2E1 + (-t4127 * t9938 + t9933) * t124 / 0
     #.2E1 + t7311 / 0.2E1 + t408 + t7116 / 0.2E1 + t464 + t7092
        t9947 = t9946 * t389
        t9951 = t5339 * t440
        t9953 = (t438 * t5040 - t9951) * t108
        t9957 = t4823 * t474
        t9959 = (t2186 * t2226 * t5060 - t9957) * t108
        t9962 = t5081 * t825
        t9964 = (t9957 - t9962) * t108
        t9965 = t9964 / 0.2E1
        t9971 = t196 / 0.2E1 + t6883 / 0.2E1
        t9973 = t435 * t9971
        t9975 = (t2141 * (t182 / 0.2E1 + t7428 / 0.2E1) - t9973) * t108
        t9978 = t213 / 0.2E1 + t6702 / 0.2E1
        t9980 = t781 * t9978
        t9982 = (t9973 - t9980) * t108
        t9983 = t9982 / 0.2E1
        t9987 = t4823 * t442
        t10005 = t466 * t9971
        t9431 = (t337 / 0.2E1 + t7153 / 0.2E1) * t4203
        t9438 = (t360 / 0.2E1 + t7172 / 0.2E1) * t4361
        t10018 = t9953 + t9959 / 0.2E1 + t9965 + t9975 / 0.2E1 + t9983 +
     # (t5090 * t9463 - t9987) * t124 / 0.2E1 + (-t5100 * t9547 + t9987)
     # * t124 / 0.2E1 + (t470 * t5391 - t472 * t5400) * t124 + (t4233 * 
     #t9431 - t10005) * t124 / 0.2E1 + (-t4391 * t9438 + t10005) * t124 
     #/ 0.2E1 + t447 + t7326 / 0.2E1 + t479 + t7134 / 0.2E1 + t7097
        t10019 = t10018 * t430
        t10022 = t5440 * t752
        t10024 = (t9879 - t10022) * t108
        t10026 = t5140 * t9190
        t10028 = (t9890 - t10026) * t108
        t10029 = t10028 / 0.2E1
        t10031 = t6897 / 0.2E1 + t569 / 0.2E1
        t10033 = t2484 * t10031
        t10035 = (t9908 - t10033) * t108
        t10036 = t10035 / 0.2E1
        t10038 = t5148 * t9597
        t10040 = t5027 * t754
        t10042 = (t10038 - t10040) * t124
        t10043 = t10042 / 0.2E1
        t10045 = t5155 * t9658
        t10047 = (t10040 - t10045) * t124
        t10048 = t10047 / 0.2E1
        t10049 = t5492 * t804
        t10050 = t5501 * t806
        t10052 = (t10049 - t10050) * t124
        t10054 = t6682 / 0.2E1 + t689 / 0.2E1
        t10056 = t4276 * t10054
        t10058 = t796 * t9906
        t10060 = (t10056 - t10058) * t124
        t10061 = t10060 / 0.2E1
        t10063 = t6714 / 0.2E1 + t712 / 0.2E1
        t10065 = t4410 * t10063
        t10067 = (t10058 - t10065) * t124
        t10068 = t10067 / 0.2E1
        t10069 = t6572 / 0.2E1
        t10070 = t6819 / 0.2E1
        t10071 = t10024 + t9893 + t10029 + t9911 + t10036 + t10043 + t10
     #048 + t10052 + t10061 + t10068 + t10069 + t761 + t10070 + t815 + t
     #6796
        t10072 = t10071 * t744
        t10073 = t10072 - t857
        t10074 = t10073 * t177
        t10075 = t5534 * t791
        t10077 = (t9951 - t10075) * t108
        t10079 = t5187 * t9203
        t10081 = (t9962 - t10079) * t108
        t10082 = t10081 / 0.2E1
        t10084 = t572 / 0.2E1 + t6902 / 0.2E1
        t10086 = t2520 * t10084
        t10088 = (t9980 - t10086) * t108
        t10089 = t10088 / 0.2E1
        t10091 = t5197 * t9608
        t10093 = t5081 * t793
        t10095 = (t10091 - t10093) * t124
        t10096 = t10095 / 0.2E1
        t10098 = t5202 * t9669
        t10100 = (t10093 - t10098) * t124
        t10101 = t10100 / 0.2E1
        t10102 = t5586 * t821
        t10103 = t5595 * t823
        t10105 = (t10102 - t10103) * t124
        t10107 = t692 / 0.2E1 + t6688 / 0.2E1
        t10109 = t4289 * t10107
        t10111 = t811 * t9978
        t10113 = (t10109 - t10111) * t124
        t10114 = t10113 / 0.2E1
        t10116 = t715 / 0.2E1 + t6720 / 0.2E1
        t10118 = t4426 * t10116
        t10120 = (t10111 - t10118) * t124
        t10121 = t10120 / 0.2E1
        t10122 = t6591 / 0.2E1
        t10123 = t6835 / 0.2E1
        t10124 = t10077 + t9965 + t10082 + t9983 + t10089 + t10096 + t10
     #101 + t10105 + t10114 + t10121 + t798 + t10122 + t830 + t10123 + t
     #6801
        t10125 = t10124 * t783
        t10126 = t857 - t10125
        t10127 = t10126 * t177
        t10129 = (t9947 - t506) * t177 / 0.4E1 + (t506 - t10019) * t177 
     #/ 0.4E1 + t10074 / 0.4E1 + t10127 / 0.4E1
        t10135 = dx * (t202 / 0.2E1 - t578 / 0.2E1)
        t10139 = t9808 * t9108 * t9867
        t10142 = t9872 * t9711 * t9874 / 0.2E1
        t10145 = t9872 * t9715 * t10129 / 0.6E1
        t10147 = t9108 * t10135 / 0.24E2
        t10149 = (t9808 * t2746 * t9867 + t9872 * t9402 * t9874 / 0.2E1 
     #+ t9872 * t9408 * t10129 / 0.6E1 - t2746 * t10135 / 0.24E2 - t1013
     #9 - t10142 - t10145 + t10147) * t65
        t10156 = t1363 * (t1373 / 0.2E1 + t1382 / 0.2E1)
        t10158 = t939 / 0.4E1
        t10159 = t942 / 0.4E1
        t10162 = t1363 * (t3023 / 0.2E1 + t3028 / 0.2E1)
        t10163 = t10162 / 0.12E2
        t10169 = (t912 - t915) * t177
        t10180 = t925 / 0.2E1
        t10181 = t928 / 0.2E1
        t10182 = t10156 / 0.6E1
        t10185 = t939 / 0.2E1
        t10186 = t942 / 0.2E1
        t10187 = t10162 / 0.6E1
        t10188 = t956 / 0.2E1
        t10189 = t959 / 0.2E1
        t10193 = (t956 - t959) * t177
        t10195 = ((t3189 - t956) * t177 - t10193) * t177
        t10199 = (t10193 - (t959 - t3194) * t177) * t177
        t10202 = t1363 * (t10195 / 0.2E1 + t10199 / 0.2E1)
        t10203 = t10202 / 0.6E1
        t10211 = t9808 * (t925 / 0.4E1 + t928 / 0.4E1 - t10156 / 0.12E2 
     #+ t10158 + t10159 - t10163 - dx * ((t912 / 0.2E1 + t915 / 0.2E1 - 
     #t1363 * (((t1598 - t912) * t177 - t10169) * t177 / 0.2E1 + (t10169
     # - (t915 - t1604) * t177) * t177 / 0.2E1) / 0.6E1 - t10180 - t1018
     #1 + t10182) * t108 / 0.2E1 - (t10185 + t10186 - t10187 - t10188 - 
     #t10189 + t10203) * t108 / 0.2E1) / 0.8E1)
        t10215 = dx * (t934 / 0.2E1 - t965 / 0.2E1) / 0.24E2
        t10221 = t9213 * t529
        t10222 = t857 - t10221
        t10224 = t72 * t10222 * t108
        t10229 = t71 * t7455 * t108
        t10232 = i - 3
        t10233 = u(t10232,t121,k,n)
        t10235 = (t2429 - t10233) * t108
        t10243 = u(t10232,j,k,n)
        t10245 = (t2418 - t10243) * t108
        t9589 = (t2769 - (t1007 / 0.2E1 - t10245 / 0.2E1) * t108) * t108
        t10252 = t547 * t9589
        t10255 = u(t10232,t126,k,n)
        t10257 = (t2432 - t10255) * t108
        t10271 = rx(t10232,j,k,0,0)
        t10272 = rx(t10232,j,k,1,1)
        t10274 = rx(t10232,j,k,2,2)
        t10276 = rx(t10232,j,k,1,2)
        t10278 = rx(t10232,j,k,2,1)
        t10280 = rx(t10232,j,k,0,1)
        t10281 = rx(t10232,j,k,1,0)
        t10285 = rx(t10232,j,k,2,0)
        t10287 = rx(t10232,j,k,0,2)
        t10293 = 0.1E1 / (t10271 * t10272 * t10274 - t10271 * t10276 * t
     #10278 - t10272 * t10285 * t10287 - t10274 * t10280 * t10281 + t102
     #76 * t10280 * t10285 + t10278 * t10281 * t10287)
        t10294 = t4 * t10293
        t10299 = u(t10232,j,t174,n)
        t10301 = (t10299 - t10243) * t177
        t10302 = u(t10232,j,t179,n)
        t10304 = (t10243 - t10302) * t177
        t9643 = t10294 * (t10271 * t10285 + t10274 * t10287 + t10278 * t
     #10280)
        t10310 = (t2455 - t9643 * (t10301 / 0.2E1 + t10304 / 0.2E1)) * t
     #108
        t10335 = t2275 + t2458 + t2499 + t2441 + t2640 + t2677 + t2694 +
     # t2709 + t2276 + t2536 + t2584 + t2601 - t868 * ((t2329 * (t3275 -
     # (t989 / 0.2E1 - t10235 / 0.2E1) * t108) * t108 - t10252) * t124 /
     # 0.2E1 + (t10252 - t2377 * (t3287 - (t1032 / 0.2E1 - t10257 / 0.2E
     #1) * t108) * t108) * t124 / 0.2E1) / 0.6E1 - t868 * (t2755 / 0.2E1
     # + (t2753 - (t2457 - t10310) * t108) * t108 / 0.2E1) / 0.6E1 - t13
     #63 * (t3203 / 0.2E1 + (t3201 - t2281 * ((t8051 / 0.2E1 - t2451 / 0
     #.2E1) * t177 - (t2448 / 0.2E1 - t8249 / 0.2E1) * t177) * t177) * t
     #108 / 0.2E1) / 0.6E1
        t10341 = (t10233 - t10243) * t124
        t10343 = (t10243 - t10255) * t124
        t9746 = t10294 * (t10271 * t10281 + t10272 * t10280 + t10276 * t
     #10287)
        t10349 = (t2438 - t9746 * (t10341 / 0.2E1 + t10343 / 0.2E1)) * t
     #108
        t10361 = (t2498 - t2535) * t124
        t10373 = t2546 / 0.2E1
        t10383 = t4 * (t2541 / 0.2E1 + t10373 - dy * ((t7567 - t2541) * 
     #t124 / 0.2E1 - (t2546 - t2555) * t124 / 0.2E1) / 0.8E1)
        t10395 = t4 * (t10373 + t2555 / 0.2E1 - dy * ((t2541 - t2546) * 
     #t124 / 0.2E1 - (t2555 - t7831) * t124 / 0.2E1) / 0.8E1)
        t10400 = (t2446 - t10299) * t108
        t10410 = t564 * t9589
        t10414 = (t2449 - t10302) * t108
        t10431 = (t2583 - t2600) * t124
        t10454 = t2427 * t2998
        t10489 = (t2693 - t2708) * t177
        t10512 = t2427 * t2984
        t10534 = (t2639 - t2676) * t177
        t10575 = t10271 ** 2
        t10576 = t10280 ** 2
        t10577 = t10287 ** 2
        t10578 = t10575 + t10576 + t10577
        t10579 = t10293 * t10578
        t10587 = t4 * (t3298 + t2414 / 0.2E1 - dx * (t1489 / 0.2E1 - (t2
     #414 - t10579) * t108 / 0.2E1) / 0.8E1)
        t10592 = t2719 / 0.2E1
        t10602 = t4 * (t2714 / 0.2E1 + t10592 - dz * ((t8182 - t2714) * 
     #t177 / 0.2E1 - (t2719 - t2728) * t177 / 0.2E1) / 0.8E1)
        t10614 = t4 * (t10592 + t2728 / 0.2E1 - dz * ((t2714 - t2719) * 
     #t177 / 0.2E1 - (t2728 - t8380) * t177 / 0.2E1) / 0.8E1)
        t10627 = t4 * (t2414 / 0.2E1 + t10579 / 0.2E1)
        t10630 = (-t10245 * t10627 + t2421) * t108
        t9952 = ((t8106 / 0.2E1 - t2571 / 0.2E1) * t177 - (t2568 / 0.2E1
     # - t8304 / 0.2E1) * t177) * t177
        t9958 = ((t8118 / 0.2E1 - t2594 / 0.2E1) * t177 - (t2591 / 0.2E1
     # - t8316 / 0.2E1) * t177) * t177
        t10004 = ((t7672 / 0.2E1 - t2685 / 0.2E1) * t124 - (t2683 / 0.2E
     #1 - t7936 / 0.2E1) * t124) * t124
        t10009 = ((t7687 / 0.2E1 - t2702 / 0.2E1) * t124 - (t2700 / 0.2E
     #1 - t7951 / 0.2E1) * t124) * t124
        t10638 = -t868 * (t2931 / 0.2E1 + (t2929 - (t2440 - t10349) * t1
     #08) * t108 / 0.2E1) / 0.6E1 - t1046 * (((t7561 - t2498) * t124 - t
     #10361) * t124 / 0.2E1 + (t10361 - (t2535 - t7825) * t124) * t124 /
     # 0.2E1) / 0.6E1 + (t10383 * t1797 - t10395 * t1799) * t124 - t868 
     #* ((t2484 * (t2762 - (t1325 / 0.2E1 - t10400 / 0.2E1) * t108) * t1
     #08 - t10410) * t177 / 0.2E1 + (t10410 - t2520 * (t2778 - (t1348 / 
     #0.2E1 - t10414 / 0.2E1) * t108) * t108) * t177 / 0.2E1) / 0.6E1 - 
     #t1046 * (((t7589 - t2583) * t124 - t10431) * t124 / 0.2E1 + (t1043
     #1 - (t2600 - t7853) * t124) * t124 / 0.2E1) / 0.6E1 - t1363 * ((t2
     #419 * t9952 - t10454) * t124 / 0.2E1 + (-t2444 * t9958 + t10454) *
     # t124 / 0.2E1) / 0.6E1 - t1046 * ((t2549 * t9768 - t2558 * t9772) 
     #* t124 + ((t7573 - t2561) * t124 - (t2561 - t7837) * t124) * t124)
     # / 0.24E2 - t1363 * (((t8176 - t2693) * t177 - t10489) * t177 / 0.
     #2E1 + (t10489 - (t2708 - t8374) * t177) * t177 / 0.2E1) / 0.6E1 - 
     #t1046 * ((t10004 * t2540 - t10512) * t177 / 0.2E1 + (-t10009 * t25
     #62 + t10512) * t177 / 0.2E1) / 0.6E1 - t1363 * (((t8161 - t2639) *
     # t177 - t10534) * t177 / 0.2E1 + (t10534 - (t2676 - t8359) * t177)
     # * t177 / 0.2E1) / 0.6E1 - t1363 * ((t10195 * t2722 - t10199 * t27
     #31) * t177 + ((t8188 - t2734) * t177 - (t2734 - t8386) * t177) * t
     #177) / 0.24E2 - t1046 * (t3183 / 0.2E1 + (t3181 - t2261 * ((t7501 
     #/ 0.2E1 - t2434 / 0.2E1) * t124 - (t2431 / 0.2E1 - t7765 / 0.2E1) 
     #* t124) * t124) * t108 / 0.2E1) / 0.6E1 + (-t10587 * t2420 + t3307
     #) * t108 + (t10602 * t956 - t10614 * t959) * t177 - t868 * ((t3156
     # - t2417 * (t3153 - (t2420 - t10245) * t108) * t108) * t108 + (t31
     #60 - (t2423 - t10630) * t108) * t108) / 0.24E2
        t10640 = t2385 * (t10335 + t10638)
        t10642 = t865 * t10640 / 0.2E1
        t10645 = t541 - dx * t6507 / 0.24E2
        t10649 = t3306 * t9108 * t10645
        t10650 = cc * t9270
        t10654 = t2488 / 0.2E1 + t10235 / 0.2E1
        t10656 = t7182 * t10654
        t10658 = t2420 / 0.2E1 + t10245 / 0.2E1
        t10660 = t2261 * t10658
        t10663 = (t10656 - t10660) * t124 / 0.2E1
        t10665 = t2529 / 0.2E1 + t10257 / 0.2E1
        t10667 = t7332 * t10665
        t10670 = (t10660 - t10667) * t124 / 0.2E1
        t10671 = t7470 ** 2
        t10672 = t7461 ** 2
        t10673 = t7465 ** 2
        t10675 = t7482 * (t10671 + t10672 + t10673)
        t10676 = t2397 ** 2
        t10677 = t2388 ** 2
        t10678 = t2392 ** 2
        t10680 = t2409 * (t10676 + t10677 + t10678)
        t10683 = t4 * (t10675 / 0.2E1 + t10680 / 0.2E1)
        t10684 = t10683 * t2431
        t10685 = t7734 ** 2
        t10686 = t7725 ** 2
        t10687 = t7729 ** 2
        t10689 = t7746 * (t10685 + t10686 + t10687)
        t10692 = t4 * (t10680 / 0.2E1 + t10689 / 0.2E1)
        t10693 = t10692 * t2434
        t10699 = t7461 * t7467 + t7463 * t7465 + t7470 * t7474
        t10192 = t7494 * t10699
        t10701 = t10192 * t7520
        t10198 = t2424 * (t2388 * t2394 + t2390 * t2392 + t2397 * t2401)
        t10707 = t10198 * t2453
        t10710 = (t10701 - t10707) * t124 / 0.2E1
        t10714 = t7725 * t7731 + t7727 * t7729 + t7734 * t7738
        t10207 = t7758 * t10714
        t10716 = t10207 * t7784
        t10719 = (t10707 - t10716) * t124 / 0.2E1
        t10721 = t2631 / 0.2E1 + t10400 / 0.2E1
        t10723 = t7532 * t10721
        t10725 = t2281 * t10658
        t10728 = (t10723 - t10725) * t177 / 0.2E1
        t10730 = t2670 / 0.2E1 + t10414 / 0.2E1
        t10732 = t7714 * t10730
        t10735 = (t10725 - t10732) * t177 / 0.2E1
        t10739 = t7996 * t8002 + t7998 * t8000 + t8005 * t8009
        t10223 = t8029 * t10739
        t10741 = t10223 * t8039
        t10743 = t10198 * t2436
        t10746 = (t10741 - t10743) * t177 / 0.2E1
        t10750 = t8194 * t8200 + t8196 * t8198 + t8203 * t8207
        t10231 = t8227 * t10750
        t10752 = t10231 * t8237
        t10755 = (t10743 - t10752) * t177 / 0.2E1
        t10756 = t8009 ** 2
        t10757 = t8002 ** 2
        t10758 = t7998 ** 2
        t10760 = t8017 * (t10756 + t10757 + t10758)
        t10761 = t2401 ** 2
        t10762 = t2394 ** 2
        t10763 = t2390 ** 2
        t10765 = t2409 * (t10761 + t10762 + t10763)
        t10768 = t4 * (t10760 / 0.2E1 + t10765 / 0.2E1)
        t10769 = t10768 * t2448
        t10770 = t8207 ** 2
        t10771 = t8200 ** 2
        t10772 = t8196 ** 2
        t10774 = t8215 * (t10770 + t10771 + t10772)
        t10777 = t4 * (t10765 / 0.2E1 + t10774 / 0.2E1)
        t10778 = t10777 * t2451
        t10781 = t10630 + t2441 + t10349 / 0.2E1 + t2458 + t10310 / 0.2E
     #1 + t10663 + t10670 + (t10684 - t10693) * t124 + t10710 + t10719 +
     # t10728 + t10735 + t10746 + t10755 + (t10769 - t10778) * t177
        t10784 = (-t10650 * t10781 + t2736) * t108
        t10786 = t2379 * (t2738 - t10784)
        t10788 = t864 * t10786 / 0.24E2
        t10789 = ut(t10232,t121,k,n)
        t10791 = (t6526 - t10789) * t108
        t10799 = ut(t10232,j,k,n)
        t10801 = (t6467 - t10799) * t108
        t10265 = (t6472 - (t541 / 0.2E1 - t10801 / 0.2E1) * t108) * t108
        t10808 = t547 * t10265
        t10811 = ut(t10232,t126,k,n)
        t10813 = (t6544 - t10811) * t108
        t10827 = ut(t10232,j,t174,n)
        t10830 = ut(t10232,j,t179,n)
        t10838 = (t6669 - t9643 * ((t10827 - t10799) * t177 / 0.2E1 + (t
     #10799 - t10830) * t177 / 0.2E1)) * t108
        t10847 = ut(t507,t121,t1364,n)
        t10849 = (t10847 - t9141) * t177
        t10853 = ut(t507,t121,t1375,n)
        t10855 = (t9144 - t10853) * t177
        t10865 = t2427 * t6461
        t10868 = ut(t507,t126,t1364,n)
        t10870 = (t10868 - t9156) * t177
        t10874 = ut(t507,t126,t1375,n)
        t10876 = (t9159 - t10874) * t177
        t10337 = ((t10849 / 0.2E1 - t9146 / 0.2E1) * t177 - (t9143 / 0.2
     #E1 - t10855 / 0.2E1) * t177) * t177
        t10344 = ((t10870 / 0.2E1 - t9161 / 0.2E1) * t177 - (t9158 / 0.2
     #E1 - t10876 / 0.2E1) * t177) * t177
        t10890 = t9208 + t579 + t9117 + t9118 + t9129 + t9136 + t9155 + 
     #t562 + t9168 + t9177 + t9184 + t9197 - t868 * ((t2329 * (t6531 - (
     #t609 / 0.2E1 - t10791 / 0.2E1) * t108) * t108 - t10808) * t124 / 0
     #.2E1 + (t10808 - t2377 * (t6549 - (t650 / 0.2E1 - t10813 / 0.2E1) 
     #* t108) * t108) * t124 / 0.2E1) / 0.6E1 - t868 * (t6675 / 0.2E1 + 
     #(t6673 - (t6671 - t10838) * t108) * t108 / 0.2E1) / 0.6E1 - t1363 
     #* ((t10337 * t2419 - t10865) * t124 / 0.2E1 + (-t10344 * t2444 + t
     #10865) * t124 / 0.2E1) / 0.6E1
        t10897 = (t6970 * t7570 - t9137) * t124
        t10902 = (-t6975 * t7834 + t9138) * t124
        t10910 = ut(t2386,j,t1364,n)
        t10912 = (t6564 - t10910) * t108
        t10918 = (t7623 * (t6566 / 0.2E1 + t10912 / 0.2E1) - t9172) * t1
     #77
        t10922 = (t9176 - t9183) * t177
        t10925 = ut(t2386,j,t1375,n)
        t10927 = (t6583 - t10925) * t108
        t10933 = (t9181 - t7809 * (t6585 / 0.2E1 + t10927 / 0.2E1)) * t1
     #77
        t10947 = (t6454 - t10827) * t108
        t10957 = t564 * t10265
        t10961 = (t6482 - t10830) * t108
        t10975 = ut(t507,t1047,t174,n)
        t10978 = ut(t507,t1047,t179,n)
        t10982 = (t10975 - t6741) * t177 / 0.2E1 + (t6741 - t10978) * t1
     #77 / 0.2E1
        t10986 = (t10982 * t7549 * t7577 - t9150) * t124
        t10990 = (t9154 - t9167) * t124
        t10993 = ut(t507,t1111,t174,n)
        t10996 = ut(t507,t1111,t179,n)
        t11000 = (t10993 - t6759) * t177 / 0.2E1 + (t6759 - t10996) * t1
     #77 / 0.2E1
        t11004 = (-t11000 * t7813 * t7841 + t9165) * t124
        t11018 = (t10847 - t6564) * t124 / 0.2E1 + (t6564 - t10868) * t1
     #24 / 0.2E1
        t11022 = (t11018 * t8149 * t8166 - t9192) * t177
        t11026 = (t9196 - t9207) * t177
        t11034 = (t10853 - t6583) * t124 / 0.2E1 + (t6583 - t10874) * t1
     #24 / 0.2E1
        t11038 = (-t11034 * t8347 * t8364 + t9205) * t177
        t11052 = (t10975 - t9141) * t124
        t11057 = (t9156 - t10993) * t124
        t11067 = t2427 * t6519
        t11071 = (t10978 - t9144) * t124
        t11076 = (t9159 - t10996) * t124
        t11096 = (t6897 * t8185 - t9209) * t177
        t11101 = (-t6902 * t8383 + t9210) * t177
        t11110 = (t10910 - t6454) * t177
        t11115 = (t6482 - t10925) * t177
        t11132 = ut(t2386,t1047,k,n)
        t11134 = (t6741 - t11132) * t108
        t11140 = (t7214 * (t6743 / 0.2E1 + t11134 / 0.2E1) - t9122) * t1
     #24
        t11144 = (t9128 - t9135) * t124
        t11147 = ut(t2386,t1111,k,n)
        t11149 = (t6759 - t11147) * t108
        t11155 = (t9133 - t7362 * (t6761 / 0.2E1 + t11149 / 0.2E1)) * t1
     #24
        t11173 = (-t10627 * t10801 + t6513) * t108
        t11182 = (t11132 - t6526) * t124
        t11187 = (t6544 - t11147) * t124
        t11210 = (t7002 - t9746 * ((t10789 - t10799) * t124 / 0.2E1 + (t
     #10799 - t10811) * t124 / 0.2E1)) * t108
        t10542 = ((t11052 / 0.2E1 - t9188 / 0.2E1) * t124 - (t9186 / 0.2
     #E1 - t11057 / 0.2E1) * t124) * t124
        t10546 = ((t11071 / 0.2E1 - t9201 / 0.2E1) * t124 - (t9199 / 0.2
     #E1 - t11076 / 0.2E1) * t124) * t124
        t11219 = -t1046 * ((t2549 * t9381 - t2558 * t9385) * t124 + ((t1
     #0897 - t9140) * t124 - (t9140 - t10902) * t124) * t124) / 0.24E2 -
     # t1363 * (((t10918 - t9176) * t177 - t10922) * t177 / 0.2E1 + (t10
     #922 - (t9183 - t10933) * t177) * t177 / 0.2E1) / 0.6E1 + (t10383 *
     # t552 - t10395 * t555) * t124 - t868 * ((t2484 * (t6459 - (t752 / 
     #0.2E1 - t10947 / 0.2E1) * t108) * t108 - t10957) * t177 / 0.2E1 + 
     #(t10957 - t2520 * (t6487 - (t791 / 0.2E1 - t10961 / 0.2E1) * t108)
     # * t108) * t177 / 0.2E1) / 0.6E1 - t1046 * (((t10986 - t9154) * t1
     #24 - t10990) * t124 / 0.2E1 + (t10990 - (t9167 - t11004) * t124) *
     # t124 / 0.2E1) / 0.6E1 - t1363 * (((t11022 - t9196) * t177 - t1102
     #6) * t177 / 0.2E1 + (t11026 - (t9207 - t11038) * t177) * t177 / 0.
     #2E1) / 0.6E1 + (t10602 * t569 - t10614 * t572) * t177 - t1046 * ((
     #t10542 * t2540 - t11067) * t177 / 0.2E1 + (-t10546 * t2562 + t1106
     #7) * t177 / 0.2E1) / 0.6E1 - t1363 * ((t2722 * t9852 - t2731 * t98
     #56) * t177 + ((t11096 - t9212) * t177 - (t9212 - t11101) * t177) *
     # t177) / 0.24E2 - t1363 * (t6911 / 0.2E1 + (t6909 - t2281 * ((t111
     #10 / 0.2E1 - t6665 / 0.2E1) * t177 - (t6663 / 0.2E1 - t11115 / 0.2
     #E1) * t177) * t177) * t108 / 0.2E1) / 0.6E1 + (-t10587 * t6469 + t
     #6735) * t108 - t1046 * (((t11140 - t9128) * t124 - t11144) * t124 
     #/ 0.2E1 + (t11144 - (t9135 - t11155) * t124) * t124 / 0.2E1) / 0.6
     #E1 - t868 * ((t6509 - t2417 * (t6506 - (t6469 - t10801) * t108) * 
     #t108) * t108 + (t6517 - (t6515 - t11173) * t108) * t108) / 0.24E2 
     #- t1046 * (t6984 / 0.2E1 + (t6982 - t2261 * ((t11182 / 0.2E1 - t69
     #98 / 0.2E1) * t124 - (t6996 / 0.2E1 - t11187 / 0.2E1) * t124) * t1
     #24) * t108 / 0.2E1) / 0.6E1 - t868 * (t7008 / 0.2E1 + (t7006 - (t7
     #004 - t11210) * t108) * t108 / 0.2E1) / 0.6E1
        t11221 = t2385 * (t10890 + t11219)
        t11225 = t6450 * t11221 / 0.4E1
        t11233 = t6469 / 0.2E1 + t10801 / 0.2E1
        t11235 = t2261 * t11233
        t11250 = ut(t2386,t121,t174,n)
        t11253 = ut(t2386,t121,t179,n)
        t11257 = (t11250 - t6526) * t177 / 0.2E1 + (t6526 - t11253) * t1
     #77 / 0.2E1
        t11261 = t10198 * t6667
        t11265 = ut(t2386,t126,t174,n)
        t11268 = ut(t2386,t126,t179,n)
        t11272 = (t11265 - t6544) * t177 / 0.2E1 + (t6544 - t11268) * t1
     #77 / 0.2E1
        t11283 = t2281 * t11233
        t11299 = (t11250 - t6454) * t124 / 0.2E1 + (t6454 - t11265) * t1
     #24 / 0.2E1
        t11303 = t10198 * t7000
        t11312 = (t11253 - t6482) * t124 / 0.2E1 + (t6482 - t11268) * t1
     #24 / 0.2E1
        t11322 = t11173 + t9117 + t11210 / 0.2E1 + t9118 + t10838 / 0.2E
     #1 + (t7182 * (t6528 / 0.2E1 + t10791 / 0.2E1) - t11235) * t124 / 0
     #.2E1 + (t11235 - t7332 * (t6546 / 0.2E1 + t10813 / 0.2E1)) * t124 
     #/ 0.2E1 + (t10683 * t6996 - t10692 * t6998) * t124 + (t10699 * t11
     #257 * t7494 - t11261) * t124 / 0.2E1 + (-t10714 * t11272 * t7758 +
     # t11261) * t124 / 0.2E1 + (t7532 * (t6456 / 0.2E1 + t10947 / 0.2E1
     #) - t11283) * t177 / 0.2E1 + (t11283 - t7714 * (t6484 / 0.2E1 + t1
     #0961 / 0.2E1)) * t177 / 0.2E1 + (t10739 * t11299 * t8029 - t11303)
     # * t177 / 0.2E1 + (-t10750 * t11312 * t8227 + t11303) * t177 / 0.2
     #E1 + (t10768 * t6663 - t10777 * t6665) * t177
        t11328 = t1946 * (t9216 / 0.2E1 + (-t10650 * t11322 + t9214) * t
     #108 / 0.2E1)
        t11330 = t1945 * t11328 / 0.8E1
        t11331 = dx * t6516
        t11334 = -t2743 + t2745 + t538 * t69 * t10224 / 0.6E1 + t3383 - 
     #t7017 + t538 * t68 * t10229 / 0.2E1 + t10642 + t3306 * t2746 * t10
     #645 - t10649 + t10788 - t7029 * t11221 / 0.4E1 + t9083 + t11225 + 
     #t11330 - t9086 - t2746 * t11331 / 0.24E2
        t11336 = t9108 * t11331 / 0.24E2
        t11339 = t538 * t1944 * t10229 / 0.2E1
        t11342 = t10781 * t2408
        t11344 = (t7454 - t11342) * t108
        t11348 = rx(t10232,t121,k,0,0)
        t11349 = rx(t10232,t121,k,1,1)
        t11351 = rx(t10232,t121,k,2,2)
        t11353 = rx(t10232,t121,k,1,2)
        t11355 = rx(t10232,t121,k,2,1)
        t11357 = rx(t10232,t121,k,0,1)
        t11358 = rx(t10232,t121,k,1,0)
        t11362 = rx(t10232,t121,k,2,0)
        t11364 = rx(t10232,t121,k,0,2)
        t11370 = 0.1E1 / (t11348 * t11349 * t11351 - t11348 * t11353 * t
     #11355 - t11349 * t11362 * t11364 - t11351 * t11357 * t11358 + t113
     #53 * t11357 * t11362 + t11355 * t11358 * t11364)
        t11371 = t11348 ** 2
        t11372 = t11357 ** 2
        t11373 = t11364 ** 2
        t11382 = t4 * t11370
        t11387 = u(t10232,t1047,k,n)
        t11401 = u(t10232,t121,t174,n)
        t11404 = u(t10232,t121,t179,n)
        t11414 = rx(t2386,t1047,k,0,0)
        t11415 = rx(t2386,t1047,k,1,1)
        t11417 = rx(t2386,t1047,k,2,2)
        t11419 = rx(t2386,t1047,k,1,2)
        t11421 = rx(t2386,t1047,k,2,1)
        t11423 = rx(t2386,t1047,k,0,1)
        t11424 = rx(t2386,t1047,k,1,0)
        t11428 = rx(t2386,t1047,k,2,0)
        t11430 = rx(t2386,t1047,k,0,2)
        t11436 = 0.1E1 / (t11414 * t11415 * t11417 - t11414 * t11419 * t
     #11421 - t11415 * t11428 * t11430 - t11417 * t11423 * t11424 + t114
     #19 * t11423 * t11428 + t11421 * t11424 * t11430)
        t11437 = t4 * t11436
        t11451 = t11424 ** 2
        t11452 = t11415 ** 2
        t11453 = t11419 ** 2
        t11466 = u(t2386,t1047,t174,n)
        t11469 = u(t2386,t1047,t179,n)
        t11473 = (t11466 - t7499) * t177 / 0.2E1 + (t7499 - t11469) * t1
     #77 / 0.2E1
        t11479 = rx(t2386,t121,t174,0,0)
        t11480 = rx(t2386,t121,t174,1,1)
        t11482 = rx(t2386,t121,t174,2,2)
        t11484 = rx(t2386,t121,t174,1,2)
        t11486 = rx(t2386,t121,t174,2,1)
        t11488 = rx(t2386,t121,t174,0,1)
        t11489 = rx(t2386,t121,t174,1,0)
        t11493 = rx(t2386,t121,t174,2,0)
        t11495 = rx(t2386,t121,t174,0,2)
        t11501 = 0.1E1 / (t11479 * t11480 * t11482 - t11479 * t11484 * t
     #11486 - t11480 * t11493 * t11495 - t11482 * t11488 * t11489 + t114
     #84 * t11488 * t11493 + t11486 * t11489 * t11495)
        t11502 = t4 * t11501
        t11510 = t7620 / 0.2E1 + (t7513 - t11401) * t108 / 0.2E1
        t11514 = t7192 * t10654
        t11518 = rx(t2386,t121,t179,0,0)
        t11519 = rx(t2386,t121,t179,1,1)
        t11521 = rx(t2386,t121,t179,2,2)
        t11523 = rx(t2386,t121,t179,1,2)
        t11525 = rx(t2386,t121,t179,2,1)
        t11527 = rx(t2386,t121,t179,0,1)
        t11528 = rx(t2386,t121,t179,1,0)
        t11532 = rx(t2386,t121,t179,2,0)
        t11534 = rx(t2386,t121,t179,0,2)
        t11540 = 0.1E1 / (t11518 * t11519 * t11521 - t11518 * t11523 * t
     #11525 - t11519 * t11532 * t11534 - t11521 * t11527 * t11528 + t115
     #23 * t11527 * t11532 + t11525 * t11528 * t11534)
        t11541 = t4 * t11540
        t11549 = t7659 / 0.2E1 + (t7516 - t11404) * t108 / 0.2E1
        t11562 = (t11466 - t7513) * t124 / 0.2E1 + t8035 / 0.2E1
        t11566 = t10192 * t7503
        t11577 = (t11469 - t7516) * t124 / 0.2E1 + t8233 / 0.2E1
        t11583 = t11493 ** 2
        t11584 = t11486 ** 2
        t11585 = t11482 ** 2
        t11588 = t7474 ** 2
        t11589 = t7467 ** 2
        t11590 = t7463 ** 2
        t11592 = t7482 * (t11588 + t11589 + t11590)
        t11597 = t11532 ** 2
        t11598 = t11525 ** 2
        t11599 = t11521 ** 2
        t10972 = t11437 * (t11414 * t11424 + t11415 * t11423 + t11419 * 
     #t11430)
        t11007 = t11502 * (t11479 * t11493 + t11482 * t11495 + t11486 * 
     #t11488)
        t11012 = t11541 * (t11518 * t11532 + t11521 * t11534 + t11525 * 
     #t11527)
        t11017 = t11502 * (t11480 * t11486 + t11482 * t11484 + t11489 * 
     #t11493)
        t11024 = t11541 * (t11519 * t11525 + t11521 * t11523 + t11528 * 
     #t11532)
        t11608 = (t7491 - t4 * (t7487 / 0.2E1 + t11370 * (t11371 + t1137
     #2 + t11373) / 0.2E1) * t10235) * t108 + t7508 + (t7505 - t11382 * 
     #(t11348 * t11358 + t11349 * t11357 + t11353 * t11364) * ((t11387 -
     # t10233) * t124 / 0.2E1 + t10341 / 0.2E1)) * t108 / 0.2E1 + t7525 
     #+ (t7522 - t11382 * (t11348 * t11362 + t11351 * t11364 + t11355 * 
     #t11357) * ((t11401 - t10233) * t177 / 0.2E1 + (t10233 - t11404) * 
     #t177 / 0.2E1)) * t108 / 0.2E1 + (t10972 * (t7555 / 0.2E1 + (t7499 
     #- t11387) * t108 / 0.2E1) - t10656) * t124 / 0.2E1 + t10663 + (t4 
     #* (t11436 * (t11451 + t11452 + t11453) / 0.2E1 + t10675 / 0.2E1) *
     # t7501 - t10684) * t124 + (t11437 * (t11415 * t11421 + t11417 * t1
     #1419 + t11424 * t11428) * t11473 - t10701) * t124 / 0.2E1 + t10710
     # + (t11007 * t11510 - t11514) * t177 / 0.2E1 + (-t11012 * t11549 +
     # t11514) * t177 / 0.2E1 + (t11017 * t11562 - t11566) * t177 / 0.2E
     #1 + (-t11024 * t11577 + t11566) * t177 / 0.2E1 + (t4 * (t11501 * (
     #t11583 + t11584 + t11585) / 0.2E1 + t11592 / 0.2E1) * t7515 - t4 *
     # (t11592 / 0.2E1 + t11540 * (t11597 + t11598 + t11599) / 0.2E1) * 
     #t7518) * t177
        t11609 = t11608 * t7481
        t11612 = rx(t10232,t126,k,0,0)
        t11613 = rx(t10232,t126,k,1,1)
        t11615 = rx(t10232,t126,k,2,2)
        t11617 = rx(t10232,t126,k,1,2)
        t11619 = rx(t10232,t126,k,2,1)
        t11621 = rx(t10232,t126,k,0,1)
        t11622 = rx(t10232,t126,k,1,0)
        t11626 = rx(t10232,t126,k,2,0)
        t11628 = rx(t10232,t126,k,0,2)
        t11634 = 0.1E1 / (t11612 * t11613 * t11615 - t11612 * t11617 * t
     #11619 - t11613 * t11626 * t11628 - t11615 * t11621 * t11622 + t116
     #17 * t11621 * t11626 + t11619 * t11622 * t11628)
        t11635 = t11612 ** 2
        t11636 = t11621 ** 2
        t11637 = t11628 ** 2
        t11646 = t4 * t11634
        t11651 = u(t10232,t1111,k,n)
        t11665 = u(t10232,t126,t174,n)
        t11668 = u(t10232,t126,t179,n)
        t11678 = rx(t2386,t1111,k,0,0)
        t11679 = rx(t2386,t1111,k,1,1)
        t11681 = rx(t2386,t1111,k,2,2)
        t11683 = rx(t2386,t1111,k,1,2)
        t11685 = rx(t2386,t1111,k,2,1)
        t11687 = rx(t2386,t1111,k,0,1)
        t11688 = rx(t2386,t1111,k,1,0)
        t11692 = rx(t2386,t1111,k,2,0)
        t11694 = rx(t2386,t1111,k,0,2)
        t11700 = 0.1E1 / (t11678 * t11679 * t11681 - t11678 * t11683 * t
     #11685 - t11679 * t11692 * t11694 - t11681 * t11687 * t11688 + t116
     #83 * t11687 * t11692 + t11685 * t11688 * t11694)
        t11701 = t4 * t11700
        t11715 = t11688 ** 2
        t11716 = t11679 ** 2
        t11717 = t11683 ** 2
        t11730 = u(t2386,t1111,t174,n)
        t11733 = u(t2386,t1111,t179,n)
        t11737 = (t11730 - t7763) * t177 / 0.2E1 + (t7763 - t11733) * t1
     #77 / 0.2E1
        t11743 = rx(t2386,t126,t174,0,0)
        t11744 = rx(t2386,t126,t174,1,1)
        t11746 = rx(t2386,t126,t174,2,2)
        t11748 = rx(t2386,t126,t174,1,2)
        t11750 = rx(t2386,t126,t174,2,1)
        t11752 = rx(t2386,t126,t174,0,1)
        t11753 = rx(t2386,t126,t174,1,0)
        t11757 = rx(t2386,t126,t174,2,0)
        t11759 = rx(t2386,t126,t174,0,2)
        t11765 = 0.1E1 / (t11743 * t11744 * t11746 - t11743 * t11748 * t
     #11750 - t11744 * t11757 * t11759 - t11746 * t11752 * t11753 + t117
     #48 * t11752 * t11757 + t11750 * t11753 * t11759)
        t11766 = t4 * t11765
        t11774 = t7884 / 0.2E1 + (t7777 - t11665) * t108 / 0.2E1
        t11778 = t7341 * t10665
        t11782 = rx(t2386,t126,t179,0,0)
        t11783 = rx(t2386,t126,t179,1,1)
        t11785 = rx(t2386,t126,t179,2,2)
        t11787 = rx(t2386,t126,t179,1,2)
        t11789 = rx(t2386,t126,t179,2,1)
        t11791 = rx(t2386,t126,t179,0,1)
        t11792 = rx(t2386,t126,t179,1,0)
        t11796 = rx(t2386,t126,t179,2,0)
        t11798 = rx(t2386,t126,t179,0,2)
        t11804 = 0.1E1 / (t11782 * t11783 * t11785 - t11782 * t11787 * t
     #11789 - t11783 * t11796 * t11798 - t11785 * t11791 * t11792 + t117
     #87 * t11791 * t11796 + t11789 * t11792 * t11798)
        t11805 = t4 * t11804
        t11813 = t7923 / 0.2E1 + (t7780 - t11668) * t108 / 0.2E1
        t11826 = t8037 / 0.2E1 + (t7777 - t11730) * t124 / 0.2E1
        t11830 = t10207 * t7767
        t11841 = t8235 / 0.2E1 + (t7780 - t11733) * t124 / 0.2E1
        t11847 = t11757 ** 2
        t11848 = t11750 ** 2
        t11849 = t11746 ** 2
        t11852 = t7738 ** 2
        t11853 = t7731 ** 2
        t11854 = t7727 ** 2
        t11856 = t7746 * (t11852 + t11853 + t11854)
        t11861 = t11796 ** 2
        t11862 = t11789 ** 2
        t11863 = t11785 ** 2
        t11188 = t11701 * (t11678 * t11688 + t11679 * t11687 + t11683 * 
     #t11694)
        t11215 = t11766 * (t11743 * t11757 + t11746 * t11759 + t11750 * 
     #t11752)
        t11222 = t11805 * (t11782 * t11796 + t11785 * t11798 + t11789 * 
     #t11791)
        t11228 = t11766 * (t11744 * t11750 + t11746 * t11748 + t11753 * 
     #t11757)
        t11234 = t11805 * (t11783 * t11789 + t11785 * t11787 + t11792 * 
     #t11796)
        t11872 = (t7755 - t4 * (t7751 / 0.2E1 + t11634 * (t11635 + t1163
     #6 + t11637) / 0.2E1) * t10257) * t108 + t7772 + (t7769 - t11646 * 
     #(t11612 * t11622 + t11613 * t11621 + t11617 * t11628) * (t10343 / 
     #0.2E1 + (t10255 - t11651) * t124 / 0.2E1)) * t108 / 0.2E1 + t7789 
     #+ (t7786 - t11646 * (t11612 * t11626 + t11615 * t11628 + t11619 * 
     #t11621) * ((t11665 - t10255) * t177 / 0.2E1 + (t10255 - t11668) * 
     #t177 / 0.2E1)) * t108 / 0.2E1 + t10670 + (t10667 - t11188 * (t7819
     # / 0.2E1 + (t7763 - t11651) * t108 / 0.2E1)) * t124 / 0.2E1 + (t10
     #693 - t4 * (t10689 / 0.2E1 + t11700 * (t11715 + t11716 + t11717) /
     # 0.2E1) * t7765) * t124 + t10719 + (t10716 - t11701 * (t11679 * t1
     #1685 + t11681 * t11683 + t11688 * t11692) * t11737) * t124 / 0.2E1
     # + (t11215 * t11774 - t11778) * t177 / 0.2E1 + (-t11222 * t11813 +
     # t11778) * t177 / 0.2E1 + (t11228 * t11826 - t11830) * t177 / 0.2E
     #1 + (-t11234 * t11841 + t11830) * t177 / 0.2E1 + (t4 * (t11765 * (
     #t11847 + t11848 + t11849) / 0.2E1 + t11856 / 0.2E1) * t7779 - t4 *
     # (t11856 / 0.2E1 + t11804 * (t11861 + t11862 + t11863) / 0.2E1) * 
     #t7782) * t177
        t11873 = t11872 * t7745
        t11883 = rx(t10232,j,t174,0,0)
        t11884 = rx(t10232,j,t174,1,1)
        t11886 = rx(t10232,j,t174,2,2)
        t11888 = rx(t10232,j,t174,1,2)
        t11890 = rx(t10232,j,t174,2,1)
        t11892 = rx(t10232,j,t174,0,1)
        t11893 = rx(t10232,j,t174,1,0)
        t11897 = rx(t10232,j,t174,2,0)
        t11899 = rx(t10232,j,t174,0,2)
        t11905 = 0.1E1 / (t11883 * t11884 * t11886 - t11883 * t11888 * t
     #11890 - t11884 * t11897 * t11899 - t11886 * t11892 * t11893 + t118
     #88 * t11892 * t11897 + t11890 * t11893 * t11899)
        t11906 = t11883 ** 2
        t11907 = t11892 ** 2
        t11908 = t11899 ** 2
        t11917 = t4 * t11905
        t11937 = u(t10232,j,t1364,n)
        t11950 = t11479 * t11489 + t11480 * t11488 + t11484 * t11495
        t11954 = t7514 * t10721
        t11961 = t11743 * t11753 + t11744 * t11752 + t11748 * t11759
        t11967 = t11489 ** 2
        t11968 = t11480 ** 2
        t11969 = t11484 ** 2
        t11972 = t8005 ** 2
        t11973 = t7996 ** 2
        t11974 = t8000 ** 2
        t11976 = t8017 * (t11972 + t11973 + t11974)
        t11981 = t11753 ** 2
        t11982 = t11744 ** 2
        t11983 = t11748 ** 2
        t11992 = u(t2386,t121,t1364,n)
        t11996 = (t11992 - t7513) * t177 / 0.2E1 + t7515 / 0.2E1
        t12000 = t10223 * t8053
        t12004 = u(t2386,t126,t1364,n)
        t12008 = (t12004 - t7777) * t177 / 0.2E1 + t7779 / 0.2E1
        t12014 = rx(t2386,j,t1364,0,0)
        t12015 = rx(t2386,j,t1364,1,1)
        t12017 = rx(t2386,j,t1364,2,2)
        t12019 = rx(t2386,j,t1364,1,2)
        t12021 = rx(t2386,j,t1364,2,1)
        t12023 = rx(t2386,j,t1364,0,1)
        t12024 = rx(t2386,j,t1364,1,0)
        t12028 = rx(t2386,j,t1364,2,0)
        t12030 = rx(t2386,j,t1364,0,2)
        t12036 = 0.1E1 / (t12014 * t12015 * t12017 - t12014 * t12019 * t
     #12021 - t12015 * t12028 * t12030 - t12017 * t12023 * t12024 + t120
     #19 * t12023 * t12028 + t12021 * t12024 * t12030)
        t12037 = t4 * t12036
        t12060 = (t11992 - t8049) * t124 / 0.2E1 + (t8049 - t12004) * t1
     #24 / 0.2E1
        t12066 = t12028 ** 2
        t12067 = t12021 ** 2
        t12068 = t12017 ** 2
        t11406 = t12037 * (t12014 * t12028 + t12017 * t12030 + t12021 * 
     #t12023)
        t12077 = (t8026 - t4 * (t8022 / 0.2E1 + t11905 * (t11906 + t1190
     #7 + t11908) / 0.2E1) * t10400) * t108 + t8044 + (t8041 - t11917 * 
     #(t11883 * t11893 + t11884 * t11892 + t11888 * t11899) * ((t11401 -
     # t10299) * t124 / 0.2E1 + (t10299 - t11665) * t124 / 0.2E1)) * t10
     #8 / 0.2E1 + t8058 + (t8055 - t11917 * (t11883 * t11897 + t11886 * 
     #t11899 + t11890 * t11892) * ((t11937 - t10299) * t177 / 0.2E1 + t1
     #0301 / 0.2E1)) * t108 / 0.2E1 + (t11502 * t11510 * t11950 - t11954
     #) * t124 / 0.2E1 + (-t11766 * t11774 * t11961 + t11954) * t124 / 0
     #.2E1 + (t4 * (t11501 * (t11967 + t11968 + t11969) / 0.2E1 + t11976
     # / 0.2E1) * t8035 - t4 * (t11976 / 0.2E1 + t11765 * (t11981 + t119
     #82 + t11983) / 0.2E1) * t8037) * t124 + (t11017 * t11996 - t12000)
     # * t124 / 0.2E1 + (-t11228 * t12008 + t12000) * t124 / 0.2E1 + (t1
     #1406 * (t8155 / 0.2E1 + (t8049 - t11937) * t108 / 0.2E1) - t10723)
     # * t177 / 0.2E1 + t10728 + (t12037 * (t12015 * t12021 + t12017 * t
     #12019 + t12024 * t12028) * t12060 - t10741) * t177 / 0.2E1 + t1074
     #6 + (t4 * (t12036 * (t12066 + t12067 + t12068) / 0.2E1 + t10760 / 
     #0.2E1) * t8051 - t10769) * t177
        t12078 = t12077 * t8016
        t12081 = rx(t10232,j,t179,0,0)
        t12082 = rx(t10232,j,t179,1,1)
        t12084 = rx(t10232,j,t179,2,2)
        t12086 = rx(t10232,j,t179,1,2)
        t12088 = rx(t10232,j,t179,2,1)
        t12090 = rx(t10232,j,t179,0,1)
        t12091 = rx(t10232,j,t179,1,0)
        t12095 = rx(t10232,j,t179,2,0)
        t12097 = rx(t10232,j,t179,0,2)
        t12103 = 0.1E1 / (t12081 * t12082 * t12084 - t12081 * t12086 * t
     #12088 - t12082 * t12095 * t12097 - t12084 * t12090 * t12091 + t120
     #86 * t12090 * t12095 + t12088 * t12091 * t12097)
        t12104 = t12081 ** 2
        t12105 = t12090 ** 2
        t12106 = t12097 ** 2
        t12115 = t4 * t12103
        t12135 = u(t10232,j,t1375,n)
        t12148 = t11518 * t11528 + t11519 * t11527 + t11523 * t11534
        t12152 = t7692 * t10730
        t12159 = t11782 * t11792 + t11783 * t11791 + t11787 * t11798
        t12165 = t11528 ** 2
        t12166 = t11519 ** 2
        t12167 = t11523 ** 2
        t12170 = t8203 ** 2
        t12171 = t8194 ** 2
        t12172 = t8198 ** 2
        t12174 = t8215 * (t12170 + t12171 + t12172)
        t12179 = t11792 ** 2
        t12180 = t11783 ** 2
        t12181 = t11787 ** 2
        t12190 = u(t2386,t121,t1375,n)
        t12194 = t7518 / 0.2E1 + (t7516 - t12190) * t177 / 0.2E1
        t12198 = t10231 * t8251
        t12202 = u(t2386,t126,t1375,n)
        t12206 = t7782 / 0.2E1 + (t7780 - t12202) * t177 / 0.2E1
        t12212 = rx(t2386,j,t1375,0,0)
        t12213 = rx(t2386,j,t1375,1,1)
        t12215 = rx(t2386,j,t1375,2,2)
        t12217 = rx(t2386,j,t1375,1,2)
        t12219 = rx(t2386,j,t1375,2,1)
        t12221 = rx(t2386,j,t1375,0,1)
        t12222 = rx(t2386,j,t1375,1,0)
        t12226 = rx(t2386,j,t1375,2,0)
        t12228 = rx(t2386,j,t1375,0,2)
        t12234 = 0.1E1 / (t12212 * t12213 * t12215 - t12212 * t12217 * t
     #12219 - t12213 * t12226 * t12228 - t12215 * t12221 * t12222 + t122
     #17 * t12221 * t12226 + t12219 * t12222 * t12228)
        t12235 = t4 * t12234
        t12258 = (t12190 - t8247) * t124 / 0.2E1 + (t8247 - t12202) * t1
     #24 / 0.2E1
        t12264 = t12226 ** 2
        t12265 = t12219 ** 2
        t12266 = t12215 ** 2
        t11607 = t12235 * (t12212 * t12226 + t12215 * t12228 + t12219 * 
     #t12221)
        t12275 = (t8224 - t4 * (t8220 / 0.2E1 + t12103 * (t12104 + t1210
     #5 + t12106) / 0.2E1) * t10414) * t108 + t8242 + (t8239 - t12115 * 
     #(t12081 * t12091 + t12082 * t12090 + t12086 * t12097) * ((t11404 -
     # t10302) * t124 / 0.2E1 + (t10302 - t11668) * t124 / 0.2E1)) * t10
     #8 / 0.2E1 + t8256 + (t8253 - t12115 * (t12081 * t12095 + t12084 * 
     #t12097 + t12088 * t12090) * (t10304 / 0.2E1 + (t10302 - t12135) * 
     #t177 / 0.2E1)) * t108 / 0.2E1 + (t11541 * t11549 * t12148 - t12152
     #) * t124 / 0.2E1 + (-t11805 * t11813 * t12159 + t12152) * t124 / 0
     #.2E1 + (t4 * (t11540 * (t12165 + t12166 + t12167) / 0.2E1 + t12174
     # / 0.2E1) * t8233 - t4 * (t12174 / 0.2E1 + t11804 * (t12179 + t121
     #80 + t12181) / 0.2E1) * t8235) * t124 + (t11024 * t12194 - t12198)
     # * t124 / 0.2E1 + (-t11234 * t12206 + t12198) * t124 / 0.2E1 + t10
     #735 + (t10732 - t11607 * (t8353 / 0.2E1 + (t8247 - t12135) * t108 
     #/ 0.2E1)) * t177 / 0.2E1 + t10755 + (t10752 - t12235 * (t12213 * t
     #12219 + t12215 * t12217 + t12222 * t12226) * t12258) * t177 / 0.2E
     #1 + (t10778 - t4 * (t10774 / 0.2E1 + t12234 * (t12264 + t12265 + t
     #12266) / 0.2E1) * t8249) * t177
        t12276 = t12275 * t8214
        t12293 = t7456 / 0.2E1 + t11344 / 0.2E1
        t12295 = t547 * t12293
        t12312 = t11479 ** 2
        t12313 = t11488 ** 2
        t12314 = t11495 ** 2
        t12333 = rx(t507,t1047,t174,0,0)
        t12334 = rx(t507,t1047,t174,1,1)
        t12336 = rx(t507,t1047,t174,2,2)
        t12338 = rx(t507,t1047,t174,1,2)
        t12340 = rx(t507,t1047,t174,2,1)
        t12342 = rx(t507,t1047,t174,0,1)
        t12343 = rx(t507,t1047,t174,1,0)
        t12347 = rx(t507,t1047,t174,2,0)
        t12349 = rx(t507,t1047,t174,0,2)
        t12355 = 0.1E1 / (t12333 * t12334 * t12336 - t12333 * t12338 * t
     #12340 - t12334 * t12347 * t12349 - t12336 * t12342 * t12343 + t123
     #38 * t12342 * t12347 + t12340 * t12343 * t12349)
        t12356 = t4 * t12355
        t12364 = t8474 / 0.2E1 + (t7578 - t11466) * t108 / 0.2E1
        t12370 = t12343 ** 2
        t12371 = t12334 ** 2
        t12372 = t12338 ** 2
        t12385 = u(t507,t1047,t1364,n)
        t12389 = (t12385 - t7578) * t177 / 0.2E1 + t7580 / 0.2E1
        t12395 = rx(t507,t121,t1364,0,0)
        t12396 = rx(t507,t121,t1364,1,1)
        t12398 = rx(t507,t121,t1364,2,2)
        t12400 = rx(t507,t121,t1364,1,2)
        t12402 = rx(t507,t121,t1364,2,1)
        t12404 = rx(t507,t121,t1364,0,1)
        t12405 = rx(t507,t121,t1364,1,0)
        t12409 = rx(t507,t121,t1364,2,0)
        t12411 = rx(t507,t121,t1364,0,2)
        t12417 = 0.1E1 / (t12395 * t12396 * t12398 - t12395 * t12400 * t
     #12402 - t12396 * t12409 * t12411 - t12398 * t12404 * t12405 + t124
     #00 * t12404 * t12409 + t12402 * t12405 * t12411)
        t12418 = t4 * t12417
        t12426 = t8536 / 0.2E1 + (t8104 - t11992) * t108 / 0.2E1
        t12439 = (t12385 - t8104) * t124 / 0.2E1 + t8168 / 0.2E1
        t12445 = t12409 ** 2
        t12446 = t12402 ** 2
        t12447 = t12398 ** 2
        t11738 = t12356 * (t12333 * t12343 + t12334 * t12342 + t12338 * 
     #t12349)
        t11760 = t12356 * (t12334 * t12340 + t12336 * t12338 + t12343 * 
     #t12347)
        t11767 = t12418 * (t12395 * t12409 + t12398 * t12411 + t12402 * 
     #t12404)
        t11773 = t12418 * (t12396 * t12402 + t12398 * t12400 + t12405 * 
     #t12409)
        t12456 = (t8432 - t4 * (t8428 / 0.2E1 + t11501 * (t12312 + t1231
     #3 + t12314) / 0.2E1) * t7620) * t108 + t8439 + (-t11502 * t11562 *
     # t11950 + t8436) * t108 / 0.2E1 + t8444 + (-t11007 * t11996 + t844
     #1) * t108 / 0.2E1 + (t11738 * t12364 - t8064) * t124 / 0.2E1 + t80
     #69 + (t4 * (t12355 * (t12370 + t12371 + t12372) / 0.2E1 + t8083 / 
     #0.2E1) * t7672 - t8092) * t124 + (t11760 * t12389 - t8110) * t124 
     #/ 0.2E1 + t8115 + (t11767 * t12426 - t7624) * t177 / 0.2E1 + t7629
     # + (t11773 * t12439 - t7676) * t177 / 0.2E1 + t7681 + (t4 * (t1241
     #7 * (t12445 + t12446 + t12447) / 0.2E1 + t7699 / 0.2E1) * t8106 - 
     #t7708) * t177
        t12457 = t12456 * t7612
        t12460 = t11518 ** 2
        t12461 = t11527 ** 2
        t12462 = t11534 ** 2
        t12481 = rx(t507,t1047,t179,0,0)
        t12482 = rx(t507,t1047,t179,1,1)
        t12484 = rx(t507,t1047,t179,2,2)
        t12486 = rx(t507,t1047,t179,1,2)
        t12488 = rx(t507,t1047,t179,2,1)
        t12490 = rx(t507,t1047,t179,0,1)
        t12491 = rx(t507,t1047,t179,1,0)
        t12495 = rx(t507,t1047,t179,2,0)
        t12497 = rx(t507,t1047,t179,0,2)
        t12503 = 0.1E1 / (t12481 * t12482 * t12484 - t12481 * t12486 * t
     #12488 - t12482 * t12495 * t12497 - t12484 * t12490 * t12491 + t124
     #86 * t12490 * t12495 + t12488 * t12491 * t12497)
        t12504 = t4 * t12503
        t12512 = t8622 / 0.2E1 + (t7581 - t11469) * t108 / 0.2E1
        t12518 = t12491 ** 2
        t12519 = t12482 ** 2
        t12520 = t12486 ** 2
        t12533 = u(t507,t1047,t1375,n)
        t12537 = t7583 / 0.2E1 + (t7581 - t12533) * t177 / 0.2E1
        t12543 = rx(t507,t121,t1375,0,0)
        t12544 = rx(t507,t121,t1375,1,1)
        t12546 = rx(t507,t121,t1375,2,2)
        t12548 = rx(t507,t121,t1375,1,2)
        t12550 = rx(t507,t121,t1375,2,1)
        t12552 = rx(t507,t121,t1375,0,1)
        t12553 = rx(t507,t121,t1375,1,0)
        t12557 = rx(t507,t121,t1375,2,0)
        t12559 = rx(t507,t121,t1375,0,2)
        t12565 = 0.1E1 / (t12543 * t12544 * t12546 - t12543 * t12548 * t
     #12550 - t12544 * t12557 * t12559 - t12546 * t12552 * t12553 + t125
     #48 * t12552 * t12557 + t12550 * t12553 * t12559)
        t12566 = t4 * t12565
        t12574 = t8684 / 0.2E1 + (t8302 - t12190) * t108 / 0.2E1
        t12587 = (t12533 - t8302) * t124 / 0.2E1 + t8366 / 0.2E1
        t12593 = t12557 ** 2
        t12594 = t12550 ** 2
        t12595 = t12546 ** 2
        t11889 = t12504 * (t12481 * t12491 + t12482 * t12490 + t12486 * 
     #t12497)
        t11911 = t12504 * (t12482 * t12488 + t12484 * t12486 + t12491 * 
     #t12495)
        t11916 = t12566 * (t12543 * t12557 + t12546 * t12559 + t12550 * 
     #t12552)
        t11922 = t12566 * (t12544 * t12550 + t12546 * t12548 + t12553 * 
     #t12557)
        t12604 = (t8580 - t4 * (t8576 / 0.2E1 + t11540 * (t12460 + t1246
     #1 + t12462) / 0.2E1) * t7659) * t108 + t8587 + (-t11541 * t11577 *
     # t12148 + t8584) * t108 / 0.2E1 + t8592 + (-t11012 * t12194 + t858
     #9) * t108 / 0.2E1 + (t11889 * t12512 - t8262) * t124 / 0.2E1 + t82
     #67 + (t4 * (t12503 * (t12518 + t12519 + t12520) / 0.2E1 + t8281 / 
     #0.2E1) * t7687 - t8290) * t124 + (t11911 * t12537 - t8308) * t124 
     #/ 0.2E1 + t8313 + t7666 + (-t11916 * t12574 + t7663) * t177 / 0.2E
     #1 + t7694 + (-t11922 * t12587 + t7691) * t177 / 0.2E1 + (t7717 - t
     #4 * (t7713 / 0.2E1 + t12565 * (t12593 + t12594 + t12595) / 0.2E1) 
     #* t8304) * t177
        t12605 = t12604 * t7651
        t12613 = t2427 * t8392
        t12617 = t11743 ** 2
        t12618 = t11752 ** 2
        t12619 = t11759 ** 2
        t12638 = rx(t507,t1111,t174,0,0)
        t12639 = rx(t507,t1111,t174,1,1)
        t12641 = rx(t507,t1111,t174,2,2)
        t12643 = rx(t507,t1111,t174,1,2)
        t12645 = rx(t507,t1111,t174,2,1)
        t12647 = rx(t507,t1111,t174,0,1)
        t12648 = rx(t507,t1111,t174,1,0)
        t12652 = rx(t507,t1111,t174,2,0)
        t12654 = rx(t507,t1111,t174,0,2)
        t12660 = 0.1E1 / (t12638 * t12639 * t12641 - t12638 * t12643 * t
     #12645 - t12639 * t12652 * t12654 - t12641 * t12647 * t12648 + t126
     #43 * t12647 * t12652 + t12645 * t12648 * t12654)
        t12661 = t4 * t12660
        t12669 = t8779 / 0.2E1 + (t7842 - t11730) * t108 / 0.2E1
        t12675 = t12648 ** 2
        t12676 = t12639 ** 2
        t12677 = t12643 ** 2
        t12690 = u(t507,t1111,t1364,n)
        t12694 = (t12690 - t7842) * t177 / 0.2E1 + t7844 / 0.2E1
        t12700 = rx(t507,t126,t1364,0,0)
        t12701 = rx(t507,t126,t1364,1,1)
        t12703 = rx(t507,t126,t1364,2,2)
        t12705 = rx(t507,t126,t1364,1,2)
        t12707 = rx(t507,t126,t1364,2,1)
        t12709 = rx(t507,t126,t1364,0,1)
        t12710 = rx(t507,t126,t1364,1,0)
        t12714 = rx(t507,t126,t1364,2,0)
        t12716 = rx(t507,t126,t1364,0,2)
        t12722 = 0.1E1 / (t12700 * t12701 * t12703 - t12700 * t12705 * t
     #12707 - t12701 * t12714 * t12716 - t12703 * t12709 * t12710 + t127
     #05 * t12709 * t12714 + t12707 * t12710 * t12716)
        t12723 = t4 * t12722
        t12731 = t8841 / 0.2E1 + (t8116 - t12004) * t108 / 0.2E1
        t12744 = t8170 / 0.2E1 + (t8116 - t12690) * t124 / 0.2E1
        t12750 = t12714 ** 2
        t12751 = t12707 ** 2
        t12752 = t12703 ** 2
        t12035 = t12661 * (t12638 * t12648 + t12639 * t12647 + t12643 * 
     #t12654)
        t12051 = t12661 * (t12639 * t12645 + t12641 * t12643 + t12648 * 
     #t12652)
        t12056 = t12723 * (t12700 * t12714 + t12703 * t12716 + t12707 * 
     #t12709)
        t12062 = t12723 * (t12701 * t12707 + t12703 * t12705 + t12710 * 
     #t12714)
        t12761 = (t8737 - t4 * (t8733 / 0.2E1 + t11765 * (t12617 + t1261
     #8 + t12619) / 0.2E1) * t7884) * t108 + t8744 + (-t11766 * t11826 *
     # t11961 + t8741) * t108 / 0.2E1 + t8749 + (-t11215 * t12008 + t874
     #6) * t108 / 0.2E1 + t8078 + (-t12035 * t12669 + t8075) * t124 / 0.
     #2E1 + (t8101 - t4 * (t8097 / 0.2E1 + t12660 * (t12675 + t12676 + t
     #12677) / 0.2E1) * t7936) * t124 + t8125 + (-t12051 * t12694 + t812
     #2) * t124 / 0.2E1 + (t12056 * t12731 - t7888) * t177 / 0.2E1 + t78
     #93 + (t12062 * t12744 - t7940) * t177 / 0.2E1 + t7945 + (t4 * (t12
     #722 * (t12750 + t12751 + t12752) / 0.2E1 + t7963 / 0.2E1) * t8118 
     #- t7972) * t177
        t12762 = t12761 * t7876
        t12765 = t11782 ** 2
        t12766 = t11791 ** 2
        t12767 = t11798 ** 2
        t12786 = rx(t507,t1111,t179,0,0)
        t12787 = rx(t507,t1111,t179,1,1)
        t12789 = rx(t507,t1111,t179,2,2)
        t12791 = rx(t507,t1111,t179,1,2)
        t12793 = rx(t507,t1111,t179,2,1)
        t12795 = rx(t507,t1111,t179,0,1)
        t12796 = rx(t507,t1111,t179,1,0)
        t12800 = rx(t507,t1111,t179,2,0)
        t12802 = rx(t507,t1111,t179,0,2)
        t12808 = 0.1E1 / (t12786 * t12787 * t12789 - t12786 * t12791 * t
     #12793 - t12787 * t12800 * t12802 - t12789 * t12795 * t12796 + t127
     #91 * t12795 * t12800 + t12793 * t12796 * t12802)
        t12809 = t4 * t12808
        t12817 = t8927 / 0.2E1 + (t7845 - t11733) * t108 / 0.2E1
        t12823 = t12796 ** 2
        t12824 = t12787 ** 2
        t12825 = t12791 ** 2
        t12838 = u(t507,t1111,t1375,n)
        t12842 = t7847 / 0.2E1 + (t7845 - t12838) * t177 / 0.2E1
        t12848 = rx(t507,t126,t1375,0,0)
        t12849 = rx(t507,t126,t1375,1,1)
        t12851 = rx(t507,t126,t1375,2,2)
        t12853 = rx(t507,t126,t1375,1,2)
        t12855 = rx(t507,t126,t1375,2,1)
        t12857 = rx(t507,t126,t1375,0,1)
        t12858 = rx(t507,t126,t1375,1,0)
        t12862 = rx(t507,t126,t1375,2,0)
        t12864 = rx(t507,t126,t1375,0,2)
        t12870 = 0.1E1 / (t12848 * t12849 * t12851 - t12848 * t12853 * t
     #12855 - t12849 * t12862 * t12864 - t12851 * t12857 * t12858 + t128
     #53 * t12857 * t12862 + t12855 * t12858 * t12864)
        t12871 = t4 * t12870
        t12879 = t8989 / 0.2E1 + (t8314 - t12202) * t108 / 0.2E1
        t12892 = t8368 / 0.2E1 + (t8314 - t12838) * t124 / 0.2E1
        t12898 = t12862 ** 2
        t12899 = t12855 ** 2
        t12900 = t12851 ** 2
        t12168 = t12809 * (t12786 * t12796 + t12787 * t12795 + t12791 * 
     #t12802)
        t12188 = t12809 * (t12787 * t12793 + t12789 * t12791 + t12796 * 
     #t12800)
        t12195 = t12871 * (t12848 * t12862 + t12851 * t12864 + t12855 * 
     #t12857)
        t12201 = t12871 * (t12849 * t12855 + t12851 * t12853 + t12858 * 
     #t12862)
        t12909 = (t8885 - t4 * (t8881 / 0.2E1 + t11804 * (t12765 + t1276
     #6 + t12767) / 0.2E1) * t7923) * t108 + t8892 + (-t11805 * t11841 *
     # t12159 + t8889) * t108 / 0.2E1 + t8897 + (-t11222 * t12206 + t889
     #4) * t108 / 0.2E1 + t8276 + (-t12168 * t12817 + t8273) * t124 / 0.
     #2E1 + (t8299 - t4 * (t8295 / 0.2E1 + t12808 * (t12823 + t12824 + t
     #12825) / 0.2E1) * t7951) * t124 + t8323 + (-t12188 * t12842 + t832
     #0) * t124 / 0.2E1 + t7930 + (-t12195 * t12879 + t7927) * t177 / 0.
     #2E1 + t7958 + (-t12201 * t12892 + t7955) * t177 / 0.2E1 + (t7981 -
     # t4 * (t7977 / 0.2E1 + t12870 * (t12898 + t12899 + t12900) / 0.2E1
     #) * t8316) * t177
        t12910 = t12909 * t7915
        t12927 = t564 * t12293
        t12949 = t2427 * t7989
        t12292 = ((t12457 - t7721) * t177 / 0.2E1 + (t7721 - t12605) * t
     #177 / 0.2E1) * t2482
        t12299 = ((t12762 - t7985) * t177 / 0.2E1 + (t7985 - t12910) * t
     #177 / 0.2E1) * t2523
        t12323 = ((t12457 - t8190) * t124 / 0.2E1 + (t8190 - t12762) * t
     #124 / 0.2E1) * t2625
        t12328 = ((t12605 - t8388) * t124 / 0.2E1 + (t8388 - t12910) * t
     #124 / 0.2E1) * t2664
        t12968 = (-t11344 * t2417 + t7457) * t108 + t7994 + (t7991 - t22
     #61 * ((t11609 - t11342) * t124 / 0.2E1 + (t11342 - t11873) * t124 
     #/ 0.2E1)) * t108 / 0.2E1 + t8397 + (t8394 - t2281 * ((t12078 - t11
     #342) * t177 / 0.2E1 + (t11342 - t12276) * t177 / 0.2E1)) * t108 / 
     #0.2E1 + (t2329 * (t8399 / 0.2E1 + (t7721 - t11609) * t108 / 0.2E1)
     # - t12295) * t124 / 0.2E1 + (t12295 - t2377 * (t8412 / 0.2E1 + (t7
     #985 - t11873) * t108 / 0.2E1)) * t124 / 0.2E1 + (t2549 * t7723 - t
     #2558 * t7987) * t124 + (t12292 * t2565 - t12613) * t124 / 0.2E1 + 
     #(-t12299 * t2588 + t12613) * t124 / 0.2E1 + (t2484 * (t9033 / 0.2E
     #1 + (t8190 - t12078) * t108 / 0.2E1) - t12927) * t177 / 0.2E1 + (t
     #12927 - t2520 * (t9044 / 0.2E1 + (t8388 - t12276) * t108 / 0.2E1))
     # * t177 / 0.2E1 + (t12323 * t2681 - t12949) * t177 / 0.2E1 + (-t12
     #328 * t2698 + t12949) * t177 / 0.2E1 + (t2722 * t8192 - t2731 * t8
     #390) * t177
        t12969 = t2385 * t12968
        t12974 = t2379 * (t2738 / 0.2E1 + t10784 / 0.2E1)
        t12976 = t864 * t12974 / 0.4E1
        t12985 = t538 * t7449 * t10224 / 0.6E1
        t12987 = t7451 * t12969 / 0.12E2
        t12988 = t11336 + t9091 - t11339 - t7018 * t11328 / 0.8E1 - t338
     #6 * t12969 / 0.12E2 + t12976 - t2378 * t12974 / 0.4E1 + t9114 - t9
     #116 - t9221 + t9223 - t2751 * t10640 / 0.2E1 - t2378 * t10786 / 0.
     #24E2 - t12985 + t12987 - t9229
        t12990 = (t11334 + t12988) * t65
        t12994 = t9254 / 0.2E1
        t12995 = -t12990 * t863 - t10642 + t10649 - t10788 - t11225 - t1
     #1330 - t11336 - t12994 - t2745 + t7017 + t9086 + t9238
        t12997 = dx * t3159 / 0.24E2
        t12999 = sqrt(t10578)
        t13007 = (t9276 - (t9274 - (-cc * t10293 * t10799 * t12999 + t92
     #72) * t108) * t108) * t108
        t13013 = t868 * (t9276 - dx * (t9278 - t13007) / 0.12E2) / 0.24E
     #2
        t13021 = dx * (t9257 + t9274 / 0.2E1 - t868 * (t9278 / 0.2E1 + t
     #13007 / 0.2E1) / 0.6E1) / 0.4E1
        t13025 = t3306 * (t1007 - dx * t3154 / 0.24E2)
        t13026 = t11339 - t12976 + t9116 - t9223 - t12997 - t13013 - t13
     #021 + t12985 - t12987 + t13025 - t9285 + t9291 + t9229
        t13041 = t4 * (t9325 + t9329 / 0.2E1 - dx * ((t9322 - t9324) * t
     #108 / 0.2E1 - (-t2409 * t2428 + t9329) * t108 / 0.2E1) / 0.8E1)
        t13052 = (t6996 - t6998) * t124
        t13069 = t9344 + t9345 - t9349 + t552 / 0.4E1 + t555 / 0.4E1 - t
     #9388 / 0.12E2 - dx * ((t9366 + t9367 - t9368 - t9371 - t9372 + t93
     #73) * t108 / 0.2E1 - (t9374 + t9375 - t9389 - t6996 / 0.2E1 - t699
     #8 / 0.2E1 + t1046 * (((t11182 - t6996) * t124 - t13052) * t124 / 0
     #.2E1 + (t13052 - (t6998 - t11187) * t124) * t124 / 0.2E1) / 0.6E1)
     # * t108 / 0.2E1) / 0.8E1
        t13074 = t4 * (t9324 / 0.2E1 + t9329 / 0.2E1)
        t13076 = t4592 / 0.4E1 + t4750 / 0.4E1 + t7723 / 0.4E1 + t7987 /
     # 0.4E1
        t13082 = (-t6528 * t7490 + t9577) * t108
        t13088 = (t9583 - t7182 * (t11182 / 0.2E1 + t6996 / 0.2E1)) * t1
     #08
        t13093 = (-t11257 * t7494 * t7512 + t9588) * t108
        t13098 = (t9141 - t11250) * t108
        t13104 = t4188 * t9120
        t13109 = (t9144 - t11253) * t108
        t13122 = t2419 * t9581
        t12480 = (t9595 / 0.2E1 + t13098 / 0.2E1) * t7614
        t12492 = (t9606 / 0.2E1 + t13109 / 0.2E1) * t7653
        t12499 = (t11052 / 0.2E1 + t9186 / 0.2E1) * t7614
        t12506 = (t11071 / 0.2E1 + t9199 / 0.2E1) * t7653
        t13137 = t13082 + t9586 + t13088 / 0.2E1 + t9591 + t13093 / 0.2E
     #1 + t11140 / 0.2E1 + t9129 + t10897 + t10986 / 0.2E1 + t9155 + (t1
     #2480 * t7618 - t13104) * t177 / 0.2E1 + (-t12492 * t7657 + t13104)
     # * t177 / 0.2E1 + (t12499 * t7670 - t13122) * t177 / 0.2E1 + (-t12
     #506 * t7685 + t13122) * t177 / 0.2E1 + (t7707 * t9143 - t7716 * t9
     #146) * t177
        t13138 = t13137 * t2480
        t13143 = (-t6546 * t7754 + t9638) * t108
        t13149 = (t9644 - t7332 * (t6998 / 0.2E1 + t11187 / 0.2E1)) * t1
     #08
        t13154 = (-t11272 * t7758 * t7776 + t9649) * t108
        t13159 = (t9156 - t11265) * t108
        t13165 = t4327 * t9131
        t13170 = (t9159 - t11268) * t108
        t13183 = t2444 * t9642
        t12542 = (t9656 / 0.2E1 + t13159 / 0.2E1) * t7878
        t12554 = (t9667 / 0.2E1 + t13170 / 0.2E1) * t7917
        t12561 = (t9188 / 0.2E1 + t11057 / 0.2E1) * t7878
        t12568 = (t9201 / 0.2E1 + t11076 / 0.2E1) * t7917
        t13198 = t13143 + t9647 + t13149 / 0.2E1 + t9652 + t13154 / 0.2E
     #1 + t9136 + t11155 / 0.2E1 + t10902 + t9168 + t11004 / 0.2E1 + (t1
     #2542 * t7882 - t13165) * t177 / 0.2E1 + (-t12554 * t7921 + t13165)
     # * t177 / 0.2E1 + (t12561 * t7934 - t13183) * t177 / 0.2E1 + (-t12
     #568 * t7949 + t13183) * t177 / 0.2E1 + (t7971 * t9158 - t7980 * t9
     #161) * t177
        t13199 = t13198 * t2521
        t13203 = t9637 / 0.4E1 + t9698 / 0.4E1 + (t13138 - t10221) * t12
     #4 / 0.4E1 + (t10221 - t13199) * t124 / 0.4E1
        t13209 = dx * (t168 / 0.2E1 - t7004 / 0.2E1)
        t13213 = t13041 * t9108 * t13069
        t13216 = t13074 * t9711 * t13076 / 0.2E1
        t13219 = t13074 * t9715 * t13203 / 0.6E1
        t13221 = t9108 * t13209 / 0.24E2
        t13223 = (t13041 * t2746 * t13069 + t13074 * t9402 * t13076 / 0.
     #2E1 + t13074 * t9408 * t13203 / 0.6E1 - t2746 * t13209 / 0.24E2 - 
     #t13213 - t13216 - t13219 + t13221) * t65
        t13236 = (t2431 - t2434) * t124
        t13254 = t13041 * (t9731 + t9732 - t9736 + t1797 / 0.4E1 + t1799
     # / 0.4E1 - t9775 / 0.12E2 - dx * ((t9753 + t9754 - t9755 - t9758 -
     # t9759 + t9760) * t108 / 0.2E1 - (t9761 + t9762 - t9776 - t2431 / 
     #0.2E1 - t2434 / 0.2E1 + t1046 * (((t7501 - t2431) * t124 - t13236)
     # * t124 / 0.2E1 + (t13236 - (t2434 - t7765) * t124) * t124 / 0.2E1
     #) / 0.6E1) * t108 / 0.2E1) / 0.8E1)
        t13258 = dx * (t1226 / 0.2E1 - t2440 / 0.2E1) / 0.24E2
        t13274 = t4 * (t9796 + t9800 / 0.2E1 - dx * ((t9793 - t9795) * t
     #108 / 0.2E1 - (-t2409 * t2445 + t9800) * t108 / 0.2E1) / 0.8E1)
        t13285 = (t6663 - t6665) * t177
        t13302 = t9815 + t9816 - t9820 + t569 / 0.4E1 + t572 / 0.4E1 - t
     #9859 / 0.12E2 - dx * ((t9837 + t9838 - t9839 - t9842 - t9843 + t98
     #44) * t108 / 0.2E1 - (t9845 + t9846 - t9860 - t6663 / 0.2E1 - t666
     #5 / 0.2E1 + t1363 * (((t11110 - t6663) * t177 - t13285) * t177 / 0
     #.2E1 + (t13285 - (t6665 - t11115) * t177) * t177 / 0.2E1) / 0.6E1)
     # * t108 / 0.2E1) / 0.8E1
        t13307 = t4 * (t9795 / 0.2E1 + t9800 / 0.2E1)
        t13309 = t5526 / 0.4E1 + t5620 / 0.4E1 + t8192 / 0.4E1 + t8390 /
     # 0.4E1
        t13315 = (-t6456 * t8025 + t10022) * t108
        t13319 = (-t11299 * t8029 * t8033 + t10026) * t108
        t13326 = (t10033 - t7532 * (t11110 / 0.2E1 + t6663 / 0.2E1)) * t
     #108
        t13331 = t5140 * t9170
        t13349 = t2540 * t10031
        t12743 = (t10849 / 0.2E1 + t9143 / 0.2E1) * t7614
        t12749 = (t10870 / 0.2E1 + t9158 / 0.2E1) * t7878
        t13362 = t13315 + t10029 + t13319 / 0.2E1 + t10036 + t13326 / 0.
     #2E1 + (t12480 * t8062 - t13331) * t124 / 0.2E1 + (-t12542 * t8073 
     #+ t13331) * t124 / 0.2E1 + (t8091 * t9186 - t8100 * t9188) * t124 
     #+ (t12743 * t7670 - t13349) * t124 / 0.2E1 + (-t12749 * t7934 + t1
     #3349) * t124 / 0.2E1 + t10918 / 0.2E1 + t9177 + t11022 / 0.2E1 + t
     #9197 + t11096
        t13363 = t13362 * t2623
        t13368 = (-t6484 * t8223 + t10075) * t108
        t13372 = (-t11312 * t8227 * t8231 + t10079) * t108
        t13379 = (t10086 - t7714 * (t6665 / 0.2E1 + t11115 / 0.2E1)) * t
     #108
        t13384 = t5187 * t9179
        t13402 = t2562 * t10084
        t12794 = (t9146 / 0.2E1 + t10855 / 0.2E1) * t7653
        t12803 = (t9161 / 0.2E1 + t10876 / 0.2E1) * t7917
        t13415 = t13368 + t10082 + t13372 / 0.2E1 + t10089 + t13379 / 0.
     #2E1 + (t12492 * t8260 - t13384) * t124 / 0.2E1 + (-t12554 * t8271 
     #+ t13384) * t124 / 0.2E1 + (t8289 * t9199 - t8298 * t9201) * t124 
     #+ (t12794 * t7685 - t13402) * t124 / 0.2E1 + (-t12803 * t7949 + t1
     #3402) * t124 / 0.2E1 + t9184 + t10933 / 0.2E1 + t9208 + t11038 / 0
     #.2E1 + t11101
        t13416 = t13415 * t2662
        t13420 = t10074 / 0.4E1 + t10127 / 0.4E1 + (t13363 - t10221) * t
     #177 / 0.4E1 + (t10221 - t13416) * t177 / 0.4E1
        t13426 = dx * (t219 / 0.2E1 - t6671 / 0.2E1)
        t13430 = t13274 * t9108 * t13302
        t13433 = t13307 * t9711 * t13309 / 0.2E1
        t13436 = t13307 * t9715 * t13420 / 0.6E1
        t13438 = t9108 * t13426 / 0.24E2
        t13440 = (t13274 * t2746 * t13302 + t13307 * t9402 * t13309 / 0.
     #2E1 + t13307 * t9408 * t13420 / 0.6E1 - t2746 * t13426 / 0.24E2 - 
     #t13430 - t13433 - t13436 + t13438) * t65
        t13453 = (t2448 - t2451) * t177
        t13471 = t13274 * (t10158 + t10159 - t10163 + t956 / 0.4E1 + t95
     #9 / 0.4E1 - t10202 / 0.12E2 - dx * ((t10180 + t10181 - t10182 - t1
     #0185 - t10186 + t10187) * t108 / 0.2E1 - (t10188 + t10189 - t10203
     # - t2448 / 0.2E1 - t2451 / 0.2E1 + t1363 * (((t8051 - t2448) * t17
     #7 - t13453) * t177 / 0.2E1 + (t13453 - (t2451 - t8249) * t177) * t
     #177 / 0.2E1) / 0.6E1) * t108 / 0.2E1) / 0.8E1)
        t13475 = dx * (t948 / 0.2E1 - t2457 / 0.2E1) / 0.24E2
        t13480 = t9232 * t71 / 0.6E1 + (t9243 + t9318) * t71 / 0.2E1 + t
     #9722 * t71 / 0.6E1 + (-t863 * t9722 + t9710 + t9714 + t9718 - t972
     #0 + t9784 - t9788) * t71 / 0.2E1 + t10149 * t71 / 0.6E1 + (-t10149
     # * t863 + t10139 + t10142 + t10145 - t10147 + t10211 - t10215) * t
     #71 / 0.2E1 - t12990 * t71 / 0.6E1 - (t12995 + t13026) * t71 / 0.2E
     #1 - t13223 * t71 / 0.6E1 - (-t13223 * t863 + t13213 + t13216 + t13
     #219 - t13221 + t13254 - t13258) * t71 / 0.2E1 - t13440 * t71 / 0.6
     #E1 - (-t13440 * t863 + t13430 + t13433 + t13436 - t13438 + t13471 
     #- t13475) * t71 / 0.2E1
        t13483 = t602 * t607
        t13488 = t643 * t648
        t13496 = t4 * (t13483 / 0.2E1 + t9325 - dy * ((t2825 * t3211 - t
     #13483) * t124 / 0.2E1 - (t9324 - t13488) * t124 / 0.2E1) / 0.8E1)
        t13502 = (t252 - t609) * t108
        t13504 = ((t250 - t252) * t108 - t13502) * t108
        t13508 = (t13502 - (t609 - t6528) * t108) * t108
        t13511 = t868 * (t13504 / 0.2E1 + t13508 / 0.2E1)
        t13513 = t112 / 0.4E1
        t13514 = t541 / 0.4E1
        t13517 = t868 * (t6503 / 0.2E1 + t6508 / 0.2E1)
        t13518 = t13517 / 0.12E2
        t13524 = (t6740 - t6743) * t108
        t13535 = t252 / 0.2E1
        t13536 = t609 / 0.2E1
        t13537 = t13511 / 0.6E1
        t13540 = t112 / 0.2E1
        t13541 = t541 / 0.2E1
        t13542 = t13517 / 0.6E1
        t13543 = t295 / 0.2E1
        t13544 = t650 / 0.2E1
        t13548 = (t295 - t650) * t108
        t13550 = ((t293 - t295) * t108 - t13548) * t108
        t13554 = (t13548 - (t650 - t6546) * t108) * t108
        t13557 = t868 * (t13550 / 0.2E1 + t13554 / 0.2E1)
        t13558 = t13557 / 0.6E1
        t13565 = t252 / 0.4E1 + t609 / 0.4E1 - t13511 / 0.12E2 + t13513 
     #+ t13514 - t13518 - dy * ((t6740 / 0.2E1 + t6743 / 0.2E1 - t868 * 
     #(((t7269 - t6740) * t108 - t13524) * t108 / 0.2E1 + (t13524 - (t67
     #43 - t11134) * t108) * t108 / 0.2E1) / 0.6E1 - t13535 - t13536 + t
     #13537) * t124 / 0.2E1 - (t13540 + t13541 - t13542 - t13543 - t1354
     #4 + t13558) * t124 / 0.2E1) / 0.8E1
        t13570 = t4 * (t13483 / 0.2E1 + t9324 / 0.2E1)
        t13572 = t5631 / 0.4E1 + t8399 / 0.4E1 + t3488 / 0.4E1 + t7456 /
     # 0.4E1
        t13580 = t858 * t108
        t13581 = t10222 * t108
        t13583 = (t9490 - t9635) * t108 / 0.4E1 + (t9635 - t13138) * t10
     #8 / 0.4E1 + t13580 / 0.4E1 + t13581 / 0.4E1
        t13589 = dy * (t6749 / 0.2E1 - t656 / 0.2E1)
        t13593 = t13496 * t9108 * t13565
        t13596 = t13570 * t9711 * t13572 / 0.2E1
        t13599 = t13570 * t9715 * t13583 / 0.6E1
        t13601 = t9108 * t13589 / 0.24E2
        t13603 = (t13496 * t2746 * t13565 + t13570 * t9402 * t13572 / 0.
     #2E1 + t13570 * t9408 * t13583 / 0.6E1 - t2746 * t13589 / 0.24E2 - 
     #t13593 - t13596 - t13599 + t13601) * t65
        t13611 = (t981 - t989) * t108
        t13613 = ((t986 - t981) * t108 - t13611) * t108
        t13617 = (t13611 - (t989 - t2488) * t108) * t108
        t13620 = t868 * (t13613 / 0.2E1 + t13617 / 0.2E1)
        t13622 = t1000 / 0.4E1
        t13623 = t1007 / 0.4E1
        t13626 = t868 * (t1870 / 0.2E1 + t3155 / 0.2E1)
        t13627 = t13626 / 0.12E2
        t13633 = (t1082 - t3213) * t108
        t13644 = t981 / 0.2E1
        t13645 = t989 / 0.2E1
        t13646 = t13620 / 0.6E1
        t13649 = t1000 / 0.2E1
        t13650 = t1007 / 0.2E1
        t13651 = t13626 / 0.6E1
        t13652 = t1024 / 0.2E1
        t13653 = t1032 / 0.2E1
        t13657 = (t1024 - t1032) * t108
        t13659 = ((t1029 - t1024) * t108 - t13657) * t108
        t13663 = (t13657 - (t1032 - t2529) * t108) * t108
        t13666 = t868 * (t13659 / 0.2E1 + t13663 / 0.2E1)
        t13667 = t13666 / 0.6E1
        t13675 = t13496 * (t981 / 0.4E1 + t989 / 0.4E1 - t13620 / 0.12E2
     # + t13622 + t13623 - t13627 - dy * ((t1082 / 0.2E1 + t3213 / 0.2E1
     # - t868 * (((t1079 - t1082) * t108 - t13633) * t108 / 0.2E1 + (t13
     #633 - (t3213 - t7555) * t108) * t108 / 0.2E1) / 0.6E1 - t13644 - t
     #13645 + t13646) * t124 / 0.2E1 - (t13649 + t13650 - t13651 - t1365
     #2 - t13653 + t13667) * t124 / 0.2E1) / 0.8E1)
        t13679 = dy * (t3219 / 0.2E1 - t2293 / 0.2E1) / 0.24E2
        t13684 = sqrt(t666)
        t13685 = cc * t13684
        t13686 = t13685 * t3380
        t13688 = t865 * t13686 / 0.2E1
        t13689 = dy * t6865
        t13693 = t9108 * t13689 / 0.24E2
        t13694 = dt * dy
        t13695 = sqrt(t661)
        t13696 = cc * t13695
        t13697 = t13696 * t4589
        t13698 = t13685 * t2371
        t13700 = (t13697 - t13698) * t124
        t13701 = sqrt(t675)
        t13702 = cc * t13701
        t13703 = t13702 * t4747
        t13705 = (t13698 - t13703) * t124
        t13707 = t13694 * (t13700 - t13705)
        t13709 = t2378 * t13707 / 0.24E2
        t13710 = t13685 * t9080
        t13712 = t3386 * t13710 / 0.12E2
        t13715 = t72 * t9636 * t124
        t13717 = t670 * t7449 * t13715 / 0.6E1
        t13720 = t71 * t4591 * t124
        t13722 = t670 * t1944 * t13720 / 0.2E1
        t13730 = t864 * t13707 / 0.24E2
        t13733 = t159 - dy * t6852 / 0.24E2
        t13737 = t3257 * t9108 * t13733
        t13740 = t13694 * (t13700 / 0.2E1 + t13705 / 0.2E1)
        t13742 = t2378 * t13740 / 0.4E1
        t13743 = sqrt(t2829)
        t13744 = cc * t13743
        t13745 = t1048 ** 2
        t13746 = t1057 ** 2
        t13747 = t1064 ** 2
        t13749 = t1070 * (t13745 + t13746 + t13747)
        t13750 = t2803 ** 2
        t13751 = t2812 ** 2
        t13752 = t2819 ** 2
        t13754 = t2825 * (t13750 + t13751 + t13752)
        t13757 = t4 * (t13749 / 0.2E1 + t13754 / 0.2E1)
        t13758 = t13757 * t1082
        t13759 = t7526 ** 2
        t13760 = t7535 ** 2
        t13761 = t7542 ** 2
        t13763 = t7548 * (t13759 + t13760 + t13761)
        t13766 = t4 * (t13754 / 0.2E1 + t13763 / 0.2E1)
        t13767 = t13766 * t3213
        t13770 = j + 3
        t13771 = u(t5,t13770,k,n)
        t13773 = (t13771 - t1077) * t124
        t13775 = t13773 / 0.2E1 + t1695 / 0.2E1
        t13777 = t1004 * t13775
        t13778 = u(i,t13770,k,n)
        t13780 = (t13778 - t1080) * t124
        t13782 = t13780 / 0.2E1 + t1916 / 0.2E1
        t13784 = t3010 * t13782
        t13787 = (t13777 - t13784) * t108 / 0.2E1
        t13788 = u(t507,t13770,k,n)
        t13790 = (t13788 - t3166) * t124
        t13792 = t13790 / 0.2E1 + t3168 / 0.2E1
        t13794 = t7214 * t13792
        t13797 = (t13784 - t13794) * t108 / 0.2E1
        t13142 = t1071 * (t1048 * t1062 + t1051 * t1064 + t1055 * t1057)
        t13803 = t13142 * t1284
        t13147 = t2880 * (t2803 * t2817 + t2806 * t2819 + t2810 * t2812)
        t13809 = t13147 * t2892
        t13812 = (t13803 - t13809) * t108 / 0.2E1
        t13816 = t7526 * t7540 + t7529 * t7542 + t7533 * t7535
        t13155 = t7549 * t13816
        t13818 = t13155 * t7585
        t13821 = (t13809 - t13818) * t108 / 0.2E1
        t13822 = rx(i,t13770,k,0,0)
        t13823 = rx(i,t13770,k,1,1)
        t13825 = rx(i,t13770,k,2,2)
        t13827 = rx(i,t13770,k,1,2)
        t13829 = rx(i,t13770,k,2,1)
        t13831 = rx(i,t13770,k,0,1)
        t13832 = rx(i,t13770,k,1,0)
        t13836 = rx(i,t13770,k,2,0)
        t13838 = rx(i,t13770,k,0,2)
        t13844 = 0.1E1 / (t13822 * t13823 * t13825 - t13822 * t13827 * t
     #13829 - t13823 * t13836 * t13838 - t13825 * t13831 * t13832 + t138
     #27 * t13831 * t13836 + t13829 * t13832 * t13838)
        t13845 = t4 * t13844
        t13851 = (t13771 - t13778) * t108
        t13853 = (t13778 - t13788) * t108
        t13182 = t13845 * (t13822 * t13832 + t13823 * t13831 + t13827 * 
     #t13838)
        t13859 = (t13182 * (t13851 / 0.2E1 + t13853 / 0.2E1) - t3217) * 
     #t124
        t13861 = t13832 ** 2
        t13862 = t13823 ** 2
        t13863 = t13827 ** 2
        t13864 = t13861 + t13862 + t13863
        t13865 = t13844 * t13864
        t13868 = t4 * (t13865 / 0.2E1 + t2830 / 0.2E1)
        t13871 = (t13780 * t13868 - t2834) * t124
        t13876 = u(i,t13770,t174,n)
        t13878 = (t13876 - t13778) * t177
        t13879 = u(i,t13770,t179,n)
        t13881 = (t13778 - t13879) * t177
        t13200 = t13845 * (t13823 * t13829 + t13825 * t13827 + t13832 * 
     #t13836)
        t13887 = (t13200 * (t13878 / 0.2E1 + t13881 / 0.2E1) - t2894) * 
     #t124
        t13892 = t8445 * t8459 + t8448 * t8461 + t8452 * t8454
        t13210 = t8468 * t13892
        t13894 = t13210 * t8476
        t13896 = t13147 * t3215
        t13899 = (t13894 - t13896) * t177 / 0.2E1
        t13903 = t8593 * t8607 + t8596 * t8609 + t8600 * t8602
        t13218 = t8616 * t13903
        t13905 = t13218 * t8624
        t13908 = (t13896 - t13905) * t177 / 0.2E1
        t13910 = (t13876 - t2885) * t124
        t13912 = t13910 / 0.2E1 + t2980 / 0.2E1
        t13914 = t7933 * t13912
        t13916 = t2692 * t13782
        t13919 = (t13914 - t13916) * t177 / 0.2E1
        t13921 = (t13879 - t2888) * t124
        t13923 = t13921 / 0.2E1 + t2999 / 0.2E1
        t13925 = t8074 * t13923
        t13928 = (t13916 - t13925) * t177 / 0.2E1
        t13929 = t8459 ** 2
        t13930 = t8452 ** 2
        t13931 = t8448 ** 2
        t13933 = t8467 * (t13929 + t13930 + t13931)
        t13934 = t2817 ** 2
        t13935 = t2810 ** 2
        t13936 = t2806 ** 2
        t13938 = t2825 * (t13934 + t13935 + t13936)
        t13941 = t4 * (t13933 / 0.2E1 + t13938 / 0.2E1)
        t13942 = t13941 * t2887
        t13943 = t8607 ** 2
        t13944 = t8600 ** 2
        t13945 = t8596 ** 2
        t13947 = t8615 * (t13943 + t13944 + t13945)
        t13950 = t4 * (t13938 / 0.2E1 + t13947 / 0.2E1)
        t13951 = t13950 * t2890
        t13954 = (t13758 - t13767) * t108 + t13787 + t13797 + t13812 + t
     #13821 + t13859 / 0.2E1 + t4462 + t13871 + t13887 / 0.2E1 + t4463 +
     # t13899 + t13908 + t13919 + t13928 + (t13942 - t13951) * t177
        t13957 = (t13744 * t13954 - t13697) * t124
        t13960 = t13694 * (t13957 / 0.2E1 + t13700 / 0.2E1)
        t13962 = t864 * t13960 / 0.4E1
        t13255 = ((t13780 / 0.2E1 - t1218 / 0.2E1) * t124 - t1919) * t12
     #4
        t13978 = t683 * t13255
        t14002 = t4 * (t2830 / 0.2E1 + t3246 - dy * ((t13865 - t2830) * 
     #t124 / 0.2E1 - t3261 / 0.2E1) / 0.8E1)
        t14009 = (t4501 - t4538) * t177
        t14020 = t4552 + t4563 + t4138 + t2313 + t4129 + t4452 + t4461 +
     # t4462 + t4463 + t4502 + t4539 + t2287 - t1046 * ((t4276 * ((t1391
     #0 / 0.2E1 - t2344 / 0.2E1) * t124 - t2983) * t124 - t13978) * t177
     # / 0.2E1 + (t13978 - t4289 * ((t13921 / 0.2E1 - t2357 / 0.2E1) * t
     #124 - t3002) * t124) * t177 / 0.2E1) / 0.6E1 + (t14002 * t1916 - t
     #3258) * t124 - t1363 * (((t8542 - t4501) * t177 - t14009) * t177 /
     # 0.2E1 + (t14009 - (t4538 - t8690) * t177) * t177 / 0.2E1) / 0.6E1
        t14033 = t3948 * t3081
        t14063 = (t2301 - t2304) * t177
        t14065 = ((t2938 - t2301) * t177 - t14063) * t177
        t14070 = (t14063 - (t2304 - t2944) * t177) * t177
        t14086 = (t4551 - t4562) * t177
        t14098 = t4573 / 0.2E1
        t14108 = t4 * (t4568 / 0.2E1 + t14098 - dz * ((t8561 - t4568) * 
     #t177 / 0.2E1 - (t4573 - t4582) * t177 / 0.2E1) / 0.8E1)
        t14120 = t4 * (t14098 + t4582 / 0.2E1 - dz * ((t4568 - t4573) * 
     #t177 / 0.2E1 - (t4582 - t8709) * t177 / 0.2E1) / 0.8E1)
        t14157 = (t4137 - t4460) * t108
        t14197 = t3948 * t2750
        t14210 = t4116 / 0.2E1
        t14220 = t4 * (t3533 / 0.2E1 + t14210 - dx * ((t3524 - t3533) * 
     #t108 / 0.2E1 - (t4116 - t4439) * t108 / 0.2E1) / 0.8E1)
        t14232 = t4 * (t14210 + t4439 / 0.2E1 - dx * ((t3533 - t4116) * 
     #t108 / 0.2E1 - (t4439 - t7487) * t108 / 0.2E1) / 0.8E1)
        t14239 = (t4128 - t4451) * t108
        t14259 = t604 * t13255
        t13380 = t108 * ((t3694 / 0.2E1 - t4493 / 0.2E1) * t108 - (t4170
     # / 0.2E1 - t7620 / 0.2E1) * t108)
        t13385 = t108 * ((t3735 / 0.2E1 - t4532 / 0.2E1) * t108 - (t4209
     # / 0.2E1 - t7659 / 0.2E1) * t108)
        t14288 = -t868 * ((t13380 * t4219 - t14033) * t177 / 0.2E1 + (-t
     #13385 * t4261 + t14033) * t177 / 0.2E1) / 0.6E1 - t1046 * (((t1388
     #7 - t2896) * t124 - t2898) * t124 / 0.2E1 + t2902 / 0.2E1) / 0.6E1
     # - t1363 * ((t14065 * t4576 - t14070 * t4585) * t177 + ((t8567 - t
     #4588) * t177 - (t4588 - t8715) * t177) * t177) / 0.24E2 - t1363 * 
     #(((t8555 - t4551) * t177 - t14086) * t177 / 0.2E1 + (t14086 - (t45
     #62 - t8703) * t177) * t177 / 0.2E1) / 0.6E1 + (t14108 * t2301 - t1
     #4120 * t2304) * t177 - t1046 * ((t2833 * ((t13780 - t1916) * t124 
     #- t2790) * t124 - t2795) * t124 + ((t13871 - t2836) * t124 - t2838
     #) * t124) / 0.24E2 - t1363 * ((t2692 * ((t8499 / 0.2E1 - t2890 / 0
     #.2E1) * t177 - (t2887 / 0.2E1 - t8647 / 0.2E1) * t177) * t177 - t2
     #951) * t124 / 0.2E1 + t2956 / 0.2E1) / 0.6E1 - t868 * (((t3596 - t
     #4137) * t108 - t14157) * t108 / 0.2E1 + (t14157 - (t4460 - t7524) 
     #* t108) * t108 / 0.2E1) / 0.6E1 - t868 * ((t3010 * ((t1079 / 0.2E1
     # - t3213 / 0.2E1) * t108 - (t1082 / 0.2E1 - t7555 / 0.2E1) * t108)
     # * t108 - t3279) * t124 / 0.2E1 + t3284 / 0.2E1) / 0.6E1 - t1046 *
     # (((t13859 - t3219) * t124 - t3221) * t124 / 0.2E1 + t3225 / 0.2E1
     #) / 0.6E1 - t1363 * ((t1777 * t3344 - t14197) * t108 / 0.2E1 + (-t
     #4188 * t9952 + t14197) * t108 / 0.2E1) / 0.6E1 + (t14220 * t981 - 
     #t14232 * t989) * t108 - t868 * (((t3564 - t4128) * t108 - t14239) 
     #* t108 / 0.2E1 + (t14239 - (t4451 - t7507) * t108) * t108 / 0.2E1)
     # / 0.6E1 - t1046 * ((t251 * ((t13773 / 0.2E1 - t1179 / 0.2E1) * t1
     #24 - t1830) * t124 - t14259) * t108 / 0.2E1 + (t14259 - t2329 * ((
     #t13790 / 0.2E1 - t1797 / 0.2E1) * t124 - t3171) * t124) * t108 / 0
     #.2E1) / 0.6E1 - t868 * ((t13613 * t4119 - t13617 * t4442) * t108 +
     # ((t4122 - t4445) * t108 - (t4445 - t7493) * t108) * t108) / 0.24E
     #2
        t14290 = t13696 * (t14020 + t14288)
        t14292 = t865 * t14290 / 0.2E1
        t14293 = t13688 - t2746 * t13689 / 0.24E2 + t13693 - t13709 - t1
     #3712 - t13717 - t13722 + t670 * t69 * t13715 / 0.6E1 + t670 * t68 
     #* t13720 / 0.2E1 + t13730 + t3257 * t2746 * t13733 - t13737 - t137
     #42 + t13962 - t2378 * t13960 / 0.4E1 - t14292
        t14295 = t7451 * t13710 / 0.12E2
        t14297 = t13694 * (t13957 - t13700)
        t14301 = t2751 * t13686 / 0.2E1
        t14302 = ut(i,t1047,t1364,n)
        t14304 = (t14302 - t6680) * t124
        t14310 = (t7990 * (t14304 / 0.2E1 + t6811 / 0.2E1) - t9617) * t1
     #77
        t14314 = (t9621 - t9628) * t177
        t14317 = ut(i,t1047,t1375,n)
        t14319 = (t14317 - t6686) * t124
        t14325 = (t9626 - t8134 * (t14319 / 0.2E1 + t6827 / 0.2E1)) * t1
     #77
        t14334 = ut(i,t13770,t174,n)
        t14336 = (t14334 - t6600) * t124
        t14344 = ut(i,t13770,k,n)
        t14346 = (t14344 - t6616) * t124
        t13656 = ((t14346 / 0.2E1 - t159 / 0.2E1) * t124 - t6621) * t124
        t14353 = t683 * t13656
        t14356 = ut(i,t13770,t179,n)
        t14358 = (t14356 - t6634) * t124
        t14377 = (t7105 - t6680) * t108 / 0.2E1 + (t6680 - t10847) * t10
     #8 / 0.2E1
        t14381 = (t14377 * t8530 * t8534 - t9599) * t177
        t14385 = (t9603 - t9612) * t177
        t14393 = (t7123 - t6686) * t108 / 0.2E1 + (t6686 - t10853) * t10
     #8 / 0.2E1
        t14397 = (-t14393 * t8678 * t8682 + t9610) * t177
        t14418 = t3948 * t6220
        t14440 = (t689 - t692) * t177
        t14442 = ((t6682 - t689) * t177 - t14440) * t177
        t14447 = (t14440 - (t692 - t6688) * t177) * t177
        t14453 = (t6682 * t8564 - t9630) * t177
        t14458 = (-t6688 * t8712 + t9631) * t177
        t13817 = t108 * ((t9446 / 0.2E1 - t9595 / 0.2E1) * t108 - (t9448
     # / 0.2E1 - t13098 / 0.2E1) * t108)
        t13826 = t108 * ((t9459 / 0.2E1 - t9606 / 0.2E1) * t108 - (t9461
     # / 0.2E1 - t13109 / 0.2E1) * t108)
        t14466 = t9592 + t620 + t705 + t9593 + t9629 + t9604 + t9613 + t
     #9622 + t9586 + t9591 - t1363 * (((t14310 - t9621) * t177 - t14314)
     # * t177 / 0.2E1 + (t14314 - (t9628 - t14325) * t177) * t177 / 0.2E
     #1) / 0.6E1 - t1046 * ((t4276 * ((t14336 / 0.2E1 - t804 / 0.2E1) * 
     #t124 - t6605) * t124 - t14353) * t177 / 0.2E1 + (t14353 - t4289 * 
     #((t14358 / 0.2E1 - t821 / 0.2E1) * t124 - t6639) * t124) * t177 / 
     #0.2E1) / 0.6E1 - t1363 * (((t14381 - t9603) * t177 - t14385) * t17
     #7 / 0.2E1 + (t14385 - (t9612 - t14397) * t177) * t177 / 0.2E1) / 0
     #.6E1 - t868 * ((t13817 * t4219 - t14418) * t177 / 0.2E1 + (-t13826
     # * t4261 + t14418) * t177 / 0.2E1) / 0.6E1 - t1363 * ((t14442 * t4
     #576 - t14447 * t4585) * t177 + ((t14453 - t9633) * t177 - (t9633 -
     # t14458) * t177) * t177) / 0.24E2
        t14476 = t3948 * t6311
        t14495 = (t9429 - t9585) * t108
        t14506 = ut(t5,t13770,k,n)
        t14508 = (t14506 - t6738) * t124
        t14518 = t604 * t13656
        t14521 = ut(t507,t13770,k,n)
        t14523 = (t14521 - t6741) * t124
        t14559 = (t13182 * ((t14506 - t14344) * t108 / 0.2E1 + (t14344 -
     # t14521) * t108 / 0.2E1) - t6747) * t124
        t14590 = (t9441 - t9590) * t108
        t14610 = (t13200 * ((t14334 - t14344) * t177 / 0.2E1 + (t14344 -
     # t14356) * t177 / 0.2E1) - t6923) * t124
        t14628 = (t13868 * t14346 - t6862) * t124
        t14637 = (t14302 - t6600) * t177
        t14642 = (t6634 - t14317) * t177
        t14656 = (t14108 * t689 - t14120 * t692) * t177 - t1363 * ((t359
     #2 * t6836 * t7157 - t14476) * t108 / 0.2E1 + (-t10337 * t4188 + t1
     #4476) * t108 / 0.2E1) / 0.6E1 + (t14220 * t252 - t14232 * t609) * 
     #t108 - t868 * (((t9422 - t9429) * t108 - t14495) * t108 / 0.2E1 + 
     #(t14495 - (t9585 - t13088) * t108) * t108 / 0.2E1) / 0.6E1 - t1046
     # * ((t251 * ((t14508 / 0.2E1 - t141 / 0.2E1) * t124 - t6954) * t12
     #4 - t14518) * t108 / 0.2E1 + (t14518 - t2329 * ((t14523 / 0.2E1 - 
     #t552 / 0.2E1) * t124 - t6973) * t124) * t108 / 0.2E1) / 0.6E1 - t8
     #68 * ((t13504 * t4119 - t13508 * t4442) * t108 + ((t9412 - t9579) 
     #* t108 - (t9579 - t13082) * t108) * t108) / 0.24E2 - t1046 * (((t1
     #4559 - t6749) * t124 - t6751) * t124 / 0.2E1 + t6755 / 0.2E1) / 0.
     #6E1 + (t14002 * t6618 - t6776) * t124 - t868 * ((t3010 * ((t7269 /
     # 0.2E1 - t6743 / 0.2E1) * t108 - (t6740 / 0.2E1 - t11134 / 0.2E1) 
     #* t108) * t108 - t6535) * t124 / 0.2E1 + t6540 / 0.2E1) / 0.6E1 - 
     #t868 * (((t9436 - t9441) * t108 - t14590) * t108 / 0.2E1 + (t14590
     # - (t9590 - t13093) * t108) * t108 / 0.2E1) / 0.6E1 - t1046 * (((t
     #14610 - t6925) * t124 - t6927) * t124 / 0.2E1 + t6931 / 0.2E1) / 0
     #.6E1 - t1046 * ((t2833 * ((t14346 - t6618) * t124 - t6849) * t124 
     #- t6854) * t124 + ((t14628 - t6864) * t124 - t6866) * t124) / 0.24
     #E2 - t1363 * ((t2692 * ((t14637 / 0.2E1 - t6919 / 0.2E1) * t177 - 
     #(t6917 / 0.2E1 - t14642 / 0.2E1) * t177) * t177 - t6695) * t124 / 
     #0.2E1 + t6711 / 0.2E1) / 0.6E1 + t9430 + t9442
        t14658 = t13696 * (t14466 + t14656)
        t14661 = t13685 * t7014
        t14663 = t6450 * t14661 / 0.4E1
        t14664 = t71 * dy
        t14665 = t13696 * t9634
        t14666 = t13685 * t856
        t14668 = (t14665 - t14666) * t124
        t14669 = t13702 * t9695
        t14671 = (t14666 - t14669) * t124
        t14674 = t14664 * (t14668 / 0.2E1 + t14671 / 0.2E1)
        t14676 = t1945 * t14674 / 0.8E1
        t14678 = t864 * t13740 / 0.4E1
        t14690 = t14346 / 0.2E1 + t6618 / 0.2E1
        t14692 = t3010 * t14690
        t14706 = t13147 * t6921
        t14722 = (t7038 - t6600) * t108 / 0.2E1 + (t6600 - t10975) * t10
     #8 / 0.2E1
        t14726 = t13147 * t6745
        t14735 = (t7041 - t6634) * t108 / 0.2E1 + (t6634 - t10978) * t10
     #8 / 0.2E1
        t14746 = t2692 * t14690
        t14761 = (t13757 * t6740 - t13766 * t6743) * t108 + (t1004 * (t1
     #4508 / 0.2E1 + t6951 / 0.2E1) - t14692) * t108 / 0.2E1 + (t14692 -
     # t7214 * (t14523 / 0.2E1 + t6970 / 0.2E1)) * t108 / 0.2E1 + (t1314
     #2 * t7045 - t14706) * t108 / 0.2E1 + (-t10982 * t13816 * t7549 + t
     #14706) * t108 / 0.2E1 + t14559 / 0.2E1 + t9592 + t14628 + t14610 /
     # 0.2E1 + t9593 + (t13892 * t14722 * t8468 - t14726) * t177 / 0.2E1
     # + (-t13903 * t14735 * t8616 + t14726) * t177 / 0.2E1 + (t7933 * (
     #t14336 / 0.2E1 + t6602 / 0.2E1) - t14746) * t177 / 0.2E1 + (t14746
     # - t8074 * (t14358 / 0.2E1 + t6636 / 0.2E1)) * t177 / 0.2E1 + (t13
     #941 * t6917 - t13950 * t6919) * t177
        t14767 = t14664 * ((t13744 * t14761 - t14665) * t124 / 0.2E1 + t
     #14668 / 0.2E1)
        t14771 = t7018 * t14674 / 0.8E1
        t14773 = t6450 * t14658 / 0.4E1
        t14775 = t7029 * t14661 / 0.4E1
        t14777 = t1945 * t14767 / 0.8E1
        t14782 = t3598 ** 2
        t14783 = t3607 ** 2
        t14784 = t3614 ** 2
        t14793 = u(t73,t13770,k,n)
        t14812 = rx(t5,t13770,k,0,0)
        t14813 = rx(t5,t13770,k,1,1)
        t14815 = rx(t5,t13770,k,2,2)
        t14817 = rx(t5,t13770,k,1,2)
        t14819 = rx(t5,t13770,k,2,1)
        t14821 = rx(t5,t13770,k,0,1)
        t14822 = rx(t5,t13770,k,1,0)
        t14826 = rx(t5,t13770,k,2,0)
        t14828 = rx(t5,t13770,k,0,2)
        t14834 = 0.1E1 / (t14812 * t14813 * t14815 - t14812 * t14817 * t
     #14819 - t14813 * t14826 * t14828 - t14815 * t14821 * t14822 + t148
     #17 * t14821 * t14826 + t14819 * t14822 * t14828)
        t14835 = t4 * t14834
        t14849 = t14822 ** 2
        t14850 = t14813 ** 2
        t14851 = t14817 ** 2
        t14864 = u(t5,t13770,t174,n)
        t14867 = u(t5,t13770,t179,n)
        t14884 = t13142 * t1084
        t14904 = t1151 * t13775
        t14917 = t5721 ** 2
        t14918 = t5714 ** 2
        t14919 = t5710 ** 2
        t14922 = t1062 ** 2
        t14923 = t1055 ** 2
        t14924 = t1051 ** 2
        t14926 = t1070 * (t14922 + t14923 + t14924)
        t14931 = t5901 ** 2
        t14932 = t5894 ** 2
        t14933 = t5890 ** 2
        t14261 = (t5707 * t5721 + t5710 * t5723 + t5714 * t5716) * t5730
        t14266 = (t5887 * t5901 + t5890 * t5903 + t5894 * t5896) * t5910
        t14271 = ((t14864 - t1277) * t124 / 0.2E1 + t1815 / 0.2E1) * t57
     #30
        t14276 = ((t14867 - t1280) * t124 / 0.2E1 + t1841 / 0.2E1) * t59
     #10
        t14942 = (t4 * (t3620 * (t14782 + t14783 + t14784) / 0.2E1 + t13
     #749 / 0.2E1) * t1079 - t13758) * t108 + (t3548 * ((t14793 - t1076)
     # * t124 / 0.2E1 + t1897 / 0.2E1) - t13777) * t108 / 0.2E1 + t13787
     # + (t3621 * (t3598 * t3612 + t3601 * t3614 + t3605 * t3607) * t365
     #7 - t13803) * t108 / 0.2E1 + t13812 + (t14835 * (t14812 * t14822 +
     # t14813 * t14821 + t14817 * t14828) * ((t14793 - t13771) * t108 / 
     #0.2E1 + t13851 / 0.2E1) - t1086) * t124 / 0.2E1 + t4139 + (t4 * (t
     #14834 * (t14849 + t14850 + t14851) / 0.2E1 + t1167 / 0.2E1) * t137
     #73 - t1715) * t124 + (t14835 * (t14813 * t14819 + t14815 * t14817 
     #+ t14822 * t14826) * ((t14864 - t13771) * t177 / 0.2E1 + (t13771 -
     # t14867) * t177 / 0.2E1) - t1286) * t124 / 0.2E1 + t4140 + (t14261
     # * t5740 - t14884) * t177 / 0.2E1 + (-t14266 * t5920 + t14884) * t
     #177 / 0.2E1 + (t14271 * t5760 - t14904) * t177 / 0.2E1 + (-t14276 
     #* t5940 + t14904) * t177 / 0.2E1 + (t4 * (t5729 * (t14917 + t14918
     # + t14919) / 0.2E1 + t14926 / 0.2E1) * t1279 - t4 * (t14926 / 0.2E
     #1 + t5909 * (t14931 + t14932 + t14933) / 0.2E1) * t1282) * t177
        t14943 = t14942 * t1069
        t14950 = t13954 * t2824
        t14952 = (t14950 - t4590) * t124
        t14954 = t14952 / 0.2E1 + t4592 / 0.2E1
        t14956 = t604 * t14954
        t14960 = t11414 ** 2
        t14961 = t11423 ** 2
        t14962 = t11430 ** 2
        t14971 = u(t2386,t13770,k,n)
        t14990 = rx(t507,t13770,k,0,0)
        t14991 = rx(t507,t13770,k,1,1)
        t14993 = rx(t507,t13770,k,2,2)
        t14995 = rx(t507,t13770,k,1,2)
        t14997 = rx(t507,t13770,k,2,1)
        t14999 = rx(t507,t13770,k,0,1)
        t15000 = rx(t507,t13770,k,1,0)
        t15004 = rx(t507,t13770,k,2,0)
        t15006 = rx(t507,t13770,k,0,2)
        t15012 = 0.1E1 / (t14990 * t14991 * t14993 - t14990 * t14995 * t
     #14997 - t14991 * t15004 * t15006 - t14993 * t14999 * t15000 + t149
     #95 * t14999 * t15004 + t14997 * t15000 * t15006)
        t15013 = t4 * t15012
        t15027 = t15000 ** 2
        t15028 = t14991 ** 2
        t15029 = t14995 ** 2
        t15042 = u(t507,t13770,t174,n)
        t15045 = u(t507,t13770,t179,n)
        t15058 = t12333 * t12347 + t12336 * t12349 + t12340 * t12342
        t15062 = t13155 * t7557
        t15069 = t12481 * t12495 + t12484 * t12497 + t12488 * t12490
        t15078 = (t15042 - t7578) * t124 / 0.2E1 + t7672 / 0.2E1
        t15082 = t7226 * t13792
        t15089 = (t15045 - t7581) * t124 / 0.2E1 + t7687 / 0.2E1
        t15095 = t12347 ** 2
        t15096 = t12340 ** 2
        t15097 = t12336 ** 2
        t15100 = t7540 ** 2
        t15101 = t7533 ** 2
        t15102 = t7529 ** 2
        t15104 = t7548 * (t15100 + t15101 + t15102)
        t15109 = t12495 ** 2
        t15110 = t12488 ** 2
        t15111 = t12484 ** 2
        t15120 = (t13767 - t4 * (t13763 / 0.2E1 + t11436 * (t14960 + t14
     #961 + t14962) / 0.2E1) * t7555) * t108 + t13797 + (t13794 - t10972
     # * ((t14971 - t7499) * t124 / 0.2E1 + t7501 / 0.2E1)) * t108 / 0.2
     #E1 + t13821 + (t13818 - t11437 * (t11414 * t11428 + t11417 * t1143
     #0 + t11421 * t11423) * t11473) * t108 / 0.2E1 + (t15013 * (t14990 
     #* t15000 + t14991 * t14999 + t14995 * t15006) * (t13853 / 0.2E1 + 
     #(t13788 - t14971) * t108 / 0.2E1) - t7559) * t124 / 0.2E1 + t7562 
     #+ (t4 * (t15012 * (t15027 + t15028 + t15029) / 0.2E1 + t7567 / 0.2
     #E1) * t13790 - t7571) * t124 + (t15013 * (t14991 * t14997 + t14993
     # * t14995 + t15000 * t15004) * ((t15042 - t13788) * t177 / 0.2E1 +
     # (t13788 - t15045) * t177 / 0.2E1) - t7587) * t124 / 0.2E1 + t7590
     # + (t12356 * t12364 * t15058 - t15062) * t177 / 0.2E1 + (-t12504 *
     # t12512 * t15069 + t15062) * t177 / 0.2E1 + (t11760 * t15078 - t15
     #082) * t177 / 0.2E1 + (-t11911 * t15089 + t15082) * t177 / 0.2E1 +
     # (t4 * (t12355 * (t15095 + t15096 + t15097) / 0.2E1 + t15104 / 0.2
     #E1) * t7580 - t4 * (t15104 / 0.2E1 + t12503 * (t15109 + t15110 + t
     #15111) / 0.2E1) * t7583) * t177
        t15121 = t15120 * t7547
        t15134 = t3948 * t8721
        t15157 = t5707 ** 2
        t15158 = t5716 ** 2
        t15159 = t5723 ** 2
        t15162 = t8445 ** 2
        t15163 = t8454 ** 2
        t15164 = t8461 ** 2
        t15166 = t8467 * (t15162 + t15163 + t15164)
        t15171 = t12333 ** 2
        t15172 = t12342 ** 2
        t15173 = t12349 ** 2
        t15185 = t7914 * t13912
        t15197 = t13210 * t8501
        t15206 = rx(i,t13770,t174,0,0)
        t15207 = rx(i,t13770,t174,1,1)
        t15209 = rx(i,t13770,t174,2,2)
        t15211 = rx(i,t13770,t174,1,2)
        t15213 = rx(i,t13770,t174,2,1)
        t15215 = rx(i,t13770,t174,0,1)
        t15216 = rx(i,t13770,t174,1,0)
        t15220 = rx(i,t13770,t174,2,0)
        t15222 = rx(i,t13770,t174,0,2)
        t15228 = 0.1E1 / (t15206 * t15207 * t15209 - t15206 * t15211 * t
     #15213 - t15207 * t15220 * t15222 - t15209 * t15215 * t15216 + t152
     #11 * t15215 * t15220 + t15213 * t15216 * t15222)
        t15229 = t4 * t15228
        t15245 = t15216 ** 2
        t15246 = t15207 ** 2
        t15247 = t15211 ** 2
        t15260 = u(i,t13770,t1364,n)
        t15270 = rx(i,t1047,t1364,0,0)
        t15271 = rx(i,t1047,t1364,1,1)
        t15273 = rx(i,t1047,t1364,2,2)
        t15275 = rx(i,t1047,t1364,1,2)
        t15277 = rx(i,t1047,t1364,2,1)
        t15279 = rx(i,t1047,t1364,0,1)
        t15280 = rx(i,t1047,t1364,1,0)
        t15284 = rx(i,t1047,t1364,2,0)
        t15286 = rx(i,t1047,t1364,0,2)
        t15292 = 0.1E1 / (t15270 * t15271 * t15273 - t15270 * t15275 * t
     #15277 - t15271 * t15284 * t15286 - t15273 * t15279 * t15280 + t152
     #75 * t15279 * t15284 + t15277 * t15280 * t15286)
        t15293 = t4 * t15292
        t15303 = (t5761 - t8497) * t108 / 0.2E1 + (t8497 - t12385) * t10
     #8 / 0.2E1
        t15322 = t15284 ** 2
        t15323 = t15277 ** 2
        t15324 = t15273 ** 2
        t14571 = t15293 * (t15271 * t15277 + t15273 * t15275 + t15280 * 
     #t15284)
        t15333 = (t4 * (t5729 * (t15157 + t15158 + t15159) / 0.2E1 + t15
     #166 / 0.2E1) * t5738 - t4 * (t15166 / 0.2E1 + t12355 * (t15171 + t
     #15172 + t15173) / 0.2E1) * t8474) * t108 + (t14271 * t5734 - t1518
     #5) * t108 / 0.2E1 + (-t11738 * t15078 + t15185) * t108 / 0.2E1 + (
     #t14261 * t5765 - t15197) * t108 / 0.2E1 + (-t12356 * t12389 * t150
     #58 + t15197) * t108 / 0.2E1 + (t15229 * (t15206 * t15216 + t15207 
     #* t15215 + t15211 * t15222) * ((t14864 - t13876) * t108 / 0.2E1 + 
     #(t13876 - t15042) * t108 / 0.2E1) - t8478) * t124 / 0.2E1 + t8481 
     #+ (t4 * (t15228 * (t15245 + t15246 + t15247) / 0.2E1 + t8486 / 0.2
     #E1) * t13910 - t8490) * t124 + (t15229 * (t15207 * t15213 + t15209
     # * t15211 + t15216 * t15220) * ((t15260 - t13876) * t177 / 0.2E1 +
     # t13878 / 0.2E1) - t8503) * t124 / 0.2E1 + t8506 + (t15293 * (t152
     #70 * t15284 + t15273 * t15286 + t15277 * t15279) * t15303 - t13894
     #) * t177 / 0.2E1 + t13899 + (t14571 * ((t15260 - t8497) * t124 / 0
     #.2E1 + t8549 / 0.2E1) - t13914) * t177 / 0.2E1 + t13919 + (t4 * (t
     #15292 * (t15322 + t15323 + t15324) / 0.2E1 + t13933 / 0.2E1) * t84
     #99 - t13942) * t177
        t15334 = t15333 * t8466
        t15337 = t5887 ** 2
        t15338 = t5896 ** 2
        t15339 = t5903 ** 2
        t15342 = t8593 ** 2
        t15343 = t8602 ** 2
        t15344 = t8609 ** 2
        t15346 = t8615 * (t15342 + t15343 + t15344)
        t15351 = t12481 ** 2
        t15352 = t12490 ** 2
        t15353 = t12497 ** 2
        t15365 = t8056 * t13923
        t15377 = t13218 * t8649
        t15386 = rx(i,t13770,t179,0,0)
        t15387 = rx(i,t13770,t179,1,1)
        t15389 = rx(i,t13770,t179,2,2)
        t15391 = rx(i,t13770,t179,1,2)
        t15393 = rx(i,t13770,t179,2,1)
        t15395 = rx(i,t13770,t179,0,1)
        t15396 = rx(i,t13770,t179,1,0)
        t15400 = rx(i,t13770,t179,2,0)
        t15402 = rx(i,t13770,t179,0,2)
        t15408 = 0.1E1 / (t15386 * t15387 * t15389 - t15386 * t15391 * t
     #15393 - t15387 * t15400 * t15402 - t15389 * t15395 * t15396 + t153
     #91 * t15395 * t15400 + t15393 * t15396 * t15402)
        t15409 = t4 * t15408
        t15425 = t15396 ** 2
        t15426 = t15387 ** 2
        t15427 = t15391 ** 2
        t15440 = u(i,t13770,t1375,n)
        t15450 = rx(i,t1047,t1375,0,0)
        t15451 = rx(i,t1047,t1375,1,1)
        t15453 = rx(i,t1047,t1375,2,2)
        t15455 = rx(i,t1047,t1375,1,2)
        t15457 = rx(i,t1047,t1375,2,1)
        t15459 = rx(i,t1047,t1375,0,1)
        t15460 = rx(i,t1047,t1375,1,0)
        t15464 = rx(i,t1047,t1375,2,0)
        t15466 = rx(i,t1047,t1375,0,2)
        t15472 = 0.1E1 / (t15450 * t15451 * t15453 - t15450 * t15455 * t
     #15457 - t15451 * t15464 * t15466 - t15453 * t15459 * t15460 + t154
     #55 * t15459 * t15464 + t15457 * t15460 * t15466)
        t15473 = t4 * t15472
        t15483 = (t5941 - t8645) * t108 / 0.2E1 + (t8645 - t12533) * t10
     #8 / 0.2E1
        t15502 = t15464 ** 2
        t15503 = t15457 ** 2
        t15504 = t15453 ** 2
        t14720 = t15473 * (t15451 * t15457 + t15453 * t15455 + t15460 * 
     #t15464)
        t15513 = (t4 * (t5909 * (t15337 + t15338 + t15339) / 0.2E1 + t15
     #346 / 0.2E1) * t5918 - t4 * (t15346 / 0.2E1 + t12503 * (t15351 + t
     #15352 + t15353) / 0.2E1) * t8622) * t108 + (t14276 * t5914 - t1536
     #5) * t108 / 0.2E1 + (-t11889 * t15089 + t15365) * t108 / 0.2E1 + (
     #t14266 * t5945 - t15377) * t108 / 0.2E1 + (-t12504 * t12537 * t150
     #69 + t15377) * t108 / 0.2E1 + (t15409 * (t15386 * t15396 + t15387 
     #* t15395 + t15391 * t15402) * ((t14867 - t13879) * t108 / 0.2E1 + 
     #(t13879 - t15045) * t108 / 0.2E1) - t8626) * t124 / 0.2E1 + t8629 
     #+ (t4 * (t15408 * (t15425 + t15426 + t15427) / 0.2E1 + t8634 / 0.2
     #E1) * t13921 - t8638) * t124 + (t15409 * (t15387 * t15393 + t15389
     # * t15391 + t15396 * t15400) * (t13881 / 0.2E1 + (t13879 - t15440)
     # * t177 / 0.2E1) - t8651) * t124 / 0.2E1 + t8654 + t13908 + (t1390
     #5 - t15473 * (t15450 * t15464 + t15453 * t15466 + t15457 * t15459)
     # * t15483) * t177 / 0.2E1 + t13928 + (t13925 - t14720 * ((t15440 -
     # t8645) * t124 / 0.2E1 + t8697 / 0.2E1)) * t177 / 0.2E1 + (t13951 
     #- t4 * (t13947 / 0.2E1 + t15472 * (t15502 + t15503 + t15504) / 0.2
     #E1) * t8647) * t177
        t15514 = t15513 * t8614
        t15533 = t3948 * t8401
        t15555 = t683 * t14954
        t14814 = ((t5835 - t8569) * t108 / 0.2E1 + (t8569 - t12457) * t1
     #08 / 0.2E1) * t4487
        t14824 = ((t6015 - t8717) * t108 / 0.2E1 + (t8717 - t12605) * t1
     #08 / 0.2E1) * t4526
        t15572 = (t4119 * t5631 - t4442 * t8399) * t108 + (t251 * ((t149
     #43 - t4267) * t124 / 0.2E1 + t4269 / 0.2E1) - t14956) * t108 / 0.2
     #E1 + (t14956 - t2329 * ((t15121 - t7721) * t124 / 0.2E1 + t7723 / 
     #0.2E1)) * t108 / 0.2E1 + (t3344 * t6019 - t15134) * t108 / 0.2E1 +
     # (-t12292 * t4456 + t15134) * t108 / 0.2E1 + (t3010 * ((t14943 - t
     #14950) * t108 / 0.2E1 + (t14950 - t15121) * t108 / 0.2E1) - t8403)
     # * t124 / 0.2E1 + t8410 + (t14952 * t2833 - t8420) * t124 + (t2692
     # * ((t15334 - t14950) * t177 / 0.2E1 + (t14950 - t15514) * t177 / 
     #0.2E1) - t8723) * t124 / 0.2E1 + t8728 + (t14814 * t4491 - t15533)
     # * t177 / 0.2E1 + (-t14824 * t4530 + t15533) * t177 / 0.2E1 + (t42
     #76 * ((t15334 - t8569) * t124 / 0.2E1 + t9053 / 0.2E1) - t15555) *
     # t177 / 0.2E1 + (t15555 - t4289 * ((t15514 - t8717) * t124 / 0.2E1
     # + t9066 / 0.2E1)) * t177 / 0.2E1 + (t4576 * t8571 - t4585 * t8719
     #) * t177
        t15573 = t13696 * t15572
        t15575 = t7451 * t15573 / 0.12E2
        t15579 = t864 * t14297 / 0.24E2
        t15580 = t14295 + t2378 * t14297 / 0.24E2 - t14301 + t7029 * t14
     #658 / 0.4E1 + t14663 + t14676 + t14678 + t2751 * t14290 / 0.2E1 - 
     #t7018 * t14767 / 0.8E1 - t14771 - t14773 - t14775 + t14777 - t1557
     #5 + t3386 * t15573 / 0.12E2 - t15579
        t15582 = (t14293 + t15580) * t65
        t15588 = t3257 * (t1218 - dy * t2793 / 0.24E2)
        t15589 = -t13688 - t13693 + t13717 + t13722 - t13730 + t13737 - 
     #t13962 + t14292 + t15588 - t14295 - t14663 - t14676
        t15593 = cc * t602 * t13695 * t157
        t15594 = t15593 / 0.2E1
        t15596 = t9235 * t13684 * t2
        t15597 = t15596 / 0.2E1
        t15599 = (-t15596 + t15593) * t124
        t15602 = cc * t643 * t13701 * t160
        t15604 = (t15596 - t15602) * t124
        t15606 = (t15599 - t15604) * t124
        t15609 = cc * t2825 * t13743 * t6616
        t15611 = (-t15593 + t15609) * t124
        t15613 = (t15611 - t15599) * t124
        t15615 = (t15613 - t15606) * t124
        t15617 = sqrt(t2865)
        t15619 = cc * t2861 * t15617 * t6622
        t15621 = (-t15619 + t15602) * t124
        t15623 = (t15604 - t15621) * t124
        t15625 = (t15606 - t15623) * t124
        t15631 = t1046 * (t15606 - dy * (t15615 - t15625) / 0.12E2) / 0.
     #24E2
        t15632 = t15599 / 0.2E1
        t15633 = t15604 / 0.2E1
        t15640 = dy * (t15632 + t15633 - t1046 * (t15615 / 0.2E1 + t1562
     #5 / 0.2E1) / 0.6E1) / 0.4E1
        t15642 = sqrt(t13864)
        t15650 = (((cc * t13844 * t14344 * t15642 - t15609) * t124 - t15
     #611) * t124 - t15613) * t124
        t15656 = t1046 * (t15613 - dy * (t15650 - t15615) / 0.12E2) / 0.
     #24E2
        t15664 = dy * (t15611 / 0.2E1 + t15632 - t1046 * (t15650 / 0.2E1
     # + t15615 / 0.2E1) / 0.6E1) / 0.4E1
        t15666 = dy * t2837 / 0.24E2
        t15667 = -t15582 * t863 - t14678 + t14773 - t14777 + t15575 + t1
     #5579 + t15594 - t15597 - t15631 - t15640 + t15656 - t15664 - t1566
     #6
        t15671 = t602 * t686
        t15673 = t56 * t700
        t15674 = t15673 / 0.2E1
        t15678 = t643 * t709
        t15686 = t4 * (t15671 / 0.2E1 + t15674 - dy * ((t2825 * t2884 - 
     #t15671) * t124 / 0.2E1 - (t15673 - t15678) * t124 / 0.2E1) / 0.8E1
     #)
        t15691 = t1363 * (t14442 / 0.2E1 + t14447 / 0.2E1)
        t15698 = (t6917 - t6919) * t177
        t15709 = t689 / 0.2E1
        t15710 = t692 / 0.2E1
        t15711 = t15691 / 0.6E1
        t15714 = t712 / 0.2E1
        t15715 = t715 / 0.2E1
        t15719 = (t712 - t715) * t177
        t15721 = ((t6714 - t712) * t177 - t15719) * t177
        t15725 = (t15719 - (t715 - t6720) * t177) * t177
        t15728 = t1363 * (t15721 / 0.2E1 + t15725 / 0.2E1)
        t15729 = t15728 / 0.6E1
        t15736 = t689 / 0.4E1 + t692 / 0.4E1 - t15691 / 0.12E2 + t9815 +
     # t9816 - t9820 - dy * ((t6917 / 0.2E1 + t6919 / 0.2E1 - t1363 * ((
     #(t14637 - t6917) * t177 - t15698) * t177 / 0.2E1 + (t15698 - (t691
     #9 - t14642) * t177) * t177 / 0.2E1) / 0.6E1 - t15709 - t15710 + t1
     #5711) * t124 / 0.2E1 - (t9842 + t9843 - t9844 - t15714 - t15715 + 
     #t15729) * t124 / 0.2E1) / 0.8E1
        t15741 = t4 * (t15671 / 0.2E1 + t15673 / 0.2E1)
        t15743 = t8571 / 0.4E1 + t8719 / 0.4E1 + t5526 / 0.4E1 + t5620 /
     # 0.4E1
        t15754 = t5148 * t9615
        t15766 = t4219 * t10054
        t15778 = (t14722 * t8468 * t8472 - t10038) * t124
        t15782 = (t6602 * t8489 - t10049) * t124
        t15788 = (t7933 * (t14637 / 0.2E1 + t6917 / 0.2E1) - t10056) * t
     #124
        t15792 = (t5679 * t9448 - t8431 * t9595) * t108 + (t5039 * t9470
     # - t15754) * t108 / 0.2E1 + (-t12499 * t8062 + t15754) * t108 / 0.
     #2E1 + (t3970 * t9929 - t15766) * t108 / 0.2E1 + (-t12743 * t7618 +
     # t15766) * t108 / 0.2E1 + t15778 / 0.2E1 + t10043 + t15782 + t1578
     #8 / 0.2E1 + t10061 + t14381 / 0.2E1 + t9604 + t14310 / 0.2E1 + t96
     #22 + t14453
        t15793 = t15792 * t4485
        t15803 = t5197 * t9624
        t15815 = t4261 * t10107
        t15827 = (t14735 * t8616 * t8620 - t10091) * t124
        t15831 = (t6636 * t8637 - t10102) * t124
        t15837 = (t8074 * (t6919 / 0.2E1 + t14642 / 0.2E1) - t10109) * t
     #124
        t15841 = (t5859 * t9461 - t8579 * t9606) * t108 + (t5090 * t9479
     # - t15803) * t108 / 0.2E1 + (-t12506 * t8260 + t15803) * t108 / 0.
     #2E1 + (t4207 * t9431 - t15815) * t108 / 0.2E1 + (-t12794 * t7657 +
     # t15815) * t108 / 0.2E1 + t15827 / 0.2E1 + t10096 + t15831 + t1583
     #7 / 0.2E1 + t10114 + t9613 + t14397 / 0.2E1 + t9629 + t14325 / 0.2
     #E1 + t14458
        t15842 = t15841 * t4524
        t15846 = (t15793 - t9635) * t177 / 0.4E1 + (t9635 - t15842) * t1
     #77 / 0.4E1 + t10074 / 0.4E1 + t10127 / 0.4E1
        t15852 = dy * (t6925 / 0.2E1 - t721 / 0.2E1)
        t15856 = t15686 * t9108 * t15736
        t15859 = t15741 * t9711 * t15743 / 0.2E1
        t15862 = t15741 * t9715 * t15846 / 0.6E1
        t15864 = t9108 * t15852 / 0.24E2
        t15866 = (t15686 * t2746 * t15736 + t15741 * t9402 * t15743 / 0.
     #2E1 + t15741 * t9408 * t15846 / 0.6E1 - t2746 * t15852 / 0.24E2 - 
     #t15856 - t15859 - t15862 + t15864) * t65
        t15873 = t1363 * (t14065 / 0.2E1 + t14070 / 0.2E1)
        t15880 = (t2887 - t2890) * t177
        t15891 = t2301 / 0.2E1
        t15892 = t2304 / 0.2E1
        t15893 = t15873 / 0.6E1
        t15896 = t2316 / 0.2E1
        t15897 = t2319 / 0.2E1
        t15901 = (t2316 - t2319) * t177
        t15903 = ((t2959 - t2316) * t177 - t15901) * t177
        t15907 = (t15901 - (t2319 - t2965) * t177) * t177
        t15910 = t1363 * (t15903 / 0.2E1 + t15907 / 0.2E1)
        t15911 = t15910 / 0.6E1
        t15919 = t15686 * (t2301 / 0.4E1 + t2304 / 0.4E1 - t15873 / 0.12
     #E2 + t10158 + t10159 - t10163 - dy * ((t2887 / 0.2E1 + t2890 / 0.2
     #E1 - t1363 * (((t8499 - t2887) * t177 - t15880) * t177 / 0.2E1 + (
     #t15880 - (t2890 - t8647) * t177) * t177 / 0.2E1) / 0.6E1 - t15891 
     #- t15892 + t15893) * t124 / 0.2E1 - (t10185 + t10186 - t10187 - t1
     #5896 - t15897 + t15911) * t124 / 0.2E1) / 0.8E1)
        t15923 = dy * (t2896 / 0.2E1 - t2325 / 0.2E1) / 0.24E2
        t15939 = t4 * (t9325 + t13488 / 0.2E1 - dy * ((t13483 - t9324) *
     # t124 / 0.2E1 - (-t2861 * t3229 + t13488) * t124 / 0.2E1) / 0.8E1)
        t15950 = (t6758 - t6761) * t108
        t15967 = t13513 + t13514 - t13518 + t295 / 0.4E1 + t650 / 0.4E1 
     #- t13557 / 0.12E2 - dy * ((t13535 + t13536 - t13537 - t13540 - t13
     #541 + t13542) * t124 / 0.2E1 - (t13543 + t13544 - t13558 - t6758 /
     # 0.2E1 - t6761 / 0.2E1 + t868 * (((t7284 - t6758) * t108 - t15950)
     # * t108 / 0.2E1 + (t15950 - (t6761 - t11149) * t108) * t108 / 0.2E
     #1) / 0.6E1) * t124 / 0.2E1) / 0.8E1
        t15972 = t4 * (t9324 / 0.2E1 + t13488 / 0.2E1)
        t15974 = t3488 / 0.4E1 + t7456 / 0.4E1 + t5646 / 0.4E1 + t8412 /
     # 0.4E1
        t15983 = t13580 / 0.4E1 + t13581 / 0.4E1 + (t9574 - t9696) * t10
     #8 / 0.4E1 + (t9696 - t13199) * t108 / 0.4E1
        t15989 = dy * (t619 / 0.2E1 - t6767 / 0.2E1)
        t15993 = t15939 * t9108 * t15967
        t15996 = t15972 * t9711 * t15974 / 0.2E1
        t15999 = t15972 * t9715 * t15983 / 0.6E1
        t16001 = t9108 * t15989 / 0.24E2
        t16003 = (t15939 * t2746 * t15967 + t15972 * t9402 * t15974 / 0.
     #2E1 + t15972 * t9408 * t15983 / 0.6E1 - t2746 * t15989 / 0.24E2 - 
     #t15993 - t15996 - t15999 + t16001) * t65
        t16016 = (t1146 - t3231) * t108
        t16034 = t15939 * (t13622 + t13623 - t13627 + t1024 / 0.4E1 + t1
     #032 / 0.4E1 - t13666 / 0.12E2 - dy * ((t13644 + t13645 - t13646 - 
     #t13649 - t13650 + t13651) * t124 / 0.2E1 - (t13652 + t13653 - t136
     #67 - t1146 / 0.2E1 - t3231 / 0.2E1 + t868 * (((t1143 - t1146) * t1
     #08 - t16016) * t108 / 0.2E1 + (t16016 - (t3231 - t7819) * t108) * 
     #t108 / 0.2E1) / 0.6E1) * t124 / 0.2E1) / 0.8E1)
        t16038 = dy * (t2286 / 0.2E1 - t3237 / 0.2E1) / 0.24E2
        t16045 = t72 * t9697 * t124
        t16047 = t679 * t7449 * t16045 / 0.6E1
        t16050 = t162 - dy * t6857 / 0.24E2
        t16054 = t3269 * t9108 * t16050
        t16057 = t71 * t4749 * t124
        t16063 = (t4295 - t4618) * t108
        t16075 = t4274 / 0.2E1
        t16085 = t4 * (t3841 / 0.2E1 + t16075 - dx * ((t3832 - t3841) * 
     #t108 / 0.2E1 - (t4274 - t4597) * t108 / 0.2E1) / 0.8E1)
        t16097 = t4 * (t16075 + t4597 / 0.2E1 - dx * ((t3841 - t4274) * 
     #t108 / 0.2E1 - (t4597 - t7751) * t108 / 0.2E1) / 0.8E1)
        t16106 = t4056 * t2761
        t16121 = (t4286 - t4609) * t108
        t16132 = j - 3
        t16133 = rx(i,t16132,k,0,0)
        t16134 = rx(i,t16132,k,1,1)
        t16136 = rx(i,t16132,k,2,2)
        t16138 = rx(i,t16132,k,1,2)
        t16140 = rx(i,t16132,k,2,1)
        t16142 = rx(i,t16132,k,0,1)
        t16143 = rx(i,t16132,k,1,0)
        t16147 = rx(i,t16132,k,2,0)
        t16149 = rx(i,t16132,k,0,2)
        t16155 = 0.1E1 / (t16133 * t16134 * t16136 - t16133 * t16138 * t
     #16140 - t16134 * t16147 * t16149 - t16136 * t16142 * t16143 + t161
     #38 * t16142 * t16147 + t16140 * t16143 * t16149)
        t16156 = t4 * t16155
        t16161 = u(t5,t16132,k,n)
        t16162 = u(i,t16132,k,n)
        t16164 = (t16161 - t16162) * t108
        t16165 = u(t507,t16132,k,n)
        t16167 = (t16162 - t16165) * t108
        t15364 = t16156 * (t16133 * t16143 + t16134 * t16142 + t16138 * 
     #t16149)
        t16173 = (t3235 - t15364 * (t16164 / 0.2E1 + t16167 / 0.2E1)) * 
     #t124
        t16183 = t16143 ** 2
        t16184 = t16134 ** 2
        t16185 = t16138 ** 2
        t16186 = t16183 + t16184 + t16185
        t16187 = t16155 * t16186
        t16195 = t4 * (t3259 + t2866 / 0.2E1 - dy * (t3251 / 0.2E1 - (t2
     #866 - t16187) * t124 / 0.2E1) / 0.8E1)
        t16199 = t4287 + t4296 + t4610 + t2326 + t4619 + t4620 + t2294 +
     # t4621 + t4660 - t868 * (((t3904 - t4295) * t108 - t16063) * t108 
     #/ 0.2E1 + (t16063 - (t4618 - t7788) * t108) * t108 / 0.2E1) / 0.6E
     #1 + (t1024 * t16085 - t1032 * t16097) * t108 - t1363 * ((t1784 * t
     #3674 - t16106) * t108 / 0.2E1 + (-t4327 * t9958 + t16106) * t108 /
     # 0.2E1) / 0.6E1 - t868 * (((t3872 - t4286) * t108 - t16121) * t108
     # / 0.2E1 + (t16121 - (t4609 - t7771) * t108) * t108 / 0.2E1) / 0.6
     #E1 - t1046 * (t3241 / 0.2E1 + (t3239 - (t3237 - t16173) * t124) * 
     #t124 / 0.2E1) / 0.6E1 + (-t16195 * t1921 + t3270) * t124
        t16228 = t4056 * t3087
        t16251 = u(i,t16132,t174,n)
        t16253 = (t16251 - t16162) * t177
        t16254 = u(i,t16132,t179,n)
        t16256 = (t16162 - t16254) * t177
        t15469 = t16156 * (t16134 * t16140 + t16136 * t16138 + t16143 * 
     #t16147)
        t16262 = (t2917 - t15469 * (t16253 / 0.2E1 + t16256 / 0.2E1)) * 
     #t124
        t16288 = (t1144 - t16162) * t124
        t16298 = t4 * (t2866 / 0.2E1 + t16187 / 0.2E1)
        t16301 = (-t16288 * t16298 + t2870) * t124
        t16312 = (t4709 - t4720) * t177
        t16324 = (t2908 - t16251) * t124
        t15487 = (t1924 - (t1220 / 0.2E1 - t16288 / 0.2E1) * t124) * t12
     #4
        t16338 = t706 * t15487
        t16342 = (t2911 - t16254) * t124
        t16359 = (t4659 - t4696) * t177
        t16384 = t4731 / 0.2E1
        t16394 = t4 * (t4726 / 0.2E1 + t16384 - dz * ((t8866 - t4726) * 
     #t177 / 0.2E1 - (t4731 - t4740) * t177 / 0.2E1) / 0.8E1)
        t16406 = t4 * (t16384 + t4740 / 0.2E1 - dz * ((t4726 - t4731) * 
     #t177 / 0.2E1 - (t4740 - t9014) * t177 / 0.2E1) / 0.8E1)
        t16411 = (t1141 - t16161) * t124
        t16421 = t641 * t15487
        t16425 = (t3172 - t16165) * t124
        t15535 = t108 * ((t4002 / 0.2E1 - t4651 / 0.2E1) * t108 - (t4328
     # / 0.2E1 - t7884 / 0.2E1) * t108)
        t15539 = t108 * ((t4043 / 0.2E1 - t4690 / 0.2E1) * t108 - (t4367
     # / 0.2E1 - t7923 / 0.2E1) * t108)
        t16452 = -t868 * (t3293 / 0.2E1 + (t3291 - t3031 * ((t1143 / 0.2
     #E1 - t3231 / 0.2E1) * t108 - (t1146 / 0.2E1 - t7819 / 0.2E1) * t10
     #8) * t108) * t124 / 0.2E1) / 0.6E1 - t868 * ((t15535 * t4363 - t16
     #228) * t177 / 0.2E1 + (-t15539 * t4394 + t16228) * t177 / 0.2E1) /
     # 0.6E1 - t1046 * (t2923 / 0.2E1 + (t2921 - (t2919 - t16262) * t124
     #) * t124 / 0.2E1) / 0.6E1 - t1363 * (t2974 / 0.2E1 + (t2972 - t272
     #0 * ((t8804 / 0.2E1 - t2913 / 0.2E1) * t177 - (t2910 / 0.2E1 - t89
     #52 / 0.2E1) * t177) * t177) * t124 / 0.2E1) / 0.6E1 - t1046 * ((t2
     #800 - t2869 * (t2797 - (t1921 - t16288) * t124) * t124) * t124 + (
     #t2874 - (t2872 - t16301) * t124) * t124) / 0.24E2 - t1363 * (((t88
     #60 - t4709) * t177 - t16312) * t177 / 0.2E1 + (t16312 - (t4720 - t
     #9008) * t177) * t177 / 0.2E1) / 0.6E1 - t1046 * ((t4410 * (t2988 -
     # (t2346 / 0.2E1 - t16324 / 0.2E1) * t124) * t124 - t16338) * t177 
     #/ 0.2E1 + (t16338 - t4426 * (t3007 - (t2359 / 0.2E1 - t16342 / 0.2
     #E1) * t124) * t124) * t177 / 0.2E1) / 0.6E1 - t1363 * (((t8847 - t
     #4659) * t177 - t16359) * t177 / 0.2E1 + (t16359 - (t4696 - t8995) 
     #* t177) * t177 / 0.2E1) / 0.6E1 - t1363 * ((t15903 * t4734 - t1590
     #7 * t4743) * t177 + ((t8872 - t4746) * t177 - (t4746 - t9020) * t1
     #77) * t177) / 0.24E2 + (t16394 * t2316 - t16406 * t2319) * t177 - 
     #t1046 * ((t292 * (t1833 - (t1198 / 0.2E1 - t16411 / 0.2E1) * t124)
     # * t124 - t16421) * t108 / 0.2E1 + (t16421 - t2377 * (t3177 - (t17
     #99 / 0.2E1 - t16425 / 0.2E1) * t124) * t124) * t108 / 0.2E1) / 0.6
     #E1 + t4697 + t4710 + t4721 - t868 * ((t13659 * t4277 - t13663 * t4
     #600) * t108 + ((t4280 - t4603) * t108 - (t4603 - t7757) * t108) * 
     #t108) / 0.24E2
        t16454 = t13702 * (t16199 + t16452)
        t16457 = ut(i,t16132,k,n)
        t16459 = (t6622 - t16457) * t124
        t16469 = (-t16298 * t16459 + t6867) * t124
        t16477 = ut(i,t1111,t1364,n)
        t16479 = (t16477 - t6606) * t177
        t16483 = ut(i,t1111,t1375,n)
        t16485 = (t6640 - t16483) * t177
        t16500 = (t6712 - t16477) * t124
        t16506 = (t8280 * (t6813 / 0.2E1 + t16500 / 0.2E1) - t9678) * t1
     #77
        t16510 = (t9682 - t9689) * t177
        t16514 = (t6718 - t16483) * t124
        t16520 = (t9687 - t8427 * (t6829 / 0.2E1 + t16514 / 0.2E1)) * t1
     #77
        t16533 = ut(i,t16132,t174,n)
        t16535 = (t6606 - t16533) * t124
        t15757 = (t6627 - (t162 / 0.2E1 - t16459 / 0.2E1) * t124) * t124
        t16549 = t706 * t15757
        t16552 = ut(i,t16132,t179,n)
        t16554 = (t6640 - t16552) * t124
        t16573 = (t7108 - t6712) * t108 / 0.2E1 + (t6712 - t10868) * t10
     #8 / 0.2E1
        t16577 = (t16573 * t8835 * t8839 - t9660) * t177
        t16581 = (t9664 - t9673) * t177
        t16589 = (t7126 - t6718) * t108 / 0.2E1 + (t6718 - t10874) * t10
     #8 / 0.2E1
        t16593 = (-t16589 * t8983 * t8987 + t9671) * t177
        t16602 = ut(t5,t16132,k,n)
        t16604 = (t6756 - t16602) * t124
        t16614 = t641 * t15757
        t16617 = ut(t507,t16132,k,n)
        t16619 = (t6759 - t16617) * t124
        t16669 = (t9525 - t9651) * t108
        t16680 = t657 + t9653 + t722 + t9654 - t1046 * ((t6859 - t2869 *
     # (t6856 - (t6624 - t16459) * t124) * t124) * t124 + (t6871 - (t686
     #9 - t16469) * t124) * t124) / 0.24E2 - t1363 * (t6729 / 0.2E1 + (t
     #6727 - t2720 * ((t16479 / 0.2E1 - t6935 / 0.2E1) * t177 - (t6933 /
     # 0.2E1 - t16485 / 0.2E1) * t177) * t177) * t124 / 0.2E1) / 0.6E1 -
     # t1363 * (((t16506 - t9682) * t177 - t16510) * t177 / 0.2E1 + (t16
     #510 - (t9689 - t16520) * t177) * t177 / 0.2E1) / 0.6E1 + (t16394 *
     # t712 - t16406 * t715) * t177 - t1046 * ((t4410 * (t6611 - (t806 /
     # 0.2E1 - t16535 / 0.2E1) * t124) * t124 - t16549) * t177 / 0.2E1 +
     # (t16549 - t4426 * (t6645 - (t823 / 0.2E1 - t16554 / 0.2E1) * t124
     #) * t124) * t177 / 0.2E1) / 0.6E1 - t1363 * (((t16577 - t9664) * t
     #177 - t16581) * t177 / 0.2E1 + (t16581 - (t9673 - t16593) * t177) 
     #* t177 / 0.2E1) / 0.6E1 - t1046 * ((t292 * (t6959 - (t144 / 0.2E1 
     #- t16604 / 0.2E1) * t124) * t124 - t16614) * t108 / 0.2E1 + (t1661
     #4 - t2377 * (t6978 - (t555 / 0.2E1 - t16619 / 0.2E1) * t124) * t12
     #4) * t108 / 0.2E1) / 0.6E1 - t868 * ((t13550 * t4277 - t13554 * t4
     #600) * t108 + ((t9496 - t9640) * t108 - (t9640 - t13143) * t108) *
     # t108) / 0.24E2 - t868 * (t6555 / 0.2E1 + (t6553 - t3031 * ((t7284
     # / 0.2E1 - t6761 / 0.2E1) * t108 - (t6758 / 0.2E1 - t11149 / 0.2E1
     #) * t108) * t108) * t124 / 0.2E1) / 0.6E1 + (t16085 * t295 - t1609
     #7 * t650) * t108 - t868 * (((t9520 - t9525) * t108 - t16669) * t10
     #8 / 0.2E1 + (t16669 - (t9651 - t13154) * t108) * t108 / 0.2E1) / 0
     #.6E1
        t16686 = t4056 * t6335
        t16701 = (t9513 - t9646) * t108
        t16721 = (t6765 - t15364 * ((t16602 - t16457) * t108 / 0.2E1 + (
     #t16457 - t16617) * t108 / 0.2E1)) * t124
        t16739 = (t6714 * t8869 - t9691) * t177
        t16744 = (-t6720 * t9017 + t9692) * t177
        t16764 = t4056 * t6233
        t16792 = (t6939 - t15469 * ((t16533 - t16457) * t177 / 0.2E1 + (
     #t16457 - t16552) * t177 / 0.2E1)) * t124
        t16052 = t108 * ((t9530 / 0.2E1 - t9656 / 0.2E1) * t108 - (t9532
     # / 0.2E1 - t13159 / 0.2E1) * t108)
        t16058 = t108 * ((t9543 / 0.2E1 - t9667 / 0.2E1) * t108 - (t9545
     # / 0.2E1 - t13170 / 0.2E1) * t108)
        t16801 = -t1363 * ((t3900 * t6843 * t7176 - t16686) * t108 / 0.2
     #E1 + (-t10344 * t4327 + t16686) * t108 / 0.2E1) / 0.6E1 - t868 * (
     #((t9506 - t9513) * t108 - t16701) * t108 / 0.2E1 + (t16701 - (t964
     #6 - t13149) * t108) * t108 / 0.2E1) / 0.6E1 - t1046 * (t6771 / 0.2
     #E1 + (t6769 - (t6767 - t16721) * t124) * t124 / 0.2E1) / 0.6E1 + (
     #-t16195 * t6624 + t6777) * t124 - t1363 * ((t15721 * t4734 - t1572
     #5 * t4743) * t177 + ((t16739 - t9694) * t177 - (t9694 - t16744) * 
     #t177) * t177) / 0.24E2 - t868 * ((t16052 * t4363 - t16764) * t177 
     #/ 0.2E1 + (-t16058 * t4394 + t16764) * t177 / 0.2E1) / 0.6E1 - t10
     #46 * (t6945 / 0.2E1 + (t6943 - (t6941 - t16792) * t124) * t124 / 0
     #.2E1) / 0.6E1 + t9690 + t9665 + t9674 + t9683 + t9652 + t9647 + t9
     #514 + t9526
        t16803 = t13702 * (t16680 + t16801)
        t16806 = -t13688 - t16047 + t13709 + t13712 - t13730 - t13742 + 
     #t3269 * t2746 * t16050 - t16054 + t679 * t68 * t16057 / 0.2E1 - t2
     #751 * t16454 / 0.2E1 - t14295 - t7029 * t16803 / 0.4E1 + t14301 - 
     #t14663 + t14676 + t14678
        t16808 = t865 * t16454 / 0.2E1
        t16811 = t679 * t1944 * t16057 / 0.2E1
        t16816 = t3906 ** 2
        t16817 = t3915 ** 2
        t16818 = t3922 ** 2
        t16821 = t1112 ** 2
        t16822 = t1121 ** 2
        t16823 = t1128 ** 2
        t16825 = t1134 * (t16821 + t16822 + t16823)
        t16830 = t2839 ** 2
        t16831 = t2848 ** 2
        t16832 = t2855 ** 2
        t16834 = t2861 * (t16830 + t16831 + t16832)
        t16837 = t4 * (t16825 / 0.2E1 + t16834 / 0.2E1)
        t16838 = t16837 * t1146
        t16841 = u(t73,t16132,k,n)
        t16849 = t1704 / 0.2E1 + t16411 / 0.2E1
        t16851 = t1045 * t16849
        t16856 = t1921 / 0.2E1 + t16288 / 0.2E1
        t16858 = t3031 * t16856
        t16861 = (t16851 - t16858) * t108 / 0.2E1
        t16102 = t1135 * (t1112 * t1126 + t1115 * t1128 + t1119 * t1121)
        t16873 = t16102 * t1306
        t16107 = t2903 * (t2839 * t2853 + t2842 * t2855 + t2846 * t2848)
        t16882 = t16107 * t2915
        t16885 = (t16873 - t16882) * t108 / 0.2E1
        t16886 = rx(t5,t16132,k,0,0)
        t16887 = rx(t5,t16132,k,1,1)
        t16889 = rx(t5,t16132,k,2,2)
        t16891 = rx(t5,t16132,k,1,2)
        t16893 = rx(t5,t16132,k,2,1)
        t16895 = rx(t5,t16132,k,0,1)
        t16896 = rx(t5,t16132,k,1,0)
        t16900 = rx(t5,t16132,k,2,0)
        t16902 = rx(t5,t16132,k,0,2)
        t16908 = 0.1E1 / (t16886 * t16887 * t16889 - t16886 * t16891 * t
     #16893 - t16887 * t16900 * t16902 - t16889 * t16895 * t16896 + t168
     #91 * t16895 * t16900 + t16893 * t16896 * t16902)
        t16909 = t4 * t16908
        t16923 = t16896 ** 2
        t16924 = t16887 ** 2
        t16925 = t16891 ** 2
        t16938 = u(t5,t16132,t174,n)
        t16941 = u(t5,t16132,t179,n)
        t16958 = t16102 * t1148
        t16978 = t1161 * t16849
        t16991 = t6090 ** 2
        t16992 = t6083 ** 2
        t16993 = t6079 ** 2
        t16996 = t1126 ** 2
        t16997 = t1119 ** 2
        t16998 = t1115 ** 2
        t17000 = t1134 * (t16996 + t16997 + t16998)
        t17005 = t6270 ** 2
        t17006 = t6263 ** 2
        t17007 = t6259 ** 2
        t16225 = (t6076 * t6090 + t6079 * t6092 + t6083 * t6085) * t6099
        t16231 = (t6256 * t6270 + t6259 * t6272 + t6263 * t6265) * t6279
        t16236 = (t1820 / 0.2E1 + (t1299 - t16938) * t124 / 0.2E1) * t60
     #99
        t16242 = (t1846 / 0.2E1 + (t1302 - t16941) * t124 / 0.2E1) * t62
     #79
        t17016 = (t4 * (t3928 * (t16816 + t16817 + t16818) / 0.2E1 + t16
     #825 / 0.2E1) * t1143 - t16838) * t108 + (t3849 * (t1902 / 0.2E1 + 
     #(t1140 - t16841) * t124 / 0.2E1) - t16851) * t108 / 0.2E1 + t16861
     # + (t3929 * (t3906 * t3920 + t3909 * t3922 + t3913 * t3915) * t396
     #5 - t16873) * t108 / 0.2E1 + t16885 + t4297 + (t1150 - t16909 * (t
     #16886 * t16896 + t16887 * t16895 + t16891 * t16902) * ((t16841 - t
     #16161) * t108 / 0.2E1 + t16164 / 0.2E1)) * t124 / 0.2E1 + (t1727 -
     # t4 * (t1188 / 0.2E1 + t16908 * (t16923 + t16924 + t16925) / 0.2E1
     #) * t16411) * t124 + t4298 + (t1308 - t16909 * (t16887 * t16893 + 
     #t16889 * t16891 + t16896 * t16900) * ((t16938 - t16161) * t177 / 0
     #.2E1 + (t16161 - t16941) * t177 / 0.2E1)) * t124 / 0.2E1 + (t16225
     # * t6109 - t16958) * t177 / 0.2E1 + (-t16231 * t6289 + t16958) * t
     #177 / 0.2E1 + (t16236 * t6129 - t16978) * t177 / 0.2E1 + (-t16242 
     #* t6309 + t16978) * t177 / 0.2E1 + (t4 * (t6098 * (t16991 + t16992
     # + t16993) / 0.2E1 + t17000 / 0.2E1) * t1301 - t4 * (t17000 / 0.2E
     #1 + t6278 * (t17005 + t17006 + t17007) / 0.2E1) * t1304) * t177
        t17017 = t17016 * t1133
        t17024 = t7790 ** 2
        t17025 = t7799 ** 2
        t17026 = t7806 ** 2
        t17028 = t7812 * (t17024 + t17025 + t17026)
        t17031 = t4 * (t16834 / 0.2E1 + t17028 / 0.2E1)
        t17032 = t17031 * t3231
        t17036 = t3174 / 0.2E1 + t16425 / 0.2E1
        t17038 = t7362 * t17036
        t17041 = (t16858 - t17038) * t108 / 0.2E1
        t17045 = t7790 * t7804 + t7793 * t7806 + t7797 * t7799
        t16276 = t7813 * t17045
        t17047 = t16276 * t7849
        t17050 = (t16882 - t17047) * t108 / 0.2E1
        t17056 = t8750 * t8764 + t8753 * t8766 + t8757 * t8759
        t16282 = t8773 * t17056
        t17058 = t16282 * t8781
        t17060 = t16107 * t3233
        t17063 = (t17058 - t17060) * t177 / 0.2E1
        t17067 = t8898 * t8912 + t8901 * t8914 + t8905 * t8907
        t16289 = t8921 * t17067
        t17069 = t16289 * t8929
        t17072 = (t17060 - t17069) * t177 / 0.2E1
        t17074 = t2985 / 0.2E1 + t16324 / 0.2E1
        t17076 = t8230 * t17074
        t17078 = t2720 * t16856
        t17081 = (t17076 - t17078) * t177 / 0.2E1
        t17083 = t3004 / 0.2E1 + t16342 / 0.2E1
        t17085 = t8369 * t17083
        t17088 = (t17078 - t17085) * t177 / 0.2E1
        t17089 = t8764 ** 2
        t17090 = t8757 ** 2
        t17091 = t8753 ** 2
        t17093 = t8772 * (t17089 + t17090 + t17091)
        t17094 = t2853 ** 2
        t17095 = t2846 ** 2
        t17096 = t2842 ** 2
        t17098 = t2861 * (t17094 + t17095 + t17096)
        t17101 = t4 * (t17093 / 0.2E1 + t17098 / 0.2E1)
        t17102 = t17101 * t2910
        t17103 = t8912 ** 2
        t17104 = t8905 ** 2
        t17105 = t8901 ** 2
        t17107 = t8920 * (t17103 + t17104 + t17105)
        t17110 = t4 * (t17098 / 0.2E1 + t17107 / 0.2E1)
        t17111 = t17110 * t2913
        t17114 = (t16838 - t17032) * t108 + t16861 + t17041 + t16885 + t
     #17050 + t4620 + t16173 / 0.2E1 + t16301 + t4621 + t16262 / 0.2E1 +
     # t17063 + t17072 + t17081 + t17088 + (t17102 - t17111) * t177
        t17115 = t17114 * t2860
        t17117 = (t4748 - t17115) * t124
        t17119 = t4750 / 0.2E1 + t17117 / 0.2E1
        t17121 = t641 * t17119
        t17125 = t11678 ** 2
        t17126 = t11687 ** 2
        t17127 = t11694 ** 2
        t17136 = u(t2386,t16132,k,n)
        t17155 = rx(t507,t16132,k,0,0)
        t17156 = rx(t507,t16132,k,1,1)
        t17158 = rx(t507,t16132,k,2,2)
        t17160 = rx(t507,t16132,k,1,2)
        t17162 = rx(t507,t16132,k,2,1)
        t17164 = rx(t507,t16132,k,0,1)
        t17165 = rx(t507,t16132,k,1,0)
        t17169 = rx(t507,t16132,k,2,0)
        t17171 = rx(t507,t16132,k,0,2)
        t17177 = 0.1E1 / (t17155 * t17156 * t17158 - t17155 * t17160 * t
     #17162 - t17156 * t17169 * t17171 - t17158 * t17164 * t17165 + t171
     #60 * t17164 * t17169 + t17162 * t17165 * t17171)
        t17178 = t4 * t17177
        t17192 = t17165 ** 2
        t17193 = t17156 ** 2
        t17194 = t17160 ** 2
        t17207 = u(t507,t16132,t174,n)
        t17210 = u(t507,t16132,t179,n)
        t17223 = t12638 * t12652 + t12641 * t12654 + t12645 * t12647
        t17227 = t16276 * t7821
        t17234 = t12786 * t12800 + t12789 * t12802 + t12793 * t12795
        t17243 = t7936 / 0.2E1 + (t7842 - t17207) * t124 / 0.2E1
        t17247 = t7377 * t17036
        t17254 = t7951 / 0.2E1 + (t7845 - t17210) * t124 / 0.2E1
        t17260 = t12652 ** 2
        t17261 = t12645 ** 2
        t17262 = t12641 ** 2
        t17265 = t7804 ** 2
        t17266 = t7797 ** 2
        t17267 = t7793 ** 2
        t17269 = t7812 * (t17265 + t17266 + t17267)
        t17274 = t12800 ** 2
        t17275 = t12793 ** 2
        t17276 = t12789 ** 2
        t17285 = (t17032 - t4 * (t17028 / 0.2E1 + t11700 * (t17125 + t17
     #126 + t17127) / 0.2E1) * t7819) * t108 + t17041 + (t17038 - t11188
     # * (t7765 / 0.2E1 + (t7763 - t17136) * t124 / 0.2E1)) * t108 / 0.2
     #E1 + t17050 + (t17047 - t11701 * (t11678 * t11692 + t11681 * t1169
     #4 + t11685 * t11687) * t11737) * t108 / 0.2E1 + t7826 + (t7823 - t
     #17178 * (t17155 * t17165 + t17156 * t17164 + t17160 * t17171) * (t
     #16167 / 0.2E1 + (t16165 - t17136) * t108 / 0.2E1)) * t124 / 0.2E1 
     #+ (t7835 - t4 * (t7831 / 0.2E1 + t17177 * (t17192 + t17193 + t1719
     #4) / 0.2E1) * t16425) * t124 + t7854 + (t7851 - t17178 * (t17156 *
     # t17162 + t17158 * t17160 + t17165 * t17169) * ((t17207 - t16165) 
     #* t177 / 0.2E1 + (t16165 - t17210) * t177 / 0.2E1)) * t124 / 0.2E1
     # + (t12661 * t12669 * t17223 - t17227) * t177 / 0.2E1 + (-t12809 *
     # t12817 * t17234 + t17227) * t177 / 0.2E1 + (t12051 * t17243 - t17
     #247) * t177 / 0.2E1 + (-t12188 * t17254 + t17247) * t177 / 0.2E1 +
     # (t4 * (t12660 * (t17260 + t17261 + t17262) / 0.2E1 + t17269 / 0.2
     #E1) * t7844 - t4 * (t17269 / 0.2E1 + t12808 * (t17274 + t17275 + t
     #17276) / 0.2E1) * t7847) * t177
        t17286 = t17285 * t7811
        t17299 = t4056 * t9026
        t17322 = t6076 ** 2
        t17323 = t6085 ** 2
        t17324 = t6092 ** 2
        t17327 = t8750 ** 2
        t17328 = t8759 ** 2
        t17329 = t8766 ** 2
        t17331 = t8772 * (t17327 + t17328 + t17329)
        t17336 = t12638 ** 2
        t17337 = t12647 ** 2
        t17338 = t12654 ** 2
        t17350 = t8208 * t17074
        t17362 = t16282 * t8806
        t17371 = rx(i,t16132,t174,0,0)
        t17372 = rx(i,t16132,t174,1,1)
        t17374 = rx(i,t16132,t174,2,2)
        t17376 = rx(i,t16132,t174,1,2)
        t17378 = rx(i,t16132,t174,2,1)
        t17380 = rx(i,t16132,t174,0,1)
        t17381 = rx(i,t16132,t174,1,0)
        t17385 = rx(i,t16132,t174,2,0)
        t17387 = rx(i,t16132,t174,0,2)
        t17393 = 0.1E1 / (t17371 * t17372 * t17374 - t17371 * t17376 * t
     #17378 - t17372 * t17385 * t17387 - t17374 * t17380 * t17381 + t173
     #76 * t17380 * t17385 + t17378 * t17381 * t17387)
        t17394 = t4 * t17393
        t17410 = t17381 ** 2
        t17411 = t17372 ** 2
        t17412 = t17376 ** 2
        t17425 = u(i,t16132,t1364,n)
        t17435 = rx(i,t1111,t1364,0,0)
        t17436 = rx(i,t1111,t1364,1,1)
        t17438 = rx(i,t1111,t1364,2,2)
        t17440 = rx(i,t1111,t1364,1,2)
        t17442 = rx(i,t1111,t1364,2,1)
        t17444 = rx(i,t1111,t1364,0,1)
        t17445 = rx(i,t1111,t1364,1,0)
        t17449 = rx(i,t1111,t1364,2,0)
        t17451 = rx(i,t1111,t1364,0,2)
        t17457 = 0.1E1 / (t17435 * t17436 * t17438 - t17435 * t17440 * t
     #17442 - t17436 * t17449 * t17451 - t17438 * t17444 * t17445 + t174
     #40 * t17444 * t17449 + t17442 * t17445 * t17451)
        t17458 = t4 * t17457
        t17468 = (t6130 - t8802) * t108 / 0.2E1 + (t8802 - t12690) * t10
     #8 / 0.2E1
        t17487 = t17449 ** 2
        t17488 = t17442 ** 2
        t17489 = t17438 ** 2
        t16579 = t17458 * (t17436 * t17442 + t17438 * t17440 + t17445 * 
     #t17449)
        t17498 = (t4 * (t6098 * (t17322 + t17323 + t17324) / 0.2E1 + t17
     #331 / 0.2E1) * t6107 - t4 * (t17331 / 0.2E1 + t12660 * (t17336 + t
     #17337 + t17338) / 0.2E1) * t8779) * t108 + (t16236 * t6103 - t1735
     #0) * t108 / 0.2E1 + (-t12035 * t17243 + t17350) * t108 / 0.2E1 + (
     #t16225 * t6134 - t17362) * t108 / 0.2E1 + (-t12661 * t12694 * t172
     #23 + t17362) * t108 / 0.2E1 + t8786 + (t8783 - t17394 * (t17371 * 
     #t17381 + t17372 * t17380 + t17376 * t17387) * ((t16938 - t16251) *
     # t108 / 0.2E1 + (t16251 - t17207) * t108 / 0.2E1)) * t124 / 0.2E1 
     #+ (t8795 - t4 * (t8791 / 0.2E1 + t17393 * (t17410 + t17411 + t1741
     #2) / 0.2E1) * t16324) * t124 + t8811 + (t8808 - t17394 * (t17372 *
     # t17378 + t17374 * t17376 + t17381 * t17385) * ((t17425 - t16251) 
     #* t177 / 0.2E1 + t16253 / 0.2E1)) * t124 / 0.2E1 + (t17458 * (t174
     #35 * t17449 + t17438 * t17451 + t17442 * t17444) * t17468 - t17058
     #) * t177 / 0.2E1 + t17063 + (t16579 * (t8854 / 0.2E1 + (t8802 - t1
     #7425) * t124 / 0.2E1) - t17076) * t177 / 0.2E1 + t17081 + (t4 * (t
     #17457 * (t17487 + t17488 + t17489) / 0.2E1 + t17093 / 0.2E1) * t88
     #04 - t17102) * t177
        t17499 = t17498 * t8771
        t17502 = t6256 ** 2
        t17503 = t6265 ** 2
        t17504 = t6272 ** 2
        t17507 = t8898 ** 2
        t17508 = t8907 ** 2
        t17509 = t8914 ** 2
        t17511 = t8920 * (t17507 + t17508 + t17509)
        t17516 = t12786 ** 2
        t17517 = t12795 ** 2
        t17518 = t12802 ** 2
        t17530 = t8349 * t17083
        t17542 = t16289 * t8954
        t17551 = rx(i,t16132,t179,0,0)
        t17552 = rx(i,t16132,t179,1,1)
        t17554 = rx(i,t16132,t179,2,2)
        t17556 = rx(i,t16132,t179,1,2)
        t17558 = rx(i,t16132,t179,2,1)
        t17560 = rx(i,t16132,t179,0,1)
        t17561 = rx(i,t16132,t179,1,0)
        t17565 = rx(i,t16132,t179,2,0)
        t17567 = rx(i,t16132,t179,0,2)
        t17573 = 0.1E1 / (t17551 * t17552 * t17554 - t17551 * t17556 * t
     #17558 - t17552 * t17565 * t17567 - t17554 * t17560 * t17561 + t175
     #56 * t17560 * t17565 + t17558 * t17561 * t17567)
        t17574 = t4 * t17573
        t17590 = t17561 ** 2
        t17591 = t17552 ** 2
        t17592 = t17556 ** 2
        t17605 = u(i,t16132,t1375,n)
        t17615 = rx(i,t1111,t1375,0,0)
        t17616 = rx(i,t1111,t1375,1,1)
        t17618 = rx(i,t1111,t1375,2,2)
        t17620 = rx(i,t1111,t1375,1,2)
        t17622 = rx(i,t1111,t1375,2,1)
        t17624 = rx(i,t1111,t1375,0,1)
        t17625 = rx(i,t1111,t1375,1,0)
        t17629 = rx(i,t1111,t1375,2,0)
        t17631 = rx(i,t1111,t1375,0,2)
        t17637 = 0.1E1 / (t17615 * t17616 * t17618 - t17615 * t17620 * t
     #17622 - t17616 * t17629 * t17631 - t17618 * t17624 * t17625 + t176
     #20 * t17624 * t17629 + t17622 * t17625 * t17631)
        t17638 = t4 * t17637
        t17648 = (t6310 - t8950) * t108 / 0.2E1 + (t8950 - t12838) * t10
     #8 / 0.2E1
        t17667 = t17629 ** 2
        t17668 = t17622 ** 2
        t17669 = t17618 ** 2
        t16719 = t17638 * (t17616 * t17622 + t17618 * t17620 + t17625 * 
     #t17629)
        t17678 = (t4 * (t6278 * (t17502 + t17503 + t17504) / 0.2E1 + t17
     #511 / 0.2E1) * t6287 - t4 * (t17511 / 0.2E1 + t12808 * (t17516 + t
     #17517 + t17518) / 0.2E1) * t8927) * t108 + (t16242 * t6283 - t1753
     #0) * t108 / 0.2E1 + (-t12168 * t17254 + t17530) * t108 / 0.2E1 + (
     #t16231 * t6314 - t17542) * t108 / 0.2E1 + (-t12809 * t12842 * t172
     #34 + t17542) * t108 / 0.2E1 + t8934 + (t8931 - t17574 * (t17551 * 
     #t17561 + t17552 * t17560 + t17556 * t17567) * ((t16941 - t16254) *
     # t108 / 0.2E1 + (t16254 - t17210) * t108 / 0.2E1)) * t124 / 0.2E1 
     #+ (t8943 - t4 * (t8939 / 0.2E1 + t17573 * (t17590 + t17591 + t1759
     #2) / 0.2E1) * t16342) * t124 + t8959 + (t8956 - t17574 * (t17552 *
     # t17558 + t17554 * t17556 + t17561 * t17565) * (t16256 / 0.2E1 + (
     #t16254 - t17605) * t177 / 0.2E1)) * t124 / 0.2E1 + t17072 + (t1706
     #9 - t17638 * (t17615 * t17629 + t17618 * t17631 + t17622 * t17624)
     # * t17648) * t177 / 0.2E1 + t17088 + (t17085 - t16719 * (t9002 / 0
     #.2E1 + (t8950 - t17605) * t124 / 0.2E1)) * t177 / 0.2E1 + (t17111 
     #- t4 * (t17107 / 0.2E1 + t17637 * (t17667 + t17668 + t17669) / 0.2
     #E1) * t8952) * t177
        t17679 = t17678 * t8919
        t17698 = t4056 * t8414
        t17720 = t706 * t17119
        t16807 = ((t6204 - t8874) * t108 / 0.2E1 + (t8874 - t12762) * t1
     #08 / 0.2E1) * t4645
        t16814 = ((t6384 - t9022) * t108 / 0.2E1 + (t9022 - t12910) * t1
     #08 / 0.2E1) * t4684
        t17737 = (t4277 * t5646 - t4600 * t8412) * t108 + (t292 * (t4427
     # / 0.2E1 + (t4425 - t17017) * t124 / 0.2E1) - t17121) * t108 / 0.2
     #E1 + (t17121 - t2377 * (t7987 / 0.2E1 + (t7985 - t17286) * t124 / 
     #0.2E1)) * t108 / 0.2E1 + (t3674 * t6388 - t17299) * t108 / 0.2E1 +
     # (-t12299 * t4614 + t17299) * t108 / 0.2E1 + t8419 + (t8416 - t303
     #1 * ((t17017 - t17115) * t108 / 0.2E1 + (t17115 - t17286) * t108 /
     # 0.2E1)) * t124 / 0.2E1 + (-t17117 * t2869 + t8421) * t124 + t9031
     # + (t9028 - t2720 * ((t17499 - t17115) * t177 / 0.2E1 + (t17115 - 
     #t17679) * t177 / 0.2E1)) * t124 / 0.2E1 + (t16807 * t4649 - t17698
     #) * t177 / 0.2E1 + (-t16814 * t4688 + t17698) * t177 / 0.2E1 + (t4
     #410 * (t9055 / 0.2E1 + (t8874 - t17499) * t124 / 0.2E1) - t17720) 
     #* t177 / 0.2E1 + (t17720 - t4426 * (t9068 / 0.2E1 + (t9022 - t1767
     #9) * t124 / 0.2E1)) * t177 / 0.2E1 + (t4734 * t8876 - t4743 * t902
     #4) * t177
        t17738 = t13702 * t17737
        t17741 = cc * t15617
        t17751 = t6624 / 0.2E1 + t16459 / 0.2E1
        t17753 = t3031 * t17751
        t17767 = t16107 * t6937
        t17783 = (t7056 - t6606) * t108 / 0.2E1 + (t6606 - t10993) * t10
     #8 / 0.2E1
        t17787 = t16107 * t6763
        t17796 = (t7059 - t6640) * t108 / 0.2E1 + (t6640 - t10996) * t10
     #8 / 0.2E1
        t17807 = t2720 * t17751
        t17822 = (t16837 * t6758 - t17031 * t6761) * t108 + (t1045 * (t6
     #956 / 0.2E1 + t16604 / 0.2E1) - t17753) * t108 / 0.2E1 + (t17753 -
     # t7362 * (t6975 / 0.2E1 + t16619 / 0.2E1)) * t108 / 0.2E1 + (t1610
     #2 * t7063 - t17767) * t108 / 0.2E1 + (-t11000 * t17045 * t7813 + t
     #17767) * t108 / 0.2E1 + t9653 + t16721 / 0.2E1 + t16469 + t9654 + 
     #t16792 / 0.2E1 + (t17056 * t17783 * t8773 - t17787) * t177 / 0.2E1
     # + (-t17067 * t17796 * t8921 + t17787) * t177 / 0.2E1 + (t8230 * (
     #t6608 / 0.2E1 + t16535 / 0.2E1) - t17807) * t177 / 0.2E1 + (t17807
     # - t8369 * (t6642 / 0.2E1 + t16554 / 0.2E1)) * t177 / 0.2E1 + (t17
     #101 * t6933 - t17110 * t6935) * t177
        t17828 = t14664 * (t14671 / 0.2E1 + (-t17741 * t17822 + t14669) 
     #* t124 / 0.2E1)
        t17833 = (-t17114 * t17741 + t13703) * t124
        t17836 = t13694 * (t13705 / 0.2E1 + t17833 / 0.2E1)
        t17843 = t6450 * t16803 / 0.4E1
        t17845 = t13694 * (t13705 - t17833)
        t17847 = t864 * t17845 / 0.24E2
        t17849 = t864 * t17836 / 0.4E1
        t17851 = t7451 * t17738 / 0.12E2
        t17855 = t1945 * t17828 / 0.8E1
        t17856 = dy * t6870
        t17860 = t9108 * t17856 / 0.24E2
        t17861 = t16808 - t16811 - t3386 * t17738 / 0.12E2 - t14771 - t7
     #018 * t17828 / 0.8E1 + t14775 - t2378 * t17836 / 0.4E1 + t679 * t6
     #9 * t16045 / 0.6E1 + t17843 + t17847 + t17849 + t17851 - t2378 * t
     #17845 / 0.24E2 + t17855 - t2746 * t17856 / 0.24E2 + t17860
        t17863 = (t16806 + t17861) * t65
        t17866 = t15602 / 0.2E1
        t17867 = t13688 + t16047 + t13730 + t16054 + t14295 + t14663 - t
     #14676 - t14678 - t16808 + t16811 - t17866 - t17843
        t17869 = sqrt(t16186)
        t17877 = (t15623 - (t15621 - (-cc * t16155 * t16457 * t17869 + t
     #15619) * t124) * t124) * t124
        t17883 = t1046 * (t15623 - dy * (t15625 - t17877) / 0.12E2) / 0.
     #24E2
        t17891 = dy * (t15633 + t15621 / 0.2E1 - t1046 * (t15625 / 0.2E1
     # + t17877 / 0.2E1) / 0.6E1) / 0.4E1
        t17893 = dy * t2873 / 0.24E2
        t17897 = t3269 * (t1220 - dy * t2798 / 0.24E2)
        t17899 = -t17863 * t863 + t15597 + t15631 - t15640 - t17847 - t1
     #7849 - t17851 - t17855 - t17860 - t17883 - t17891 - t17893 + t1789
     #7
        t17914 = t4 * (t15674 + t15678 / 0.2E1 - dy * ((t15671 - t15673)
     # * t124 / 0.2E1 - (-t2861 * t2907 + t15678) * t124 / 0.2E1) / 0.8E
     #1)
        t17925 = (t6933 - t6935) * t177
        t17942 = t9815 + t9816 - t9820 + t712 / 0.4E1 + t715 / 0.4E1 - t
     #15728 / 0.12E2 - dy * ((t15709 + t15710 - t15711 - t9842 - t9843 +
     # t9844) * t124 / 0.2E1 - (t15714 + t15715 - t15729 - t6933 / 0.2E1
     # - t6935 / 0.2E1 + t1363 * (((t16479 - t6933) * t177 - t17925) * t
     #177 / 0.2E1 + (t17925 - (t6935 - t16485) * t177) * t177 / 0.2E1) /
     # 0.6E1) * t124 / 0.2E1) / 0.8E1
        t17947 = t4 * (t15673 / 0.2E1 + t15678 / 0.2E1)
        t17949 = t5526 / 0.4E1 + t5620 / 0.4E1 + t8876 / 0.4E1 + t9024 /
     # 0.4E1
        t17960 = t5155 * t9676
        t17972 = t4363 * t10063
        t17984 = (-t17783 * t8773 * t8777 + t10045) * t124
        t17988 = (-t6608 * t8794 + t10050) * t124
        t17994 = (t10065 - t8230 * (t16479 / 0.2E1 + t6933 / 0.2E1)) * t
     #124
        t17998 = (t6048 * t9532 - t8736 * t9656) * t108 + (t5048 * t9554
     # - t17960) * t108 / 0.2E1 + (-t12561 * t8073 + t17960) * t108 / 0.
     #2E1 + (t4082 * t9938 - t17972) * t108 / 0.2E1 + (-t12749 * t7882 +
     # t17972) * t108 / 0.2E1 + t10048 + t17984 / 0.2E1 + t17988 + t1006
     #8 + t17994 / 0.2E1 + t16577 / 0.2E1 + t9665 + t16506 / 0.2E1 + t96
     #83 + t16739
        t17999 = t17998 * t4643
        t18009 = t5202 * t9685
        t18021 = t4394 * t10116
        t18033 = (-t17796 * t8921 * t8925 + t10098) * t124
        t18037 = (-t6642 * t8942 + t10103) * t124
        t18043 = (t10118 - t8369 * (t6935 / 0.2E1 + t16485 / 0.2E1)) * t
     #124
        t18047 = (t6228 * t9545 - t8884 * t9667) * t108 + (t5100 * t9563
     # - t18009) * t108 / 0.2E1 + (-t12568 * t8271 + t18009) * t108 / 0.
     #2E1 + (t4365 * t9438 - t18021) * t108 / 0.2E1 + (-t12803 * t7921 +
     # t18021) * t108 / 0.2E1 + t10101 + t18033 / 0.2E1 + t18037 + t1012
     #1 + t18043 / 0.2E1 + t9674 + t16593 / 0.2E1 + t9690 + t16520 / 0.2
     #E1 + t16744
        t18048 = t18047 * t4682
        t18052 = t10074 / 0.4E1 + t10127 / 0.4E1 + (t17999 - t9696) * t1
     #77 / 0.4E1 + (t9696 - t18048) * t177 / 0.4E1
        t18058 = dy * (t704 / 0.2E1 - t6941 / 0.2E1)
        t18062 = t17914 * t9108 * t17942
        t18065 = t17947 * t9711 * t17949 / 0.2E1
        t18068 = t17947 * t9715 * t18052 / 0.6E1
        t18070 = t9108 * t18058 / 0.24E2
        t18072 = (t17914 * t2746 * t17942 + t17947 * t9402 * t17949 / 0.
     #2E1 + t17947 * t9408 * t18052 / 0.6E1 - t2746 * t18058 / 0.24E2 - 
     #t18062 - t18065 - t18068 + t18070) * t65
        t18085 = (t2910 - t2913) * t177
        t18103 = t17914 * (t10158 + t10159 - t10163 + t2316 / 0.4E1 + t2
     #319 / 0.4E1 - t15910 / 0.12E2 - dy * ((t15891 + t15892 - t15893 - 
     #t10185 - t10186 + t10187) * t124 / 0.2E1 - (t15896 + t15897 - t159
     #11 - t2910 / 0.2E1 - t2913 / 0.2E1 + t1363 * (((t8804 - t2910) * t
     #177 - t18085) * t177 / 0.2E1 + (t18085 - (t2913 - t8952) * t177) *
     # t177 / 0.2E1) / 0.6E1) * t124 / 0.2E1) / 0.8E1)
        t18107 = dy * (t2312 / 0.2E1 - t2919 / 0.2E1) / 0.24E2
        t18112 = t13603 * t71 / 0.6E1 + (-t13603 * t863 + t13593 + t1359
     #6 + t13599 - t13601 + t13675 - t13679) * t71 / 0.2E1 + t15582 * t7
     #1 / 0.6E1 + (t15589 + t15667) * t71 / 0.2E1 + t15866 * t71 / 0.6E1
     # + (-t15866 * t863 + t15856 + t15859 + t15862 - t15864 + t15919 - 
     #t15923) * t71 / 0.2E1 - t16003 * t71 / 0.6E1 - (-t16003 * t863 + t
     #15993 + t15996 + t15999 - t16001 + t16034 - t16038) * t71 / 0.2E1 
     #- t17863 * t71 / 0.6E1 - (t17867 + t17899) * t71 / 0.2E1 - t18072 
     #* t71 / 0.6E1 - (-t18072 * t863 + t18062 + t18065 + t18068 - t1807
     #0 + t18103 - t18107) * t71 / 0.2E1
        t18115 = t745 * t750
        t18120 = t784 * t789
        t18128 = t4 * (t18115 / 0.2E1 + t9796 - dz * ((t3054 * t3114 - t
     #18115) * t177 / 0.2E1 - (t9795 - t18120) * t177 / 0.2E1) / 0.8E1)
        t18134 = (t399 - t752) * t108
        t18136 = ((t397 - t399) * t108 - t18134) * t108
        t18140 = (t18134 - (t752 - t6456) * t108) * t108
        t18143 = t868 * (t18136 / 0.2E1 + t18140 / 0.2E1)
        t18150 = (t6563 - t6566) * t108
        t18161 = t399 / 0.2E1
        t18162 = t752 / 0.2E1
        t18163 = t18143 / 0.6E1
        t18166 = t440 / 0.2E1
        t18167 = t791 / 0.2E1
        t18171 = (t440 - t791) * t108
        t18173 = ((t438 - t440) * t108 - t18171) * t108
        t18177 = (t18171 - (t791 - t6484) * t108) * t108
        t18180 = t868 * (t18173 / 0.2E1 + t18177 / 0.2E1)
        t18181 = t18180 / 0.6E1
        t18188 = t399 / 0.4E1 + t752 / 0.4E1 - t18143 / 0.12E2 + t13513 
     #+ t13514 - t13518 - dz * ((t6563 / 0.2E1 + t6566 / 0.2E1 - t868 * 
     #(((t7305 - t6563) * t108 - t18150) * t108 / 0.2E1 + (t18150 - (t65
     #66 - t10912) * t108) * t108 / 0.2E1) / 0.6E1 - t18161 - t18162 + t
     #18163) * t177 / 0.2E1 - (t13540 + t13541 - t13542 - t18166 - t1816
     #7 + t18181) * t177 / 0.2E1) / 0.8E1
        t18193 = t4 * (t18115 / 0.2E1 + t9795 / 0.2E1)
        t18195 = t6397 / 0.4E1 + t9033 / 0.4E1 + t3488 / 0.4E1 + t7456 /
     # 0.4E1
        t18204 = (t9947 - t10072) * t108 / 0.4E1 + (t10072 - t13363) * t
     #108 / 0.4E1 + t13580 / 0.4E1 + t13581 / 0.4E1
        t18210 = dz * (t6572 / 0.2E1 - t797 / 0.2E1)
        t18214 = t18128 * t9108 * t18188
        t18217 = t18193 * t9711 * t18195 / 0.2E1
        t18220 = t18193 * t9715 * t18204 / 0.6E1
        t18222 = t9108 * t18210 / 0.24E2
        t18224 = (t18128 * t2746 * t18188 + t18193 * t9402 * t18195 / 0.
     #2E1 + t18193 * t9408 * t18204 / 0.6E1 - t2746 * t18210 / 0.24E2 - 
     #t18214 - t18217 - t18220 + t18222) * t65
        t18232 = (t1260 - t1325) * t108
        t18234 = ((t1258 - t1260) * t108 - t18232) * t108
        t18238 = (t18232 - (t1325 - t2631) * t108) * t108
        t18241 = t868 * (t18234 / 0.2E1 + t18238 / 0.2E1)
        t18248 = (t1653 - t3117) * t108
        t18259 = t1260 / 0.2E1
        t18260 = t1325 / 0.2E1
        t18261 = t18241 / 0.6E1
        t18264 = t1341 / 0.2E1
        t18265 = t1348 / 0.2E1
        t18269 = (t1341 - t1348) * t108
        t18271 = ((t1346 - t1341) * t108 - t18269) * t108
        t18275 = (t18269 - (t1348 - t2670) * t108) * t108
        t18278 = t868 * (t18271 / 0.2E1 + t18275 / 0.2E1)
        t18279 = t18278 / 0.6E1
        t18287 = t18128 * (t1260 / 0.4E1 + t1325 / 0.4E1 - t18241 / 0.12
     #E2 + t13622 + t13623 - t13627 - dz * ((t1653 / 0.2E1 + t3117 / 0.2
     #E1 - t868 * (((t1651 - t1653) * t108 - t18248) * t108 / 0.2E1 + (t
     #18248 - (t3117 - t8155) * t108) * t108 / 0.2E1) / 0.6E1 - t18259 -
     # t18260 + t18261) * t177 / 0.2E1 - (t13649 + t13650 - t13651 - t18
     #264 - t18265 + t18279) * t177 / 0.2E1) / 0.8E1)
        t18291 = dz * (t3123 / 0.2E1 - t2341 / 0.2E1) / 0.24E2
        t18296 = t745 * t802
        t18301 = t784 * t819
        t18309 = t4 * (t18296 / 0.2E1 + t15674 - dz * ((t3054 * t3313 - 
     #t18296) * t177 / 0.2E1 - (t15673 - t18301) * t177 / 0.2E1) / 0.8E1
     #)
        t18315 = (t804 - t806) * t124
        t18317 = ((t6602 - t804) * t124 - t18315) * t124
        t18321 = (t18315 - (t806 - t6608) * t124) * t124
        t18324 = t1046 * (t18317 / 0.2E1 + t18321 / 0.2E1)
        t18331 = (t6811 - t6813) * t124
        t18342 = t804 / 0.2E1
        t18343 = t806 / 0.2E1
        t18344 = t18324 / 0.6E1
        t18347 = t821 / 0.2E1
        t18348 = t823 / 0.2E1
        t18352 = (t821 - t823) * t124
        t18354 = ((t6636 - t821) * t124 - t18352) * t124
        t18358 = (t18352 - (t823 - t6642) * t124) * t124
        t18361 = t1046 * (t18354 / 0.2E1 + t18358 / 0.2E1)
        t18362 = t18361 / 0.6E1
        t18369 = t804 / 0.4E1 + t806 / 0.4E1 - t18324 / 0.12E2 + t9344 +
     # t9345 - t9349 - dz * ((t6811 / 0.2E1 + t6813 / 0.2E1 - t1046 * ((
     #(t14304 - t6811) * t124 - t18331) * t124 / 0.2E1 + (t18331 - (t681
     #3 - t16500) * t124) * t124 / 0.2E1) / 0.6E1 - t18342 - t18343 + t1
     #8344) * t177 / 0.2E1 - (t9371 + t9372 - t9373 - t18347 - t18348 + 
     #t18362) * t177 / 0.2E1) / 0.8E1
        t18374 = t4 * (t18296 / 0.2E1 + t15673 / 0.2E1)
        t18376 = t9053 / 0.4E1 + t9055 / 0.4E1 + t4592 / 0.4E1 + t4750 /
     # 0.4E1
        t18385 = (t15793 - t10072) * t124 / 0.4E1 + (t10072 - t17999) * 
     #t124 / 0.4E1 + t9637 / 0.4E1 + t9698 / 0.4E1
        t18391 = dz * (t6819 / 0.2E1 - t829 / 0.2E1)
        t18395 = t18309 * t9108 * t18369
        t18398 = t18374 * t9711 * t18376 / 0.2E1
        t18401 = t18374 * t9715 * t18385 / 0.6E1
        t18403 = t9108 * t18391 / 0.24E2
        t18405 = (t18309 * t2746 * t18369 + t18374 * t9402 * t18376 / 0.
     #2E1 + t18374 * t9408 * t18385 / 0.6E1 - t2746 * t18391 / 0.24E2 - 
     #t18395 - t18398 - t18401 + t18403) * t65
        t18413 = (t2344 - t2346) * t124
        t18415 = ((t2980 - t2344) * t124 - t18413) * t124
        t18419 = (t18413 - (t2346 - t2985) * t124) * t124
        t18422 = t1046 * (t18415 / 0.2E1 + t18419 / 0.2E1)
        t18429 = (t3315 - t3317) * t124
        t18440 = t2344 / 0.2E1
        t18441 = t2346 / 0.2E1
        t18442 = t18422 / 0.6E1
        t18445 = t2357 / 0.2E1
        t18446 = t2359 / 0.2E1
        t18450 = (t2357 - t2359) * t124
        t18452 = ((t2999 - t2357) * t124 - t18450) * t124
        t18456 = (t18450 - (t2359 - t3004) * t124) * t124
        t18459 = t1046 * (t18452 / 0.2E1 + t18456 / 0.2E1)
        t18460 = t18459 / 0.6E1
        t18468 = t18309 * (t2344 / 0.4E1 + t2346 / 0.4E1 - t18422 / 0.12
     #E2 + t9731 + t9732 - t9736 - dz * ((t3315 / 0.2E1 + t3317 / 0.2E1 
     #- t1046 * (((t8549 - t3315) * t124 - t18429) * t124 / 0.2E1 + (t18
     #429 - (t3317 - t8854) * t124) * t124 / 0.2E1) / 0.6E1 - t18440 - t
     #18441 + t18442) * t177 / 0.2E1 - (t9758 + t9759 - t9760 - t18445 -
     # t18446 + t18460) * t177 / 0.2E1) / 0.8E1)
        t18472 = dz * (t3323 / 0.2E1 - t2365 / 0.2E1) / 0.24E2
        t18477 = dt * dz
        t18478 = sqrt(t3058)
        t18479 = cc * t18478
        t18480 = t1386 ** 2
        t18481 = t1395 ** 2
        t18482 = t1402 ** 2
        t18484 = t1408 * (t18480 + t18481 + t18482)
        t18485 = t3032 ** 2
        t18486 = t3041 ** 2
        t18487 = t3048 ** 2
        t18489 = t3054 * (t18485 + t18486 + t18487)
        t18492 = t4 * (t18484 / 0.2E1 + t18489 / 0.2E1)
        t18493 = t18492 * t1653
        t18494 = t8126 ** 2
        t18495 = t8135 ** 2
        t18496 = t8142 ** 2
        t18498 = t8148 * (t18494 + t18495 + t18496)
        t18501 = t4 * (t18489 / 0.2E1 + t18498 / 0.2E1)
        t18502 = t18501 * t3117
        t17650 = t1499 * (t1386 * t1396 + t1387 * t1395 + t1391 * t1402)
        t18510 = t17650 * t1511
        t17654 = t3110 * (t3032 * t3042 + t3033 * t3041 + t3037 * t3048)
        t18516 = t17654 * t3319
        t18519 = (t18510 - t18516) * t108 / 0.2E1
        t18523 = t8126 * t8136 + t8127 * t8135 + t8131 * t8142
        t17661 = t8149 * t18523
        t18525 = t17661 * t8172
        t18528 = (t18516 - t18525) * t108 / 0.2E1
        t18529 = k + 3
        t18530 = u(t5,j,t18529,n)
        t18532 = (t18530 - t1365) * t177
        t18534 = t18532 / 0.2E1 + t1367 / 0.2E1
        t18536 = t1534 * t18534
        t18537 = u(i,j,t18529,n)
        t18539 = (t18537 - t1624) * t177
        t18541 = t18539 / 0.2E1 + t1626 / 0.2E1
        t18543 = t2949 * t18541
        t18546 = (t18536 - t18543) * t108 / 0.2E1
        t18547 = u(t507,j,t18529,n)
        t18549 = (t18547 - t3115) * t177
        t18551 = t18549 / 0.2E1 + t3189 / 0.2E1
        t18553 = t7623 * t18551
        t18556 = (t18543 - t18553) * t108 / 0.2E1
        t18560 = t8507 * t8517 + t8508 * t8516 + t8512 * t8523
        t17682 = t8530 * t18560
        t18562 = t17682 * t8538
        t18564 = t17654 * t3119
        t18567 = (t18562 - t18564) * t124 / 0.2E1
        t18571 = t8812 * t8822 + t8813 * t8821 + t8817 * t8828
        t17688 = t8835 * t18571
        t18573 = t17688 * t8843
        t18576 = (t18564 - t18573) * t124 / 0.2E1
        t18577 = t8517 ** 2
        t18578 = t8508 ** 2
        t18579 = t8512 ** 2
        t18581 = t8529 * (t18577 + t18578 + t18579)
        t18582 = t3042 ** 2
        t18583 = t3033 ** 2
        t18584 = t3037 ** 2
        t18586 = t3054 * (t18582 + t18583 + t18584)
        t18589 = t4 * (t18581 / 0.2E1 + t18586 / 0.2E1)
        t18590 = t18589 * t3315
        t18591 = t8822 ** 2
        t18592 = t8813 ** 2
        t18593 = t8817 ** 2
        t18595 = t8834 * (t18591 + t18592 + t18593)
        t18598 = t4 * (t18586 / 0.2E1 + t18595 / 0.2E1)
        t18599 = t18598 * t3317
        t18602 = u(i,t121,t18529,n)
        t18604 = (t18602 - t2936) * t177
        t18606 = t18604 / 0.2E1 + t2938 / 0.2E1
        t18608 = t7990 * t18606
        t18610 = t3120 * t18541
        t18613 = (t18608 - t18610) * t124 / 0.2E1
        t18614 = u(i,t126,t18529,n)
        t18616 = (t18614 - t2957) * t177
        t18618 = t18616 / 0.2E1 + t2959 / 0.2E1
        t18620 = t8280 * t18618
        t18623 = (t18610 - t18620) * t124 / 0.2E1
        t18624 = rx(i,j,t18529,0,0)
        t18625 = rx(i,j,t18529,1,1)
        t18627 = rx(i,j,t18529,2,2)
        t18629 = rx(i,j,t18529,1,2)
        t18631 = rx(i,j,t18529,2,1)
        t18633 = rx(i,j,t18529,0,1)
        t18634 = rx(i,j,t18529,1,0)
        t18638 = rx(i,j,t18529,2,0)
        t18640 = rx(i,j,t18529,0,2)
        t18646 = 0.1E1 / (t18624 * t18625 * t18627 - t18624 * t18629 * t
     #18631 - t18625 * t18638 * t18640 - t18627 * t18633 * t18634 + t186
     #29 * t18633 * t18638 + t18631 * t18634 * t18640)
        t18647 = t4 * t18646
        t18653 = (t18530 - t18537) * t108
        t18655 = (t18537 - t18547) * t108
        t17728 = t18647 * (t18624 * t18638 + t18627 * t18640 + t18631 * 
     #t18633)
        t18661 = (t17728 * (t18653 / 0.2E1 + t18655 / 0.2E1) - t3121) * 
     #t177
        t18668 = (t18602 - t18537) * t124
        t18670 = (t18537 - t18614) * t124
        t17740 = t18647 * (t18625 * t18631 + t18627 * t18629 + t18634 * 
     #t18638)
        t18676 = (t17740 * (t18668 / 0.2E1 + t18670 / 0.2E1) - t3321) * 
     #t177
        t18678 = t18638 ** 2
        t18679 = t18631 ** 2
        t18680 = t18627 ** 2
        t18681 = t18678 + t18679 + t18680
        t18682 = t18646 * t18681
        t18685 = t4 * (t18682 / 0.2E1 + t3059 / 0.2E1)
        t18688 = (t18539 * t18685 - t3063) * t177
        t18689 = (t18493 - t18502) * t108 + t18519 + t18528 + t18546 + t
     #18556 + t18567 + t18576 + (t18590 - t18599) * t124 + t18613 + t186
     #23 + t18661 / 0.2E1 + t5521 + t18676 / 0.2E1 + t5522 + t18688
        t18691 = sqrt(t834)
        t18692 = cc * t18691
        t18693 = t18692 * t5523
        t18695 = (t18479 * t18689 - t18693) * t177
        t18696 = sqrt(t839)
        t18697 = cc * t18696
        t18698 = t18697 * t2371
        t18700 = (t18693 - t18698) * t177
        t18702 = t18477 * (t18695 - t18700)
        t18704 = t864 * t18702 / 0.24E2
        t18705 = ut(t5,j,t18529,n)
        t18707 = (t18705 - t6560) * t177
        t18715 = ut(i,j,t18529,n)
        t18717 = (t18715 - t6561) * t177
        t17769 = ((t18717 / 0.2E1 - t210 / 0.2E1) * t177 - t6700) * t177
        t18724 = t743 * t17769
        t18727 = ut(t507,j,t18529,n)
        t18729 = (t18727 - t6564) * t177
        t18744 = t5242 / 0.2E1
        t18754 = t4 * (t4799 / 0.2E1 + t18744 - dx * ((t4790 - t4799) * 
     #t108 / 0.2E1 - (t5242 - t5437) * t108 / 0.2E1) / 0.8E1)
        t18766 = t4 * (t18744 + t5437 / 0.2E1 - dx * ((t4799 - t5242) * 
     #t108 / 0.2E1 - (t5437 - t8022) * t108 / 0.2E1) / 0.8E1)
        t18773 = (t9892 - t10028) * t108
        t18789 = t5027 * t6267
        t18817 = (t10042 - t10047) * t124
        t18829 = t5489 / 0.2E1
        t18839 = t4 * (t5484 / 0.2E1 + t18829 - dy * ((t8486 - t5484) * 
     #t124 / 0.2E1 - (t5489 - t5498) * t124 / 0.2E1) / 0.8E1)
        t18851 = t4 * (t18829 + t5498 / 0.2E1 - dy * ((t5484 - t5489) * 
     #t124 / 0.2E1 - (t5498 - t8791) * t124 / 0.2E1) / 0.8E1)
        t18860 = t5027 * t6183
        t18875 = (t9910 - t10035) * t108
        t18905 = (t10060 - t10067) * t124
        t18916 = t10069 + t10070 + t761 + t815 - t1363 * ((t396 * ((t187
     #07 / 0.2E1 - t193 / 0.2E1) * t177 - t6881) * t177 - t18724) * t108
     # / 0.2E1 + (t18724 - t2484 * ((t18729 / 0.2E1 - t569 / 0.2E1) * t1
     #77 - t6900) * t177) * t108 / 0.2E1) / 0.6E1 + (t18754 * t399 - t18
     #766 * t752) * t108 - t868 * (((t9887 - t9892) * t108 - t18773) * t
     #108 / 0.2E1 + (t18773 - (t10028 - t13319) * t108) * t108 / 0.2E1) 
     #/ 0.6E1 - t1046 * ((t4831 * t7090 * t7368 - t18789) * t108 / 0.2E1
     # + (-t10542 * t5140 + t18789) * t108 / 0.2E1) / 0.6E1 - t868 * ((t
     #18136 * t5245 - t18140 * t5440) * t108 + ((t9881 - t10024) * t108 
     #- (t10024 - t13315) * t108) * t108) / 0.24E2 - t1046 * (((t15778 -
     # t10042) * t124 - t18817) * t124 / 0.2E1 + (t18817 - (t10047 - t17
     #984) * t124) * t124 / 0.2E1) / 0.6E1 + (t18839 * t804 - t18851 * t
     #806) * t124 - t868 * ((t13817 * t5148 - t18860) * t124 / 0.2E1 + (
     #-t16052 * t5155 + t18860) * t124 / 0.2E1) / 0.6E1 - t868 * (((t990
     #3 - t9910) * t108 - t18875) * t108 / 0.2E1 + (t18875 - (t10035 - t
     #13326) * t108) * t108 / 0.2E1) / 0.6E1 - t868 * ((t2949 * ((t7305 
     #/ 0.2E1 - t6566 / 0.2E1) * t108 - (t6563 / 0.2E1 - t10912 / 0.2E1)
     # * t108) * t108 - t6463) * t177 / 0.2E1 + t6478 / 0.2E1) / 0.6E1 -
     # t1046 * (((t15788 - t10060) * t124 - t18905) * t124 / 0.2E1 + (t1
     #8905 - (t10067 - t17994) * t124) * t124 / 0.2E1) / 0.6E1
        t18917 = ut(i,t121,t18529,n)
        t18919 = (t18917 - t6680) * t177
        t18929 = t796 * t17769
        t18932 = ut(i,t126,t18529,n)
        t18934 = (t18932 - t6712) * t177
        t18970 = (t17740 * ((t18917 - t18715) * t124 / 0.2E1 + (t18715 -
     # t18932) * t124 / 0.2E1) - t6817) * t177
        t19004 = (t17728 * ((t18705 - t18715) * t108 / 0.2E1 + (t18715 -
     # t18727) * t108 / 0.2E1) - t6570) * t177
        t19022 = (t18685 * t18717 - t6794) * t177
        t19038 = t4 * (t3059 / 0.2E1 + t3352 - dz * ((t18682 - t3059) * 
     #t177 / 0.2E1 - t3367 / 0.2E1) / 0.8E1)
        t19042 = -t1363 * ((t4276 * ((t18919 / 0.2E1 - t689 / 0.2E1) * t
     #177 - t6685) * t177 - t18929) * t124 / 0.2E1 + (t18929 - t4410 * (
     #(t18934 / 0.2E1 - t712 / 0.2E1) * t177 - t6717) * t177) * t124 / 0
     #.2E1) / 0.6E1 - t1046 * ((t18317 * t5492 - t18321 * t5501) * t124 
     #+ ((t15782 - t10052) * t124 - (t10052 - t17988) * t124) * t124) / 
     #0.24E2 - t1363 * (((t18970 - t6819) * t177 - t6821) * t177 / 0.2E1
     # + t6825 / 0.2E1) / 0.6E1 - t1046 * ((t3120 * ((t14304 / 0.2E1 - t
     #6813 / 0.2E1) * t124 - (t6811 / 0.2E1 - t16500 / 0.2E1) * t124) * 
     #t124 - t6615) * t177 / 0.2E1 + t6633 / 0.2E1) / 0.6E1 - t1363 * ((
     #(t19004 - t6572) * t177 - t6574) * t177 / 0.2E1 + t6578 / 0.2E1) /
     # 0.6E1 - t1363 * ((t3062 * ((t18717 - t6697) * t177 - t6781) * t17
     #7 - t6786) * t177 + ((t19022 - t6796) * t177 - t6798) * t177) / 0.
     #24E2 + (t19038 * t6697 - t6844) * t177 + t10061 + t10068 + t10036 
     #+ t10043 + t10048 + t10029 + t9893 + t9911
        t19044 = t18692 * (t18916 + t19042)
        t19046 = t6450 * t19044 / 0.4E1
        t19049 = t18477 * (t18695 / 0.2E1 + t18700 / 0.2E1)
        t19054 = t210 - dz * t6784 / 0.24E2
        t19058 = t3363 * t9108 * t19054
        t19059 = t18697 * t9080
        t19061 = t3386 * t19059 / 0.12E2
        t19062 = dz * t6797
        t19065 = sqrt(t848)
        t19066 = cc * t19065
        t19067 = t19066 * t5617
        t19069 = (t18698 - t19067) * t177
        t19072 = t18477 * (t18700 / 0.2E1 + t19069 / 0.2E1)
        t19074 = t864 * t19072 / 0.4E1
        t19100 = t2335 + t5522 + t5459 + t5470 + t5479 + t5257 + t5264 +
     # t2355 + t5513 + t5520 + t5521 + t5452 - t1363 * ((t3062 * ((t1853
     #9 - t1626) * t177 - t3019) * t177 - t3024) * t177 + ((t18688 - t30
     #65) * t177 - t3067) * t177) / 0.24E2 - t1363 * (((t18676 - t3323) 
     #* t177 - t3325) * t177 / 0.2E1 + t3329 / 0.2E1) / 0.6E1 + (t1626 *
     # t19038 - t3364) * t177
        t18154 = ((t18539 / 0.2E1 - t939 / 0.2E1) * t177 - t1629) * t177
        t19114 = t743 * t18154
        t19137 = (t5256 - t5451) * t108
        t19153 = t5027 * t2776
        t19181 = (t5469 - t5478) * t124
        t19201 = t5027 * t2599
        t19216 = (t5263 - t5458) * t108
        t19236 = t796 * t18154
        t19308 = (t5512 - t5519) * t124
        t19319 = -t1363 * ((t396 * ((t18532 / 0.2E1 - t925 / 0.2E1) * t1
     #77 - t1614) * t177 - t19114) * t108 / 0.2E1 + (t19114 - t2484 * ((
     #t18549 / 0.2E1 - t956 / 0.2E1) * t177 - t3192) * t177) * t108 / 0.
     #2E1) / 0.6E1 + (t1260 * t18754 - t1325 * t18766) * t108 - t868 * (
     #((t4835 - t5256) * t108 - t19137) * t108 / 0.2E1 + (t19137 - (t545
     #1 - t8043) * t108) * t108 / 0.2E1) / 0.6E1 - t1046 * ((t1808 * t45
     #00 - t19153) * t108 / 0.2E1 + (-t10004 * t5140 + t19153) * t108 / 
     #0.2E1) / 0.6E1 - t868 * ((t18234 * t5245 - t18238 * t5440) * t108 
     #+ ((t5248 - t5443) * t108 - (t5443 - t8028) * t108) * t108) / 0.24
     #E2 - t1046 * (((t8480 - t5469) * t124 - t19181) * t124 / 0.2E1 + (
     #t19181 - (t5478 - t8785) * t124) * t124 / 0.2E1) / 0.6E1 + (t18839
     # * t2344 - t18851 * t2346) * t124 - t868 * ((t13380 * t5148 - t192
     #01) * t124 / 0.2E1 + (-t15535 * t5155 + t19201) * t124 / 0.2E1) / 
     #0.6E1 - t868 * (((t4860 - t5263) * t108 - t19216) * t108 / 0.2E1 +
     # (t19216 - (t5458 - t8057) * t108) * t108 / 0.2E1) / 0.6E1 - t1363
     # * ((t4276 * ((t18604 / 0.2E1 - t2301 / 0.2E1) * t177 - t2941) * t
     #177 - t19236) * t124 / 0.2E1 + (t19236 - t4410 * ((t18616 / 0.2E1 
     #- t2316 / 0.2E1) * t177 - t2962) * t177) * t124 / 0.2E1) / 0.6E1 -
     # t1046 * ((t18415 * t5492 - t18419 * t5501) * t124 + ((t8492 - t55
     #04) * t124 - (t5504 - t8797) * t124) * t124) / 0.24E2 - t1046 * ((
     #t3120 * ((t8549 / 0.2E1 - t3317 / 0.2E1) * t124 - (t3315 / 0.2E1 -
     # t8854 / 0.2E1) * t124) * t124 - t2992) * t177 / 0.2E1 + t2997 / 0
     #.2E1) / 0.6E1 - t1363 * (((t18661 - t3123) * t177 - t3125) * t177 
     #/ 0.2E1 + t3129 / 0.2E1) / 0.6E1 - t868 * ((t2949 * ((t1651 / 0.2E
     #1 - t3117 / 0.2E1) * t108 - (t1653 / 0.2E1 - t8155 / 0.2E1) * t108
     #) * t108 - t2766) * t177 / 0.2E1 + t2775 / 0.2E1) / 0.6E1 - t1046 
     #* (((t8505 - t5512) * t124 - t19308) * t124 / 0.2E1 + (t19308 - (t
     #5519 - t8810) * t124) * t124 / 0.2E1) / 0.6E1
        t19321 = t18692 * (t19100 + t19319)
        t19324 = t71 * dz
        t19332 = t17654 * t6815
        t19346 = t18717 / 0.2E1 + t6697 / 0.2E1
        t19348 = t2949 * t19346
        t19362 = t17654 * t6568
        t19380 = t3120 * t19346
        t19393 = (t18492 * t6563 - t18501 * t6566) * t108 + (t17650 * t7
     #112 - t19332) * t108 / 0.2E1 + (-t11018 * t18523 * t8149 + t19332)
     # * t108 / 0.2E1 + (t1534 * (t18707 / 0.2E1 + t6878 / 0.2E1) - t193
     #48) * t108 / 0.2E1 + (t19348 - t7623 * (t18729 / 0.2E1 + t6897 / 0
     #.2E1)) * t108 / 0.2E1 + (t14377 * t18560 * t8530 - t19362) * t124 
     #/ 0.2E1 + (-t16573 * t18571 * t8835 + t19362) * t124 / 0.2E1 + (t1
     #8589 * t6811 - t18598 * t6813) * t124 + (t7990 * (t18919 / 0.2E1 +
     # t6682 / 0.2E1) - t19380) * t124 / 0.2E1 + (t19380 - t8280 * (t189
     #34 / 0.2E1 + t6714 / 0.2E1)) * t124 / 0.2E1 + t19004 / 0.2E1 + t10
     #069 + t18970 / 0.2E1 + t10070 + t19022
        t19395 = t18692 * t10071
        t19398 = t18697 * t856
        t19400 = (t19395 - t19398) * t177
        t19403 = t19324 * ((t18479 * t19393 - t19395) * t177 / 0.2E1 + t
     #19400 / 0.2E1)
        t19405 = t1945 * t19403 / 0.8E1
        t19408 = t72 * t10073 * t177
        t19412 = t18477 * (t18700 - t19069)
        t19414 = t2378 * t19412 / 0.24E2
        t19416 = t9108 * t19062 / 0.24E2
        t19417 = t18697 * t3380
        t19419 = t2751 * t19417 / 0.2E1
        t19422 = t71 * t5525 * t177
        t19426 = t865 * t19321 / 0.2E1
        t19427 = -t18704 - t19046 - t2378 * t19049 / 0.4E1 + t3363 * t27
     #46 * t19054 - t19058 - t19061 - t2746 * t19062 / 0.24E2 + t19074 +
     # t2751 * t19321 / 0.2E1 + t19405 + t843 * t69 * t19408 / 0.6E1 - t
     #19414 + t19416 - t19419 + t843 * t68 * t19422 / 0.2E1 - t19426
        t19430 = t843 * t1944 * t19422 / 0.2E1
        t19432 = t2378 * t19072 / 0.4E1
        t19436 = t864 * t19049 / 0.4E1
        t19437 = t18697 * t7014
        t19439 = t7029 * t19437 / 0.4E1
        t19443 = t7451 * t19059 / 0.12E2
        t19445 = t864 * t19412 / 0.24E2
        t19453 = t5027 * t9057
        t19462 = t4929 ** 2
        t19463 = t4938 ** 2
        t19464 = t4945 ** 2
        t19482 = u(t73,j,t18529,n)
        t19499 = t17650 * t1655
        t19512 = t5781 ** 2
        t19513 = t5772 ** 2
        t19514 = t5776 ** 2
        t19517 = t1396 ** 2
        t19518 = t1387 ** 2
        t19519 = t1391 ** 2
        t19521 = t1408 * (t19517 + t19518 + t19519)
        t19526 = t6150 ** 2
        t19527 = t6141 ** 2
        t19528 = t6145 ** 2
        t19537 = u(t5,t121,t18529,n)
        t19545 = t1430 * t18534
        t19549 = u(t5,t126,t18529,n)
        t19559 = rx(t5,j,t18529,0,0)
        t19560 = rx(t5,j,t18529,1,1)
        t19562 = rx(t5,j,t18529,2,2)
        t19564 = rx(t5,j,t18529,1,2)
        t19566 = rx(t5,j,t18529,2,1)
        t19568 = rx(t5,j,t18529,0,1)
        t19569 = rx(t5,j,t18529,1,0)
        t19573 = rx(t5,j,t18529,2,0)
        t19575 = rx(t5,j,t18529,0,2)
        t19581 = 0.1E1 / (t19559 * t19560 * t19562 - t19559 * t19564 * t
     #19566 - t19560 * t19573 * t19575 - t19562 * t19568 * t19569 + t195
     #64 * t19568 * t19573 + t19566 * t19569 * t19575)
        t19582 = t4 * t19581
        t19611 = t19573 ** 2
        t19612 = t19566 ** 2
        t19613 = t19562 ** 2
        t18649 = (t5771 * t5781 + t5772 * t5780 + t5776 * t5787) * t5794
        t18657 = (t6140 * t6150 + t6141 * t6149 + t6145 * t6156) * t6163
        t18687 = ((t19537 - t1504) * t177 / 0.2E1 + t1738 / 0.2E1) * t57
     #94
        t18703 = ((t19549 - t1507) * t177 / 0.2E1 + t1757 / 0.2E1) * t61
     #63
        t19622 = (t4 * (t4951 * (t19462 + t19463 + t19464) / 0.2E1 + t18
     #484 / 0.2E1) * t1651 - t18493) * t108 + (t4952 * (t4929 * t4939 + 
     #t4930 * t4938 + t4934 * t4945) * t4975 - t18510) * t108 / 0.2E1 + 
     #t18519 + (t4732 * ((t19482 - t1596) * t177 / 0.2E1 + t1598 / 0.2E1
     #) - t18536) * t108 / 0.2E1 + t18546 + (t18649 * t5804 - t19499) * 
     #t124 / 0.2E1 + (-t18657 * t6173 + t19499) * t124 / 0.2E1 + (t4 * (
     #t5793 * (t19512 + t19513 + t19514) / 0.2E1 + t19521 / 0.2E1) * t15
     #06 - t4 * (t19521 / 0.2E1 + t6162 * (t19526 + t19527 + t19528) / 0
     #.2E1) * t1509) * t124 + (t18687 * t5813 - t19545) * t124 / 0.2E1 +
     # (-t18703 * t6182 + t19545) * t124 / 0.2E1 + (t19582 * (t19559 * t
     #19573 + t19562 * t19575 + t19566 * t19568) * ((t19482 - t18530) * 
     #t108 / 0.2E1 + t18653 / 0.2E1) - t1657) * t177 / 0.2E1 + t5326 + (
     #t19582 * (t19560 * t19566 + t19562 * t19564 + t19569 * t19573) * (
     #(t19537 - t18530) * t124 / 0.2E1 + (t18530 - t19549) * t124 / 0.2E
     #1) - t1513) * t177 / 0.2E1 + t5327 + (t4 * (t19581 * (t19611 + t19
     #612 + t19613) / 0.2E1 + t1413 / 0.2E1) * t18532 - t1417) * t177
        t19623 = t19622 * t1407
        t19630 = t18689 * t3053
        t19632 = (t19630 - t5524) * t177
        t19634 = t19632 / 0.2E1 + t5526 / 0.2E1
        t19636 = t743 * t19634
        t19640 = t12014 ** 2
        t19641 = t12023 ** 2
        t19642 = t12030 ** 2
        t19660 = u(t2386,j,t18529,n)
        t19673 = t12395 * t12405 + t12396 * t12404 + t12400 * t12411
        t19677 = t17661 * t8157
        t19684 = t12700 * t12710 + t12701 * t12709 + t12705 * t12716
        t19690 = t12405 ** 2
        t19691 = t12396 ** 2
        t19692 = t12400 ** 2
        t19695 = t8136 ** 2
        t19696 = t8127 ** 2
        t19697 = t8131 ** 2
        t19699 = t8148 * (t19695 + t19696 + t19697)
        t19704 = t12710 ** 2
        t19705 = t12701 ** 2
        t19706 = t12705 ** 2
        t19715 = u(t507,t121,t18529,n)
        t19719 = (t19715 - t8104) * t177 / 0.2E1 + t8106 / 0.2E1
        t19723 = t7641 * t18551
        t19727 = u(t507,t126,t18529,n)
        t19731 = (t19727 - t8116) * t177 / 0.2E1 + t8118 / 0.2E1
        t19737 = rx(t507,j,t18529,0,0)
        t19738 = rx(t507,j,t18529,1,1)
        t19740 = rx(t507,j,t18529,2,2)
        t19742 = rx(t507,j,t18529,1,2)
        t19744 = rx(t507,j,t18529,2,1)
        t19746 = rx(t507,j,t18529,0,1)
        t19747 = rx(t507,j,t18529,1,0)
        t19751 = rx(t507,j,t18529,2,0)
        t19753 = rx(t507,j,t18529,0,2)
        t19759 = 0.1E1 / (t19737 * t19738 * t19740 - t19737 * t19742 * t
     #19744 - t19738 * t19751 * t19753 - t19740 * t19746 * t19747 + t197
     #42 * t19746 * t19751 + t19744 * t19747 * t19753)
        t19760 = t4 * t19759
        t19789 = t19751 ** 2
        t19790 = t19744 ** 2
        t19791 = t19740 ** 2
        t19800 = (t18502 - t4 * (t18498 / 0.2E1 + t12036 * (t19640 + t19
     #641 + t19642) / 0.2E1) * t8155) * t108 + t18528 + (t18525 - t12037
     # * (t12014 * t12024 + t12015 * t12023 + t12019 * t12030) * t12060)
     # * t108 / 0.2E1 + t18556 + (t18553 - t11406 * ((t19660 - t8049) * 
     #t177 / 0.2E1 + t8051 / 0.2E1)) * t108 / 0.2E1 + (t12418 * t12426 *
     # t19673 - t19677) * t124 / 0.2E1 + (-t12723 * t12731 * t19684 + t1
     #9677) * t124 / 0.2E1 + (t4 * (t12417 * (t19690 + t19691 + t19692) 
     #/ 0.2E1 + t19699 / 0.2E1) * t8168 - t4 * (t19699 / 0.2E1 + t12722 
     #* (t19704 + t19705 + t19706) / 0.2E1) * t8170) * t124 + (t11773 * 
     #t19719 - t19723) * t124 / 0.2E1 + (-t12062 * t19731 + t19723) * t1
     #24 / 0.2E1 + (t19760 * (t19737 * t19751 + t19740 * t19753 + t19744
     # * t19746) * (t18655 / 0.2E1 + (t18547 - t19660) * t108 / 0.2E1) -
     # t8159) * t177 / 0.2E1 + t8162 + (t19760 * (t19738 * t19744 + t197
     #40 * t19742 + t19747 * t19751) * ((t19715 - t18547) * t124 / 0.2E1
     # + (t18547 - t19727) * t124 / 0.2E1) - t8174) * t177 / 0.2E1 + t81
     #77 + (t4 * (t19759 * (t19789 + t19790 + t19791) / 0.2E1 + t8182 / 
     #0.2E1) * t18549 - t8186) * t177
        t19801 = t19800 * t8147
        t19814 = t5027 * t9035
        t19827 = t5771 ** 2
        t19828 = t5780 ** 2
        t19829 = t5787 ** 2
        t19832 = t8507 ** 2
        t19833 = t8516 ** 2
        t19834 = t8523 ** 2
        t19836 = t8529 * (t19832 + t19833 + t19834)
        t19841 = t12395 ** 2
        t19842 = t12404 ** 2
        t19843 = t12411 ** 2
        t19855 = t17682 * t8551
        t19867 = t7976 * t18606
        t19885 = t15280 ** 2
        t19886 = t15271 ** 2
        t19887 = t15275 ** 2
        t19896 = u(i,t1047,t18529,n)
        t19906 = rx(i,t121,t18529,0,0)
        t19907 = rx(i,t121,t18529,1,1)
        t19909 = rx(i,t121,t18529,2,2)
        t19911 = rx(i,t121,t18529,1,2)
        t19913 = rx(i,t121,t18529,2,1)
        t19915 = rx(i,t121,t18529,0,1)
        t19916 = rx(i,t121,t18529,1,0)
        t19920 = rx(i,t121,t18529,2,0)
        t19922 = rx(i,t121,t18529,0,2)
        t19928 = 0.1E1 / (t19906 * t19907 * t19909 - t19906 * t19911 * t
     #19913 - t19907 * t19920 * t19922 - t19909 * t19915 * t19916 + t199
     #11 * t19915 * t19920 + t19913 * t19916 * t19922)
        t19929 = t4 * t19928
        t19958 = t19920 ** 2
        t19959 = t19913 ** 2
        t19960 = t19909 ** 2
        t19969 = (t4 * (t5793 * (t19827 + t19828 + t19829) / 0.2E1 + t19
     #836 / 0.2E1) * t5802 - t4 * (t19836 / 0.2E1 + t12417 * (t19841 + t
     #19842 + t19843) / 0.2E1) * t8536) * t108 + (t18649 * t5817 - t1985
     #5) * t108 / 0.2E1 + (-t12418 * t12439 * t19673 + t19855) * t108 / 
     #0.2E1 + (t18687 * t5798 - t19867) * t108 / 0.2E1 + (-t11767 * t197
     #19 + t19867) * t108 / 0.2E1 + (t15293 * (t15270 * t15280 + t15271 
     #* t15279 + t15275 * t15286) * t15303 - t18562) * t124 / 0.2E1 + t1
     #8567 + (t4 * (t15292 * (t19885 + t19886 + t19887) / 0.2E1 + t18581
     # / 0.2E1) * t8549 - t18590) * t124 + (t14571 * ((t19896 - t8497) *
     # t177 / 0.2E1 + t8499 / 0.2E1) - t18608) * t124 / 0.2E1 + t18613 +
     # (t19929 * (t19906 * t19920 + t19909 * t19922 + t19913 * t19915) *
     # ((t19537 - t18602) * t108 / 0.2E1 + (t18602 - t19715) * t108 / 0.
     #2E1) - t8540) * t177 / 0.2E1 + t8543 + (t19929 * (t19907 * t19913 
     #+ t19909 * t19911 + t19916 * t19920) * ((t19896 - t18602) * t124 /
     # 0.2E1 + t18668 / 0.2E1) - t8553) * t177 / 0.2E1 + t8556 + (t4 * (
     #t19928 * (t19958 + t19959 + t19960) / 0.2E1 + t8561 / 0.2E1) * t18
     #604 - t8565) * t177
        t19970 = t19969 * t8528
        t19978 = t796 * t19634
        t19982 = t6140 ** 2
        t19983 = t6149 ** 2
        t19984 = t6156 ** 2
        t19987 = t8812 ** 2
        t19988 = t8821 ** 2
        t19989 = t8828 ** 2
        t19991 = t8834 * (t19987 + t19988 + t19989)
        t19996 = t12700 ** 2
        t19997 = t12709 ** 2
        t19998 = t12716 ** 2
        t20010 = t17688 * t8856
        t20022 = t8266 * t18618
        t20040 = t17445 ** 2
        t20041 = t17436 ** 2
        t20042 = t17440 ** 2
        t20051 = u(i,t1111,t18529,n)
        t20061 = rx(i,t126,t18529,0,0)
        t20062 = rx(i,t126,t18529,1,1)
        t20064 = rx(i,t126,t18529,2,2)
        t20066 = rx(i,t126,t18529,1,2)
        t20068 = rx(i,t126,t18529,2,1)
        t20070 = rx(i,t126,t18529,0,1)
        t20071 = rx(i,t126,t18529,1,0)
        t20075 = rx(i,t126,t18529,2,0)
        t20077 = rx(i,t126,t18529,0,2)
        t20083 = 0.1E1 / (t20061 * t20062 * t20064 - t20061 * t20066 * t
     #20068 - t20062 * t20075 * t20077 - t20064 * t20070 * t20071 + t200
     #66 * t20070 * t20075 + t20068 * t20071 * t20077)
        t20084 = t4 * t20083
        t20113 = t20075 ** 2
        t20114 = t20068 ** 2
        t20115 = t20064 ** 2
        t20124 = (t4 * (t6162 * (t19982 + t19983 + t19984) / 0.2E1 + t19
     #991 / 0.2E1) * t6171 - t4 * (t19991 / 0.2E1 + t12722 * (t19996 + t
     #19997 + t19998) / 0.2E1) * t8841) * t108 + (t18657 * t6186 - t2001
     #0) * t108 / 0.2E1 + (-t12723 * t12744 * t19684 + t20010) * t108 / 
     #0.2E1 + (t18703 * t6167 - t20022) * t108 / 0.2E1 + (-t12056 * t197
     #31 + t20022) * t108 / 0.2E1 + t18576 + (t18573 - t17458 * (t17435 
     #* t17445 + t17436 * t17444 + t17440 * t17451) * t17468) * t124 / 0
     #.2E1 + (t18599 - t4 * (t18595 / 0.2E1 + t17457 * (t20040 + t20041 
     #+ t20042) / 0.2E1) * t8854) * t124 + t18623 + (t18620 - t16579 * (
     #(t20051 - t8802) * t177 / 0.2E1 + t8804 / 0.2E1)) * t124 / 0.2E1 +
     # (t20084 * (t20061 * t20075 + t20064 * t20077 + t20068 * t20070) *
     # ((t19549 - t18614) * t108 / 0.2E1 + (t18614 - t19727) * t108 / 0.
     #2E1) - t8845) * t177 / 0.2E1 + t8848 + (t20084 * (t20062 * t20068 
     #+ t20064 * t20066 + t20071 * t20075) * (t18670 / 0.2E1 + (t18614 -
     # t20051) * t124 / 0.2E1) - t8858) * t177 / 0.2E1 + t8861 + (t4 * (
     #t20083 * (t20113 + t20114 + t20115) / 0.2E1 + t8866 / 0.2E1) * t18
     #616 - t8870) * t177
        t20125 = t20124 * t8833
        t20160 = (t5245 * t6397 - t5440 * t9033) * t108 + (t4500 * t6423
     # - t19453) * t108 / 0.2E1 + (-t12323 * t5447 + t19453) * t108 / 0.
     #2E1 + (t396 * ((t19623 - t5329) * t177 / 0.2E1 + t5331 / 0.2E1) - 
     #t19636) * t108 / 0.2E1 + (t19636 - t2484 * ((t19801 - t8190) * t17
     #7 / 0.2E1 + t8192 / 0.2E1)) * t108 / 0.2E1 + (t14814 * t5463 - t19
     #814) * t124 / 0.2E1 + (-t16807 * t5474 + t19814) * t124 / 0.2E1 + 
     #(t5492 * t9053 - t5501 * t9055) * t124 + (t4276 * ((t19970 - t8569
     #) * t177 / 0.2E1 + t8571 / 0.2E1) - t19978) * t124 / 0.2E1 + (t199
     #78 - t4410 * ((t20125 - t8874) * t177 / 0.2E1 + t8876 / 0.2E1)) * 
     #t124 / 0.2E1 + (t2949 * ((t19623 - t19630) * t108 / 0.2E1 + (t1963
     #0 - t19801) * t108 / 0.2E1) - t9037) * t177 / 0.2E1 + t9042 + (t31
     #20 * ((t19970 - t19630) * t124 / 0.2E1 + (t19630 - t20125) * t124 
     #/ 0.2E1) - t9059) * t177 / 0.2E1 + t9064 + (t19632 * t3062 - t9076
     #) * t177
        t20161 = t18692 * t20160
        t20165 = t865 * t19417 / 0.2E1
        t20166 = t19066 * t10124
        t20168 = (t19398 - t20166) * t177
        t20171 = t19324 * (t19400 / 0.2E1 + t20168 / 0.2E1)
        t20173 = t7018 * t20171 / 0.8E1
        t20175 = t6450 * t19437 / 0.4E1
        t20177 = t1945 * t20171 / 0.8E1
        t20179 = t7451 * t20161 / 0.12E2
        t20182 = t843 * t7449 * t19408 / 0.6E1
        t20185 = -t19430 - t19432 + t2378 * t18702 / 0.24E2 + t19436 - t
     #19439 - t7018 * t19403 / 0.8E1 + t19443 + t19445 + t3386 * t20161 
     #/ 0.12E2 + t20165 - t20173 + t20175 + t20177 - t20179 - t20182 + t
     #7029 * t19044 / 0.4E1
        t20187 = (t19427 + t20185) * t65
        t20193 = t3363 * (t939 - dz * t3022 / 0.24E2)
        t20194 = t18704 + t19046 + t19058 - t19074 + t20193 - t19405 - t
     #19416 + t19426 + t19430 - t19436 - t19443 - t19445
        t20197 = t9235 * t18696 * t2
        t20198 = t20197 / 0.2E1
        t20201 = cc * t745 * t18691 * t208
        t20202 = t20201 / 0.2E1
        t20204 = (-t20197 + t20201) * t177
        t20207 = cc * t784 * t19065 * t211
        t20209 = (t20197 - t20207) * t177
        t20211 = (t20204 - t20209) * t177
        t20214 = cc * t3054 * t18478 * t6561
        t20216 = (-t20201 + t20214) * t177
        t20218 = (t20216 - t20204) * t177
        t20220 = (t20218 - t20211) * t177
        t20222 = sqrt(t3094)
        t20224 = cc * t3090 * t20222 * t6580
        t20226 = (-t20224 + t20207) * t177
        t20228 = (t20209 - t20226) * t177
        t20230 = (t20211 - t20228) * t177
        t20236 = t1363 * (t20211 - dz * (t20220 - t20230) / 0.12E2) / 0.
     #24E2
        t20237 = t20204 / 0.2E1
        t20238 = t20209 / 0.2E1
        t20245 = dz * (t20237 + t20238 - t1363 * (t20220 / 0.2E1 + t2023
     #0 / 0.2E1) / 0.6E1) / 0.4E1
        t20247 = sqrt(t18681)
        t20255 = (((cc * t18646 * t18715 * t20247 - t20214) * t177 - t20
     #216) * t177 - t20218) * t177
        t20261 = t1363 * (t20218 - dz * (t20255 - t20220) / 0.12E2) / 0.
     #24E2
        t20269 = dz * (t20216 / 0.2E1 + t20237 - t1363 * (t20255 / 0.2E1
     # + t20220 / 0.2E1) / 0.6E1) / 0.4E1
        t20271 = dz * t3066 / 0.24E2
        t20272 = -t20187 * t863 - t20165 - t20175 - t20177 + t20179 + t2
     #0182 - t20198 + t20202 - t20236 - t20245 + t20261 - t20269 - t2027
     #1
        t20287 = t4 * (t9796 + t18120 / 0.2E1 - dz * ((t18115 - t9795) *
     # t177 / 0.2E1 - (-t3090 * t3134 + t18120) * t177 / 0.2E1) / 0.8E1)
        t20298 = (t6582 - t6585) * t108
        t20315 = t13513 + t13514 - t13518 + t440 / 0.4E1 + t791 / 0.4E1 
     #- t18180 / 0.12E2 - dz * ((t18161 + t18162 - t18163 - t13540 - t13
     #541 + t13542) * t177 / 0.2E1 - (t18166 + t18167 - t18181 - t6582 /
     # 0.2E1 - t6585 / 0.2E1 + t868 * (((t7320 - t6582) * t108 - t20298)
     # * t108 / 0.2E1 + (t20298 - (t6585 - t10927) * t108) * t108 / 0.2E
     #1) / 0.6E1) * t177 / 0.2E1) / 0.8E1
        t20320 = t4 * (t9795 / 0.2E1 + t18120 / 0.2E1)
        t20322 = t3488 / 0.4E1 + t7456 / 0.4E1 + t6410 / 0.4E1 + t9044 /
     # 0.4E1
        t20331 = t13580 / 0.4E1 + t13581 / 0.4E1 + (t10019 - t10125) * t
     #108 / 0.4E1 + (t10125 - t13416) * t108 / 0.4E1
        t20337 = dz * (t760 / 0.2E1 - t6591 / 0.2E1)
        t20341 = t20287 * t9108 * t20315
        t20344 = t20320 * t9711 * t20322 / 0.2E1
        t20347 = t20320 * t9715 * t20331 / 0.6E1
        t20349 = t9108 * t20337 / 0.24E2
        t20351 = (t20287 * t2746 * t20315 + t20320 * t9402 * t20322 / 0.
     #2E1 + t20320 * t9408 * t20331 / 0.6E1 - t2746 * t20337 / 0.24E2 - 
     #t20341 - t20344 - t20347 + t20349) * t65
        t20364 = (t1679 - t3137) * t108
        t20382 = t20287 * (t13622 + t13623 - t13627 + t1341 / 0.4E1 + t1
     #348 / 0.4E1 - t18278 / 0.12E2 - dz * ((t18259 + t18260 - t18261 - 
     #t13649 - t13650 + t13651) * t177 / 0.2E1 - (t18264 + t18265 - t182
     #79 - t1679 / 0.2E1 - t3137 / 0.2E1 + t868 * (((t1677 - t1679) * t1
     #08 - t20364) * t108 / 0.2E1 + (t20364 - (t3137 - t8353) * t108) * 
     #t108 / 0.2E1) / 0.6E1) * t177 / 0.2E1) / 0.8E1)
        t20386 = dz * (t2334 / 0.2E1 - t3143 / 0.2E1) / 0.24E2
        t20402 = t4 * (t15674 + t18301 / 0.2E1 - dz * ((t18296 - t15673)
     # * t177 / 0.2E1 - (-t3090 * t3333 + t18301) * t177 / 0.2E1) / 0.8E
     #1)
        t20413 = (t6827 - t6829) * t124
        t20430 = t9344 + t9345 - t9349 + t821 / 0.4E1 + t823 / 0.4E1 - t
     #18361 / 0.12E2 - dz * ((t18342 + t18343 - t18344 - t9371 - t9372 +
     # t9373) * t177 / 0.2E1 - (t18347 + t18348 - t18362 - t6827 / 0.2E1
     # - t6829 / 0.2E1 + t1046 * (((t14319 - t6827) * t124 - t20413) * t
     #124 / 0.2E1 + (t20413 - (t6829 - t16514) * t124) * t124 / 0.2E1) /
     # 0.6E1) * t177 / 0.2E1) / 0.8E1
        t20435 = t4 * (t15673 / 0.2E1 + t18301 / 0.2E1)
        t20437 = t4592 / 0.4E1 + t4750 / 0.4E1 + t9066 / 0.4E1 + t9068 /
     # 0.4E1
        t20446 = t9637 / 0.4E1 + t9698 / 0.4E1 + (t15842 - t10125) * t12
     #4 / 0.4E1 + (t10125 - t18048) * t124 / 0.4E1
        t20452 = dz * (t814 / 0.2E1 - t6835 / 0.2E1)
        t20456 = t20402 * t9108 * t20430
        t20459 = t20435 * t9711 * t20437 / 0.2E1
        t20462 = t20435 * t9715 * t20446 / 0.6E1
        t20464 = t9108 * t20452 / 0.24E2
        t20466 = (t20402 * t2746 * t20430 + t20435 * t9402 * t20437 / 0.
     #2E1 + t20435 * t9408 * t20446 / 0.6E1 - t2746 * t20452 / 0.24E2 - 
     #t20456 - t20459 - t20462 + t20464) * t65
        t20479 = (t3335 - t3337) * t124
        t20497 = t20402 * (t9731 + t9732 - t9736 + t2357 / 0.4E1 + t2359
     # / 0.4E1 - t18459 / 0.12E2 - dz * ((t18440 + t18441 - t18442 - t97
     #58 - t9759 + t9760) * t177 / 0.2E1 - (t18445 + t18446 - t18460 - t
     #3335 / 0.2E1 - t3337 / 0.2E1 + t1046 * (((t8697 - t3335) * t124 - 
     #t20479) * t124 / 0.2E1 + (t20479 - (t3337 - t9002) * t124) * t124 
     #/ 0.2E1) / 0.6E1) * t177 / 0.2E1) / 0.8E1)
        t20501 = dz * (t2354 / 0.2E1 - t3343 / 0.2E1) / 0.24E2
        t20506 = cc * t20222
        t20507 = t1426 ** 2
        t20508 = t1435 ** 2
        t20509 = t1442 ** 2
        t20511 = t1448 * (t20507 + t20508 + t20509)
        t20512 = t3068 ** 2
        t20513 = t3077 ** 2
        t20514 = t3084 ** 2
        t20516 = t3090 * (t20512 + t20513 + t20514)
        t20519 = t4 * (t20511 / 0.2E1 + t20516 / 0.2E1)
        t20520 = t20519 * t1679
        t20521 = t8324 ** 2
        t20522 = t8333 ** 2
        t20523 = t8340 ** 2
        t20525 = t8346 * (t20521 + t20522 + t20523)
        t20528 = t4 * (t20516 / 0.2E1 + t20525 / 0.2E1)
        t20529 = t20528 * t3137
        t19531 = t1544 * (t1426 * t1436 + t1427 * t1435 + t1431 * t1442)
        t20537 = t19531 * t1556
        t19535 = t3130 * (t3068 * t3078 + t3069 * t3077 + t3073 * t3084)
        t20543 = t19535 * t3339
        t20546 = (t20537 - t20543) * t108 / 0.2E1
        t20550 = t8324 * t8334 + t8325 * t8333 + t8329 * t8340
        t19543 = t8347 * t20550
        t20552 = t19543 * t8370
        t20555 = (t20543 - t20552) * t108 / 0.2E1
        t20556 = k - 3
        t20557 = u(t5,j,t20556,n)
        t20559 = (t1376 - t20557) * t177
        t20561 = t1378 / 0.2E1 + t20559 / 0.2E1
        t20563 = t1555 * t20561
        t20564 = u(i,j,t20556,n)
        t20566 = (t1630 - t20564) * t177
        t20568 = t1632 / 0.2E1 + t20566 / 0.2E1
        t20570 = t2967 * t20568
        t20573 = (t20563 - t20570) * t108 / 0.2E1
        t20574 = u(t507,j,t20556,n)
        t20576 = (t3135 - t20574) * t177
        t20578 = t3194 / 0.2E1 + t20576 / 0.2E1
        t20580 = t7809 * t20578
        t20583 = (t20570 - t20580) * t108 / 0.2E1
        t20587 = t8655 * t8665 + t8656 * t8664 + t8660 * t8671
        t19567 = t8678 * t20587
        t20589 = t19567 * t8686
        t20591 = t19535 * t3139
        t20594 = (t20589 - t20591) * t124 / 0.2E1
        t20598 = t8960 * t8970 + t8961 * t8969 + t8965 * t8976
        t19577 = t8983 * t20598
        t20600 = t19577 * t8991
        t20603 = (t20591 - t20600) * t124 / 0.2E1
        t20604 = t8665 ** 2
        t20605 = t8656 ** 2
        t20606 = t8660 ** 2
        t20608 = t8677 * (t20604 + t20605 + t20606)
        t20609 = t3078 ** 2
        t20610 = t3069 ** 2
        t20611 = t3073 ** 2
        t20613 = t3090 * (t20609 + t20610 + t20611)
        t20616 = t4 * (t20608 / 0.2E1 + t20613 / 0.2E1)
        t20617 = t20616 * t3335
        t20618 = t8970 ** 2
        t20619 = t8961 ** 2
        t20620 = t8965 ** 2
        t20622 = t8982 * (t20618 + t20619 + t20620)
        t20625 = t4 * (t20613 / 0.2E1 + t20622 / 0.2E1)
        t20626 = t20625 * t3337
        t20629 = u(i,t121,t20556,n)
        t20631 = (t2942 - t20629) * t177
        t20633 = t2944 / 0.2E1 + t20631 / 0.2E1
        t20635 = t8134 * t20633
        t20637 = t3142 * t20568
        t20640 = (t20635 - t20637) * t124 / 0.2E1
        t20641 = u(i,t126,t20556,n)
        t20643 = (t2963 - t20641) * t177
        t20645 = t2965 / 0.2E1 + t20643 / 0.2E1
        t20647 = t8427 * t20645
        t20650 = (t20637 - t20647) * t124 / 0.2E1
        t20651 = rx(i,j,t20556,0,0)
        t20652 = rx(i,j,t20556,1,1)
        t20654 = rx(i,j,t20556,2,2)
        t20656 = rx(i,j,t20556,1,2)
        t20658 = rx(i,j,t20556,2,1)
        t20660 = rx(i,j,t20556,0,1)
        t20661 = rx(i,j,t20556,1,0)
        t20665 = rx(i,j,t20556,2,0)
        t20667 = rx(i,j,t20556,0,2)
        t20673 = 0.1E1 / (t20651 * t20652 * t20654 - t20651 * t20656 * t
     #20658 - t20652 * t20665 * t20667 - t20654 * t20660 * t20661 + t206
     #56 * t20660 * t20665 + t20658 * t20661 * t20667)
        t20674 = t4 * t20673
        t20680 = (t20557 - t20564) * t108
        t20682 = (t20564 - t20574) * t108
        t19618 = t20674 * (t20651 * t20665 + t20654 * t20667 + t20658 * 
     #t20660)
        t20688 = (t3141 - t19618 * (t20680 / 0.2E1 + t20682 / 0.2E1)) * 
     #t177
        t20695 = (t20629 - t20564) * t124
        t20697 = (t20564 - t20641) * t124
        t19631 = t20674 * (t20652 * t20658 + t20654 * t20656 + t20661 * 
     #t20665)
        t20703 = (t3341 - t19631 * (t20695 / 0.2E1 + t20697 / 0.2E1)) * 
     #t177
        t20705 = t20665 ** 2
        t20706 = t20658 ** 2
        t20707 = t20654 ** 2
        t20708 = t20705 + t20706 + t20707
        t20709 = t20673 * t20708
        t20712 = t4 * (t3095 / 0.2E1 + t20709 / 0.2E1)
        t20715 = (-t20566 * t20712 + t3099) * t177
        t20716 = (t20520 - t20529) * t108 + t20546 + t20555 + t20573 + t
     #20583 + t20594 + t20603 + (t20617 - t20626) * t124 + t20640 + t206
     #50 + t5615 + t20688 / 0.2E1 + t5616 + t20703 / 0.2E1 + t20715
        t20719 = (-t20506 * t20716 + t19067) * t177
        t20721 = t18477 * (t19069 - t20719)
        t20723 = t864 * t20721 / 0.24E2
        t20731 = t19535 * t6831
        t20740 = ut(t5,j,t20556,n)
        t20742 = (t6579 - t20740) * t177
        t20747 = ut(i,j,t20556,n)
        t20749 = (t6580 - t20747) * t177
        t20751 = t6702 / 0.2E1 + t20749 / 0.2E1
        t20753 = t2967 * t20751
        t20757 = ut(t507,j,t20556,n)
        t20759 = (t6583 - t20757) * t177
        t20770 = t19535 * t6587
        t20783 = ut(i,t121,t20556,n)
        t20785 = (t6686 - t20783) * t177
        t20791 = t3142 * t20751
        t20795 = ut(i,t126,t20556,n)
        t20797 = (t6718 - t20795) * t177
        t20814 = (t6589 - t19618 * ((t20740 - t20747) * t108 / 0.2E1 + (
     #t20747 - t20757) * t108 / 0.2E1)) * t177
        t20825 = (t6833 - t19631 * ((t20783 - t20747) * t124 / 0.2E1 + (
     #t20747 - t20795) * t124 / 0.2E1)) * t177
        t20829 = (-t20712 * t20749 + t6799) * t177
        t20830 = (t20519 * t6582 - t20528 * t6585) * t108 + (t19531 * t7
     #130 - t20731) * t108 / 0.2E1 + (-t11034 * t20550 * t8347 + t20731)
     # * t108 / 0.2E1 + (t1555 * (t6883 / 0.2E1 + t20742 / 0.2E1) - t207
     #53) * t108 / 0.2E1 + (t20753 - t7809 * (t6902 / 0.2E1 + t20759 / 0
     #.2E1)) * t108 / 0.2E1 + (t14393 * t20587 * t8678 - t20770) * t124 
     #/ 0.2E1 + (-t16589 * t20598 * t8983 + t20770) * t124 / 0.2E1 + (t2
     #0616 * t6827 - t20625 * t6829) * t124 + (t8134 * (t6688 / 0.2E1 + 
     #t20785 / 0.2E1) - t20791) * t124 / 0.2E1 + (t20791 - t8427 * (t672
     #0 / 0.2E1 + t20797 / 0.2E1)) * t124 / 0.2E1 + t10122 + t20814 / 0.
     #2E1 + t10123 + t20825 / 0.2E1 + t20829
        t20836 = t19324 * (t20168 / 0.2E1 + (-t20506 * t20830 + t20166) 
     #* t177 / 0.2E1)
        t20838 = t1945 * t20836 / 0.8E1
        t20841 = t18477 * (t19069 / 0.2E1 + t20719 / 0.2E1)
        t20843 = t864 * t20841 / 0.4E1
        t20847 = (t10113 - t10120) * t124
        t19776 = (t6705 - (t213 / 0.2E1 - t20749 / 0.2E1) * t177) * t177
        t20871 = t811 * t19776
        t20903 = t4 * (t3365 + t3095 / 0.2E1 - dz * (t3357 / 0.2E1 - (t3
     #095 - t20709) * t177 / 0.2E1) / 0.8E1)
        t20942 = t5081 * t6288
        t20968 = t5336 / 0.2E1
        t20978 = t4 * (t5037 / 0.2E1 + t20968 - dx * ((t5028 - t5037) * 
     #t108 / 0.2E1 - (t5336 - t5531) * t108 / 0.2E1) / 0.8E1)
        t20990 = t4 * (t20968 + t5531 / 0.2E1 - dx * ((t5037 - t5336) * 
     #t108 / 0.2E1 - (t5531 - t8220) * t108 / 0.2E1) / 0.8E1)
        t20994 = t10122 + t10123 + t798 + t830 - t1046 * (((t15837 - t10
     #113) * t124 - t20847) * t124 / 0.2E1 + (t20847 - (t10120 - t18043)
     # * t124) * t124 / 0.2E1) / 0.6E1 - t1363 * ((t4289 * (t6691 - (t69
     #2 / 0.2E1 - t20785 / 0.2E1) * t177) * t177 - t20871) * t124 / 0.2E
     #1 + (t20871 - t4426 * (t6723 - (t715 / 0.2E1 - t20797 / 0.2E1) * t
     #177) * t177) * t124 / 0.2E1) / 0.6E1 - t1363 * (t6839 / 0.2E1 + (t
     #6837 - (t6835 - t20825) * t177) * t177 / 0.2E1) / 0.6E1 + (-t20903
     # * t6702 + t6845) * t177 - t1046 * (t6651 / 0.2E1 + (t6649 - t3142
     # * ((t14319 / 0.2E1 - t6829 / 0.2E1) * t124 - (t6827 / 0.2E1 - t16
     #514 / 0.2E1) * t124) * t124) * t177 / 0.2E1) / 0.6E1 - t1363 * ((t
     #6791 - t3098 * (t6788 - (t6702 - t20749) * t177) * t177) * t177 + 
     #(t6803 - (t6801 - t20829) * t177) * t177) / 0.24E2 - t1046 * ((t50
     #69 * t7096 * t7387 - t20942) * t108 / 0.2E1 + (-t10546 * t5187 + t
     #20942) * t108 / 0.2E1) / 0.6E1 - t868 * ((t18173 * t5339 - t18177 
     #* t5534) * t108 + ((t9953 - t10077) * t108 - (t10077 - t13368) * t
     #108) * t108) / 0.24E2 + (t20978 * t440 - t20990 * t791) * t108 + t
     #10114 + t10121
        t20998 = (t9982 - t10088) * t108
        t21018 = t781 * t19776
        t21037 = (t9964 - t10081) * t108
        t21064 = (t10095 - t10100) * t124
        t21076 = t5583 / 0.2E1
        t21086 = t4 * (t5578 / 0.2E1 + t21076 - dy * ((t8634 - t5578) * 
     #t124 / 0.2E1 - (t5583 - t5592) * t124 / 0.2E1) / 0.8E1)
        t21098 = t4 * (t21076 + t5592 / 0.2E1 - dy * ((t5578 - t5583) * 
     #t124 / 0.2E1 - (t5592 - t8939) * t124 / 0.2E1) / 0.8E1)
        t21131 = t5081 * t6200
        t21143 = t10089 + t10096 + t10101 + t10082 - t868 * (((t9975 - t
     #9982) * t108 - t20998) * t108 / 0.2E1 + (t20998 - (t10088 - t13379
     #) * t108) * t108 / 0.2E1) / 0.6E1 - t1363 * ((t435 * (t6886 - (t19
     #6 / 0.2E1 - t20742 / 0.2E1) * t177) * t177 - t21018) * t108 / 0.2E
     #1 + (t21018 - t2520 * (t6905 - (t572 / 0.2E1 - t20759 / 0.2E1) * t
     #177) * t177) * t108 / 0.2E1) / 0.6E1 - t868 * (((t9959 - t9964) * 
     #t108 - t21037) * t108 / 0.2E1 + (t21037 - (t10081 - t13372) * t108
     #) * t108 / 0.2E1) / 0.6E1 - t1046 * ((t18354 * t5586 - t18358 * t5
     #595) * t124 + ((t15831 - t10105) * t124 - (t10105 - t18037) * t124
     #) * t124) / 0.24E2 - t1046 * (((t15827 - t10095) * t124 - t21064) 
     #* t124 / 0.2E1 + (t21064 - (t10100 - t18033) * t124) * t124 / 0.2E
     #1) / 0.6E1 + (t21086 * t821 - t21098 * t823) * t124 - t1363 * (t65
     #95 / 0.2E1 + (t6593 - (t6591 - t20814) * t177) * t177 / 0.2E1) / 0
     #.6E1 - t868 * (t6493 / 0.2E1 + (t6491 - t2967 * ((t7320 / 0.2E1 - 
     #t6585 / 0.2E1) * t108 - (t6582 / 0.2E1 - t10927 / 0.2E1) * t108) *
     # t108) * t177 / 0.2E1) / 0.6E1 + t9983 + t9965 - t868 * ((t13826 *
     # t5197 - t21131) * t124 / 0.2E1 + (-t16058 * t5202 + t21131) * t12
     #4 / 0.2E1) / 0.6E1
        t21145 = t19066 * (t20994 + t21143)
        t21155 = (t5606 - t5613) * t124
        t20105 = (t1635 - (t942 / 0.2E1 - t20566 / 0.2E1) * t177) * t177
        t21179 = t811 * t20105
        t21208 = t5351 + t2342 + t2366 + t5358 + t5546 + t5553 + t5564 +
     # t5573 + t5616 + t5607 + t5614 + t5615 - t1046 * (((t8653 - t5606)
     # * t124 - t21155) * t124 / 0.2E1 + (t21155 - (t5613 - t8958) * t12
     #4) * t124 / 0.2E1) / 0.6E1 - t1363 * ((t4289 * (t2947 - (t2304 / 0
     #.2E1 - t20631 / 0.2E1) * t177) * t177 - t21179) * t124 / 0.2E1 + (
     #t21179 - t4426 * (t2968 - (t2319 / 0.2E1 - t20643 / 0.2E1) * t177)
     # * t177) * t124 / 0.2E1) / 0.6E1 - t1046 * ((t18452 * t5586 - t184
     #56 * t5595) * t124 + ((t8640 - t5598) * t124 - (t5598 - t8945) * t
     #124) * t124) / 0.24E2
        t21260 = t5081 * t2788
        t21297 = t5081 * t2615
        t21312 = (t5357 - t5552) * t108
        t21332 = t781 * t20105
        t21351 = (t5350 - t5545) * t108
        t21369 = (t5563 - t5572) * t124
        t21396 = -t1363 * (t3347 / 0.2E1 + (t3345 - (t3343 - t20703) * t
     #177) * t177 / 0.2E1) / 0.6E1 - t1046 * (t3013 / 0.2E1 + (t3011 - t
     #3142 * ((t8697 / 0.2E1 - t3337 / 0.2E1) * t124 - (t3335 / 0.2E1 - 
     #t9002 / 0.2E1) * t124) * t124) * t177 / 0.2E1) / 0.6E1 - t1363 * (
     #t3147 / 0.2E1 + (t3145 - (t3143 - t20688) * t177) * t177 / 0.2E1) 
     #/ 0.6E1 - t1363 * ((t3029 - t3098 * (t3026 - (t1632 - t20566) * t1
     #77) * t177) * t177 + (t3103 - (t3101 - t20715) * t177) * t177) / 0
     #.24E2 - t1046 * ((t1813 * t4823 - t21260) * t108 / 0.2E1 + (-t1000
     #9 * t5187 + t21260) * t108 / 0.2E1) / 0.6E1 + (-t1632 * t20903 + t
     #3376) * t177 - t868 * ((t18271 * t5339 - t18275 * t5534) * t108 + 
     #((t5342 - t5537) * t108 - (t5537 - t8226) * t108) * t108) / 0.24E2
     # + (t1341 * t20978 - t1348 * t20990) * t108 - t868 * ((t13385 * t5
     #197 - t21297) * t124 / 0.2E1 + (-t15539 * t5202 + t21297) * t124 /
     # 0.2E1) / 0.6E1 - t868 * (((t5098 - t5357) * t108 - t21312) * t108
     # / 0.2E1 + (t21312 - (t5552 - t8255) * t108) * t108 / 0.2E1) / 0.6
     #E1 - t1363 * ((t435 * (t1617 - (t928 / 0.2E1 - t20559 / 0.2E1) * t
     #177) * t177 - t21332) * t108 / 0.2E1 + (t21332 - t2520 * (t3197 - 
     #(t959 / 0.2E1 - t20576 / 0.2E1) * t177) * t177) * t108 / 0.2E1) / 
     #0.6E1 - t868 * (((t5073 - t5350) * t108 - t21351) * t108 / 0.2E1 +
     # (t21351 - (t5545 - t8241) * t108) * t108 / 0.2E1) / 0.6E1 + (t210
     #86 * t2357 - t21098 * t2359) * t124 - t1046 * (((t8628 - t5563) * 
     #t124 - t21369) * t124 / 0.2E1 + (t21369 - (t5572 - t8933) * t124) 
     #* t124 / 0.2E1) / 0.6E1 - t868 * (t2784 / 0.2E1 + (t2782 - t2967 *
     # ((t1677 / 0.2E1 - t3137 / 0.2E1) * t108 - (t1679 / 0.2E1 - t8353 
     #/ 0.2E1) * t108) * t108) * t177 / 0.2E1) / 0.6E1
        t21398 = t19066 * (t21208 + t21396)
        t21400 = t865 * t21398 / 0.2E1
        t21402 = t6450 * t21145 / 0.4E1
        t21410 = t5081 * t9070
        t21419 = t5167 ** 2
        t21420 = t5176 ** 2
        t21421 = t5183 ** 2
        t21439 = u(t73,j,t20556,n)
        t21456 = t19531 * t1681
        t21469 = t5961 ** 2
        t21470 = t5952 ** 2
        t21471 = t5956 ** 2
        t21474 = t1436 ** 2
        t21475 = t1427 ** 2
        t21476 = t1431 ** 2
        t21478 = t1448 * (t21474 + t21475 + t21476)
        t21483 = t6330 ** 2
        t21484 = t6321 ** 2
        t21485 = t6325 ** 2
        t21494 = u(t5,t121,t20556,n)
        t21502 = t1462 * t20561
        t21506 = u(t5,t126,t20556,n)
        t21516 = rx(t5,j,t20556,0,0)
        t21517 = rx(t5,j,t20556,1,1)
        t21519 = rx(t5,j,t20556,2,2)
        t21521 = rx(t5,j,t20556,1,2)
        t21523 = rx(t5,j,t20556,2,1)
        t21525 = rx(t5,j,t20556,0,1)
        t21526 = rx(t5,j,t20556,1,0)
        t21530 = rx(t5,j,t20556,2,0)
        t21532 = rx(t5,j,t20556,0,2)
        t21538 = 0.1E1 / (t21516 * t21517 * t21519 - t21516 * t21521 * t
     #21523 - t21517 * t21530 * t21532 - t21519 * t21525 * t21526 + t215
     #21 * t21525 * t21530 + t21523 * t21526 * t21532)
        t21539 = t4 * t21538
        t21568 = t21530 ** 2
        t21569 = t21523 ** 2
        t21570 = t21519 ** 2
        t20433 = (t5951 * t5961 + t5952 * t5960 + t5956 * t5967) * t5974
        t20440 = (t6320 * t6330 + t6321 * t6329 + t6325 * t6336) * t6343
        t20465 = (t1743 / 0.2E1 + (t1549 - t21494) * t177 / 0.2E1) * t59
     #74
        t20471 = (t1762 / 0.2E1 + (t1552 - t21506) * t177 / 0.2E1) * t63
     #43
        t21579 = (t4 * (t5189 * (t21419 + t21420 + t21421) / 0.2E1 + t20
     #511 / 0.2E1) * t1677 - t20520) * t108 + (t5190 * (t5167 * t5177 + 
     #t5168 * t5176 + t5172 * t5183) * t5213 - t20537) * t108 / 0.2E1 + 
     #t20546 + (t4972 * (t1604 / 0.2E1 + (t1602 - t21439) * t177 / 0.2E1
     #) - t20563) * t108 / 0.2E1 + t20573 + (t20433 * t5984 - t21456) * 
     #t124 / 0.2E1 + (-t20440 * t6353 + t21456) * t124 / 0.2E1 + (t4 * (
     #t5973 * (t21469 + t21470 + t21471) / 0.2E1 + t21478 / 0.2E1) * t15
     #51 - t4 * (t21478 / 0.2E1 + t6342 * (t21483 + t21484 + t21485) / 0
     #.2E1) * t1554) * t124 + (t20465 * t5993 - t21502) * t124 / 0.2E1 +
     # (-t20471 * t6362 + t21502) * t124 / 0.2E1 + t5420 + (t1683 - t215
     #39 * (t21516 * t21530 + t21519 * t21532 + t21523 * t21525) * ((t21
     #439 - t20557) * t108 / 0.2E1 + t20680 / 0.2E1)) * t177 / 0.2E1 + t
     #5421 + (t1558 - t21539 * (t21517 * t21523 + t21519 * t21521 + t215
     #26 * t21530) * ((t21494 - t20557) * t124 / 0.2E1 + (t20557 - t2150
     #6) * t124 / 0.2E1)) * t177 / 0.2E1 + (t1457 - t4 * (t1453 / 0.2E1 
     #+ t21538 * (t21568 + t21569 + t21570) / 0.2E1) * t20559) * t177
        t21580 = t21579 * t1447
        t21587 = t20716 * t3089
        t21589 = (t5618 - t21587) * t177
        t21591 = t5620 / 0.2E1 + t21589 / 0.2E1
        t21593 = t781 * t21591
        t21597 = t12212 ** 2
        t21598 = t12221 ** 2
        t21599 = t12228 ** 2
        t21617 = u(t2386,j,t20556,n)
        t21630 = t12543 * t12553 + t12544 * t12552 + t12548 * t12559
        t21634 = t19543 * t8355
        t21641 = t12848 * t12858 + t12849 * t12857 + t12853 * t12864
        t21647 = t12553 ** 2
        t21648 = t12544 ** 2
        t21649 = t12548 ** 2
        t21652 = t8334 ** 2
        t21653 = t8325 ** 2
        t21654 = t8329 ** 2
        t21656 = t8346 * (t21652 + t21653 + t21654)
        t21661 = t12858 ** 2
        t21662 = t12849 ** 2
        t21663 = t12853 ** 2
        t21672 = u(t507,t121,t20556,n)
        t21676 = t8304 / 0.2E1 + (t8302 - t21672) * t177 / 0.2E1
        t21680 = t7824 * t20578
        t21684 = u(t507,t126,t20556,n)
        t21688 = t8316 / 0.2E1 + (t8314 - t21684) * t177 / 0.2E1
        t21694 = rx(t507,j,t20556,0,0)
        t21695 = rx(t507,j,t20556,1,1)
        t21697 = rx(t507,j,t20556,2,2)
        t21699 = rx(t507,j,t20556,1,2)
        t21701 = rx(t507,j,t20556,2,1)
        t21703 = rx(t507,j,t20556,0,1)
        t21704 = rx(t507,j,t20556,1,0)
        t21708 = rx(t507,j,t20556,2,0)
        t21710 = rx(t507,j,t20556,0,2)
        t21716 = 0.1E1 / (t21694 * t21695 * t21697 - t21694 * t21699 * t
     #21701 - t21695 * t21708 * t21710 - t21697 * t21703 * t21704 + t216
     #99 * t21703 * t21708 + t21701 * t21704 * t21710)
        t21717 = t4 * t21716
        t21746 = t21708 ** 2
        t21747 = t21701 ** 2
        t21748 = t21697 ** 2
        t21757 = (t20529 - t4 * (t20525 / 0.2E1 + t12234 * (t21597 + t21
     #598 + t21599) / 0.2E1) * t8353) * t108 + t20555 + (t20552 - t12235
     # * (t12212 * t12222 + t12213 * t12221 + t12217 * t12228) * t12258)
     # * t108 / 0.2E1 + t20583 + (t20580 - t11607 * (t8249 / 0.2E1 + (t8
     #247 - t21617) * t177 / 0.2E1)) * t108 / 0.2E1 + (t12566 * t12574 *
     # t21630 - t21634) * t124 / 0.2E1 + (-t12871 * t12879 * t21641 + t2
     #1634) * t124 / 0.2E1 + (t4 * (t12565 * (t21647 + t21648 + t21649) 
     #/ 0.2E1 + t21656 / 0.2E1) * t8366 - t4 * (t21656 / 0.2E1 + t12870 
     #* (t21661 + t21662 + t21663) / 0.2E1) * t8368) * t124 + (t11922 * 
     #t21676 - t21680) * t124 / 0.2E1 + (-t12201 * t21688 + t21680) * t1
     #24 / 0.2E1 + t8360 + (t8357 - t21717 * (t21694 * t21708 + t21697 *
     # t21710 + t21701 * t21703) * (t20682 / 0.2E1 + (t20574 - t21617) *
     # t108 / 0.2E1)) * t177 / 0.2E1 + t8375 + (t8372 - t21717 * (t21695
     # * t21701 + t21697 * t21699 + t21704 * t21708) * ((t21672 - t20574
     #) * t124 / 0.2E1 + (t20574 - t21684) * t124 / 0.2E1)) * t177 / 0.2
     #E1 + (t8384 - t4 * (t8380 / 0.2E1 + t21716 * (t21746 + t21747 + t2
     #1748) / 0.2E1) * t20576) * t177
        t21758 = t21757 * t8345
        t21771 = t5081 * t9046
        t21784 = t5951 ** 2
        t21785 = t5960 ** 2
        t21786 = t5967 ** 2
        t21789 = t8655 ** 2
        t21790 = t8664 ** 2
        t21791 = t8671 ** 2
        t21793 = t8677 * (t21789 + t21790 + t21791)
        t21798 = t12543 ** 2
        t21799 = t12552 ** 2
        t21800 = t12559 ** 2
        t21812 = t19567 * t8699
        t21824 = t8119 * t20633
        t21842 = t15460 ** 2
        t21843 = t15451 ** 2
        t21844 = t15455 ** 2
        t21853 = u(i,t1047,t20556,n)
        t21863 = rx(i,t121,t20556,0,0)
        t21864 = rx(i,t121,t20556,1,1)
        t21866 = rx(i,t121,t20556,2,2)
        t21868 = rx(i,t121,t20556,1,2)
        t21870 = rx(i,t121,t20556,2,1)
        t21872 = rx(i,t121,t20556,0,1)
        t21873 = rx(i,t121,t20556,1,0)
        t21877 = rx(i,t121,t20556,2,0)
        t21879 = rx(i,t121,t20556,0,2)
        t21885 = 0.1E1 / (t21863 * t21864 * t21866 - t21863 * t21868 * t
     #21870 - t21864 * t21877 * t21879 - t21866 * t21872 * t21873 + t218
     #68 * t21872 * t21877 + t21870 * t21873 * t21879)
        t21886 = t4 * t21885
        t21915 = t21877 ** 2
        t21916 = t21870 ** 2
        t21917 = t21866 ** 2
        t21926 = (t4 * (t5973 * (t21784 + t21785 + t21786) / 0.2E1 + t21
     #793 / 0.2E1) * t5982 - t4 * (t21793 / 0.2E1 + t12565 * (t21798 + t
     #21799 + t21800) / 0.2E1) * t8684) * t108 + (t20433 * t5997 - t2181
     #2) * t108 / 0.2E1 + (-t12566 * t12587 * t21630 + t21812) * t108 / 
     #0.2E1 + (t20465 * t5978 - t21824) * t108 / 0.2E1 + (-t11916 * t216
     #76 + t21824) * t108 / 0.2E1 + (t15473 * (t15450 * t15460 + t15451 
     #* t15459 + t15455 * t15466) * t15483 - t20589) * t124 / 0.2E1 + t2
     #0594 + (t4 * (t15472 * (t21842 + t21843 + t21844) / 0.2E1 + t20608
     # / 0.2E1) * t8697 - t20617) * t124 + (t14720 * (t8647 / 0.2E1 + (t
     #8645 - t21853) * t177 / 0.2E1) - t20635) * t124 / 0.2E1 + t20640 +
     # t8691 + (t8688 - t21886 * (t21863 * t21877 + t21866 * t21879 + t2
     #1870 * t21872) * ((t21494 - t20629) * t108 / 0.2E1 + (t20629 - t21
     #672) * t108 / 0.2E1)) * t177 / 0.2E1 + t8704 + (t8701 - t21886 * (
     #t21864 * t21870 + t21866 * t21868 + t21873 * t21877) * ((t21853 - 
     #t20629) * t124 / 0.2E1 + t20695 / 0.2E1)) * t177 / 0.2E1 + (t8713 
     #- t4 * (t8709 / 0.2E1 + t21885 * (t21915 + t21916 + t21917) / 0.2E
     #1) * t20631) * t177
        t21927 = t21926 * t8676
        t21935 = t811 * t21591
        t21939 = t6320 ** 2
        t21940 = t6329 ** 2
        t21941 = t6336 ** 2
        t21944 = t8960 ** 2
        t21945 = t8969 ** 2
        t21946 = t8976 ** 2
        t21948 = t8982 * (t21944 + t21945 + t21946)
        t21953 = t12848 ** 2
        t21954 = t12857 ** 2
        t21955 = t12864 ** 2
        t21967 = t19577 * t9004
        t21979 = t8411 * t20645
        t21997 = t17625 ** 2
        t21998 = t17616 ** 2
        t21999 = t17620 ** 2
        t22008 = u(i,t1111,t20556,n)
        t22018 = rx(i,t126,t20556,0,0)
        t22019 = rx(i,t126,t20556,1,1)
        t22021 = rx(i,t126,t20556,2,2)
        t22023 = rx(i,t126,t20556,1,2)
        t22025 = rx(i,t126,t20556,2,1)
        t22027 = rx(i,t126,t20556,0,1)
        t22028 = rx(i,t126,t20556,1,0)
        t22032 = rx(i,t126,t20556,2,0)
        t22034 = rx(i,t126,t20556,0,2)
        t22040 = 0.1E1 / (t22018 * t22019 * t22021 - t22018 * t22023 * t
     #22025 - t22019 * t22032 * t22034 - t22021 * t22027 * t22028 + t220
     #23 * t22027 * t22032 + t22025 * t22028 * t22034)
        t22041 = t4 * t22040
        t22070 = t22032 ** 2
        t22071 = t22025 ** 2
        t22072 = t22021 ** 2
        t22081 = (t4 * (t6342 * (t21939 + t21940 + t21941) / 0.2E1 + t21
     #948 / 0.2E1) * t6351 - t4 * (t21948 / 0.2E1 + t12870 * (t21953 + t
     #21954 + t21955) / 0.2E1) * t8989) * t108 + (t20440 * t6366 - t2196
     #7) * t108 / 0.2E1 + (-t12871 * t12892 * t21641 + t21967) * t108 / 
     #0.2E1 + (t20471 * t6347 - t21979) * t108 / 0.2E1 + (-t12195 * t216
     #88 + t21979) * t108 / 0.2E1 + t20603 + (t20600 - t17638 * (t17615 
     #* t17625 + t17616 * t17624 + t17620 * t17631) * t17648) * t124 / 0
     #.2E1 + (t20626 - t4 * (t20622 / 0.2E1 + t17637 * (t21997 + t21998 
     #+ t21999) / 0.2E1) * t9002) * t124 + t20650 + (t20647 - t16719 * (
     #t8952 / 0.2E1 + (t8950 - t22008) * t177 / 0.2E1)) * t124 / 0.2E1 +
     # t8996 + (t8993 - t22041 * (t22018 * t22032 + t22021 * t22034 + t2
     #2025 * t22027) * ((t21506 - t20641) * t108 / 0.2E1 + (t20641 - t21
     #684) * t108 / 0.2E1)) * t177 / 0.2E1 + t9009 + (t9006 - t22041 * (
     #t22019 * t22025 + t22021 * t22023 + t22028 * t22032) * (t20697 / 0
     #.2E1 + (t20641 - t22008) * t124 / 0.2E1)) * t177 / 0.2E1 + (t9018 
     #- t4 * (t9014 / 0.2E1 + t22040 * (t22070 + t22071 + t22072) / 0.2E
     #1) * t20643) * t177
        t22082 = t22081 * t8981
        t22117 = (t5339 * t6410 - t5534 * t9044) * t108 + (t4823 * t6436
     # - t21410) * t108 / 0.2E1 + (-t12328 * t5541 + t21410) * t108 / 0.
     #2E1 + (t435 * (t5425 / 0.2E1 + (t5423 - t21580) * t177 / 0.2E1) - 
     #t21593) * t108 / 0.2E1 + (t21593 - t2520 * (t8390 / 0.2E1 + (t8388
     # - t21758) * t177 / 0.2E1)) * t108 / 0.2E1 + (t14824 * t5557 - t21
     #771) * t124 / 0.2E1 + (-t16814 * t5568 + t21771) * t124 / 0.2E1 + 
     #(t5586 * t9066 - t5595 * t9068) * t124 + (t4289 * (t8719 / 0.2E1 +
     # (t8717 - t21927) * t177 / 0.2E1) - t21935) * t124 / 0.2E1 + (t219
     #35 - t4426 * (t9024 / 0.2E1 + (t9022 - t22082) * t177 / 0.2E1)) * 
     #t124 / 0.2E1 + t9051 + (t9048 - t2967 * ((t21580 - t21587) * t108 
     #/ 0.2E1 + (t21587 - t21758) * t108 / 0.2E1)) * t177 / 0.2E1 + t907
     #5 + (t9072 - t3142 * ((t21927 - t21587) * t124 / 0.2E1 + (t21587 -
     # t22082) * t124 / 0.2E1)) * t177 / 0.2E1 + (-t21589 * t3098 + t907
     #7) * t177
        t22118 = t19066 * t22117
        t22120 = t7451 * t22118 / 0.12E2
        t22121 = t19061 + t20723 + t20838 + t20843 - t7029 * t21145 / 0.
     #4E1 + t19074 - t7018 * t20836 / 0.8E1 - t2378 * t20841 / 0.4E1 + t
     #19414 + t19419 + t21400 - t19432 + t21402 + t19439 - t19443 + t221
     #20
        t22124 = t72 * t10126 * t177
        t22127 = dz * t6802
        t22129 = t9108 * t22127 / 0.24E2
        t22134 = t852 * t7449 * t22124 / 0.6E1
        t22137 = t71 * t5619 * t177
        t22139 = t852 * t1944 * t22137 / 0.2E1
        t22147 = t213 - dz * t6789 / 0.24E2
        t22151 = t3375 * t9108 * t22147
        t22156 = t852 * t69 * t22124 / 0.6E1 - t19445 + t22129 - t3386 *
     # t22118 / 0.12E2 - t20165 - t22134 - t22139 + t852 * t68 * t22137 
     #/ 0.2E1 - t20173 - t20175 - t2751 * t21398 / 0.2E1 + t3375 * t2746
     # * t22147 - t22151 + t20177 - t2378 * t20721 / 0.24E2 - t2746 * t2
     #2127 / 0.24E2
        t22158 = (t22121 + t22156) * t65
        t22161 = -t20723 - t20838 - t20843 - t19074 - t21400 - t21402 + 
     #t19443 - t22120 + t19445 - t22129 + t20165 + t22134
        t22162 = t20207 / 0.2E1
        t22167 = t3375 * (t942 - dz * t3027 / 0.24E2)
        t22170 = sqrt(t20708)
        t22178 = (t20228 - (t20226 - (-cc * t20673 * t20747 * t22170 + t
     #20224) * t177) * t177) * t177
        t22185 = dz * (t20238 + t20226 / 0.2E1 - t1363 * (t20230 / 0.2E1
     # + t22178 / 0.2E1) / 0.6E1) / 0.4E1
        t22191 = t1363 * (t20228 - dz * (t20230 - t22178) / 0.12E2) / 0.
     #24E2
        t22193 = dz * t3102 / 0.24E2
        t22194 = -t22158 * t863 + t20175 - t20177 + t20198 + t20236 - t2
     #0245 + t22139 + t22151 - t22162 + t22167 - t22185 - t22191 - t2219
     #3
        t22198 = t18224 * t71 / 0.6E1 + (-t18224 * t863 + t18214 + t1821
     #7 + t18220 - t18222 + t18287 - t18291) * t71 / 0.2E1 + t18405 * t7
     #1 / 0.6E1 + (-t18405 * t863 + t18395 + t18398 + t18401 - t18403 + 
     #t18468 - t18472) * t71 / 0.2E1 + t20187 * t71 / 0.6E1 + (t20194 + 
     #t20272) * t71 / 0.2E1 - t20351 * t71 / 0.6E1 - (-t20351 * t863 + t
     #20341 + t20344 + t20347 - t20349 + t20382 - t20386) * t71 / 0.2E1 
     #- t20466 * t71 / 0.6E1 - (-t20466 * t863 + t20456 + t20459 + t2046
     #2 - t20464 + t20497 - t20501) * t71 / 0.2E1 - t22158 * t71 / 0.6E1
     # - (t22161 + t22194) * t71 / 0.2E1
        t22204 = t9248 + t9110 + t9094 - t9309 + t9089 - t9227 + t9242 +
     # t1942 - t9317 + t7448 - t7028 + t9307
        t22205 = t7453 - t2271 + t9100 - t9238 - t9229 - t9285 - t7017 -
     # t2745 - t9291 - t9086 - t9223 - t9116
        t22221 = t13025 + t10649 + t11339 - t12997 + t12985 - t11336 + t
     #9238 + t9229 - t9285 + t7017 - t2745 + t9291
        t22222 = t9086 - t9223 + t9116 - t12994 - t10642 - t13021 - t112
     #25 - t12976 - t13013 - t12987 - t11330 - t10788
        t22236 = t9232 * dt / 0.2E1 + (t22204 + t22205) * dt - t9232 * t
     #9108 + t9722 * dt / 0.2E1 + (t9784 + t9710 + t9714 - t9788 + t9718
     # - t9720) * dt - t9722 * t9108 + t10149 * dt / 0.2E1 + (t10211 + t
     #10139 + t10142 - t10215 + t10145 - t10147) * dt - t10149 * t9108 -
     # t12990 * dt / 0.2E1 - (t22221 + t22222) * dt + t12990 * t9108 - t
     #13223 * dt / 0.2E1 - (t13254 + t13213 + t13216 - t13258 + t13219 -
     # t13221) * dt + t13223 * t9108 - t13440 * dt / 0.2E1 - (t13471 + t
     #13430 + t13433 - t13475 + t13436 - t13438) * dt + t13440 * t9108
        t22246 = t15588 + t13737 + t13722 - t15666 + t13717 - t13693 + t
     #15594 + t14292 - t15664 + t14773 - t13962 + t15656
        t22247 = t15575 - t14777 + t15579 - t15597 - t13688 - t15640 - t
     #14663 - t14678 - t15631 - t14295 - t14676 - t13730
        t22263 = t17897 + t16054 + t16811 - t17893 + t16047 - t17860 + t
     #15597 + t13688 - t15640 + t14663 - t14678 + t15631
        t22264 = t14295 - t14676 + t13730 - t17866 - t16808 - t17891 - t
     #17843 - t17849 - t17883 - t17851 - t17855 - t17847
        t22273 = t13603 * dt / 0.2E1 + (t13675 + t13593 + t13596 - t1367
     #9 + t13599 - t13601) * dt - t13603 * t9108 + t15582 * dt / 0.2E1 +
     # (t22246 + t22247) * dt - t15582 * t9108 + t15866 * dt / 0.2E1 + (
     #t15919 + t15856 + t15859 - t15923 + t15862 - t15864) * dt - t15866
     # * t9108 - t16003 * dt / 0.2E1 - (t16034 + t15993 + t15996 - t1603
     #8 + t15999 - t16001) * dt + t16003 * t9108 - t17863 * dt / 0.2E1 -
     # (t22263 + t22264) * dt + t17863 * t9108 - t18072 * dt / 0.2E1 - (
     #t18103 + t18062 + t18065 - t18107 + t18068 - t18070) * dt + t18072
     # * t9108
        t22288 = t20193 + t19058 + t19430 - t20271 + t20182 - t19416 + t
     #20202 + t19426 - t20269 + t19046 - t19436 + t20261
        t22289 = t20179 - t19405 + t18704 - t20198 - t20165 - t20245 - t
     #20175 - t19074 - t20236 - t19443 - t20177 - t19445
        t22305 = t22167 + t22151 + t22139 - t22193 + t22134 - t22129 + t
     #20198 + t20165 - t20245 + t20175 - t19074 + t20236
        t22306 = t19443 - t20177 + t19445 - t22162 - t21400 - t22185 - t
     #21402 - t20843 - t22191 - t22120 - t20838 - t20723
        t22310 = t18224 * dt / 0.2E1 + (t18287 + t18214 + t18217 - t1829
     #1 + t18220 - t18222) * dt - t18224 * t9108 + t18405 * dt / 0.2E1 +
     # (t18468 + t18395 + t18398 - t18472 + t18401 - t18403) * dt - t184
     #05 * t9108 + t20187 * dt / 0.2E1 + (t22288 + t22289) * dt - t20187
     # * t9108 - t20351 * dt / 0.2E1 - (t20382 + t20341 + t20344 - t2038
     #6 + t20347 - t20349) * dt + t20351 * t9108 - t20466 * dt / 0.2E1 -
     # (t20497 + t20456 + t20459 - t20501 + t20462 - t20464) * dt + t204
     #66 * t9108 - t22158 * dt / 0.2E1 - (t22305 + t22306) * dt + t22158
     # * t9108
        
       unew(i,j,k) = t108 * t13480 * t55 + t124 * t18112 * t55 + t17
     #7 * t22198 * t55 + dt * t2 + t1
       utnew(i,j,k) = t108 * t22236 * t55 + t124 * 
     #t22273 * t55 + t177 * t22310 * t55 + t2

        return
      end
