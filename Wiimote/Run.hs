{-# LANGUAGE RebindableSyntax #-}
module Main where

import qualified Prelude as P
import Copilot.Language.Prelude
import Copilot.Language
import Wiimote

vInited :: [Word32]
vInited = repeat 0

vAcc0, vAcc1, vAcc2 :: [Word8]
vAcc0 = repeat 0
vAcc1 = repeat 0
vAcc2 = repeat 0

main :: IO ()
main = interpret 50 [var "g_inited" vInited,
                     var "g_acc0" vAcc0,
                     var "g_acc1" vAcc1,
                     var "g_acc2" vAcc2] wiiSpec
