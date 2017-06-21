# 音信号のためのWavelet変換の実装例と各種ツール
2007.7.24 伊藤 彰則

## 1. インストール

インストールにはgccとmakeが必要．gccがない場合には，Makefile の CC=gcc
の部分を変更する．Windowsの場合は cygwin を使えば簡単．

make

で各種ツールがコンパイルできる．

## 2. ツールの説明

* calc-error file1.wav file2.wav

  file1.wavを原音，file2.wav を加工音として，加工音のSN比を計算・表示する．

* addnoise mag infile.wav outfile.wav

  大きさ -mag～+mag の一様乱数を使って，infile.wav に雑音を付加し，outfile.wav に
  出力する．

* wavdiff infile1.wav infile2.wav outfile.wav

  infile1.wav と infile2.wav のサンプルごとの差を計算し，outfile.wav に出力する．

* wavelet-sample infile.wav outfile.wav

  infile.wav の信号をWaveletにより分解・再合成して outfile.wav に出力する．プログラム
  が正しければ，outfile.wav の内容は infile.wav とほぼ同じになる．

## 3. ライセンス

伊藤彰則が著作権を保持する．
(C) Copyright 2007 by Akinori Ito, aito@fw.ipsj.or.jp

利用者は研究・教育目的に限り，このソフトウェアを利用することができる．
本プログラムはas isで提供されるものであり，著作権者は本プログラムに関する要望・
質問等に対応する義務を有しない．著作権者は，プログラムの利用可能性および利用し
た結果について一切の保証を行わない．
