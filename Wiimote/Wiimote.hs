{-# LANGUAGE RebindableSyntax #-}
module Wiimote where

import qualified Prelude as P
import Copilot.Language.Prelude
import Copilot.Language

inited :: Stream Word32
inited = extern "g_inited"

acc :: Stream Word64
acc = (a2 * 2 ^ 16) .|.
      (a1 * 2 ^ 8) .|. a0
  where
    a0, a1, a2 :: Stream Word64
    a0 = cast $ externW8 "g_acc0"
    a1 = cast $ externW8 "g_acc1"
    a2 = cast $ externW8 "g_acc2"

wiiSpec :: Spec
wiiSpec = do
  trigger "l_cwiid_open" (inited == 0) []
  trigger "l_cwiid_set_rpt_mode" (inited == 1) []
  trigger "l_cwiid_get_state" (inited == 2) []
  trigger "pout64" (inited == 2) [arg acc]
