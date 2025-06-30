import java.util.Date;

public class UndergraduateStudent extends StudentInfo{
    private String tutor; // 辅导员

    public UndergraduateStudent(String studentNumber, String studentName, String gander, Date birthday, String academy, String major, String tutor) {
        super(studentNumber, studentName, gander, birthday, academy, major);
        this.tutor = tutor;
    }

    @Override
    public String toString() {
        return String.join("_", studentNumber, studentName, gander, birthday.toString(), academy, major, tutor);
    }
}
