package Experiment3;

import java.util.Random;

public class GAOperations {

    /**
     * 随机初始化种群
     * 要求：每个染色体是 1..codeNum 的一个排列，且每种编码 codes[i] 必须恰好出现 codeCount[i] 次
     */
    public void RandomInitialiation(int popNum, int length, int[] codes, int codeNum, int[] codeCount, int[][] iniPop) {
        Random random = new Random();
        int[] nJs = new int[codeNum]; // 记录每种编码还剩多少个未使用

        for (int p = 0; p < popNum; p++) {
            // 重置计数
            for (int i = 0; i < codeNum; i++) {
                nJs[i] = codeCount[i];
            }

            // 构建一个合法排列
            for (int j = 0; j < length; j++) {
                // 随机选择一种还没用完的编码
                int validCount = 0;
                for (int k = 0; k < codeNum; k++) {
                    if (nJs[k] > 0) validCount++;
                }
                if (validCount == 0) break; // 防御

                int rand = random.nextInt(validCount);
                int idx = 0;
                for (int k = 0; k < codeNum; k++) {
                    if (nJs[k] > 0) {
                        if (idx == rand) {
                            iniPop[p][j] = codes[k];
                            nJs[k]--;
                            break;
                        }
                        idx++;
                    }
                }
            }
        }
    }

    /**
     * 计算单个染色体的适应度（TSP：路径越短，适应度越高）
     * @return 1 / 总距离（距离为0或无边时返回极小值）
     */
    public static double computeFitness(int[] pop, int length, int[][] a) {
        double total = 0.0;
        for (int i = 0; i < length - 1; i++) {
            int from = pop[i];
            int to = pop[i + 1];
            if (a[from][to] <= 0) return 1e-9; // 无边或负值，惩罚
            total += a[from][to];
        }
        // 加上回到起点
        if (a[pop[length - 1]][pop[0]] <= 0) return 1e-9;
        total += a[pop[length - 1]][pop[0]];

        return total > 0 ? 1.0 / total : 1e-9;
    }

    /**
     * 轮盘赌选择（按适应度比例选择）
     */
    public static void roundBet(int popNum, int length, int[][] iniPop1, double[] fitness) {
        Random random = new Random();
        double[] cumProb = new double[popNum]; // 累积概率
        double sumFit = 0.0;

        // 计算总适应度
        for (int i = 0; i < popNum; i++) {
            sumFit += fitness[i];
        }
        if (sumFit <= 0) return; // 防御

        // 构建累积概率
        cumProb[0] = fitness[0] / sumFit;
        for (int i = 1; i < popNum; i++) {
            cumProb[i] = cumProb[i - 1] + fitness[i] / sumFit;
        }

        // 生成新种群
        int[][] newPop = new int[popNum][length];
        for (int i = 0; i < popNum; i++) {
            double r = random.nextDouble();
            int selected = 0;
            for (int j = 0; j < popNum; j++) {
                if (r <= cumProb[j]) {
                    selected = j;
                    break;
                }
            }
            System.arraycopy(iniPop1[selected], 0, newPop[i], 0, length);
        }

        // 写回原种群
        for (int i = 0; i < popNum; i++) {
            System.arraycopy(newPop[i], 0, iniPop1[i], 0, length);
        }
    }

    /**
     * 扰动变异：在 disPos 位置附近随机交换若干位置
     */
    public static void Disturbance(int[][] iniPop, int popNum, int length, int disPos) {
        Random random = new Random();
        int range = 3; // 扰动范围：disPos ± range

        for (int p = 0; p < popNum; p++) {
            if (random.nextDouble() < 0.3) { // 30% 概率触发扰动
                int center = disPos;
                int start = Math.max(0, center - range);
                int end = Math.min(length - 1, center + range);

                // 随机交换 1~3 次
                int times = 1 + random.nextInt(3);
                for (int t = 0; t < times; t++) {
                    int i = start + random.nextInt(end - start + 1);
                    int j = start + random.nextInt(end - start + 1);
                    if (i != j) {
                        int temp = iniPop[p][i];
                        iniPop[p][i] = iniPop[p][j];
                        iniPop[p][j] = temp;
                    }
                }
            }
        }
    }

    /**
     * 获取 code 在 codes 数组中的下标位置
     */
    public static int getCodePos(int code, int codeNum, int[] codes) {
        for (int pos = 0; pos < codeNum; pos++) {
            if (code == codes[pos]) {
                return pos;
            }
        }
        return -1;
    }

    /* ------------------- 测试主函数 ------------------- */
    public static void main(String[] args) {
        int popNum = 10;
        int length = 5;
        int[] codes = {1, 2, 3};
        int codeNum = 3;
        int[] codeCount = {2, 2, 1}; // 编码 1 出现 2 次，2 出现 2 次，3 出现 1 次
        int[][] iniPop = new int[popNum][length];

        GAOperations ga = new GAOperations();
        ga.RandomInitialiation(popNum, length, codes, codeNum, codeCount, iniPop);

        System.out.println("初始种群：");
        for (int i = 0; i < popNum; i++) {
            for (int j = 0; j < length; j++) {
                System.out.print(iniPop[i][j] + " ");
            }
            System.out.println();
        }

        // 测试适应度
        int[][] a = {
                {0,0,0,0,0,0},
                {0,0,10,15,20,25},
                {0,10,0,35,25,30},
                {0,15,35,0,30,20},
                {0,20,25,30,0,15},
                {0,25,30,20,15,0}
        };
        double[] fitness = new double[popNum];
        for (int i = 0; i < popNum; i++) {
            fitness[i] = computeFitness(iniPop[i], length, a);
        }

        System.out.println("\n轮盘赌选择后：");
        roundBet(popNum, length, iniPop, fitness);
        for (int i = 0; i < popNum; i++) {
            for (int j = 0; j < length; j++) {
                System.out.print(iniPop[i][j] + " ");
            }
            System.out.println();
        }

        System.out.println("\n扰动后（disPos=2）：");
        Disturbance(iniPop, popNum, length, 2);
        for (int i = 0; i < popNum; i++) {
            for (int j = 0; j < length; j++) {
                System.out.print(iniPop[i][j] + " ");
            }
            System.out.println();
        }
    }
}