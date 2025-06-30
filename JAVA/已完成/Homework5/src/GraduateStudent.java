import java.util.Date;

public class GraduateStudent extends StudentInfo{
    private String supervisor; // 导师

    public GraduateStudent(String studentNumber, String studentName, String gander, Date birthday, String academy, String major, String supervisor) {
        super(studentNumber, studentName, gander, birthday, academy, major);
        this.supervisor = supervisor;
    }

    @Override
    public String toString() {
        return String.format("{\"academy\":\"%s\",\"birthday\":\"%s\",\"gander\":\"%s\",\"major\":\"%s\",\"studentName\":\"%s\",\"studentNumber\":\"%s\",\"supervisor\":\"%s\"}",
                academy, birthday, gander, major, studentName, studentNumber, supervisor);
    }
}
