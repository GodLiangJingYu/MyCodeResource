import java.util.Date;

public class DoctoralStudent extends StudentInfo{
    private String supervisor; // 导师
    private String researchFields; // 研究方向

    public DoctoralStudent(String studentNumber, String studentName, String gander, Date birthday, String academy, String major, String supervisor, String researchFields) {
        super(studentNumber, studentName, gander, birthday, academy, major);
        this.supervisor = supervisor;
        this.researchFields = researchFields;
    }

    @Override
    public String toString() {
        return String.format("<studentInfo><studentNumber>%s</studentNumber><studentName>%s</studentName><gander>%s</gander><birthday>%s</birthday><academy>%s</academy><major>%s</major><supervisor>%s</supervisor><researchFields>%s</researchFields></studentInfo>",
                studentNumber, studentName, gander, birthday, academy, major, supervisor, researchFields);
    }
}
