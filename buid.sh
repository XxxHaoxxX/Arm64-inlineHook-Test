clear
start=$(date +%s)
cd loader/jni
/data/x0/ndk/ndk-build
cd ../../hook/jni
/data/x0/ndk/ndk-build
cd ../libs/arm64-v8a/
end=$(date +%s)
diff=$(( end - start ))
echo "编译总耗时: $diff s"
../../../loader/libs/arm64-v8a/loader