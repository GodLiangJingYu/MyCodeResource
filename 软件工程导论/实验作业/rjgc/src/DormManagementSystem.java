import com.google.gson.*;
import com.google.gson.reflect.TypeToken;

import javax.swing.*;
import java.awt.*;
import java.io.*;
import java.lang.reflect.Type;
import java.util.ArrayList;
import java.util.List;

// 学生类
class Student {
    private String id;
    private String name;
    private String gender;

    public Student(String id, String name, String gender) {
        this.id = id;
        this.name = name;
        this.gender = gender;
    }

    public String getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public String getGender() {
        return gender;
    }
}

// 宿舍类
class Room {
    private String roomNumber;
    private String type;
    private int capacity;
    private int occupied;

    public Room(String roomNumber, String type, int capacity) {
        this.roomNumber = roomNumber;
        this.type = type;
        this.capacity = capacity;
        this.occupied = 0;
    }

    public String getRoomNumber() {
        return roomNumber;
    }

    public String getType() {
        return type;
    }

    public int getCapacity() {
        return capacity;
    }

    public int getOccupied() {
        return occupied;
    }

    public void checkIn() {
        if (occupied < capacity) {
            occupied++;
        }
    }

    public void checkOut() {
        if (occupied > 0) {
            occupied--;
        }
    }
}

// 数据管理类
class DataManager {
    private static final String DATA_FILE = "dorm_data.json";
    private List<Student> students;
    private List<Room> rooms;

    public DataManager() {
        loadData();
    }

    public List<Student> getStudents() {
        return students;
    }

    public List<Room> getRooms() {
        return rooms;
    }

    public void addStudent(Student student) {
        students.add(student);
        saveData();
    }

    public void addRoom(Room room) {
        rooms.add(room);
        saveData();
    }

    public void saveData() {
        try (Writer writer = new FileWriter(DATA_FILE)) {
            Gson gson = new GsonBuilder().setPrettyPrinting().create();
            JsonObject jsonObject = new JsonObject();
            jsonObject.add("students", gson.toJsonTree(students));
            jsonObject.add("rooms", gson.toJsonTree(rooms));
            gson.toJson(jsonObject, writer);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void loadData() {
        try (Reader reader = new FileReader(DATA_FILE)) {
            Gson gson = new Gson();
            JsonObject jsonObject = gson.fromJson(reader, JsonObject.class);
            Type studentListType = new TypeToken<List<Student>>() {}.getType();
            Type roomListType = new TypeToken<List<Room>>() {}.getType();
            students = gson.fromJson(jsonObject.get("students"), studentListType);
            rooms = gson.fromJson(jsonObject.get("rooms"), roomListType);
        } catch (FileNotFoundException e) {
            // 初次运行时文件不存在，初始化空列表
            students = new ArrayList<>();
            rooms = new ArrayList<>();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

// 主程序
public class DormManagementSystem {
    private DataManager dataManager;

    public DormManagementSystem() {
        dataManager = new DataManager();
    }

    public void startUI() {
        JFrame frame = new JFrame("学生宿舍管理系统");
        frame.setSize(600, 400);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        JPanel panel = new JPanel();
        panel.setLayout(new GridLayout(6, 1));

        // 添加学生按钮
        JButton addStudentBtn = new JButton("添加学生");
        addStudentBtn.addActionListener(e -> {
            String id = JOptionPane.showInputDialog("学生 ID:");
            String name = JOptionPane.showInputDialog("学生姓名:");
            String gender = JOptionPane.showInputDialog("学生性别:");
            dataManager.addStudent(new Student(id, name, gender));
            JOptionPane.showMessageDialog(frame, "学生添加成功！");
        });
        panel.add(addStudentBtn);

        // 添加宿舍按钮
        JButton addRoomBtn = new JButton("添加宿舍");
        addRoomBtn.addActionListener(e -> {
            String roomNumber = JOptionPane.showInputDialog("宿舍号:");
            String type = JOptionPane.showInputDialog("宿舍类型:");
            int capacity = Integer.parseInt(JOptionPane.showInputDialog("宿舍容量:"));
            dataManager.addRoom(new Room(roomNumber, type, capacity));
            JOptionPane.showMessageDialog(frame, "宿舍添加成功！");
        });
        panel.add(addRoomBtn);

        // 查看学生按钮
        JButton viewStudentsBtn = new JButton("查看所有学生");
        viewStudentsBtn.addActionListener(e -> {
            List<Student> students = dataManager.getStudents();
            StringBuilder message = new StringBuilder("学生列表:\n");
            for (Student student : students) {
                message.append(String.format("ID: %s, 姓名: %s, 性别: %s\n", student.getId(), student.getName(), student.getGender()));
            }
            JOptionPane.showMessageDialog(frame, message.toString());
        });
        panel.add(viewStudentsBtn);

        // 查看宿舍按钮
        JButton viewRoomsBtn = new JButton("查看所有宿舍");
        viewRoomsBtn.addActionListener(e -> {
            List<Room> rooms = dataManager.getRooms();
            StringBuilder message = new StringBuilder("宿舍列表:\n");
            for (Room room : rooms) {
                message.append(String.format("宿舍号: %s, 类型: %s, 容量: %d, 已入住: %d\n",
                        room.getRoomNumber(), room.getType(), room.getCapacity(), room.getOccupied()));
            }
            JOptionPane.showMessageDialog(frame, message.toString());
        });
        panel.add(viewRoomsBtn);

        // 退出系统按钮
        JButton exitBtn = new JButton("退出系统");
        exitBtn.addActionListener(e -> System.exit(0));
        panel.add(exitBtn);

        frame.add(panel);
        frame.setVisible(true);
    }

    public static void main(String[] args) {
        DormManagementSystem system = new DormManagementSystem();
        system.startUI();
    }
}