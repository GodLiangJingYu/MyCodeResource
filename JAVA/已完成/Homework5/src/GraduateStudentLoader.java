import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONArray;
import com.alibaba.fastjson.JSONObject;
import com.alibaba.fastjson.JSONException;

import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.charset.StandardCharsets;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class GraduateStudentLoader {

    public List<GraduateStudent> loadGraduateStudents(String filePath) {
        List<GraduateStudent> graduateStudents = new ArrayList<>();
        JSONArray jsonArray = null;

        try {
            System.out.println("Reading JSON file from: " + filePath);
            String content = new String(Files.readAllBytes(Paths.get(filePath)), StandardCharsets.UTF_8);
            jsonArray = JSON.parseArray(content); // Parse JSON content
            System.out.println("Successfully parsed JSON.");
        } catch (IOException e) {
            System.out.println("Error reading JSON file: " + e.getMessage());
            return graduateStudents; // Return empty list on error
        } catch (JSONException e) {
            System.out.println("Error parsing JSON: " + e.getMessage());
            return graduateStudents; // Return empty list if parsing fails
        }

        // Check if jsonArray is null before processing
        if (jsonArray == null) {
            System.out.println("Parsed JSONArray is null.");
            return graduateStudents; // Return empty list if parsing failed
        }

        // Process the jsonArray
        for (Object obj : jsonArray) {
            JSONObject jsonObject = (JSONObject) obj;
            GraduateStudent student = jsonObject.toJavaObject(GraduateStudent.class);
            graduateStudents.add(student);
        }

        return graduateStudents;
    }
}
