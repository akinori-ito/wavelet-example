# ���M���̂��߂�Wavelet�ϊ��̎�����Ɗe��c�[��
2007.7.24 �ɓ� ����

## 1. �C���X�g�[��

�C���X�g�[���ɂ�gcc��make���K�v�Dgcc���Ȃ��ꍇ�ɂ́CMakefile �� CC=gcc
�̕�����ύX����DWindows�̏ꍇ�� cygwin ���g���ΊȒP�D

make

�Ŋe��c�[�����R���p�C���ł���D

## 2. �c�[���̐���

* calc-error file1.wav file2.wav

  file1.wav�������Cfile2.wav �����H���Ƃ��āC���H����SN����v�Z�E�\������D

* addnoise mag infile.wav outfile.wav

  �傫�� -mag�`+mag �̈�l�������g���āCinfile.wav �ɎG����t�����Coutfile.wav ��
  �o�͂���D

* wavdiff infile1.wav infile2.wav outfile.wav

  infile1.wav �� infile2.wav �̃T���v�����Ƃ̍����v�Z���Coutfile.wav �ɏo�͂���D

* wavelet-sample infile.wav outfile.wav

  infile.wav �̐M����Wavelet�ɂ�蕪���E�č������� outfile.wav �ɏo�͂���D�v���O����
  ����������΁Coutfile.wav �̓��e�� infile.wav �Ƃقړ����ɂȂ�D

## 3. ���C�Z���X

�ɓ����������쌠��ێ�����D
(C) Copyright 2007 by Akinori Ito, aito@fw.ipsj.or.jp

���p�҂͌����E����ړI�Ɍ���C���̃\�t�g�E�F�A�𗘗p���邱�Ƃ��ł���D
�{�v���O������as is�Œ񋟂������̂ł���C���쌠�҂͖{�v���O�����Ɋւ���v�]�E
���ⓙ�ɑΉ�����`����L���Ȃ��D���쌠�҂́C�v���O�����̗��p�\������ї��p��
�����ʂɂ��Ĉ�؂̕ۏ؂��s��Ȃ��D
