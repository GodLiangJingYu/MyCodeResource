import java.text.SimpleDateFormat;
import java.util.*;

public class StudentManagementSystem {
    private List<UndergraduateStudent> undergraduates = new ArrayList<>();
    private List<GraduateStudent> graduates = new ArrayList<>();
    private List<DoctoralStudent> doctoralStudents = new ArrayList<>();

    public void loadStudents() {
        try {
            StudentLoader sl = new StudentLoader();
            // 确保路径正确
            undergraduates = sl.loadUndergraduateStudents("us.txt");

            GraduateStudentLoader gsl = new GraduateStudentLoader();
            graduates = gsl.loadGraduateStudents("gs.json");

            DoctoralStudentLoader dsl = new DoctoralStudentLoader();
            doctoralStudents = dsl.loadDoctoralStudents("ds.xml");
        } catch (Exception e) {
            System.out.println("加载学生失败: " + e.getMessage());
            e.printStackTrace();
        }
    }


    public void showMenu() {
        Scanner scanner = new Scanner(System.in);
        while (true) {
            System.out.println("[A] 打印全部学生信息");
            System.out.println("[B] 新增一个本科生");
            System.out.println("[C] 新增一个研究生");
            System.out.println("[D] 新增一个博士生");
            System.out.println("[E] 按学号查找学生");
            System.out.println("[F] 按姓名查找学生");
            System.out.println("[G] 学生排序");
            System.out.println("[Q] 退出");
            String choice = scanner.nextLine();
            switch (choice.toUpperCase()) {
                case "A":
                    printAllStudents();
                    break;
                case "B":
                    addUndergraduateStudent();
                    break;
                case "C":
                    addGraduateStudent();
                    break;
                case "D":
                    addDoctoralStudent();
                    break;
                case "E":
                    System.out.print("请输入学号: ");
                    String studentNumber = scanner.nextLine();
                    findStudentByNumber(studentNumber);
                    break;
                case "F":
                    System.out.print("请输入姓名: ");
                    String studentName = scanner.nextLine();
                    findStudentByName(studentName);
                    break;
                case "G":
                    sortStudents();
                    break;
                case "Q":
                    System.out.println("退出系统");
                    return;
                default:
                    System.out.println("无效选择，请重试");
            }
        }
    }

    public void printAllStudents() {
        // 打印本科生
        for (UndergraduateStudent student : undergraduates) {
            System.out.println(student);
        }
        // 打印研究生
        for (GraduateStudent student : graduates) {
            System.out.println(student);
        }
        // 打印博士生
        for (DoctoralStudent student : doctoralStudents) {
            System.out.println(student);
        }
    }

    public void addUndergraduateStudent() {
        Scanner scanner = new Scanner(System.in);
        System.out.print("输入学号: ");
        String studentNumber = scanner.nextLine();
        System.out.print("输入姓名: ");
        String studentName = scanner.nextLine();
        System.out.print("输入性别: ");
        String gander = scanner.nextLine();
        System.out.print("输入生日 (格式: yyyy-MM-dd): ");
        String birthdayStr = scanner.nextLine();
        Date birthday = null;
        try {
            birthday = new SimpleDateFormat("yyyy-MM-dd").parse(birthdayStr);
        } catch (Exception e) {
            System.out.println("日期格式错误，新增本科生失败。");
            return;
        }
        System.out.print("输入学院: ");
        String academy = scanner.nextLine();
        System.out.print("输入专业: ");
        String major = scanner.nextLine();
        System.out.print("输入辅导员");
        String tutor = scanner.nextLine();
        // 创建新的本科生对象，并添加到列表中
        undergraduates.add(new UndergraduateStudent(studentNumber, studentName, gander, birthday, academy, major,tutor));
        System.out.println("新增本科生成功！");
    }


    public void addGraduateStudent() {
        Scanner scanner = new Scanner(System.in);
        System.out.print("输入学号: ");
        String studentNumber = scanner.nextLine();
        System.out.print("输入姓名: ");
        String studentName = scanner.nextLine();
        System.out.print("输入性别: ");
        String gander = scanner.nextLine();
        System.out.print("输入生日 (格式: yyyy-MM-dd): ");
        String birthdayStr = scanner.nextLine();
        Date birthday = null;

        // 处理生日字符串转换为 Date
        try {
            birthday = new SimpleDateFormat("yyyy-MM-dd").parse(birthdayStr);
        } catch (Exception e) {
            System.out.println("日期格式错误，新增研究生失败。");
            return;
        }

        System.out.print("输入学院: ");
        String academy = scanner.nextLine();
        System.out.print("输入专业: ");
        String major = scanner.nextLine();
        System.out.print("输入导师: ");
        String supervisor = scanner.nextLine();

        // 创建新的研究生对象，并添加到列表中
        graduates.add(new GraduateStudent(studentNumber, studentName, gander, birthday, academy, major, supervisor));
        System.out.println("新增研究生成功！");
    }


    public void addDoctoralStudent() {
        Scanner scanner = new Scanner(System.in);
        System.out.print("输入学号: ");
        String studentNumber = scanner.nextLine();
        System.out.print("输入姓名: ");
        String studentName = scanner.nextLine();
        System.out.print("输入性别: ");
        String gander = scanner.nextLine();
        System.out.print("输入生日 (格式: yyyy-MM-dd): ");
        String birthdayStr = scanner.nextLine();
        Date birthday = null;

        // 处理生日字符串转换为 Date
        try {
            birthday = new SimpleDateFormat("yyyy年MM月dd日").parse(birthdayStr);
        } catch (Exception e) {
            System.out.println("日期格式错误，新增博士生失败。");
            return;
        }

        System.out.print("输入学院: ");
        String academy = scanner.nextLine();
        System.out.print("输入专业: ");
        String major = scanner.nextLine();
        System.out.print("输入导师: ");
        String supervisor = scanner.nextLine();
        System.out.print("输入研究方向: ");
        String researchFields = scanner.nextLine();

        // 创建新的博士生对象，并添加到列表中
        doctoralStudents.add(new DoctoralStudent(studentNumber, studentName, gander, birthday, academy, major, supervisor, researchFields));
        System.out.println("新增博士生成功！");
    }


    public void findStudentByNumber(String studentNumber) {
        for (UndergraduateStudent student : undergraduates) {
            if (student.getStudentNumber().equals(studentNumber)) {
                System.out.println(student);
                return;
            }
        }
        for (GraduateStudent student : graduates) {
            if (student.getStudentNumber().equals(studentNumber)) {
                System.out.println(student);
                return;
            }
        }
        for (DoctoralStudent student : doctoralStudents) {
            if (student.getStudentNumber().equals(studentNumber)) {
                System.out.println(student);
                return;
            }
        }
        System.out.println("未找到该学号的学生。");
    }

    public void findStudentByName(String studentName) {
        for (UndergraduateStudent student : undergraduates) {
            if (student.getName().equals(studentName)) {
                System.out.println(student);
                return;
            }
        }
        for (GraduateStudent student : graduates) {
            if (student.getName().equals(studentName)) {
                System.out.println(student);
                return;
            }
        }
        for (DoctoralStudent student : doctoralStudents) {
            if (student.getName().equals(studentName)) {
                System.out.println(student);
                return;
            }
        }
        System.out.println("未找到该姓名的学生。");
    }

    public void sortStudents() {
        Comparator<StudentInfo> byNumber = Comparator.comparing(StudentInfo::getStudentNumber);

        Collections.sort(undergraduates, byNumber);
        Collections.sort(graduates, byNumber);
        Collections.sort(doctoralStudents, byNumber);

        System.out.println("学生排序成功！");

        System.out.println("排序后的本科生：");
        for (UndergraduateStudent student : undergraduates) {
            System.out.println(student);
        }

        System.out.println("排序后的研究生：");
        for (GraduateStudent student : graduates) {
            System.out.println(student);
        }

        System.out.println("排序后的博士生：");
        for (DoctoralStudent student : doctoralStudents) {
            System.out.println(student);
        }
    }




    public static void main(String[] args) throws Exception {
        StudentManagementSystem sms = new StudentManagementSystem();
        sms.loadStudents();
        sms.showMenu();
    }
}
