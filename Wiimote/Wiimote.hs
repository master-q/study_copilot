{-# LANGUAGE RebindableSyntax #-}
module Wiimote where

import qualified Prelude as P
import Copilot.Language.Prelude
import Copilot.Language

type StreamTdouble = (Stream Double, Stream Double, Stream Double)

inited :: Stream Word32
inited = [0,1,2] ++ 3

buttons :: Stream Word16
buttons = externW16 "g_buttons"

pressedB :: Stream Bool
pressedB = (buttons .&. 4) /= 0

onB :: Stream Bool
onB = n && not p
  where n = pressedB
        p = [False] ++ n

offB :: Stream Bool
offB = not n && p
  where n = pressedB
        p = [False] ++ n

dAcc :: StreamTdouble
dAcc = (externFun "cast_double" [funArg $ externW8 "g_acc0"],
        externFun "cast_double" [funArg $ externW8 "g_acc1"],
        externFun "cast_double" [funArg $ externW8 "g_acc2"])

dZero :: StreamTdouble
dZero = (externFun "cast_double" [funArg $ externW8 "g_zero0"],
         externFun "cast_double" [funArg $ externW8 "g_zero1"],
         externFun "cast_double" [funArg $ externW8 "g_zero2"])

dOne :: StreamTdouble
dOne = (externFun "cast_double" [funArg $ externW8 "g_one0"],
        externFun "cast_double" [funArg $ externW8 "g_one1"],
        externFun "cast_double" [funArg $ externW8 "g_one2"])

aXyz :: StreamTdouble -> StreamTdouble -> StreamTdouble -> StreamTdouble
aXyz (a1,a2,a3) (z1,z2,z3) (o1,o2,o3) =
  ((a1 - z1) / (o1 - z1), (a2 - z2) / (o2 - z2), (a3 - z3) / (o3 - z3))

acc :: StreamTdouble -> Stream Double
acc (ax, ay, az) = sqrt $ ax ** 2 + ay ** 2 + az ** 2

accPrint :: Stream Bool
accPrint = (accB0 > 4.0) && ((accB1 > 4.0) && (accB2 > 4.0))
  where accB0 = acc $ aXyz dAcc dZero dOne
        accB1 = [0] ++ accB0
        accB2 = [0] ++ accB1

btnPrint:: Stream Bool
btnPrint = pressedB && (b1 && b2)
  where b1 = [False] ++ pressedB
        b2 = [False] ++ b1

wiiSpec :: Spec
wiiSpec = do
  trigger "l_cwiid_open" (inited == 0) []
  trigger "l_cwiid_get_acc_cal" (inited == 1) []
  trigger "l_cwiid_set_rpt_mode" (inited == 2) []
  trigger "l_cwiid_get_state" (inited == 3) []
  trigger "pout_d" (accPrint && btnPrint) [arg $ acc $ aXyz dAcc dZero dOne]
  trigger "pout_i" onB []
  trigger "pout_s" offB []
