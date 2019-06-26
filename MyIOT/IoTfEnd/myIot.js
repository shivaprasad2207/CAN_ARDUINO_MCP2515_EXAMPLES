
//var resourcePath = 'http://10.0.0.7/iot';

var resourcePath = 'http://127.0.0.1:5000/iot';

function setLed(ledId){
	ledOnId = 'led_'+ledId+'_on';
	ledOffId = 'led_'+ledId+'_off';
	var clickOn = document.getElementById(ledOnId).checked ;
	var clickOff = document.getElementById(ledOffId).checked ;
	action = null;
	if (clickOn){
		action = 'ON';
	}
	if (clickOff){
		action = 'OFF';
	}
	if (action){
		var myData = {"action":action};
		$.ajax({
			type: 'PUT',
			url: resourcePath+'/led/'+ledId,
			contentType: 'application/json',
			data: JSON.stringify(myData), // access in body
		}).done(function (data) {
			console.log(JSON.stringify(data));
			console.log('SUCCESS');
		}).fail(function (data) {
			console.log(JSON.stringify(data));
			console.log('FAIL');
		}).always(function (data) {
			console.log(data);
			console.log('ALWAYS');
		});
	}
}

function setFan(fanId){
	fanOnId = 'fan_'+fanId+'_on';
	fanOffId = 'fan_'+fanId+'_off';
	var clickOn = document.getElementById(fanOnId).checked ;
	var clickOff = document.getElementById(fanOffId).checked ;
	action = null;
	if (clickOn){
		action = 'ON';
	}
	if (clickOff){
		action = 'OFF';
	}
	if (action){
		var myData = {"action":action};
		$.ajax({
			type: 'PUT',
			url: resourcePath+'/fan/'+fanId,
			contentType: 'application/json',
			data: JSON.stringify(myData), // access in body
		}).done(function (data) {
			console.log(JSON.stringify(data));
			console.log('SUCCESS');
		}).fail(function (data) {
			console.log(JSON.stringify(data));
			console.log('FAIL');
		}).always(function (data) {
			console.log(data);
			console.log('ALWAYS');
		});
	}
}

function setMotor(motorId){
	motorOnId = 'motor_'+motorId+'_on';
	motorOffId = 'motor_'+motorId+'_off';
	var clickOn = document.getElementById(motorOnId).checked ;
	var clickOff = document.getElementById(motorOffId).checked ;
	action = null;
	if (clickOn){
		action = 'ON';
	}
	if (clickOff){
		action = 'OFF';
	}
	if (action){
		var myData = {"action":action};
		$.ajax({
			type: 'PUT',
			url: resourcePath+'/motor/'+motorId,
			contentType: 'application/json',
			data: JSON.stringify(myData), // access in body
		}).done(function (data) {
			console.log(JSON.stringify(data));
			console.log('SUCCESS');
		}).fail(function (data) {
			console.log(JSON.stringify(data));
			console.log('FAIL');
		}).always(function (data) {
			console.log(data);
			console.log('ALWAYS');
		});
	}
}