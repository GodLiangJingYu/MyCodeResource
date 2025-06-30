import org.dom4j.Document;
import org.dom4j.Element;
import org.dom4j.io.SAXReader;

import java.io.File;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class DoctoralStudentLoader {

    public List<DoctoralStudent> loadDoctoralStudents(String filename) throws Exception {
        List<DoctoralStudent> students = new ArrayList<>();
        SAXReader reader = new SAXReader();
        Document document = reader.read(new File(filename));
        Element root = document.getRootElement();

        // 使用正确的日期格式
        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy年MM月dd日");

        for (Element studentElement : root.elements("studentInfo")) {
            String studentNumber = studentElement.elementText("studentNumber");
            String studentName = studentElement.elementText("studentName");
            String gander = studentElement.elementText("gander");
            Date birthday = dateFormat.parse(studentElement.elementText("birthday"));
            String academy = studentElement.elementText("academy");
            String major = studentElement.elementText("major");
            String supervisor = studentElement.elementText("supervisor");
            String researchFields = studentElement.elementText("researchFields");

            students.add(new DoctoralStudent(studentNumber, studentName, gander, birthday, academy, major, supervisor, researchFields));
        }
        return students;
    }
}
