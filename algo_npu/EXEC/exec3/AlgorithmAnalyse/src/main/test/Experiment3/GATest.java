package Experiment3;

import org.junit.Assert;
import org.junit.Test;

public class GATest {
    int popNum = 10;
    int length = 5; // 与 codes.length 和 codeCount.length 相同，表示 TSP 路径长度
    int codeNum = 5; // 城市种类数
    int[] codes = {1, 2, 3, 4, 5}; // 城市编号 1-5
    int[] codeCount = {1, 1, 1, 1, 1}; // 每种城市恰好出现 1 次 (标准 TSP)
    // 修正 a 矩阵为 6x6 (索引 0-5)，以容纳城市编号 1-5
    int[][] a = {
            {100, 100, 100, 100, 100, 100}, // row 0 (哨兵行，避免访问 0)
            {100, 100, 3, 1, 5, 8},         // row 1
            {100, 3, 100, 6, 7, 9},         // row 2
            {100, 1, 6, 100, 4, 2},         // row 3
            {100, 5, 7, 4, 100, 3},         // row 4
            {100, 8, 9, 2, 3, 100}          // row 5
    };
    int[][] iniPop = new int[popNum][length];
    GAOperations gaOperations = new GAOperations();

    @Test
    public void TestRandomInitialiation()
    {
        gaOperations.RandomInitialiation(popNum, length, codes, codeNum, codeCount, iniPop);
        int i, j;
        int[] nJs = new int[codeNum];//统计每种编码出现次数
        for(i = 0; i < popNum; i++)
        {
            for(j = 0; j < codeNum; j++)
            {
                nJs[j] = 0;
            }
            for(j = 0; j < length; j++) //统计每个code出现次数
            {
                int pos = GAOperations.getCodePos(iniPop[i][j], codeNum, codes);//获取code 在codes中的位置
                nJs[pos]++;
            }
            for(j = 0; j < codeNum; j++)
            {
                Assert.assertEquals(nJs[j], codeCount[j]);
            }
        }
    }

    @Test
    public void TestComputeFitness()
    {
        int[] pop = {1, 3, 5, 4, 2}; // 值 1-5 都在 a 矩阵索引 1-5 范围内
        // 计算路径: 1->3->5->4->2->1
        // 距离: a[1][3]=1 + a[3][5]=2 + a[5][4]=3 + a[4][2]=7 + a[2][1]=3 = 16
        double fit = GAOperations.computeFitness(pop, length, a);
        Assert.assertTrue("Expected fitness close to 1/16.0, but got: " + fit, Math.abs(fit - 1.0/16.0) < 0.0001);
    }

    @Test
    public void TestRoundBet()
    {
        gaOperations.RandomInitialiation(popNum, length, codes, codeNum, codeCount, iniPop);
        int i, j;
        int[] nJs = new int[codeNum];//统计每种编码出现次数
        double[] fitness = new double[popNum];
        for(i = 0; i < popNum; i++)
        {
            fitness[i] = GAOperations.computeFitness(iniPop[i], length, a); // 现在调用不会越界
        }
        GAOperations.roundBet(popNum, length, iniPop, fitness);
        for(i = 0; i < popNum; i++)
        {
            for(j = 0; j < codeNum; j++)
            {
                nJs[j] = 0;
            }
            for(j = 0; j < length; j++) //统计每个code出现次数
            {
                int pos = GAOperations.getCodePos(iniPop[i][j], codeNum, codes);//获取code 在codes中的位置
                nJs[pos]++;
            }
            for(j = 0; j < codeNum; j++)
            {
                Assert.assertEquals(nJs[j], codeCount[j]);
            }
        }
    }

    @Test
    public void TestDisturbance()
    {
        gaOperations.RandomInitialiation(popNum, length, codes, codeNum, codeCount, iniPop);
        int i, j;
        int[] nJs = new int[codeNum];//统计每种编码出现次数
        GAOperations.Disturbance(iniPop, popNum, length, 5);
        for(i = 0; i < popNum; i++)
        {
            for(j = 0; j < codeNum; j++)
            {
                nJs[j] = 0;
            }
            for(j = 0; j < length; j++) //统计每个code出现次数
            {
                int pos = GAOperations.getCodePos(iniPop[i][j], codeNum, codes);//获取code 在codes中的位置
                nJs[pos]++;
            }
            for(j = 0; j < codeNum; j++)
            {
                Assert.assertEquals(nJs[j], codeCount[j]);
            }
        }
    }
}