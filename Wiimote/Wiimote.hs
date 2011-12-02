{-# LANGUAGE RebindableSyntax #-}
module Wiimote where

import qualified Prelude as P
import Copilot.Language.Prelude
import Copilot.Language

inited :: Stream Word32
inited = extern "g_inited"

dAcc0, dAcc1, dAcc2 :: Stream Double
dAcc0 = externFun "cast_double" [funArg $ externW8 "g_acc0"]
dAcc1 = externFun "cast_double" [funArg $ externW8 "g_acc1"]
dAcc2 = externFun "cast_double" [funArg $ externW8 "g_acc2"]

dZero0, dZero1, dZero2, dOne0, dOne1, dOne2 :: Stream Double
dZero0 = externFun "cast_double" [funArg $ externW8 "g_zero0"]
dZero1 = externFun "cast_double" [funArg $ externW8 "g_zero1"]
dZero2 = externFun "cast_double" [funArg $ externW8 "g_zero2"]
dOne0 = externFun "cast_double" [funArg $ externW8 "g_one0"]
dOne1 = externFun "cast_double" [funArg $ externW8 "g_one1"]
dOne2 = externFun "cast_double" [funArg $ externW8 "g_one2"]

aX, aY, aZ :: Stream Double
aX = (dAcc0 - dZero0) / (dOne0 - dZero0)
aY = (dAcc0 - dZero0) / (dOne0 - dZero0)
aZ = (dAcc0 - dZero0) / (dOne0 - dZero0)

acc :: Stream Double
acc = sqrt $ px + py + pz
  where px = aX ** 2
        py = aY ** 2
        pz = aZ ** 2

wiiSpec :: Spec
wiiSpec = do
  trigger "l_cwiid_open" (inited == 0) []
  trigger "l_cwiid_get_acc_cal" (inited == 1) []
  trigger "l_cwiid_set_rpt_mode" (inited == 2) []
  trigger "l_cwiid_get_state" (inited == 3) []
  trigger "pout_d" (inited == 3) [arg acc]
