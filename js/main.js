

document.getElementById('registroUsuario').onsubmit = async function (e) {
    e.preventDefault();

    usuario = document.getElementById('usuario').value;
    contra =  document.getElementById('contra').value;
    orden = "crea";
    
    response = await fetch(`/cgi-bin/funciones.cgi`,{
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded'
        },
        body: new URLSearchParams({
            'u':usuario,
            'c':contra,
            'orden':orden
        })
    });
    data = await response.text();
    document.getElementById('resultado').innerHTML = decodeURI(data);
};


document.getElementById('eliminaUsuario').onsubmit = async function (e) {
    e.preventDefault();

    usuario = document.getElementById('usuarioElim').value;
    contra =  "";
    orden = "elimina";
    
    response = await fetch(`/cgi-bin/funciones.cgi`,{
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded'
        },
        body: new URLSearchParams({
            'u':usuario,
            'c':contra,
            'orden':orden
        })
    });
    data = await response.text();
    document.getElementById('resultadoElim').innerHTML = decodeURI(data);
};


document.getElementById('editUsuario').onsubmit = async function (e) {
    e.preventDefault();

    usuario = document.getElementById('usuarioEdit').value;
    contra =  document.getElementById('contraEdit').value;
    orden = "edita";
    
    response = await fetch(`/cgi-bin/funciones.cgi`,{
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded'
        },
        body: new URLSearchParams({
            'u':usuario,
            'c':contra,
            'orden':orden
        })
    });
    data = await response.text();
    document.getElementById('resultadoEdit').innerHTML = decodeURI(data);
};

