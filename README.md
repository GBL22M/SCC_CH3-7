# SCC_CH3-7
## UE5 CPP - Pawn 클래스 기반 캐릭터 만들기
### 1. 과제 개요
#### 필수 과제
* C++ pawn 클래스와 충돌 컴포넌트 구성
  * CapsuleComponent, SkeletalMeshComponent, SpringArm, CameraComponent 등 추가
  * Default Pawn 설정
* enhanced input 매핑 & 바인딩
  * IMC와 IA 구현
  * pawn 클래스에서 함수 바인딩
* 이동 로직
  * AddActorLocalOffset, AddActorWorldOffset, AddActorLocalRotation 등 함수를 사용하여 움직임 구현
#### 도전 과제
* 6 DOF의 드론 구현
  * 전,후/좌,우/상,하 이동
  * Yaw, Pitch, Roll 회전
* 중력 및 낙하 구현
  * Tick 함수에서 인공 중력 구현

### 2. 구현 내용
#### 필수 과제
* **C++ Pawn 클래스 구현** 
  * Pawn을 상속받아 구현하였고, 아래 사진과 같은 구조를 이루도록 구현 <br/>
   ![image](https://github.com/user-attachments/assets/eeb9076e-8e88-4c3b-931f-b06c501c892c)
   ![image](https://github.com/user-attachments/assets/4050c036-b2b9-400c-a1d9-455b2ec52d94)
* **enhanced input mapping & binding**
  * Move와 Look Input Action을 구현 후 C++에서 바인딩
  * player controller 클래스
   ![image](https://github.com/user-attachments/assets/c23ea3aa-3f2f-47fe-b260-4f3bd935ac86)
  * player character 클래스 (pawn)
    ![image](https://github.com/user-attachments/assets/3499819c-80b8-44b2-811e-1b4c2abc9d35)    
* **이동 로직**
  * AddActorLocalOffset 함수와 AddActorLocalRotation 함수 사용
  * 마우스 Y축 이동을 통한 pitch 회전에 있어서 캐릭터는 돌아가지 않도록 spring arm을 회전 시킴
    ![image](https://github.com/user-attachments/assets/38a8b802-4807-4bb7-8a46-eb7b83274f0f)

#### 도전 과제
* **6 DOF의 드론 구현**
  * 이동 구현
    * WASD로 전후좌우 이동을 구현
      * AddActorLocalOffset을 사용하여 이동 구현
      * 드론의 Tilt움직임을 구현하기 위해서, Move할때 최대 Tilt 각도를 구함
      ![image](https://github.com/user-attachments/assets/3768d07f-82c5-471a-8fd3-10af5d5aee98)
    * QE로 Roll구현
      * AddActorLocalRotation으로 구현
      ![image](https://github.com/user-attachments/assets/5eee1c6f-820a-4cd4-b136-2b2d90d5fa46)
    * Space/Shift로 Z축 이동 구현
      * Input Action의 값 타입을 Axis 1D를 사용하여 구현
      ![image](https://github.com/user-attachments/assets/4911befd-0706-4e66-8da6-d36155e0095e)
  * 회전 구현
    * 마우스를 통해 Yaw와 Pitch 이동을 할 수 있도록 구현
      ![image](https://github.com/user-attachments/assets/f34dfe57-2ba5-44d6-b6ac-29c1268d2163)
* **중력 및 낙하 구현**
  * 지면에 있지 않은 경우 중력을 Tick에서 적용<br/>
  ![image](https://github.com/user-attachments/assets/fdf7aaa5-378d-4ada-9f69-03642cfdd11b)
  * SweepSingleByChannel 함수를 통해 지면에 있는지를 체크
  ![image](https://github.com/user-attachments/assets/7b168a45-c1d1-4fbe-a661-8bc6cb660dfd)

### 3. 추가 구현 설명
* **Tilt 구현**
  * Roll이나 Yaw 움직임이 없는 경우 (only 키보드로 전후좌우 이동만 있을 경우) Tilting을 진행
  * 멈춘 경우 다시 원래자리(키보드 누르기 이전의 위치)로 이동하도록 구현
  * AddActorLocalRotation과 Lerp 사용
  ![image](https://github.com/user-attachments/assets/f0d63d8f-e13b-47ea-a6b1-099054956ca9)
* **Reset 구현**
   * 조작의 편의를 위해 Roll 회전값을 0으로 만들어주는 함수 구현 (R키를 통해 사용)
    ![image](https://github.com/user-attachments/assets/8af3613b-5c6b-49a0-bdcc-98eff686bbda)
* **충돌 구현**
  * 지면 충돌
    * Tick함수에서 IsGround 함수를 통해 체크를 하고, 지면에 있다면 Pitch 회전을 막음
    * 땅에 닿는 순간 Roll회전을 0으로 초기화
  ![image](https://github.com/user-attachments/assets/8664f1da-bafd-44ad-8b1d-90098d57ff8c)
  * static mesh와 충돌
    * Tick에서 아래의 CollisionCheck 함수를 통해 충돌했는지 확인
    * 충돌한 경우 Lerp를 통해 캐릭터를 뒤로 밀어서 Mesh 안으로 들어가지 못하게 함
    ![image](https://github.com/user-attachments/assets/6b4c13f6-5abd-4d93-8260-5048bfad99aa)
* **Niagara System**
  * Niagara를 통해 particle 효과와 캐릭터 trail 효과 구현 <br/>
   ![image](https://github.com/user-attachments/assets/4051e0bc-47d9-45fb-8250-b7c768981454)
   ![image](https://github.com/user-attachments/assets/d298164e-b5da-4817-b635-6163df8a417a)

### 4. 시연 영상
https://drive.google.com/file/d/13dGKQwgyRxK478yzeuiRclVL6T3S-cmB/view?usp=drive_link

### 5. 개선 사항
* 부족한 점
  * 현재 움직임은 Tilting 하는 순간이 WASD 키를 입력 받은 순간만 수행하며, 중간에 Roll이나 Yaw회전이 들어오면 Tilting을 해서 원래 위치로 돌아가는 것을 하지 않는다.
  * 현재 로직이 Move를 start 하는 순간 위치를 저장해두고, Move가 끝난 순간 저장해둔 위치로 돌아가도록 되어있기 때문이다.
  * **Look(마우스 회전) 이 있는 상태에서 Move(키보드)를 눌렀을 때에도 Tilting이 되도록 수정해야 한다.**
* 개선 사항
  * Input Action 관련 내용 공부 후 수정
  * Roll이나 Yaw를 한 경우 위치를 다시 저장하기
  * 가속도 사용하기
