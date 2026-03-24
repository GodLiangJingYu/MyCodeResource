; ModuleID = 'D:/MyCodeResource/编译原理/作业/作业1/example.c'
source_filename = "D:/MyCodeResource/\E7\BC\96\E8\AF\91\E5\8E\9F\E7\90\86/\E4\BD\9C\E4\B8\9A/\E4\BD\9C\E4\B8\9A1/example.c"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@globalVar = dso_local global i32 42, align 4
@.str = private unnamed_addr constant [26 x i8] c"Result is greater than 25\00", align 1
@.str.1 = private unnamed_addr constant [21 x i8] c"Result is 25 or less\00", align 1
@.str.2 = private unnamed_addr constant [29 x i8] c"x = %d, y = %d, result = %d\0A\00", align 1
@.str.3 = private unnamed_addr constant [30 x i8] c"squared = %d, globalVar = %d\0A\00", align 1
@.str.4 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca [100 x i32], align 16
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  store i32 0, ptr %1, align 4
  store i32 10, ptr %2, align 4
  store i32 20, ptr %3, align 4
  store i32 25, ptr %6, align 4
  %8 = load i32, ptr %2, align 4
  %9 = load i32, ptr %3, align 4
  %10 = call i32 @add(i32 noundef %8, i32 noundef %9)
  store i32 %10, ptr %4, align 4
  %11 = load i32, ptr %4, align 4
  %12 = icmp sgt i32 %11, 25
  br i1 %12, label %13, label %14

13:                                               ; preds = %0
  call void @printMessage(ptr noundef @.str)
  br label %15

14:                                               ; preds = %0
  call void @printMessage(ptr noundef @.str.1)
  br label %15

15:                                               ; preds = %14, %13
  store i32 0, ptr %7, align 4
  br label %16

16:                                               ; preds = %25, %15
  %17 = load i32, ptr %7, align 4
  %18 = icmp slt i32 %17, 5
  br i1 %18, label %19, label %28

19:                                               ; preds = %16
  %20 = load i32, ptr %7, align 4
  %21 = mul nsw i32 %20, 2
  %22 = load i32, ptr %7, align 4
  %23 = sext i32 %22 to i64
  %24 = getelementptr inbounds [100 x i32], ptr %5, i64 0, i64 %23
  store i32 %21, ptr %24, align 4
  br label %25

25:                                               ; preds = %19
  %26 = load i32, ptr %7, align 4
  %27 = add nsw i32 %26, 1
  store i32 %27, ptr %7, align 4
  br label %16, !llvm.loop !8

28:                                               ; preds = %16
  %29 = load i32, ptr %2, align 4
  %30 = load i32, ptr %3, align 4
  %31 = load i32, ptr %4, align 4
  %32 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, i32 noundef %29, i32 noundef %30, i32 noundef %31)
  %33 = load i32, ptr %6, align 4
  %34 = load i32, ptr @globalVar, align 4
  %35 = call i32 (ptr, ...) @printf(ptr noundef @.str.3, i32 noundef %33, i32 noundef %34)
  ret i32 0
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @add(i32 noundef %0, i32 noundef %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %0, ptr %3, align 4
  store i32 %1, ptr %4, align 4
  %5 = load i32, ptr %3, align 4
  %6 = load i32, ptr %4, align 4
  %7 = add nsw i32 %5, %6
  ret i32 %7
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @printMessage(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  %3 = load ptr, ptr %2, align 8
  %4 = call i32 (ptr, ...) @printf(ptr noundef @.str.4, ptr noundef %3)
  ret void
}

declare dso_local i32 @printf(ptr noundef, ...) #1

attributes #0 = { noinline nounwind optnone uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2, !3, !4, !5, !6}
!llvm.ident = !{!7}

!0 = distinct !DICompileUnit(language: DW_LANG_C11, file: !1, producer: "clang version 21.1.5", isOptimized: false, runtimeVersion: 0, emissionKind: NoDebug, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "D:/MyCodeResource/\E7\BC\96\E8\AF\91\E5\8E\9F\E7\90\86/\E4\BD\9C\E4\B8\9A/\E4\BD\9C\E4\B8\9A1/example.c", directory: "D:/MyCodeResource/\E7\BC\96\E8\AF\91\E5\8E\9F\E7\90\86/\E4\BD\9C\E4\B8\9A/\E4\BD\9C\E4\B8\9A1")
!2 = !{i32 2, !"Debug Info Version", i32 3}
!3 = !{i32 1, !"wchar_size", i32 2}
!4 = !{i32 8, !"PIC Level", i32 2}
!5 = !{i32 7, !"uwtable", i32 2}
!6 = !{i32 1, !"MaxTLSAlign", i32 65536}
!7 = !{!"clang version 21.1.5"}
!8 = distinct !{!8, !9}
!9 = !{!"llvm.loop.mustprogress"}
