#!/bin/bash

../../../bin/macosx/cktool buildbank game.ckbx
../../../bin/macosx/cktool buildstream music.wav

cp game.ckb music.cks android/assets
