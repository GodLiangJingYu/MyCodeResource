import java.io.BufferedReader;
import java.io.FileReader;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class StudentLoader {

    public List<UndergraduateStudent> loadUndergraduateStudents(String filename) throws Exception {
        List<UndergraduateStudent> students = new ArrayList<>();
        try (BufferedReader br = new BufferedReader(new FileReader(filename))) {
            String line;
            SimpleDateFormat sdf = new SimpleDateFormat("yyyy年MM月dd日");
            while ((line = br.readLine()) != null) {
                String[] parts = line.split("_");
                if (parts.length == 7) {
                    String studentNumber = parts[0];
                    String studentName = parts[1];
                    String gander = parts[2];
                    Date birthday = sdf.parse(parts[3]);
                    String academy = parts[4];
                    String major = parts[5];
                    String tutor = parts[6];
                    students.add(new UndergraduateStudent(studentNumber, studentName, gander, birthday, academy, major, tutor));
                }
            }
        }
        return students;
    }
}
