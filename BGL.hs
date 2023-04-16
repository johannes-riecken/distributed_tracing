import Test.QuickCheck
import Foreign.C
import Foreign.Ptr
import Data.Function.Pointless
import System.IO.Unsafe
import Foreign.Marshal.Array
import Data.List

instance CoArbitrary CChar where
  coarbitrary = coarbitraryIntegral

instance Arbitrary CBool where
  arbitrary = chooseEnum (0,1)
  shrink 1 = [0]
  shrink 0 = []

selfLoop :: [CChar] -> CBool
selfLoop xs = fromIntegral . fromEnum $ any (uncurry (==)) (zip xs (tail xs))
foreign import ccall "LLVMFuzzerTestOneInput" cSelfLoop' :: Ptr CChar -> CChar -> CBool

cSelfLoop :: [CChar] -> CBool
cSelfLoop xs = unsafePerformIO $ do
    xs' <- newArray xs
    pure $ cSelfLoop' xs' (genericLength xs)

main :: IO ()
main = do
    -- print $ cSelfLoop [0,1]
    quickCheck $ \xs -> selfLoop xs === cSelfLoop xs
