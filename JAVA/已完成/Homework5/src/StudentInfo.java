import java.util.Date;

// 抽象类
public abstract class StudentInfo {
    protected String studentNumber; // 学号
    protected String studentName; // 姓名
    protected String gander; // 性别
    protected Date birthday; // 生日
    protected String academy; // 学院
    protected String major; // 专业

    // 构造函数
    public StudentInfo(String studentNumber, String studentName, String gander, Date birthday, String academy, String major) {
        this.studentNumber = studentNumber;
        this.studentName = studentName;
        this.gander = gander;
        this.birthday = birthday;
        this.academy = academy;
        this.major = major;
    }

    public String getStudentNumber() {
        return studentNumber;
    }

    public String getName() {
        return studentName;
    }

    public String getGander() {
        return gander; // 可选，提供性别获取方法
    }

    public abstract String toString(); // 子类实现具体的打印格式
}
