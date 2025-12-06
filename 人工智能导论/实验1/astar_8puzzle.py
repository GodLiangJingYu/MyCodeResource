import heapq
from typing import List, Tuple, Optional
import copy

class PuzzleState:
    """
    8数码问题的状态类
    """
    def __init__(self, board: List[List[int]], g_cost: int = 0, parent=None):
        self.board = board  # 3x3的棋盘
        self.g_cost = g_cost  # 从起始节点到当前节点的实际代价
        self.parent = parent  # 父节点，用于回溯路径
        self.blank_pos = self.find_blank()  # 空格位置
    
    def find_blank(self) -> Tuple[int, int]:
        """找到空格（数字0）的位置"""
        for i in range(3):
            for j in range(3):
                if self.board[i][j] == 0:
                    return (i, j)
        return (0, 0)
    
    def get_neighbors(self) -> List['PuzzleState']:
        """获取所有可能的下一步状态"""
        neighbors = []
        x, y = self.blank_pos
        # 四个方向：上、下、左、右
        directions = [(-1, 0), (1, 0), (0, -1), (0, 1)]
        
        for dx, dy in directions:
            new_x, new_y = x + dx, y + dy
            # 检查边界
            if 0 <= new_x < 3 and 0 <= new_y < 3:
                # 交换空格和相邻数字
                new_board = copy.deepcopy(self.board)
                new_board[x][y], new_board[new_x][new_y] = new_board[new_x][new_y], new_board[x][y]
                neighbor = PuzzleState(new_board, self.g_cost + 1, self)
                neighbors.append(neighbor)
        
        return neighbors
    
    def manhattan_distance(self, goal_state: 'PuzzleState') -> int:
        """计算曼哈顿距离启发函数"""
        distance = 0
        for i in range(3):
            for j in range(3):
                if self.board[i][j] != 0:  # 不计算空格
                    value = self.board[i][j]
                    # 在目标状态中找到该值的位置
                    goal_pos = None
                    for gi in range(3):
                        for gj in range(3):
                            if goal_state.board[gi][gj] == value:
                                goal_pos = (gi, gj)
                                break
                        if goal_pos:
                            break
                    if goal_pos:
                        distance += abs(i - goal_pos[0]) + abs(j - goal_pos[1])
        return distance
    
    def misplaced_tiles(self, goal_state: 'PuzzleState') -> int:
        """计算不在位元素个数启发函数"""
        count = 0
        for i in range(3):
            for j in range(3):
                if self.board[i][j] != 0 and self.board[i][j] != goal_state.board[i][j]:
                    count += 1
        return count
    
    def is_goal(self, goal_state: 'PuzzleState') -> bool:
        """判断是否为目标状态"""
        return self.board == goal_state.board
    
    def __lt__(self, other):
        """用于优先队列的比较"""
        return self.g_cost < other.g_cost
    
    def __eq__(self, other):
        """判断两个状态是否相等"""
        return self.board == other.board
    
    def __hash__(self):
        """用于将状态放入集合中"""
        return hash(tuple(tuple(row) for row in self.board))
    
    def __str__(self):
        """打印棋盘状态"""
        result = ""
        for row in self.board:
            result += " ".join(map(str, row)) + "\n"
        return result


class AStarSolver:
    """
    A*算法求解8数码问题
    """
    def __init__(self, heuristic_type: str = "manhattan"):
        self.heuristic_type = heuristic_type  # 启发函数类型
    
    def solve(self, initial_state: PuzzleState, goal_state: PuzzleState) -> Optional[List[PuzzleState]]:
        """
        使用A*算法求解8数码问题
        """
        # open表：使用优先队列存储待访问的节点
        open_list = []
        # closed表：存储已访问的节点
        closed_set = set()
        
        # 计算初始节点的f值
        initial_h = self.calculate_heuristic(initial_state, goal_state)
        initial_f = initial_state.g_cost + initial_h
        heapq.heappush(open_list, (initial_f, id(initial_state), initial_state))
        
        nodes_expanded = 0  # 记录扩展的节点数
        
        while open_list:
            # 从open表中取出f值最小的节点
            current_f, _, current_state = heapq.heappop(open_list)
            
            # 如果当前状态是目标状态，则返回路径
            if current_state.is_goal(goal_state):
                print(f"找到解决方案！扩展了 {nodes_expanded} 个节点")
                return self.reconstruct_path(current_state)
            
            # 将当前节点加入closed表
            closed_set.add(current_state)
            nodes_expanded += 1
            
            # 扩展当前节点的所有邻居
            for neighbor in current_state.get_neighbors():
                # 如果邻居已经在closed表中，则跳过
                if neighbor in closed_set:
                    continue
                
                # 计算邻居节点的启发值和f值
                neighbor_h = self.calculate_heuristic(neighbor, goal_state)
                neighbor_f = neighbor.g_cost + neighbor_h
                
                # 检查邻居是否在open表中
                in_open_list = False
                for i, (f_val, _, state) in enumerate(open_list):
                    if state == neighbor:
                        in_open_list = True
                        # 如果新路径更优，则更新
                        if neighbor.g_cost < state.g_cost:
                            open_list[i] = (neighbor_f, id(neighbor), neighbor)
                            heapq.heapify(open_list)
                        break
                
                # 如果邻居不在open表中，则添加进去
                if not in_open_list:
                    heapq.heappush(open_list, (neighbor_f, id(neighbor), neighbor))
        
        # 没有找到解决方案
        print("未找到解决方案！")
        return None
    
    def calculate_heuristic(self, state: PuzzleState, goal_state: PuzzleState) -> int:
        """根据指定的启发函数类型计算启发值"""
        if self.heuristic_type == "manhattan":
            return state.manhattan_distance(goal_state)
        elif self.heuristic_type == "misplaced":
            return state.misplaced_tiles(goal_state)
        else:
            raise ValueError(f"未知的启发函数类型: {self.heuristic_type}")
    
    def reconstruct_path(self, final_state: PuzzleState) -> List[PuzzleState]:
        """从目标节点回溯路径到初始节点"""
        path = []
        current = final_state
        while current:
            path.append(current)
            current = current.parent
        path.reverse()
        return path


def print_solution(solution_path: List[PuzzleState], heuristic_type: str):
    """打印解决方案"""
    print(f"\n使用 {heuristic_type} 启发函数的解决方案：")
    print(f"解决方案步数: {len(solution_path) - 1}")
    print("解决方案路径:")
    
    for i, state in enumerate(solution_path):
        print(f"步骤 {i}:")
        print(state)
        print("-" * 10)


def main():
    print("A*算法求解8数码问题")
    print("=" * 30)
    
    # 定义初始状态和目标状态
    initial_board = [
        [2, 8, 3],
        [1, 6, 4],
        [7, 0, 5]
    ]
    
    goal_board = [
        [1, 2, 3],
        [8, 0, 4],
        [7, 6, 5]
    ]
    
    initial_state = PuzzleState(initial_board)
    goal_state = PuzzleState(goal_board)
    
    print("初始状态:")
    print(initial_state)
    
    print("目标状态:")
    print(goal_state)
    
    # 使用曼哈顿距离启发函数
    print("\n正在使用曼哈顿距离启发函数求解...")
    solver_manhattan = AStarSolver("manhattan")
    solution_manhattan = solver_manhattan.solve(initial_state, goal_state)
    
    if solution_manhattan:
        print_solution(solution_manhattan, "曼哈顿距离")
    else:
        print("使用曼哈顿距离启发函数未找到解决方案")
    
    # 使用不在位元素个数启发函数
    print("\n" + "=" * 50)
    print("正在使用不在位元素个数启发函数求解...")
    solver_misplaced = AStarSolver("misplaced")
    solution_misplaced = solver_misplaced.solve(initial_state, goal_state)
    
    if solution_misplaced:
        print_solution(solution_misplaced, "不在位元素个数")
    else:
        print("使用不在位元素个数启发函数未找到解决方案")


if __name__ == "__main__":
    main()
